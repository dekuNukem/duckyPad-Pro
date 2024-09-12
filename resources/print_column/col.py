from columnar import columnar

col1 = ["CTRL / RCTRL",\
"SHIFT / RSHIFT",\
"ALT / RALT",\
"WINDOWS / RWINDOWS",\
"COMMAND / RCOMMAND ",\
"OPTION / ROPTION",\
"ESC",\
"ENTER",\
"UP/DOWN/LEFT/RIGHT",\
"SPACE",\
"BACKSPACE",\
"TAB",\
"CAPSLOCK",\
"PRINTSCREEN",\
"SCROLLLOCK",\
"PAUSE",\
"BREAK",\
"INSERT",\
"HOME",\
"PAGEUP / PAGEDOWN",\
"DELETE",\
"END",\
"MENU",\
"POWER",\
"F1 to F24"]

col2 = ["(media keys)",\
"MK_VOLUP",\
"MK_VOLDOWN",\
"MK_MUTE",\
"MK_PREV",\
"MK_NEXT",\
"MK_PP (play/pause)",\
"MK_STOP",\
"",\
"(numpad keys)",\
"NUMLOCK",\
"KP_SLASH",\
"KP_ASTERISK",\
"KP_MINUS",\
"KP_PLUS",\
"KP_ENTER",\
"KP_0 to KP_9",\
"KP_DOT",\
"KP_EQUAL",\
"",\
"(Japanese input method)",\
"ZENKAKUHANKAKU",\
"HENKAN",\
"MUHENKAN",\
"KATAKANAHIRAGANA"]

data = []

for index, col1_item in enumerate(col1):
	col2_item = 'opps!'
	if index < len(col2):
		col2_item = col2[index]
	data.append([col1_item, '|' , col2_item])

print(data)
table = columnar(data, None, no_borders=True, min_column_width=4)
print(table)
