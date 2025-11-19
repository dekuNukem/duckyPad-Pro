# Writing duckyScript

[Get duckyPad Pro](https://www.tindie.com/products/37399/) | [Official Discord](https://discord.gg/4sJCBx5) | [Getting Started](./getting_started.md) | [Table of Contents](#table-of-contents)

------

## Overview

duckyScript is a simple language for **automating keyboard/mouse inputs**.

It was originally developed for [USB Rubber Ducky](https://shop.hak5.org/products/usb-rubber-ducky-deluxe).

## Quick Examples

In the **simplest form**, you just tell it what key to press!

Ideal for key combos:

* New Tab: `CONTROL t`

* Task Manager: `CONTROL SHIFT ESC`

--------

Once familiar, you can write **longer multi-line** scripts for more complex actions.

##### Open Webpage:

```
WINDOWS r
DELAY 500
STRING https://youtu.be/dQw4w9WgXcQ
ENTER
```

---------

At full potential, duckyScript is closer to a **general-purpose language**. You can:

* Use Variables, Conditions, and Loops.
* Read Buttons
* Write to OLED screen
* Change RGB colors
* And more!

This allows writing ultra-specific macros for your exact needs.

## First Time?

There are quite a few commands, and it can be a bit daunting.

The **first few sections** (from `Comments` to `Mouse`) is more than enough to get started.

You can skim through the rest once familiar with the basics.

Also **playing with sample profiles** is a good way to get a feel of duckyScript.

Of course, people at the [Official Discord](https://discord.gg/4sJCBx5) are always happy to help!

## 👉👉 duckyScript Cheatsheet 👈👈

[Click me to download](https://github.com/dekuNukem/duckyPad-Pro/blob/master/doc/duckyscript_cheatsheet.pdf?raw=1) a copy of the quick reference guide!

Much easier to lookup than going through this whole page.

## List of Commands

- [Comments](#comments)
    - [`REM` and `//`](#rem-and-)
    - [`REM_BLOCK` and `END_REM`](#rem_block-and-end_rem)
- [Typing](#typing)
    - [`STRING` and `STRINGLN`](#string-and-stringln)
    - [`STRINGLN_BLOCK` and `END_STRINGLN`](#stringln_block-and-end_stringln)
    - [`STRING_BLOCK` and `END_STRING`](#string_block-and-end_string)
- [Pressing Keys](#pressing-keys)
    - [Special Keys](#special-keys)
    - [`KEYDOWN` / `KEYUP`](#keydown--keyup)
    - [`REPEAT`](#repeat)
- [Timing](#timing)
    - [`DELAY`](#delay)
    - [`DEFAULTDELAY`](#defaultdelay)
    - [`DEFAULTCHARDELAY`](#defaultchardelay)
    - [`CHARJITTER n`](#charjitter-n)
- [Mouse](#mouse)
    - [Mouse Buttons](#mouse-buttons)
    - [`MOUSE_MOVE X Y`](#mouse_move-x-y)
    - [`MOUSE_WHEEL X`](#mouse_wheel-x)
- [Multiple Actions](#multiple-actions)
- [Profile Switching](#profile-switching)
    - [`PREV_PROFILE` / `NEXT_PROFILE`](#prev_profile--next_profile)
    - [`GOTO_PROFILE`](#goto_profile)
    - [Autoswitcher](#autoswitcher)
- [OLED](#oled)
    - [`OLED_CURSOR x y`](#oled_cursor-x-y)
    - [`OLED_PRINT`](#oled_print)
    - [`OLED_CLEAR`](#oled_clear)
    - [`OLED_CIRCLE`](#oled_circle)
    - [`OLED_LINE`](#oled_line)
    - [`OLED_RECT`](#oled_rect)
    - [`OLED_UPDATE`](#oled_update)
    - [`OLED_RESTORE`](#oled_restore)
- [Per-Key RGB](#per-key-rgb)
    - [`SWC_SET n r g b`](#swc_set-n-r-g-b)
    - [`SWC_FILL r g b`](#swc_fill-r-g-b)
    - [`SWC_RESET n`](#swc_reset-n)
- [Constants](#constants)
- [Variables](#variables)
    - [Persistent Global Variables](#persistent-global-variables)
    - [Operators](#operators)
    - [Arguments](#arguments)
- [Advanced Printing](#advanced-printing)
    - [Print Format](#print-format)
    - [Leading Zeros](#leading-zeros)
- [Real-time Clock (RTC)](#real-time-clock-rtc)
    - [Setting RTC](#setting-rtc)
    - [Reading RTC](#reading-rtc)
- [Conditional Statements](#conditional-statements)
- [Loops](#loops)
    - [`LBREAK`](#lbreak)
    - [`CONTINUE`](#continue)
    - [Infinite Loop](#infinite-loop)
- [Functions](#functions)
- [Reading Inputs](#reading-inputs)
    - [Blocking Read](#blocking-read)
    - [Non-Blocking Read](#non-blocking-read)
    - [Key ID](#key-id)
- [Randomisation](#randomisation)
- [Miscellaneous](#miscellaneous)
    - [`DP_SLEEP`](#dp_sleep)
    - [`HALT`](#halt)
- [Reserved Variables](#reserved-variables)

## Comments

### `REM` and `//`

Any line **starting with** those is ignored.

```
REM This is a comment
// This is comment too!
```

### `REM_BLOCK` and `END_REM`

Comment block. Everything in-between is ignored.

```
REM_BLOCK
    Put as much comment here
    as you want!
END_REM
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Typing

### `STRING` and `STRINGLN`

`STRING` types out whatever after it **`AS-IS`**.

```
STRING Hello world!
// types out "Hello world!"
```

`STRINGLN` also presses **enter key** at the end.

### `STRINGLN_BLOCK` and `END_STRINGLN`

Type out everything inside as-is.

Also presses **enter key** at the end of each line.

```
STRINGLN_BLOCK

According to all known laws of aviation,
there is no way a bee should be able to fly.

END_STRINGLN
```

### `STRING_BLOCK` and `END_STRING`

Similar to above, but without new lines.

### Printing Variables / Print Formatting

See [Advanced Printing](#advanced-printing)

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Pressing Keys

### Special Keys

duckyScript supports many special keys.

They can be used on their own:

```
WINDOWS
```

...or combined with a character to form shortcuts:

```
WINDOWS s
```

...or chained even longer:

```
WINDOWS SHIFT s
```

------

* Type the key name **as-is** in **`ALL CAPS`**.

* Keys are pressed in sequence from **left-to-right**, then released **right-to-left**.

------

List of Special Keys:

``` 
  CTRL / RCTRL         |     (media keys)             
  SHIFT / RSHIFT       |     MK_VOLUP                 
  ALT / RALT           |     MK_VOLDOWN               
  WINDOWS / RWINDOWS   |     MK_MUTE                  
  COMMAND / RCOMMAND   |     MK_PREV                  
  OPTION / ROPTION     |     MK_NEXT                  
  ESC                  |     MK_PP (play/pause)       
  ENTER                |     MK_STOP                  
  UP/DOWN/LEFT/RIGHT   |                              
  SPACE                |     (numpad keys)            
  BACKSPACE            |     NUMLOCK                  
  TAB                  |     KP_SLASH                 
  CAPSLOCK             |     KP_ASTERISK              
  PRINTSCREEN          |     KP_MINUS                 
  SCROLLLOCK           |     KP_PLUS                  
  PAUSE                |     KP_ENTER                 
  BREAK                |     KP_0 to KP_9             
  INSERT               |     KP_DOT                   
  HOME                 |     KP_EQUAL                 
  PAGEUP / PAGEDOWN    |                              
  DELETE               |     (Japanese input method)  
  END                  |     ZENKAKUHANKAKU           
  MENU                 |     HENKAN                   
  POWER                |     MUHENKAN                 
  F1 to F24            |     KATAKANAHIRAGANA         

```

### `KEYDOWN` / `KEYUP`

Hold/release a key.

Allows more fine-grained control.

Can be used to input [Alt Codes](https://en.wikipedia.org/wiki/Alt_code) for special characters:

```
// types out ¼
KEYDOWN ALT
KP_1
KP_7
KP_2
KEYUP ALT
```

### `REPEAT`

Repeats the **last line** **`n`** times.

```
STRING Hello world
REPEAT 10
// types out "Hello world" 11 times (1 original + 10 repeats)
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Timing

### `DELAY`

Creates a pause (in milliseconds) in execution.

Useful for **waiting for UI to catch up**.

```
WINDOWS r
DELAY 1000 // wait 1000 milliseconds, or 1 second
STRING cmd
```

### `DEFAULTDELAY`

How long to wait between **`each input-generating command`**.

* Default: 20ms
    * Try 50-100 if computer can't keep up.

```
DEFAULTDELAY 50
// Wait 50ms between each command below

ALT
DOWN
ENTER
```

### `DEFAULTCHARDELAY`

How long to wait between **`each letter`** when **`typing text`**.

* Default: 20ms

```
DEFAULTCHARDELAY 50
// Wait 50ms between each letter 

STRING Hello World!
```

### `CHARJITTER n`

Adds an **additional** random delay between 0 and `n` milliseconds after `each key stroke`.

Can make typing more human-like.

* Set to 0 to disable.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Mouse

### Mouse Buttons

* `LMOUSE`: Click LEFT mouse button

* `RMOUSE`: Click RIGHT mouse button

* `MMOUSE`: Click MIDDLE mouse button

* Can be used with `KEYDOWN` / `KEYUP` commands.

### `MOUSE_MOVE X Y`

Move mouse cursor `X` pixels horizontally, and `Y` pixels vertically.

* For `X`, a positive number moves RIGHT, negative number moves LEFT.

* For `Y`, a positive number moves UP, negative number moves DOWN.

* Set to 0 if no movement needed.

* **Disable mouse acceleration** for pixel-accurate results

### `MOUSE_WHEEL X`

Scroll mouse wheel `X` lines.

* A positive number scrolls UP, negative number scrolls DOWN.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Multiple Actions

`LOOP` command lets you to **assign multiple actions to one key**.

You can use it to toggle / cycle through several actions like this:

```
LOOP0:
STRING first action
ENTER

LOOP1:
STRING second action
ENTER

LOOP2:
STRING third action
ENTER
```

* When pressed, a counter increments, and the script at the corresponding loop is executed.

* Keep the code inside simple!

* For more complex needs, see [Loops](#loops) section below.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Profile Switching

### `PREV_PROFILE` / `NEXT_PROFILE`

Switch to the previous / next profile.

### `GOTO_PROFILE`

Jump to a profile by name. **Case sensitive!**

This ends the current script execution.

```
GOTO_PROFILE NumPad
```

### Autoswitcher

Also try the [Autoswitcher](https://github.com/dekuNukem/duckyPad-profile-autoswitcher) for **switching profile automatically** based on **active window**!

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## OLED

### `OLED_CURSOR x y`

Set where to print on screen.

`x y`: Pixel coordinates between `0` and `127`.

Characters are **7 pixels wide, 10 pixels tall.**

Characters print from **top-left** corner.

### `OLED_PRINT`

`OLED_PRINT hello world!` 

Prints the message into display buffer at current cursor location.

### `OLED_CLEAR`

Clears the display buffer.

### `OLED_CIRCLE`

`OLED_CIRCLE x y radius fill`

* `x y`: Origin coordinate
* `radius`: In Pixels
* `fill`: 0 or 1

### `OLED_LINE`

`OLED_LINE x1 y1 x2 y2`
* `x1, y1`: Starting Point
* `X2, y2`: Ending Point

### `OLED_RECT`

`OLED_RECT x1 y1 x2 y2 fill`

* `x1, y1`: Starting Corner
* `X2, y2`: Ending Corner
* `fill`: 0 or 1

### `OLED_UPDATE`

Actually update the OLED.

You should use `OLED_CLEAR`, `OLED_CURSOR`, `OLED_PRINT`, etc, to set up the display, then use this to print it.

This is **much faster** than updating the whole screen for every change.

### `OLED_RESTORE`

Restore the default profile/key name display. `OLED_UPDATE` **NOT NEEDED**.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Per-Key RGB

### `SWC_SET n r g b`

Change LED color of a switch

Set `n` to 0 for current key.

Set `n` between 1 to 20 for a particular key.

`r, g, b` must be between 0 and 255.

### `SWC_FILL r g b`

Change color of **ALL** LEDs.

`r, g, b` must be between 0 and 255.

### `SWC_RESET n`

Reset the key back to default color.

Set `n` to 0 for current key.

Set `n` from 1 to 20 for a particular key.

Set `n` to 99 for all keys.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Constants

You can use `DEFINE` to, well, define a constant.

It can be either **integer** or **string**.

The content is **replaced AS-IS** during preprocessing, very much like `#define` in C.

```
DEFINE MY_EMAIL example@gmail.com
DEFINE MY_AGE 69

STRING My email is MY_EMAIL!
STRING I'm MY_AGE years old! 
```

Internally, `TRUE` is `1`, and `FALSE` is `0`.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Variables

You can declare a variable using `VAR` command:

```
// Declaration
VAR $spam = 0
VAR $eggs = 10

// Assignment
$spam = 20
```

* Variables must start with dollar sign `$`.

* Variables are **unsigned 16-bit integers**, and can hold values from **0 to 65535**.

    * Treated as signed integers in some commands.

* All variables have **global scope**, and can be referenced **anywhere in the script**.

### Persistent Global Variables

There are 32 pre-defined global variables that provides **non-volatile** data storage.

* `$_GV0` to `$_GV31`
* Available across **all profiles**
* Persists over reboots

### Internal Reserved Variables

Some variables are always available, you can read/write to them to adjust settings.

[Full list](#reserved-variables)

### Operators

You can perform operations on constants and variables.

#### Mathematics

```
=       Assignment
+       Add       
-       Subtract  
*       Multiply  
/       Divide    
%       Modulus   
**      Exponent
```

Example:

```
$spam = 2+3
$spam = $eggs * 10
```

#### Comparison

All comparisons evaluate to **either 0 or 1**.

```
==        Equal                  
!=        Not equal              
>         Greater than           
<         Less than              
>=        Greater than or equal  
<=        Less than or equal   
```

#### Logical 

| Operator |          Name         | Comment                                                |
|:--------:|:---------------------:|--------------------------------------------------------|
|    &&    |      Logical AND      | Evaluates to 1 if BOTH side are non-zero, otherwise 0. |
|   \|\|   |       Logical OR      | Evaluates to 1 if ANY side is non-zero, otherwise 0.   |

#### Bitwise

```
&        Bitwise AND   
|        Bitwise OR    
^        Bitwise XOR
<<       Left Shift    
>>       Right Shift   
```

### Arguments

You can use **constant, variable, or expression** as arguments in commands.

```
VAR $amount = 100
DELAY $amount*2+5
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Advanced Printing

You can print **the value of variables** when using `STRING`, `STRINGLN`, and `OLED_PRINT`:

```
STRING The value is: $spam
```

### Print Format

Write to `$_STR_PRINT_FORMAT` to adjust **how numbers are printed**.

|Value|Format|Example|
|:---:|:---:|:---:|
|0|Decimal Unsigned (default)|`65409`|
|1|Decimal Signed|`-127`|
|2|Hexadecimal Lower Case|`ff81`|
|3|Hexadecimal Upper Case|`FF81`|

```
VAR $foo = 65409

$_STR_PRINT_FORMAT = 0
STRINGLN The value is: $foo

$_STR_PRINT_FORMAT = 1
STRINGLN The value is: $foo

$_STR_PRINT_FORMAT = 2
STRINGLN The value is: $foo

$_STR_PRINT_FORMAT = 3
STRINGLN The value is: $foo
```

```
The value is: 65409
The value is: -127
The value is: ff81
The value is: FF81
```

### Leading Zeros

Write to `$_STR_PRINT_PADDING` to adjust **padding**.

* **Leading zeros** are added if the variable has fewer digits.

* Set to 0 for no padding.

* Works with all print formats.

```
$_STR_PRINT_PADDING = 2

VAR $year = 2025
VAR $month = 8
VAR $day = 5
STRING Date is: $year-$month-$day
```

```
Date is: 2025-08-05
```

## Real-time Clock (RTC)

duckyPad can keep track of **current date and time** for use in scripts.

### Setting RTC

On cold-boot, duckyPad doesn't know what time it is.

It must be set once, after which it will keep time **as long as it is powered-on**.

* RTC is **automatically set** when using the [Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher)

    * A **clock icon** appears when RTC is valid

    ![Alt text](../resources/photos/app/rtc.png)

* You can also set it manually

    * [HID Commands](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/blob/master/HID_details.md)

    * [Sample Script](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/blob/master/hid_example/ex3_set_rtc.py)

### Reading RTC

#### Validity Check

First, read `$_RTC_IS_VALID`. **Do not proceed** if value is 0.

```
IF $_RTC_IS_VALID == 0 THEN
    // RTC is uninitialised, do not proceed.
    HALT
END_IF
```

#### UTC Offset

The RTC always runs in **UTC**.

Local time is obtained by adding an **UTC Offset in `MINUTES`**

* It is **set automatically** to your **local timezone** when using the [Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher).

* You can check (and manually adjust) the offset by reading/writing `$_RTC_UTC_OFFSET` variable

    * Can be positive, 0, or negative.

#### Time and Date

With valid RTC and correct UTC offset, you can now read from the variables below:

| Name      | Comment                | Range |
| ------------- | -------------------------- | --------------- |
| `$_RTC_YEAR`  | **4-digit** Year          | e.g. `2025`    |
| `$_RTC_MONTH` | Month              | `1–12`          |
| `$_RTC_DAY`   | Day              | `1–31`          |
| `$_RTC_HOUR`   | Hour | `0–23` |
| `$_RTC_MINUTE` | Minute                | `0–59` |
| `$_RTC_SECOND` | Second                | `0–60` |
| `$_RTC_WDAY`  | Day of Week (`0 = Sunday`) | `0–6`           |
| `$_RTC_YDAY`  | Day of Year (`0 = Jan 1`) | `0–365`           |

#### Example Usage

```
$_STR_PRINT_PADDING = 2
STRINGLN $_RTC_YEAR-$_RTC_MONTH-$_RTC_DAY $_RTC_HOUR:$_RTC_MINUTE:$_RTC_SECOND
```

```
2025-09-18 09:07:23
```
## Conditional Statements

`IF` statement is used to conditionally execute code.

At simplest, it involves `IF`, `THEN`, and `END_IF`:

```
IF expression THEN
    code to execute
END_IF
```

The code inside is executed if the **expression evaluates to non-zero**.

Indent doesn't matter, feel free to add them for a cleaner look.

----

You can use `ELSE IF` and `ELSE` for additional checks.

If the first `IF` evaluate to 0, `ELSE IF`s are checked and executed if condition is met.

If none of the conditions are met, then code inside `ELSE` is executed.

```
VAR $spam = 5

IF $spam == 0 THEN
    STRING spam is zero!
ELSE IF $spam == 1 THEN
    STRING spam is one!
ELSE
    STRING spam is none of those!
END_IF
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Loops

You can use `WHILE` loops to repeat instructions until a certain condition is met.

Syntax:

```
WHILE expression
    code to repeat
END_WHILE
```

If `expression` evaluates to zero, the code is skipped. Otherwise the code inside is repeated.

This simple example loops 3 times.

```
VAR $i = 0
WHILE $i < 3
    STRINGLN Counter is $i!
    $i = $i + 1
END_WHILE
```

```
Counter is 0!
Counter is 1!
Counter is 2!
```

### `LBREAK`

Use `LBREAK` to **exit a loop** immediately.

```
VAR $i = 0
WHILE TRUE
    STRINGLN Counter is $i!
    $i = $i + 1

    IF $i == 3 THEN
        LBREAK
    END_IF
END_WHILE
```
```
Counter is 0!
Counter is 1!
Counter is 2!
```

### `CONTINUE`

Use `CONTINUE` to **jump to the start of loop** immediately.

```
VAR $i = 0
WHILE $i < 5
    $i = $i + 1

    IF $i == 3 THEN
        CONTINUE
    END_IF

    STRINGLN Counter is $i!
END_WHILE
```

Here when `$count` is 3, it skips printing and starts from the top instead.

```
Counter is 1!
Counter is 2!
Counter is 4!
Counter is 5!
```

### Infinite Loop

To exit an infinite loop, you can [check button status](#reading-inputs), or turn on `Allow Abort` in the key settings.

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Functions

**Functions** let you run a block of code efficiently.

Syntax:

```
FUNCTION func_name()
    code
END_FUNCTION
```

You can use **`RETURN`** to exit a function early.

Arguments and return values are NOT supported, use variables instead (they are global scope).

```
FUNCTION print_info()
    STRING My email is example@gmail.com
    STRING I'm 69 years old!
END_FUNCTION

// call it
print_info()
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Reading Inputs

You can read the status of:

* Built-in Switches
* Rotary Encoders
* Expansion Module Switches

### Blocking Read

Simplest method.

Just read `$_BLOCKING_READKEY` reserved variable.

It will block until a key is pressed.

```
VAR $this_key = $_BLOCKING_READKEY
// Waits here until a key is pressed

IF $this_key == 1 THEN
    // do something here
ELSE IF $this_key == 2 THEN
    // do something else
END_IF

```

### Non-Blocking Read

Read reserved variable `$_READKEY`, returns immediately.

Returns 0 if no key is pressed. Key ID otherwise.

Check this in a loop to perform work even when no key is pressed.

```
VAR $this_key = 0

WHILE TRUE
    $this_key = $_READKEY
    IF $this_key == 1 THEN
        // handling button press
    END_IF

    // otherwise do work here
END_WHILE
```

### Key ID

This is the number returned by methods above.

```
duckyPad Pro (2024):

1-20:
    * Built-in keys
    * Top left is 1
    * Bottom right is 20

21: Upper Rotary Encoder Clockwise
22: Upper Rotary Encoder Counterclockwise
23: Upper Rotary Encoder Push-down

24: Lower Rotary Encoder Clockwise
25: Lower Rotary Encoder Counterclockwise
26: Lower Rotary Encoder Push-down

27: Plus Button
28: Minus Button

37+: External Switches
```

![Alt text](../resources/photos/keyid.png)

```
duckyPad (2020):

1-15:
    * Top left is 1
    * Bottom right is 15
    * Plus button 16, Minus button 17.
```

![Alt text](../resources/photos/keyid_dp20.png)

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Randomisation

Read from `$_RANDOM_INT` to get a random number.

By default, it is between 0 and 65535.

You can change the upper and lower bounds (**inclusive**) by writing to `_RANDOM_MAX` and `_RANDOM_MIN`.

```
$_RANDOM_MIN = 0
$_RANDOM_MAX = 100
STRINGLN Random number: $_RANDOM_INT
```

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

## Miscellaneous

### `DP_SLEEP`

Make duckyPad go to sleep.

Backlight and screen are turned off.

Press any key to wake up.

### `HALT`

Stop execution immediately

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)


## Reserved Variables

There are a few **reserved variables** that are always available.

You can read or write (RW) to adjust settings. Some are read-only (RO).

| Name                                                                 | Access | Description                                                                                    |
| --------------------------------------------------------------------------- | ----- | ---------------------------------------------------------------------------------------------- |
| **`$_RANDOM_MIN`**<br>**`$_RANDOM_MAX`**<br>**`$_RANDOM_INT`**              | RW    | See [Randomisation](#randomisation)                                       |
| **`$_TIME_S`**<br>**`$_TIME_MS`**                                           | RO    | Elapsed time since power-on, in **seconds** or **milliseconds**.                               |
| **`$_READKEY`**<br>**`$_BLOCKING_READKEY`**                                 | RO    | See [Reading Inputs](#reading-inputs)                                      |
| **`$_IS_NUMLOCK_ON`**<br>**`$_IS_CAPSLOCK_ON`**<br>**`$_IS_SCROLLLOCK_ON`** | RO    | Returns **1 if LED is on**, **0 otherwise**.                                                     |
| **`$_DEFAULTDELAY`**<br>**`$_DEFAULTCHARDELAY`**<br>**`$_CHARJITTER`**      | RW    | Aliases.                               |
| **`$_ALLOW_ABORT`**<br>**`$_DONT_REPEAT`**                                  | RW    | **1 to enable**, **0 to disable**.                                      |
| **`$_THIS_KEYID`**                                                          | RO    | Returns the [Key ID](#key-id) for the **current script**     |
| **`$_DP_MODEL`**                                                            | RO    | Device model. Returns:<br>`1` for duckyPad (2020)<br>`2` for duckyPad Pro (2024)                              |
| **`$_KEYPRESS_COUNT`**                                                      | RW    | Number of times the current key was pressed in the **current profile**.<br>Assign **0 to reset**. |
| **`$_LOOP_SIZE`**                                                           | RO    | Used by the `LOOP` command.<br>Do not modify.                                                     |
| **`$_NEEDS_EPILOGUE`**                                                      | RO    | Internal use only. Do not modify.                                                              |
|**`$_RTC_IS_VALID`**<br>**`$_RTC_YEAR`**<br>**`$_RTC_MONTH`**<br>**`$_RTC_DAY`**<br>**`$_RTC_HOUR`**<br>**`$_RTC_MINUTE`**<br>**`$_RTC_SECOND`**<br>**`$_RTC_WDAY`**<br>**`$_RTC_YDAY`**|RO|See [Real-time Clock](#real-time-clock-rtc)|
|**`$_RTC_UTC_OFFSET`**|RW|See [Real-time Clock](#real-time-clock-rtc)|
|**`$_STR_PRINT_FORMAT`**<br>**`$_STR_PRINT_PADDING`**<br>|RW|See [Advanced Printing](#advanced-printing)|

-------
[⬆️⬆️⬆️⬆️⬆️⬆️ Back to Top ⬆️⬆️⬆️⬆️⬆️⬆️](#list-of-commands)

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
