#ifndef __KEYPRESS_TASK_H
#define __KEYPRESS_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>
#include "input_task.h"

#define KEYPRESS_TASK_STACK_SIZE 4096

void keypress_task(void *dummy);
void start_sleeping(void);
void wakeup_from_sleep_and_load_profile(uint8_t profile_to_load);
void wakeup_from_sleep_no_load(void);
void update_last_keypress(void);
void block_until_anykey(uint8_t event_type);
void block_until_plus_minus_long_press(void);

extern volatile uint32_t last_keypress;
extern volatile uint8_t is_sleeping;
extern volatile uint8_t is_busy;
extern volatile uint8_t oled_brightness;

#ifdef __cplusplus
}
#endif

#endif


