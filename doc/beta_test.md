# [BETA TEST] duckyPad "Power User" Update: Jan 2026

Happy new year! Here's another **major update** that makes your duckyPad even more powerful!

This update focuses mainly on **advanced features** for **power users**, and significantly expands duckyScript's capabilities.

But of course, everyone will benefit from the new features and improved performance!

**Highlights:**

* **32-bit** Overhaul
* Proper **Function Calls**
* **Print Format** Specifiers
* **User-defined Headers** and **StdLib**
* **Horizontal** Mouse Scrolling
* Streamlined Syntax
* New Commands & Bug Fixes

----
* Only for **[duckyPad Pro (2024)](https://dekunukem.github.io/duckyPad-Pro/README.html)** FOR NOW
    * Will port to OG duckyPad around Feb/March 2026
----

## Changelog Archive

Looking for older update notes? [Click me](./update_notes)

## Update Firmware

* [Get Beta firmware](../firmware/DPP_FW_3.0.0_5aca4781.bin)
    * Click `View Raw` to download
* [Follow this guide](https://dekunukem.github.io/duckyPad-Pro/doc/fw_update.html) to update
    * Use the file you just downloaded!

## Download Latest Apps

* [Configurator](https://github.com/duckyPad/duckyPad-Configurator/releases/tag/4.0.0)
* [Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/releases/tag/1.1.1)

## Do Your Worst!

* Update FW and download latest apps.
* ⚠️ **Load & save** your duckyPad Pro in latest configurator to **refresh the files**
* See if your **existing scripts** still work!
* If so, scroll down, take a look at the new features and try them out.
    * Give it a good thrashing

Bugs are expected, let me know if you run into any!

* [Discord](https://discord.gg/4sJCBx5)
* Email: `dekuNukem` `gmail.com`
* [Github Issue](https://github.com/dekuNukem/duckypad-pro/issues)

## Table of Contents

- [Syntax Shakeup](#syntax-shakeup)
- [In a 32-bit World](#in-a-32-bit-world)
- [Print Format Specifiers](#print-format-specifiers)
- [Numerical Padding](#numerical-padding)
- [Proper Function Calls](#proper-function-calls)
- [Built-in Functions](#built-in-functions)
- ["Random Letter" Commands](#random-letter-commands)
- [New Reserved Variables](#new-reserved-variables)
- [New Mouse Commands](#new-mouse-commands)
- [Performance & Bugfixes](#performance--bugfixes)
- [Deep(er) Dive](#deeper-dive)

## Syntax Shakeup

* `$` prefix is **no longer required** when working with variables
    * **EXCEPT** when **printing**
* New **Augmented Assignment** Operators 
    * `+=`, `-=`, `*=` etc.
```
VAR foo = 100
foo += 5
STRING Value is: $foo
```
* New **Logical NOT** operator `!`
* `THEN` is **no longer required** in `IF` statements
* **Inline comments** with `//`
```
IF !foo 
    DELAY 100 // only if foo is 0
END_IF
```

## In a 32-bit World

* **All variable types** are now **32-bit wide**
* Adjustable **Math Mode** by writing to `_UNSIGNED_MATH`
* **Signed Mode** (Default)
    * Can hold values between **−2,147,483,648 and 2,147,483,647**
    * Suitable for **all general purpose** calculations
* **Unsigned Mode**
    * Range: **0 to 4,294,967,295**
    * Suitable for **large numbers** or **bitwise operations**
* **Stick to signed mode** if unsure

## Print Format Specifiers

**C-Style Format Specifiers** are now supported when **printing variables**!

Use it to adjust **print format** and **padding**.

* Without specifier, variable are printed as **signed decimal** number
```
VAR foo = -10
STRING Value is $foo
```
```
Value is -10
```

To add a specifier: **Immediately after the variable name**, type `%`, then a **data-type indicator letter**.

* `%d` to print variable as **Signed Decimal**
    * Default, same as no specifier.
* `%u` to print variable as **Unsigned Decimal**
* `%x` to print variable as **Lowercase Hexadecimal**
* `%X` to print variable as **Uppercase Hexadecimal**

```
VAR $foo = -10

STRING Value is: $foo%d
STRING Value is: $foo%u
STRING Value is: $foo%x
STRING Value is: $foo%X
```

```
Value is: -10
Value is: 4294967286
Value is: fffffff6
Value is: FFFFFFF6
```

## Numerical Padding

* To pad with **SPACE**
    * Add a **width number** just **after `%`** and **before the letter**
    * The output will be **at least that** wide
    * Any extra space are padded with **space characters**
```
VAR $foo = 5
STRING I have $foo%10d apples!
```
```
I have          5 apples!
```

* To pad with **LEADING-ZERO**
    * Right **after `%`**, add a `0`, then width number, then the letter.
    * The output will be **at least that** wide
    * Any extra space are padded with `0`
* Useful for printing **dates** and **hex numbers**
```
VAR $foo = 5
STRING I have $foo%010d apples!
```
```
I have 0000000005 apples!
```

------

⚠️ This method replaces `_STR_PRINT_FORMAT` and `_STR_PRINT_PADDING`, which have been removed.

-----

## Proper Function Calls

Functions now support **arguments**, **return values**, **local variables**, and **recursive** calls, making them much more powerful and versatile.

### FUN FUN FUN

You can now use `FUN` and `END_FUN` to declare functions.

Old `FUNCTION` and `END_FUNCTION` still works of course.

### Back to Basics

As before, plain functions are handy for performing repetitive tasks.

```
FUN print_addr()
    STRINGLN 123 Ducky Lane
    STRINGLN Pond City, QU 12345
END_FUN

print_addr() // call it
```

### Arguments and Returns

But now, you can also pass **up to 8 arguments** into a function and specify a **return value**.

```
FUN add_number(a, b)
    RETURN a + b
END_FUN

VAR total = add_number(10, 20)
```

### Variable Scoping

Variables declared **outside functions** have **global scope**, they can be **accessed anywhere**.

Variables declared **inside functions** now have **local scope**, they are only accessible **within the function**.

If a local variable has the **same name** as a global variable, the **local var takes priority** within that function.

```
// Both global scope
VAR x = 10
VAR y = 20

FUN scope_demo()
    VAR x = 5 // This x is local, will shadow the global x.
    x = x + y
    STRINGLN Local x is: $x
END_FUN
```
```
Local x is: 25
```

### Nested / Recursive Calls

Now possible!

```
FUN factorial(n)
    IF n <= 1
        RETURN 1
    END_IF
    RETURN n * factorial(n - 1)
END_FUN

VAR fact = factorial(5)
```

## duckyPad Standard Library

With much more powerful function calls, a set of **handy helper functions** are provided as a **StdLib**.

To use them, add `USE_STDLIB` in your code.

[More Info / Contribute](https://github.com/duckyPad/DPDS-Standard-Library/blob/master/README.md)

```
USE_STDLIB

STRINGLN Press Key 3 to continue...
WAITKEY(3)

VAR score_1 = 100
VAR score_2 = 500
VAR high_score = MAX(score_1, score_2)

STRINGLN The high score is: $high_score
```

## User Headers

You can now also **create your own header** for custom helper functions and more.

* Click `Edit Headers` Button
* Write code
* Add `USE_UH` to your script to include them
    * The header is added to your source code **AS-IS** during preprocessing

## Built-in Functions

A few built-in functions have been added.

They are intended for **low-level tinkering** and are completely optional.

* [See main doc](duckyscript_info.md#built-in-functions) for details

**`PEEK8(addr)`**

**`POKE8(addr, value)`**

**`RANDCHR(value)`**

**`RANDINT(lower, upper)`**

**`PUTS(value)`**

**`HIDTX(addr)`**

## "Random Letter" Commands 

Types a random letter. Added from [official duckyScript specs](https://docs.hak5.org/hak5-usb-rubber-ducky/duckyscript-quick-reference/).

```
RANDOM_LOWERCASE_LETTER     RANDOM_NUMBER
RANDOM_UPPERCASE_LETTER     RANDOM_SPECIAL
RANDOM_LETTER               RANDOM_CHAR
```
```
RANDOM_NUMBER
REPEAT 7
// types 8 random numbers
```

For more granular control, use `RANDCHR()` built-in function.

## New Reserved Variables

* `_SW_BITFIELD` to read status of **all buttons at once**
* `_UNSIGNED_MATH` to switch between **arithmetic modes**

## New Mouse Commands

### Mouse Side Buttons

`FMOUSE`: **Forward** side button

`BMOUSE`: **Backward** side button

### Horizontal Mouse Scrolling

* `MOUSE_SCROLL h v`

Scroll mouse wheel **Horizontal** `h` lines, and **Vertical** `v` lines.

* `h`: Positive scrolls RIGHT, negative scrolls LEFT.
* `v`: Positive scrolls UP, negative scrolls DOWN.
* Set `0` for no scroll

## Performance & Bugfixes

* **Faster Read/Write Speeds** in USB Storage Mode.
* Adjusted HID buffer to support `6KRO`
    * Including in Bluetooth mode
* Randomization now uses **Hardware RNG**
* Fixed runtime-user-assigned LED colors not saving properly.
* Compiler Optimization to reduce code size.

## Deep(er) Dive

Did you know the duckyScript you write is actually **compiled into a binary** and **executed on a virtual machine** on the duckyPad itself?

This update began as a simple excursion to add 32-bit math and proper function calls. Unsurprisingly it cascaded into a month-long rewrite of the entire compiler and VM. Key changes include:

* duckyScript is now **pre-processed into Python**
    * So I can use its built-in AST and Symtable parsers
    * This is faster and less error-prone than rolling my own from scratch
* The VM now has a proper **memory map** and **stack layout**
    * With FP-based **calling convention** to support proper function calls
* Instructions are now **variable-length** with mild optimizations to reduce code size

The VM is now more general-purpose than ever, and theoretically other languages can be compiled for it too instead of just duckyScript!

**Want to learn more?** The VM now has its own repo, full details here:

https://github.com/duckyPad/DuckStack

## How'd It Go?

Let me know if you run into any bugs or have comments / suggestions.

* [Discord](https://discord.gg/4sJCBx5)
* Email: `dekuNukem` `gmail.com`
* [Github Issue](https://github.com/dekuNukem/duckypad-pro/issues)
