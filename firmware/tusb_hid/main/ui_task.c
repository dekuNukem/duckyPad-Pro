#include "ui_task.h"
#include "driver/gpio.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "esp_log.h"
#include <string.h>
#include "shared.h"

static const char *TAG = "UI";
spi_device_handle_t my_spi_handle;
gpio_config_t oled_dc_cofig;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;

void oled_init(void)
{
    oled_dc_cofig.pin_bit_mask = BIT64(OLED_DC);
    oled_dc_cofig.mode = GPIO_MODE_OUTPUT;
    oled_dc_cofig.intr_type = GPIO_INTR_DISABLE;
    oled_dc_cofig.pull_up_en = false;
    oled_dc_cofig.pull_down_en = false;
    ESP_ERROR_CHECK(gpio_config(&oled_dc_cofig));

    buscfg.sclk_io_num = OLED_SCK;
    buscfg.mosi_io_num = OLED_MOSI;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 128;
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    devcfg.clock_speed_hz = 2000000;
    devcfg.mode = 0;
    devcfg.queue_size = 1;
    devcfg.spics_io_num = OLED_CS;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &my_spi_handle));
    ssd1306_init();
    ssd1306_SetCursor(20, 60);
    ssd1306_WriteString("hello world!!", Font_7x10, White);
    ssd1306_UpdateScreen();
}

void print_nosd(void)
{
    printf("NO SD!\n");
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 5);
    ssd1306_WriteString("Please Insert SD Card",Font_7x10,White);
    ssd1306_UpdateScreen();
}