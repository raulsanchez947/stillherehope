#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025

SAVE_BLOCK2_SIZE = 0xF2C
SAVE_BLOCK1_SECTOR_SIZE = SECTOR_DATA_SIZE

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

# One tile below the Rustboro Gym door so the player doesn't instantly re-enter.
RESCUE_X = 27
RESCUE_Y = 20


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


def repair_save(input_path: Path, output_path: Path):
    buf = bytearray(input_path.read_bytes())
    if len(buf) != SECTOR_SIZE * 32:
        raise SystemExit(f"{input_path}: expected 128 KiB Emerald save, got {len(buf)} bytes")

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
        raise SystemExit(f"{input_path}: no valid save slots found")

    for slot_base, counter in valid_slots:
        slot_num = slot_base // 14 + 1
        for i in range(14):
            sector_index = slot_base + i
            base = sector_index * SECTOR_SIZE
            sector_id, _, signature, _ = sector_footer(buf, sector_index)
            if signature != SECTOR_SIGNATURE:
                continue

            if sector_id == 0:
                buf[base + 0x09] |= CONTINUE_GAME_WARP
                new_checksum = checksum(buf[base:base + SECTOR_DATA_SIZE], SAVE_BLOCK2_SIZE)
                struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)
                touched.append((slot_num, sector_index, counter, "set continue warp flag"))

            elif sector_id == 1:
                data = bytearray(buf[base:base + SECTOR_DATA_SIZE])
                struct.pack_into("<hh", data, SB1_POS, RESCUE_X, RESCUE_Y)
                write_warp(data, SB1_LOCATION)
                write_warp(data, SB1_CONTINUE_GAME_WARP)
                write_warp(data, SB1_DYNAMIC_WARP)
                struct.pack_into("<H", data, SB1_MAP_LAYOUT_ID, LAYOUT_RUSTBORO_CITY)
                buf[base:base + SECTOR_DATA_SIZE] = data
                new_checksum = checksum(buf[base:base + SECTOR_DATA_SIZE], SAVE_BLOCK1_SECTOR_SIZE)
                struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)
                touched.append((slot_num, sector_index, counter, "move player outside Rustboro Gym"))

        slot_touched = clear_range_across_sb1(buf, slot_base, SB1_OBJECT_EVENT_TEMPLATES, OBJECT_EVENT_TEMPLATES_SIZE)
        for sector_index, sector_data_offset, write_len in slot_touched:
            touched.append(
                (
                    slot_num,
                    sector_index,
                    counter,
                    f"cleared stale object templates at 0x{sector_data_offset:X} ({write_len} bytes)",
                )
            )

    if output_path.exists():
        backup = output_path.with_suffix(output_path.suffix + ".bak")
        shutil.copy2(output_path, backup)
    output_path.write_bytes(buf)
    return touched


def main():
    parser = argparse.ArgumentParser(description="Move a Quest save out of Rustboro Gym and clear stale object state.")
    parser.add_argument("input_save", type=Path)
    parser.add_argument("output_save", type=Path)
    args = parser.parse_args()

    touched = repair_save(args.input_save, args.output_save)
    print(args.output_save)
    for slot_num, sector_index, counter, action in touched:
        print(f"  slot {slot_num} sector {sector_index:02d} counter={counter}: {action}")


if __name__ == "__main__":
    main()
