#!/usr/bin/env python3
"""
Process Paul's overworld and battle sprites for pokeemerald.
"""

import sys
from PIL import Image, ImagePalette
import numpy as np

# ---------------------------------------------------------------------------
# Task 1: Overworld sprite
# ---------------------------------------------------------------------------

OVERWORLD_SRC = "/Users/ivan/Desktop/Paul Sprite overworld.jpg"
OVERWORLD_OUT = (
    "/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration"
    "/graphics/object_events/pics/people/paul.png"
)
PAL_OUT = (
    "/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration"
    "/graphics/object_events/pics/people/paul.pal"
)

# Frame regions in the source image (x_start, x_end, y_start, y_end)
# 3 columns at x=86-135, 136-185, 186-235  (col width ~50px)
# South rows 0-60, North rows 63-120, West rows 123-178
FRAME_COLS = [(86, 136), (136, 186), (186, 236)]
FRAME_ROWS = [
    (0, 61),    # south
    (63, 121),  # north
    (123, 179), # west
]
# Frame order: south0, south1, south2, north0, north1, north2, west0, west1, west2
# i.e. row-major: for each direction row, for each column

FRAME_SIZE = (16, 32)  # output size per frame
MAGENTA = (255, 0, 255)


def is_background(r, g, b):
    """Green-screen removal: pixel is bg if g > r+15 and g > b+15 and g > 50."""
    return g > r + 15 and g > b + 15 and g > 50


def extract_frames(src_img):
    """Extract 9 frames from the source image, remove background, resize to 16x32."""
    src = np.array(src_img.convert("RGB"))
    frames = []
    for row_range in FRAME_ROWS:
        for col_range in FRAME_COLS:
            y0, y1 = row_range
            x0, x1 = col_range
            cell = src[y0:y1, x0:x1].copy()
            # Replace green background with magenta
            r, g, b = cell[:, :, 0], cell[:, :, 1], cell[:, :, 2]
            mask = (g.astype(int) > r.astype(int) + 15) & \
                   (g.astype(int) > b.astype(int) + 15) & \
                   (g > 50)
            cell[mask] = [255, 0, 255]
            # Convert to PIL and resize
            frame_img = Image.fromarray(cell, "RGB")
            frame_img = frame_img.resize(FRAME_SIZE, Image.LANCZOS)
            frames.append(frame_img)
    return frames  # list of 9 RGB PIL images, 16x32 each


