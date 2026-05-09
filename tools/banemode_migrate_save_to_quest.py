#!/usr/bin/env python3
"""
Migrate an old BaneMode Emerald save into the Quest Engine migration shell.

This script intentionally uses a fresh Quest Engine save as the destination
shell, then transplants the old progress into it while reshaping SaveBlock1's
bag data and minimally converting SaveBlock2.

Why shell-based?
- The physical sector ordering / counters / signatures remain emulator-safe.
- We only rewrite the logical save content we actually understand.
- New Quest Engine-only pockets default to empty rather than inheriting garbage.
"""

from __future__ import annotations

import argparse
import shutil
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025
NUM_MAIN_SECTORS = 14
NUM_TOTAL_SECTORS = 32
SLOT_BASES = (0, 14)
SPECIAL_SECTOR_IDS = (28, 29, 30, 31)


# Old BaneMode save layout (measured from the old working repo).
OLD_SB2_SIZE = 3848
OLD_SB1_SIZE = 16032
OLD_PC_ITEMS_COUNT = 50
OLD_BAG_ITEMS_COUNT = 96
OLD_BAG_KEYITEMS_COUNT = 30
OLD_BAG_POKEBALLS_COUNT = 16
OLD_BAG_TMHM_COUNT = 64
OLD_BAG_BERRIES_COUNT = 46

OLD_SB2_PLAYER_NAME = 0
OLD_SB2_PLAYER_GENDER = 8
OLD_SB2_SPECIAL_WARP_FLAGS = 9
OLD_SB2_TRAINER_ID = 10
OLD_SB2_PLAY_TIME = 14  # hours(2), mins, secs, vblanks
OLD_SB2_OPTIONS_BUTTON = 19
OLD_SB2_POKEDEX = 24
OLD_SB2_LOCAL_TIME = 152
OLD_SB2_GCN_LINK_FLAGS = 164
OLD_SB2_ENCRYPTION_KEY = 168

OLD_SB1_PC_ITEMS = 1176
OLD_SB1_BAG = 1376
OLD_SB1_POKEBLOCKS = 2384


# New Quest Engine save layout (measured from the migration repo).
NEW_SB2_SIZE = 3340
NEW_SB1_SIZE = 15696
NEW_PC_ITEMS_COUNT = 99
NEW_BAG_ITEMS_COUNT = 60
NEW_BAG_KEYITEMS_COUNT = 50
NEW_BAG_POKEBALLS_COUNT = 16
NEW_BAG_TMHM_COUNT = 108
NEW_BAG_BERRIES_COUNT = 50
NEW_BAG_MEDICINE_COUNT = 45
NEW_BAG_BATTLEITEMS_COUNT = 40
NEW_BAG_POWERUP_COUNT = 30
NEW_BAG_MEGASTONES_COUNT = 47
NEW_BAG_ZCRYSTALS_COUNT = 35

NEW_SB2_SAVE_SENTINEL = 0
NEW_SB2_SAVE_VERSION = 2
NEW_SB2_EXPANSION_VERSION = 4
NEW_SB2_PLAYER_NAME = 6
NEW_SB2_PLAYER_GENDER = 14
NEW_SB2_SPECIAL_WARP_FLAGS = 15
NEW_SB2_TRAINER_ID = 16
NEW_SB2_PLAY_TIME = 20
NEW_SB2_OPTIONS_BUTTON = 25
NEW_SB2_POKEDEX = 32
NEW_SB2_LOCAL_TIME = 160
NEW_SB2_GCN_LINK_FLAGS = 172
NEW_SB2_ENCRYPTION_KEY = 176

