import sys
from PIL import Image

if len(sys.argv) != 2:
    print("specify the buffer file")
    sys.exit(1)

filename = sys.argv[1]

try:
	file_split = filename.split("_")
	SSD1306_WIDTH = int(file_split[-2])
	SSD1306_HEIGHT = int(file_split[-1].lower().replace(".bin", ''))
except Exception as e:
	print("error parsing file name:", e)
	exit()

SSD1306_BUFFER_SIZE = SSD1306_WIDTH * SSD1306_HEIGHT // 8

with open(filename, 'rb') as file:
    oled_buf = file.read()

if len(oled_buf) != SSD1306_BUFFER_SIZE:
	print("size mismatch")
	exit()

def get_pixel(x, y, buf):
    if x >= SSD1306_WIDTH or y >= SSD1306_HEIGHT:
        return 0
    
    position = x + (y // 8) * SSD1306_WIDTH
    bit = y % 8
    
    if buf[position] & (1 << bit):
        return 1 # white
    return 0 # black

image = Image.new('1', (SSD1306_WIDTH, SSD1306_HEIGHT), "black")  # Create a new black image
pixels = image.load()

for y in range(SSD1306_HEIGHT):
    for x in range(SSD1306_WIDTH):
    	pixels[x, y] = get_pixel(x, y, oled_buf);

image.save("output.png")