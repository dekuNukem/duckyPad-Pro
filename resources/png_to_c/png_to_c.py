from PIL import Image


img = Image.open("image.png")
# img = img.convert('1')  # Ensure the image is in 1-bit mode

# Get image dimensions
width, height = img.size

pixels = img.getdata()

# print(width, height)

for y in range(height):
    for x in range(width):
    	this_pixel = pixels[y*width+x]
    	# print(x, y, this_pixel, this_pixel == (0,0,0,0))
    	if this_pixel != (0,0,0,0):
    		print(f"ssd1306_DrawPixel(origx+{x}, origy+{y}, WHITE);")

