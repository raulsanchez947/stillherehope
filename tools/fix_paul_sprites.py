from pathlib import Path
from collections import Counter
from collections import deque

from PIL import Image


KEY = (115, 197, 164, 255)


def crop_grid(img: Image.Image, cols: int, rows: int) -> list[Image.Image]:
    cell_w = img.width // cols
    cell_h = img.height // rows
    out = []
    for y in range(rows):
        for x in range(cols):
            out.append(img.crop((x * cell_w, y * cell_h, (x + 1) * cell_w, (y + 1) * cell_h)))
    return out


def trim_to_content(img: Image.Image) -> Image.Image:
    rgba = img.convert("RGBA")
    w, h = rgba.size
    border_pixels = []
    for x in range(w):
        border_pixels.append(rgba.getpixel((x, 0)))
        border_pixels.append(rgba.getpixel((x, h - 1)))
    for y in range(1, h - 1):
        border_pixels.append(rgba.getpixel((0, y)))
        border_pixels.append(rgba.getpixel((w - 1, y)))
    bg_colors = {color for color, _count in Counter(border_pixels).most_common(4)}

    xs = []
    ys = []
    for y in range(h):
        for x in range(w):
            if rgba.getpixel((x, y)) not in bg_colors:
                xs.append(x)
                ys.append(y)
    if not xs:
        return rgba
    return rgba.crop((min(xs), min(ys), max(xs) + 1, max(ys) + 1))


def extract_components(img: Image.Image) -> list[Image.Image]:
    rgba = img.convert("RGBA")
    w, h = rgba.size
    border_pixels = []
    for x in range(w):
        border_pixels.append(rgba.getpixel((x, 0)))
        border_pixels.append(rgba.getpixel((x, h - 1)))
    for y in range(1, h - 1):
        border_pixels.append(rgba.getpixel((0, y)))
        border_pixels.append(rgba.getpixel((w - 1, y)))
    bg_colors = {color for color, _count in Counter(border_pixels).most_common(4)}

    visited = set()
    components = []
    for y in range(h):
        for x in range(w):
            if (x, y) in visited:
                continue
            px = rgba.getpixel((x, y))
            if px in bg_colors:
                visited.add((x, y))
                continue
            queue = deque([(x, y)])
            points = []
            while queue:
                cx, cy = queue.popleft()
                if (cx, cy) in visited:
                    continue
                visited.add((cx, cy))
                if rgba.getpixel((cx, cy)) in bg_colors:
                    continue
                points.append((cx, cy))
                if cx > 0:
                    queue.append((cx - 1, cy))
                if cx < w - 1:
                    queue.append((cx + 1, cy))
                if cy > 0:
                    queue.append((cx, cy - 1))
                if cy < h - 1:
                    queue.append((cx, cy + 1))

            if len(points) < 20:
                continue

            xs = [p[0] for p in points]
            ys = [p[1] for p in points]
            bbox = (min(xs), min(ys), max(xs) + 1, max(ys) + 1)
            comp = Image.new("RGBA", (bbox[2] - bbox[0], bbox[3] - bbox[1]), (0, 0, 0, 0))
            for px_x, px_y in points:
                comp.putpixel((px_x - bbox[0], px_y - bbox[1]), rgba.getpixel((px_x, px_y)))
            components.append((bbox, comp))

    components.sort(key=lambda item: (item[0][1], item[0][0]))
    return [comp for _bbox, comp in components]


def strip_checkerboard(frame: Image.Image) -> Image.Image:
    rgba = frame.convert("RGBA")
    w, h = rgba.size
    border_pixels = []
    for x in range(w):
        border_pixels.append(rgba.getpixel((x, 0)))
        border_pixels.append(rgba.getpixel((x, h - 1)))
    for y in range(1, h - 1):
        border_pixels.append(rgba.getpixel((0, y)))
        border_pixels.append(rgba.getpixel((w - 1, y)))

    # The sprite sheet background is a checkerboard. Use the most common border
    # colors, then remove only pixels connected to the border so we do not carve
    # holes out of the sprite itself.
    bg_colors = {color for color, _count in Counter(border_pixels).most_common(4)}
    visited = set()
    to_clear = set()
    queue = deque()

    for x in range(w):
        queue.append((x, 0))
        queue.append((x, h - 1))
    for y in range(1, h - 1):
        queue.append((0, y))
        queue.append((w - 1, y))

    while queue:
        x, y = queue.popleft()
        if (x, y) in visited:
            continue
        visited.add((x, y))
        px = rgba.getpixel((x, y))
        if px not in bg_colors:
            continue
        to_clear.add((x, y))
        if x > 0:
            queue.append((x - 1, y))
        if x < w - 1:
            queue.append((x + 1, y))
        if y > 0:
            queue.append((x, y - 1))
        if y < h - 1:
            queue.append((x, y + 1))

    out = Image.new("RGBA", rgba.size, (0, 0, 0, 0))
    for y in range(h):
        for x in range(w):
            px = rgba.getpixel((x, y))
            if (x, y) not in to_clear:
                out.putpixel((x, y), px)
    return out


