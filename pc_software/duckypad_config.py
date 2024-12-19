import os
import sys
import time
import copy
import shutil
from datetime import datetime
import duck_objs
import webbrowser
import check_update
from tkinter import *
from tkinter import filedialog
from tkinter import simpledialog
from tkinter.colorchooser import askcolor
from tkinter import messagebox
import urllib.request
from platformdirs import *
import subprocess
import hid_op
import make_bytecode
from shared import *
import my_compare

"""
0.13.5
changed old HOLD to EMUK command

0.13.6
Added japanese IME keys "KATAKANAHIRAGANA", "HENKAN", "MUHENKAN", "KATAKANA", "HIRAGANA", "ZENKAKUHANKAKU"

0.14.0
added EMUK replacement
only show last 50 characters when deleting folders
fixed syntax check bug where MMOUSE isnt recognized
script checking now provides error details
defaultdelay and defaultchardelay now resets correctly when running a new script in pc test-run
ask to confirm if trying to quit while saving data

0.15.0 2023 01 23
working on adding DSB support

1.0.0 2023 02 06
duckyScript 3 public beta

1.0.1 2023 02 13
minor bug fixes
better handles saving when code contains errors
removed unused code
fixed a function detection bug

1.1.0 2023 02 14
updated dsb binary format to allow banked loading
cleaned up opcode values

1.1.1 2023 02 16
changed loop break command to LBREAK to avoid conflict with BREAK keyboard key

1.2.0 2023 02 17
added profile import function
script display now hidden if no key is selected
automatically selects first profile after loading
added color to key apply and remove button
added minimum firmware version check

1.2.1 2023 02 21
added hid busy check

1.2.2 2023 03 01
fixed HID busy detection bug

1.3.0 2023 05 02
Fixed a firmware version parse bug
getting ready for public release
added firmware version compatibility check with upper and lower bound, both HID and file based.

1.3.5 2023 05 12
fixed a bug where it tries to load junk macOS files
added back COMMAND key 

1.4.0 2023 07 01
added _TIME_S read-only variable
Updated colour pickers to provide an appropriate initial colour and title for the dialog window. (PR#135)

1.4.1 2023 07 01
Fixed a crash when typing EMUK command too slowly

1.4.2 2023 09 12
added REM_BLOCK and REM_END

1.5.0 2023 09 18
added STRINGLN_BLOCK and STRINGLN_END
added STRING_BLOCK and STRING_END
adjust INJECT_MOD behaviour

1.5.1 2023 09 20
STRINGLN_BLOCK and STRING_BLOCK now preserves empty lines and white spaces

1.6.1 2023 10 10
automatically expands MOUSE_MOVE is value is more than 127
checks if duckypad is busy before trying to connect

1.6.2 2023 11 11
increased max profile to 64

1.6.3 2023 11 30
automatically splits STRING/STRINGLN commands if too long

1.6.4 2024 01 22
Fixed a bug where TRUE and FALSE is replaced with 1 and 0 inside STRING statements

2.0.0 2024 11 21
New for duckyPad Pro

2.0.1 2024 11 22
Fixed off-by-1 error in GOTO_PROFILE

2.0.2 2024 12 17
Fixed text parsed as comments in STRING blocks

"""

THIS_VERSION_NUMBER = '2.0.2'
MIN_DUCKYPAD_FIRMWARE_VERSION = "1.0.0"
MAX_DUCKYPAD_FIRMWARE_VERSION = "1.5.0"

UI_SCALE = float(os.getenv("DUCKYPAD_UI_SCALE", default=1))
USB_MSC_MOUNTPOINT = os.getenv("DUCKYPAD_MS_MOUNTPOINT", default=None)
USB_MSC_SECONDS_TO_WAIT = int(os.getenv("DUCKYPAD_MS_TIMEOUT", default=15))

def ensure_dir(dir_path):
    os.makedirs(dir_path, exist_ok=1)

def is_root():
    return os.getuid() == 0

def scaled_size(size: int) -> int:
    return int(size * UI_SCALE)

appname = 'duckypad_config'
appauthor = 'dekuNukem'
app_save_path = user_data_dir(appname, appauthor, roaming=True)
backup_path = os.path.join(app_save_path, 'profile_backups')
ensure_dir(app_save_path)
ensure_dir(backup_path)

print("\n\n--------------------------")
print("\n\nWelcome to duckyPad Configurator!\n")
print("This window prints debug information.")
print("Used for troubleshooting if it crashes.\n\n")

default_button_color = 'SystemButtonFace'
if 'linux' in sys.platform:
    default_button_color = 'grey'

MAIN_WINDOW_WIDTH = scaled_size(1070)
MAIN_WINDOW_HEIGHT = scaled_size(605)
PADDING = scaled_size(10)
HEIGHT_ROOT_FOLDER_LF = scaled_size(50)
INVALID_ROOT_FOLDER_STRING = "<-- Press to connect to duckyPad"
last_rgb = (238,130,238)
discord_link_url = "https://raw.githubusercontent.com/dekuNukem/duckyPad/master/resources/discord_link.txt"

def open_discord_link():
    try:
        webbrowser.open(str(urllib.request.urlopen(discord_link_url).read().decode('latin-1')).split('\n')[0])
    except Exception as e:
        messagebox.showerror("Error", "I can't find the discord link...\n"+str(e))

def open_duckypad_user_manual_url():
    webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/getting_started.md')

def open_duckypad_troubleshooting_url():
    webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/troubleshooting.md')

def app_update_click(event):
    webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/releases/latest')

def reset_key_button_relief():
    for item in key_button_list:
        item.config(borderwidth=1, relief="solid")

def rgb_to_hex(rgb_tuple):
    return '#%02x%02x%02x' % rgb_tuple

def ui_reset():
    global selected_key
    profile_add_button.config(state=DISABLED)
    profile_remove_button.config(state=DISABLED)
    profile_rename_button.config(state=DISABLED)
    profile_up_button.config(state=DISABLED)
    profile_down_button.config(state=DISABLED)
    profile_dupe_button.config(state=DISABLED)
    save_button.config(state=DISABLED)
    kd_R1.config(state=DISABLED)
    kd_R2.config(state=DISABLED)
    dim_unused_keys_checkbox.config(state=DISABLED)
    rotate_keys_checkbox.config(state=DISABLED)
    key_remove_button.config(state=DISABLED)
    key_name_textbox.config(state=DISABLED)
    reset_key_button_relief()
    update_key_button_appearances(None)
    key_name_textbox.delete('1.0', 'end')
    selected_key = None
    key_color_button.config(background=default_button_color)
    bg_color_button.config(background=default_button_color)
    kd_color_button.config(background=default_button_color)
    custom_key_color_checkbox.deselect()
    custom_key_color_checkbox.config(state=DISABLED)
    allow_abort_checkbox.config(state=DISABLED)
    dont_repeat_checkbox.config(state=DISABLED)
    script_textbox.delete(1.0, 'end')
    profile_lstbox.delete(0, 'end')
    check_syntax_label.config(text="", fg="green")
    profile_import_button.config(state=DISABLED)
    exp_page_plus_button.config(state=DISABLED)
    exp_page_minus_button.config(state=DISABLED)

def fw_update_click(event):
    webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/fw_update.md')

def print_fw_update_label(current_fw_str=None):
    fw_result, this_version = check_update.get_firmware_update_status(current_fw_str), current_fw_str
    if fw_result == 0:
        dp_fw_update_label.config(text='Firmware (' + str(this_version) +'): Up to date', fg='black', bg=default_button_color)
        dp_fw_update_label.unbind("<Button-1>")
    elif fw_result == 1:
        dp_fw_update_label.config(text='Firmware (' + str(this_version) +'): Update available! Click me!', fg='black', bg='orange', cursor="hand2")
        dp_fw_update_label.bind("<Button-1>", fw_update_click)
    else:
        dp_fw_update_label.config(text='Firmware: Unknown', fg='black', bg=default_button_color)
        dp_fw_update_label.unbind("<Button-1>")
    return this_version

def get_fw_str_hid():
    dp_info = hid_op.get_dp_info()
    if dp_info is None:
        return None
    return f"{dp_info[3]}.{dp_info[4]}.{dp_info[5]}"

is_root_folder_duckypad_msc = False

FW_OK = 0
FW_TOO_LOW = 1
FW_TOO_HIGH = 2
FW_UNKNOWN = 3

def check_fw_support(current_fw_str):
    try:
        if check_update.versiontuple(current_fw_str) < check_update.versiontuple(MIN_DUCKYPAD_FIRMWARE_VERSION):
            return FW_TOO_LOW
        if check_update.versiontuple(current_fw_str) > check_update.versiontuple(MAX_DUCKYPAD_FIRMWARE_VERSION):
            return FW_TOO_HIGH
        return FW_OK
    except Exception as e:
        print('check_fw_support', current_fw_str, e)
        return FW_UNKNOWN

