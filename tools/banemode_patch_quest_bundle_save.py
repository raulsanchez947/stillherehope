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

SB1_SIZE = 15696
SB2_ENCRYPTION_KEY = 0xB0

SB1_ITEMS = 0x624
SB1_KEY = 0x714
SB1_BALL = 0x7DC
SB1_TM = 0x81C
SB1_BERRIES = 0x9CC
SB1_MED = 0xA94
SB1_BATTLE = 0xB48
SB1_POWER = 0xBE8
SB1_MEGA = 0xC60
SB1_Z = 0xD1C

ITEMS_CAP = 60
KEY_CAP = 50
BALL_CAP = 16
TM_CAP = 108
BERRY_CAP = 50
MED_CAP = 45
BATTLE_CAP = 40
POWER_CAP = 30
MEGA_CAP = 47
Z_CAP = 35

ITEM_TM01 = 582
ITEM_TM100 = 681
ITEM_HOLY_BLUNT = 54  # Sacred Ash repurpose
ITEM_TM_CASE = 715
ITEM_POKEMON_BOX_LINK = 717
ITEM_FIRE_STONE = 211
ITEM_DAWN_STONE = 220

ITEM_SWEET_APPLE = 221
ITEM_TART_APPLE = 222
ITEM_CRACKED_POT = 223
ITEM_CHIPPED_POT = 224
ITEM_GALARICA_CUFF = 225
ITEM_GALARICA_WREATH = 226
ITEM_DRAGON_SCALE = 227
ITEM_UPGRADE = 228
ITEM_PROTECTOR = 229
ITEM_ELECTIRIZER = 230
ITEM_MAGMARIZER = 231
ITEM_DUBIOUS_DISC = 232
ITEM_REAPER_CLOTH = 233
ITEM_PRISM_SCALE = 234
ITEM_WHIPPED_DREAM = 235
ITEM_SACHET = 236
ITEM_OVAL_STONE = 237

ITEM_METAL_COAT = 441
ITEM_KINGS_ROCK = 465
ITEM_RAZOR_CLAW = 492
ITEM_RAZOR_FANG = 493

ITEM_VENUSAURITE = 292
ITEM_DIANCITE = 338

ITEM_BLACK_AUGURITE = 766
ITEM_LINKING_CORD = 767
ITEM_PEAT_BLOCK = 768
ITEM_AUSPICIOUS_ARMOR = 770
ITEM_LEADERS_CREST = 774
ITEM_MALICIOUS_ARMOR = 775
ITEM_SCROLL_OF_DARKNESS = 777
ITEM_SCROLL_OF_WATERS = 778
ITEM_SYRUPY_APPLE = 804
ITEM_UNREMARKABLE_TEACUP = 805
ITEM_MASTERPIECE_TEACUP = 806

KEY_ITEM_BUNDLE = [
    (ITEM_TM_CASE, 1),
    (ITEM_POKEMON_BOX_LINK, 1),
]

ITEMS_BUNDLE = [
    (ITEM_HOLY_BLUNT, 1),
    (ITEM_FIRE_STONE, 99),
    (ITEM_FIRE_STONE + 1, 99),
    (ITEM_FIRE_STONE + 2, 99),
    (ITEM_FIRE_STONE + 3, 99),
    (ITEM_FIRE_STONE + 4, 99),
    (ITEM_FIRE_STONE + 5, 99),
    (ITEM_FIRE_STONE + 6, 99),
    (ITEM_FIRE_STONE + 7, 99),
    (ITEM_FIRE_STONE + 8, 99),
    (ITEM_FIRE_STONE + 9, 99),
    (ITEM_SWEET_APPLE, 99),
    (ITEM_TART_APPLE, 99),
    (ITEM_CRACKED_POT, 99),
    (ITEM_CHIPPED_POT, 99),
    (ITEM_GALARICA_CUFF, 99),
    (ITEM_GALARICA_WREATH, 99),
    (ITEM_DRAGON_SCALE, 99),
    (ITEM_UPGRADE, 99),
    (ITEM_PROTECTOR, 99),
    (ITEM_ELECTIRIZER, 99),
    (ITEM_MAGMARIZER, 99),
    (ITEM_DUBIOUS_DISC, 99),
    (ITEM_REAPER_CLOTH, 99),
    (ITEM_PRISM_SCALE, 99),
    (ITEM_WHIPPED_DREAM, 99),
    (ITEM_SACHET, 99),
    (ITEM_OVAL_STONE, 99),
    (ITEM_METAL_COAT, 99),
    (ITEM_KINGS_ROCK, 99),
    (ITEM_RAZOR_CLAW, 99),
    (ITEM_RAZOR_FANG, 99),
    (ITEM_BLACK_AUGURITE, 99),
    (ITEM_LINKING_CORD, 99),
    (ITEM_PEAT_BLOCK, 99),
    (ITEM_AUSPICIOUS_ARMOR, 99),
    (ITEM_LEADERS_CREST, 99),
    (ITEM_MALICIOUS_ARMOR, 99),
    (ITEM_SCROLL_OF_DARKNESS, 99),
    (ITEM_SCROLL_OF_WATERS, 99),
    (ITEM_SYRUPY_APPLE, 99),
    (ITEM_UNREMARKABLE_TEACUP, 99),
    (ITEM_MASTERPIECE_TEACUP, 99),
]


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


