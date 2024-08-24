import os
import random
from PIL import Image, ImageDraw
import struct
import dp_rgb


start_color_hex = "937aff"
end_color_hex = "fb8500"

start_color = struct.unpack('BBB', bytes.fromhex(start_color_hex))
end_color = struct.unpack('BBB', bytes.fromhex(end_color_hex))

# green to blue, works really well
start_color = (0, 255, 0)
end_color = (16, 16, 255)

rstep = (end_color[0] - start_color[0]) / 19
gstep = (end_color[1] - start_color[1]) / 19
bstep = (end_color[2] - start_color[2]) / 19

already_done = []

for x in range(20):
	this_color = (int(start_color[0] + rstep * x), int(start_color[1] + gstep * x), int(start_color[2] + bstep * x))
	# print("SWCOLOR_" + str(x+1), this_color[0], this_color[1], this_color[2])
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