def select_root_folder(root_path=None, is_msc=False):
    global profile_list
    global dp_root_folder_path
    global is_root_folder_duckypad_msc

    is_root_folder_duckypad_msc = is_msc
    if root_path is None:
        root_path = filedialog.askdirectory()
    if len(root_path) <= 0:
        return
    dp_root_folder_path = root_path
    dp_root_folder_display.set("Selected: " + root_path)
    root_folder_path_label.config(foreground='navy')
    profile_list = duck_objs.build_profile(root_path)

    ui_reset()
    update_profile_display()
    enable_buttons()
    try:
        profile_lstbox.select_set(0)
        update_profile_display()
    except Exception as e:
        print("select_root_folder:", e)

def incompatible_fw_msgbox(current_fw_str, fw_status):
    if fw_status == FW_TOO_LOW:
        if messagebox.askokcancel("Info", f"duckyPad firmware too low!\n\nCurrent: {current_fw_str}\nSupported: Between {MIN_DUCKYPAD_FIRMWARE_VERSION} and {MAX_DUCKYPAD_FIRMWARE_VERSION}.\n\nSee how to update it?"):
            fw_update_click(None)
    elif fw_status == FW_TOO_HIGH:
        if messagebox.askokcancel("Info", f"duckyPad firmware too high!\n\nCurrent: {current_fw_str}\nSupported: Between {MIN_DUCKYPAD_FIRMWARE_VERSION} and {MAX_DUCKYPAD_FIRMWARE_VERSION}.\n\nSee how to update this app?"):
            app_update_click(None)
    else:
        messagebox.showinfo("Info", f"duckyPad firmware unknown!\n\n")

def put_duckypad_in_msc_mode_and_get_drive_path(reset_ui=True):
    disk_label = None
    if USB_MSC_MOUNTPOINT:
        disk_label = USB_MSC_MOUNTPOINT
    else:
        dp_info = hid_op.get_dp_info()
        if dp_info is None:
            return None
        disk_label = f'DP{dp_info[6]}_{dp_info[9]:02X}{dp_info[10]:02X}'
    print("disk label should be", disk_label)
    duckypad_drive_path = hid_op.get_duckypad_drive(disk_label)
    # already mounted
    if duckypad_drive_path is not None:
        return duckypad_drive_path
    
    hid_op.duckypad_hid_sw_reset(reboot_into_usb_msc_mode=1)

    if reset_ui:
        ui_reset()
    root.update()
    root_folder_path_label.config(foreground="blue")
    entry_time = time.time()
    while 1:
        duckypad_drive_path = hid_op.get_duckypad_drive(disk_label)
        if duckypad_drive_path is not None:
            break
        time_elapsed = time.time() - entry_time
        if time_elapsed > USB_MSC_SECONDS_TO_WAIT:
            break
        dp_root_folder_display.set(f"duckyPad Pro detected! Waiting for storage... {int(USB_MSC_SECONDS_TO_WAIT - time_elapsed)}")
        root.update()
        time.sleep(0.5)
    dp_root_folder_display.set("")
    return duckypad_drive_path

def connect_button_click():
    if hid_op.get_duckypad_path() is None:
        if(messagebox.askokcancel("Info", "duckyPad not found!\n\nManually select a folder instead?") == False):
            return
        select_root_folder()
        return

    init_success = True
    hid_op.duckypad_hid_close()
    try:
        hid_op.duckypad_hid_init()
        is_dp_ready, comment = hid_op.is_dp_ready()
        if is_dp_ready is False:
            messagebox.showerror("Error", comment)
            return
        fw_str = get_fw_str_hid()
        fw_status = check_fw_support(fw_str)
        if fw_status != FW_OK:
            init_success = False
            incompatible_fw_msgbox(fw_str, fw_status)
        print_fw_update_label(fw_str)
    except Exception as e:
        print("connect_button_click:", e)
        init_success = False

    if init_success is False and 'linux' in sys.platform:
        box_result = messagebox.askyesnocancel("Info", "duckyPad detected, but additional permissions needed.\n\nClick Yes for instructions\n\nClick No to configure via SD card.")
        if box_result is True:
            webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/linux_macos_notes.md')
        elif box_result is False:
            select_root_folder()
        return

    if init_success is False and 'darwin' in sys.platform and is_root() is False:
        box_result = messagebox.askyesnocancel("Info", "duckyPad detected, but additional permissions needed to access it.\n\nClick Yes for instructions\n\nClick No to configure via SD card.")
        if box_result is True:
            webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/linux_macos_notes.md')
        elif box_result is False:
            select_root_folder()
        return

    if init_success is False and 'darwin' in sys.platform and is_root() is True:
        box_result = messagebox.askyesnocancel("Info", "duckyPad detected, however, due to macOS restrictions, you need to enable some privacy settings.\n\nClick Yes to learn how.\n\nClick No to configure via SD card.")
        if box_result is True:
            webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/linux_macos_notes.md')
        elif box_result is False:
            select_root_folder()
        return
    
    duckypad_drive_path = put_duckypad_in_msc_mode_and_get_drive_path()
    if duckypad_drive_path is None:
        if(messagebox.askokcancel("Info", "duckyPad drive not found!\n\nSelect manually instead?") == False):
            return
        select_root_folder()
        return
    select_root_folder(duckypad_drive_path, is_msc=True)
    
def enable_buttons():
    profile_add_button.config(state=NORMAL)
    profile_remove_button.config(state=NORMAL)
    profile_rename_button.config(state=NORMAL)
    profile_up_button.config(state=NORMAL)
    profile_down_button.config(state=NORMAL)
    profile_dupe_button.config(state=NORMAL)
    save_button.config(state=NORMAL)
    backup_button.config(state=NORMAL)
    kd_R1.config(state=NORMAL)
    kd_R2.config(state=NORMAL)
    dim_unused_keys_checkbox.config(state=NORMAL)
    rotate_keys_checkbox.config(state=NORMAL)
    key_remove_button.config(state=NORMAL)
    profile_import_button.config(state=NORMAL)
    exp_page_plus_button.config(state=NORMAL)
    exp_page_minus_button.config(state=NORMAL)

def profile_shift_up():
    global profile_var
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0 or selection[0] == 0:
        return
    source = selection[0]
    destination = selection[0] - 1
    profile_list[destination], profile_list[source] = profile_list[source], profile_list[destination]
    update_profile_display()
    profile_lstbox.selection_clear(0, len(profile_list))
    profile_lstbox.selection_set(destination)
    update_profile_display()

def profile_shift_down():
    global profile_var
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0 or selection[0] == len(profile_list) - 1:
        return
    source = selection[0]
    destination = selection[0] + 1
    profile_list[destination], profile_list[source] = profile_list[source], profile_list[destination]
    update_profile_display()
    profile_lstbox.selection_clear(0, len(profile_list))
    profile_lstbox.selection_set(destination)
    update_profile_display()

def adapt_color(rgb_tuple):
    if (rgb_tuple[0]*0.299 + rgb_tuple[1]*0.587 + rgb_tuple[2]*0.114) > 145:
        return "black"
    return 'white'

def update_profile_display():
    global selected_key
    profile_var.set([' '+x.name for x in profile_list]) # update profile listbox
    if len(profile_lstbox.curselection()) <= 0:
        return
    index = profile_lstbox.curselection()[0]
    bg_color_hex = rgb_to_hex(profile_list[index].bg_color)
    bg_color_button.config(background=bg_color_hex)

    if profile_list[index].kd_color is None:
        kd_R1.select()
        kd_color_button.config(background=default_button_color)
    else:
        kd_R2.select()
        kd_color_button.config(background=rgb_to_hex(profile_list[index].kd_color))

    if profile_list[index].dim_unused:
        dim_unused_keys_checkbox.select()
    else:
        dim_unused_keys_checkbox.deselect()

    if profile_list[index].is_landscape:
        rotate_keys_checkbox.select()
    else:
        rotate_keys_checkbox.deselect()

    update_key_button_appearances(index)
    reset_key_button_relief()
    key_name_textbox.delete('1.0', 'end')
    selected_key = None
    key_color_button.config(background=default_button_color)
    custom_key_color_checkbox.deselect()
    custom_key_color_checkbox.config(state=DISABLED)
    allow_abort_checkbox.config(state=DISABLED)
    dont_repeat_checkbox.config(state=DISABLED)
    script_textbox.delete(1.0, 'end')
    check_syntax_label.config(text="", fg="green")

def make_key_button_text_from_two_lines(line1, line2):
    if line1 is None:
        return ''
    if line2 is None or len(line2) == 0:
        return clean_input(line1, KEY_NAME_MAX_CHAR_PER_LINE, is_filename=False)
    return clean_input(line1, KEY_NAME_MAX_CHAR_PER_LINE, is_filename=False) + "\n" + clean_input(line2, KEY_NAME_MAX_CHAR_PER_LINE, is_filename=False)

