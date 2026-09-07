#!/usr/bin/env python3
"""Convert a plain PBM (P1) image to the optional profile image format."""
import argparse
from pathlib import Path

WIDTH, HEIGHT = 128, 117
HEADER = b"DPIM\x01\x80\x75\x00"


def pack_pbm(text):
    if len(text) > 65536:
        raise ValueError("PBM exceeds 64 KiB")
    tokens = " ".join(line.split("#", 1)[0] for line in text.splitlines()).split()
    if len(tokens) != 3 + WIDTH * HEIGHT or tokens[:3] != ["P1", "128", "117"]:
        raise ValueError("Expected P1 PBM, exactly 128 by 117 pixels")
    data = bytearray(WIDTH * HEIGHT // 8)
    for index, pixel in enumerate(tokens[3:]):
        if pixel not in ("0", "1"):
            raise ValueError("PBM pixels must be 0 (white) or 1 (black)")
        if pixel == "0":
            data[index // 8] |= 0x80 >> (index % 8)
    return HEADER + data


def example_image():
    """A generic outlined grid; no labels, key actions, or personal data."""
    data = bytearray(WIDTH * HEIGHT // 8)
    for y in range(HEIGHT):
        for x in range(WIDTH):
            if x in (0, 32, 64, 96, 127) or y in (0, 22, 46, 69, 92, 116):
                data[y * 16 + x // 8] |= 0x80 >> (x % 8)
    return HEADER + data


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--input", type=Path, help="128x117 plain (P1) PBM")
    source.add_argument("--example", action="store_true", help="generate a generic grid")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        if args.example:
            data = example_image()
        else:
            with args.input.open("r", encoding="ascii") as file:
                text = file.read(65537)
            data = pack_pbm(text)
        # Never silently replace a user's image.
        with args.output.open("xb") as file:
            file.write(data)
    except (OSError, UnicodeError, ValueError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
