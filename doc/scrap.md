
* Early October

* 21 days

* Similar to [last time](https://www.kickstarter.com/projects/dekunukem/duckypad-do-it-all-mechanical-macropad). Early bird rewards for immediately shipping.

* Rest of the rewards December / early 2025.



### Even Further

* Explore the full potential with **advanced interactive scripting**
* Read Buttons
* Write to OLED screen
* Change RGB colors
* Conditions and Loops
* Write ultra-specific macros for your exact needs

```
RGB_SET 1 128 0 255

OLED_PRINT You are in a maze
OLED_PRINT of twisty little passages

VAR $choice = $_READKEY

IF $choice == 1 THEN
    OLED_CURSOR 0 10
    OLED_PRINT It is a dead end.
    OLED_PRINT Something moves
    OLED_PRINT behind you.
END_IF
```

![alt_text](resources/photos/maze.jpeg)



--------

Introducing **duckyPad Pro**, the second entry to the duckyPad family!

With 4 years worth of feedback, long-requested upgrades, brand new features, and free from the shackles of QMK/VIA, duckyPad Pro aims to be the **most capable** macropad available today.

![alt_text](../resources/photos/title.jpeg)

## Keep in Touch

If you're interested, please consider:

* Signing up [Kickstarter Pre-launch Page](https://www.kickstarter.com/projects/dekunukem/duckypad-pro-advanced-macro-scripting-beyond-qmk-via) to get notified!

* Joining our [Official Discord](https://discord.gg/4sJCBx5) for latest discussions and updates!

![alt_text](../resources/photos/quarter.jpeg)

## What's New

Compared to [original duckyPad](https://github.com/dekuNukem/duckyPad/), we now have:

* **Rotary Encoders (finally!)**
	* Two encoders
	* Built-in tactile switch
	* Six duckyScript actions

![alt_text](../resources/photos/re.jpeg)

* **Bluetooth**
	* Now works as BT keyboard & Mouse!
	* Bluetooth 5.0
	* Built-in Antenna for both BT and Wi-Fi

![alt_text](../resources/photos/bt.jpeg)

![alt_text](../resources/photos/ipad.gif)

* **Expansion Modules**
	* Wire up your own **switches / buttons / foot pedals!**
	* **Daisy-chainable**

![alt_text](../resources/photos/exp.gif)

* **Bigger OLED Screen**
	* 128x128
	* **Portrait/landscape orientation**

* **Faster CPU**
	* ESP32-S3
	* 5x faster
	* Much simpler setup for tinkerers

* **More Keys**
	* 20 Switches
	* In 4x5 Grid

* **duckyScript Engine**
	* Separate script for **on press** and **on release**
	* **Abort execution** with any key
	* **Unlimited** `EMUK` Keypresses
	* Read `NUMLOCK` / `CAPSLOCK` / `SCROLLLOCK` status
	* Switch profile by name (instead of number)
	* Much longer & more complex scripting than QMK/VIA
	* 64 Profiles, 3712 macros.
	* Many under-the-hood optimizations

* **QoL Updates**
	* Mount SD Card as **USB Storage**
	* Much faster configurator load/save
	* **Drag & drop** firmware update
	* More sturdy +/- button
	* Reinforced hotswap socket solder pads

* **New Configurator**
	* New UI for new features!
	* General cleanup

![alt_text](../resources/photos/config.png)

[<img src="../resources/photos/youtube.png">](https://www.youtube.com/watch?v=uzL-kk1gB_Y)

--------

![alt_text](../resources/photos/re.jpeg)

![alt_text](../resources/photos/ipad.gif)

![alt_text](../resources/photos/exp.gif)

![alt_text](../resources/photos/youtube.png)

<table>
  <tr>
    <td><a href="https://www.youtube.com/watch?v=uzL-kk1gB_Y" title="YouTube" rel="noopener"><img src="https://i.imgur.com/767Nnyt.png"></a></td>
  </tr>
</table>


Four years after the original duckyPad, I'm happy to announce **duckyPad Pro**!

	* Many small improvements (migration guide?)

this doc covers whats new, kickstarter plans, pricing, and future plans

(landscape/portrait)

p make_toc.py ../doc/duckyscript_info.md

#### (OPTIONAL) Load from SD card

If app complains, you can also load from local files.

* Push the SD card inwards to pop it out

* Mount on your computer

* Select the **`entire SD card`**

![Alt text](../resources/photos/app/select_root.png)

* It should load just the same

* Built-in
	* Comments
	* Timing and Delay
	* Typing
	* Profiles
	* Sleep
	* KEYUP KEYDOWN

* Special Keys

* Mouse
	* Movement
	* Scroll
	* Buttons

* OLED
	* Clear Screen
	* Set Cursor
	* Print Text
	* Draw Line
	* Draw Rectangle
	* Draw Circle
	* Restore Display

* RGB
	* Fill Color
	* Set Color
	* Restore color

* Randomisation

* Calculation
	* Constants
	* Variables
	* Operators
	* Built-in variables

* Reading Buttons
	* Blocking
	* Non-blocking

* Flow control
	* IF
	* WHILE
	* Functions
	* HALT


### `EMUK`

Emulates a regular key.

It holds a key when you press it, and release only when you release it. Makes it behave more like a traditional keyboard.

Possible uses include push-to-talk voice chat, or WASD gamepad.

You should only use `EMUK` command **on its own**, i.e. the script should only have a single line of `EMUK` command and **nothing else**.

### `REPEAT`

Repeats the last line **`n`** times.

```
STRING Hello world
REPEAT 10
// types out "Hello world" 11 times (1 original + 10 repeats)
```

For more advanced usage, use functions and `WHILE` loops in [duckyScript 3](duckyscript3_instructions.md).

---------

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