def update_key_button_appearances(profile_index):
    if profile_index is None:
        place_obsw_buttons_portrait()
        for x in range(MECH_OBSW_COUNT):
            key_button_list[x].config(background=default_button_color, text='')
        return
    if profile_list[profile_index].is_landscape:
        place_obsw_buttons_landscape()
    else:
        place_obsw_buttons_portrait()
    for key_index, item in enumerate(profile_list[profile_index].keylist):
        if item is not None:
            this_color = None
            if item.color is not None:
                key_button_list[key_index].config(background=rgb_to_hex(item.color))
                this_color = item.color
            else:
                key_button_list[key_index].config(background=rgb_to_hex(profile_list[profile_index].bg_color))
                this_color = profile_list[profile_index].bg_color
            this_key_name = make_key_button_text_from_two_lines(item.name, item.name_line2)
            if is_rotary_encoder_button(key_index):
                this_key_name = item.name
            key_button_list[key_index].config(text=this_key_name, foreground=adapt_color(this_color))
        elif item is None and profile_list[profile_index].dim_unused is False:
            key_button_list[key_index].config(text='', background=rgb_to_hex(profile_list[profile_index].bg_color))
        elif item is None and profile_list[profile_index].dim_unused:
            key_button_list[key_index].config(background=default_button_color, text='')

def kd_radiobutton_auto_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    profile_list[selection[0]].kd_color = None
    update_profile_display()

def kd_radiobutton_custom_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    profile_list[selection[0]].kd_color = last_rgb
    update_profile_display()

def dim_unused_keys_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    profile_list[selection[0]].dim_unused = bool(dim_unused_keys_checkbox_var.get())
    update_profile_display()

def on_profile_lstbox_select(event):
    global current_selected_expansion_module
    current_selected_expansion_module = 0
    exp_page_update()
    scripts_lf.place_forget()
    empty_script_label.place(x=scaled_size(800), y=scaled_size(200))
    key_name_textbox.delete('1.0', 'end')
    key_name_textbox.config(state=DISABLED)
    update_profile_display()

def bg_color_click(event):
    global profile_list
    global last_rgb
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    result = askcolor(color=profile_list[selection[0]].bg_color, title="Background color for " + profile_list[selection[0]].name + " profile")[0]
    if result is None:
        return
    last_rgb = result
    profile_list[selection[0]].bg_color = result
    update_profile_display()

def kd_color_click(event):
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0 or kd_color_var.get() == 0:
        return
    result = askcolor(color=profile_list[selection[0]].kd_color, title="Activation color for " + profile_list[selection[0]].name + " profile")[0]
    if result is None:
        return
    profile_list[selection[0]].kd_color = result
    update_profile_display()

invalid_filename_characters = ['<', '>', ':', '"', '/', '\\', '|', '?', '*']

def clean_input(str_input, len_limit=None, is_filename=True):
    result = ''.join([x for x in str_input if 32 <= ord(x) <= 126 and x not in invalid_filename_characters])
    if is_filename is False:
        result = ''.join([x for x in str_input if 32 <= ord(x) <= 126])
    while('  ' in result):
        result = result.replace('  ', ' ')
    if len_limit is not None:
        result = result[:len_limit]
    return result.strip()

def profile_add_click():
    global profile_list
    if len(profile_list) >= duck_objs.MAX_PROFILE_COUNT:
        return
    answer = simpledialog.askstring("Input", "New profile name?", parent=profiles_lf)
    if answer is None:
        return

    insert_point = len(profile_list)
    try:
        insert_point = profile_lstbox.curselection()[0] + 1
    except Exception as e:
        # print('insert:', e)
        pass

    answer = clean_input(answer, 16)

    if len(answer) <= 0:# or answer in [x.name for x in profile_list]:
        return

    new_profile = duck_objs.dp_profile()
    new_profile.name = answer
    profile_list.insert(insert_point, new_profile)
    update_profile_display()
    profile_lstbox.selection_clear(0, len(profile_list))
    profile_lstbox.selection_set(insert_point)
    update_profile_display()

def profile_remove_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    profile_list.pop(selection[0])
    update_profile_display()
    profile_lstbox.selection_clear(0, len(profile_list))
    profile_lstbox.selection_set(selection[0])
    update_profile_display()
    if len(profile_list) <= 0 or len(profile_lstbox.curselection()) <= 0:
        update_key_button_appearances(None)
        reset_key_button_relief()

def profile_dupe_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    answer = simpledialog.askstring("Input", "New name?", parent=profiles_lf, initialvalue=profile_list[selection[0]].name)
    if answer is None:
        return
    answer = clean_input(answer, 13)
    if len(answer) <= 0: # or answer in [x.name for x in profile_list]:
        return
    new_profile = copy.deepcopy(profile_list[selection[0]])
    new_profile.name = answer
    profile_list.insert(selection[0] + 1, new_profile)
    update_profile_display()
    profile_lstbox.selection_clear(0, len(profile_list))
    profile_lstbox.selection_set(selection[0] + 1)
    update_profile_display()

def profile_rename_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    answer = simpledialog.askstring("Input", "New name?", parent=profiles_lf, initialvalue=profile_list[selection[0]].name)
    if answer is None:
        return
    answer = clean_input(answer, 13)
    if len(answer) <= 0 or answer in [x.name for x in profile_list]:
        return
    profile_list[selection[0]].name = answer
    update_profile_display()

def validate_data_objs(save_path):
    # update path and indexs of profile and keys
    for profile_index, this_profile in enumerate(profile_list):
        this_profile.path = os.path.join(save_path, 'profile'+str(profile_index+1)+'_'+str(this_profile.name))
        for key_index, this_key in enumerate(this_profile.keylist):
            if this_key is None:
                continue
            this_key.path = os.path.join(this_profile.path, 'key'+str(key_index+1)+'.txt')
            this_key.path_on_release = os.path.join(this_profile.path, 'key'+str(key_index+1)+'-release.txt')
            this_key.index = key_index + 1

def make_final_script(ds_key, pgm_listing):
    final_listing = []
    if ds_key.allow_abort:
        final_listing.append("$_ALLOW_ABORT = 1")
    if ds_key.dont_repeat:
        final_listing.append("$_DONT_REPEAT = 1")
    final_listing += pgm_listing
    return final_listing

def compile_all_scripts():
    try:
        for this_profile in profile_list:
            for this_key in this_profile.keylist:
                if this_key is not None:
                    this_key.binary_array = make_bytecode.make_dsb_with_exception(make_final_script(this_key, this_key.script.lstrip().split('\n')), profile_list)
                    if len(this_key.script_on_release.lstrip()) > 0:
                        this_key.binary_array_on_release = make_bytecode.make_dsb_with_exception(make_final_script(this_key, this_key.script_on_release.lstrip().split('\n')), profile_list)
                    if len(this_key.binary_array) >= 65500 or (this_key.binary_array_on_release is not None and len(this_key.binary_array_on_release) >= 65500):
                        messagebox.showerror("Error", f'Script size too large!\n\nProfile: {this_profile.name}\nKey: {this_key.name}')
                        return False
        return True
    except Exception as e:
        error_msg = "Code contains error!\n"
        error_msg += f"Profile [{this_profile.name}] Key [{this_key.name}]:\n"
        error_msg += str(e)
        messagebox.showerror("Error", error_msg)
    return False

def copy_keymaps(dest_path):
    source_keymap_folder = "keymaps"
    destination_keymap_folder = os.path.join(dest_path, "keymaps")
    if not os.path.isdir(source_keymap_folder):
        return
    ensure_dir(destination_keymap_folder)
    for item in [str(x.path) for x in os.scandir(source_keymap_folder) if 'dpkm_' in x.path.lower() and x.path.lower().endswith('.txt')]:
        destination_file = os.path.join(destination_keymap_folder, os.path.basename(item))

        if not os.path.exists(destination_file):
            print("Copying keymap:", item, destination_keymap_folder)
            shutil.copy2(item, destination_keymap_folder)
        else:
            print(f"keymap already exists: {destination_file}")

