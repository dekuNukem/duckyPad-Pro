#ifndef __INPUT_TASK_H
#define __INPUT_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define SW_PLUS_GPIO GPIO_NUM_17
#define SW_MINUS_GPIO GPIO_NUM_18

#define SW_MINUS 20
#define SW_PLUS 21

#define SW_EVENT_RELEASE 0
#define SW_EVENT_SHORT_PRESS 1

typedef struct
{
    uint8_t id;
    uint8_t type;
} switch_event_t;

void switch_init(void);
void kb_scan_task(void *dummy);


#define SW_SCAN_TASK_STACK_SIZE 2048

extern QueueHandle_t switch_event_queue;

#ifdef __cplusplus
}
#endif

#endif


