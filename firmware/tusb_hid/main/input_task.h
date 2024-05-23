#ifndef __INPUT_TASK_H
#define __INPUT_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define SW_1 GPIO_NUM_15
#define SW_2 GPIO_NUM_16
#define SW_3 GPIO_NUM_17
#define SW_4 GPIO_NUM_18
#define SW_5 GPIO_NUM_21
#define SW_6 GPIO_NUM_48
#define SW_7 GPIO_NUM_47
#define SW_8 GPIO_NUM_33
#define SW_9 GPIO_NUM_34
#define SW_10 GPIO_NUM_35
#define SW_11 GPIO_NUM_36
#define SW_12 GPIO_NUM_37
#define SW_13 GPIO_NUM_38
#define SW_14 GPIO_NUM_39
#define SW_15 GPIO_NUM_40
#define SW_16 GPIO_NUM_41
#define SW_17 GPIO_NUM_42
#define SW_18_U0TX GPIO_NUM_43
#define SW_19_U0RX GPIO_NUM_44
#define SW_20 GPIO_NUM_45

#define SW_PLUS GPIO_NUM_13
#define SW_MINUS GPIO_NUM_14
#define SW_RE1 GPIO_NUM_0
#define SW_RE2 GPIO_NUM_3

#define RE1_A GPIO_NUM_1
#define RE1_B GPIO_NUM_2
#define RE2_A GPIO_NUM_4
#define RE2_B GPIO_NUM_5

#define ROTARY_ENCODER_UPPER 1
#define ROTARY_ENCODER_LOWER 2

void switch_init(void);
void my_rotary_encoder_init(void);
void get_rc(void);

#ifdef __cplusplus
}
#endif

#endif


