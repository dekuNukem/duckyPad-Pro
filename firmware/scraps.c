
/* Initialize the oled screen */
void ssd1306_init(void) {
    // Init OLED
    ssd1306_write_cmd_byte(0xAE); //display off
    ssd1306_write_cmd_byte(0x20); //Set Memory Addressing Mode
    ssd1306_write_cmd_byte(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd1306_write_cmd_byte(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd1306_write_cmd_byte(0xC0); // Mirror vertically
#else
    ssd1306_write_cmd_byte(0xC8); //Set COM Output Scan Direction
#endif

    ssd1306_write_cmd_byte(0x00); //---set low column address
    ssd1306_write_cmd_byte(0x10); //---set high column address

    ssd1306_write_cmd_byte(0x40); //--set start line address - CHECK

    // ssd1306_SetContrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_write_cmd_byte(0xA0); // Mirror horizontally
#else
    ssd1306_write_cmd_byte(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif


// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    ssd1306_write_cmd_byte(0xFF);
#else
    ssd1306_write_cmd_byte(0xA8); //--set multiplex ratio(1 to 64) - CHECK
#endif

#if (SSD1306_HEIGHT == 32)
    ssd1306_write_cmd_byte(0x1F); //
#elif (SSD1306_HEIGHT == 64)
    ssd1306_write_cmd_byte(0x3F); //
#elif (SSD1306_HEIGHT == 128)
    ssd1306_write_cmd_byte(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_write_cmd_byte(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd1306_write_cmd_byte(0xD3); //-set display offset - CHECK
    ssd1306_write_cmd_byte(0x00); //-not offset

    ssd1306_write_cmd_byte(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_write_cmd_byte(0xF0); //--set divide ratio

    ssd1306_write_cmd_byte(0xD9); //--set pre-charge period
    ssd1306_write_cmd_byte(0x22); //

    ssd1306_write_cmd_byte(0xDA); //--set com pins hardware configuration - CHECK
#if (SSD1306_HEIGHT == 32)
    ssd1306_write_cmd_byte(0x02);
#elif (SSD1306_HEIGHT == 64)
    ssd1306_write_cmd_byte(0x12);
#elif (SSD1306_HEIGHT == 128)
    ssd1306_write_cmd_byte(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_write_cmd_byte(0xDB); //--set vcomh
    ssd1306_write_cmd_byte(0x20); //0x20,0.77xVcc

    ssd1306_write_cmd_byte(0x8D); //--set DC-DC enable
    ssd1306_write_cmd_byte(0x14); //
    ssd1306_write_cmd_byte(0xAF); //display on

    // Clear screen
    ssd1306_Fill(Black);

    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
}

    
    spi_device_acquire_bus(my_spi_handle, portMAX_DELAY);
    ssd1306_init();
    // ssd1306_WriteString("hello", Font_7x10, White);
    ssd1306_UpdateScreen();
    spi_device_release_bus(my_spi_handle);

char* my_data = "hello";
spi_transaction_t my_transaction;

void write_test(void)
{
    ESP_ERROR_CHECK(spi_device_acquire_bus(my_spi_handle, portMAX_DELAY));
    uint8_t tx_size_bytes = strlen(my_data);

    memset(&my_transaction, 0, sizeof(my_transaction));
    my_transaction.length = tx_size_bytes * 8;
    my_transaction.tx_buffer = my_data;
    ESP_ERROR_CHECK(spi_device_polling_transmit(my_spi_handle, &my_transaction));
    spi_device_release_bus(my_spi_handle);
}

    spi_transaction_t t = {
        .length = 5,
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = test,
    };
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
switches
while(1)
    {
        printf("GPIO%d: %d\n", SW_1, gpio_get_level(SW_1));
        printf("GPIO%d: %d\n", SW_2, gpio_get_level(SW_2));
        printf("GPIO%d: %d\n", SW_3, gpio_get_level(SW_3));
        printf("GPIO%d: %d\n", SW_4, gpio_get_level(SW_4));
        printf("GPIO%d: %d\n", SW_5, gpio_get_level(SW_5));
        printf("GPIO%d: %d\n", SW_6, gpio_get_level(SW_6));
        printf("GPIO%d: %d\n", SW_7, gpio_get_level(SW_7));
        printf("GPIO%d: %d\n", SW_8, gpio_get_level(SW_8));
        printf("GPIO%d: %d\n", SW_9, gpio_get_level(SW_9));
        printf("GPIO%d: %d\n", SW_10, gpio_get_level(SW_10));
        printf("GPIO%d: %d\n", SW_11, gpio_get_level(SW_11));
        printf("GPIO%d: %d\n", SW_12, gpio_get_level(SW_12));
        printf("GPIO%d: %d\n", SW_13, gpio_get_level(SW_13));
        printf("GPIO%d: %d\n", SW_14, gpio_get_level(SW_14));
        printf("GPIO%d: %d\n", SW_15, gpio_get_level(SW_15));
        printf("GPIO%d: %d\n", SW_16, gpio_get_level(SW_16));
        printf("GPIO%d: %d\n", SW_17, gpio_get_level(SW_17));
        // printf("GPIO%d: %d\n", SW_18, gpio_get_level(SW_18));
        printf("GPIO%d: %d\n", SW_19_U0RX, gpio_get_level(SW_19_U0RX));
        printf("GPIO%d: %d\n", SW_20, gpio_get_level(SW_20));
        printf("------------\n\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

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

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
static const char *TAG = "example";

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

/********* Application ***************/

typedef enum {
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;

#define DISTANCE_MAX        125
#define DELTA_SCALAR        5

static void mouse_draw_square_next_delta(int8_t *delta_x_ret, int8_t *delta_y_ret)
{
    static mouse_dir_t cur_dir = MOUSE_DIR_RIGHT;
    static uint32_t distance = 0;

    // Calculate next delta
    if (cur_dir == MOUSE_DIR_RIGHT) {
        *delta_x_ret = DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_DOWN) {
        *delta_x_ret = 0;
        *delta_y_ret = DELTA_SCALAR;
    } else if (cur_dir == MOUSE_DIR_LEFT) {
        *delta_x_ret = -DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_UP) {
        *delta_x_ret = 0;
        *delta_y_ret = -DELTA_SCALAR;
    }

    // Update cumulative distance for current direction
    distance += DELTA_SCALAR;
    // Check if we need to change direction
    if (distance >= DISTANCE_MAX) {
        distance = 0;
        cur_dir++;
        if (cur_dir == MOUSE_DIR_MAX) {
            cur_dir = 0;
        }
    }
}

static void app_send_hid_demo(void)
{
    // Keyboard output: Send key 'a/A' pressed and released
    ESP_LOGI(TAG, "Sending Keyboard report");
    uint8_t keycode[6] = {HID_KEY_A};
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    vTaskDelay(pdMS_TO_TICKS(50));
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);

    // Mouse output: Move mouse cursor in square trajectory
    ESP_LOGI(TAG, "Sending Mouse report");
    int8_t delta_x;
    int8_t delta_y;
    for (int i = 0; i < (DISTANCE_MAX / DELTA_SCALAR) * 4; i++) {
        // Get the next x and y delta in the draw square pattern
        mouse_draw_square_next_delta(&delta_x, &delta_y);
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void)
{
    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    while (1) {
        if (tud_mounted()) {
            static bool send_hid_data = true;
            if (send_hid_data) {
                app_send_hid_demo();
            }
            send_hid_data = !gpio_get_level(APP_BUTTON);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
