#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025

SB1_FIRST_SECTOR_ID = 1
SB1_LAST_SECTOR_ID = 4

OLD_PC_COUNT = 50
OLD_BAG_ITEMS = 30
OLD_BAG_KEYS = 30
OLD_BAG_BALLS = 16
OLD_BAG_TMHM = 64
OLD_BAG_BERRIES = 46

NEW_PC_COUNT = 50
NEW_BAG_ITEMS = 42
NEW_BAG_KEYS = 36
NEW_BAG_BALLS = 20
NEW_BAG_TMHM = 64
NEW_BAG_BERRIES = 50

SLOT_SIZE = 4
PC_OFFSET = 0x498
BAG_OFFSET = 0x560
OLD_TAIL_OFFSET = 0x848
NEW_TAIL_OFFSET = 0x8B0

OLD_SB1_TOTAL = SECTOR_DATA_SIZE * 4
NEW_SB1_TOTAL = SECTOR_DATA_SIZE * 4


def checksum(data: bytes, size: int) -> int:
    total = 0
    for offset in range(0, size // 4 * 4, 4):
        total = (total + struct.unpack_from("<I", data, offset)[0]) & 0xFFFFFFFF
    return ((total >> 16) + (total & 0xFFFF)) & 0xFFFF


def sector_footer(buf: bytes, sector_index: int):
    base = sector_index * SECTOR_SIZE
    sector_id = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 116)[0]
    sector_checksum = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 118)[0]
    signature = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 120)[0]
    counter = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 124)[0]
    return sector_id, sector_checksum, signature, counter


def copy_slots(src: bytes, src_offset: int, src_count: int, dst: bytearray, dst_offset: int, dst_count: int):
    copy_count = min(src_count, dst_count)
    dst[dst_offset:dst_offset + dst_count * SLOT_SIZE] = b"\x00" * (dst_count * SLOT_SIZE)
    dst[dst_offset:dst_offset + copy_count * SLOT_SIZE] = src[src_offset:src_offset + copy_count * SLOT_SIZE]


def convert_saveblock1(sb1_old: bytes) -> bytes:
    if len(sb1_old) != OLD_SB1_TOTAL:
        raise ValueError(f"expected {OLD_SB1_TOTAL} bytes of SaveBlock1 data, got {len(sb1_old)}")

    sb1_new = bytearray(len(sb1_old))

    # Copy everything before pc/bag as-is.
    sb1_new[:PC_OFFSET] = sb1_old[:PC_OFFSET]

    # PC items stay the same size in the new layout.
    pc_size = NEW_PC_COUNT * SLOT_SIZE
    sb1_new[PC_OFFSET:PC_OFFSET + pc_size] = sb1_old[PC_OFFSET:PC_OFFSET + pc_size]

    # Old pocket offsets.
    old_items_off = BAG_OFFSET
    old_keys_off = old_items_off + OLD_BAG_ITEMS * SLOT_SIZE
    old_balls_off = old_keys_off + OLD_BAG_KEYS * SLOT_SIZE
    old_tmhm_off = old_balls_off + OLD_BAG_BALLS * SLOT_SIZE
    old_berries_off = old_tmhm_off + OLD_BAG_TMHM * SLOT_SIZE

    # New pocket offsets.
    new_items_off = BAG_OFFSET
    new_keys_off = new_items_off + NEW_BAG_ITEMS * SLOT_SIZE
    new_balls_off = new_keys_off + NEW_BAG_KEYS * SLOT_SIZE
    new_tmhm_off = new_balls_off + NEW_BAG_BALLS * SLOT_SIZE
    new_berries_off = new_tmhm_off + NEW_BAG_TMHM * SLOT_SIZE

    copy_slots(sb1_old, old_items_off, OLD_BAG_ITEMS, sb1_new, new_items_off, NEW_BAG_ITEMS)
    copy_slots(sb1_old, old_keys_off, OLD_BAG_KEYS, sb1_new, new_keys_off, NEW_BAG_KEYS)
    copy_slots(sb1_old, old_balls_off, OLD_BAG_BALLS, sb1_new, new_balls_off, NEW_BAG_BALLS)
    copy_slots(sb1_old, old_tmhm_off, OLD_BAG_TMHM, sb1_new, new_tmhm_off, NEW_BAG_TMHM)
    copy_slots(sb1_old, old_berries_off, OLD_BAG_BERRIES, sb1_new, new_berries_off, NEW_BAG_BERRIES)

    # Shift the remainder of SaveBlock1 forward to the new tail offset.
    tail_len = len(sb1_old) - OLD_TAIL_OFFSET
    sb1_new[NEW_TAIL_OFFSET:NEW_TAIL_OFFSET + tail_len] = sb1_old[OLD_TAIL_OFFSET:]

    return bytes(sb1_new)


def read_sb1_slot(buf: bytes, slot_base: int):
    sectors = {}
    counters = []
    for i in range(14):
        sector_index = slot_base + i
        sid, _, sig, counter = sector_footer(buf, sector_index)
        if sig == SECTOR_SIGNATURE and SB1_FIRST_SECTOR_ID <= sid <= SB1_LAST_SECTOR_ID:
            sectors[sid] = sector_index
            counters.append(counter)
    if set(sectors) >= {1, 2, 3, 4}:
        data = bytearray()
        order = []
        for sid in range(1, 5):
            sector_index = sectors[sid]
            base = sector_index * SECTOR_SIZE
            data.extend(buf[base:base + SECTOR_DATA_SIZE])
            order.append(sector_index)
        return bytes(data), order, max(counters) if counters else None
    return None, None, None


def write_sb1_slot(buf: bytearray, sector_order: list[int], sb1_new: bytes):
    for chunk_index, sector_index in enumerate(sector_order):
        chunk = sb1_new[chunk_index * SECTOR_DATA_SIZE:(chunk_index + 1) * SECTOR_DATA_SIZE]
        base = sector_index * SECTOR_SIZE
        buf[base:base + SECTOR_DATA_SIZE] = chunk
        new_checksum = checksum(chunk, SECTOR_DATA_SIZE)
        struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)


def repair_save(path: Path, dry_run: bool):
    buf = bytearray(path.read_bytes())
    if len(buf) != SECTOR_SIZE * 32:
        raise SystemExit(f"{path}: expected 128 KiB Emerald save, got {len(buf)} bytes")

    touched = []
    valid_slots = []
    for slot_base in (0, 14):
        sb1_old, sector_order, counter = read_sb1_slot(buf, slot_base)
        if sb1_old is not None:
            valid_slots.append((slot_base, sb1_old, sector_order, counter))

    if not valid_slots:
        raise SystemExit(f"{path}: no valid SaveBlock1 slots found")

    for slot_base, sb1_old, sector_order, counter in valid_slots:
        sb1_new = convert_saveblock1(sb1_old)
        write_sb1_slot(buf, sector_order, sb1_new)
        touched.append((slot_base // 14 + 1, counter, sector_order))

    if not dry_run:
        backup = path.with_suffix(path.suffix + ".bag-layout.bak")
        if not backup.exists():
            shutil.copy2(path, backup)
        path.write_bytes(buf)

    return touched


def main():
    parser = argparse.ArgumentParser(description="Convert old Emerald save bag layout to the current Bane Mode layout.")
    parser.add_argument("save", type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    touched = repair_save(args.save, args.dry_run)
    print(args.save)
    for slot_num, counter, sector_order in touched:
        print(f"  slot {slot_num} counter={counter}: rewrote sectors {sector_order}")
    if args.dry_run:
        print("  dry run only")
    else:
        print("  converted")


if __name__ == "__main__":
    main()
