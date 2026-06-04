"""
duckyPad Pro HID example: mouse control over the custom command interface.

This lets you drive the mouse purely over USB HID from the PC, with no
dependency on the physical buttons / DuckyScript on the device itself.

Commands implemented (must match firmware hid_task.h):

    HID_COMMAND_MOUSE_MOVE_DELTA  (0x05) -> move by a relative delta
    HID_COMMAND_MOUSE_MOVE_STATIC (0x06) -> move to an absolute position
    HID_COMMAND_MOUSE_CLICK       (0x07) -> press (and hold) button(s)
    HID_COMMAND_MOUSE_RELEASE     (0x08) -> release button(s)

Packet layout (PC -> duckyPad), 64 byte buffer:
    [0] HID Report ID, always 5
    [1] Sequence number (ignored by firmware)
    [2] Command type
    [3..] command-specific parameters

For more details, see:
https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/HID_details.md
"""

import time

import hid

PC_TO_DUCKYPAD_HID_BUF_SIZE = 64
DUCKYPAD_TO_PC_HID_BUF_SIZE = 64

# command IDs, keep in sync with firmware/dpp_fw/main/hid_task.h
HID_COMMAND_MOUSE_MOVE_DELTA = 0x05
HID_COMMAND_MOUSE_MOVE_STATIC = 0x06
HID_COMMAND_MOUSE_CLICK = 0x07
HID_COMMAND_MOUSE_RELEASE = 0x08

# button bitmask (matches the HID mouse report)
MOUSE_BUTTON_LEFT = 1 << 0
MOUSE_BUTTON_RIGHT = 1 << 1
MOUSE_BUTTON_MIDDLE = 1 << 2
MOUSE_BUTTON_4 = 1 << 3
MOUSE_BUTTON_5 = 1 << 4

# absolute coordinate range understood by the firmware (Report ID 6)
MOUSE_ABS_MAX = 32767

h = hid.device()


def get_duckypad_path():
	for device_dict in hid.enumerate():
		if device_dict['vendor_id'] == 0x0483 and \
		   device_dict['product_id'] == (0xd11c + 1) and \
		   device_dict['usage'] == 58:
			return device_dict['path']
	return None


# wait up to 0.5 seconds for response
def hid_read():
	read_start = time.time()
	while time.time() - read_start <= 0.5:
		result = h.read(DUCKYPAD_TO_PC_HID_BUF_SIZE)
		if len(result) > 0:
			return result
		time.sleep(0.01)
	return []


def duckypad_hid_write(hid_buf_64b):
	if len(hid_buf_64b) != PC_TO_DUCKYPAD_HID_BUF_SIZE:
		raise ValueError('PC-to-duckyPad buffer wrong size, should be exactly 64 Bytes')
	duckypad_path = get_duckypad_path()
	if duckypad_path is None:
		raise OSError('duckyPad Not Found!')
	h.open_path(duckypad_path)
	h.set_nonblocking(1)
	h.write(hid_buf_64b)
	result = hid_read()
	h.close()
	return result


def _make_buf(command_type):
	buf = [0] * PC_TO_DUCKYPAD_HID_BUF_SIZE
	buf[0] = 5              # HID Report ID, always 5
	buf[1] = 0              # sequence number
	buf[2] = command_type   # command type
	return buf


def _to_int8(value):
	"""Clamp to signed 8-bit and convert to the unsigned byte the wire expects."""
	value = max(-127, min(127, int(value)))
	return value & 0xff


def mouse_move_delta(x, y):
	"""Move the mouse by a relative delta. x/y are clamped to -127..127."""
	buf = _make_buf(HID_COMMAND_MOUSE_MOVE_DELTA)
	buf[3] = _to_int8(x)
	buf[4] = _to_int8(y)
	return duckypad_hid_write(buf)


def mouse_move_static(x, y):
	"""Move the mouse to an absolute position.

	x/y are in the range 0..MOUSE_ABS_MAX, where 0 is the left/top edge and
	MOUSE_ABS_MAX is the right/bottom edge of the screen.
	"""
	x = max(0, min(MOUSE_ABS_MAX, int(x)))
	y = max(0, min(MOUSE_ABS_MAX, int(y)))
	buf = _make_buf(HID_COMMAND_MOUSE_MOVE_STATIC)
	buf[3] = x & 0xff
	buf[4] = (x >> 8) & 0xff
	buf[5] = y & 0xff
	buf[6] = (y >> 8) & 0xff
	return duckypad_hid_write(buf)


def mouse_click(button_mask=MOUSE_BUTTON_LEFT):
	"""Press (and hold) the given mouse button(s)."""
	buf = _make_buf(HID_COMMAND_MOUSE_CLICK)
	buf[3] = button_mask & 0xff
	return duckypad_hid_write(buf)


def mouse_release(button_mask=0):
	"""Release mouse button(s). A mask of 0 releases everything."""
	buf = _make_buf(HID_COMMAND_MOUSE_RELEASE)
	buf[3] = button_mask & 0xff
	return duckypad_hid_write(buf)


def move_to_fraction(fx, fy):
	"""Convenience: move to a fraction (0.0..1.0) of the screen."""
	return mouse_move_static(int(fx * MOUSE_ABS_MAX), int(fy * MOUSE_ABS_MAX))


if __name__ == '__main__':
	print("Moving mouse to the centre of the screen...")
	move_to_fraction(0.5, 0.5)
	time.sleep(0.5)

	print("Nudging right and down a little (relative)...")
	for _ in range(20):
		mouse_move_delta(5, 5)
		time.sleep(0.02)

	print("Left click...")
	mouse_click(MOUSE_BUTTON_LEFT)
	time.sleep(0.1)
	mouse_release(MOUSE_BUTTON_LEFT)

	print("Done.")
