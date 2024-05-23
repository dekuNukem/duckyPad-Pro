#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "rotary_encoder.h"

#include "esp_log.h"

static const char *TAG = "INPUT";

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive

sw_stat button_status[TOTAL_SW_COUNT];

rotary_encoder_info_t upper_rc_info;
rotary_encoder_info_t lower_rc_info;
QueueHandle_t event_queue;

void my_rotary_encoder_init(void)
{
	// esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    memset(&upper_rc_info, 0, sizeof(rotary_encoder_info_t));
    ESP_ERROR_CHECK(rotary_encoder_init(&upper_rc_info, RE1_A, RE1_B, ROTARY_ENCODER_UPPER));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&upper_rc_info, ENABLE_HALF_STEPS));

	memset(&lower_rc_info, 0, sizeof(rotary_encoder_info_t));
    ESP_ERROR_CHECK(rotary_encoder_init(&lower_rc_info, RE2_A, RE2_B, ROTARY_ENCODER_LOWER));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&lower_rc_info, ENABLE_HALF_STEPS));

    event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&upper_rc_info, event_queue));
	ESP_ERROR_CHECK(rotary_encoder_set_queue(&lower_rc_info, event_queue));
}

void get_rc(void)
{
	UBaseType_t items_in_queue = uxQueueMessagesWaiting(event_queue);
	printf("queue: %d\n", items_in_queue);
	// Wait for incoming events on the event queue.
	rotary_encoder_event_t event = { 0 };
	if (xQueueReceive(event_queue, &event, 0) == pdTRUE)
		printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
}

void input_test(void)
{
	printf("cs1: %d\n", gpio_get_level(SW_17_SPICS1_GPIO));
}

void switch_init(void)
{
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(SW_0_GPIO) | \
				BIT64(SW_1_GPIO) | \
				BIT64(SW_2_GPIO) | \
				BIT64(SW_3_GPIO) | \
				BIT64(SW_4_GPIO) | \
				BIT64(SW_5_GPIO) | \
				BIT64(SW_6_GPIO) | \
				BIT64(SW_7_GPIO) | \
				BIT64(SW_8_GPIO) | \
				BIT64(SW_9_GPIO) | \
				BIT64(SW_10_GPIO) | \
				BIT64(SW_11_GPIO) | \
				BIT64(SW_12_GPIO) | \
				BIT64(SW_13_GPIO) | \
				BIT64(SW_14_GPIO) | \
				BIT64(SW_15_GPIO) | \
				BIT64(SW_16_GPIO) | \
				BIT64(SW_17_SPICS1_GPIO) | \
			    BIT64(SW_18_U0RX_GPIO) | \
				BIT64(SW_19_GPIO) | \
                BIT64(SW_PLUS_GPIO) | \
                BIT64(SW_MINUS_GPIO) | \
                BIT64(SW_RE1_GPIO) | \
                BIT64(SW_RE2_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));
	// gpio_isr_handler_add(SW_15_GPIO, sw_isr, NULL);
	// gpio_get_level(SW_RE2);
}