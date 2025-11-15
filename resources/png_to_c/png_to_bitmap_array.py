#!/usr/bin/env python3
"""
png2c.py - Convert a 1-bit black & white PNG into a C bitmap array
compatible with ssd1306_DrawBitmap().

Usage:
    python png2c.py input.png array_name > output.c

Optional:
    Add "--invert" if you want white pixels to be '1' bits instead of black.

Example:
    python png2c.py logo.png logo_bitmap > logo.c
"""

import sys
from PIL import Image


def png_to_c_array(path, array_name, invert=False):
    # Open image and ensure 1-bit mode
    img = Image.open(path).convert("1")
    width, height = img.size

    # Get pixel access
    pixels = img.load()

    byte_width = (width + 7) // 8
    data = []

    # Pack bits row by row, left to right, MSB first
    for y in range(height):
        for bx in range(byte_width):
            byte = 0
            for bit in range(8):
                x = bx * 8 + bit
                byte <<= 1
                if x < width:
                    # In 1-bit images: 0 = black, 255 = white
                    pixel = pixels[x, y]
                    on = (pixel == 0)  # black is 'on'
                    if invert:
                        on = not on
                    if on:
                        byte |= 0x01
            data.append(byte)

    c_macro_height = f"{array_name.upper()}_HEIGHT"
    c_macro_width = f"{array_name.upper()}_WIDTH"

    # Print C array
    print(f"// Converted from: {path}")
    print(f"// Size: {width}x{height} pixels")
    print()
    print(f"#define {c_macro_height} ({height})")
    print(f"#define {c_macro_width} ({width})")
    print(f"const unsigned char {array_name}[] = {{")

    # Format: 12 bytes per line
    bytes_per_line = 12
    for i in range(0, len(data), bytes_per_line):
        chunk = data[i:i + bytes_per_line]
        line = ", ".join(f"0x{b:02X}" for b in chunk)
        print(f"    {line},")
    print("};")
    print()
    print(f"ssd1306_DrawBitmap(x, y, {array_name}, {c_macro_width}, {c_macro_height}, White);")

def main():
    if len(sys.argv) < 3:
        print("Usage: python png2c.py input.png array_name [--invert]", file=sys.stderr)
        sys.exit(1)

    path = sys.argv[1]
    array_name = sys.argv[2]
    invert = "--invert" in sys.argv[3:]

    png_to_c_array(path, array_name, invert=invert)


if __name__ == "__main__":
    main()
