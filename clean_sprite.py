from PIL import Image


INPUT_PATH = "sprite.png"
OUTPUT_PATH = "sprite_clean.png"
TOLERANCE = 15


def within_tolerance(pixel, background, tolerance):
    return all(abs(pixel[i] - background[i]) <= tolerance for i in range(3))


img = Image.open(INPUT_PATH).convert("RGBA")
background = img.getpixel((0, 0))

cleaned_pixels = []
for pixel in img.getdata():
    if within_tolerance(pixel, background, TOLERANCE):
        cleaned_pixels.append((pixel[0], pixel[1], pixel[2], 0))
    else:
        cleaned_pixels.append(pixel)

img.putdata(cleaned_pixels)
img.save(OUTPUT_PATH, "PNG")

print(f"Saved {OUTPUT_PATH}")
