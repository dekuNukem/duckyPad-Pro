import os
import hid
import time
import sys
import psutil

HID_WAIT_TIME = float(os.getenv('HID_WAIT_TIME', 0))

if 'win32' in sys.platform:
    import win32api

def get_duckypad_path():
    if 'win32' in sys.platform:
        for device_dict in hid.enumerate():
            if device_dict['vendor_id'] == 0x0483 and \
            device_dict['product_id'] == (0xd11c+1) and \
            device_dict['usage'] == 58:
                return device_dict['path']
    else:
        for device_dict in hid.enumerate():
            if device_dict['vendor_id'] == 0x0483 and \
            device_dict['product_id'] == (0xd11c+1):
                return device_dict['path']
    return None

PC_TO_DUCKYPAD_HID_BUF_SIZE = 64
DUCKYPAD_TO_PC_HID_BUF_SIZE = 64

HID_RESPONSE_OK = 0
HID_RESPONSE_ERROR = 1
HID_RESPONSE_BUSY = 2
HID_RESPONSE_EOF = 3

HID_COMMAND_SW_RESET = 20

h = hid.device()

def _read_duckypad():
    """
    Read from the duckyPad & discard the result if we got a result that is
    shorter than a single byte.
    """
    read_start = time.time()
    while time.time() - read_start <= 1:
        result = h.read(DUCKYPAD_TO_PC_HID_BUF_SIZE)
        if len(result) > 1:
            return result
        time.sleep(0.005)
    return []

def duckypad_hid_init():
    duckypad_path = get_duckypad_path()
    if duckypad_path is None:
        raise OSError('duckyPad Not Found!')
    h.open_path(duckypad_path)

def duckypad_hid_close():
    h.close()

def get_dp_info():
    pc_to_duckypad_buf = [0] * PC_TO_DUCKYPAD_HID_BUF_SIZE
    pc_to_duckypad_buf[0] = 5   # HID Usage ID, always 5
    try:
        duckypad_hid_close()
        duckypad_hid_init()
        h.write(pc_to_duckypad_buf)
        result = _read_duckypad()
    except Exception as e:
        print("get_dp_info:", e)
        return None
    return result

def is_dp_ready():
    dp_info = get_dp_info()
    if dp_info is None:
        return False, 'duckyPad not Found!'
    if dp_info[2] == 0:
        return True, 'All good!'
    return False, 'duckyPad is busy!\n\nMake sure no script is running.'

def duckypad_hid_sw_reset(reboot_into_usb_msc_mode=False):
    pc_to_duckypad_buf = [0] * PC_TO_DUCKYPAD_HID_BUF_SIZE
    pc_to_duckypad_buf[0] = 5   # HID Usage ID, always 5
    pc_to_duckypad_buf[2] = HID_COMMAND_SW_RESET    # Command type
    if(reboot_into_usb_msc_mode):
        pc_to_duckypad_buf[3] = 1 
    h.write(pc_to_duckypad_buf)

def get_duckypad_drive(vol_str):
    removable_drives = [x for x in psutil.disk_partitions() if 'removable' in x.opts.lower()]
    if len(removable_drives) == 0:
        return None
    for item in removable_drives:
        vol_label = win32api.GetVolumeInformation(item.mountpoint)[0]
        if vol_str.strip().lower() in vol_label.strip().lower():
            return item.mountpoint
    return None

result = get_duckypad_drive("DP24_9BB0")
print(result)

# duckypad_hid_init()
# print(is_dp_ready())
# duckypad_hid_sw_reset()