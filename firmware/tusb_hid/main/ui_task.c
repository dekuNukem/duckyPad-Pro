#include "ui_task.h"
#include "driver/gpio.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "esp_log.h"
#include <string.h>
#include "shared.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "input_task.h"

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

void print_noprofile(void)
{
    ssd1306_Fill(Black);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "No Valid");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 35);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Profiles");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 47);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Instructions:");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 80);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "duckyPad.com");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 92);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    ssd1306_UpdateScreen();
}


void print_nomemory(void)
{
    ssd1306_Fill(Black);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Memory Allocation");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 35);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Failed!");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 47);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Instructions:");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 80);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "duckyPad.com");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 92);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    ssd1306_UpdateScreen();
}


#define CELL_LONG_SIDE 32
#define CELL_SHORT_SIDE 23
#define TITLE_BAR_HEIGHT 11
#define CELL_LINE_BUF_SIZE 16
char cell_temp_buf[CELL_LINE_BUF_SIZE];
char cell_first_line[CELL_LINE_BUF_SIZE];
char cell_second_line[CELL_LINE_BUF_SIZE];

void draw_cell_content(uint8_t cell_x, uint8_t cell_y, char* name)
{
    if(name == NULL)
        return;
    uint8_t cell_pixel_x = cell_x * CELL_LONG_SIDE;
    uint8_t cell_pixel_y = cell_y * CELL_SHORT_SIDE + TITLE_BAR_HEIGHT + 1;
    
    uint8_t str_size = strlen(name);
    if(str_size == 0)
        return;
    if(str_size > 10)
        str_size = 10;

    memset(cell_temp_buf, 0, CELL_LINE_BUF_SIZE);
    strncpy(cell_temp_buf, name, str_size);
    str_size = strlen(cell_temp_buf);
    // printf("%d %s\n", str_size, cell_temp_buf);

    /*
    str_size
    1-4: one line of 7x10
    5: one line of 6x8
    6-8: two lines of 7x10
    9-10: two lines of 6x8

    maybe also add splitting lines based on space character?
    if keyname has space and both before and after fits on a line, then split at space
    otherwise split at 4 or 5
    */
    uint8_t str_pixel_x, str_pixel_y;
    memset(cell_first_line, 0, CELL_LINE_BUF_SIZE);
    memset(cell_second_line, 0, CELL_LINE_BUF_SIZE);
    if(str_size <= 4)
    {
        str_pixel_x = cell_pixel_x + (CELL_LONG_SIDE - (str_size * 7)) / 2;
        str_pixel_y = cell_pixel_y + 6;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_temp_buf, Font_7x10, White);
    }
    else if(str_size == 5)
    {
        str_pixel_x = cell_pixel_x + 2;
        str_pixel_y = cell_pixel_y + 7;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_temp_buf, Font_6x8, White);
    }
    else if(str_size >= 6 && str_size <= 8)
    {
        strncpy(cell_first_line, cell_temp_buf, 4);
        strncpy(cell_second_line, cell_temp_buf+4, 4);
        // first line
        str_pixel_x = cell_pixel_x + 2;
        str_pixel_y = cell_pixel_y + 2;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_first_line, Font_7x10, White);
        // second line
        str_pixel_x = cell_pixel_x + (CELL_LONG_SIDE - (strlen(cell_second_line) * 7)) / 2;
        str_pixel_y = cell_pixel_y + 2 + 10;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_second_line, Font_7x10, White);
    }
    else if(str_size >= 9)
    {
        strncpy(cell_first_line, cell_temp_buf, 5);
        strncpy(cell_second_line, cell_temp_buf+5, 5);
        // first line
        str_pixel_x = cell_pixel_x + 2;
        str_pixel_y = cell_pixel_y + 3;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_first_line, Font_6x8, White);
        // second line
        str_pixel_x = cell_pixel_x + 1 + (CELL_LONG_SIDE - (strlen(cell_second_line) * 6)) / 2;
        str_pixel_y = cell_pixel_y + 3 + 8;
        ssd1306_SetCursor(str_pixel_x, str_pixel_y);
        ssd1306_WriteString(cell_second_line, Font_6x8, White);
    }
}

void draw_profile(profile_info* this_profile)
{
  ssd1306_Fill(Black);
  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "P%d: %s", this_profile->pf_number, this_profile->pf_name);
	ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
	ssd1306_WriteString(oled_line_buf, Font_7x10, White);
	
  ssd1306_Line(0,10,127,10,White);
  // ssd1306_dashed_line(0,33,127,33,White);
  // ssd1306_dashed_line(0,57,127,57,White);
  // ssd1306_dashed_line(0,80,127,80,White);
  // ssd1306_dashed_line(0,103,127,103,White);
  // ssd1306_dashed_line(32,10,32,127,White);
  // ssd1306_dashed_line(64,10,64,127,White);
  // ssd1306_dashed_line(96,10,96,127,White);
  // ssd1306_dashed_line(0,127,127,127,White);

  for (size_t rrr = 0; rrr < SW_MATRIX_NUM_ROWS; rrr++)
  {
    for (size_t ccc = 0; ccc < SW_MATRIX_NUM_COLS; ccc++)
    {
      uint8_t this_sw_index = rowcol_to_index(rrr, ccc);
      memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
      sprintf(oled_line_buf, "-");
      if(strlen(this_profile->sw_name[this_sw_index]))
        sprintf(oled_line_buf, "%s", this_profile->sw_name[this_sw_index]);
      draw_cell_content(ccc, rrr, oled_line_buf);
    }
  }
  ssd1306_UpdateScreen();
}