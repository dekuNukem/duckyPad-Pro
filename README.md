# duckyPad Pro: Expandable Input Automation

-----------

duckyPad Pro is a 20-key mechanical macropad with unrivaled input automation capability using **duckyScript**.

Incorporating 4 years worth of feedback from the [original duckyPad](link_here), now with **Rotary Encoders**, **Bluetooth**, **Expansion Modules**, and more!

duckyPad Pro is designed for power users who demand advanced macro scripting and customization way beyond QMK/VIA.

![alt_text](resources/photos/title.jpeg)

duckyPad Pro has all the basics:

* Hot-Swap
* Rotary Encoders
* Per-key RGB
* USB-C
* 100% **Open-source**

Along with features **unlike any other**:

* Custom **[duckyScript](duckyscript_info.md)** engine, **NOT QMK/VIA**.
* Much **longer and sophisticated** macros
* Configurable **OLED** screen
* **Expansion Modules** to wire up to 32 external switches
* **Bluetooth**
* microSD storage
* **64 profiles**, 3712 macros total.
* [Automatic profile switching](https://github.com/dekuNukem/duckyPad-profile-autoswitcher) based on active window.
* No proprietary drivers, works out-of-box.
* **Hacker friendly** with tinkering guide!

The macros can be as simple as `Ctrl + C`, or as complex as filling forms, launching applications, or even [creating root backdoors](https://shop.hak5.org/blogs/usb-rubber-ducky/the-3-second-reverse-shell-with-a-usb-rubber-ducky) or [grabbing passwords](https://shop.hak5.org/blogs/usb-rubber-ducky/15-second-password-hack-mr-robot-style). It's all up to you!

## New Features

Compared to [original duckyPad](https://github.com/dekuNukem/duckyPad/), DPP now has:

* **Rotary Encoders**
	* Two encoders
	* Built-in tactile switch

* **More Keys**
	* 20 Switches
	* 4x5 Grid

* **Expansion Modules**
	* **6 Channels** per module
	* Wire up any switch you want
	* Foot pedal, emergency shutoff, toggle switch, etc
	* **Daisy chain** up to **5 Modules** with regular USB-C cable

* **Bigger OLED Screen**
	* 128x128
	* **Configurable orientation**

* **Faster CPU**
	* ESP32-S3
	* 5x faster
	* Much simpler IDE setup for tinkerers

* **Wireless Communication**
	* **Bluetooth** (fully working, add your own battery)
	* Wi-Fi (Capable but unimplemented for now)

* **duckyScript Engine**
	* Separate scripts for **on press** and **on release**
	* **Abort execution** with any key
	* Many small improvements (migration guide?)

* **QoL Updates**
	* Mount SD as **USB Storage**
	* Faster configurator load/save
	* **Drag & drop** firmware update
	* More sturdy +/- button
	* Reinforced hotswap socket solder pads

## duckyScript vs QMK/VIA

duckyPad Pro runs **duckyScript**, originally designed for [USB Rubber Ducky](https://shop.hak5.org/products/usb-rubber-ducky-deluxe).

Compared to QMK/VIA, you get:

* **Much Longer Macros**
	* [Entire Bee Movie](resources/beemovie.txt) in one macro? Yes please!
	* Up to 3712 macros in total

* **Advanced Interactive Scripting**
	* Variables, `IF` statements, loops, functions, etc.
	* Reading Buttons
	* Print to OLED screen
	* Change RGB Color
	* Finer timing control
	* Randomization

* **Local Storage**
	* Everything stored on SD card
	* Quick Configuration / Backup / Duplication
	* No sketchy drivers or apps

* **Existing Resources**
	* Lots of [scripts](https://github.com/hak5/usbrubberducky-payloads) to try out
	* Thriving [Discord](https://discord.gg/4sJCBx5) community

While QMK/VIA are great for regular keyboards, they are rather hopeless at even the basics of input automations.

duckyScript is a no-brainer for power users with more demanding scripting needs.

## Showcase

and pressing shortcuts

![gif_alt_text](gif_here)

autoswitcher

![gif_alt_text](gif_here)

security search

![gif_alt_text](gif_here)

fun with expansion modules

foot panel, emergency shutoff, turnkey switch?

![gif_alt_text](gif_here)

## Features

### Mechanical Switches

### Keycaps

### Hot-swap Sockets

### Per-key RGB

### OLED screen

### duckyScript

### Companion App