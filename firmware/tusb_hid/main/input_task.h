#ifndef __INPUT_TASK_H
#define __INPUT_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "rotary_encoder.h"

#define SW_MATRIX_NUM_COLS 4
#define SW_MATRIX_NUM_ROWS 5

// OB = onboard
#define MECH_OBSW_COUNT (SW_MATRIX_NUM_COLS * SW_MATRIX_NUM_ROWS)
#define TOTAL_OBSW_COUNT (28)

#define MAX_TOTAL_SW_COUNT 100

#define SW_PLUS_GPIO GPIO_NUM_17
#define SW_MINUS_GPIO GPIO_NUM_18
#define SW_RE1_GPIO GPIO_NUM_0
#define SW_RE2_GPIO GPIO_NUM_3

#define SD_CARD_DETECT_GPIO GPIO_NUM_6

#define RE1_A GPIO_NUM_2
#define RE1_B GPIO_NUM_1
#define RE2_A GPIO_NUM_5
#define RE2_B GPIO_NUM_4

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

#define SW_EVENT_RELEASE 0
#define SW_EVENT_SHORT_PRESS 1
#define SW_EVENT_LONG_PRESS 2

// mechnical switches
#define MSW_0 0
#define MSW_1 1
#define MSW_2 2
#define MSW_3 3
#define MSW_4 4
#define MSW_5 5
#define MSW_6 6
#define MSW_7 7
#define MSW_8 8
#define MSW_9  9
#define MSW_10  10
#define MSW_11  11
#define MSW_12  12
#define MSW_13  13
#define MSW_14  14
#define MSW_15  15
#define MSW_16  16
#define MSW_17  17
#define MSW_18  18
#define MSW_19  19
#define MAX_MSW MSW_19
#define RE1_CW 20
#define RE1_CCW 21
#define RE1_SW 22
#define RE2_CW 23
#define RE2_CCW 24
#define RE2_SW 25
#define SW_MINUS 26
#define SW_PLUS 27

#define EXP_BUTTON_START 36
#define EXP_BUTTON_END 99

/*
0 to 19: mechanical switches
20 to 25: rotary encoders
26 to 35: spare gpio pins, unused
36 to 99: expansion channels
*/

typedef struct
{
    uint8_t id;
    uint8_t type;
} switch_event_t;

void switch_init(void);
void my_rotary_encoder_init(void);
void kb_scan_task(void *dummy);
uint8_t rowcol_to_index(uint8_t row, uint8_t col);
uint8_t poll_sw_state(uint8_t swid_zero_indexed, uint8_t perform_new_scan);
void clear_sw_re_queue(void);
void expansion_uart_init(void);
uint8_t sw_queue_has_keydown_event(void);
uint8_t re_event_to_swid(rotary_encoder_event_t* re_event);

#define SW_SCAN_TASK_STACK_SIZE 2048

extern QueueHandle_t rotary_encoder_event_queue;
extern QueueHandle_t switch_event_queue;

#define KBSCAN_MUTEX_TIMEOUT_MS 100
#define INPUT_TASK_FREQ_MS 13

#ifdef __cplusplus
}
#endif

#endif


