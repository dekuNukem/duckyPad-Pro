import sys
from PIL import Image
import os

if len(sys.argv) != 2:
    print("Usage: python script.py <image_path>")
    sys.exit(1)

input_filename = sys.argv[1]

# Open the image
try:
    image = Image.open(input_filename)
except FileNotFoundError:
    print(f"File not found: {input_filename}")
    sys.exit(1)

# Get the original dimensions
original_width, original_height = image.size

# Calculate the new dimensions (4x the size)
new_width = original_width * 4
new_height = original_height * 4

# Resize the image
resized_image = image.resize((new_width, new_height), Image.NEAREST)

# Create the output filename
output_filename = f"out_{os.path.basename(input_filename)}"

# Save the resized image
resized_image.save(output_filename)

print(f"Resized image saved as {output_filename}")