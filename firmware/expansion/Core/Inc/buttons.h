#ifndef __BUTTONS_H
#define __BUTTONS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx_hal.h"
#include "cQueue.h"

#define MAX_CHANNELS 64
#define CHANNEL_COUNT 6
#define CHANNEL_1 0
#define CHANNEL_2 1
#define CHANNEL_3 2
#define CHANNEL_4 3
#define CHANNEL_5 4
#define CHANNEL_6 5

#define SW_EVENT_RELEASE 0
#define SW_EVENT_SHORT_PRESS 1

#define SW_EVENT_QUEUE_SIZE 16

#define CMD_SW_PRESS_BITMASK 0x80
#define CMD_SW_RELEASE_BITMASK 0xc0

void channel_update(uint8_t start_id);
void sw_event_queue_init(void);

extern Queue_t switch_event_queue;

#ifdef __cplusplus
}
#endif

#endif


