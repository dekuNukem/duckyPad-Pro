
def print_both(text):
    short_path = Path(*filepath.parts[-2:])
    print(text)
    print(short_path)
    # tk_strvar.set(short_path)
    # tk_root.update()

# if len(sys.argv) != 3:
#     print(__file__, "sd_dir modified_dir")
#     exit()

# sd_dir = sys.argv[1]
# modified_dir = sys.argv[2]

# duckypad_file_sync(sd_dir, modified_dir)


[-50:]
[-50:]
[-50:]
[-50:]
[-50:]
duckypad_file_whitelist = [
    "profile",
    "config",
    "key",
]

duckypad_file_blacklist = [
    "keymaps",
]

def is_duckypad_file(name):
    return True
    for item in duckypad_file_blacklist:
        if item.lower().strip() in name.lower().strip():
            return False
    for item in duckypad_file_whitelist:
        if item.lower().strip() in name.lower().strip():
            return True
    return False

for i in range(HOW_MANY_NOP_AFTER_VMVAR):
        assembly_listing.append(get_empty_instruction())
def abort_allowed_click():
    print('hello!')

is_abort_allowed_var = IntVar()
abort_allowed_checkbox = Checkbutton(scripts_lf, text="Press key\nto abort", variable=is_abort_allowed_var, command=abort_allowed_click)
abort_allowed_checkbox.place(x=scaled_size(220), y=scaled_size(410))

update_key_button_appearances(profile_index)
, font=(None, 13)
if profile_list[index].dim_unused:
        dim_unused_keys_checkbox.select()
    else:
        dim_unused_keys_checkbox.deselect()

    if profile_list[index].is_landscape:
        rotate_keys_checkbox.select()
    else:
        rotate_keys_checkbox.deselect()

if bool(is_in_landscape_var.get()):
        place_obsw_buttons_landscape()
    else:
        place_obsw_buttons_portrait()
key_char_limit_portrait = "max 2 lines\n5 char per line"
key_char_limit_landscape = "max 2 lines\n4 char per line"

key_char_limit_label = Label(master=name_editor_lf, fg='grey')
key_char_limit_label.place(x=scaled_size(200), y=scaled_size(0))
root.update()
rotate_keys_click

update_key_button_appearances, disable to see expansion button default labels

ch_button_x = 45
CH_BUTTON_WIDTH = 85

EXPCH0_button = Button(expansion_lf, text="CH0", relief="solid")
EXPCH0_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 0 - 2), width=CH_BUTTON_WIDTH, height=25)

EXPCH1_button = Button(expansion_lf, text="CH1", relief="solid")
EXPCH1_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 1 - 2), width=CH_BUTTON_WIDTH, height=25)

EXPCH2_button = Button(expansion_lf, text="CH2", relief="solid")
EXPCH2_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 2 - 2), width=CH_BUTTON_WIDTH, height=25)

EXPCH3_button = Button(expansion_lf, text="CH3", relief="solid")
EXPCH3_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 3 - 2), width=CH_BUTTON_WIDTH, height=25)

EXPCH4_button = Button(expansion_lf, text="CH4", relief="solid")
EXPCH4_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 4 - 2), width=CH_BUTTON_WIDTH, height=25)

EXPCH5_button = Button(expansion_lf, text="CH5", relief="solid")
EXPCH5_button.place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 5 - 2), width=CH_BUTTON_WIDTH, height=25)




# expansion module buttons
for mmm in range(MAX_EXPANSION_MODULE_COUNT):
    for ccc in range(CHANNELS_PER_EXPANSION_MODULE):
        print(f"module {mmm}, channel {ccc}")
help_button = Button(root_folder_lf, text="Help!", command=create_help_window)
help_button.place(x=scaled_size(5), y=0, width=scaled_size(65), height=scaled_size(25))

def create_help_window():
    help_window = Toplevel(root)
    help_window.title("duckyPad help")
    help_window.geometry(str(scaled_size(280)) + "x" + str(scaled_size(180)))
    help_window.resizable(width=FALSE, height=FALSE)
    help_window.grab_set()

    user_manual_label = Label(master=help_window, text="Not sure what to do?")
    user_manual_label.place(x=scaled_size(80), y=scaled_size(5))
    user_manual_button = Button(help_window, text="User Manual", command=open_duckypad_user_manual_url)
    user_manual_button.place(x=scaled_size(60), y=scaled_size(30), width=scaled_size(160))

    troubleshoot_label = Label(master=help_window, text="Problems?")
    troubleshoot_label.place(x=scaled_size(110), y=scaled_size(60))
    troubleshoot_button = Button(help_window, text="Troubleshooting Guides", command=open_duckypad_troubleshooting_url)
    troubleshoot_button.place(x=scaled_size(50), y=scaled_size(85), width=scaled_size(180))

    discord_label = Label(master=help_window, text="Questions or comments?")
    discord_label.place(x=scaled_size(70), y=scaled_size(60 + 55))
    discord_button = Button(help_window, text="Official Discord Chatroom", command=open_discord_link)
    discord_button.place(x=scaled_size(50), y=scaled_size(85 + 55), width=scaled_size(180))