NEW_SB1_PC_ITEMS = 1176
NEW_SB1_BAG_ITEMS = 1572
NEW_SB1_BAG_KEYITEMS = 1812
NEW_SB1_BAG_POKEBALLS = 2012
NEW_SB1_BAG_TMHM = 2076
NEW_SB1_BAG_BERRIES = 2508
NEW_SB1_BAG_MEDICINE = 2708
NEW_SB1_BAG_BATTLEITEMS = 2888
NEW_SB1_BAG_POWERUP = 3048
NEW_SB1_BAG_MEGASTONES = 3168
NEW_SB1_BAG_ZCRYSTALS = 3356
NEW_SB1_POKEBLOCKS = 3496

ITEM_SLOT_SIZE = 4
PLAYER_NAME_LENGTH_PLUS_ONE = 8
TRAINER_ID_LENGTH = 4
PLAY_TIME_SIZE = 6
POKEDEX_SIZE = 128
LOCAL_TIME_SIZE = 12
GCN_LINK_FLAGS_SIZE = 4
ENCRYPTION_KEY_SIZE = 4


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


def copy_item_slots(src: bytes, src_offset: int, src_count: int, dst: bytearray, dst_offset: int, dst_count: int):
    src_size = src_count * ITEM_SLOT_SIZE
    dst_size = dst_count * ITEM_SLOT_SIZE
    dst[dst_offset:dst_offset + dst_size] = b"\x00" * dst_size
    copy_size = min(src_size, dst_size)
    dst[dst_offset:dst_offset + copy_size] = src[src_offset:src_offset + copy_size]


def convert_saveblock1(sb1_old: bytes, sb1_shell: bytes) -> bytes:
    if len(sb1_old) < OLD_SB1_POKEBLOCKS:
        raise ValueError(f"old SaveBlock1 too small: {len(sb1_old)} < {OLD_SB1_POKEBLOCKS}")
    if len(sb1_shell) < NEW_SB1_SIZE:
        raise ValueError(f"shell SaveBlock1 too small: {len(sb1_shell)} < {NEW_SB1_SIZE}")

    out = bytearray(sb1_shell[:NEW_SB1_SIZE])
    old_sb1_size = len(sb1_old)

    # Copy stable prefix directly.
    out[:OLD_SB1_PC_ITEMS] = sb1_old[:OLD_SB1_PC_ITEMS]

    # PC items.
    copy_item_slots(sb1_old, OLD_SB1_PC_ITEMS, OLD_PC_ITEMS_COUNT, out, NEW_SB1_PC_ITEMS, NEW_PC_ITEMS_COUNT)

    # Old bag layout lives in one struct Bag block.
    old_items = OLD_SB1_BAG
    old_keys = old_items + OLD_BAG_ITEMS_COUNT * ITEM_SLOT_SIZE
    old_balls = old_keys + OLD_BAG_KEYITEMS_COUNT * ITEM_SLOT_SIZE
    old_tmhm = old_balls + OLD_BAG_POKEBALLS_COUNT * ITEM_SLOT_SIZE
    old_berries = old_tmhm + OLD_BAG_TMHM_COUNT * ITEM_SLOT_SIZE

    copy_item_slots(sb1_old, old_items, OLD_BAG_ITEMS_COUNT, out, NEW_SB1_BAG_ITEMS, NEW_BAG_ITEMS_COUNT)
    copy_item_slots(sb1_old, old_keys, OLD_BAG_KEYITEMS_COUNT, out, NEW_SB1_BAG_KEYITEMS, NEW_BAG_KEYITEMS_COUNT)
    copy_item_slots(sb1_old, old_balls, OLD_BAG_POKEBALLS_COUNT, out, NEW_SB1_BAG_POKEBALLS, NEW_BAG_POKEBALLS_COUNT)
    copy_item_slots(sb1_old, old_tmhm, OLD_BAG_TMHM_COUNT, out, NEW_SB1_BAG_TMHM, NEW_BAG_TMHM_COUNT)
    copy_item_slots(sb1_old, old_berries, OLD_BAG_BERRIES_COUNT, out, NEW_SB1_BAG_BERRIES, NEW_BAG_BERRIES_COUNT)

    # New extra pockets intentionally remain empty.
    for offset, count in (
        (NEW_SB1_BAG_MEDICINE, NEW_BAG_MEDICINE_COUNT),
        (NEW_SB1_BAG_BATTLEITEMS, NEW_BAG_BATTLEITEMS_COUNT),
        (NEW_SB1_BAG_POWERUP, NEW_BAG_POWERUP_COUNT),
        (NEW_SB1_BAG_MEGASTONES, NEW_BAG_MEGASTONES_COUNT),
        (NEW_SB1_BAG_ZCRYSTALS, NEW_BAG_ZCRYSTALS_COUNT),
    ):
        out[offset:offset + count * ITEM_SLOT_SIZE] = b"\x00" * (count * ITEM_SLOT_SIZE)

    # Copy the rest of the old block after the old bag into the new post-bag tail.
    old_tail = sb1_old[OLD_SB1_POKEBLOCKS:old_sb1_size]
    tail_copy = min(len(old_tail), NEW_SB1_SIZE - NEW_SB1_POKEBLOCKS)
    out[NEW_SB1_POKEBLOCKS:NEW_SB1_POKEBLOCKS + tail_copy] = old_tail[:tail_copy]

    return bytes(out)


