#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "rotary_encoder.h"
#include "shared.h"
#include "esp_log.h"

static const char *TAG = "INPUT";

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive
#define SWITCH_EVENT_QUEUE_SIZE 10

rotary_encoder_info_t upper_rc_info;
rotary_encoder_info_t lower_rc_info;
QueueHandle_t rotary_encoder_event_queue;
QueueHandle_t switch_event_queue;

void my_rotary_encoder_init(void)
{
	// esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()

	// below is called in MAIN, no need to call twice
    // ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    memset(&upper_rc_info, 0, sizeof(rotary_encoder_info_t));
    ESP_ERROR_CHECK(rotary_encoder_init(&upper_rc_info, RE1_A, RE1_B, ROTARY_ENCODER_UPPER));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&upper_rc_info, ENABLE_HALF_STEPS));

	memset(&lower_rc_info, 0, sizeof(rotary_encoder_info_t));
    ESP_ERROR_CHECK(rotary_encoder_init(&lower_rc_info, RE2_A, RE2_B, ROTARY_ENCODER_LOWER));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&lower_rc_info, ENABLE_HALF_STEPS));

    rotary_encoder_event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&upper_rc_info, rotary_encoder_event_queue));
	ESP_ERROR_CHECK(rotary_encoder_set_queue(&lower_rc_info, rotary_encoder_event_queue));
}

uint8_t this_sw_state[TOTAL_OBSW_COUNT];
uint8_t last_sw_state[TOTAL_OBSW_COUNT];
uint32_t last_press_ts[TOTAL_OBSW_COUNT];

uint8_t rowcol_to_index(uint8_t row, uint8_t col)
{
	if(row >= SW_MATRIX_NUM_ROWS || col >= SW_MATRIX_NUM_COLS)
		return 0;
	return row*SW_MATRIX_NUM_COLS + col;
}

void scan_row(uint8_t *sw_buf, uint8_t this_col)
{
	sw_buf[rowcol_to_index(0, this_col)] = gpio_get_level(SWM_ROW0_GPIO);
	sw_buf[rowcol_to_index(1, this_col)] = gpio_get_level(SWM_ROW1_GPIO);
	sw_buf[rowcol_to_index(2, this_col)] = gpio_get_level(SWM_ROW2_GPIO);
	sw_buf[rowcol_to_index(3, this_col)] = gpio_get_level(SWM_ROW3_GPIO);
	sw_buf[rowcol_to_index(4, this_col)] = gpio_get_level(SWM_ROW4_GPIO);
}

void sw_matrix_col_reset(void)
{
	gpio_set_level(SWM_COL0_GPIO, 0);
	gpio_set_level(SWM_COL1_GPIO, 0);
	gpio_set_level(SWM_COL2_GPIO, 0);
	gpio_set_level(SWM_COL3_GPIO, 0);
}

void kb_scan_task(void *dummy)
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(20));

		memset(this_sw_state, 0, TOTAL_OBSW_COUNT);
		gpio_set_level(SWM_COL0_GPIO, 1);
		gpio_set_level(SWM_COL1_GPIO, 0);
		gpio_set_level(SWM_COL2_GPIO, 0);
		gpio_set_level(SWM_COL3_GPIO, 0);
		vTaskDelay(pdMS_TO_TICKS(1));
		scan_row(this_sw_state, 0);

		gpio_set_level(SWM_COL0_GPIO, 0);
		gpio_set_level(SWM_COL1_GPIO, 1);
		gpio_set_level(SWM_COL2_GPIO, 0);
		gpio_set_level(SWM_COL3_GPIO, 0);
		vTaskDelay(pdMS_TO_TICKS(1));
		scan_row(this_sw_state, 1);

		gpio_set_level(SWM_COL0_GPIO, 0);
		gpio_set_level(SWM_COL1_GPIO, 0);
		gpio_set_level(SWM_COL2_GPIO, 1);
		gpio_set_level(SWM_COL3_GPIO, 0);
		vTaskDelay(pdMS_TO_TICKS(1));
		scan_row(this_sw_state, 2);

		gpio_set_level(SWM_COL0_GPIO, 0);
		gpio_set_level(SWM_COL1_GPIO, 0);
		gpio_set_level(SWM_COL2_GPIO, 0);
		gpio_set_level(SWM_COL3_GPIO, 1);
		vTaskDelay(pdMS_TO_TICKS(1));
		scan_row(this_sw_state, 3);

		sw_matrix_col_reset(); // need time to settle, do not remove
		vTaskDelay(pdMS_TO_TICKS(1));

		this_sw_state[SW_PLUS] = 1 - gpio_get_level(SW_PLUS_GPIO);
		this_sw_state[SW_MINUS] = 1 - gpio_get_level(SW_MINUS_GPIO);
		this_sw_state[SW_RE1] = 1 - gpio_get_level(SW_RE1_GPIO);
		this_sw_state[SW_RE2] = 1 - gpio_get_level(SW_RE2_GPIO);

		for (uint8_t i = 0; i < TOTAL_OBSW_COUNT; i++)
		{
			if(this_sw_state[i] == 1 && last_sw_state[i] == 0)
			{
				switch_event_t sw_event = 
				{
					.id = i,
					.type = SW_EVENT_SHORT_PRESS,
				};
				last_press_ts[i] = millis();
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
			else if(this_sw_state[i] == 1 && last_press_ts[i] != MY_UINT32_MAX && millis() - last_press_ts[i] > 500)
			{
				switch_event_t sw_event = 
				{
					.id = i,
					.type = SW_EVENT_LONG_PRESS,
				};
				last_press_ts[i] = MY_UINT32_MAX;
				xQueueSend(switch_event_queue, &sw_event, NULL);
			}
		}
		memcpy(last_sw_state, this_sw_state, TOTAL_OBSW_COUNT);
	}
}

void sd_card_det_isr(void* args)
{
	esp_restart();
}

void switch_init(void)
{
	const gpio_config_t sw_matrix_col_config = {
        .pin_bit_mask = BIT64(SWM_COL0_GPIO) | \
                BIT64(SWM_COL1_GPIO) | \
                BIT64(SWM_COL2_GPIO) | \
                BIT64(SWM_COL3_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = false,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&sw_matrix_col_config));
    sw_matrix_col_reset();

    const gpio_config_t sw_matrix_row_config = {
        .pin_bit_mask = BIT64(SWM_ROW0_GPIO) | \
                BIT64(SWM_ROW1_GPIO) | \
                BIT64(SWM_ROW2_GPIO) | \
                BIT64(SWM_ROW3_GPIO) | \
                BIT64(SWM_ROW4_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = false,
        .pull_down_en = true,
    };
    ESP_ERROR_CHECK(gpio_config(&sw_matrix_row_config));

    const gpio_config_t button_config = {
        .pin_bit_mask = BIT64(SW_PLUS_GPIO) | \
                BIT64(SW_MINUS_GPIO) | \
                BIT64(SW_RE1_GPIO) | \
                BIT64(SW_RE2_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&button_config));

    const gpio_config_t sd_det_config = {
        .pin_bit_mask = BIT64(SD_CARD_DETECT_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&sd_det_config));
	gpio_isr_handler_add(SD_CARD_DETECT_GPIO, sd_card_det_isr, NULL);
	switch_event_queue = xQueueCreate(SWITCH_EVENT_QUEUE_SIZE, sizeof(switch_event_t));
}