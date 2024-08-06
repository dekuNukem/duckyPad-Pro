import zlib

def calculate_crc32(filename):
    with open(filename, 'rb') as file:
        file_data = file.read()
        crc = zlib.crc32(file_data)
    return crc & 0xFFFFFFFF

result = calculate_crc32("DPP_FW_2.3.4.bin")
print(hex(result))