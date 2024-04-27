#include "driver/gpio.h"

#include "input_task.h"

void gpio_init(void)
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
                BIT64(RE1_A) | \
                BIT64(RE1_B) | \
                BIT64(RE2_SW) | \
                BIT64(RE2_A) | \
                BIT64(RE2_B),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));
}