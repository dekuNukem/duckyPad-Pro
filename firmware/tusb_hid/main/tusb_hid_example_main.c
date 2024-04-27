/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#include "input_task.h"

static const char *TAG = "MAIN";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD) ),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE) )
};

/**
 * @brief String descriptor
 */
const char* hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "Example HID interface",  // 4: HID
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

#define SW_1 GPIO_NUM_15
#define SW_2 GPIO_NUM_16
#define SW_3 GPIO_NUM_17
#define SW_4 GPIO_NUM_18
#define SW_5 GPIO_NUM_21
#define SW_6 GPIO_NUM_48
#define SW_7 GPIO_NUM_47
#define SW_8 GPIO_NUM_33
#define SW_9 GPIO_NUM_34
#define SW_10 GPIO_NUM_35
#define SW_11 GPIO_NUM_36
#define SW_12 GPIO_NUM_37
#define SW_13 GPIO_NUM_38
#define SW_14 GPIO_NUM_39
#define SW_15 GPIO_NUM_40
#define SW_16 GPIO_NUM_41
#define SW_17 GPIO_NUM_42
#define SW_18_U0TX GPIO_NUM_43
#define SW_19_U0RX GPIO_NUM_44
#define SW_20 GPIO_NUM_45

#define SW_PLUS GPIO_NUM_13
#define SW_MINUS GPIO_NUM_14
#define RE1_SW GPIO_NUM_0
#define RE1_A GPIO_NUM_1
#define RE1_B GPIO_NUM_2
#define RE2_SW GPIO_NUM_3
#define RE2_A GPIO_NUM_4
#define RE2_B GPIO_NUM_5

void app_main(void)
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
    
    while(1)
    {
        printf("GPIO%d: %d\n", SW_PLUS, gpio_get_level(SW_PLUS));
        printf("GPIO%d: %d\n", SW_MINUS, gpio_get_level(SW_MINUS));
        printf("GPIO%d: %d\n", RE1_SW, gpio_get_level(RE1_SW));
        printf("GPIO%d: %d\n", RE1_A, gpio_get_level(RE1_A));
        printf("GPIO%d: %d\n", RE1_B, gpio_get_level(RE1_B));
        printf("GPIO%d: %d\n", RE2_SW, gpio_get_level(RE2_SW));
        printf("GPIO%d: %d\n", RE2_A, gpio_get_level(RE2_A));
        printf("GPIO%d: %d\n", RE2_B, gpio_get_level(RE2_B));
        printf("------------\n\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
