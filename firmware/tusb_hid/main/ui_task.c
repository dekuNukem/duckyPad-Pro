#include "ui_task.h"
#include "driver/gpio.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "esp_log.h"
#include <string.h>
#include "shared.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "UI";
spi_device_handle_t my_spi_handle;
gpio_config_t oled_dc_cofig;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;

void oled_init(void)
{
    oled_dc_cofig.pin_bit_mask = BIT64(OLED_DC) | BIT64(OLED_RESET);
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

    gpio_set_level(OLED_RESET, 0);
    vTaskDelay(pdMS_TO_TICKS(25));
    gpio_set_level(OLED_RESET, 1);
    vTaskDelay(pdMS_TO_TICKS(25));

    ssd1306_init();
    ssd1306_SetCursor(20, 60);
    ssd1306_WriteString("hello world!!", Font_7x10, White);
    ssd1306_UpdateScreen();
}

uint8_t center_line(uint8_t line_len, uint8_t char_width_pixels, uint8_t oled_width_pixels)
{
  int16_t start_pixel = (oled_width_pixels - line_len * char_width_pixels) / 2;
  if(start_pixel < 0)
    start_pixel = 0;
  return start_pixel;
}

#define OLED_LINE_BUF_SIZE 32
char oled_line_buf[OLED_LINE_BUF_SIZE];

void print_nosd(void)
{
    ssd1306_Fill(Black);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "duckyPad Pro");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 10);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "FW V%d.%d.%d", fw_version_major, fw_version_minor, fw_version_patch);
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 25);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);


    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Please Insert");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 55);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "SD card");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 67);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);


    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Instructions:");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 100);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "duckyPad.com");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 112);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    ssd1306_UpdateScreen();
}