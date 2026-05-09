#!/usr/bin/env python3
"""
Patch a fresh Quest Engine save with a BaneMode-friendly recovery bundle.

This is intentionally conservative:
- preserve a Quest save shell that the emulator already accepts
- patch money / coins / a couple of safe convenience fields
- leave the rest of the save structure intact

Why conservative?
- We already know broad save-structure edits can make emulator acceptance flaky.
- Fresh new games in the Quest build already receive the big BaneMode item bundle.
- This tool is meant to make a restart less painful, not to re-implement the whole
  old-save migration in one jump.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025
NUM_MAIN_SECTORS = 14
NUM_TOTAL_SECTORS = 32
SLOT_BASES = (0, 14)

# SaveBlock1 offsets from Quest build include/global.h
SB1_SIZE = 15696
SB1_MONEY = 0x490
SB1_COINS = 0x494
SB1_REGISTERED_ITEM_COMPAT = 0x496
SB1_REGISTERED_ITEMS = 0xA26

# Safe convenience values
ITEM_NONE = 0
ITEM_POKEMON_BOX_LINK = 717
MAX_MONEY = 999999
MAX_COINS = 9999
MAX_REGISTERED_ITEMS = 4


def footer(buf: bytes, sector_index: int):
    base = sector_index * SECTOR_SIZE
    sector_id = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 116)[0]
    sector_checksum = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 118)[0]
    signature = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 120)[0]
    counter = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 124)[0]
    return sector_id, sector_checksum, signature, counter


def checksum(data: bytes) -> int:
    total = 0
    usable = (len(data) // 4) * 4
    for offset in range(0, usable, 4):
        total = (total + struct.unpack_from("<I", data, offset)[0]) & 0xFFFFFFFF
    return ((total >> 16) + (total & 0xFFFF)) & 0xFFFF


def pick_latest_slot(buf: bytes) -> int:
    best = None
    for slot_base in SLOT_BASES:
        ids = []
        counters = []
        for i in range(NUM_MAIN_SECTORS):
            sid, _, sig, counter = footer(buf, slot_base + i)
            if sig == SECTOR_SIGNATURE and sid < NUM_MAIN_SECTORS:
                ids.append(sid)
                counters.append(counter)
        if set(ids) >= set(range(NUM_MAIN_SECTORS)):
            slot_counter = max(counters) if counters else -1
            if best is None or slot_counter > best[1]:
                best = (slot_base, slot_counter)
    if best is None:
        raise SystemExit("No valid full save slot found")
    return best[0]


def map_slot_by_sector_id(buf: bytes, slot_base: int):
    out = {}
    for i in range(NUM_MAIN_SECTORS):
        sector_index = slot_base + i
        sid, _, sig, _ = footer(buf, sector_index)
        if sig == SECTOR_SIGNATURE and sid < NUM_MAIN_SECTORS:
            out[sid] = sector_index
    missing = [sid for sid in range(NUM_MAIN_SECTORS) if sid not in out]
    if missing:
        raise SystemExit(f"Slot {slot_base} missing sectors {missing}")
    return out


def read_slot_blob(buf: bytes, sector_map: dict[int, int], first_sid: int, last_sid: int) -> bytes:
    blob = bytearray()
    for sid in range(first_sid, last_sid + 1):
        sector_index = sector_map[sid]
        base = sector_index * SECTOR_SIZE
        blob.extend(buf[base:base + SECTOR_DATA_SIZE])
    return bytes(blob)


def write_slot_blob(dst: bytearray, target_slot_base: int, first_sid: int, last_sid: int, blob: bytes, counter: int):
    for chunk_index, sid in enumerate(range(first_sid, last_sid + 1)):
        dst_sector_index = target_slot_base + sid
        dst_base = dst_sector_index * SECTOR_SIZE
        chunk = blob[chunk_index * SECTOR_DATA_SIZE:(chunk_index + 1) * SECTOR_DATA_SIZE]
        if len(chunk) < SECTOR_DATA_SIZE:
            chunk = chunk + b"\x00" * (SECTOR_DATA_SIZE - len(chunk))
        dst[dst_base:dst_base + SECTOR_DATA_SIZE] = chunk
        struct.pack_into("<H", dst, dst_base + SECTOR_DATA_SIZE + 116, sid)
        struct.pack_into("<H", dst, dst_base + SECTOR_DATA_SIZE + 118, checksum(chunk))
        struct.pack_into("<I", dst, dst_base + SECTOR_DATA_SIZE + 120, SECTOR_SIGNATURE)
        struct.pack_into("<I", dst, dst_base + SECTOR_DATA_SIZE + 124, counter)


def patch_saveblock1(sb1: bytes) -> bytes:
    if len(sb1) < SB1_SIZE:
        raise ValueError(f"SaveBlock1 too small: {len(sb1)} < {SB1_SIZE}")

    out = bytearray(sb1[:SB1_SIZE])
    struct.pack_into("<I", out, SB1_MONEY, MAX_MONEY)
    struct.pack_into("<H", out, SB1_COINS, MAX_COINS)
    struct.pack_into("<H", out, SB1_REGISTERED_ITEM_COMPAT, ITEM_POKEMON_BOX_LINK)

    # Make the first registered item slot mirror the compat item and clear the rest.
    for i in range(MAX_REGISTERED_ITEMS):
        item = ITEM_POKEMON_BOX_LINK if i == 0 else ITEM_NONE
        struct.pack_into("<H", out, SB1_REGISTERED_ITEMS + i * 2, item)

    return bytes(out)


def patch(save_path: Path, output_path: Path):
    raw = bytearray(save_path.read_bytes())
    if len(raw) != SECTOR_SIZE * NUM_TOTAL_SECTORS:
        raise SystemExit("Expected a 128 KiB Emerald-format save")

    slot = pick_latest_slot(raw)
    sector_map = map_slot_by_sector_id(raw, slot)
    sb2_blob = read_slot_blob(raw, sector_map, 0, 0)
    sb1_blob = read_slot_blob(raw, sector_map, 1, 4)
    storage_blob = read_slot_blob(raw, sector_map, 5, 13)

    new_sb1 = patch_saveblock1(sb1_blob[:SB1_SIZE])

    _, _, _, old_counter = footer(raw, slot)
    new_counter = old_counter + 1

    sb1_write = new_sb1 + b"\x00" * (SECTOR_DATA_SIZE * 4 - len(new_sb1))

    for target_slot in SLOT_BASES:
        write_slot_blob(raw, target_slot, 0, 0, sb2_blob, new_counter)
        write_slot_blob(raw, target_slot, 1, 4, sb1_write, new_counter)
        write_slot_blob(raw, target_slot, 5, 13, storage_blob, new_counter)

    output_path.write_bytes(raw)


def main():
    parser = argparse.ArgumentParser(description="Patch a fresh Quest save with BaneMode recovery values.")
    parser.add_argument("save", type=Path, help="Input Quest save")
    parser.add_argument("output", type=Path, help="Output patched save")
    args = parser.parse_args()

    patch(args.save, args.output)
    print(args.output)


if __name__ == "__main__":
    main()
