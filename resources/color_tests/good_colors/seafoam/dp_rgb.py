"""
duckyPad HID example: HID write

For more details, see:

https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/HID_details.md

"""

import hid

PC_TO_DUCKYPAD_HID_BUF_SIZE = 64

def get_duckypad_path():
    for device_dict in hid.enumerate():
        if device_dict['vendor_id'] == 0x0483 and \
        device_dict['product_id'] == (0xd11c+1) and \
        device_dict['usage'] == 58:
            return device_dict['path']
    return None

duckypad_path = get_duckypad_path()

if duckypad_path is None:
    print("duckyPad not found!")
    exit()

print("duckyPad Pro found!")
h = hid.device()
h.open_path(duckypad_path)

# h.close()

def set_duckypad_key_color(swid, rrr, ggg, bbb):
    pc_to_duckypad_buf = [0] * PC_TO_DUCKYPAD_HID_BUF_SIZE
    pc_to_duckypad_buf[0] = 5   # HID Usage ID, always 5
    pc_to_duckypad_buf[2] = 4   # Command type

    pc_to_duckypad_buf[3] = swid
    pc_to_duckypad_buf[4] = rrr
    pc_to_duckypad_buf[5] = ggg
    pc_to_duckypad_buf[6] = bbb
    # print("\n\nSending to duckyPad:\n", pc_to_duckypad_buf)
    h.write(pc_to_duckypad_buf)