# Getting Started with duckyPad

[Get it on Kickstarter!](https://www.kickstarter.com/projects/dekunukem/duckypad-pro-advanced-macro-scripting-beyond-qmk-via) | [Official Discord](https://discord.gg/4sJCBx5) | [Getting Started](./doc/getting_started.md) | [Table of Contents](#table-of-contents)

------

## Join Our Discord!

Feel free to join our [Official Discord](https://discord.gg/4sJCBx5) for discussions and latest updates!

## Kit Assembly

[See this guide](./kit_assembly.md) to put your duckyPad together!

## Quick Start

Check that power switch is ON:

![Alt text](../resources/photos/onoff.png)

Plug it in, use the **UPPER** USB port, and voilà:

![Alt text](../resources/photos/discord.jpeg)

* Press a key to execute script.

* Press `+` and `-` to switch profiles.

* **`Hold down`** `+` or `-` for settings

![Alt text](../resources/photos/settings.png)

* Select the correct **keyboard region**

* Adjust others to your liking

* Press any key to exit

### Bluetooth Mode

* To use BT, plug into a **USB power bank**.

* BT status is displayed on **top left corner**

* Question mark = Discoverable

![alt_text](../resources/photos/app/bt_searching.png)

* It should show up on your computer, make sure the PIN matches.

![alt_text](../resources/photos/bt.jpeg)

* No question mark = Paired

![alt_text](../resources/photos/app/bt_paired.png)

* Should work the same as wired mode.

## Create Your Own Scripts

After playing with examples and see what duckyPad can do, time to write your own!

### Download Configurator

[Head here to download the latest release](link here).

#### Windows

Unzip, click `duckypad_config.exe`:

![Alt text](../resources/photos/app/duckexe.png)

#### Mac

![Alt text](../resources/photos/underc.gif)

#### Linux

![Alt text](../resources/photos/underc.gif)

### "Untrusted App" Warnings

Your system might complain when launching the app.

This is because the code isn't digitally signed, which costs hundreds of dollars a year.

Feel free to [review the files](../pc_software). You can also run the source code directly with Python.

For Windows 10, click `More info` and then `Run anyway`.

![Alt text](../resources/photos/app/defender.png)

For macOS, **`RIGHT CLICK`** on the app and select `Open`. You might need to do it twice.

![Alt text](../resources/photos/app/macos_warning.png)

### Using Configurator

Press the `Connect` button near top left.

![Alt text](../resources/photos/app/connect.png)

duckyPad Pro should show up as a USB storage drive.

Everything should load up. Might take a few seconds.

![Alt text](../resources/photos/app/overview.png)

It may seem complex at first, let's break it down!

### Profiles

* The leftmost column is for *profiles*.

* You typically create one for **each app** you want to control.

![Alt text](../resources/photos/app/profiles.png)

* Use the buttons to create, duplicate, name, and import a profile.

* Use the up/down/x button to reorder/delete profiles.

* Choose a background color for the RGB backlight.

* `Activation color` is the color that a key changes into when you press it.

### Keys

The middle columns are for *keys*:

![Alt text](../resources/photos/app/keys.png)

* Shows all the keys for the current profile.

* Click to select, drag to rearrange.

* Type to give a name, or click `Remove` to delete.

----------

* **`Custom Key Color`**: Set an unique color.

* **`Press Any Key to Abort`**: Allow exiting early in longer macros.

	* Do not select if you're writing your own button-checking code

* **`Disable Auto-Repeat`**: Macro won't repeat when key is held down.

* Click **`Rotate`** for landscape mode.

### Scripts

Finally, the rightmost column is for *scripts*:

![Alt text](../resources/photos/app/scripts.png)

* Shows the **duckyScript** that the key will execute.

* [**Read about its usage here**](duckyscript_info.md).

* Separate scripts for **On Press** and **On Release**

* If not sure, just leave **On Release** blank.

* Code check will be performed as you type, errors will be highlighted in yellow.

### Make Sure to Save!

* Press `Save` button to write the changes back to duckyPad.

![Alt text](../resources/photos/app/save.png)

### Automatic backups

* A local backup is created every time you save.

* Press `Backup...` button to access the backups.

* To restore, [format the SD card in FAT or FAT32](../resources/photos/app/format.png), then copy everything back.

### Profile Auto-switching / Remote Control

You can [use this app](https://github.com/dekuNukem/duckyPad-profile-autoswitcher) to **switch profiles automatically** based on **current active window**.

You can also **control duckyPad from PC** by sending HID commands, see same link for details.

### Careful with Passwords!

It might be tempting to have duckyPad type out passwords, but all scripts are stored as **plain text** on SD card. So not a good idea!

duckyPad is NOT designed or certified to be a security device, so use at your own risk.

## Tips and Tricks

### Keyboard Shortcuts

The most obvious use case is putting your commonly used hotkeys on duckyPad!

Create a profile and add them in.

For many applications, you can find an official list of shortcuts. Just search `app_name shortcuts` on Google. Examples:

[Firefox](https://support.mozilla.org/en-US/kb/keyboard-shortcuts-perform-firefox-tasks-quickly) and [Chrome](https://support.google.com/chrome/answer/157179?co=GENIE.Platform%3DDesktop)

[Windows](https://support.microsoft.com/en-gb/help/12445/windows-keyboard-shortcuts) and [macOS](https://support.apple.com/en-gb/HT201236)

[Word](https://support.microsoft.com/en-us/office/keyboard-shortcuts-in-word-95ef89dd-7142-4b50-afb2-f762f663ceb2), [Excel](https://support.microsoft.com/en-us/office/keyboard-shortcuts-in-excel-1798d9d5-842a-42b8-9c99-9b7213f0040f), [PowerPoint](https://support.microsoft.com/en-us/office/use-keyboard-shortcuts-to-create-powerpoint-presentations-ebb3d20e-dcd4-444f-a38e-bb5c5ed180f4), and other [Office apps](https://support.microsoft.com/en-us/office/keyboard-shortcuts-in-office-e765366f-24fc-4054-870d-39b214f223fd).

[Photoshop](https://helpx.adobe.com/uk/photoshop/using/default-keyboard-shortcuts.html), [Final Cut Pro](https://support.apple.com/en-gb/guide/final-cut-pro/ver90ba5929/mac), and [Premiere Pro](https://helpx.adobe.com/uk/premiere-pro/using/keyboard-shortcuts.html).

### Launching Apps on Windows - Task Bar

Another popular usage is launching apps. The easiest way is using Task Bar:

Find the app, `Right click -> More -> Pin to taskbar`:

![Alt text](../resources/photos/app/start.png)

Now you can use `WIN + number` to launch them:

![Alt text](../resources/photos/app/taskbar.png)

In duckyScript, it would be `WINDOWS 1`, `WINDOWS 2`, etc.

### Launching Apps on Windows - Shortcuts

This method works with **`ANY FILE`**, not just apps!

`Right click` on any file, select `Create shortcut`.

![Alt text](../resources/photos/app/shortcut.png)

Find the new shortcut, right click and select `Properties`

![Alt text](../resources/photos/app/right_prop.png)

Set a hotkey in the `Shortcut Key` box:

![Alt text](../resources/photos/app/sc_prop.png)

Use duckyPad to press the combo to launch anything with a push of a button.

In this case, the duckyScript would be `CONTROL ALT D`.

### Launching Apps on macOS

Go to `System Preferences` -> `Keyboard` -> `Shortcuts` -> `App Shortcuts`:

Press `+` button, select app, assign a hotkey, and set up duckyPad accordingly:

![Alt text](../resources/photos/app/mac_sc.png)

### Advanced Scripting with Autohotkey

You can use the free and open-source [Autohotkey](https://www.autohotkey.com) for even more advanced scripting needs.

duckyPad can press a simple combo like `WIN + F1`, which triggers autohotkey to execute a more complex script on your PC.

* Download and install [autohotkey](https://www.autohotkey.com)

* Download and save [this sample autohotkey script](../resources/duckypad_pro_autohotkey_script.ahk) as an `.ahk` file.

* Activate the script by right clicking and select `Run Script`.

![Alt text](../resources/photos/app/ahk_run.png)

* Switch to `autohotkey` profile, press a key.

* Corresponding message box will appear.

![Alt text](../resources/photos/app/ahk_box.png)

* Write your own AHK script!

Check out the [official tutorials](https://www.autohotkey.com/docs_1.0/Tutorial.htm), and how to [launch AHK on startup](https://www.autohotkey.com/docs/FAQ.htm#Startup).

For macOS, you can try [bettertouchtool](https://folivora.ai).

## I'm Having Issues!

Please take a look at the [Common issues / Troubleshooting](troubleshooting.md) guide.

If that doesn't help, feel free to contact me by [opening an issue](https://github.com/dekuNukem/duckypad/issues) or ask in the [official duckyPad discord](https://discord.gg/4sJCBx5).

## USB Firmware Updates

You can update duckyPad's firmware for new features and bug fixes. 

[Please see this guide](./fw_update.md).

## Table of Contents

[Main page](../README.md)

[Getting Started Guide](getting_started.md)

[Kit Assembly Guide](kit_assembly.md)

[Using duckyScript](duckyscript_info.md)

[duckyScript VM](bytecode_vm.md)

[Tinkering Guide](tinkering_guide.md)

[Troubleshooting](troubleshooting.md)

[Firmware Update](fw_update.md)

## Questions or Comments?

Please feel free to [open an issue](https://github.com/dekuNukem/duckypad-pro/issues), ask in the [official duckyPad discord](https://discord.gg/4sJCBx5), or email `dekuNukem`@`gmail`.`com`!