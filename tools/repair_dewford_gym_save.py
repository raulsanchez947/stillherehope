#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025

SB1_OBJECT_EVENT_TEMPLATES = 0x0C70
OBJECT_EVENT_TEMPLATES_SIZE = 64 * 0x18


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


def clear_range_across_sb1(buf: bytearray, slot_base: int, start: int, size: int):
    remaining = size
    cursor = start
    touched = []

    while remaining > 0:
        sector_in_sb1 = cursor // SECTOR_DATA_SIZE
        if sector_in_sb1 < 0 or sector_in_sb1 > 3:
            raise SystemExit("Object template range fell outside SaveBlock1")

        sector_index = slot_base + 1 + sector_in_sb1
        sector_data_offset = cursor % SECTOR_DATA_SIZE
        write_len = min(remaining, SECTOR_DATA_SIZE - sector_data_offset)

        sector_base = sector_index * SECTOR_SIZE
        data = bytearray(buf[sector_base:sector_base + SECTOR_DATA_SIZE])
        data[sector_data_offset:sector_data_offset + write_len] = b"\x00" * write_len
        buf[sector_base:sector_base + SECTOR_DATA_SIZE] = data

        new_checksum = checksum(data, SECTOR_DATA_SIZE)
        struct.pack_into("<H", buf, sector_base + SECTOR_DATA_SIZE + 118, new_checksum)

        touched.append((sector_index, sector_data_offset, write_len))
        cursor += write_len
        remaining -= write_len

    return touched


def repair_save(path: Path, dry_run: bool):
    buf = bytearray(path.read_bytes())
    if len(buf) != SECTOR_SIZE * 32:
        raise SystemExit(f"{path}: expected 128 KiB Emerald save, got {len(buf)} bytes")

    touched = []
    valid_slots = []

    for slot_base in (0, 14):
        ids = []
        counters = []
        for i in range(14):
            sid, _, sig, counter = sector_footer(buf, slot_base + i)
            if sig == SECTOR_SIGNATURE and sid < 14:
                ids.append(sid)
                counters.append(counter)

        if set(ids) >= {0, 1, 2, 3, 4}:
            valid_slots.append((slot_base, max(counters) if counters else None))

    if not valid_slots:
        raise SystemExit(f"{path}: no valid save slots found")

    for slot_base, counter in valid_slots:
        slot_touched = clear_range_across_sb1(buf, slot_base, SB1_OBJECT_EVENT_TEMPLATES, OBJECT_EVENT_TEMPLATES_SIZE)
        for sector_index, sector_data_offset, write_len in slot_touched:
            touched.append((slot_base // 14 + 1, sector_index, counter, sector_data_offset, write_len))

    if not dry_run:
        backup = path.with_suffix(path.suffix + ".dewford-objects.bak")
        if not backup.exists():
            shutil.copy2(path, backup)
        path.write_bytes(buf)

    return touched


def main():
    parser = argparse.ArgumentParser(description="Clear stale object event templates from Emerald save slots.")
    parser.add_argument("save", type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    touched = repair_save(args.save, args.dry_run)
    print(args.save)
    for slot_num, sector_index, counter, sector_data_offset, write_len in touched:
        print(
            f"  slot {slot_num} sector {sector_index:02d} counter={counter}: "
            f"cleared 0x{write_len:X} bytes at sb1-sector-offset 0x{sector_data_offset:X}"
        )
    if args.dry_run:
        print("  dry run only")
    else:
        print("  repaired")


if __name__ == "__main__":
    main()
