#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "buttons.h"
#include "main.h"
#include "cQueue.h"

uint8_t last_channel_status[CHANNEL_COUNT];
Queue_t switch_event_queue;

void channel_update(uint8_t start_id)
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
    {
      uint8_t this_id = start_id + i;
      if(this_id >= MAX_CHANNELS)
        continue;
      uint8_t this_cmd = (this_id & 0x3f) | CMD_SW_PRESS_BITMASK;
      __disable_irq();
      q_push(&switch_event_queue, &this_cmd);
      __enable_irq();
    }
    if(this_channel_status[i] == 1 && last_channel_status[i] == 0)
    {
      uint8_t this_id = start_id + i;
      if(this_id >= MAX_CHANNELS)
        continue;
      uint8_t this_cmd = (this_id & 0x3f) | CMD_SW_RELEASE_BITMASK;
      __disable_irq();
      q_push(&switch_event_queue, &this_cmd);
      __enable_irq();
    }
  }
  memcpy(last_channel_status, this_channel_status, CHANNEL_COUNT);
}

void sw_event_queue_init(void)
{
  q_init(&switch_event_queue, sizeof(uint8_t), SW_EVENT_QUEUE_SIZE, FIFO, false);
  memset(last_channel_status, 1, CHANNEL_COUNT);
}

