# duckyPad Software macOS / Linux Notes

[Get duckyPad Pro](https://www.tindie.com/products/37399/) | [Official Discord](https://discord.gg/4sJCBx5) | [Getting Started](./getting_started.md) | [Table of Contents](#table-of-contents)

---------

## macOS

On macOS, you can run the configurator as a **pre-packaged app**, or execute the **source code** directly.

### macOS: Pre-packaged App

* Download the latest **macOS** release:

  * [Configurator](https://github.com/duckyPad/duckyPad-Configurator/releases/latest)

  * [Profile Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/releases/latest)

* Double click to unzip

* **Right Click** on the folder, select `New Terminal at Folder`

![The right click menu with the option "New Terminal at Folder" higlighted.](../resources/photos/app/term.png)

* Type `sh ./run.sh`, press enter.

![The text to run the configurator shown in a terminal window.](../resources/photos/app/run.png)

* Type your password. The letters won't appear, but press enter and the configurator should launch!

> [!IMPORTANT]
> Use this to start the app every time, it grants the configurator permission to connect to the duckyPad.

---------

* If macOS complains about unsigned app

* Go to Settings > Privacy & Security

* Scroll down to security section and click "Allow Anyway" 

![Warning pop-up with the text: "duckypad_config cannot be opened because it is from an unidentified developer. macOS cannot verify that this app is free from malware."](../resources/photos/app/unknown.png)

![Settings screen in Privacy & Security, the warning "‘duckypad_config’ was blocked from use because it is not from an identified developer," is shown above a button with the text "Allow Anyway".](../resources/photos/app/perf.png)

---------

If the configurator complains about **needing additional permissions:**

* Try granting "Input Monitoring" permissions for the `Terminal app`.

  ![Error pop-up with the text "duckypad detected, but I need additional permissions" and three options: Yes, No, and Cancel.](../resources/photos/app/detected-but-additional-permissions.png)
  
  ![Input Monitoring settings granted to the Terminal and configurator app](../resources/photos/app/input-monitoring.png)

* If still not working, try "Full Disk Access" or "Developer Tools".

* If everything else fails, you can also edit your duckypad by manually entering USB mount mode (hold +, select key 6) and selecting the newly appeared SD card.

### macOS: Run from Source

* Download the latest source:
  * [Configurator](https://github.com/duckyPad/duckyPad-Configurator/releases/latest)
  * [Profile Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/releases/latest)
* Double click to unzip
* **Right Click** on the folder, select `New Terminal at Folder`

----

* Use [homebrew](https://brew.sh) to install python-tk: `brew install python-tk`
* **[Optional]** Make a virtualenv and activate it: `python3 -m venv .venv; source .venv/bin/activate`
* Install dependencies: `pip3 install -r requirements.txt`
* Launch the app: `sudo python3 ./duckypad_config.py`

-----------

## Linux

You can run the code from source under Linux.

* Download the latest source:
  * [Configurator](https://github.com/duckyPad/duckyPad-Configurator/releases/latest)
  * [Profile Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/releases/latest)
* Unzip & open a terminal at the directory
* Install tkinter: `sudo apt install python3-tk`
* Install dependencies: `sudo pip3 install -r requirements.txt`
* Launch the app: `sudo DUCKYPAD_UI_SCALE=1 python3 ./duckypad_config.py`
  * For High-DPI screens, adjust `DUCKYPAD_UI_SCALE` environment variable.

If still not working:

* Setup the udev rules
  * Create the file: `sudo touch /etc/udev/rules.d/20-duckyPad.rules`
  * Add the following contents:
      ```
      SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="d11c", TAG+="uaccess", TAG+="udev-acl"
      SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="d11d", TAG+="uaccess", TAG+="udev-acl"
  * Give it the correct permissions: `sudo chmod 644 /etc/udev/rules.d/20-duckyPad.rules`
  * Give it the correct ownership: `sudo chown root:root /etc/udev/rules.d/20-duckyPad.rules`
  * Reload udev rules: `sudo udevadm control --reload-rules`
  * Trigger udev: `sudo udevadm trigger`
```
