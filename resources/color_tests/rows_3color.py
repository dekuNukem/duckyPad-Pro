import os
import random
from PIL import Image, ImageDraw
import struct
import dp_rgb
import colorsys

def adjust_color(rgb):
	return rgb
    # r, g, b = [x / 255.0 for x in rgb]
    # h, l, s = colorsys.rgb_to_hls(r, g, b)
    # print(h,l,s)
    # l = l * 0.75
    # if l > 1:
    # 	l = 1
    # brightened_r, brightened_g, brightened_b = colorsys.hls_to_rgb(h, l, s)
    # brightened_rgb = tuple(int(x * 255) for x in (brightened_r, brightened_g, brightened_b))
    # return brightened_rgb

def average_color(color1, color2):
    return tuple((c1 + c2) // 2 for c1, c2 in zip(color1, color2))

# green to blue, works really well
# start_color = (255, 0, 0)
# middle_color = (255, 255, 255)
# end_color = (0, 0, 255)

start_color = struct.unpack('BBB', bytes.fromhex("2ECC71"))
middle_color = struct.unpack('BBB', bytes.fromhex("8E44AD"))
end_color = struct.unpack('BBB', bytes.fromhex("F39C12"))

startmiddle_color = average_color(start_color, middle_color)
middleend_color = average_color(middle_color, end_color)

row_color_array = [adjust_color(start_color), adjust_color(startmiddle_color), adjust_color(middle_color), adjust_color(middleend_color), adjust_color(end_color)]

already_done = []

for x in range(20):
	current_row = int(x/4)
	# print(x, current_row)
	this_color = (row_color_array[current_row][0], row_color_array[current_row][1], row_color_array[current_row][2])
	print("SWCOLOR_" + str(x+1), this_color[0], this_color[1], this_color[2])
	already_done.append(this_color)

im = Image.new('RGB', (400,500), (255, 255, 255))
draw = ImageDraw.Draw(im)

# x, y
draw.rectangle(((0, 0), (100, 100)), fill=(already_done[0][0], already_done[0][1], already_done[0][2]))
draw.rectangle(((100, 0), (200, 100)), fill=(already_done[1][0], already_done[1][1], already_done[1][2]))
draw.rectangle(((200, 0), (300, 100)), fill=(already_done[2][0], already_done[2][1], already_done[2][2]))
draw.rectangle(((300, 0), (400, 100)), fill=(already_done[3][0], already_done[3][1], already_done[3][2]))

draw.rectangle(((0, 100), (100, 200)), fill=(already_done[4][0], already_done[4][1], already_done[4][2]))
draw.rectangle(((100, 100), (200, 200)), fill=(already_done[5][0], already_done[5][1], already_done[5][2]))
draw.rectangle(((200, 100), (300, 200)), fill=(already_done[6][0], already_done[6][1], already_done[6][2]))
draw.rectangle(((300, 100), (400, 200)), fill=(already_done[7][0], already_done[7][1], already_done[7][2]))

draw.rectangle(((0, 200), (100, 300)), fill=(already_done[8][0], already_done[8][1], already_done[8][2]))
draw.rectangle(((100, 200), (200, 300)), fill=(already_done[9][0], already_done[9][1], already_done[9][2]))
draw.rectangle(((200, 200), (300, 300)), fill=(already_done[10][0], already_done[10][1], already_done[10][2]))
draw.rectangle(((300, 200), (400, 300)), fill=(already_done[11][0], already_done[11][1], already_done[11][2]))

draw.rectangle(((0, 300), (100, 400)), fill=(already_done[12][0], already_done[12][1], already_done[12][2]))
draw.rectangle(((100, 300), (200, 400)), fill=(already_done[13][0], already_done[13][1], already_done[13][2]))
draw.rectangle(((200, 300), (300, 400)), fill=(already_done[14][0], already_done[14][1], already_done[14][2]))
draw.rectangle(((300, 300), (400, 400)), fill=(already_done[15][0], already_done[15][1], already_done[15][2]))

draw.rectangle(((0, 400), (100, 500)), fill=(already_done[16][0], already_done[16][1], already_done[16][2]))
draw.rectangle(((100, 400), (200, 500)), fill=(already_done[17][0], already_done[17][1], already_done[17][2]))
draw.rectangle(((200, 400), (300, 500)), fill=(already_done[18][0], already_done[18][1], already_done[18][2]))
draw.rectangle(((300, 400), (400, 500)), fill=(already_done[19][0], already_done[19][1], already_done[19][2]))

im.save("image.png", "PNG")

import time

now = time.time()
for x in range(20):
	dp_rgb.set_duckypad_key_color(x, already_done[x][0], already_done[x][1], already_done[x][2])

print(time.time() - now)