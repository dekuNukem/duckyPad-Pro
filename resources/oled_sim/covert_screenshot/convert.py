import os
from PIL import Image

SSD1306_WIDTH = 128
SSD1306_HEIGHT = 128
SSD1306_BUFFER_SIZE = SSD1306_WIDTH * SSD1306_HEIGHT // 8

def get_pixel(x, y, buf):
    if x >= SSD1306_WIDTH or y >= SSD1306_HEIGHT:
        return 0
    
    position = x + (y // 8) * SSD1306_WIDTH
    bit = y % 8
    
    if buf[position] & (1 << bit):
        return 1  # white
    return 0  # black

def convert_bin_to_png(filename):
    with open(filename, 'rb') as file:
        oled_buf = file.read()

    if len(oled_buf) != SSD1306_BUFFER_SIZE:
        print(f"Size mismatch for {filename}")
        return

    image = Image.new('1', (SSD1306_WIDTH, SSD1306_HEIGHT), "black")  # Create a new black image
    pixels = image.load()

    for y in range(SSD1306_HEIGHT):
        for x in range(SSD1306_WIDTH):
            pixels[x, y] = get_pixel(x, y, oled_buf)

    output_filename = f"{os.path.splitext(filename)[0]}.png"
    image.save(output_filename)
    print(f"Converted {filename} to {output_filename}")


for filename in os.listdir('.'):
    if filename.endswith('.bin'):
        convert_bin_to_png(filename)