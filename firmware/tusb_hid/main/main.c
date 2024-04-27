#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#include "input_task.h"
#include "sd_task.h"
#include "oled_task.h"

#include "driver/spi_master.h"

static const char *TAG = "MAIN";

#define OLED_MOSI 9
#define OLED_CS 11
#define OLED_DC 12
#define OLED_SCK 8

void app_main(void)
{
    gpio_init();
    sd_init();

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = OLED_SCK,
        .mosi_io_num = OLED_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 128*128/8,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
