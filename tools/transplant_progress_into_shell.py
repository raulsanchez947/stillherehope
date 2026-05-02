#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

SECTOR_SIZE = 0x1000
SECTOR_DATA_SIZE = 3968
SECTOR_SIGNATURE = 0x08012025
NUM_MAIN_SECTORS = 14
SLOT_BASES = (0, 14)


def footer(buf: bytes, sector_index: int):
    base = sector_index * SECTOR_SIZE
    sector_id = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 116)[0]
    sector_checksum = struct.unpack_from("<H", buf, base + SECTOR_DATA_SIZE + 118)[0]
    signature = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 120)[0]
    counter = struct.unpack_from("<I", buf, base + SECTOR_DATA_SIZE + 124)[0]
    return sector_id, sector_checksum, signature, counter


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


def transplant(source: Path, shell: Path, output: Path):
    src = bytearray(source.read_bytes())
    dst = bytearray(shell.read_bytes())
    if len(src) != SECTOR_SIZE * 32 or len(dst) != SECTOR_SIZE * 32:
        raise SystemExit("Expected 128 KiB Emerald saves for both source and shell")

    src_slot = pick_latest_slot(src)
    dst_slot = pick_latest_slot(dst)
    src_map = map_slot_by_sector_id(src, src_slot)
    dst_map = map_slot_by_sector_id(dst, dst_slot)

    _, _, _, dst_counter = footer(dst, dst_slot)
    _, _, _, src_counter = footer(src, src_slot)
    new_counter = max(dst_counter, src_counter) + 1

    # Use the shell's physical ordering as the destination skeleton, but replace
    # each logical sector's full contents with the source progress.
    for target_slot in SLOT_BASES:
        for logical_sid in range(NUM_MAIN_SECTORS):
            src_sector_index = src_map[logical_sid]
            dst_sector_index = target_slot + logical_sid
            src_base = src_sector_index * SECTOR_SIZE
            dst_base = dst_sector_index * SECTOR_SIZE
            dst[dst_base:dst_base + SECTOR_SIZE] = src[src_base:src_base + SECTOR_SIZE]
            struct.pack_into("<H", dst, dst_base + SECTOR_DATA_SIZE + 116, logical_sid)
            struct.pack_into("<I", dst, dst_base + SECTOR_DATA_SIZE + 120, SECTOR_SIGNATURE)
            struct.pack_into("<I", dst, dst_base + SECTOR_DATA_SIZE + 124, new_counter)

    backup = output.with_suffix(output.suffix + ".bak")
    if output.exists() and not backup.exists():
        shutil.copy2(output, backup)
    output.write_bytes(dst)


def main():
    parser = argparse.ArgumentParser(description="Transplant Emerald save progress into a known-good shell save.")
    parser.add_argument("source", type=Path, help="Old progress save")
    parser.add_argument("shell", type=Path, help="Fresh emulator-accepted save shell")
    parser.add_argument("output", type=Path, help="Output transplanted save")
    args = parser.parse_args()
    transplant(args.source, args.shell, args.output)
    print(args.output)


if __name__ == "__main__":
    main()