script_common_commands_lf = LabelFrame(scripts_lf, text="Common commands", width=script_textbox.winfo_width(), height=scaled_size(105))
script_common_commands_lf.place(x=PADDING, y=scaled_size(300))
root.update()

script_button_xy_list = [(SCRIPT_BUTTON_GAP, PADDING), (SCRIPT_BUTTON_GAP*2+SCRIPT_BUTTON_WIDTH, PADDING), (SCRIPT_BUTTON_GAP*3+SCRIPT_BUTTON_WIDTH*2, PADDING), (SCRIPT_BUTTON_GAP, PADDING+BUTTON_HEIGHT+2), (SCRIPT_BUTTON_GAP*2+SCRIPT_BUTTON_WIDTH, PADDING+BUTTON_HEIGHT+2), (SCRIPT_BUTTON_GAP*3+SCRIPT_BUTTON_WIDTH*2, PADDING+BUTTON_HEIGHT+2), (SCRIPT_BUTTON_GAP, (PADDING+BUTTON_HEIGHT)*2+2), (SCRIPT_BUTTON_GAP*2+SCRIPT_BUTTON_WIDTH, (PADDING+BUTTON_HEIGHT)*2+2), (SCRIPT_BUTTON_GAP*3+SCRIPT_BUTTON_WIDTH*2, (PADDING+BUTTON_HEIGHT)*2+2)]
script_button_commands = ["STRINGLN", "STRING", "DELAY", "CTRL", "SHIFT", "ALT", "ENTER", "REPEAT", "more..."]
script_command_button_list = []


    first_instruction = get_empty_instruction()
    first_instruction['opcode'] = OP_JMP
    first_instruction['oparg'] = DS3_INIT_JUMP_OFFSET
    assembly_listing.append(first_instruction)

def save_click():
    save_everything(os.path.join(backup_path, make_default_backup_dir_name()))
    if os.path.isdir(dp_root_folder_path):
        save_everything(dp_root_folder_path)
        dp_root_folder_display.set("Done!")
        return
    try:
        put_duckypad_in_msc_mode_and_get_drive_path(reset_ui=False)
        save_everything(dp_root_folder_path)
        dp_root_folder_display.set("Ejecting...")
        root.update()
        hid_op.eject_drive(dp_root_folder_path)
        hid_op.duckypad_hid_close()
        hid_op.duckypad_hid_init()
        hid_op.duckypad_hid_sw_reset()
        dp_root_folder_display.set("Done!")
    except Exception as e:
        print('save_click:',e)
        messagebox.showinfo("save_click", str(e))

    disk_label = f'DP{dp_info[6]}_{dp_info[9]:02X}{dp_info[10]:02X}'
    print("disk label should be", disk_label)

    duckypad_drive_path = hid_op.get_duckypad_drive(disk_label)
    # Already mounted
    if duckypad_drive_path is not None:
        select_root_folder(duckypad_drive_path)
        return

    root_folder_path_label.config(foreground="blue")
    dp_root_folder_display.set("duckyPad Pro detected! Waiting for storage to show up...")

    hid_op.duckypad_hid_sw_reset(reboot_into_usb_msc_mode=1)
    ui_reset()
    root.update()
    seconds_to_wait = 8
    entry_time = time.time()
    while 1:
        duckypad_drive_path = hid_op.get_duckypad_drive(disk_label)
        if duckypad_drive_path is not None:
            break
        time_elapsed = time.time() - entry_time
        if time_elapsed > seconds_to_wait:
            break
        dp_root_folder_display.set(f"duckyPad Pro detected! Waiting for storage to show up... {int(seconds_to_wait - time_elapsed)}")
        root.update()
        time.sleep(0.5)
    dp_root_folder_display.set("")
    
    if duckypad_drive_path is None:
        if(messagebox.askokcancel("Info", "duckyPad drive not found!\n\nSelect manually instead?") == False):
            return
        select_root_folder()
        return
    select_root_folder(duckypad_drive_path)


----------------

# hid_op.duckypad_hid_sw_reset(reboot_into_usb_msc_mode=1)
shes!
disk label should be DP24_9BB0
root_path D:\
Exception in Tkinter callback
Traceback (most recent call last):
  File "C:\Program Files\Python310\lib\tkinter\__init__.py", line 1921, in __call__
    return self.func(*args)
  File "C:\Users\allen\Desktop\repos\duckyPad2\pc_software\duckypad_config.py", line 404, in connect_button_click
    select_root_folder(duckypad_drive_path)
  File "C:\Users\allen\Desktop\repos\duckyPad2\pc_software\duckypad_config.py", line 306, in select_root_folder
    duckypad_fw_ver = print_fw_update_label()
  File "C:\Users\allen\Desktop\repos\duckyPad2\pc_software\duckypad_config.py", line 257, in print_fw_update_label
    fw_result, this_version = check_firmware_update(current_fw_str)
  File "C:\Users\allen\Desktop\repos\duckyPad2\pc_software\duckypad_config.py", line 242, in check_firmware_update
    filelist = os.listdir(dp_root_folder_path)
