# duckyPad Pro: Input Automation for Power Users

-----------

duckyPad Pro is a 20-key mechanical macropad for power users who demand **advanced macro scripting and customisation** beyond the shackles of QMK/VIA.

Incorporating 4 years worth of feedback from the [original duckyPad](link_here), now with **Rotary Encoders**, **Bluetooth**, **Expansion Modules**, and more!

![alt_text](resources/photos/title.jpeg)

duckyPad Pro has all the basics:

* Hot-Swap
* Rotary Encoders
* Per-key RGB
* USB-C
* 100% **Open-source**

But also features **unlike any other**:

* Custom **[duckyScript](duckyscript_info.md)** engine, **NOT QMK/VIA**.
* Much **longer and sophisticated** macros
* Configurable **OLED** screen
* **Expansion Modules** for external switches / foot pedals
* **Bluetooth**
* **64 profiles**, 3712 macros total.
* [Automatic profile switching](https://github.com/dekuNukem/duckyPad-profile-autoswitcher) based on active window.
* microSD storage
* No proprietary drivers, works out-of-box.
* **Hacker friendly** with tinkering guide

The macros can be as simple as `Ctrl + C`, or as complex as filling forms, launching applications, or even [creating root backdoors](https://shop.hak5.org/blogs/usb-rubber-ducky/the-3-second-reverse-shell-with-a-usb-rubber-ducky) or [grabbing passwords](https://shop.hak5.org/blogs/usb-rubber-ducky/15-second-password-hack-mr-robot-style). It's all up to you!

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
	* Fine timing control
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

## How It Works / Showcase

* 64 Profiles, typically one for each app

* 58 Key per profile (20 on-board switches + 6 rotary encoder actions + 32 external switches)

* Write duckyScript for each key

* Shown on OLED screen, show a few screenshots

Starting simple, you can consolidate all the shortcuts to speedup your workflow.

* discord, firefox, photoshop

up a notch to fill forms, type boilerplate text emails

* DHL, email shipped

with expansion module, wire up own switch / foot pedals

* foot pedal to hide/switch window
* emergency switch, key?
* photo here

of course, duckyscript original purpose, hacking pentest

autoswitcher

## Specifications

#### Keys

* 20 Mechanical Switches
* 4x5 Grid
* Cherry-MX style
* Kailh hot-swap socket

#### RGB Backlight

* Per-key configurable
* Adjustable brightness

#### Rotary Encoder

* Two encoders
* Bourns Inc. PEC12R-4215F-S0024
* 24 Detents
* Built-in tactile switch

#### Mounting Plate

* Standard: 1.6mm Pure White FR4 (same material as PCB)
* Optional: 1.6mm Brushed Aluminum

#### Expansion Module

* Wire up any switch you want
* Foot pedal, emergency shutoff, toggle switch, etc
* **6 Channels** per module
* **Daisy chain** up to **5 Modules** with regular USB-C cable

#### OLED screen

* 128x128
* **Configurable orientation**
* Print-to-screen in duckyScript

#### Microcontroller

* ESP32-S3
* Dual-Core @ 240 MHz 
* 4MB Flash Memory
* 512KB RAM
* 2MB PSRAM

#### Wireless Communication

* **Bluetooth** (fully working, add your own battery)
* BLE 4.2
* Wi-Fi (Capable but currently unimplemented)

#### Configurator

* Fully open-source
* Windows / Mac / Linux
* Manage profiles/keys
* Write/debug duckyScript
* No need to create an account
* No data collection whatsoever
* No internet connection required


