# Getting Started with duckyPad Pro

[Support on Kickstarter!](https://www.kickstarter.com/projects/dekunukem/duckypad-pro-advanced-macro-scripting-beyond-qmk-via) | [Official Discord](https://discord.gg/4sJCBx5) | [Getting Started](./doc/getting_started.md) | [Table of Contents](#table-of-contents)

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

#### Windows

![Alt text](../resources/photos/underc.gif)

Windows might complain about unsigned app.

Click `More info` and then `Run anyway`.

Feel free to [review the files](../pc_software). You can also run the source code directly with Python.

![Alt text](../resources/photos/app/defender.png)

#### Mac

![Alt text](../resources/photos/underc.gif)

#### Linux

![Alt text](../resources/photos/underc.gif)

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

Let's make a new profile!

Click `New` button, give it a name.

![Alt text](../resources/photos/app/pname.png)

* Choose a background color for the RGB backlight.

* Use the `up/down/x` buttons to reorder/delete profiles.

* `Activation color` is the color that a key changes into when you press it.

### Keys

The middle columns are for *keys*:

![Alt text](../resources/photos/app/keys.png)

* Select an empty key in the `Onboard Switches` section
* Type to give it a name
* Drag to rearrange

---------

* **`Custom Key Color`**: Set an unique color.

* **`Press Any Key to Abort`**: Allow exiting early in longer macros.

	* Do not select if you're writing your own button-checking code

* **`Disable Auto-Repeat`**: Macro won't repeat when key is held down.

* Click **`Rotate`** for landscape mode.

### Scripts

Finally, the rightmost column is for *scripts*:

![Alt text](../resources/photos/app/scripts.png)

* This shows the **duckyScript** that the key will execute.

	* Can be anything from simple shortcuts to full-blown programs!

	* [**Read about detailed usage here**](duckyscript_info.md).

	* For this example, put in `STRING Hello World!`

* Separate scripts for **On Press** and **On Release**

	* If not sure, just leave **On Release** blank.

* Code check will be performed as you type.

### Remember to Save!

* Press `Save` button to write the changes back to duckyPad.

* It might take a while to write new data

* Subsequent writes will be much faster

### Try It Out!

duckyPad should reboot after saving.

Press `+/-` button to go to the new profile, and press the key.

It should execute the script and type out `Hello World!`:

![Alt text](../resources/photos/app/hello.gif)

That's it! Now you know how to set up your duckyPad!

🎉 🎉 Great Job, but we're not done yet!

⚠️⚠️ **Please keep reading** for more useful information!

------------

### Rotary Encoders

You can assign duckyScript actions to Rotary Encoders just like any other key.

Some ideas:

* Volume Up/Down
* Zoom In/Out
* Undo/Redo
* Brush Size

### External Switches

You can also wire up your own **Buttons / Switches / Foot pedals** using the optional **Expansion Module**.

[Click me for details](https://github.com/dekuNukem/duckyPad-expansion-modules)

### Profile Auto-Switching

You can also **switch profiles automatically** based on **current active window**.

[Click me for details](https://github.com/dekuNukem/duckyPad-profile-autoswitcher)

### Careful with Passwords!

It might be tempting to have duckyPad type out passwords, but beware that all scripts are stored as **plain text** on SD card. Not a good idea!

### Automatic backups

* A local backup is created every time you save.
* Press `Backup...` button to access the backups.
* To restore, [format the SD card in FAT or FAT32](../resources/photos/app/format.png), then copy everything back.

## Tips and Tricks

### Keyboard Shortcuts

The most obvious use case is putting your commonly used hotkeys on duckyPad!

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

Then assign the combo on duckyPad!

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