def save_everything(save_path):
    if compile_all_scripts() is False:
        return False
    dp_root_folder_display.set("Saving...")
    root.update()
    try:
        validate_data_objs(save_path)
        ensure_dir(save_path)
        my_dirs = [d for d in os.listdir(save_path) if os.path.isdir(os.path.join(save_path, d))]
        my_dirs = [x for x in my_dirs if x.startswith('profile') and x[7].isnumeric() and '_' in x]
        my_dirs = [os.path.join(save_path, d) for d in my_dirs if d.startswith("profile")]
        for item in my_dirs:
            try:
                status_message = f"Deleting {item}..."
                print(status_message)
                dp_root_folder_display.set(status_message)
                root.update()
                shutil.rmtree(item)
                time.sleep(0.05)
            except FileNotFoundError:
                continue
        for this_profile in profile_list:
            os.mkdir(this_profile.path)
            time.sleep(0.05)

            config_file = open(os.path.join(this_profile.path, 'config.txt'), 'w', encoding='utf8', newline='')
            for this_key in this_profile.keylist:
                if this_key is None:
                    continue
                config_file.write(f"z{this_key.index} {this_key.name}\n")
                if this_key.name_line2 is not None and len(this_key.name_line2) > 0:
                    config_file.write(f"x{this_key.index} {this_key.name_line2}\n")
                if this_key.allow_abort:
                    config_file.write(f"ab {this_key.index}\n")
                if this_key.dont_repeat:
                    config_file.write(f"dr {this_key.index}\n")

            config_file.write('BG_COLOR %d %d %d\n' % (this_profile.bg_color))
            if this_profile.kd_color is not None:
                config_file.write('KEYDOWN_COLOR %d %d %d\n' % (this_profile.kd_color))
            if this_profile.dim_unused is False:
                config_file.write('DIM_UNUSED_KEYS 0\n')
            if this_profile.is_landscape:
                config_file.write('IS_LANDSCAPE 1\n')
            for key_index, this_key in enumerate(this_profile.keylist):
                if this_key is None:
                    continue
                # newline='' is important, it forces python to not write \r, only \n
                # otherwise it will read back as double \n
                with open(this_key.path, 'w', encoding='utf8', newline='') as key_file:
                    status_message = f"Writing {this_key.path}..."
                    print(status_message)
                    dp_root_folder_display.set(status_message)
                    root.update()
                    key_file.write(this_key.script)
                
                if this_key.path_on_release is not None and len(this_key.script_on_release) > 0:
                    with open(this_key.path_on_release, 'w', encoding='utf8', newline='') as key_file:
                        status_message = "Writing {this_key.path_on_release}..."
                        print(status_message)
                        dp_root_folder_display.set(status_message)
                        root.update()
                        key_file.write(this_key.script_on_release)

                dsb_path = this_key.path
                pre, ext = os.path.splitext(dsb_path)
                dsb_path = pre + '.dsb'
                dsb_path_onrelease = pre + "-release.dsb"

                with open(dsb_path, 'wb') as dsb_file:
                    status_message = f"Writing {dsb_path}..."
                    print(status_message)
                    dp_root_folder_display.set(status_message)
                    root.update()
                    dsb_file.write(this_key.binary_array)
                if this_key.binary_array_on_release is not None:
                    with open(dsb_path_onrelease, 'wb') as dsb_file:
                        status_message = f"Writing {dsb_path_onrelease}..."
                        print(status_message)
                        dp_root_folder_display.set(status_message)
                        root.update()
                        dsb_file.write(this_key.binary_array_on_release)
                if this_key.color is not None:
                    config_file.write('SWCOLOR_%d %d %d %d\n' % (this_key.index, this_key.color[0], this_key.color[1], this_key.color[2]))
            config_file.close()
        
        copy_keymaps(save_path)

        return True
    except Exception as e:
        error_msg = f"Save Failed:\n\n{e}"
        messagebox.showerror("Error", error_msg)
        dp_root_folder_display.set("Save FAILED!")
    return False

def make_default_backup_dir_name():
    return 'duckyPad_backup_' + datetime.now().strftime("%Y-%m-%dT%H-%M-%S")

def save_click():
    this_backup_path = os.path.join(backup_path, make_default_backup_dir_name())
    if save_everything(this_backup_path) is False:
        messagebox.showerror("Error", "Backup save failed")
        return
    try:
        if os.path.isdir(dp_root_folder_path) is False:
            put_duckypad_in_msc_mode_and_get_drive_path(reset_ui=False)
        my_compare.duckypad_file_sync(dp_root_folder_path, this_backup_path)
        if is_root_folder_duckypad_msc:
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

def backup_button_click():
    messagebox.showinfo("Backups", "All your backups are here!\n\nCopy back to SD card to restore")
    if 'darwin' in sys.platform:
        subprocess.Popen(["open", backup_path])
    elif 'linux' in sys.platform:
        subprocess.Popen(["xdg-open", backup_path])
    else:
        webbrowser.open(backup_path)

def key_button_click_event(event):
    key_button_click(event.widget)

root = Tk()
root.title("duckyPad Configurator v" + THIS_VERSION_NUMBER)
root.geometry(str(MAIN_WINDOW_WIDTH) + "x" + str(MAIN_WINDOW_HEIGHT))
root.resizable(width=FALSE, height=FALSE)
profile_list = []

on_press_release_rb_var = IntVar()
on_press_release_rb_var.set(0)

def get_correct_script_text(key_obj):
    if on_press_release_rb_var.get() == 1:
        return key_obj.script_on_release.lstrip().rstrip('\r\n')
    return key_obj.script.lstrip().rstrip('\r\n')

def key_button_click(button_widget):
    global last_rgb
    global selected_key
    if len(profile_lstbox.curselection()) <= 0:
        return
    key_name_textbox.config(state=NORMAL)
    profile_index = profile_lstbox.curselection()[0]
    selected_key = key_button_list.index(button_widget)
    reset_key_button_relief()
    this_borderwidth = scaled_size(7)
    if is_rotary_encoder_button(selected_key) or is_expansion_button(selected_key):
        this_borderwidth = scaled_size(5)
    button_widget.config(borderwidth=this_borderwidth, relief='sunken')
    key_name_textbox.delete('1.0', 'end')
    on_press_release_rb_var.set(0)
    thissss_key = profile_list[profile_index].keylist[selected_key]
    if thissss_key is not None:
        scripts_lf.place(x=scaled_size(750), y=scaled_size(50))
        empty_script_label.place_forget()
        this_key_name = make_key_button_text_from_two_lines(thissss_key.name, thissss_key.name_line2)
        key_name_textbox.insert(1.0, this_key_name)
        script_textbox.delete(1.0, 'end')
        # script_textbox.tag_remove("error", '1.0', 'end')
        script_text = get_correct_script_text(thissss_key)
        script_textbox.insert(1.0, script_text)
        if len(thissss_key.script_on_release) > 0:
            on_release_rb.configure(fg='green4')
        else:
            on_release_rb.configure(fg='gray20')
    else:
        scripts_lf.place_forget()
        empty_script_label.place(x=scaled_size(800), y=scaled_size(200))
        key_color_button.config(background=default_button_color)
        custom_key_color_checkbox.deselect()
        custom_key_color_checkbox.config(state=DISABLED)
        allow_abort_checkbox.config(state=DISABLED)
        dont_repeat_checkbox.config(state=DISABLED)
        script_textbox.delete(1.0, 'end')
        return

    custom_key_color_checkbox.config(state=NORMAL)
    allow_abort_checkbox.config(state=NORMAL)
    dont_repeat_checkbox.config(state=NORMAL)
    if thissss_key.color is None:
        custom_key_color_checkbox.deselect()
        key_color_button.config(background=default_button_color)
    else:
        custom_key_color_checkbox.select()
        last_rgb = thissss_key.color
        key_color_button.config(background=rgb_to_hex(thissss_key.color))

    if thissss_key.allow_abort:
        allow_abort_checkbox.select()
    else:
        allow_abort_checkbox.deselect()

    if thissss_key.dont_repeat:
        dont_repeat_checkbox.select()
    else:
        dont_repeat_checkbox.deselect()
    check_syntax()

# ------------- Folder select -------------
dp_root_folder_display = StringVar()
dp_root_folder_path= ''
dp_root_folder_display.set(INVALID_ROOT_FOLDER_STRING)

root_folder_lf = LabelFrame(root, text="Files", width=scaled_size(1050), height=HEIGHT_ROOT_FOLDER_LF)
root_folder_lf.place(x=PADDING, y=0)
root.update()

root_folder_select_button = Button(root_folder_lf, text="Connect", command=connect_button_click)
root_folder_select_button.place(x=scaled_size(5), y=0, width=scaled_size(75), height=scaled_size(25))

root_folder_path_label = Label(master=root_folder_lf, textvariable=dp_root_folder_display, foreground='red')
root_folder_path_label.place(x=scaled_size(90), y=0)

save_button = Button(root_folder_lf, text="Save", command=save_click, state=DISABLED)
save_button.place(x=scaled_size(630+270), y=0, width=scaled_size(65), height=scaled_size(25))

backup_button = Button(root_folder_lf, text="Backup...", command=backup_button_click)
backup_button.place(x=scaled_size(700+270), y=0, width=scaled_size(65), height=scaled_size(25))

# ------------- Profiles frame -------------

profile_var = StringVar()
profiles_lf = LabelFrame(root, text="Profiles", width=scaled_size(260), height=scaled_size(473))
profiles_lf.place(x=PADDING, y=HEIGHT_ROOT_FOLDER_LF)
root.update()

profile_lstbox = Listbox(profiles_lf, listvariable=profile_var, height=16, exportselection=0) #, selectmode='single'?
profile_lstbox.place(x=scaled_size(32), y=PADDING, width=scaled_size(182), height=scaled_size(270))
profile_lstbox.bind('<<ListboxSelect>>', on_profile_lstbox_select)

profile_up_button = Button(profiles_lf, text="↑", command=profile_shift_up, state=DISABLED)
profile_up_button.place(x=scaled_size(5), y=scaled_size(60), width=scaled_size(20), height=scaled_size(40))

profile_remove_button = Button(profiles_lf, text="X", command=profile_remove_click, state=DISABLED)
profile_remove_button.place(x=scaled_size(5), y=scaled_size(110), width=scaled_size(20), height=scaled_size(40))