def convert_saveblock2(sb2_old: bytes, sb2_shell: bytes) -> bytes:
    if len(sb2_old) < OLD_SB2_SIZE:
        raise ValueError(f"old SaveBlock2 too small: {len(sb2_old)} < {OLD_SB2_SIZE}")
    if len(sb2_shell) < NEW_SB2_SIZE:
        raise ValueError(f"shell SaveBlock2 too small: {len(sb2_shell)} < {NEW_SB2_SIZE}")

    out = bytearray(sb2_shell[:NEW_SB2_SIZE])

    # Preserve the shell's migration metadata layout but ensure it is marked current.
    out[NEW_SB2_SAVE_SENTINEL] = 0xFF
    struct.pack_into("<H", out, NEW_SB2_SAVE_VERSION, 6)
    struct.pack_into("<H", out, NEW_SB2_EXPANSION_VERSION, 0)

    # Player identity / play time / core UI state.
    out[NEW_SB2_PLAYER_NAME:NEW_SB2_PLAYER_NAME + PLAYER_NAME_LENGTH_PLUS_ONE] = \
        sb2_old[OLD_SB2_PLAYER_NAME:OLD_SB2_PLAYER_NAME + PLAYER_NAME_LENGTH_PLUS_ONE]
    out[NEW_SB2_PLAYER_GENDER] = sb2_old[OLD_SB2_PLAYER_GENDER]
    out[NEW_SB2_SPECIAL_WARP_FLAGS] = sb2_old[OLD_SB2_SPECIAL_WARP_FLAGS]
    out[NEW_SB2_TRAINER_ID:NEW_SB2_TRAINER_ID + TRAINER_ID_LENGTH] = \
        sb2_old[OLD_SB2_TRAINER_ID:OLD_SB2_TRAINER_ID + TRAINER_ID_LENGTH]
    out[NEW_SB2_PLAY_TIME:NEW_SB2_PLAY_TIME + PLAY_TIME_SIZE] = \
        sb2_old[OLD_SB2_PLAY_TIME:OLD_SB2_PLAY_TIME + PLAY_TIME_SIZE]
    out[NEW_SB2_OPTIONS_BUTTON] = sb2_old[OLD_SB2_OPTIONS_BUTTON]

    # Preserve Pokedex / time / encryption bits where the layout is well-understood.
    out[NEW_SB2_POKEDEX:NEW_SB2_POKEDEX + POKEDEX_SIZE] = \
        sb2_old[OLD_SB2_POKEDEX:OLD_SB2_POKEDEX + POKEDEX_SIZE]
    out[NEW_SB2_LOCAL_TIME:NEW_SB2_LOCAL_TIME + LOCAL_TIME_SIZE] = \
        sb2_old[OLD_SB2_LOCAL_TIME:OLD_SB2_LOCAL_TIME + LOCAL_TIME_SIZE]
    out[NEW_SB2_GCN_LINK_FLAGS:NEW_SB2_GCN_LINK_FLAGS + GCN_LINK_FLAGS_SIZE] = \
        sb2_old[OLD_SB2_GCN_LINK_FLAGS:OLD_SB2_GCN_LINK_FLAGS + GCN_LINK_FLAGS_SIZE]
    out[NEW_SB2_ENCRYPTION_KEY:NEW_SB2_ENCRYPTION_KEY + ENCRYPTION_KEY_SIZE] = \
        sb2_old[OLD_SB2_ENCRYPTION_KEY:OLD_SB2_ENCRYPTION_KEY + ENCRYPTION_KEY_SIZE]

    return bytes(out)