def quantize_with_key(img: Image.Image, colors: int = 16, transparent: bool = False) -> Image.Image:
    rgba = img.convert("RGBA")
    bg = Image.new("RGBA", rgba.size, KEY)
    bg.alpha_composite(rgba)
    flat = bg.convert("RGB")
    q = flat.quantize(colors=colors, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)

    # Force a 16-entry palette and reserve palette slot 0 for the native key color.
    raw = q.getpalette()[:768]
    entries = [tuple(raw[i : i + 3]) for i in range(0, len(raw), 3)]
    used = []
    for color in entries:
        if color not in used:
            used.append(color)
        if len(used) == colors:
            break

    rgb_key = KEY[:3]
    used = [c for c in used if c != rgb_key]
    ordered = [rgb_key] + used[: colors - 1]
    while len(ordered) < colors:
        ordered.append((0, 0, 0))

    pal_img = Image.new("P", (1, 1))
    palette = []
    for color in ordered:
        palette.extend(color)
    palette.extend([0, 0, 0] * (256 - colors))
    pal_img.putpalette(palette)

    remapped = flat.quantize(palette=pal_img, dither=Image.Dither.NONE)
    if transparent:
        remapped.info["transparency"] = 0
    return remapped


def build_overworld(src: Path, dest: Path) -> None:
    img = Image.open(src).convert("RGBA")
    components = extract_components(img)
    if len(components) != 12:
        raise RuntimeError(f"Expected 12 Paul overworld components, found {len(components)}")

    rows = [components[i : i + 3] for i in range(0, 12, 3)]
    front_row = rows[0]
    side_row = rows[1]
    back_row = rows[3]

    # Match the standard 9-frame NPC order:
    # down1, up1, side1, down2, up2, side2, down3, up3, side3
    chosen = []
    for i in range(3):
        chosen.append(front_row[i])
        chosen.append(back_row[i])
        chosen.append(side_row[i])

    strip = Image.new("RGBA", (16 * len(chosen), 32), (0, 0, 0, 0))
    for i, frame in enumerate(chosen):
        resized = frame.resize((16, 32), Image.Resampling.NEAREST)
        strip.alpha_composite(resized, (i * 16, 0))

    quantize_with_key(strip, transparent=False).save(dest)


def build_battle_front(src: Path, dest: Path, pal_dest: Path) -> None:
    img = Image.open(src).convert("RGBA")
    img = img.crop((1, 1, img.width - 2, img.height - 2))
    cells = crop_grid(img, 3, 2)

    # Top-left pose is the cleanest front-facing stance from the supplied sheet.
    front = strip_checkerboard(cells[0])
    resized = front.resize((42, 64), Image.Resampling.NEAREST)

    canvas = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
    x = (64 - resized.width) // 2
    canvas.alpha_composite(resized, (x, 0))

    paletted = quantize_with_key(canvas, transparent=True)
    paletted.save(dest)

    raw = paletted.getpalette()[: 16 * 3]
    lines = ["JASC-PAL", "0100", "16"]
    for i in range(16):
        r, g, b = raw[i * 3 : i * 3 + 3]
        lines.append(f"{r} {g} {b}")
    pal_dest.write_text("\n".join(lines) + "\n", encoding="ascii")


def main() -> None:
    repo = Path("/Users/ivan/Documents/BaneMode_Rocket_Porymap_Work/pokeemerald-rocket-porymap")

    build_overworld(
        Path("/Users/ivan/Downloads/ChatGPT Image Apr 18, 2026 at 09_12_21 PM.png"),
        repo / "graphics/object_events/pics/people/banemode/paul.png",
    )
    # Palette source png is mirrored from the overworld png in this project layout.
    (repo / "graphics/object_events/palettes/banemode/paul.png").write_bytes(
        (repo / "graphics/object_events/pics/people/banemode/paul.png").read_bytes()
    )

    build_battle_front(
        Path("/Users/ivan/Downloads/spritesheet-3.png"),
        repo / "graphics/trainers/front_pics/wally.png",
        repo / "graphics/trainers/palettes/wally.pal",
    )


if __name__ == "__main__":
    main()
