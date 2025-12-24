# duckyPad Feature Update: Jan 2026

Another big update with many under-the-hood improvements!

* Only for **[duckyPad Pro (2024)](https://dekunukem.github.io/duckyPad-Pro/README.html)** FOR NOW

	* Will port to OG duckyPad around Feb/March 2026

## What's New

### New Scripting Engine

The duckyScript **compiler and bytecode VM** has been completely revamped.

* **32-bit** Arithmetic
	* All variables are now 32-bit!
		* User-declared Variables
		* Persistent Global Variables
		* Reserved Variables
	* Both **Signed** and **Unsigned** Modes

* Proper **Function Call Support**:
	* Arguments
	* Local Variables
	* Return Value
	* Nested and Recursive Calls

### Syntax Shakeup

* The `$` prefix is **no longer required** for variable declaration and operations.
	* Except for printing

* **Inline comments** with `//` 

* New **Logical NOT** operator `!`

* New **reserved variables**
	* `_SW_BITFIELD` to read status of **all buttons at once**
	* `_UNSIGNED_MATH` to switch between **signed** and **unsigned** mode

### Performance & Bugfixes

* Faster Read/Write speed in USB Storage Mode.

* Fixed runtime user-assigned color states not saving properly.

## Update Firmware

* Click me to download the beta firmware
* Update your duckyPad Pro by [following this guide](https://dekunukem.github.io/duckyPad-Pro/doc/fw_update.html)
	* Use the file you just downloaded!

## New Features

no $ no problem

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



## Deep Dive

## How'd it go?