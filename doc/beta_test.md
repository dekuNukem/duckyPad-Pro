# duckyPad Feature Update: Jan 2026

Another big update with many under-the-hood improvements!

* Only for **[duckyPad Pro (2024)](https://dekunukem.github.io/duckyPad-Pro/README.html)** FOR NOW

	* Will port to OG duckyPad around Feb/March 2026

## What's New

Quick overview, scroll down for more details.

### Revamped Scripting Engine

* **32-Bit Arithmetic** across all variable types with **signed AND unsigned** modes.
* **Proper Function Calls** with arguments, local variables, return values, and nested/recursive calls.

### Syntax Shakeup

* The `$` prefix is **no longer required** for variable declaration and operations.
	* Except for printing
* **Inline comments** with `//` 
* New **Logical NOT** operator `!`
* New **reserved variables**
	* `_SW_BITFIELD` to read status of **all buttons at once**
	* `_UNSIGNED_MATH` to switch between **signed** and **unsigned** mode

### Performance & Bugfixes

* **Faster Read/Write Speeds** in USB Storage Mode.
* Fixed runtime-user-assigned switch colors not saving properly.

## Update Firmware

* Click me to download the beta firmware
* Update your duckyPad Pro by [following this guide](https://dekunukem.github.io/duckyPad-Pro/doc/fw_update.html)
	* Use the file you just downloaded!

## New Features Guided Tour

### In a 32-bit World

no $ no problem

### Functions

### Misc

```
VAR spam = 5
VAR eggs = spam * 2
```

```
function args example
```

```
function return value and local variable example
mention shadowing
```

## Deep(er) Dive

duckStack VM page. completely revamped from ground up. instruction set and memory map. compiler example and C VM example. even from different languages or hand-write assembly.




## How'd it go?