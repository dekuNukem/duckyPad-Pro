#ifndef __INPUT_TASK_H
#define __INPUT_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define TOTAL_SW_COUNT 24
#define MAPPABLE_SW_COUNT (TOTAL_SW_COUNT-2)

#define SW_0_GPIO GPIO_NUM_15
#define SW_1_GPIO GPIO_NUM_16
#define SW_2_GPIO GPIO_NUM_17
#define SW_3_GPIO GPIO_NUM_18
#define SW_4_GPIO GPIO_NUM_21
#define SW_5_GPIO GPIO_NUM_48
#define SW_6_GPIO GPIO_NUM_47
#define SW_7_GPIO GPIO_NUM_33
#define SW_8_GPIO GPIO_NUM_34
#define SW_9_GPIO GPIO_NUM_35
#define SW_10_GPIO GPIO_NUM_36
#define SW_11_GPIO GPIO_NUM_37
#define SW_12_GPIO GPIO_NUM_38
#define SW_13_GPIO GPIO_NUM_39
#define SW_14_GPIO GPIO_NUM_40
#define SW_15_GPIO GPIO_NUM_41
#define SW_16_GPIO GPIO_NUM_42
#define SW_17_GPIO GPIO_NUM_26
#define SW_18_GPIO GPIO_NUM_44
#define SW_19_GPIO GPIO_NUM_45

#define SW_PLUS_GPIO GPIO_NUM_13
#define SW_MINUS_GPIO GPIO_NUM_14
#define SW_RE1_GPIO GPIO_NUM_0
#define SW_RE2_GPIO GPIO_NUM_3

#define SD_CARD_DETECT_GPIO GPIO_NUM_6

#define RE1_A GPIO_NUM_1
#define RE1_B GPIO_NUM_2
#define RE2_A GPIO_NUM_4
#define RE2_B GPIO_NUM_5

#define ROTARY_ENCODER_UPPER 1
#define ROTARY_ENCODER_LOWER 2

#define SW_0 0
#define SW_1 1
#define SW_2 2
#define SW_3 3
#define SW_4 4
#define SW_5 5
#define SW_6 6
#define SW_7 7
#define SW_8 8
#define SW_9 9
#define SW_10 10
#define SW_11 11
#define SW_12 12
#define SW_13 13
#define SW_14 14
#define SW_15 15
#define SW_16 16
#define SW_17 17
#define SW_18 18
#define SW_19 19

#define SW_PLUS 20
#define SW_MINUS 21
#define SW_RE1 22
#define SW_RE2 23

typedef struct
{
    uint8_t level;
    uint32_t last_press_ms;
} switch_status_t;

typedef struct
{
    uint8_t id;
    uint8_t level;
} switch_event_t;

void switch_init(void);
void my_rotary_encoder_init(void);
void get_rc(void);
void input_test(void);

#ifdef __cplusplus
}
#endif

#endif


