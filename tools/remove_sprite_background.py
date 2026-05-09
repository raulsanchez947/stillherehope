#!/usr/bin/env python3
from __future__ import annotations

import argparse
from collections import deque
from pathlib import Path

from PIL import Image


def color_distance_sq(a: tuple[int, int, int], b: tuple[int, int, int]) -> int:
    return sum((av - bv) * (av - bv) for av, bv in zip(a, b))


def darkest_corner_color(img: Image.Image) -> tuple[int, int, int]:
    w, h = img.size
    corners = [
        img.getpixel((0, 0))[:3],
        img.getpixel((w - 1, 0))[:3],
        img.getpixel((0, h - 1))[:3],
        img.getpixel((w - 1, h - 1))[:3],
    ]
    return min(corners, key=lambda rgb: sum(rgb))


def remove_background(
    src: Path,
    dest: Path,
    tolerance: int = 10,
    background: tuple[int, int, int] | None = None,
) -> None:
    img = Image.open(src).convert("RGBA")
    w, h = img.size
    bg = background or darkest_corner_color(img)
    threshold_sq = tolerance * tolerance

    def is_background(x: int, y: int) -> bool:
        r, g, b, _a = img.getpixel((x, y))
        return color_distance_sq((r, g, b), bg) <= threshold_sq

    visited: set[tuple[int, int]] = set()
    clear: set[tuple[int, int]] = set()
    queue: deque[tuple[int, int]] = deque()

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
        if not is_background(x, y):
            continue

        clear.add((x, y))
        if x > 0:
            queue.append((x - 1, y))
        if x < w - 1:
            queue.append((x + 1, y))
        if y > 0:
            queue.append((x, y - 1))
        if y < h - 1:
            queue.append((x, y + 1))

    out = img.copy()
    for x, y in clear:
        r, g, b, _a = out.getpixel((x, y))
        out.putpixel((x, y), (r, g, b, 0))

    dest.parent.mkdir(parents=True, exist_ok=True)
    out.save(dest, "PNG")
    print(f"Background color: #{bg[0]:02x}{bg[1]:02x}{bg[2]:02x}")
    print(f"Cleared pixels: {len(clear)}")
    print(f"Saved: {dest}")


def parse_hex_color(value: str) -> tuple[int, int, int]:
    text = value.strip().removeprefix("#")
    if len(text) != 6:
        raise argparse.ArgumentTypeError("expected a color like #000000")
    try:
        return tuple(int(text[i : i + 2], 16) for i in (0, 2, 4))
    except ValueError as exc:
        raise argparse.ArgumentTypeError("expected a color like #000000") from exc


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Remove a near-black, edge-connected sprite sheet background."
    )
    parser.add_argument("input", type=Path, help="Source sprite sheet PNG")
    parser.add_argument("output", type=Path, help="Transparent output PNG")
    parser.add_argument(
        "--tolerance",
        type=int,
        default=10,
        help="RGB distance tolerance from the sampled background color",
    )
    parser.add_argument(
        "--background",
        type=parse_hex_color,
        default=None,
        help="Override the sampled darkest corner color, for example #000000",
    )
    args = parser.parse_args()

    remove_background(args.input, args.output, args.tolerance, args.background)


if __name__ == "__main__":
    main()
