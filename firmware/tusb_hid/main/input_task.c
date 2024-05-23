#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "rotary_encoder.h"

#include "esp_log.h"

static const char *TAG = "INPUT";

#define ROT_ENC_A_GPIO (RE1_A)
#define ROT_ENC_B_GPIO (RE1_B)

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive

rotary_encoder_info_t info;
QueueHandle_t event_queue;

void my_rotary_encoder_init(void)
{
	// esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    memset(&info, 0, sizeof(rotary_encoder_info_t));
    ESP_ERROR_CHECK(rotary_encoder_init(&info, ROT_ENC_A_GPIO, ROT_ENC_B_GPIO));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&info, ENABLE_HALF_STEPS));
    event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&info, event_queue));
}

void get_rc(void)
{
	UBaseType_t items_in_queue = uxQueueMessagesWaiting(event_queue);
	printf("queue: %d\n", items_in_queue);
	// Wait for incoming events on the event queue.
	rotary_encoder_event_t event = { 0 };
	if (xQueueReceive(event_queue, &event, 500) == pdTRUE)
		printf("Event: position %ld, direction %d\n", event.state.position, event.state.direction);
}

void switch_init(void)
{
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(SW_1) | \
				BIT64(SW_2) | \
				BIT64(SW_3) | \
				BIT64(SW_4) | \
				BIT64(SW_5) | \
				BIT64(SW_6) | \
				BIT64(SW_7) | \
				BIT64(SW_8) | \
				BIT64(SW_9) | \
				BIT64(SW_10) | \
				BIT64(SW_11) | \
				BIT64(SW_12) | \
				BIT64(SW_13) | \
				BIT64(SW_14) | \
				BIT64(SW_15) | \
				BIT64(SW_16) | \
				BIT64(SW_17) | \
				/* BIT64(SW_18_U0TX) | */ \
			    BIT64(SW_19_U0RX) | \
				BIT64(SW_20) | \
                BIT64(SW_PLUS) | \
                BIT64(SW_MINUS) | \
                BIT64(RE1_SW) | \
                BIT64(RE2_SW),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));
	
}