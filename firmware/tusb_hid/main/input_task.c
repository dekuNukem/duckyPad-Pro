#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "rotary_encoder.h"
#include "shared.h"
#include "esp_log.h"

static const char *TAG = "INPUT";

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive
#define SWITCH_EVENT_QUEUE_SIZE 10

const uint8_t sw_index_to_gpio_lookup[TOTAL_SW_COUNT] = {GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_21, GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_33, GPIO_NUM_34, GPIO_NUM_35, GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_38, GPIO_NUM_39, GPIO_NUM_40, GPIO_NUM_41, GPIO_NUM_42, GPIO_NUM_26, GPIO_NUM_44, GPIO_NUM_45, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_0, GPIO_NUM_3};
switch_status_t sw_status[TOTAL_SW_COUNT];

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

void get_rc(void)
{
	UBaseType_t items_in_queue = uxQueueMessagesWaiting(rotary_encoder_event_queue);
	printf("queue: %d\n", items_in_queue);
	// Wait for incoming events on the event queue.
	rotary_encoder_event_t event = { 0 };
	if (xQueueReceive(rotary_encoder_event_queue, &event, 0) == pdTRUE)
		printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
}

uint8_t this_sw_matrix_state[SW_MATRIX_BUF_SIZE];
uint8_t last_sw_matrix_state[SW_MATRIX_BUF_SIZE];

uint8_t rowcol_to_index(uint8_t row, uint8_t col)
{
	if(row >= SW_MATRIX_NUM_ROWS || col >= SW_MATRIX_NUM_COLS)
		return 0;
	return row*SW_MATRIX_NUM_COLS + col;
}

void scan_row(uint8_t this_col)
{
	this_sw_matrix_state[rowcol_to_index(0, this_col)] = gpio_get_level(SWM_ROW0_GPIO);
	this_sw_matrix_state[rowcol_to_index(1, this_col)] = gpio_get_level(SWM_ROW1_GPIO);
	this_sw_matrix_state[rowcol_to_index(2, this_col)] = gpio_get_level(SWM_ROW2_GPIO);
	this_sw_matrix_state[rowcol_to_index(3, this_col)] = gpio_get_level(SWM_ROW3_GPIO);
	this_sw_matrix_state[rowcol_to_index(4, this_col)] = gpio_get_level(SWM_ROW4_GPIO);
}

void input_test(void)
{
	memset(this_sw_matrix_state, 0, SW_MATRIX_BUF_SIZE);
	gpio_set_level(SWM_COL0_GPIO, 1);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);
	vTaskDelay(pdMS_TO_TICKS(1));
	scan_row(0);

	gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 1);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);
	vTaskDelay(pdMS_TO_TICKS(1));
	scan_row(1);

	gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 1);
    gpio_set_level(SWM_COL3_GPIO, 0);
	vTaskDelay(pdMS_TO_TICKS(1));
	scan_row(2);

	gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 1);
	vTaskDelay(pdMS_TO_TICKS(1));
	scan_row(3);

	// turn everything off, needed
	gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);
	vTaskDelay(pdMS_TO_TICKS(1));

	for (int rrr = 0; rrr < SW_MATRIX_NUM_ROWS; ++rrr)
    {
        for (int ccc = 0; ccc < SW_MATRIX_NUM_COLS; ++ccc)
        {
            // printf("R%dC%dS%d ", rrr, ccc, this_sw_matrix_state[rowcol_to_index(rrr, ccc)]);
			printf("%d ", this_sw_matrix_state[rowcol_to_index(rrr, ccc)]);
        }
        printf("\n");
    }

	printf("---------------\n");
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

    gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);

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