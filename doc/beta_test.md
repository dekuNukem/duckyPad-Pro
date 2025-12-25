# duckyPad Feature Update: Jan 2026

Another big update with many under-the-hood improvements!

* Only for **[duckyPad Pro (2024)](https://dekunukem.github.io/duckyPad-Pro/README.html)** FOR NOW

	* Will port to OG duckyPad around Feb/March 2026

## What's New

### Revamped Scripting Engine

* Variables are now **32-bit wide**
* **32-Bit Arithmetic** with **signed AND unsigned** modes.
* **Proper Function Calls** with arguments, local variables, return values, and recursion.

### Syntax Shakeup

* `$` prefix is **no longer required** for variables (except for printing)
* **Inline comments** with `//` 
* New **Logical NOT** operator `!`
* New **reserved variables**
	* `_SW_BITFIELD` to read status of **all buttons at once**
	* `_UNSIGNED_MATH` to switch between **arithmetic modes**

### Performance & Bugfixes

* **Faster Read/Write Speeds** in USB Storage Mode.
* Fixed runtime-user-assigned LED colors not saving properly.

## Update Firmware

* [Click me to download beta firmware](../firmware/DPP_FW_3.0.0_5aca4781.bin)
* [Follow this guide](https://dekunukem.github.io/duckyPad-Pro/doc/fw_update.html) to update
	* Use the file you just downloaded!

## Download Latest Apps

* [Configurator](https://github.com/duckyPad/duckyPad-Configurator/releases/tag/4.0.0)
* [Autoswitcher](https://github.com/duckyPad/duckyPad-Profile-Autoswitcher/releases/tag/1.1.1)

## Guided Tour

### In a 32-bit World

* **All variable types** are now **32-bit wide**
	* User-declared
	* Persistent Globals
	* Reserved Variables
* `$` prefix is **no longer required** when working with variables
	* **EXCEPT** when **printing them to a string**.
* **Signed Mode** (Default)
	* Can hold values between **−2,147,483,648 and 2,147,483,647**
	* Suitable for general purpose calculations
```
VAR my_score = 100
my_score = my_score - 500
STRINGLN Current score is: $my_score
```
* **Unsigned Mode**
	* Set `_UNSIGNED_MATH = 1`
	* Range: **0 to 4,294,967,295**
	* Suitable for large numbers or bitwise operations
```
_UNSIGNED_MATH = 1
VAR large_val = 4000000000
STRINGLN Large unsigned value: $large_val
```

### Functions

Functions have been expanded to support **arguments**, **return values**, **local variables**, and **recursive** calls.

#### ↩️ Back to Basics

As before, plain functions are handy for performing repetitive tasks.

```
FUNCTION print_addr()
    STRINGLN Shipping Address:
    STRINGLN 123 Ducky Lane
    STRINGLN Pond City, QU 12345
END_FUNCTION

print_addr() // call it
```

#### 🆕 Arguments and Returns

But now, you can also pass **up to 8** arguments into a function and specify a return value.

* **`0` is returned** without an explicit `RETURN` command.

```
FUNCTION add_number(a, b)
    RETURN a + b
END_FUNCTION

VAR total = add_number(10, 20)
```

#### 🆕 Variable Scoping

Variables declared **outside functions** have **global scope**, they can be **accessed anywhere**.

Variables declared **inside functions** now have **local scope**, they are only accessible **within the function**.

If a local variable has the **same name** as a global variable, the **local var takes priority** within that function.

```
// Both global scope
VAR x = 10
VAR y = 20

FUNCTION scope_demo()
    VAR x = 5 // This x is local, will shadow the global x.
    x = x + y
    STRINGLN Local x is: $x
    // should be 5 + 20 = 25
END_FUNCTION
```

#### 🆕 Nested / Recursive Calls

You can now:

* Call other functions while inside a function
* Or **call the same function again** while already in it!
* Beware stack overflow. Ensure proper end conditions.

```
FUNCTION factorial(n)
    IF n <= 1 THEN
        RETURN 1
    END_IF
    RETURN n * factorial(n - 1)
END_FUNCTION

VAR fact = factorial(5)
```

## Deep(er) Dive

Did you know that the duckyScript you write is actually **compiled into a binary** and **executed on a virtual machine** on the duckyPad itself?

This update began as a simple excursion to add 32-bit math and proper function calls. Unsurprisingly it cascaded into a month-long rewrite of the entire compiler and VM. Key changes include:

* duckyScript source is now **pre-processed into Python**
	* So I can use its built-in AST and Symtable parsers
	* This is faster and less error-prone than rolling my own from scratch
* The VM now has a proper **memory map** and **stack layout**
* With FP-based **calling convention** that supports args, locals, and recursion.
* Instructions are now **variable-length** to reduce code size

The VM is now more general-purpose than ever, and theoretically other languages can be compiled to run on it too instead of just duckyScript!

**Want to learn more?** The VM now has its own repo, full details here:

https://github.com/duckyPad/DuckStack

## How'd It Go?

Hopefully everything still works!

With so many changes, do give it a good thrashing to see if you can find any bugs!

If/when you do, let me know 😅!

* [Discord](https://discord.gg/4sJCBx5)
* Email: `dekuNukem` `gmail.com`
* [Github Issue](https://github.com/dekuNukem/duckypad-pro/issues)
