#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "buttons.h"
#include "main.h"

uint8_t last_channel_status[CHANNEL_COUNT];

void channel_update(void)
{
  uint8_t this_channel_status[CHANNEL_COUNT];

  this_channel_status[CHANNEL_1] = HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin);
  this_channel_status[CHANNEL_2] = HAL_GPIO_ReadPin(CH2_GPIO_Port, CH2_Pin);
  this_channel_status[CHANNEL_3] = HAL_GPIO_ReadPin(CH3_GPIO_Port, CH3_Pin);
  this_channel_status[CHANNEL_4] = HAL_GPIO_ReadPin(CH4_GPIO_Port, CH4_Pin);
  this_channel_status[CHANNEL_5] = HAL_GPIO_ReadPin(CH5_GPIO_Port, CH5_Pin);
  this_channel_status[CHANNEL_6] = HAL_GPIO_ReadPin(CH6_GPIO_Port, CH6_Pin);

  for (int i = 0; i < CHANNEL_COUNT; ++i)
  {
    if(this_channel_status[i] == 0 && last_channel_status[i] == 1)
      printf("channel %d pressed!\n", i);
    if(this_channel_status[i] == 1 && last_channel_status[i] == 0)
      printf("channel %d released!\n", i);
  }
  memcpy(last_channel_status, this_channel_status, CHANNEL_COUNT);
}
