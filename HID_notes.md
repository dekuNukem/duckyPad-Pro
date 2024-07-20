# duckyPad2 HID Command Protocols

## HID Basics

## HID Packet Structure

### PC-to-duckyPad

duckyPad expects a **fixed 64-byte** packet from PC:


|   Byte#  |         Description        |
|:--------:|:--------------------------:|
|     0    | HID Usage ID (always 0x05) |
|     1    |        Command type        |
| 2 - 63 |          Payloads          |

* Byte 0 is always 0x05

* Byte 2 is command type, [introduced below](#hid-commands).

* The rest of the buffer is **payload**, set to 0 for unused portion.

### duckyPad-to-PC

Once received a packet from PC, duckyPad will reply with a **fixed 64-byte** response:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    HID Usage ID (always 0x04)    |
|     1    | Status. 0 = SUCCESS, 1 = ERROR. 2 = BUSY |
| 2 - 63 |             Payloads             |

* Byte 0 is always 0x04

* Byte 1 is status, can be `SUCCESS`, `ERROR`, or `BUSY`.

* `BUSY` is returned if duckyPad is executing a script, or in a menu.

* The rest of the buffer is **payload**, will be 0 if unused.

## HID Commands

### Info (0x00)

If command type is 0x00, duckyPad will return its device information.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x00        |
| 2 - 63 |        0x00        |

* duckyPad to PC:

|  Byte# |           Description          |
|:------:|:------------------------------:|
|    0   |              0x04              |
|    2   |0 = SUCCESS, 1 = ERROR, 2 = BUSY|
|    3   |     Firmware version Major     |
|    4   |     Firmware version Minor     |
|    5   |     Firmware version Patch     |
| 7 - 10 | Serial number (unsigned 32bit) |
|   11   |     Current profile number     |
|   12   |     Sleep status. 0=awake, 1=sleeping   |
| 13-63  |              0x00                 |

### Goto Profile (0x01)

If command type is 0x01, duckyPad will jump to a particular profile.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x01        |
|     3    |   Profile number to jump to        |
| 4 ... 63 |        0x00        |

* duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 2 - 63 |             0x00             |

### Previous Profile (0x02)

If command type is 0x02, duckyPad will go to the previous profile.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x02        |
| 2 - 63 |        0x00        |

* duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 2 - 63 |             0x00             |


### Next Profile (0x03)

If command type is 0x03, duckyPad will go to the next profile.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x03        |
| 2 - 63 |        0x00        |

* duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 2 - 63 |             0x00             |

### Set RGB Colour: Single (0x04)

**TO BE IMPLEMENTED**

Set RGB color of a single LED.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x05        |
|     3    |LED index, 1 to 15  |
|     4    |Red  |
|     5    |Green  |
|     6    |Blue  |
| 7 ... 63 |        0x00        |

### Set RGB Colour: Bulk (0x05)

**TO BE IMPLEMENTED**

Set RGB color of ALL LEDs, ideal for animations.

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x05        |
|     3    |LED 1 Red  |
|     4    |LED 1 Green  |
|     5    |LED 1 Blue  |
|     6    |LED 2 Red  |
|     7    |LED 2 Green  |
|     8    |LED 2 Blue  |
|     ....    |....  |
|     45    |LED 15 Red  |
|     46    |LED 15 Green  |
|     47    |LED 15 Blue  |
| 48 ... 63 |        0x00        |

### Software reset (0x14)

If command type is 0x14, duckyPad will perform a software reset.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x14        |
| 2 - 63 | 0x00 |

* duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 2 - 63 | 0x00 |

### Sleep (0x15)

If command type is 0x15, duckyPad will go to sleep.

* PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    |        0x15        |
| 2 - 63 | 0x00 |

* duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 2 - 63 | 0x00 |

## Questions or Comments?

Please feel free to [open an issue](https://github.com/dekuNukem/duckypad/issues), ask in the [official duckyPad discord](https://discord.gg/4sJCBx5), DM me on discord `dekuNukem#6998`, or email `dekuNukem`@`gmail`.`com` for inquires.
