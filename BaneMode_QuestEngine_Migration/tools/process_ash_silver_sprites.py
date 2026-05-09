from __future__ import annotations

from pathlib import Path
from typing import Iterable

from PIL import Image, ImageFilter


ROOT = Path(__file__).resolve().parent.parent

ASH_SRC = Path("/Users/ivan/Downloads/_ash_ketchum___hg_ss_style_sprite_revamp_2_by_ianskelskey_dkfol39-fullview.jpg")
SILVER_SRC = Path("/Users/ivan/Downloads/Silver Sprite.png")

ASH_CLEAN = ROOT / "ash_clean.png"
SILVER_CLEAN = ROOT / "silver_clean.png"
ASH_INDEXED = ROOT / "ash_indexed.png"
SILVER_INDEXED = ROOT / "silver_indexed.png"
ASH_FINAL = ROOT / "ash_final.png"
SILVER_FINAL = ROOT / "silver_final.png"

ASH_DEST = ROOT / "graphics/object_events/pics/people/ash.png"
SILVER_DEST = ROOT / "graphics/object_events/pics/people/silver.png"

FRAME_W = 16
FRAME_H = 32
ROWS = 4
COLS = 3


def color_dist(a: tuple[int, int, int], b: tuple[int, int, int]) -> int:
    return max(abs(a[0] - b[0]), abs(a[1] - b[1]), abs(a[2] - b[2]))


def remove_bg(path: Path, tolerance: int, out_path: Path) -> tuple[Image.Image, tuple[int, int, int], int]:
    src = Image.open(path).convert("RGBA")
    bg = src.getpixel((0, 0))[:3]
    px = src.load()
    fringing = 0
    for y in range(src.height):
        for x in range(src.width):
            r, g, b, a = px[x, y]
            if color_dist((r, g, b), bg) <= tolerance:
                px[x, y] = (0, 0, 0, 0)
            elif color_dist((r, g, b), bg) <= tolerance + 12:
                fringing += 1
    src.save(out_path)
    return src, bg, fringing


def defringe_alpha(img: Image.Image, radius: int = 1) -> Image.Image:
    rgba = img.convert("RGBA")
    alpha = rgba.getchannel("A")
    for _ in range(radius):
        alpha = alpha.filter(ImageFilter.MinFilter(3))
    rgba.putalpha(alpha)
    return rgba


def make_indexed(
    img: Image.Image,
    *,
    max_visible_colors: int = 15,
    fixed_pal_rgb: list[tuple[int, int, int]] | None = None,
) -> tuple[Image.Image, int, int]:
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
        return out, 0, 0

    if fixed_pal_rgb is not None:
        pal_rgb = fixed_pal_rgb
    else:
        # Quantize only visible pixels so transparent areas don't bias the palette dark.
        actual_colors = min(max_visible_colors, len(visible_colors))
        strip = Image.new("RGB", (len(visible_pixels), 1))
        strip.putdata(visible_pixels)
        quantized = strip.quantize(colors=actual_colors, dither=Image.Dither.NONE)
        raw_palette = quantized.getpalette()
        base_palette = raw_palette[:actual_colors * 3]
        pal_rgb = [(base_palette[i * 3], base_palette[i * 3 + 1], base_palette[i * 3 + 2])
                   for i in range(actual_colors)]

    actual_colors = len(pal_rgb)
    base_palette = [c for rgb in pal_rgb for c in rgb]
    palette = [0, 0, 0] + base_palette
    palette.extend([0] * (768 - len(palette)))

    out = Image.new("P", rgba.size, color=0)
    out.putpalette(palette)
    out.info["transparency"] = 0

    dst = out.load()
    for y in range(h):
        for x in range(w):
            r, g, b, a = rgba_px[x, y]
            if a == 0:
                dst[x, y] = 0
            else:
                dst[x, y] = min(range(actual_colors),
                                key=lambda i: (r - pal_rgb[i][0]) ** 2 + (g - pal_rgb[i][1]) ** 2 + (b - pal_rgb[i][2]) ** 2) + 1

    final_visible_indexes = {
        dst[x, y]
        for y in range(h)
        for x in range(w)
        if dst[x, y] != 0
    }
    return out, len(visible_colors), len(final_visible_indexes)


def extract_pal_rgb(indexed_img: Image.Image, max_colors: int = 15) -> list[tuple[int, int, int]]:
    """Return up to max_colors palette entries from an indexed PNG (index 0 is transparent, skipped)."""
    raw = indexed_img.getpalette()
    return [(raw[(i + 1) * 3], raw[(i + 1) * 3 + 1], raw[(i + 1) * 3 + 2])
            for i in range(max_colors)]


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
    return canvas, new_w > FRAME_W or new_h > FRAME_H


def cluster_rows(bboxes: Iterable[tuple[int, int, int, int]], y_tol: int = 16) -> list[list[tuple[int, int, int, int]]]:
    rows: list[list[tuple[int, int, int, int]]] = []
    for bbox in sorted(bboxes, key=lambda b: (b[1], b[0])):
        if not rows or abs(bbox[1] - rows[-1][0][1]) > y_tol:
            rows.append([bbox])
        else:
            rows[-1].append(bbox)
    for row in rows:
        row.sort(key=lambda b: b[0])
    return rows


