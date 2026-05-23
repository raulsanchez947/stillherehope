#!/usr/bin/env python3
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

SAVE_BLOCK2_SIZE = 0xF2C
SB1_SIZE = SECTOR_DATA_SIZE * 4
CONTINUE_GAME_WARP = 1 << 0

SB1_POS = 0x00
SB1_LOCATION = 0x04
SB1_CONTINUE_GAME_WARP = 0x0C
SB1_DYNAMIC_WARP = 0x14
SB1_MAP_LAYOUT_ID = 0x32
SB1_OBJECT_EVENT_TEMPLATES = 0x0C70
OBJECT_EVENT_TEMPLATES_SIZE = 64 * 0x18

MAP_GROUP_RUSTBORO_CITY = 0
MAP_NUM_RUSTBORO_CITY = 3
LAYOUT_RUSTBORO_CITY = 4
WARP_ID_NONE = -1
RESCUE_X = 27
RESCUE_Y = 20


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


def slot_counter(buf: bytes, slot_base: int) -> int:
    counters = []
    for i in range(NUM_MAIN_SECTORS):
        sid, _, sig, counter = footer(buf, slot_base + i)
        if sig == SECTOR_SIGNATURE and sid < NUM_MAIN_SECTORS:
            counters.append(counter)
    return max(counters) if counters else -1


def pick_latest_slot(buf: bytes) -> int:
    best_slot = SLOT_BASES[0]
    best_counter = -1
    for slot_base in SLOT_BASES:
        counter = slot_counter(buf, slot_base)
        if counter > best_counter:
            best_counter = counter
            best_slot = slot_base
    return best_slot


def raw_sector_map(buf: bytes, slot_base: int) -> dict[int, int]:
    out: dict[int, int] = {}
    for i in range(NUM_MAIN_SECTORS):
        sector_index = slot_base + i
        sid, _, sig, _ = footer(buf, sector_index)
        if sig == SECTOR_SIGNATURE and sid < NUM_MAIN_SECTORS:
            out[sid] = sector_index
    missing = [sid for sid in range(NUM_MAIN_SECTORS) if sid not in out]
    if missing:
        raise SystemExit(f"Slot {slot_base} missing sector ids {missing}")
    return out


def read_slot_blob(buf: bytes, sector_map: dict[int, int], first_sid: int, last_sid: int) -> bytes:
    blob = bytearray()
    for sid in range(first_sid, last_sid + 1):
        sector_index = sector_map[sid]
        base = sector_index * SECTOR_SIZE
        blob.extend(buf[base:base + SECTOR_DATA_SIZE])
    return bytes(blob)


def write_warp(data: bytearray, offset: int):
    struct.pack_into(
        "<bbbBhh",
        data,
        offset,
        MAP_GROUP_RUSTBORO_CITY,
        MAP_NUM_RUSTBORO_CITY,
        WARP_ID_NONE,
        0,
        RESCUE_X,
        RESCUE_Y,
    )


def patch_sb2(sb2: bytes) -> bytes:
    out = bytearray(sb2[:SECTOR_DATA_SIZE])
    out[0x09] |= CONTINUE_GAME_WARP
    return bytes(out)


def patch_sb1(sb1: bytes) -> bytes:
    out = bytearray(sb1[:SB1_SIZE])
    struct.pack_into("<hh", out, SB1_POS, RESCUE_X, RESCUE_Y)
    write_warp(out, SB1_LOCATION)
    write_warp(out, SB1_CONTINUE_GAME_WARP)
    write_warp(out, SB1_DYNAMIC_WARP)
    struct.pack_into("<H", out, SB1_MAP_LAYOUT_ID, LAYOUT_RUSTBORO_CITY)
    out[SB1_OBJECT_EVENT_TEMPLATES:SB1_OBJECT_EVENT_TEMPLATES + OBJECT_EVENT_TEMPLATES_SIZE] = b"\x00" * OBJECT_EVENT_TEMPLATES_SIZE
    return bytes(out)


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


def rebuild_save(input_path: Path, output_path: Path):
    raw = bytearray(input_path.read_bytes())
    if len(raw) != SECTOR_SIZE * NUM_TOTAL_SECTORS:
        raise SystemExit("Expected a 128 KiB Emerald-format save")

    slot = pick_latest_slot(raw)
    sector_map = raw_sector_map(raw, slot)

    sb2_blob = read_slot_blob(raw, sector_map, 0, 0)
    sb1_blob = read_slot_blob(raw, sector_map, 1, 4)
    storage_blob = read_slot_blob(raw, sector_map, 5, 13)

    new_sb2 = patch_sb2(sb2_blob)
    new_sb1 = patch_sb1(sb1_blob)

    old_counter = slot_counter(raw, slot)
    new_counter = old_counter + 1 if old_counter >= 0 else 1

    sb1_write = new_sb1 + b"\x00" * (SECTOR_DATA_SIZE * 4 - len(new_sb1))

    for target_slot in SLOT_BASES:
        write_slot_blob(raw, target_slot, 0, 0, new_sb2, new_counter)
        write_slot_blob(raw, target_slot, 1, 4, sb1_write, new_counter)
        write_slot_blob(raw, target_slot, 5, 13, storage_blob, new_counter)

    output_path.write_bytes(raw)


def main():
    parser = argparse.ArgumentParser(description="Rebuild a Quest save cleanly and reset Rustboro Gym/Juan state.")
    parser.add_argument("input_save", type=Path)
    parser.add_argument("output_save", type=Path)
    args = parser.parse_args()

    rebuild_save(args.input_save, args.output_save)
    print(args.output_save)


if __name__ == "__main__":
    main()
