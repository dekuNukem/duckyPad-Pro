/* SPDX-License-Identifier: MIT */
#include "profile_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_WIDTH 128
#define IMAGE_HEIGHT 117
#define IMAGE_BYTES (IMAGE_WIDTH / 8 * IMAGE_HEIGHT)

bool profile_image_draw(const char *root, const char *profile, bool landscape,
                        profile_image_pixel_fn pixel)
{
    if (!root || !profile || !pixel) return false;
    size_t length = 0;
    for (; length < 17 && profile[length]; ++length) {
        unsigned char c = (unsigned char)profile[length];
        if (c < 32 || c > 126 || c == '/' || c == '\\') return false;
    }
    if (length == 0 || length > 16) return false;
    char path[256];
    int n = snprintf(path, sizeof(path), "%s/profile_%s/%s", root, profile,
                     landscape ? "profile-image-landscape.dppi" : "profile-image.dppi");
    if (n < 0 || (size_t)n >= sizeof(path)) return false;
    FILE *file = fopen(path, "rb");
    if (!file) return false;
    unsigned char header[8];
    const unsigned char expected[8] = {'D', 'P', 'I', 'M', 1, 128, 117, 0};
    if (fread(header, 1, sizeof(header), file) != sizeof(header) ||
        memcmp(header, expected, sizeof(header)) != 0) {
        fclose(file);
        return false;
    }
    unsigned char *data = malloc(IMAGE_BYTES);
    if (!data) {
        fclose(file);
        return false;
    }
    bool valid = fread(data, 1, IMAGE_BYTES, file) == IMAGE_BYTES;
    if (valid) valid = fgetc(file) == EOF && !ferror(file);
    if (fclose(file) != 0) valid = false;
    /* Do not touch the display until the complete file has been validated. */
    if (valid) {
        for (unsigned y = 0; y < IMAGE_HEIGHT; ++y) {
            for (unsigned x = 0; x < IMAGE_WIDTH; ++x) {
                bool white = (data[y * 16 + x / 8] & (0x80u >> (x % 8))) != 0;
                pixel((uint8_t)x, (uint8_t)(y + 11), white);
            }
        }
    }
    free(data);
    return valid;
}