profile_down_button = Button(profiles_lf, text="↓", command=profile_shift_down, state=DISABLED)
profile_down_button.place(x=scaled_size(5), y=scaled_size(160), width=scaled_size(20), height=scaled_size(40))

BUTTON_WIDTH = int(profiles_lf.winfo_width() / 2.5)
BUTTON_HEIGHT = scaled_size(25)
BUTTON_Y_POS = scaled_size(295)

profile_add_button = Button(profiles_lf, text="New", command=profile_add_click, state=DISABLED)
profile_add_button.place(x=PADDING*2, y=BUTTON_Y_POS, width=BUTTON_WIDTH, height=BUTTON_HEIGHT)

profile_dupe_button = Button(profiles_lf, text="Duplicate", command=profile_dupe_click, state=DISABLED)
profile_dupe_button.place(x=PADDING * 2.5 + BUTTON_WIDTH, y=BUTTON_Y_POS, width=BUTTON_WIDTH, height=BUTTON_HEIGHT)

profile_rename_button = Button(profiles_lf, text="Rename", command=profile_rename_click, state=DISABLED)
profile_rename_button.place(x=PADDING * 2.5 + BUTTON_WIDTH, y=BUTTON_Y_POS + BUTTON_HEIGHT + int(PADDING/2), width=BUTTON_WIDTH, height=BUTTON_HEIGHT)

bg_color_label = Label(master=profiles_lf, text="Background color:")
bg_color_label.place(x=scaled_size(20), y=scaled_size(355))

bg_color_button = Label(master=profiles_lf, borderwidth=1, relief="solid")
bg_color_button.place(x=scaled_size(160), y=scaled_size(356), width=scaled_size(60), height=scaled_size(20))
bg_color_button.bind("<Button-1>", bg_color_click)

kd_color_label = Label(master=profiles_lf, text="Activation color:")
kd_color_label.place(x=scaled_size(20), y=scaled_size(380))

kd_color_button = Label(master=profiles_lf, borderwidth=1, relief="solid")
kd_color_button.place(x=scaled_size(160), y=scaled_size(407), width=scaled_size(60), height=scaled_size(20))
kd_color_button.bind("<Button-1>", kd_color_click)

dim_unused_keys_checkbox_var = IntVar()
dim_unused_keys_checkbox = Checkbutton(profiles_lf, text="Dim unused keys", variable=dim_unused_keys_checkbox_var, command=dim_unused_keys_click, state=DISABLED)
dim_unused_keys_checkbox.place(x=scaled_size(20), y=scaled_size(425))

kd_color_var = IntVar()
kd_R1 = Radiobutton(profiles_lf, text="      Auto", variable=kd_color_var, value=0, command=kd_radiobutton_auto_click, state=DISABLED)
kd_R1.place(x=scaled_size(130), y=scaled_size(380))
kd_R2 = Radiobutton(profiles_lf, text="", variable=kd_color_var, value=1, command=kd_radiobutton_custom_click, state=DISABLED)
kd_R2.place(x=scaled_size(130), y=scaled_size(405))

# ------------- RE frame -----------------
re_lf = LabelFrame(root, text="Rotary Encoders", width=scaled_size(150), height=scaled_size(205))
re_lf.place(x=scaled_size(590), y=scaled_size(50))
root.update()
RE_BUTTON_WIDTH = scaled_size(80)
RE_BUTTON_HEIGHT = scaled_size(25)

# ------------- Keys frame -------------
selected_key = None

keys_lf = LabelFrame(root, text="Onboard Switches", width=scaled_size(300), height=scaled_size(330))
keys_lf.place(x=profiles_lf.winfo_x() + profiles_lf.winfo_width() + PADDING, y=profiles_lf.winfo_y())
root.update()

def rotate_keys_click():
    global profile_list
    selection = profile_lstbox.curselection()
    if len(selection) <= 0:
        return
    profile_list[selection[0]].is_landscape = bool(is_in_landscape_var.get())
    update_key_button_appearances(selection[0])
    if(is_key_selected()):
        key_button_click(key_button_list[selected_key])

is_in_landscape_var = IntVar()
rotate_keys_checkbox = Checkbutton(keys_lf, text="Rotate", variable=is_in_landscape_var, command=rotate_keys_click, state=DISABLED)
rotate_keys_checkbox.place(x=scaled_size(10), y=scaled_size(0))

key_instruction_label = Label(master=keys_lf, text="click to edit, drag to rearrange")
root.update()
key_instruction_label.place(x=scaled_size(100), y=scaled_size(0))

def search_button(rootx, rooty):
    for index, item in enumerate(key_button_list):
        xstart = item.winfo_rootx()
        xend = item.winfo_rootx() + item.winfo_width()
        ystart = item.winfo_rooty()
        yend = item.winfo_rooty() + item.winfo_height()
        if xstart < rootx < xend and ystart < rooty < yend:
            return index
    return None

drag_source_button_index = None
drag_destination_button_index = None
def button_drag_start(event):
    global drag_source_button_index
    global drag_destination_button_index
    if len(profile_lstbox.curselection()) <= 0:
        return
    profile_index = profile_lstbox.curselection()[0]
    drag_source_button_index = key_button_list.index(event.widget)
    # if empty button
    if profile_list[profile_index].keylist[drag_source_button_index] is None:
        drag_source_button_index = None
        drag_destination_button_index = None
        return
    drag_destination_button_index = search_button(event.x_root, event.y_root)
    if drag_source_button_index == drag_destination_button_index:
        drag_source_button_index = None
        drag_destination_button_index = None
        return
    reset_key_button_relief()
    event.widget.config(borderwidth=7, relief='sunken')

    if drag_source_button_index is not None and drag_destination_button_index is not None and drag_destination_button_index != drag_source_button_index:
        move_here_text = "move\nhere"
        if is_rotary_encoder_button(drag_destination_button_index) or is_expansion_button(drag_destination_button_index):
            move_here_text = "move"
        key_button_list[drag_destination_button_index].config(text=move_here_text, background='white', foreground='black', borderwidth=4)
    else:
        update_key_button_appearances(profile_index)

def update_keylist_index():
    if len(profile_lstbox.curselection()) <= 0:
        return
    profile_index = profile_lstbox.curselection()[0]
    for index, item in enumerate(profile_list[profile_index].keylist):
        if item is not None:
            item.index = index + 1

def button_drag_release(event):
    global drag_source_button_index
    global drag_destination_button_index
    if len(profile_lstbox.curselection()) <= 0:
        return
    if drag_source_button_index == drag_destination_button_index:
        return
    # print('source:', drag_source_button_index)
    # print('destination:', drag_destination_button_index)
    # print('------')
    profile_index = profile_lstbox.curselection()[0]
    update_key_button_appearances(profile_index)
    reset_key_button_relief()
    if drag_source_button_index is not None and drag_destination_button_index is not None:
        profile_list[profile_index].keylist[drag_destination_button_index], profile_list[profile_index].keylist[drag_source_button_index] = profile_list[profile_index].keylist[drag_source_button_index], profile_list[profile_index].keylist[drag_destination_button_index]
        update_profile_display()
        update_keylist_index()
    if drag_source_button_index is not None and drag_destination_button_index is None:
        key_button_click(key_button_list[drag_source_button_index])
    if drag_destination_button_index is not None:
        key_button_click(key_button_list[drag_destination_button_index])
    drag_source_button_index = None
    drag_destination_button_index = None

KEY_BUTTON_HEADROOM = scaled_size(20)
KEY_BUTTON_WIDTH = scaled_size(60)
KEY_BUTTON_HEIGHT = scaled_size(50)
KEY_BUTTON_GAP = 12
COL_GAP = 6

