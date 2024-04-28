"""
duckyPad HID example: HID write

For more details, see:

https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/HID_details.md

"""

import hid

PC_TO_DUCKYPAD_HID_BUF_SIZE = 64

h = hid.device()

def get_duckypad_path():
    path_dict = {}
    for device_dict in hid.enumerate():
        if device_dict['vendor_id'] == 12346 and device_dict['product_id'] == 16388:
            path_dict[device_dict['usage']] = device_dict['path']
    if len(path_dict) == 0:
        return None
    if 58 in path_dict:
        return path_dict[58]
    return list(path_dict.values())[0]

pc_to_duckypad_buf = []
for x in range(PC_TO_DUCKYPAD_HID_BUF_SIZE):
    pc_to_duckypad_buf.append(x)

# pc_to_duckypad_buf = [0] * PC_TO_DUCKYPAD_HID_BUF_SIZE
pc_to_duckypad_buf[0] = 5   # HID Usage ID, always 5
# pc_to_duckypad_buf[1] = 0   # Sequence Number
# pc_to_duckypad_buf[2] = 3   # Command type

# print("----\nsudo is needed on Linux!\n----")

duckypad_path = get_duckypad_path()

if duckypad_path is None:
    print("duckyPad not found!")
    exit()

print("duckypad2 found!")
h.open_path(duckypad_path)
h.set_nonblocking(1)
print("\n\nSending to duckyPad:\n", pc_to_duckypad_buf)
h.write(pc_to_duckypad_buf)
h.close()
print('done!')