OSError: [WinError 1006] The volume for a file has been externally altered so that the opened file is no longer valid: 'D:\\'
HID_NOP = 0
HID_DUMP = 1
HID_SAVE = 2
current_hid_op = HID_NOP
is_using_hid = False

def t1_worker():
    global current_hid_op
    while(1):
        time.sleep(0.2)
        if current_hid_op == HID_NOP:
            continue
        is_dp_ready, comment = hid_op.is_dp_ready()
        if is_dp_ready is False:
            messagebox.showerror("Error", comment)
            dp_root_folder_display.set("")
            current_hid_op = HID_NOP
            continue
        if current_hid_op == HID_DUMP:
            root_folder_path_label.config(foreground='navy')
            dp_root_folder_display.set("Loading...")
            current_hid_op = HID_NOP
            try:
                hid_op.dump_from_hid(hid_dump_path, dp_root_folder_display)
                select_root_folder(hid_dump_path, check_fw=False)
                print("done!")
                dp_root_folder_display.set("done!")
            except Exception as e:
                messagebox.showerror("Error", "error:\n\n"+str(traceback.format_exc()))
                dp_root_folder_display.set("HID load error!")
                continue
        if current_hid_op == HID_SAVE:
            hid_op.duckypad_hid_close()
            try:
                hid_op.duckypad_hid_init()
                hid_op.duckypad_hid_file_sync(hid_dump_path, hid_modified_dir_path, dp_root_folder_display)
                hid_op.duckypad_hid_sw_reset()
                try:
                    shutil.rmtree(hid_dump_path)
                    time.sleep(0.05)
                except FileNotFoundError:
                    pass
                os.rename(hid_modified_dir_path, hid_dump_path)
            except Exception as e:
                messagebox.showerror("error", "Save error: " + str(traceback.format_exc()))
                dp_root_folder_display.set("Save FAILED!")
            current_hid_op = HID_NOP

t1 = threading.Thread(target=t1_worker, daemon=True)
t1.start()


new_key = duck_objs.dp_key()
        new_key.name = keyname_line1
        new_key.name_line2 = keyname_line2
        profile_list[profile_index].keylist[selected_key] = new_key
        update_keylist_index()

# def on_closing():
#     root.destroy()

# root.protocol("WM_DELETE_WINDOW", on_closing)

# select_root_folder("sample_profiles")

