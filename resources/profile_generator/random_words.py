import os
import sys
import string
import shutil
import random

profile_number = 99
MAX_SW_COUNT = 20

root_name = 'duck_test'

if os.path.isdir(root_name):
	shutil.rmtree(root_name)
os.mkdir(root_name)

color_pool = [8, 16, 32, 64]
word_list = [
    "bake", "zoom", "lamp", "fish", "rope", "wave", "desk", "jump", "rock", "mint",
    "flag", "wish", "sock", "farm", "gift", "lamp", "deck", "song", "duck", "tree",
    "bell", "wind", "fire", "bold", "link", "fist", "pile", "rush", "book", "nest",
    "dove", "mail", "hunt", "wave", "salt", "leaf", "ring", "horn", "pack", "veil",
    "fork", "line", "gold", "sail", "clip", "crab", "wolf", "veil", "seem", "clay",
    "cat", "dog", "pig", "bat", "rat", "cow", "bee", "ant", "owl", "hen",
    "fox", "sun", "car", "bus", "van", "map", "pen", "box", "cup", "key",
    "top", "hat", "log", "web", "net", "zip", "fan", "jam", "pot", "pan",
    "bag", "rug", "wig", "lip", "cap", "tip", "toy", "gum", "rod", "kit",
    "cut", "mud", "dot", "bay", "bud", "tap", "pop", "fin", "paw", "bed",
    "apple", "bread", "crisp", "dance", "eagle", "flame", "grape", "haste", "ivory", "jelly",
    "knack", "lemon", "magic", "noble", "ocean", "pearl", "quilt", "roast", "shade", "twist",
    "umbra", "valve", "whale", "xenon", "yacht", "zebra", "brush", "charm", "drift", "eager",
    "frost", "globe", "hover", "index", "jolly", "kneel", "lodge", "mango", "niche", "opera",
    "piano", "quark", "rider", "sheep", "tower", "ultra", "vivid", "waltz", "xerox", "youth",
    "am", "an", "as", "at", "be", "by", "do", "go", "he", "if",
    "in", "is", "it", "me", "my", "no", "of", "on", "or", "so",
]

word_list = list(set(word_list))

def get_random_name():
	return random.choice(word_list)

def get_random_colors():
	ret = ''
	for item in random.choices(color_pool, k=3):
		ret += str(item) + ' '
	return ret

root_dir = os.path.join(".", root_name)
for x in range(0, profile_number + 1):
	if random.randint(0, 1):
		continue
	profile_dir_name = os.path.join(root_dir, 'profile' + str(x) + "_" + get_random_name()[:13])
	os.mkdir(profile_dir_name)
	config_name = os.path.join(profile_dir_name, 'config.txt')
	config_content = ''
	for y in range(1, MAX_SW_COUNT+1):
		config_content += 'SWNAMEFL_' + str(y) + ' ' + get_random_name()[:5] + "\n"
		if random.randint(0, 1):
			config_content += 'SWNAMESL_' + str(y) + ' ' + get_random_name()[:5] + "\n"
	for y in range(1, MAX_SW_COUNT+1):
		config_content += 'SWCOLOR_' + str(y) + ' ' + get_random_colors() + "\n"
	# print(config_content)

	with open(config_name, 'w') as config_file:
		config_file.write(config_content)

	for y in range(1, MAX_SW_COUNT+1):
		script_name = os.path.join(profile_dir_name, 'key' + str(y) + '.txt')
		script_content = 'STRING ' + get_random_name() + '\nENTER\n'
		with open(script_name, 'w') as script_file:
			script_file.write(script_content)
