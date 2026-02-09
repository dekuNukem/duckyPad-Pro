#ifndef __MYPWM_TASK_H
#define __MYPWM_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (34) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (8192) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_CLK_SRC            LEDC_AUTO_CLK
#define LEDC_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 4 kHz

void example_ledc_init(void);
void my_pwm_test(void);

#ifdef __cplusplus
}
#endif

#endif


