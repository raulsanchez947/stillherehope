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
SB1_FLAGS = 0x1270
SB1_VARS = 0x139C

MAP_GROUP_ROUTE119 = 0
MAP_NUM_ROUTE119 = 34
LAYOUT_ROUTE119 = 35
WARP_ID_NONE = -1
VARS_START = 0x4000
VAR_WEATHER_INSTITUTE_STATE = 0x40B3
FLAG_HIDE_ROUTE_119_TEAM_AQUA = 0x37A
FLAG_HIDE_WEATHER_INSTITUTE_1F_WORKERS = 0x37C
FLAG_HIDE_WEATHER_INSTITUTE_2F_WORKERS = 0x37D
FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M = 0x3E0
TRAINER_FLAGS_START = 0x500
MAX_TRAINERS_COUNT = 864
SYSTEM_FLAGS = TRAINER_FLAGS_START + MAX_TRAINERS_COUNT
FLAG_BADGE01_GET = SYSTEM_FLAGS + 0x7
NUM_BADGES = 8

# Just outside the Weather Institute door on Route 119.
RESCUE_X = 6
RESCUE_Y = 33


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
        MAP_GROUP_ROUTE119,
        MAP_NUM_ROUTE119,
        WARP_ID_NONE,
        0,
        RESCUE_X,
        RESCUE_Y,
    )


def set_flag(data: bytearray, sector_saveblock_offset: int, flag: int, value: bool):
    byte_offset = SB1_FLAGS - sector_saveblock_offset + (flag >> 3)
    bit = 1 << (flag & 7)
    if value:
        data[byte_offset] |= bit
    else:
        data[byte_offset] &= ~bit


def repair_save(path: Path, dry_run: bool, badge_count: int | None):
    buf = bytearray(path.read_bytes())
    if len(buf) != SECTOR_SIZE * 32:
        raise SystemExit(f"{path}: expected 128 KiB Emerald save, got {len(buf)} bytes")

    touched = []
    slot_summaries = []

    for slot_base in (0, 14):
        counters = []
        ids = []
        for i in range(14):
            sid, sector_checksum, sig, counter = sector_footer(buf, slot_base + i)
            if sig == SECTOR_SIGNATURE and sid < 14:
                counters.append(counter)
                ids.append(sid)
        slot_summaries.append((slot_base // 14 + 1, sorted(ids), counters[0] if counters else None))

    for sector_index in range(28):
        base = sector_index * SECTOR_SIZE
        sector_id, _, signature, counter = sector_footer(buf, sector_index)
        if signature != SECTOR_SIGNATURE:
            continue

        if sector_id == 0:
            buf[base + 0x09] |= CONTINUE_GAME_WARP
            new_checksum = checksum(buf[base:base + SECTOR_DATA_SIZE], SAVE_BLOCK2_SIZE)
            struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)
            touched.append((sector_index, sector_id, counter, "set continue warp flag"))

        elif sector_id == 1:
            data = buf[base:base + SECTOR_DATA_SIZE]
            struct.pack_into("<hh", data, SB1_POS, RESCUE_X, RESCUE_Y)
            write_warp(data, SB1_LOCATION)
            write_warp(data, SB1_CONTINUE_GAME_WARP)
            write_warp(data, SB1_DYNAMIC_WARP)
            struct.pack_into("<H", data, SB1_MAP_LAYOUT_ID, LAYOUT_ROUTE119)
            buf[base:base + SECTOR_DATA_SIZE] = data
            new_checksum = checksum(buf[base:base + SECTOR_DATA_SIZE], SAVE_BLOCK1_SECTOR_SIZE)
            struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)
            touched.append((sector_index, sector_id, counter, "move continue to Route 119 outside Weather Institute"))

        elif sector_id == 2:
            data = bytearray(buf[base:base + SECTOR_DATA_SIZE])
            sector_saveblock_offset = SECTOR_DATA_SIZE
            weather_var_offset = SB1_VARS - sector_saveblock_offset + (VAR_WEATHER_INSTITUTE_STATE - VARS_START) * 2
            struct.pack_into("<H", data, weather_var_offset, 0)
            set_flag(data, sector_saveblock_offset, FLAG_HIDE_ROUTE_119_TEAM_AQUA, False)
            set_flag(data, sector_saveblock_offset, FLAG_HIDE_WEATHER_INSTITUTE_1F_WORKERS, True)
            set_flag(data, sector_saveblock_offset, FLAG_HIDE_WEATHER_INSTITUTE_2F_WORKERS, True)
            set_flag(data, sector_saveblock_offset, FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M, True)
            if badge_count is not None:
                for badge_index in range(NUM_BADGES):
                    set_flag(data, sector_saveblock_offset, FLAG_BADGE01_GET + badge_index, badge_index < badge_count)
            buf[base:base + SECTOR_DATA_SIZE] = data
            new_checksum = checksum(buf[base:base + SECTOR_DATA_SIZE], SAVE_BLOCK1_SECTOR_SIZE)
            struct.pack_into("<H", buf, base + SECTOR_DATA_SIZE + 118, new_checksum)
            action = "normalize Weather Institute takeover flags"
            if badge_count is not None:
                action += f" and set {badge_count} badge flags"
            touched.append((sector_index, sector_id, counter, action))

    if not touched:
        raise SystemExit(f"{path}: no valid SaveBlock1/2 sectors found")

    if not dry_run:
        backup = path.with_suffix(path.suffix + ".weather-broken.bak")
        if not backup.exists():
            shutil.copy2(path, backup)
        path.write_bytes(buf)

    return slot_summaries, touched


def main():
    parser = argparse.ArgumentParser(description="Repair Bane Mode saves stuck in Weather Institute with blank sprites.")
    parser.add_argument("saves", nargs="+", type=Path)
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--badges", type=int, choices=range(NUM_BADGES + 1), metavar="0-8")
    args = parser.parse_args()

    for path in args.saves:
        slots, touched = repair_save(path, args.dry_run, args.badges)
        print(path)
        for slot_num, ids, counter in slots:
            print(f"  slot {slot_num}: ids={ids} counter={counter}")
        for sector_index, sector_id, counter, action in touched:
            print(f"  sector {sector_index:02d} id={sector_id:02d} counter={counter}: {action}")
        if args.dry_run:
            print("  dry run only")
        else:
            print("  repaired")


if __name__ == "__main__":
    main()