key_button_xy_list = [
# ROW 0
(KEY_BUTTON_GAP,KEY_BUTTON_HEADROOM+COL_GAP),
(KEY_BUTTON_GAP*2+KEY_BUTTON_WIDTH,KEY_BUTTON_HEADROOM+COL_GAP),
(KEY_BUTTON_GAP*3+KEY_BUTTON_WIDTH*2,KEY_BUTTON_HEADROOM+COL_GAP),
(KEY_BUTTON_GAP*4+KEY_BUTTON_WIDTH*3,KEY_BUTTON_HEADROOM+COL_GAP),
# ROW 1
(KEY_BUTTON_GAP,KEY_BUTTON_HEADROOM+COL_GAP*2+KEY_BUTTON_HEIGHT),
(KEY_BUTTON_GAP*2+KEY_BUTTON_WIDTH,KEY_BUTTON_HEADROOM+COL_GAP*2+KEY_BUTTON_HEIGHT),
(KEY_BUTTON_GAP*3+KEY_BUTTON_WIDTH*2,KEY_BUTTON_HEADROOM+COL_GAP*2+KEY_BUTTON_HEIGHT),
(KEY_BUTTON_GAP*4+KEY_BUTTON_WIDTH*3,KEY_BUTTON_HEADROOM+COL_GAP*2+KEY_BUTTON_HEIGHT),
# ROW 2
(KEY_BUTTON_GAP,KEY_BUTTON_HEADROOM+COL_GAP*3+KEY_BUTTON_HEIGHT*2),
(KEY_BUTTON_GAP*2+KEY_BUTTON_WIDTH,KEY_BUTTON_HEADROOM+COL_GAP*3+KEY_BUTTON_HEIGHT*2),
(KEY_BUTTON_GAP*3+KEY_BUTTON_WIDTH*2,KEY_BUTTON_HEADROOM+COL_GAP*3+KEY_BUTTON_HEIGHT*2),
(KEY_BUTTON_GAP*4+KEY_BUTTON_WIDTH*3,KEY_BUTTON_HEADROOM+COL_GAP*3+KEY_BUTTON_HEIGHT*2),
# ROW 3
(KEY_BUTTON_GAP,KEY_BUTTON_HEADROOM+COL_GAP*4+KEY_BUTTON_HEIGHT*3),
(KEY_BUTTON_GAP*2+KEY_BUTTON_WIDTH,KEY_BUTTON_HEADROOM+COL_GAP*4+KEY_BUTTON_HEIGHT*3),
(KEY_BUTTON_GAP*3+KEY_BUTTON_WIDTH*2,KEY_BUTTON_HEADROOM+COL_GAP*4+KEY_BUTTON_HEIGHT*3),
(KEY_BUTTON_GAP*4+KEY_BUTTON_WIDTH*3,KEY_BUTTON_HEADROOM+COL_GAP*4+KEY_BUTTON_HEIGHT*3),
# ROW 4
(KEY_BUTTON_GAP,KEY_BUTTON_HEADROOM+COL_GAP*5+KEY_BUTTON_HEIGHT*4),
(KEY_BUTTON_GAP*2+KEY_BUTTON_WIDTH,KEY_BUTTON_HEADROOM+COL_GAP*5+KEY_BUTTON_HEIGHT*4),
(KEY_BUTTON_GAP*3+KEY_BUTTON_WIDTH*2,KEY_BUTTON_HEADROOM+COL_GAP*5+KEY_BUTTON_HEIGHT*4),
(KEY_BUTTON_GAP*4+KEY_BUTTON_WIDTH*3,KEY_BUTTON_HEADROOM+COL_GAP*5+KEY_BUTTON_HEIGHT*4)
]

