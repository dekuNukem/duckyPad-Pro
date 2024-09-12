# Writing duckyScript

[Get duckyPad](https://www.tindie.com/products/21984/) | [Official Discord](https://discord.gg/4sJCBx5) | [Getting Started](getting_started.md) | [Table of Contents](#table-of-contents)

------

## Overview

duckyScript is a simple language for automating keyboard/mouse inputs.

It was originally developed for [USB Rubber Ducky](https://shop.hak5.org/products/usb-rubber-ducky-deluxe).

## Quick Examples

In the **simplest form**, you just tell it what key to press!

Ideal for key combos:

* New Tab: `CONTROL t`

* Task Manager: `CONTROL SHIFT ESC`

--------

Once familiar, you can write **longer multi-line** scripts for more complex actions.

##### Open a Webpage:

```
WINDOWS r
DELAY 500
STRING https://youtu.be/dQw4w9WgXcQ
ENTER

```

---------

At full potential, duckyScript is closer to a **general-purpose language**. You can:

* Read Buttons
* Write to OLED screen
* Change RGB colors
* Variables, Conditions, and Loops.
* And more!

This allows writing ultra-specific macros for your exact needs.

## First Time?

There are a lot of commands, and it can be a bit daunting.

The `first 2 sections` (`Built-in` and `Special Keys`) is more than enough to get started. Then you can skim through the rest.

Also **playing with sample profiles** is a good way to get a feel of duckyScript.

Of course, people at the [Official Discord](https://discord.gg/4sJCBx5) are always happy to help!

## List of Commands

it's long, use this table of contents to navigate

include list to go back at each section?


## Comments

### `REM` and `//`

Any line starting with those is ignored.

### `REM_BLOCK` and `END_REM`

Comment block. Everything in-between is ignored.

## Timing

### `DELAY`

Creates a pause (in milliseconds) in execution.

Useful for **waiting for UI to catch up**.

```
DELAY 1000
// wait 1000 milliseconds, or 1 second
```

### `DEFAULTDELAY`

Change how long to wait between **`each line of code`**.

Default is 20ms.

### `DEFAULTCHARDELAY`

Change how long to wait between each **`key stroke`**.

Default is 20ms.

### `CHARJITTER X`

Adds an **additional** random delay between 0 and X milliseconds after `each key stroke`.

Can be used to make typing more human-like.

Set to 0 to disable.

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

## Pressing Keys

### Key Combos

duckyScript supports many special keys.

They can be used on their own:

`WINDOWS`

...or combined with a character to form shortcuts:

`WINDOWS s`

...or chained even longer:

`WINDOWS SHIFT s`

------

* Type the key name **as-is** in **`ALL CAPS`**.

* Keys are pressed in sequence from **left-to-right**, then released **right-to-left**.

------

Supported Special Keys:

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

Hold/release a key on command.

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

## Mouse

### Mouse Buttons

* `LMOUSE`: Click LEFT mouse button

* `RMOUSE`: Click RIGHT mouse button

* `MMOUSE`: Click MIDDLE mouse button

### `MOUSE_MOVE X Y`

Move mouse cursor `X` pixels horizontally, and `Y` pixels vertically.

* For `X`, a positive number moves RIGHT, negative number moves LEFT.

* For `Y`, a positive number moves UP, negative number moves DOWN.

* Set `X` or `Y` to 0 if no movement needed.

* **Disable mouse acceleration** for pixel-accurate results!

### `MOUSE_WHEEL X`

Scroll mouse wheel `X` lines.

* A positive number scrolls UP, negative number scrolls DOWN.

## OLED

### `OLED_CURSOR x y`

Set where to print on screen.

`x`, `y`: coordinates between `0` and `127`.

Characters print from **top-left** corner.

### `OLED_PRINT`

`OLED_PRINT hello world!` 

Prints the message into display buffer at current cursor location.

### `OLED_CLEAR`

Clears the display buffer.

### `OLED_UPDATE`

Actually update the OLED.

You should use `OLED_CLEAR`, `OLED_CURSOR`, and `OLED_PRINT` to set up the display, then use this to print it.

This is **much faster** than updating the whole screen for every change.

### `OLED_RESTORE`

Restore the default profile/key name display. `OLED_UPDATE` **NOT NEEDED**.

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

Set `n` from 1 to 15 for a particular key.

Set `n` to 99 for all keys.

## Profile Switching

### Autoswitcher

Try the [autoswitcher](https://github.com/dekuNukem/duckyPad-profile-autoswitcher) to jump to a profile based on **current window**!

### `PREV_PROFILE` / `NEXT_PROFILE`

Switch to the previous / next profile.

### `GOTO_PROFILE`

Jump to a particular profile by name. **Case sensitive!**

```
GOTO_PROFILE NumPad
```

## Constants

You can use `DEFINE` to, well, define a constant.

It can be either **integer** or **string**.

The content is **replaced AS-IS** during pre-processing, very much like `#define` in C.

```
DEFINE MY_EMAIL example@gmail.com
DEFINE MY_AGE 69

STRING My email is MY_EMAIL!
STRING I'm MY_AGE years old! 
```

Internally, `TRUE` is `1`, and `FALSE` is `0`.

## Variables

You can declare a variable using `VAR` command:

```
// Declaration
VAR $spam = 0
VAR $eggs = 10

// Assignment
$spam = 20
```

Variables must start with dollar sign `$`.

Variables are **unsigned 16-bit integers**, and can hold values from **0 to 65535**.

All variables have **global scope**, and can be referenced anywhere in the script.

They can be printed with `STRING`, `STRINGLN`, and `OLED_PRINT`.

```
STRING The value is: $spam
```

## Reserved Variables

There are a few **reserved variables** that are always available.

You can read or write (RW) to adjust settings. Some are read-only (RO).

#### `$_RANDOM_MIN` (RW)

Lower bound of random number generator (RNG).

#### `$_RANDOM_MAX` (RW)

Upper bound of RNG.

#### `$_RANDOM_INT` (RW)

Get a random number between the upper and lower bound (**inclusive**).

#### `$_TIME_S` (RO)

Get current time in **seconds**.

#### `$_TIME_MS` (RO)

Get current time in **milliseconds**.

#### `$_READKEY` (RO)

Returns the first active key number.

Returns 0 if no key is pressed.

#### `$_BLOCKING_READKEY` (RO)

Wait until a key is pressed, then returns its number.

## Operators

You can perform operations on constants and variables.

### Mathematics

```
=       Assignment
+       Add       
-       Subtract  
*       Multiply  
/       Divide    
%       Modulus   
^       Exponent
```

Example:

```
$spam = 2+3
$spam = $eggs * 10
```

### Comparison

All comparisons evaluate to **either 0 or 1**.

```
==        Equal                  
!=        Not equal              
>         Greater than           
<         Less than              
>=        Greater than or equal  
<=        Less than or equal   
```

### Logical 

| Operator |          Name         | Comment                                                |
|:--------:|:---------------------:|--------------------------------------------------------|
|    &&    |      Logical AND      | Evaluates to 1 if BOTH side are non-zero, otherwise 0. |
|   \|\|   |       Logical OR      | Evaluates to 1 if ANY side is non-zero, otherwise 0.   |

### Bitwise

```
&        Bitwise AND   
|        Bitwise OR    
<<       Left Shift    
>>       Right Shift   
```

## Argument Options

You can use **constant, variable, or expression** as arguments in commands.

```
VAR $amount = 100
DELAY $amount*2+5
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

Here when `$count` is 3, it skips printing and start from beginning instead.

```
Counter is 1!
Counter is 2!
Counter is 4!
Counter is 5!
```

To exit an infinite loop, you can [check button status](#reading-buttons), or turn on `Allow Abort` in the key settings.


## Functions

Functions let you run a block of code efficiently.

Syntax:

```
FUNCTION func_name()
    code
END_FUNCTION
```

You can use **`RETURN`** to exit a function early.

Arguments and return values are NOT supported, use global variables instead.

```
FUNCTION print_info()
    STRING My email is example@gmail.com
    STRING I'm 69 years old!
END_FUNCTION

// call it
print_info()
```

## Reading Inputs

You can read the status of:

* Built-in Switches
* Rotary Encoders
* Expansion Module Switches

### Key ID

A number will be returned for each key.

```
1-20:
    * Built-in keys
    * Top left is 1
    * Bottom right is 20.

21: Upper Rotary Encoder Clockwise
22: Upper Rotary Encoder Counterclockwise
23: Upper Rotary Encoder Push-down

24: Lower Rotary Encoder Clockwise
25: Lower Rotary Encoder Counterclockwise
26: Lower Rotary Encoder Push-down

27: Plus Button
28: Minus Button

37-99: External Switches
```

### Blocking Read

Simplest method.

Just read `$_BLOCKING_READKEY` internal variable.

It will block until a key is pressed.

```
VAR $this_key = $_BLOCKING_READKEY

IF $this_key == 1 THEN
    // do something here
ELSE IF $this_key == 2 THEN
    // do something else
ELSE IF $this_key == 3 THEN
    // etc
END_IF

```

### Non-Blocking Read

Read internal variable `$_READKEY`, returns immediately.

Returns 0 if no key is pressed. Key ID otherwise.

Usually, you check this in a loop to perform some work even when no key is pressed.

```
VAR $this_key = 0

WHILE TRUE
    $this_key = $_READKEY
    IF $this_key == 1 THEN
        // handling button press
    END_IF

    // otherwise perform work here
END_WHILE

STRINGLN I pressed key $k!
```

## Randomisation

Read from `$_RANDOM_INT` to get a random number.

By default, it is between 0 and 65535.

You can change the upper and lower bounds (**inclusive**) by writing to `_RANDOM_MAX` and `_RANDOM_MIN`.

```
$_RANDOM_MIN = 0
$_RANDOM_MAX = 100
STRINGLN Random number: $_RANDOM_INT
```

## Miscellaneous

### `DP_SLEEP`

Make duckyPad go to sleep.

Backlight and screen are turned off.

Press any key to wake up.

## `HALT`

Stop execution immediately


---------

### `LOOP`

Allows you to **assign different actions to the same key**.

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

* When you press a key, a counter increments, and the script at the corresponding loop is executed.

* Loop and color state should persist through profile switches and reboots
