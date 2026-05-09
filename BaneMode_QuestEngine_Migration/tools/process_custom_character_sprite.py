from __future__ import annotations

from collections import deque
from pathlib import Path
from typing import Iterable

from PIL import Image


ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "sprite_clean.png"
INDEXED = ROOT / "sprite_indexed.png"
FINAL = ROOT / "sprite_final.png"
DEST = ROOT / "graphics/object_events/pics/people/custom_character.png"

FRAME_W = 16
FRAME_H = 32
SHEET_COLS = 3
SHEET_ROWS = 4


def find_components(img: Image.Image) -> list[tuple[int, tuple[int, int, int, int]]]:
    rgba = img.convert("RGBA")
    alpha = rgba.getchannel("A")
    w, h = rgba.size
    px = alpha.load()
    seen = [[False] * w for _ in range(h)]
    comps: list[tuple[int, tuple[int, int, int, int]]] = []

    for y in range(h):
        for x in range(w):
            if seen[y][x]:
                continue
            seen[y][x] = True
            if px[x, y] == 0:
                continue

            q: deque[tuple[int, int]] = deque([(x, y)])
            pts: list[tuple[int, int]] = []
            while q:
                cx, cy = q.popleft()
                pts.append((cx, cy))
                for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                    if 0 <= nx < w and 0 <= ny < h and not seen[ny][nx]:
                        seen[ny][nx] = True
                        if px[nx, ny] != 0:
                            q.append((nx, ny))

            xs = [p[0] for p in pts]
            ys = [p[1] for p in pts]
            comps.append((len(pts), (min(xs), min(ys), max(xs) + 1, max(ys) + 1)))

    return comps


def make_indexed(img: Image.Image, *, max_visible_colors: int = 15) -> tuple[Image.Image, int]:
    rgba = img.convert("RGBA")
    rgba_px = rgba.load()
    w, h = rgba.size

    visible_pixels: list[tuple[int, int, int]] = [
        rgba_px[x, y][:3]
        for y in range(h)
        for x in range(w)
        if rgba_px[x, y][3] != 0
    ]
    visible_colors = set(visible_pixels)

    if not visible_pixels:
        out = Image.new("P", rgba.size, 0)
        out.putpalette([0] * 768)
        out.info["transparency"] = 0
        return out, 0

    # Quantize only visible pixels so transparent areas don't bias the palette dark.
    actual_colors = min(max_visible_colors, len(visible_colors))
    strip = Image.new("RGB", (len(visible_pixels), 1))
    strip.putdata(visible_pixels)
    quantized = strip.quantize(colors=actual_colors, dither=Image.Dither.NONE)
    raw_palette = quantized.getpalette()
    base_palette = raw_palette[:actual_colors * 3]
    pal_rgb = [(base_palette[i * 3], base_palette[i * 3 + 1], base_palette[i * 3 + 2])
               for i in range(actual_colors)]

    palette = [0, 0, 0] + base_palette
    palette.extend([0] * (768 - len(palette)))

    out = Image.new("P", rgba.size, color=0)
    out.putpalette(palette)
    out.info["transparency"] = 0

    dst_idx = out.load()
    for y in range(h):
        for x in range(w):
            r, g, b, a = rgba_px[x, y]
            if a == 0:
                dst_idx[x, y] = 0
            else:
                dst_idx[x, y] = min(range(actual_colors),
                                    key=lambda i: (r - pal_rgb[i][0]) ** 2 + (g - pal_rgb[i][1]) ** 2 + (b - pal_rgb[i][2]) ** 2) + 1

    return out, len(visible_colors)


def fit_frame(frame: Image.Image) -> tuple[Image.Image, bool]:
    frame = frame.convert("RGBA")
    scale = min(FRAME_W / frame.width, FRAME_H / frame.height)
    new_w = max(1, round(frame.width * scale))
    new_h = max(1, round(frame.height * scale))
    resized = frame.resize((new_w, new_h), Image.Resampling.NEAREST)
    canvas = Image.new("RGBA", (FRAME_W, FRAME_H), (0, 0, 0, 0))
    paste_x = (FRAME_W - new_w) // 2
    paste_y = FRAME_H - new_h
    canvas.paste(resized, (paste_x, paste_y), resized)
    cropped = new_w > FRAME_W or new_h > FRAME_H
    return canvas, cropped


def row_sort(items: Iterable[tuple[int, int, int, int]]) -> list[list[tuple[int, int, int, int]]]:
    bboxes = sorted(items, key=lambda b: (b[1], b[0]))
    rows: list[list[tuple[int, int, int, int]]] = []
    for bbox in bboxes:
        if not rows or abs(bbox[1] - rows[-1][0][1]) > 12:
            rows.append([bbox])
        else:
            rows[-1].append(bbox)
    for row in rows:
        row.sort(key=lambda b: b[0])
    return rows


def main() -> None:
    if not SRC.exists():
        raise FileNotFoundError(f"Missing source sprite sheet: {SRC}")

    source = Image.open(SRC).convert("RGBA")
    indexed_source, source_visible_colors = make_indexed(source)
    indexed_source.save(INDEXED)

    components = find_components(source)
    mini_bboxes = [
        bbox
        for area, bbox in components
        if 200 <= area <= 400
        and 45 <= bbox[0] <= 105
        and bbox[2] <= 125
        and 40 <= bbox[1] <= 145
        and bbox[3] <= 170
    ]

    rows = row_sort(mini_bboxes)
    if len(rows) != SHEET_ROWS or any(len(row) != SHEET_COLS for row in rows):
        raise RuntimeError(
            f"Expected a 4x3 mini-sprite grid, found row sizes {[len(row) for row in rows]} from {len(mini_bboxes)} candidates"
        )

    # Source row order inferred from the mini sprite cluster in the provided sheet:
    # row 3 = front/south, row 4 = back/north, row 2 = west, row 1 = east.
    ordered_rows = [
        rows[2],  # walk south
        rows[3],  # walk north
        rows[1],  # walk west
        rows[0],  # walk east
    ]

    final_rgba = Image.new("RGBA", (FRAME_W * SHEET_COLS, FRAME_H * SHEET_ROWS), (0, 0, 0, 0))
    crop_warnings: list[str] = []
    placed_frames = 0
    for row_idx, row in enumerate(ordered_rows):
        for col_idx, bbox in enumerate(row):
            frame = source.crop(bbox)
            fitted, cropped = fit_frame(frame)
            if cropped:
                crop_warnings.append(f"row={row_idx + 1} col={col_idx + 1} bbox={bbox}")
            final_rgba.paste(fitted, (col_idx * FRAME_W, row_idx * FRAME_H), fitted)
            placed_frames += 1

    final_indexed, final_visible_colors = make_indexed(final_rgba)
    final_indexed.save(FINAL)
    DEST.parent.mkdir(parents=True, exist_ok=True)
    final_indexed.save(DEST)

    print(f"source_visible_colors={source_visible_colors}")
    print(f"final_visible_colors={final_visible_colors}")
    print(f"frames_detected={len(mini_bboxes)}")
    print(f"frames_placed={placed_frames}")
    print(f"sprite_indexed={INDEXED}")
    print(f"sprite_final={FINAL}")
    print(f"custom_character_png={DEST}")
    if source_visible_colors > 15:
        print(f"WARNING: source sprite used {source_visible_colors} visible colors before quantization")
    if crop_warnings:
        print("WARNING: some frames were cropped during resize")
        for warning in crop_warnings:
            print(f"  {warning}")
    else:
        print("resize_cropping=none")


if __name__ == "__main__":
    main()
