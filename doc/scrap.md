Incorporating 4 years worth of feedback from the [original duckyPad](https://github.com/dekuNukem/duckyPad), duckyPad Pro combines long-requested upgrades with a no-nonsense design, aiming simply to be the most capable macropad available today.

 now with **Rotary Encoders**, **Bluetooth**, **Expansion Modules**, and more!


## Specifications

#### Keys

* 20 Mechanical Switches
* 4x5 Grid
* Cherry-MX style
* Kailh hot-swap socket

![alt_text](resources/photos/keyside.jpeg)

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

#### OLED screen

* 128x128
* **Configurable orientation**
* Shows what each key does
* Print-to-screen in duckyScript

screenshots here


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



-----------

### Goto Profile by **NUMBER** (0x01)

Jump to a particular profile.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x01        |
|     3    |   Profile number<br>(**1-indexed**)    |
| 4 ... 63 |        0x00        |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 |             0x00             |



Switches & Keycaps

gat green
box navy
milky white

photo here

RGB Backlight
north

Mounting plate

Rotary encoder

shaft length
knob diameter
D shaped
copy spec sheet here



aimed simply to be the most capable macropad, bar none.


The macros can be as simple as `Ctrl + C`, or as complex as filling forms, launching applications, or even [creating root backdoors](https://shop.hak5.org/blogs/usb-rubber-ducky/the-3-second-reverse-shell-with-a-usb-rubber-ducky) or [grabbing passwords](https://shop.hak5.org/blogs/usb-rubber-ducky/15-second-password-hack-mr-robot-style). It's all up to you!


Zoom In: `CTRL +`

Next Tab: `CTRL SHIFT TAB`


## Wi-Fi

* The hardware for BT also works for Wi-Fi

* But the code is unimplemented right now

* But 


#### Expansion Module

* Wire up any switch you want
* Foot pedal, emergency shutoff, toggle switch, etc
* **6 Channels** per module
* **Daisy chain** up to **5 Modules** with regular USB-C cable


----------



## Specifications

#### Keys

20 Mechanical Switches
4x5 Grid
Cherry-MX style
Kailh hot-swap socket

![alt_text](resources/photos/keyside.jpeg)

#### RGB Backlight

* Per-key configurable
* Adjustable brightness

#### Rotary Encoder

Two encoders
Bourns Inc. PEC12R-4215F-S0024
24 Detents
Built-in tactile switch

#### Mounting Plate

* Standard: 1.6mm Pure White FR4 (same material as PCB)
* Optional: 1.6mm Brushed Aluminum

#### OLED screen

* 128x128
* **Configurable orientation**
* Shows what each key does
* Print-to-screen in duckyScript

#### Microcontroller

ESP32-S3
Dual-Core @ 240 MHz 
4MB Flash Memory
512KB RAM
2MB PSRAM

mention show up as USB drive!