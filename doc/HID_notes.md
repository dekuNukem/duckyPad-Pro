# duckyPad2 HID Command Protocols

## HID Basics

## HID Packet Structure

### PC-to-duckyPad

duckyPad expects a **fixed 64-byte** packet from PC:

|   Byte#  |         Description        |
|:--------:|:--------------------------:|
|     0    | HID Usage ID (always 0x05) |
|     1    |       Reserved      |
|     2    |        Command Type      |
| 3 ... 63 |          Payloads          |

### duckyPad-to-PC

duckyPad will respond with a **fixed 64-byte** response:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    HID Usage ID (always 0x04)    |
|     1    |          Reserved         |
|     2    | Status<br>0 = SUCCESS<br>1 = ERROR<br>2 = BUSY||
| 3 ... 63 |             Payload             |


* `BUSY` is returned if duckyPad is executing a script, or in a menu.

## HID Commands

### Query Info (0x00)

Get device information.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x00        |
| 3 ... 63 |        0x00        |

💬 duckyPad to PC:

|  Byte# |           Description          |
|:------:|:------------------------------:|
|    0   |              0x04              |
|    1   |         Reserved        |
|    2   |0 = SUCCESS|
|    3   |     Firmware version Major     |
|    4   |     Firmware version Minor     |
|    5   |     Firmware version Patch     |
|    6   |     Hardware revision<br>20 = duckyPad<br>24 = duckyPad Pro     |
| 7 - 10 | Serial number (unsigned 32bit) |
|   11   |     Current profile number     |
|   12   |     is_sleeping  |
| 13-63  |              0x00                 |

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

-----------

### Goto Profile by **NAME** (0x17 / 23)

Jump to a particular profile.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x17        |
|     3 ... 63    |   Profile Name String<br>**Case Sensitive**<br>Zero terminated  |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 |             0x00             |

-----------

### Previous Profile (0x02)

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x02        |
| 3 ... 63 |        0x00        |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 |             0x00             |


-----------

### Next Profile (0x03)

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x03        |
| 3 ... 63 |        0x00        |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 |             0x00             |

-----------

### Set RGB Colour: Single (0x04)

Change color of a single LED.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x04        |
|     3    |LED index, 0 to 19  |
|     4    |Red  |
|     5    |Green  |
|     6    |Blue  |
| 7 ... 63 |        0x00        |

-----------

### Set RGB Colour: Bulk (0x05)

**NOT IMPLEMENTED YET**

Change color of ALL LEDs at once.

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x05        |
|     3    |LED 0 Red  |
|     4    |LED 0 Green  |
|     5    |LED 0 Blue  |
|     6    |LED 1 Red  |
|     7    |LED 1 Green  |
|     8    |LED 1 Blue  |
|     ....    |....  |

-----------

### Software reset (0x14 / 20)

Perform a software reset.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x14        |
|     3    |        reboot options<br>0 = normal<br>1 = reboot into USB MSC mode  |
| 3 ... 63 | 0x00 |

💬 duckyPad to PC:

Nothing, because it's rebooting!

**Wait at least 5 seconds** before trying to talk to it again.

-----------

### Sleep (0x15 / 21)

Make duckyPad go to sleep.

Screen and RGB LEDs are turned off.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x15        |
| 3 ... 63 | 0x00 |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 | 0x00 |

-----------

### Wake up (0x16 / 22)

Wake up from sleep

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x16        |
| 3 ... 63 | 0x00 |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | Status, 0 = SUCCESS |
| 3 ... 63 | 0x00 |


## Table of Contents

[Main page](README.md)

[Getting Started Guide](./doc/getting_started.md)

[Kit Assembly Guide](./doc/kit_assembly.md)

[Using duckyScript](./doc/duckyscript_info.md)

[duckyScript VM](./doc/bytecode_vm.md)

[Tinkering Guide](./doc/tinkering_guide.md)

[Troubleshooting](./doc/troubleshooting.md)

[Firmware Update](./doc/fw_update.md)

## Questions or Comments?

Please feel free to [open an issue](https://github.com/dekuNukem/duckypad-pro/issues), ask in the [official duckyPad discord](https://discord.gg/4sJCBx5), or email `dekuNukem`@`gmail`.`com`!