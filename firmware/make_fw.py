import os
import sys
import shutil
import zlib

def calculate_crc32(filename):
    with open(filename, 'rb') as file:
        file_data = file.read()
        crc = zlib.crc32(file_data)
    return crc & 0xFFFFFFFF

if sys.platform != 'win32':
	print(__file__ + ": not under windows")
	exit()

main_c_path = os.path.join(".", 'dpp_fw')
main_c_path = os.path.join(main_c_path, 'main')
main_c_path = os.path.join(main_c_path, 'main.c')
main_c_file = open(main_c_path)
main_c_lines = main_c_file.readlines()
main_c_file.close()

fw_ver_major = None
fw_ver_minor = None
fw_ver_patch = None

for cline in main_c_lines:
	cline = cline.strip().strip("\r\n")
	if 'fw_version_major' in cline:
		fw_ver_major = int(cline.split('=')[-1].split(';')[0].strip())
	if 'fw_version_minor' in cline:
		fw_ver_minor = int(cline.split('=')[-1].split(';')[0].strip())
	if 'fw_version_patch' in cline:
		fw_ver_patch = int(cline.split('=')[-1].split(';')[0].strip())

# print(fw_ver_major, fw_ver_minor, fw_ver_patch)

if(fw_ver_major is None or fw_ver_minor is None or fw_ver_patch is None):
	print("fw version parsing failed")
	exit()

fw_bin_path = os.path.join("", 'dpp_fw')
fw_bin_path = os.path.join(fw_bin_path, 'build')
fw_bin_path = os.path.join(fw_bin_path, 'tusb_hid.bin')

if os.path.exists(fw_bin_path) is False:
	print(fw_bin_path, "not found")
	exit()

fw_crc = calculate_crc32(fw_bin_path)

new_filename = f"DPP_FW_{fw_ver_major}.{fw_ver_minor}.{fw_ver_patch}_{fw_crc:x}.bin"
new_fw_path = os.path.join(".", new_filename)
shutil.copy2(fw_bin_path, new_fw_path)
print("Copied", new_fw_path)