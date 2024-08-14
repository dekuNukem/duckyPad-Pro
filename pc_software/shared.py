KEY_NAME_MAX_CHAR_PER_LINE = 5

SW_MATRIX_NUM_COLS = 4
SW_MATRIX_NUM_ROWS = 5
MECH_OBSW_COUNT = (SW_MATRIX_NUM_COLS * SW_MATRIX_NUM_ROWS)
ROTARY_ENCODER_SW_COUNT = 6
ONBOARD_SPARE_GPIO_COUNT = 10
MAX_EXPANSION_MODULE_COUNT = 5
CHANNELS_PER_EXPANSION_MODULE = 6
MAX_EXPANSION_CHANNEL = MAX_EXPANSION_MODULE_COUNT * CHANNELS_PER_EXPANSION_MODULE

"""
0 to 19: mechanical switches
20 to 25: rotary encoders
26 to 35: spare gpio pins, unused
36 to 65: expansion channels
"""

BUTTON_RE1_CW = 20
BUTTON_RE1_CCW = 21
BUTTON_RE1_PUSH = 22
BUTTON_RE2_CW = 23
BUTTON_RE2_CCW = 24
BUTTON_RE2_PUSH = 25

def is_rotary_encoder_button(key_index_start_from_0):
    return BUTTON_RE1_CW <= key_index_start_from_0 <= BUTTON_RE2_PUSH

def is_expansion_button(key_index_start_from_0):
    return 36 <= key_index_start_from_0 <= 65