upper_re_cw = Label(master=re_lf, text='RE1 CW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_cw.place(x=scaled_size(7), y=scaled_size(5), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_cw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(upper_re_cw)

upper_re_ccw = Label(master=re_lf, text='RE1 CCW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_ccw.place(x=scaled_size(7), y=scaled_size(35), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_ccw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(upper_re_ccw)

upper_re_sw = Label(master=re_lf, text='RE1 Push', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_sw.place(x=scaled_size(7), y=scaled_size(65), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_sw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(upper_re_sw)

lower_re_cw = Label(master=re_lf, text='RE2 CW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_cw.place(x=scaled_size(7), y=scaled_size(95), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_cw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(lower_re_cw)

lower_re_ccw = Label(master=re_lf, text='RE2 CCW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_ccw.place(x=scaled_size(7), y=scaled_size(125), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_ccw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(lower_re_ccw)

lower_re_sw = Label(master=re_lf, text='RE2 Push', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_sw.place(x=scaled_size(7), y=scaled_size(155), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_sw.bind("<Button-1>", rotary_encoder_button_click_event)
key_button_list.append(lower_re_sw)


script_textbox.place(x=key_button_list[0].winfo_x(), y=KEY_BUTTON_HEADROOM+PADDING-3, width=key_button_list[0].winfo_x() + KEY_BUTTON_WIDTH - KEY_BUTTON_GAP, height=key_button_list[0].winfo_y() - key_button_list[0].winfo_y() + KEY_BUTTON_HEIGHT + 5)

# ------------- RE frame -----------------

def reset_rotary_encoder_button_relief():
    upper_re_cw.config(borderwidth=1, relief="solid")
    upper_re_ccw.config(borderwidth=1, relief="solid")
    upper_re_sw.config(borderwidth=1, relief="solid")
    lower_re_cw.config(borderwidth=1, relief="solid")
    lower_re_ccw.config(borderwidth=1, relief="solid")
    lower_re_sw.config(borderwidth=1, relief="solid")

def rotary_encoder_button_click_event(event):
    rotary_encoder_button_click(event.widget)

def rotary_encoder_button_click(button_widget):
    print(button_widget)
    reset_key_button_relief()
    reset_rotary_encoder_button_relief()
    button_widget.config(borderwidth=3, relief='sunken')

RE_BUTTON_WIDTH = scaled_size(130)
RE_BUTTON_HEIGHT = scaled_size(25)

re_lf = LabelFrame(root, text="Rotary Encoders", width=scaled_size(150), height=scaled_size(205))
re_lf.place(x=scaled_size(590), y=scaled_size(50))
root.update()

upper_re_cw = Label(master=re_lf, text='RE1 CW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_cw.place(x=scaled_size(7), y=scaled_size(5), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_cw.bind("<Button-1>", rotary_encoder_button_click_event)

upper_re_ccw = Label(master=re_lf, text='RE1 CCW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_ccw.place(x=scaled_size(7), y=scaled_size(35), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_ccw.bind("<Button-1>", rotary_encoder_button_click_event)

upper_re_sw = Label(master=re_lf, text='RE1 Push', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_sw.place(x=scaled_size(7), y=scaled_size(65), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_sw.bind("<Button-1>", rotary_encoder_button_click_event)

lower_re_cw = Label(master=re_lf, text='RE2 CW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_cw.place(x=scaled_size(7), y=scaled_size(95), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_cw.bind("<Button-1>", rotary_encoder_button_click_event)

lower_re_ccw = Label(master=re_lf, text='RE2 CCW', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_ccw.place(x=scaled_size(7), y=scaled_size(125), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_ccw.bind("<Button-1>", rotary_encoder_button_click_event)

lower_re_sw = Label(master=re_lf, text='RE2 Push', borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_sw.place(x=scaled_size(7), y=scaled_size(155), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_sw.bind("<Button-1>", rotary_encoder_button_click_event)

# ------------- ------------





MAIN_WINDOW_WIDTH = scaled_size(1070)
MAIN_WINDOW_HEIGHT = scaled_size(605)
MAIN_COLUMN_HEIGHT = scaled_size(533)
PADDING = scaled_size(10)
HEIGHT_ROOT_FOLDER_LF = scaled_size(50)


REM_BLOCK
ESC
END_REM
DELAY 20
REM_BLOCK
GUI
END_REM
# def debug_set_root_folder():
#     global profile_list
#     global dp_root_folder_path
#     dir_result = "/Users/allen/Desktop/sample_profiles"
#     # dir_result = "/Users/allen/Desktop/empty"
#     dp_root_folder_path= dir_result
#     dp_root_folder_display.set("Selected: " + dir_result)
#     root_folder_path_label.config(foreground='navy')

#     profile_list = duck_objs.build_profile(dir_result)
#     update_profile_display()
#     enable_buttons()
 # or result in [x.name for x in profile_list[profile_index].keylist if x is not None]
 
def get_clean_key_name_2lines(user_text):
    split_line = user_text.replace('\r', '').split('\n')[:2]
    if len(split_line) == 0:
        return False, None, None
    line1_clean = None
    line2_clean = None
    print(split_line)
    if len(split_line) == 1:
        line1_clean = clean_input(split_line[0], KEY_NAME_MAX_LEN_PER_LINE, is_filename=False)
    if len(split_line) > 1:
        line2_clean = clean_input(split_line[1], KEY_NAME_MAX_LEN_PER_LINE, is_filename=False)
    print("line1:", line1_clean)
    print("line2:", line2_clean)

def get_clean_key_name_2lines(user_text):
    split_line = user_text.replace('\r', '').split('\n')[:2]
    if len(split_line) == 0:
        return False, None, None
    if len(split_line) == 1:
        line1_clean = clean_input(split_line[0], KEY_NAME_MAX_LEN_PER_LINE, is_filename=False)
        return True, None, None

def clean_input(str_input, len_limit=None, clean_filename=True):

script_textbox = Text(scripts_lf, relief='solid', borderwidth=1, padx=2, pady=2, spacing3=5, wrap="word")
script_textbox.place(x=key_button_list[0].winfo_x(), y=KEY_BUTTON_HEADROOM+PADDING-3, width=key_button_list[-1].winfo_x() + KEY_BUTTON_WIDTH - KEY_BUTTON_GAP, height=key_button_list[-1].winfo_y() - key_button_list[0].winfo_y() + KEY_BUTTON_HEIGHT + 5)
root.update()
script_textbox.bind("<<Modified>>", script_textbox_event)
script_textbox.tag_configure("error", background="#ffff00")


key_name_textbox = Text(keys_lf, state=DISABLED, relief='solid', borderwidth=1, padx=2, pady=2, spacing3=5, wrap="word")


if first_word == REM_BLOCK:
            print("hhhhh")
            exit()
        elif first_word == cmd_REM_BLOCK:
            return_dict['comment_block_dict'][line_number_starting_from_1] = None
            presult = PARSE_OK
            pcomment = ''
        elif first_word == cmd_END_REM:
            rem_block_keys = list(return_dict['comment_block_dict'].keys())
            if len(rem_block_keys) <= 0:
                presult = PARSE_ERROR
                pcomment = "orphan END_REM block"
            else:
                return_dict['comment_block_dict'][rem_block_keys[0]] = line_number_starting_from_1
                presult = PARSE_OK
                pcomment = ''

"""
### LCR (Loop Counter Reset)

* This command resets the counter for `LOOP` commands.

* `LCR` on its own resets **ALL** loop counters in the **CURRENT PROFILE** to **ZERO**. Effectively making everything start from `LOOP0` again.

* `LCR x` resets key `x` loop counter to ZERO, making that key start from `LOOP0` again. `x` is between 1 to 15.

* `LCR x y` resets key `x` loop counter to `y`, making that key start from `LOOP`y again. `x` is between 1 to 15. `y` is between 0 to 9.

* Try not to mix `LOOP` and `LCR` commands in the same script, probably won't end well.

"""

def load_keymap_from_file(file_path):
    this_keymap = dp_keymap()
    this_keymap.file_name = os.path.basename(file_path)
    this_keymap.display_name = this_keymap.file_name.replace('dpkm_', '').replace('.txt', '')
    with open(file_path, encoding='utf8') as keymap_file:
        this_keymap.content = keymap_file.readlines()
    this_keymap.is_valid = 1
    return this_keymap

def load_keymap(root_dir_path):
    keymap_folder_path = os.path.join(root_dir_path, 'keymaps')
    if os.path.isdir(keymap_folder_path) is False:
        return []
    keymap_file_list = [d for d in os.listdir(keymap_folder_path) if d.startswith("dpkm_") and d.endswith(".txt")]
    return_list = []
    for item in keymap_file_list:
        this_keymap = load_keymap_from_file(os.path.join(keymap_folder_path, item))
        if this_keymap.is_valid:
            return_list.append(this_keymap)
    return return_list
    
profile_remove_button = Button(profiles_lf, text="Remove", command=profile_remove_click, state=DISABLED)
profile_remove_button.place(x=PADDING * 2, y=BUTTON_Y_POS + BUTTON_HEIGHT + int(PADDING/2), width=BUTTON_WIDTH, height=BUTTON_HEIGHT)


    # ----------------- bytecode header --------------
    # dsb_header_size = 8
    # pgm_start = dsb_header_size
    # dsb_header = bytearray()
    # dsb_header += pgm_start.to_bytes(2, endianness)
    # dsb_header += var_bin_start.to_bytes(2, endianness)
    # dsb_header += str_bin_start.to_bytes(2, endianness)
    # dsb_header += bytearray(2)
    # output_bin_array = dsb_header + output_bin_array
    # ------------------------------------------------

    # print("label_to_addr_dict:", label_to_addr_dict)  
    # print("var_addr_dict:", var_addr_dict)
    # print('var_lookup:', var_lookup)
    # print("var_bin_start:", var_bin_start)
    # print("str_bin_start:", str_bin_start)
    # print("str_list:", str_list)

    # print('assembly line count:', len(assembly_listing))
    # print("label_dict:", label_dict)
    # print("func_lookup:", func_lookup)
    # print("str_lookup:", str_lookup)
    # print("var_lookup:", var_lookup)

    # print("--------- Bytecode header ---------")
    # for index, number in enumerate(output_bin_array[:8]):
    #   print("0x{:02x}".format(number), end=' ')

def write_var(var_name, var_value):
    ins_list = []
    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_PUSHC
    this_instruction['oparg'] = var_value
    ins_list.append(this_instruction)
    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_POP
    this_instruction['oparg'] = var_name
    ins_list.append(this_instruction)
    return ins_list

def get_int_args(pgm_line):
    return [int(x) for x in pgm_line.split(" ")[1:] if len(x) > 0]

def backup_button_click():
    if config_dict['auto_backup_enabled']:
        messagebox.showinfo("Backups", "Auto backup is ON!\n\nAll your backups are here!")
        if 'darwin' in sys.platform:
            subprocess.Popen(["open", backup_path])
        elif 'linux' in sys.platform:
            subprocess.Popen(["xdg-open", backup_path])
        else:
            webbrowser.open(backup_path)
    else:
        messagebox.showinfo("Backups", "Auto backup is OFF!\n\nSelect a folder to save a backup.")
        dir_result = filedialog.askdirectory(initialdir=os.path.join(os.path.expanduser('~'), "Desktop"))
        if len(dir_result) <= 0:
            return
        save_everything(os.path.join(dir_result, make_default_backup_dir_name()))
DS3 was released by [Hak5](https://docs.hak5.org/hak5-usb-rubber-ducky/) in late 2022 with major improvements such as:

* 
    # search longest name first
    # define_list = sorted(list(define_dict.keys()), key=len, reverse=True)
    # for key in define_list:
    #   if key in rvalue_str:
    #       rvalue_str = rvalue_str.replace(key, str(define_dict[key])).strip()

### Bytecode Compiler

With DS3 now very much a general-purpose language, it takes a lot more work to process it properly.

Examples include:

* Expression evaluation via abstract syntax tree (AST)

* Flow control with jump tables

* Arithmetic and call stacks

This is much too complicated to do on-device, therefore, DS3 is now compiled into **bytecode**, and executed on a **virtual stack machine**.

The configurator takes care of everything, so it's business as usual.

However, if you want to manually edit the SD card, you'll need to compile the script and copy over the binary too.


DEFAULTCHARDELAY 25
DEFAULTDELAY 25

// WINDOWS R
// DELAY 750

// STRINGLN notepad
// DELAY 750

// STRINGLN Welcome to duckyScript 3 beta test!
// ENTER

STRINGLN Declare variables and perform operations!
VAR $spam = 10
$spam = 10 * 4 + 2
STRINGLN    Spam is: $spam
ENTER

//---------------
$spam = 0
$_RANDOM_MIN = 1
$_RANDOM_MAX = 10

STRINGLN Conditional statements and loops!

WHILE $spam < 4
    VAR $this_number = $_RANDOM_INT
    STRING  Random number: $this_number

    IF $this_number >= 5 THEN
        STRINGLN , it is more than 5!
    ELSE
        STRINGLN , it is less than 5!
    END_IF

    $spam = $spam + 1
END_WHILE
ENTER

//----------------
STRINGLN Print to OLED screen too!
STRINGLN Look at the screen!

OLED_CLEAR
OLED_CURSOR 20 0
OLED_PRINT Hello World!
OLED_CURSOR 20 12
OLED_PRINT Press a key!
OLED_UPDATE
DELAY 2000
OLED_RESTORE

BCLR
WHILE 1
    OLED_CLEAR
    OLED_CURSOR 20 0
    OLED_PRINT Hello World!
    OLED_CURSOR 20 12
    OLED_PRINT Press a key!
    OLED_CURSOR 10 52
    OLED_PRINT Press + to exit

    VAR $this_key = $_READKEY

    IF $this_key != 0 THEN
        OLED_CURSOR 10 30
        OLED_PRINT I pressed key $this_key!
    END_IF

    IF $this_key == 17 THEN
        BREAK
    END_IF

    OLED_UPDATE
    DELAY 50
END_WHILE

OLED_RESTORE

// WHILE 1
// OLED_CLEAR
// OLED_CURSOR 64 32
// OLED_PRINT $_READKEY
// OLED_UPDATE
// DELAY 100

// IF $_READKEY == 1 THEN
//  BREAK
// END_IF

// END_WHILE
// OLED_RESTORE

void restore_profile(uint8_t profile_id)
{
  load_profile(profile_id);
  print_legend();
  f_closedir(&dir);
  f_close(&sd_file);
  save_last_profile(profile_id);
  reset_hold_cache();
}
        

        # ret += "".join([chr(x) for x in result[3:]]).strip('\0')
                # no need to dump dsb file since we'll be generating a new one
                # if(fff[0].lower().endswith('.dsb')):
                #     continue
//---------------
STRINGLN Also conditional statements!
IF $spam > 40 THEN
    STRINGLN    It is more than 40!
ELSE IF $spam > 20 THEN
    STRINGLN    It is more than 20!
ELSE
    STRINGLN    Nevermind!
END_IF
ENTER

// WHILE 1
//  OLED_CLEAR
//  OLED_CURSOR 64 32
//  OLED_PRINT $_READKEY
//  OLED_UPDATE
//  DELAY 100

//  IF $_READKEY == 1 THEN
//      BREAK
//  END_IF

// END_WHILE
// REM sjldkf
// OLED_RESTORE



with open(dps_path, 'w+', encoding='utf8', newline='') as setting_file:
            for line in dp_settings.list_of_lines:
                setting_file.write(line.replace('\r', '').replace('\n', '')+'\n');
was released by [Hak5](https://docs.hak5.org/hak5-usb-rubber-ducky/)
    # if hash1 != hash2:
    #   print(f"{file1} and {file2} are different!")
    #   print(hash1, hash2)
    #   print(open(file1,'rb').read())
    #   print(open(file2,'rb').read())
LOOP0:
SWCC 1 200 0 0
STRINGLN first action

LOOP1:
SWCC 1 0 200 0
STRINGLN second action

LOOP2:
SWCC 1 0 0 200
STRINGLN third action




    if result_dict['state_save_needed']:
        this_instruction = get_empty_instruction()
        this_instruction['opcode'] = OP_PUSHC
        this_instruction['oparg'] = 1
        assembly_listing.append(this_instruction)
        this_instruction = get_empty_instruction()
        this_instruction['opcode'] = OP_POP
        this_instruction['oparg'] = "_NEEDS_SPS"
        assembly_listing.append(this_instruction)

$_LOOP_MAX = 3

IF $_KEYPRESS_COUNT % $_LOOP_MAX == 0 THEN
STRINGLN first action
END_IF

IF $_KEYPRESS_COUNT % $_LOOP_MAX == 1 THEN
STRINGLN second action
END_IF

IF $_KEYPRESS_COUNT % $_LOOP_MAX == 2 THEN
STRINGLN third action
END_IF



----------


BCLR
VAR $COND = 1
WHILE $COND
VAR $BUTT = $_READKEY
STRINGLN $BUTT
DELAY 500
IF $BUTT == 16 THEN
$COND = 0
END_IF
END_WHILE


---
VAR $C = 5
WHILE 1
    WHILE 1
    STRINGLN C IS $C
    $C = $C - 1
    BREAK
    IF $C == 2 THEN
    BREAK
    END_IF
    END_WHILE
BREAK
END_WHILE


---------------

VAR $INDEX = 2 * 3
VAR $RED = 33 * 2
VAR $GREEN = 44 / 2
VAR $BLUE = 55 + 4

SWCC $INDEX    8  $GREEN $BLUE

SWCC 1 100 0 0
DELAY 500
SWCC 1 0 100 0
DELAY 500
SWCC 1 0 0 100
DELAY 500

SWCA

VAR $C = 255

WHILE $C > 5
SWCC 1 $C $C $C
DELAY 10
$C = $C - 2
END_WHILE 


SWCC 1 200 0 0
SWCC 2 200 0 0
SWCC 3 200 0 0
SWCC 4 200 0 0
SWCC 5 200 0 0
SWCC 6 200 0 0
SWCC 7 200 0 0
SWCC 8 200 0 0
SWCC 9 200 0 0
SWCC 10 200 0 0
SWCC 11 200 0 0
SWCC 12 200 0 0
SWCC 13 200 0 0
SWCC 14 200 0 0
SWCC 15 200 0 0
DELAY 500

SWCC 1 0 200 0
SWCC 2 0 200 0
SWCC 3 0 200 0
SWCC 4 0 200 0
SWCC 5 0 200 0
SWCC 6 0 200 0
SWCC 7 0 200 0
SWCC 8 0 200 0
SWCC 9 0 200 0
SWCC 10 0 200 0
SWCC 11 0 200 0
SWCC 12 0 200 0
SWCC 13 0 200 0
SWCC 14 0 200 0
SWCC 15 0 200 0
DELAY 500

SWCC 1 0 0 200
SWCC 2 0 0 200
SWCC 3 0 0 200
SWCC 4 0 0 200
SWCC 5 0 0 200
SWCC 6 0 0 200
SWCC 7 0 0 200
SWCC 8 0 0 200
SWCC 9 0 0 200
SWCC 10 0 0 200
SWCC 11 0 0 200
SWCC 12 0 0 200
SWCC 13 0 0 200
SWCC 14 0 0 200
SWCC 15 0 0 200
DELAY 500


VAR $c = 63

SWCR   $c


---

VAR $YY = 20

OLC 0 $YY
OLP           
OLC 0 $YY
OLP $_RANDOM_INT
OLU
-------
WHILE TRUE
STRINGLN $_READKEY
DELAY 500
END_WHILE



def make_swcolor_instruction(pgm_line):
    split = [x for x in pgm_line.split(' ') if len(x) > 0]
    cmd = split[0].strip()
    sw = 0
    if cmd.startswith("SWCOLOR_"):
        sw = int(cmd.split('_')[1])
    rrrr = int(split[1])
    gggg = int(split[2])
    bbbb = int(split[3])

    ins_list = []
    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_SWCR
    this_instruction['oparg'] = get_combined_value(sw, rrrr)
    ins_list.append(this_instruction)

    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_SWCG
    this_instruction['oparg'] = get_combined_value(sw, gggg)
    ins_list.append(this_instruction)

    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_SWCB
    this_instruction['oparg'] = get_combined_value(sw, bbbb)
    ins_list.append(this_instruction)

    this_instruction = get_empty_instruction()
    this_instruction['opcode'] = OP_SWCE
    ins_list.append(this_instruction)

    return ins_list

def get_combined_value(b0, b1):
    return ((b0 % 0xff) << 8) | (b1 % 0xff)

  else if(this_opcode == OP_PUSHV)
  {
    
    uint16_t this_value;
    if(op_data == DEFAULTDELAY_ADDR)
      this_value = defaultdelay_value;
    else if (op_data == DEFAULTCHARDELAY_ADDR)
      this_value = defaultchardelay_value;
    else if (op_data == CHARJITTER_ADDR)
      this_value = charjitter_value;
    else
      this_value= make_uint16(pgm_start[op_data], pgm_start[op_data+1]);
    op_result = stack_push(&arithmetic_stack, this_value);
    if(op_result != STACK_OP_OK)
    {
      exe->result = op_result;
      return;
    }
  }

def get_partial_varname_addr(msg, vad):
    for x in range(len(msg)+1):
        partial_name = msg[:x]
        if partial_name in vad:
            return partial_name, vad[partial_name]
    return None, None

def replace_var_in_str(msg, vad):
    search = msg.split('$')
    if len(search) == 1:
        return msg
    print('before', msg)
    for item in [x.strip() for x in search if len(x.strip()) != 0]:
        var_name, var_addr = get_partial_varname_addr(item, vad)
        if var_addr is not None:
            # print(f"!!!!!!!!! {item} contains a variable")
            twobyte = var_addr.to_bytes(2, endianness)
            # print(f"${var_name}", f"|{var_addr}|", chr(twobyte[0]), chr(twobyte[1]))
            msg = msg.replace(f"${var_name}", f"{var_boundary_chr}{chr(twobyte[0])}{chr(twobyte[1])}{var_boundary_chr}")
    print('after', msg)
    return msg

void led_animation_handler(void)
{
  frame_counter++;
  for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
  {
    int32_t current_frame = frame_counter - neo_anime[idx].animation_start;
    if(current_frame <= 0)
      continue;
    if(neo_anime[idx].animation_type == ANIMATION_NO_ANIMATION)
      continue;
    else if(neo_anime[idx].animation_type == ANIMATION_FULLY_ON)
    {
      for (int i = 0; i < THREE; ++i)
        neo_anime[idx].current_color[i] = neo_anime[idx].target_color[i];
      set_pixel_color(neo_anime[idx].index, neo_anime[idx].target_color[0], neo_anime[idx].target_color[1], neo_anime[idx].target_color[2]);
    }
    else if(neo_anime[idx].animation_type == ANIMATION_CROSS_FADE)
    {
      if(current_frame <= neo_anime[idx].animation_duration)
      {
        for (int i = 0; i < THREE; ++i)
        {
          neo_anime[idx].current_color[i] += neo_anime[idx].step[i];
          if(neo_anime[idx].current_color[i] > 255)
            neo_anime[idx].current_color[i] = 255;
          if(neo_anime[idx].current_color[i] < 0)
            neo_anime[idx].current_color[i] = 0;
        }
      }
      else
      {
         for (int i = 0; i < THREE; ++i)
          neo_anime[idx].current_color[i] = neo_anime[idx].target_color[i];
      }
      set_pixel_color(neo_anime[idx].index, neo_anime[idx].current_color[0], neo_anime[idx].current_color[1], neo_anime[idx].current_color[2]);
    }  
  }
  neopixel_show(red_buf, green_buf, blue_buf);
}


else if(this_opcode == OP_PUSHV)
  {
    uint16_t this_value = make_uint16(pgm_start[op_data], pgm_start[op_data+1]);
    op_result = stack_push(&arithmetic_stack, this_value);
    if(op_result != STACK_OP_OK)
    {
      exe->result = op_result;
      return;
    }
  }

script_exe_warning_showed = False

def run_script():
    global script_exe_warning_showed
    if len(script_textbox.get("1.0",END)) <= 2:
        return
    warning_msg = "You are about to execute this script.\n> MAKE SURE YOU TRUST IT!\n> Result might differ from real duckyPad.\n\nExecution will start after a 2-second delay."
    if 'darwin' in sys.platform:
        warning_msg = "You are about to execute this script.\n> MAKE SURE YOU TRUST IT!\n> Result might differ from real duckyPad.\n> If nothing happens, give this app Accessibility permission.\n> Check Getting Started Guide for details. \n\nExecution will start after a 2-second delay."
    if(script_exe_warning_showed is False):
        if(messagebox.askokcancel("Warning", warning_msg) == False):
            return
    # try:
    #     import autogui
    #     time.sleep(2)
    #     exe_result, exe_note = autogui.execute_file(script_textbox.get("1.0",END).replace('\r', '').split('\n'))
    #     if exe_result != 0:
    #         messagebox.showerror("Error", exe_note)
    # except Exception as e:
    #     messagebox.showerror("Error", "execution failed:\n\n" + str(e))
    script_exe_warning_showed = True

execute_button = Button(scripts_lf, text="Run script", command=run_script, state=DISABLED)
execute_button.place(x=scaled_size(160), y=scaled_size(417), width=scaled_size(80), height=BUTTON_HEIGHT)
root.update()
    execute_button.config(state=NORMAL, fg="red")


pc_app_update_label = Label(master=updates_lf)
pc_app_update_label.place(x=scaled_size(5), y=scaled_size(0))

--------------------

    # third pass, replace lines that are too long

    print("---------Second pass OK!---------\n")

    third_pass_program_listing = []
    MAX_LINE_LEN = 245
    for item in second_pass_program_listing:
        orig_line_number_starting_from_1 = item[0]
        code_content = item[1]
        first_word = code_content.split(" ")[0]

        if len(code_content) > MAX_LINE_LEN:
            if first_word.startswith(cmd_STRING) is False:
                rdict['is_success'] = False
                rdict['comments'] = "Line too long, unable to auto-split"
                rdict['error_line_number_starting_from_1'] = orig_line_number_starting_from_1
                rdict['error_line_str'] = code_content
                return rdict
            chunk_size = MAX_LINE_LEN
            chunks = [code_content[i:i+chunk_size] for i in range(0, len(code_content), chunk_size)]
            for item in chunks:
                # print(f"{orig_line_number_starting_from_1} {first_word} {item}")
                third_pass_program_listing.append((orig_line_number_starting_from_1, f"{first_word} {item}"))
        else:
            # print(f"{orig_line_number_starting_from_1} {code_content}")
            third_pass_program_listing.append((orig_line_number_starting_from_1, code_content))

    final_dict = run_once([x[1] for x in third_pass_program_listing])
    final_dict["compact_listing"] = third_pass_program_listing

    if_info_list = []
    for item in if_raw_info:    
        if_root = list(item.keys())[0]
        if_info = {
        'root':if_root,
        "else_if":item[if_root]['else_if'],
        "else":item[if_root]['else'],
        "end_if":item[if_root]['end_if'],
        }
        if_info_list.append(if_info)
    final_dict["if_info"] = if_info_list
    print("---------Preprocessing Done!---------\n")
    return final_dict