def get_qty(buf: bytearray, offset: int, key: int) -> int:
    return struct.unpack_from("<H", buf, offset)[0] ^ (key & 0xFFFF)


def set_qty(buf: bytearray, offset: int, value: int, key: int):
    struct.pack_into("<H", buf, offset, value ^ (key & 0xFFFF))


def add_item_to_pocket(sb1: bytearray, pocket_offset: int, capacity: int, item_id: int, quantity: int, key: int):
    first_empty = None
    for i in range(capacity):
        slot = pocket_offset + i * 4
        cur_item = struct.unpack_from("<H", sb1, slot)[0]
        if cur_item == item_id:
            cur_qty = get_qty(sb1, slot + 2, key)
            set_qty(sb1, slot + 2, min(999, max(cur_qty, quantity)), key)
            return
        if cur_item == 0 and first_empty is None:
            first_empty = slot
    if first_empty is None:
        raise SystemExit(f"No free slot available for item {item_id} in pocket {hex(pocket_offset)}")
    struct.pack_into("<H", sb1, first_empty, item_id)
    set_qty(sb1, first_empty + 2, quantity, key)


def patch_saveblock1(sb1: bytes, encryption_key: int) -> bytes:
    if len(sb1) < SB1_SIZE:
        raise ValueError(f"SaveBlock1 too small: {len(sb1)} < {SB1_SIZE}")

    out = bytearray(sb1[:SB1_SIZE])

    for item_id, qty in KEY_ITEM_BUNDLE:
        add_item_to_pocket(out, SB1_KEY, KEY_CAP, item_id, qty, encryption_key)

    for item_id, qty in ITEMS_BUNDLE:
        add_item_to_pocket(out, SB1_ITEMS, ITEMS_CAP, item_id, qty, encryption_key)

    for item_id in range(ITEM_TM01, ITEM_TM100 + 1):
        add_item_to_pocket(out, SB1_TM, TM_CAP, item_id, 1, encryption_key)

    for item_id in range(ITEM_VENUSAURITE, ITEM_DIANCITE + 1):
        add_item_to_pocket(out, SB1_MEGA, MEGA_CAP, item_id, 1, encryption_key)

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

    encryption_key = struct.unpack_from("<I", sb2_blob, SB2_ENCRYPTION_KEY)[0]
    new_sb1 = patch_saveblock1(sb1_blob[:SB1_SIZE], encryption_key)

    _, _, _, old_counter = footer(raw, slot)
    new_counter = old_counter + 1

    sb1_write = new_sb1 + b"\x00" * (SECTOR_DATA_SIZE * 4 - len(new_sb1))

    for target_slot in SLOT_BASES:
        write_slot_blob(raw, target_slot, 0, 0, sb2_blob, new_counter)
        write_slot_blob(raw, target_slot, 1, 4, sb1_write, new_counter)
        write_slot_blob(raw, target_slot, 5, 13, storage_blob, new_counter)

    output_path.write_bytes(raw)


def main():
    parser = argparse.ArgumentParser(description="Patch a Quest save with the missing Birch BaneMode bundle.")
    parser.add_argument("save", type=Path, help="Input Quest save")
    parser.add_argument("output", type=Path, help="Output patched save")
    args = parser.parse_args()
    patch(args.save, args.output)
    print(args.output)


if __name__ == "__main__":
    main()