key_button_list = []
for x in range(MECH_OBSW_COUNT):
    this_button = Label(master=keys_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
    this_button.bind("<Button-1>", key_button_click_event)
    this_button.bind("<B1-Motion>", button_drag_start)
    this_button.bind("<ButtonRelease-1>", button_drag_release)
    key_button_list.append(this_button)

def place_obsw_buttons_portrait():
    key_char_limit_label.config(text=key_char_limit_portrait)
    for index in range(MECH_OBSW_COUNT):
        key_button_list[index].place(x=key_button_xy_list[index][0], y=key_button_xy_list[index][1], width=KEY_BUTTON_WIDTH, height=KEY_BUTTON_HEIGHT)

def place_obsw_buttons_landscape():
    ROTATED_BUTTON_WIDTH_HEIGHT = 53
    ROTATED_BUTTON_GAP = 5
    ROTATED_BUTTON_Y_START = 45

    key_char_limit_label.config(text=key_char_limit_landscape)

    key_button_list[3].place(x=scaled_size(ROTATED_BUTTON_GAP), y=scaled_size(ROTATED_BUTTON_Y_START), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[7].place(x=scaled_size(ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT), y=scaled_size(ROTATED_BUTTON_Y_START), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[11].place(x=scaled_size(ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*2), y=scaled_size(ROTATED_BUTTON_Y_START), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[15].place(x=scaled_size(ROTATED_BUTTON_GAP*4 + ROTATED_BUTTON_WIDTH_HEIGHT*3), y=scaled_size(ROTATED_BUTTON_Y_START), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[19].place(x=scaled_size(ROTATED_BUTTON_GAP*5 + ROTATED_BUTTON_WIDTH_HEIGHT*4), y=scaled_size(ROTATED_BUTTON_Y_START), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)

    key_button_list[2].place(x=scaled_size(ROTATED_BUTTON_GAP), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP + ROTATED_BUTTON_WIDTH_HEIGHT), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[6].place(x=scaled_size(ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP + ROTATED_BUTTON_WIDTH_HEIGHT), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[10].place(x=scaled_size(ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*2), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP + ROTATED_BUTTON_WIDTH_HEIGHT), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[14].place(x=scaled_size(ROTATED_BUTTON_GAP*4 + ROTATED_BUTTON_WIDTH_HEIGHT*3), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP + ROTATED_BUTTON_WIDTH_HEIGHT), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[18].place(x=scaled_size(ROTATED_BUTTON_GAP*5 + ROTATED_BUTTON_WIDTH_HEIGHT*4), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP + ROTATED_BUTTON_WIDTH_HEIGHT), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)

    key_button_list[1].place(x=scaled_size(ROTATED_BUTTON_GAP), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT*2), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[5].place(x=scaled_size(ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT*2), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[9].place(x=scaled_size(ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*2), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT*2), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[13].place(x=scaled_size(ROTATED_BUTTON_GAP*4 + ROTATED_BUTTON_WIDTH_HEIGHT*3), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT*2), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[17].place(x=scaled_size(ROTATED_BUTTON_GAP*5 + ROTATED_BUTTON_WIDTH_HEIGHT*4), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT*2), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)

    key_button_list[0].place(x=scaled_size(ROTATED_BUTTON_GAP), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*3), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[4].place(x=scaled_size(ROTATED_BUTTON_GAP*2 + ROTATED_BUTTON_WIDTH_HEIGHT), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*3), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[8].place(x=scaled_size(ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*2), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*3), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[12].place(x=scaled_size(ROTATED_BUTTON_GAP*4 + ROTATED_BUTTON_WIDTH_HEIGHT*3), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*3), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)
    key_button_list[16].place(x=scaled_size(ROTATED_BUTTON_GAP*5 + ROTATED_BUTTON_WIDTH_HEIGHT*4), y=scaled_size(ROTATED_BUTTON_Y_START+ROTATED_BUTTON_GAP*3 + ROTATED_BUTTON_WIDTH_HEIGHT*3), width=ROTATED_BUTTON_WIDTH_HEIGHT, height=ROTATED_BUTTON_WIDTH_HEIGHT)

upper_re_cw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_cw.place(x=scaled_size(60), y=scaled_size(5), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_cw.bind("<Button-1>", key_button_click_event)
upper_re_cw.bind("<B1-Motion>", button_drag_start)
upper_re_cw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(upper_re_cw)
r1cw_label = Label(master=re_lf, text="RE1 CW")
r1cw_label.place(x=scaled_size(0), y=scaled_size(7))

upper_re_ccw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_ccw.place(x=scaled_size(60), y=scaled_size(35), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_ccw.bind("<Button-1>", key_button_click_event)
upper_re_ccw.bind("<B1-Motion>", button_drag_start)
upper_re_ccw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(upper_re_ccw)
r1ccw_label = Label(master=re_lf, text="RE1 CCW")
r1ccw_label.place(x=scaled_size(0), y=scaled_size(37))

upper_re_sw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
upper_re_sw.place(x=scaled_size(60), y=scaled_size(65), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
upper_re_sw.bind("<Button-1>", key_button_click_event)
upper_re_sw.bind("<B1-Motion>", button_drag_start)
upper_re_sw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(upper_re_sw)
r1btn_label = Label(master=re_lf, text="RE1 Push")
r1btn_label.place(x=scaled_size(0), y=scaled_size(67))

lower_re_cw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_cw.place(x=scaled_size(60), y=scaled_size(95), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_cw.bind("<Button-1>", key_button_click_event)
lower_re_cw.bind("<B1-Motion>", button_drag_start)
lower_re_cw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(lower_re_cw)
r2cw_label = Label(master=re_lf, text="RE2 CW")
r2cw_label.place(x=scaled_size(0), y=scaled_size(97))

lower_re_ccw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_ccw.place(x=scaled_size(60), y=scaled_size(125), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_ccw.bind("<Button-1>", key_button_click_event)
lower_re_ccw.bind("<B1-Motion>", button_drag_start)
lower_re_ccw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(lower_re_ccw)
r2ccw_label = Label(master=re_lf, text="RE2 CCW")
r2ccw_label.place(x=scaled_size(0), y=scaled_size(127))

lower_re_sw = Label(master=re_lf, borderwidth=1, relief="solid", background=default_button_color, font=(None, 13))
lower_re_sw.place(x=scaled_size(60), y=scaled_size(155), width=RE_BUTTON_WIDTH, height=RE_BUTTON_HEIGHT)
lower_re_sw.bind("<Button-1>", key_button_click_event)
lower_re_sw.bind("<B1-Motion>", button_drag_start)
lower_re_sw.bind("<ButtonRelease-1>", button_drag_release)
key_button_list.append(lower_re_sw)
r2btn_label = Label(master=re_lf, text="RE2 Push")
r2btn_label.place(x=scaled_size(0), y=scaled_size(157))

# unused, just to pad it out
for x in range(ONBOARD_SPARE_GPIO_COUNT):
    this_button = Label(master=keys_lf)
    key_button_list.append(this_button)

name_editor_lf = LabelFrame(root, text="Key Config", width=scaled_size(300), height=scaled_size(143))
name_editor_lf.place(x=profiles_lf.winfo_x() + profiles_lf.winfo_width() + PADDING, y=scaled_size(380))
root.update()

key_char_limit_portrait = "Name:\nmax 2 lines\n5 char per line"
key_char_limit_landscape = "Name:\nmax 2 lines\n4 char per line"

key_char_limit_label = Label(master=name_editor_lf)
key_char_limit_label.place(x=scaled_size(17), y=scaled_size(0))
root.update()

def keyname_textbox_modified_event(event):
    key_rename_click()
    key_name_textbox.tk.call(key_name_textbox._w, 'edit', 'modified', 0)

key_name_textbox = Text(name_editor_lf, state=DISABLED, wrap="word")
key_name_textbox.place(x=scaled_size(107), y=scaled_size(5), width=scaled_size(80), height=scaled_size(40))
key_name_textbox.bind("<<Modified>>", keyname_textbox_modified_event)

def get_clean_key_name_2lines(user_text):
    split_line = user_text.replace('\r', '').split('\n')[:2]
    line1_clean = ''
    line2_clean = ''
    try:
        line1_clean = clean_input(split_line[0], KEY_NAME_MAX_CHAR_PER_LINE, is_filename=False)
    except:
        pass
    try:
        line2_clean = clean_input(split_line[1], KEY_NAME_MAX_CHAR_PER_LINE, is_filename=False)
    except:
        pass
    return line1_clean, line2_clean

def key_rename_click():
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    keyname_line1, keyname_line2 = get_clean_key_name_2lines(key_name_textbox.get("1.0", END))
    if len(keyname_line1) == 0:
        return
    if profile_list[profile_index].keylist[selected_key] is not None:
        profile_list[profile_index].keylist[selected_key].name = keyname_line1
        profile_list[profile_index].keylist[selected_key].name_line2 = keyname_line2
    else:
        new_key = duck_objs.dp_key()
        new_key.name = keyname_line1
        new_key.name_line2 = keyname_line2
        profile_list[profile_index].keylist[selected_key] = new_key
        update_keylist_index()
        key_button_click(key_button_list[selected_key])
    update_key_button_appearances(profile_index)
    
def key_remove_click():
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    profile_list[profile_index].keylist[selected_key] = None
    update_key_button_appearances(profile_index)
    key_button_click(key_button_list[selected_key])

key_remove_button = Button(name_editor_lf, text="Remove\nKey", command=key_remove_click, state=DISABLED, fg="red")
key_remove_button.place(x=scaled_size(200), y=scaled_size(5), width=scaled_size(80), height=scaled_size(40))

def is_key_selected():
    if len(profile_lstbox.curselection()) <= 0:
        return False
    if selected_key is None:
        return False
    return True

def key_color_button_click(event):
    global last_rgb
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is not None:
        # Color picker should have an initial colour set in colour picker
        initial_color = profile_list[profile_index].keylist[selected_key].color if profile_list[profile_index].keylist[selected_key].color is not None else profile_list[profile_index].bg_color
        result = askcolor(color=initial_color, title="Key color for " + profile_list[profile_index].keylist[selected_key].name)[0]
        if result is None:
            return
        last_rgb = result
        profile_list[profile_index].keylist[selected_key].color = result
    update_key_button_appearances(profile_index)
    key_button_click(key_button_list[selected_key])

def custom_key_color_click():
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    if key_color_type_var.get():
        profile_list[profile_index].keylist[selected_key].color = last_rgb
    else:
        profile_list[profile_index].keylist[selected_key].color = None
    update_key_button_appearances(profile_index)
    key_button_click(key_button_list[selected_key])

def allow_abort_click():
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    profile_list[profile_index].keylist[selected_key].allow_abort = allow_abort_var.get()
    print(profile_list[profile_index].keylist[selected_key].allow_abort)

def dont_repeat_click():
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    profile_list[profile_index].keylist[selected_key].dont_repeat = dont_repeat_var.get()
    print(profile_list[profile_index].keylist[selected_key].dont_repeat)

key_color_type_var = IntVar()
custom_key_color_checkbox = Checkbutton(name_editor_lf, text="Custom Key Color", variable=key_color_type_var, command=custom_key_color_click, state=DISABLED)
custom_key_color_checkbox.place(x=scaled_size(15), y=scaled_size(55))

allow_abort_var = IntVar()
allow_abort_checkbox = Checkbutton(name_editor_lf, text="Press Any Key to Abort", variable=allow_abort_var, command=allow_abort_click, state=DISABLED)
allow_abort_checkbox.place(x=scaled_size(15), y=scaled_size(75))

dont_repeat_var = IntVar()
dont_repeat_checkbox = Checkbutton(name_editor_lf, text="Disable Auto-Repeat", variable=dont_repeat_var, command=dont_repeat_click, state=DISABLED)
dont_repeat_checkbox.place(x=scaled_size(15), y=scaled_size(95))

key_color_button = Label(master=name_editor_lf, borderwidth=1, relief="solid")
key_color_button.place(x=scaled_size(150), y=scaled_size(57), width=scaled_size(60), height=scaled_size(20))
key_color_button.bind("<Button-1>", key_color_button_click)
root.update()
# ------------- Scripts frame -------------
scripts_lf = LabelFrame(root, text="Scripts", width=scaled_size(310), height=scaled_size(473))

def open_duckyscript_url():
    webbrowser.open('https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/duckyscript_info.md')

def script_instruction_click(event):
    open_duckyscript_url()

script_instruction = Label(master=scripts_lf, text="Read more about Duckyscript...", fg="blue", cursor="hand2")
root.update()
script_instruction.place(x=scaled_size(60), y=0)
script_instruction.bind("<Button-1>", script_instruction_click)

last_textbox_edit = 0
def script_textbox_modified():
    global last_textbox_edit
    if is_key_selected() == False:
        return
    last_textbox_edit = time.time()
    profile_index = profile_lstbox.curselection()[0]
    thissss_key = profile_list[profile_index].keylist[selected_key]
    if thissss_key is None:
        return
    if len(thissss_key.script_on_release) > 0:
        on_release_rb.configure(fg='green4')
    else:
        on_release_rb.configure(fg='gray20')
    current_text = script_textbox.get(1.0, END).lstrip()
    if on_press_release_rb_var.get():
        thissss_key.script_on_release = current_text
    else:
        thissss_key.script = current_text
    
def script_textbox_event(event):
    script_textbox_modified()
    script_textbox.tk.call(script_textbox._w, 'edit', 'modified', 0)

script_textbox = Text(scripts_lf, relief='solid', borderwidth=1, padx=2, pady=2, spacing3=5, wrap="word")
script_textbox.place(x=PADDING, y=scaled_size(50), width=scaled_size(285), height=scaled_size(360))
root.update()
script_textbox.bind("<<Modified>>", script_textbox_event)
script_textbox.tag_configure("error", background="#ffff00")

def on_press_rb_click():
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    script_textbox.delete(1.0, 'end')
    # script_textbox.tag_remove("error", '1.0', 'end')
    script_textbox.insert(1.0, profile_list[profile_index].keylist[selected_key].script.lstrip().rstrip('\r\n'))

is_onrelease_warning_shown = 0

def on_release_rb_click():
    global is_onrelease_warning_shown
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    if is_onrelease_warning_shown == 0:
        messagebox.showinfo("howdy!", "For advanced users only!\n\nLeave blank if unsure\n\nRead guide for more info")
        is_onrelease_warning_shown = 1
    script_textbox.delete(1.0, 'end')
    # script_textbox.tag_remove("error", '1.0', 'end')
    script_textbox.insert(1.0, profile_list[profile_index].keylist[selected_key].script_on_release.lstrip().rstrip('\r\n'))

on_press_rb = Radiobutton(scripts_lf, text="On Press", variable=on_press_release_rb_var, value=0, command=on_press_rb_click)
on_press_rb.place(x=scaled_size(50), y=scaled_size(20))
on_release_rb = Radiobutton(scripts_lf, text="On Release", variable=on_press_release_rb_var, value=1, command=on_release_rb_click)
on_release_rb.place(x=scaled_size(150), y=scaled_size(20))
root.update()

def find_index(lst, query):
    for index, item in enumerate(lst):
        if query in item:
            return index
    return None        

last_check_syntax_program_listing = []
def check_syntax():
    global last_check_syntax_program_listing
    if is_key_selected() == False:
        return
    profile_index = profile_lstbox.curselection()[0]
    if profile_list[profile_index].keylist[selected_key] is None:
        return
    program_listing = profile_list[profile_index].keylist[selected_key].script.split('\n')
    if on_press_release_rb_var.get() == 1:
        program_listing = profile_list[profile_index].keylist[selected_key].script_on_release.split('\n')
    if program_listing == last_check_syntax_program_listing:
        # print("check_syntax: same")
        return
    result_dict, bin_arr = make_bytecode.make_dsb_no_exception(program_listing, profile_list)
    last_check_syntax_program_listing = program_listing.copy()
    script_textbox.tag_remove("error", '1.0', 'end')
    if result_dict is None:
        check_syntax_label.config(text="Code seems OK..", fg="green")       
        return
    error_lnum = find_index(program_listing, result_dict['line_content'])
    error_text = result_dict['comments']
    if error_lnum is not None:
        error_lnum += 1
        script_textbox.tag_add("error", str(error_lnum)+".0", str(error_lnum)+".0 lineend")
    else:
        error_text = f"Error on line: {result_dict['line_content']}\n{result_dict['comments']}"
    check_syntax_label.config(text=error_text, fg='red')

check_syntax_label = Label(scripts_lf, text="")
check_syntax_label.place(x=scaled_size(10), y=scaled_size(417))
root.update()

resources_lf = LabelFrame(root, text="Resources", width=scaled_size(516+214), height=scaled_size(70))
resources_lf.place(x=scaled_size(10), y=scaled_size(525))

updates_lf = LabelFrame(root, text="Updates", width=scaled_size(310), height=scaled_size(70))
updates_lf.place(x=scaled_size(750), y=scaled_size(525))

pc_app_update_label = Label(master=updates_lf)
pc_app_update_label.place(x=scaled_size(5), y=scaled_size(0))
update_stats = check_update.get_pc_app_update_status(THIS_VERSION_NUMBER)

if update_stats == 0:
    pc_app_update_label.config(text='This app (' + str(THIS_VERSION_NUMBER) + '): Up to date', fg='black', bg=default_button_color)
    pc_app_update_label.unbind("<Button-1>")
elif update_stats == 1:
    pc_app_update_label.config(text='This app (' + str(THIS_VERSION_NUMBER) + '): Update available! Click me!', fg='black', bg='orange', cursor="hand2")
    pc_app_update_label.bind("<Button-1>", app_update_click)
else:
    pc_app_update_label.config(text='This app (' + str(THIS_VERSION_NUMBER) + '): Unknown', fg='black', bg=default_button_color)
    pc_app_update_label.unbind("<Button-1>")

dp_fw_update_label = Label(master=updates_lf, text="Firmware: Unknown")
dp_fw_update_label.place(x=scaled_size(5), y=scaled_size(25))

def open_profile_autoswitcher_url():
    webbrowser.open('https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/README.md')

def import_profile_click():
    global profile_list
    # messagebox.showinfo("Import", f"Select a folder containing the profiles")
    import_path = filedialog.askdirectory()
    if len(import_path) <= 0:
        return
    is_success, content = duck_objs.import_profile(import_path)
    if is_success is False:
        messagebox.showinfo("Import", f"Import failed:\n\n{content}")
        return
    profile_list += content
    update_profile_display()

def open_tindie_store():
    webbrowser.open('https://dekunukem.github.io/duckyPad-Pro/doc/store_links.html')

user_manual_button = Button(resources_lf, text="User\nManual", command=open_duckypad_user_manual_url)
user_manual_button.place(x=scaled_size(10), y=scaled_size(0), width=scaled_size(100))

autoswitch_button = Button(resources_lf, text="Profile\nAutoswitcher", command=open_profile_autoswitcher_url)
autoswitch_button.place(x=scaled_size(150), y=scaled_size(0), width=scaled_size(100))

discord_button = Button(resources_lf, text="Discord\nChatroom", command=open_discord_link)
discord_button.place(x=scaled_size(300), y=scaled_size(0), width=scaled_size(100))

troubleshoot_button = Button(resources_lf, text="Troubleshooting\nGuide", command=open_duckypad_troubleshooting_url)
troubleshoot_button.place(x=scaled_size(450), y=scaled_size(0), width=scaled_size(100))

tindie_button = Button(resources_lf, text="Accessories &\nUpgrades", command=open_tindie_store)
tindie_button.place(x=scaled_size(600), y=scaled_size(0), width=scaled_size(100))

profile_import_button = Button(profiles_lf, text="Import", command=import_profile_click, state=DISABLED)
profile_import_button.place(x=PADDING * 2, y=BUTTON_Y_POS + BUTTON_HEIGHT + int(PADDING/2), width=BUTTON_WIDTH, height=BUTTON_HEIGHT)

empty_script_label = Label(root, text="<-- Select a key")
empty_script_label.place(x=scaled_size(800), y=scaled_size(200))
root.update()

# ------------- Expansion frame ------------

current_selected_expansion_module = 0

def exp_page_update():
    current_module_label.config(text=f"Module {current_selected_expansion_module+1}")
    for index,item in enumerate(key_button_list):
        if is_expansion_button(index):
            item.place(x=scaled_size(999), y=scaled_size(999)) # place_forget doesnt seem to work
            item.config(relief="solid")
            item.config(borderwidth=1)
            root.update()

    this_module_start = EXP_BUTTON_START + current_selected_expansion_module * CHANNELS_PER_EXPANSION_MODULE
    key_button_list[this_module_start+0].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 0 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+1].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 1 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+2].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 2 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+3].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 3 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+4].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 4 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+5].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 5 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+6].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 6 - 2), width=CH_BUTTON_WIDTH, height=25)
    key_button_list[this_module_start+7].place(x=scaled_size(ch_button_x), y=scaled_size(chy_start + chy_step * 7 - 2), width=CH_BUTTON_WIDTH, height=25)
    scripts_lf.place_forget()
    empty_script_label.place(x=scaled_size(800), y=scaled_size(200))
    key_name_textbox.delete('1.0', 'end')
    key_name_textbox.config(state=DISABLED)
    custom_key_color_checkbox.deselect()
    custom_key_color_checkbox.config(state=DISABLED)
    allow_abort_checkbox.config(state=DISABLED)
    dont_repeat_checkbox.config(state=DISABLED)
    update_profile_display()
    root.update()

def exp_page_plus_button_click():
    global current_selected_expansion_module
    current_selected_expansion_module = (current_selected_expansion_module + 1) % MAX_EXPANSION_MODULE_COUNT
    exp_page_update()

def exp_page_minus_button_click():
    global current_selected_expansion_module
    current_selected_expansion_module = (current_selected_expansion_module - 1) % MAX_EXPANSION_MODULE_COUNT
    exp_page_update()


expansion_lf = LabelFrame(root, text="Expansion Modules", width=scaled_size(150), height=scaled_size(263))
expansion_lf.place(x=scaled_size(590), y=scaled_size(260))
root.update()


exp_page_minus_button = Button(expansion_lf, text="-", command=exp_page_minus_button_click, state=DISABLED)
exp_page_minus_button.place(x=scaled_size(10), y=scaled_size(0), width=scaled_size(25), height=scaled_size(22))

exp_page_plus_button = Button(expansion_lf, text="+", command=exp_page_plus_button_click, state=DISABLED)
exp_page_plus_button.place(x=scaled_size(110), y=scaled_size(0), width=scaled_size(25), height=scaled_size(22))

current_module_label = Label(master=expansion_lf)
current_module_label.place(x=scaled_size(45), y=scaled_size(0))

root.update()

ch_label_x = 5
chy_start = 30
chy_step = 27

EXPCH0_label = Label(master=expansion_lf, text="CH1")
EXPCH0_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 0))