def connected_bboxes(img: Image.Image, min_area: int, max_area: int) -> list[tuple[int, int, int, int]]:
    rgba = img.convert("RGBA")
    alpha = rgba.getchannel("A")
    w, h = rgba.size
    seen = [[False] * w for _ in range(h)]
    px = alpha.load()
    out: list[tuple[int, int, int, int]] = []

    stack: list[tuple[int, int]]
    for y in range(h):
        for x in range(w):
            if seen[y][x]:
                continue
            seen[y][x] = True
            if px[x, y] == 0:
                continue
            stack = [(x, y)]
            pts: list[tuple[int, int]] = []
            while stack:
                cx, cy = stack.pop()
                pts.append((cx, cy))
                for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                    if 0 <= nx < w and 0 <= ny < h and not seen[ny][nx]:
                        seen[ny][nx] = True
                        if px[nx, ny] != 0:
                            stack.append((nx, ny))
            if min_area <= len(pts) <= max_area:
                xs = [p[0] for p in pts]
                ys = [p[1] for p in pts]
                out.append((min(xs), min(ys), max(xs) + 1, max(ys) + 1))
    return out


def extract_ash_frames(img: Image.Image) -> list[list[Image.Image]]:
    # Left mini grid in the provided Ash sheet is a regular 3x4 block.
    x_edges = [20, 148, 276, 404]
    y_edges = [20, 147, 274, 401, 512]
    source_rows: list[list[Image.Image]] = []
    for r in range(4):
        row = []
        for c in range(3):
            row.append(img.crop((x_edges[c], y_edges[r], x_edges[c + 1], y_edges[r + 1])))
        source_rows.append(row)
    ordered = [source_rows[0], source_rows[2], source_rows[1], source_rows[3]]
    return ordered


def extract_silver_frames(img: Image.Image) -> list[list[Image.Image]]:
    # Left mini grid is a regular 4x4 area from the provided sheet.
    x_edges = [0, 32, 64, 96, 128]
    y_edges = [2, 50, 98, 146, 192]
    source_rows: list[list[Image.Image]] = []
    for r in range(4):
        row = []
        for c in range(3):
            row.append(img.crop((x_edges[c], y_edges[r], x_edges[c + 1], y_edges[r + 1])))
        source_rows.append(row)
    ordered = [source_rows[0], source_rows[2], source_rows[1], source_rows[3]]
    return ordered


def build_final(frames: list[list[Image.Image]]) -> tuple[Image.Image, list[str]]:
    canvas = Image.new("RGBA", (COLS * FRAME_W, ROWS * FRAME_H), (0, 0, 0, 0))
    crop_warnings: list[str] = []
    for r, row in enumerate(frames):
        for c, frame in enumerate(row):
            fitted, cropped = fit_frame(frame)
            if cropped:
                crop_warnings.append(f"row={r + 1} col={c + 1}")
            canvas.paste(fitted, (c * FRAME_W, r * FRAME_H), fitted)
    return canvas, crop_warnings


def process_sprite(name: str, src_path: Path, clean_path: Path, indexed_path: Path, final_path: Path, dest_path: Path, tolerance: int) -> dict[str, object]:
    clean_rgba, bg, fringing = remove_bg(src_path, tolerance, clean_path)
    if name == "ash":
        clean_rgba = defringe_alpha(clean_rgba, radius=1)
        clean_rgba.save(clean_path)
    indexed_clean, visible_pre_quant, indexed_visible = make_indexed(clean_rgba)
    indexed_clean.save(indexed_path)
    shared_pal = extract_pal_rgb(indexed_clean)

    if name == "ash":
        frames = extract_ash_frames(clean_rgba)
    else:
        frames = extract_silver_frames(clean_rgba)

    final_rgba, crop_warnings = build_final(frames)
    final_indexed, _, final_visible = make_indexed(final_rgba, fixed_pal_rgb=shared_pal)
    final_indexed.save(final_path)
    dest_path.parent.mkdir(parents=True, exist_ok=True)
    final_indexed.save(dest_path)

    return {
        "name": name,
        "bg": bg,
        "visible_pre_quant": visible_pre_quant,
        "visible_indexed": indexed_visible,
        "visible_final": final_visible,
        "crop_warnings": crop_warnings,
        "fringing": fringing,
        "clean": clean_path,
        "indexed": indexed_path,
        "final": final_path,
        "dest": dest_path,
    }


def main() -> None:
    ash = process_sprite("ash", ASH_SRC, ASH_CLEAN, ASH_INDEXED, ASH_FINAL, ASH_DEST, tolerance=20)
    silver = process_sprite("silver", SILVER_SRC, SILVER_CLEAN, SILVER_INDEXED, SILVER_FINAL, SILVER_DEST, tolerance=15)

    for sprite in (ash, silver):
        print(f"[{sprite['name']}] background={sprite['bg']}")
        print(f"[{sprite['name']}] visible_colors_before_quant={sprite['visible_pre_quant']}")
        print(f"[{sprite['name']}] visible_colors_final={sprite['visible_final']}")
        print(f"[{sprite['name']}] clean={sprite['clean']}")
        print(f"[{sprite['name']}] indexed={sprite['indexed']}")
        print(f"[{sprite['name']}] final={sprite['final']}")
        print(f"[{sprite['name']}] dest={sprite['dest']}")
        if sprite["visible_pre_quant"] > 15:
            print(f"WARNING: {sprite['name']} source exceeded 15 visible colors before quantization")
        if sprite["crop_warnings"]:
            print(f"WARNING: {sprite['name']} had cropped frames during resize")
            for warning in sprite["crop_warnings"]:
                print(f"  {warning}")
        else:
            print(f"[{sprite['name']}] resize_cropping=none")

    if ash["fringing"] > 200:
        print(f"WARNING: ash background removal may have visible fringing ({ash['fringing']} near-background opaque pixels)")
    else:
        print(f"[ash] fringing_check=clean ({ash['fringing']})")


if __name__ == "__main__":
    main()