def build_gba_output(frames):
    """
    Build the 144x32 GBA-scrambled tile layout.
    Tile index n → position x=(n%18)*8, y=(n//18)*8
    Each frame f occupies tiles f*8 .. f*8+7 (2 wide × 4 tall = 16x32)
    """
    out = Image.new("RGB", (144, 32), MAGENTA)
    for f, frame in enumerate(frames):
        frame_arr = np.array(frame)
        for t in range(8):
            tile_row = t // 2
            tile_col = t % 2
            # pixel offset within frame
            px = tile_col * 8
            py = tile_row * 8
            abs_tile = f * 8 + t
            out_x = (abs_tile % 18) * 8
            out_y = (abs_tile // 18) * 8
            # Extract 8x8 tile from frame
            tile = frame_arr[py:py+8, px:px+8]
            tile_img = Image.fromarray(tile, "RGB")
            out.paste(tile_img, (out_x, out_y))
    return out


def quantize_overworld(rgb_img):
    """
    Quantize to indexed with max 16 colors, index 0 = magenta.
    Returns PIL Image in mode 'P'.
    """
    # Find all unique colors, forcing magenta first
    arr = np.array(rgb_img)
    h, w, _ = arr.shape

    # Collect unique colors (excluding magenta)
    pixels = arr.reshape(-1, 3)
    unique_colors = set()
    for px in pixels:
        c = tuple(px)
        if c != MAGENTA:
            unique_colors.add(c)

    unique_colors = list(unique_colors)

    # Quantize using PIL's built-in quantizer on non-magenta pixels
    # We'll use a mask approach: quantize the whole image to 15 colors, then prepend magenta
    # First, replace magenta with a placeholder that won't appear naturally
    placeholder = (254, 0, 254)
    arr2 = arr.copy()
    mask = (arr2[:, :, 0] == 255) & (arr2[:, :, 1] == 0) & (arr2[:, :, 2] == 255)
    arr2[mask] = placeholder

    img2 = Image.fromarray(arr2, "RGB")
    # Quantize to 15 colors (we'll add magenta as color 0)
    quantized = img2.quantize(colors=15, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)

    # Get the palette
    palette_data = quantized.getpalette()  # 768 ints (256 colors * 3)
    q_arr = np.array(quantized)

    # Build new 16-color palette: index 0 = magenta, indices 1-15 = quantized colors
    new_palette = list(MAGENTA)  # index 0
    # Collect unique palette entries used by the quantized image
    used_indices = set(np.unique(q_arr))
    # Add the 15 quantized colors
    for i in range(15):
        r = palette_data[i * 3]
        g = palette_data[i * 3 + 1]
        b = palette_data[i * 3 + 2]
        new_palette.extend([r, g, b])
    # Pad to 256 colors
    while len(new_palette) < 256 * 3:
        new_palette.extend([0, 0, 0])

    # Build index map: old index → new index (shifted by 1)
    # But first rebuild q_arr so magenta pixels → 0, others → old+1 (clamped to 15)
    # Find which quantized index maps to the placeholder color
    placeholder_idx = None
    for i in range(256):
        r = palette_data[i * 3]
        g = palette_data[i * 3 + 1]
        b = palette_data[i * 3 + 2]
        if (r, g, b) == placeholder:
            placeholder_idx = i
            break

    new_arr = np.zeros((h, w), dtype=np.uint8)
    for y in range(h):
        for x in range(w):
            old_idx = q_arr[y, x]
            if placeholder_idx is not None and old_idx == placeholder_idx:
                new_arr[y, x] = 0
            elif mask[y, x]:
                new_arr[y, x] = 0
            else:
                new_arr[y, x] = min(old_idx + 1, 15)

    result = Image.fromarray(new_arr, "P")
    result.putpalette(new_palette)
    return result, new_palette


def save_jasc_pal(palette_data, filepath, num_colors=16):
    """Save palette in JASC-PAL format."""
    lines = ["JASC-PAL", "0100", str(num_colors)]
    for i in range(num_colors):
        r = palette_data[i * 3]
        g = palette_data[i * 3 + 1]
        b = palette_data[i * 3 + 2]
        lines.append(f"{r} {g} {b}")
    with open(filepath, "w") as f:
        f.write("\n".join(lines) + "\n")


def process_overworld():
    print("=== Task 1: Overworld sprite ===")
    src_img = Image.open(OVERWORLD_SRC)
    print(f"Source: {src_img.size} {src_img.mode}")

    frames = extract_frames(src_img)
    print(f"Extracted {len(frames)} frames (each {FRAME_SIZE})")

    rgb_out = build_gba_output(frames)
    print(f"GBA layout image: {rgb_out.size}")

    indexed, palette_data = quantize_overworld(rgb_out)
    print(f"Indexed image: {indexed.size} mode={indexed.mode}")

    # Verify color count
    used = set(np.array(indexed).flatten())
    print(f"Unique palette indices used: {sorted(used)} ({len(used)} colors)")

    indexed.save(OVERWORLD_OUT)
    print(f"Saved overworld PNG: {OVERWORLD_OUT}")

    save_jasc_pal(palette_data, PAL_OUT)
    print(f"Saved palette: {PAL_OUT}")


# ---------------------------------------------------------------------------
# Task 2: Battle sprite
# ---------------------------------------------------------------------------

BATTLE_SRC = "/Users/ivan/Desktop/Paul Battle presentation.png"
BATTLE_OUT = (
    "/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration"
    "/graphics/trainers/front_pics/wally.png"
)
WALLY_PAL_OUT = (
    "/Users/ivan/Documents/New project/BaneMode_QuestEngine_Migration"
    "/graphics/trainers/palettes/wally.pal"
)


def process_battle():
    print("\n=== Task 2: Battle sprite ===")
    src_img = Image.open(BATTLE_SRC)
    print(f"Source: {src_img.size} {src_img.mode}")

    # Resize to 64x64
    resized = src_img.resize((64, 64), Image.LANCZOS)
    print(f"Resized: {resized.size}")

    # Separate alpha from RGB
    rgba_arr = np.array(resized.convert("RGBA"))
    alpha = rgba_arr[:, :, 3]
    rgb = rgba_arr[:, :, :3]

    # Find transparent pixels (alpha < 128)
    transparent_mask = alpha < 128

    # Quantize non-transparent pixels to 15 colors
    # Replace transparent pixels with a placeholder
    placeholder_color = (1, 1, 1)
    rgb_work = rgb.copy()
    rgb_work[transparent_mask] = placeholder_color

    work_img = Image.fromarray(rgb_work, "RGB")
    quantized = work_img.quantize(colors=15, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)

    palette_data = quantized.getpalette()
    q_arr = np.array(quantized)
    h, w = q_arr.shape

    # Find placeholder index
    placeholder_idx = None
    for i in range(256):
        r = palette_data[i * 3]
        g = palette_data[i * 3 + 1]
        b = palette_data[i * 3 + 2]
        if (r, g, b) == placeholder_color:
            placeholder_idx = i
            break

    # Build new 16-color palette: index 0 = transparent (0,0,0 for battle sprites)
    new_palette = [0, 0, 0]  # index 0 = background/transparent
    for i in range(15):
        r = palette_data[i * 3]
        g = palette_data[i * 3 + 1]
        b = palette_data[i * 3 + 2]
        new_palette.extend([r, g, b])
    while len(new_palette) < 256 * 3:
        new_palette.extend([0, 0, 0])

    # Build index array
    new_arr = np.zeros((h, w), dtype=np.uint8)
    for y in range(h):
        for x in range(w):
            if transparent_mask[y, x]:
                new_arr[y, x] = 0
            else:
                old_idx = q_arr[y, x]
                if placeholder_idx is not None and old_idx == placeholder_idx:
                    new_arr[y, x] = 0
                else:
                    new_arr[y, x] = min(old_idx + 1, 15)

    result = Image.fromarray(new_arr, "P")
    result.putpalette(new_palette)

    used = set(new_arr.flatten())
    print(f"Unique palette indices used: {sorted(used)} ({len(used)} colors)")

    result.save(BATTLE_OUT)
    print(f"Saved battle PNG: {BATTLE_OUT}")

    save_jasc_pal(new_palette, WALLY_PAL_OUT)
    print(f"Saved wally palette: {WALLY_PAL_OUT}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    process_overworld()
    process_battle()
    print("\nDone.")