def migrate(source: Path, shell: Path, output: Path):
    src = bytearray(source.read_bytes())
    dst = bytearray(shell.read_bytes())
    if len(src) != SECTOR_SIZE * NUM_TOTAL_SECTORS or len(dst) != SECTOR_SIZE * NUM_TOTAL_SECTORS:
        raise SystemExit("Expected 128 KiB Emerald saves for both source and shell")

    src_slot = pick_latest_slot(src)
    dst_slot = pick_latest_slot(dst)
    src_map = map_slot_by_sector_id(src, src_slot)
    dst_map = map_slot_by_sector_id(dst, dst_slot)

    src_sb2 = read_slot_blob(src, src_map, 0, 0)[:OLD_SB2_SIZE]
    src_sb1 = read_slot_blob(src, src_map, 1, 4)[:OLD_SB1_SIZE]
    src_storage = read_slot_blob(src, src_map, 5, 13)

    shell_sb2 = read_slot_blob(dst, dst_map, 0, 0)[:NEW_SB2_SIZE]
    shell_sb1 = read_slot_blob(dst, dst_map, 1, 4)[:NEW_SB1_SIZE]

    new_sb2 = convert_saveblock2(src_sb2, shell_sb2)
    new_sb1 = convert_saveblock1(src_sb1, shell_sb1)

    _, _, _, src_counter = footer(src, src_slot)
    _, _, _, dst_counter = footer(dst, dst_slot)
    new_counter = max(src_counter, dst_counter) + 1

    sb2_blob = new_sb2 + b"\x00" * (SECTOR_DATA_SIZE - len(new_sb2))
    sb1_blob = new_sb1 + b"\x00" * (SECTOR_DATA_SIZE * 4 - len(new_sb1))

    for target_slot in SLOT_BASES:
        write_slot_blob(dst, target_slot, 0, 0, sb2_blob, new_counter)
        write_slot_blob(dst, target_slot, 1, 4, sb1_blob, new_counter)
        write_slot_blob(dst, target_slot, 5, 13, src_storage, new_counter)

    # Preserve Hall of Fame / trainer hill / recorded battle sectors from source where present.
    for sector_index in SPECIAL_SECTOR_IDS:
        src_base = sector_index * SECTOR_SIZE
        dst_base = sector_index * SECTOR_SIZE
        dst[dst_base:dst_base + SECTOR_SIZE] = src[src_base:src_base + SECTOR_SIZE]

    backup = output.with_suffix(output.suffix + ".bak")
    if output.exists() and not backup.exists():
        shutil.copy2(output, backup)
    output.write_bytes(dst)


def main():
    parser = argparse.ArgumentParser(description="Migrate an old BaneMode Emerald save into a Quest Engine save shell.")
    parser.add_argument("source", type=Path, help="Old BaneMode save (.sav)")
    parser.add_argument("shell", type=Path, help="Fresh Quest Engine save shell accepted by the emulator")
    parser.add_argument("output", type=Path, help="Output migrated save path")
    args = parser.parse_args()

    migrate(args.source, args.shell, args.output)
    print(args.output)


if __name__ == "__main__":
    main()
