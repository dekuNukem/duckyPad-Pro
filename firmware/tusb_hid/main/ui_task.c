#include "ui_task.h"
#include "driver/gpio.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "UI";
spi_device_handle_t my_spi_handle;

void oled_init(void)
{

    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(OLED_DC),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = false,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = OLED_SCK,
        .mosi_io_num = OLED_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 128,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,
        .mode = 0,
        .queue_size = 1,
        .spics_io_num = OLED_CS,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &my_spi_handle));

    ssd1306_init();
    ssd1306_SetCursor(20, 20);
    ssd1306_WriteString("hello world!", Font_7x10, White);
    ssd1306_UpdateScreen();
}
