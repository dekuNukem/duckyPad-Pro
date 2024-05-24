#include <string.h>
#include "driver/gpio.h"
#include "input_task.h"
#include "rotary_encoder.h"

#include "esp_log.h"

static const char *TAG = "INPUT";

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive
#define SWITCH_EVENT_QUEUE_SIZE 10

const uint8_t sw_index_to_gpio_lookup[TOTAL_SW_COUNT] = {GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_21, GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_33, GPIO_NUM_34, GPIO_NUM_35, GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_38, GPIO_NUM_39, GPIO_NUM_40, GPIO_NUM_41, GPIO_NUM_42, GPIO_NUM_26, GPIO_NUM_44, GPIO_NUM_45, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_0, GPIO_NUM_3};
uint8_t switch_status_all[TOTAL_SW_COUNT];

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

void input_test(void)
{
	switch_event_t this_sw_event;
	if (xQueueReceive(switch_event_queue, &this_sw_event, 0) == pdTRUE)
		printf("id: %d lvl: %d\n", this_sw_event.id, this_sw_event.level);
}

volatile uint32_t last_sw_isr;
volatile switch_event_t sw_event;

void sw_isr(void * args)
{
	uint32_t now = xTaskGetTickCountFromISR();
	sw_event.id = (uint8_t)args;
	sw_event.level = 1 - gpio_get_level(sw_index_to_gpio_lookup[sw_event.id]);
	switch_status_all[sw_event.id] = sw_event.level;

	if(now - last_sw_isr <= 66)
		return;
	xQueueSendFromISR(switch_event_queue, &sw_event, NULL);
	last_sw_isr = now;
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
				BIT64(SW_17_GPIO) | \
			    BIT64(SW_18_GPIO) | \
				BIT64(SW_19_GPIO) | \
                BIT64(SW_PLUS_GPIO) | \
                BIT64(SW_MINUS_GPIO) | \
                BIT64(SW_RE1_GPIO) | \
                BIT64(SW_RE2_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));
	gpio_isr_handler_add(SW_0_GPIO, sw_isr, SW_0);
	gpio_isr_handler_add(SW_1_GPIO, sw_isr, SW_1);
	gpio_isr_handler_add(SW_2_GPIO, sw_isr, SW_2);
	gpio_isr_handler_add(SW_3_GPIO, sw_isr, SW_3);
	gpio_isr_handler_add(SW_4_GPIO, sw_isr, SW_4);
	gpio_isr_handler_add(SW_5_GPIO, sw_isr, SW_5);
	gpio_isr_handler_add(SW_6_GPIO, sw_isr, SW_6);
	gpio_isr_handler_add(SW_7_GPIO, sw_isr, SW_7);
	gpio_isr_handler_add(SW_8_GPIO, sw_isr, SW_8);
	gpio_isr_handler_add(SW_9_GPIO, sw_isr, SW_9);
	gpio_isr_handler_add(SW_10_GPIO, sw_isr, SW_10);
	gpio_isr_handler_add(SW_11_GPIO, sw_isr, SW_11);
	gpio_isr_handler_add(SW_12_GPIO, sw_isr, SW_12);
	gpio_isr_handler_add(SW_13_GPIO, sw_isr, SW_13);
	gpio_isr_handler_add(SW_14_GPIO, sw_isr, SW_14);
	gpio_isr_handler_add(SW_15_GPIO, sw_isr, SW_15);
	gpio_isr_handler_add(SW_16_GPIO, sw_isr, SW_16);
	gpio_isr_handler_add(SW_17_GPIO, sw_isr, SW_17);
	gpio_isr_handler_add(SW_18_GPIO, sw_isr, SW_18);
	gpio_isr_handler_add(SW_19_GPIO, sw_isr, SW_19);
	gpio_isr_handler_add(SW_PLUS_GPIO, sw_isr, SW_PLUS);
	gpio_isr_handler_add(SW_MINUS_GPIO, sw_isr, SW_MINUS);
	gpio_isr_handler_add(SW_RE1_GPIO, sw_isr, SW_RE1);
	gpio_isr_handler_add(SW_RE2_GPIO, sw_isr, SW_RE2);
	switch_event_queue = xQueueCreate(SWITCH_EVENT_QUEUE_SIZE, sizeof(switch_event_t));
}