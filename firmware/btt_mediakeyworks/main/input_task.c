#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "esp_log.h"

static const char *TAG = "INPUT";

QueueHandle_t switch_event_queue;

#define SWITCH_EVENT_QUEUE_SIZE 8
#define TOTAL_OBSW_COUNT 24

uint8_t this_sw_state[TOTAL_OBSW_COUNT];
uint8_t last_sw_state[TOTAL_OBSW_COUNT];

void kb_scan_task(void *dummy)
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(20));

        this_sw_state[SW_PLUS] = 1 - gpio_get_level(SW_PLUS_GPIO);
		this_sw_state[SW_MINUS] = 1 - gpio_get_level(SW_MINUS_GPIO);

        for (uint8_t i = 0; i < TOTAL_OBSW_COUNT; i++)
		{
			if(this_sw_state[i] == 1 && last_sw_state[i] == 0)
			{
				switch_event_t sw_event = 
				{
					.id = i,
					.type = SW_EVENT_SHORT_PRESS,
				};
				xQueueSend(switch_event_queue, &sw_event, NULL);
			}
			else if(this_sw_state[i] == 0 && last_sw_state[i] == 1)
			{
				switch_event_t sw_event = 
				{
					.id = i,
					.type = SW_EVENT_RELEASE,
				};
				xQueueSend(switch_event_queue, &sw_event, NULL);
			}
		}
		memcpy(last_sw_state, this_sw_state, TOTAL_OBSW_COUNT);
	}
}

void switch_init(void)
{
	const gpio_config_t button_config = {
        .pin_bit_mask = BIT64(SW_PLUS_GPIO) | BIT64(SW_MINUS_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&button_config));

	switch_event_queue = xQueueCreate(SWITCH_EVENT_QUEUE_SIZE, sizeof(switch_event_t));
}