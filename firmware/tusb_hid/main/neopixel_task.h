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

void neopixel_init(void);

#ifdef __cplusplus
}
#endif

#endif


