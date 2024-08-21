# duckyPad2 HID Command Protocols

## HID Basics

## HID Packet Structure

### PC-to-duckyPad

duckyPad expects a **fixed 64-byte** packet from PC:

|   Byte#  |         Description        |
|:--------:|:--------------------------:|
|     0    | HID Usage ID (always 0x05) |
|     1    |       Reserved      |
|     2    |        Command        |
| 3 ... 63 |          Payloads          |

* Byte 2 is command type, [introduced below](#hid-commands).

* The rest of the buffer is **payload**, set to 0 for unused portion.

### duckyPad-to-PC

Once received a packet from PC, duckyPad will reply with a **fixed 64-byte** response:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    HID Usage ID (always 0x04)    |
|     1    |          Reserved         |
|     2    | Status<br>0 = SUCCESS<br>1 = ERROR<br>2 = BUSY|
| 3 ... 63 |             Payloads             |


* `BUSY` is returned if duckyPad is executing a script, or in a menu.

* The rest of the buffer is **payload**, will be 0 if unused.

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

### Goto Profile (0x01)

Jump to a particular profile.

💬 PC to duckyPad:

|   Byte#  |   Description   |
|:--------:|:---------------:|
|     0    |        0x05        |
|     1    | Reserved |
|     2    |        0x01        |
|     3    |   Profile number<br>1-indexed    |
| 4 ... 63 |        0x00        |

💬 duckyPad to PC:

|   Byte#  |            Description           |
|:--------:|:--------------------------------:|
|     0    |    0x04    |
|     1    |          Reserved         |
|     2    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
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
|     2    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
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
|     2    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
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

### Set RGB Colour: Bulk (0x05)

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

### Software reset (0x14) (20)

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

No response because it's rebooting!

Wait at least 3 seconds before trying to talk to it again.

-----------

### Sleep (0x15) (21)

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
|     2    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 3 ... 63 | 0x00 |

-----------

### Wake up (0x16) (22)

Wake up and reload the current profile.

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
|     2    | 0 = SUCCESS, 1 = ERROR, 2 = BUSY |
| 3 ... 63 | 0x00 |

