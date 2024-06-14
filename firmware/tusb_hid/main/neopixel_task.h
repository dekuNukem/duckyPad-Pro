#ifndef __NEOPIXEL_TASK_H
#define __NEOPIXEL_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#define NEOPIXEL_COUNT 20
#define NEOPIXEL_PIN 46

#define BRIGHTNESS_LEVELS 5

#define THREE 3
#define DEFAULT_BG_RED 64
#define DEFAULT_BG_GREEN 16
#define DEFAULT_BG_BLUE 255
#define DEFAULT_KD_RED 255
#define DEFAULT_KD_GREEN 255
#define DEFAULT_KD_BLUE 255

void neopixel_init(void);

#ifdef __cplusplus
}
#endif

#endif


