import os
import serial
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
esp32_dfu_port = None
for port, desc, hwid in sorted(ports):
	if "303A" in hwid:
		esp32_dfu_port = port

if esp32_dfu_port is None:
	print("ESP32 not found")
	exit()

build_path = "C:\\Users\\allen\\Desktop\\repos\\duckyPad2\\firmware\\dpp_fw\\build\\"
# os.system calls windows cmd prompt
os.system(f"c:\\Users\\allen\\.espressif\\python_env\\idf5.3_py3.11_env\\Scripts\\python.exe C:\\Users\\allen\\esp\\v5.3\\esp-idf\\components\\esptool_py\\esptool\\esptool.py -p {esp32_dfu_port} -b 460800 --before default_reset --after hard_reset --chip esp32s3 write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 {build_path}bootloader\\bootloader.bin 0x10000 {build_path}tusb_hid.bin 0x8000 {build_path}partition_table\\partition-table.bin 0xd000 {build_path}ota_data_initial.bin")
