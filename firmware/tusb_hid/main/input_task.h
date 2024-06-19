#ifndef __INPUT_TASK_H
#define __INPUT_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define SW_MATRIX_NUM_COLS 4
#define SW_MATRIX_NUM_ROWS 5

// OB = onboard
#define MECH_OBSW_COUNT (SW_MATRIX_NUM_COLS * SW_MATRIX_NUM_ROWS)
// 20 mechanical switches, + and - button, 3 for each rotary encoder
#define TOTAL_OBSW_COUNT (MECH_OBSW_COUNT + 8)

#define SW_PLUS_GPIO GPIO_NUM_18
#define SW_MINUS_GPIO GPIO_NUM_17
#define SW_RE1_GPIO GPIO_NUM_0
#define SW_RE2_GPIO GPIO_NUM_3

#define SD_CARD_DETECT_GPIO GPIO_NUM_6

#define RE1_A GPIO_NUM_1
#define RE1_B GPIO_NUM_2
#define RE2_A GPIO_NUM_4
#define RE2_B GPIO_NUM_5

#define SWM_COL0_GPIO GPIO_NUM_42
#define SWM_COL1_GPIO GPIO_NUM_41
#define SWM_COL2_GPIO GPIO_NUM_39
#define SWM_COL3_GPIO GPIO_NUM_38

#define SWM_ROW0_GPIO GPIO_NUM_45
#define SWM_ROW1_GPIO GPIO_NUM_40
#define SWM_ROW2_GPIO GPIO_NUM_37
#define SWM_ROW3_GPIO GPIO_NUM_36
#define SWM_ROW4_GPIO GPIO_NUM_35

#define ROTARY_ENCODER_UPPER 1
#define ROTARY_ENCODER_LOWER 2

#define SW_PLUS 20
#define SW_MINUS 21
#define SW_RE1 22
#define SW_RE2 23

typedef struct
{
    uint8_t id;
    uint8_t level;
} switch_event_t;

void switch_init(void);
void my_rotary_encoder_init(void);
void get_rc(void);
void input_test(void);
void kb_scan_task(void *dummy);
uint8_t rowcol_to_index(uint8_t row, uint8_t col);

#define SW_SCAN_TASK_STACK_SIZE 2048

extern QueueHandle_t rotary_encoder_event_queue;
extern QueueHandle_t switch_event_queue;

#ifdef __cplusplus
}
#endif

#endif


