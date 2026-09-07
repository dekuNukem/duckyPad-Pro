/* SPDX-License-Identifier: MIT */
#ifndef DPP_PROFILE_IMAGE_H
#define DPP_PROFILE_IMAGE_H
#include <stdbool.h>
#include <stdint.h>

/* Coordinates exclude the firmware-owned header (rows 0..10). */
typedef void (*profile_image_pixel_fn)(uint8_t x, uint8_t y, bool white);
bool profile_image_draw(const char *root, const char *profile, bool landscape,
                        profile_image_pixel_fn pixel);
#endif
