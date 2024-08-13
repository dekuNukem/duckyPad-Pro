#include "stm32f0xx_hal.h"
#include "delay_us.h"

TIM_HandleTypeDef* us_timer;

void delay_us_init(TIM_HandleTypeDef* htim_base)
{
	us_timer = htim_base;
	HAL_TIM_Base_Start(us_timer);
}

uint32_t micros(void)
{
  return us_timer->Instance->CNT;
}

void delay_us(uint32_t delay)
{
  uint32_t end_time = micros() + delay;
  while(micros() < end_time);
}

