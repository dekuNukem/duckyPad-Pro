"""
duckyPad HID example: List HID devices

For more details, see:

https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/HID_details.md

"""

import hid

for device_dict in hid.enumerate():
    keys = list(device_dict.keys())
    keys.sort()
    for key in keys:
        print("%s : %s" % (key, device_dict[key]))
    print()