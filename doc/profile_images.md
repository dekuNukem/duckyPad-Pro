# Optional profile images (experimental)

This adds an optional monochrome image for the key-label area of a profile.
Profiles without a valid image keep their existing text labels. The profile
title and status indicators remain firmware-owned; key actions and LEDs are
not changed.

Unlike a drawing macro, this is part of the normal profile renderer. It does
not keep the script VM busy. Images are loaded when the profile is drawn,
including normal profile entry and native restore paths. A temporary OLED
macro can still draw its own screen.

## Try it

1. Build firmware containing this feature.
2. Copy `resources/profile_image/example.dppi` into a profile folder as
   `profile-image.dppi`.
3. Reload that profile. The example displays an empty outlined grid in place
   of the key labels. Remove the file and reload to return to text labels.

For a landscape profile, use `profile-image-landscape.dppi`. It is a separate
image in logical screen coordinates; the existing OLED rotation is applied
by the firmware. A missing landscape image falls back to landscape text,
not to the portrait image. Design the artwork for the landscape key layout.

To make your own image, export a **128 x 117, plain PBM (P1)** file:

```sh
python3 resources/profile_image/pack_pbm.py --input artwork.pbm --output profile-image.dppi
```

No additional Python packages are needed. PBM uses 0 for white and 1 for
black; the converter preserves that appearance. It rejects other sizes,
binary PBM (P4), invalid pixels and oversized files. Output files must not
already exist. To regenerate the supplied example at another path:

```sh
python3 resources/profile_image/pack_pbm.py --example --output grid.dppi
```

## File format: DPIM version 1

Exactly 1880 bytes: an eight-byte header followed by 1872 bitmap bytes.

```text
44 50 49 4d 01 80 75 00
 D  P  I  M v1 width height reserved
```

Width is 128 and height is 117. Pixels are row-major, 16 bytes per row,
most-significant bit first; 1 means white and 0 means black. Image row zero
maps to logical screen row 11. Screen rows 0 through 10 are never replaced.
The image includes any desired grid lines as well as icons or labels.

The loader validates the entire file before drawing. Unsupported headers,
short/long files, read/close errors or allocation failures preserve the native
text screen. Payload memory is bounded to 1872 heap bytes and is freed after
each draw. There is no shared image cache that can leak across profiles.
File reads happen on native profile redraws, not in a timer or per-key poll.

## Tests and current validation

```sh
python3 -m unittest discover -s resources/profile_image -v
```

The C-loader tests require a host C compiler and compile the actual loader
with warnings treated as errors. Tests cover payload bounds, bit order,
header preservation, invalid files, orientation selection and switching to
a profile without an image. Converter tests cover PBM semantics, rejection
and the reproducible example. They do not emulate ESP32 SD or OLED timing.

The baseline and modified application were built with ESP-IDF 5.3.1 and the
dependency versions in the original lockfile (esp_tinyusb 1.4.4, led_strip
2.5.4, tinyusb 0.15.0~10). With a current component manager, explicitly pin
these versions for reproduction: otherwise a transitive TinyUSB upgrade may
be selected. The vendor snapshot lacks integrity metadata; fetching the same
versions adds that metadata without changing the source files. No dependency
upgrade is part of this feature.

On that build the application grew by 832 bytes. A duckyPad Pro user has
confirmed that their custom icons appear after installing the firmware.
Physical landscape, wake/sleep, rapid profile-switch, macro-restore and
longer LED/USB regression tests remain to be completed. SD redraw latency
has not been instrumented. The file format and storage strategy are open to
maintainer feedback; no bootloader, partition, security or electrical
configuration changes are required by the feature.