EXPCH1_label = Label(master=expansion_lf, text="CH2")
EXPCH1_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 1))

EXPCH2_label = Label(master=expansion_lf, text="CH3")
EXPCH2_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 2))

EXPCH3_label = Label(master=expansion_lf, text="CH4")
EXPCH3_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 3))

EXPCH4_label = Label(master=expansion_lf, text="CH5")
EXPCH4_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 4))

EXPCH5_label = Label(master=expansion_lf, text="CH6")
EXPCH5_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 5))

EXPCH5_label = Label(master=expansion_lf, text="CH7")
EXPCH5_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 6))

EXPCH5_label = Label(master=expansion_lf, text="CH8")
EXPCH5_label.place(x=scaled_size(ch_label_x), y=scaled_size(chy_start + chy_step * 7))

ch_button_x = 45
CH_BUTTON_WIDTH = 85

# expansion module buttons
for mmm in range(MAX_EXPANSION_MODULE_COUNT):
    for ccc in range(CHANNELS_PER_EXPANSION_MODULE):
        this_ch_button = Label(expansion_lf, relief="solid")
        this_ch_button.bind("<Button-1>", key_button_click_event)
        this_ch_button.bind("<B1-Motion>", button_drag_start)
        this_ch_button.bind("<ButtonRelease-1>", button_drag_release)
        key_button_list.append(this_ch_button)

current_selected_expansion_module = 0
exp_page_update()

root.update()

# --------------------

def repeat_func():
    if time.time() - last_textbox_edit >= 0.75:
        check_syntax()
    root.after(500, repeat_func)

root.after(500, repeat_func)

# select_root_folder("sample_profiles")
my_compare.tk_root = root
my_compare.tk_strvar = dp_root_folder_display
root.mainloop()
