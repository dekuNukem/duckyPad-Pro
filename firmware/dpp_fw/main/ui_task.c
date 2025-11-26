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
#include "neopixel_task.h"
#include "bluetooth_task.h"
#include "keypress_task.h"
#include "ds_vm.h"
#include "hid_task.h"

static const char *TAG = "UI";
spi_device_handle_t my_spi_handle;
gpio_config_t oled_dc_cofig;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;

uint32_t sleep_after_ms_index_to_time_lookup[SLEEP_OPTION_SIZE] = {
  2*ONE_MINUTE_IN_MS,
  5*ONE_MINUTE_IN_MS,
  15*ONE_MINUTE_IN_MS,
  30*ONE_MINUTE_IN_MS,
  ONE_HOUR_IN_MS,
  2*ONE_HOUR_IN_MS,
  DONT_SLEEP,
  };

const char* sleep_index_to_string_lookup[SLEEP_OPTION_SIZE] = {
  "2 min",
  "5 min",
  "15 min",
  "30 min",
  "1 Hr",
  "2 Hrs",
  "Never",
};

const char* bt_mode_to_string_lookup[BT_MODE_SIZE] = {
  "Never",
  "Always",
  "Auto",
};

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
    delay_ms(25);
    gpio_set_level(OLED_RESET, 1);
    delay_ms(25);

    ssd1306_init();
}

uint8_t center_line(uint8_t line_len, uint8_t char_width_pixels, uint8_t oled_width_pixels)
{
  int16_t start_pixel = (oled_width_pixels - line_len * char_width_pixels) / 2;
  if(start_pixel < 0)
    start_pixel = 0;
  return start_pixel;
}

#define OLED_LINE_BUF_SIZE 64
char oled_line_buf[OLED_LINE_BUF_SIZE];

void draw_nosd(void)
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

void draw_noprofile(void)
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

#define CELL_LONG_SIDE 32
#define CELL_SHORT_SIDE 23
#define TITLE_BAR_HEIGHT 11

void draw_cell_content(uint8_t cell_col, uint8_t cell_row, char* first_line, char* second_line)
{
  if(cell_col >= SW_MATRIX_NUM_COLS || cell_row >= SW_MATRIX_NUM_ROWS)
    return;
  uint8_t cell_pixel_x = cell_col * CELL_LONG_SIDE;
  uint8_t cell_pixel_y = cell_row * CELL_SHORT_SIDE + TITLE_BAR_HEIGHT + 1;
  
  uint8_t first_line_size = strlen(first_line);
  uint8_t second_line_size = strlen(second_line);

  if(first_line_size == 0)
      return;

  uint8_t str_pixel_x, str_pixel_y;
  if(first_line_size <= 4 && second_line_size == 0)
  {
    str_pixel_x = cell_pixel_x + (CELL_LONG_SIDE - (first_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 6;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_7x10, White);
  }
  else if(first_line_size == 5 && second_line_size == 0)
  {
    str_pixel_x = cell_pixel_x + 2;
    str_pixel_y = cell_pixel_y + 7;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_6x8, White);
  }
  else if(first_line_size <= 4 && second_line_size <= 4)
  {
    // first line
    str_pixel_x = cell_pixel_x + (CELL_LONG_SIDE - (first_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 1;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_7x10, White);
    // second line
    str_pixel_x = cell_pixel_x + (CELL_LONG_SIDE - (second_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 1 + 10;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(second_line, Font_7x10, White);
  }
  else
  {
    // first line
    str_pixel_x = cell_pixel_x + 1 + (CELL_LONG_SIDE - (first_line_size * 6)) / 2;
    str_pixel_y = cell_pixel_y + 2;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_6x8, White);
    // second line
    str_pixel_x = cell_pixel_x + 1 + (CELL_LONG_SIDE - (second_line_size * 6)) / 2;
    str_pixel_y = cell_pixel_y + 3 + 8;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(second_line, Font_6x8, White);
  }
}

#define ROTATED_DISPLAY_ROWS 4
#define ROTATED_DISPLAY_COLS 5
#define ROTATED_CELL_WIDTH 25
#define ROTATED_CELL_HEIGHT 29

uint8_t cell_pixel_x_lookup_rotated_5cols[ROTATED_DISPLAY_COLS] =
{0, 26, 52, 77, 103};
uint8_t cell_pixel_y_lookup_rotated_4rows[ROTATED_DISPLAY_ROWS] =
{12, 41, 70, 99};

void draw_cell_content_rotated(uint8_t cell_col, uint8_t cell_row, char* first_line, char* second_line)
{
  if(cell_col >= ROTATED_DISPLAY_COLS || cell_row >= ROTATED_DISPLAY_ROWS)
    return;
  uint8_t cell_pixel_x = cell_pixel_x_lookup_rotated_5cols[cell_col];
  uint8_t cell_pixel_y = cell_pixel_y_lookup_rotated_4rows[cell_row];

  uint8_t first_line_size = strlen(first_line);
  uint8_t second_line_size = strlen(second_line);

  if(first_line_size == 0)
    return;
  // 4 char per line max
  first_line[4] = 0;
  second_line[4] = 0;

  uint8_t str_pixel_x, str_pixel_y;
  if(first_line_size <= 3 && second_line_size == 0)
  {
    str_pixel_x = cell_pixel_x + (ROTATED_CELL_WIDTH - (first_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 8;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_7x10, White);
  }
  else if(first_line_size > 3 && second_line_size == 0)
  {
    str_pixel_x = cell_pixel_x + 1;
    str_pixel_y = cell_pixel_y + 8;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_6x8, White);
  }
  else if(first_line_size <= 3 && second_line_size <= 3)
  {
    // first line
    str_pixel_x = cell_pixel_x + (ROTATED_CELL_WIDTH - (first_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 4;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_7x10, White);
    // second line
    str_pixel_x = cell_pixel_x + (ROTATED_CELL_WIDTH - (second_line_size * 7)) / 2;
    str_pixel_y = cell_pixel_y + 4 + 10;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(second_line, Font_7x10, White);
  }
  else
  {
    // first line
    str_pixel_x = cell_pixel_x + 1 + (ROTATED_CELL_WIDTH - (first_line_size * 6)) / 2;
    str_pixel_y = cell_pixel_y + 4;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(first_line, Font_6x8, White);
    // second line
    str_pixel_x = cell_pixel_x + 1 + (ROTATED_CELL_WIDTH - (second_line_size * 6)) / 2;
    str_pixel_y = cell_pixel_y + 4 + 10;
    ssd1306_SetCursor(str_pixel_x, str_pixel_y);
    ssd1306_WriteString(second_line, Font_6x8, White);
  }
}

void draw_profile_normal(profile_info* this_profile)
{
  ssd1306_Fill(Black);
  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "%s", this_profile->pf_name);
	ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
	ssd1306_WriteString(oled_line_buf, Font_7x10, White);
	
  ssd1306_Line(0,10,127,10,White); // title solid line

  for (size_t rrr = 0; rrr < SW_MATRIX_NUM_ROWS; rrr++)
  {
    for (size_t ccc = 0; ccc < SW_MATRIX_NUM_COLS; ccc++)
    {
      uint8_t this_sw_index = rowcol_to_index(rrr, ccc);
      draw_cell_content(ccc, rrr, this_profile->sw_name_firstline[this_sw_index], this_profile->sw_name_secondline[this_sw_index]);
    }
  }
  
  ssd1306_Line(0,33,127,33,White); // horizontal lines
  ssd1306_Line(0,57,127,57,White);
  ssd1306_Line(0,80,127,80,White);
  ssd1306_Line(0,103,127,103,White);
  ssd1306_Line(0,127,127,127,White);

  ssd1306_Line(32,10,32,127,White); // vertical lines
  ssd1306_Line(64,10,64,127,White);
  ssd1306_Line(96,10,96,127,White);

  draw_status_icon(0, 0, 0);
  draw_kbled_icon(kb_led_status, 0);

  ssd1306_UpdateScreen();
}

void draw_profile_rotated(profile_info* this_profile)
{
  ssd1306_Fill(Black);
  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "%s", this_profile->pf_name);
	ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
	ssd1306_WriteString(oled_line_buf, Font_7x10, White);
	
  ssd1306_Line(0,10,127,10,White); // title solid line

  // uint8_t cell_col, uint8_t cell_row, char* first_line, char* second_line
  draw_cell_content_rotated(0, 0, this_profile->sw_name_firstline[3], this_profile->sw_name_secondline[3]);
  draw_cell_content_rotated(1, 0, this_profile->sw_name_firstline[7], this_profile->sw_name_secondline[7]);
  draw_cell_content_rotated(2, 0, this_profile->sw_name_firstline[11], this_profile->sw_name_secondline[11]);
  draw_cell_content_rotated(3, 0, this_profile->sw_name_firstline[15], this_profile->sw_name_secondline[15]);
  draw_cell_content_rotated(4, 0, this_profile->sw_name_firstline[19], this_profile->sw_name_secondline[19]);

  draw_cell_content_rotated(0, 1, this_profile->sw_name_firstline[3-1], this_profile->sw_name_secondline[3-1]);
  draw_cell_content_rotated(1, 1, this_profile->sw_name_firstline[7-1], this_profile->sw_name_secondline[7-1]);
  draw_cell_content_rotated(2, 1, this_profile->sw_name_firstline[11-1], this_profile->sw_name_secondline[11-1]);
  draw_cell_content_rotated(3, 1, this_profile->sw_name_firstline[15-1], this_profile->sw_name_secondline[15-1]);
  draw_cell_content_rotated(4, 1, this_profile->sw_name_firstline[19-1], this_profile->sw_name_secondline[19-1]);

  draw_cell_content_rotated(0, 2, this_profile->sw_name_firstline[3-2], this_profile->sw_name_secondline[3-2]);
  draw_cell_content_rotated(1, 2, this_profile->sw_name_firstline[7-2], this_profile->sw_name_secondline[7-2]);
  draw_cell_content_rotated(2, 2, this_profile->sw_name_firstline[11-2], this_profile->sw_name_secondline[11-2]);
  draw_cell_content_rotated(3, 2, this_profile->sw_name_firstline[15-2], this_profile->sw_name_secondline[15-2]);
  draw_cell_content_rotated(4, 2, this_profile->sw_name_firstline[19-2], this_profile->sw_name_secondline[19-2]);

  draw_cell_content_rotated(0, 3, this_profile->sw_name_firstline[3-3], this_profile->sw_name_secondline[3-3]);
  draw_cell_content_rotated(1, 3, this_profile->sw_name_firstline[7-3], this_profile->sw_name_secondline[7-3]);
  draw_cell_content_rotated(2, 3, this_profile->sw_name_firstline[11-3], this_profile->sw_name_secondline[11-3]);
  draw_cell_content_rotated(3, 3, this_profile->sw_name_firstline[15-3], this_profile->sw_name_secondline[15-3]);
  draw_cell_content_rotated(4, 3, this_profile->sw_name_firstline[19-3], this_profile->sw_name_secondline[19-3]);

  ssd1306_Line(0,39,127,39,White); // horizontal lines
  ssd1306_Line(0,68,127,68,White);
  ssd1306_Line(0,97,127,97,White);
  ssd1306_Line(25,10,25,127,White); // vertical lines
  ssd1306_Line(51,10,51,127,White);
  ssd1306_Line(76,10,76,127,White);
  ssd1306_Line(102,10,102,127,White);

  draw_status_icon(0, 0, 0);
  draw_kbled_icon(kb_led_status, 0);

  ssd1306_UpdateScreen();
}

void draw_profile(profile_info* this_profile)
{
  if(this_profile->is_landscape)
  {
    ssd1306_set_rotation_only_for_128x128_do_not_use_for_anything_else(SSD1306_ROTATE_CW90);
    draw_profile_rotated(this_profile);
  }
  else
  {
    ssd1306_set_rotation_only_for_128x128_do_not_use_for_anything_else(SSD1306_NO_ROTATION);
    draw_profile_normal(this_profile);
  }
  // ssd1306_take_screenshot();
}

void draw_settings(dp_global_settings *dps)
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Settings");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_Line(0,10,128,10,White);

//------------------------------

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "1-Brightness %d%%", brightness_index_to_percent_lookup[dps->brightness_index]);
  ssd1306_SetCursor(0, 14);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

//------------------------------

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "2-Sleep: %s", sleep_index_to_string_lookup[dps->sleep_index]);
  ssd1306_SetCursor(0, 29);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

//------------------------------

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "3-Keyboard Region");
  ssd1306_SetCursor(0, 45);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  if(strlen(dps->current_kb_layout) >= 9)
  {
    strcpy(oled_line_buf, dps->current_kb_layout+5);
    oled_line_buf[strlen(oled_line_buf)-4] = 0; // don't print .txt extension
  }
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 57);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

//------------------------------

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "4-Use BT: %s", bt_mode_to_string_lookup[dps->bt_mode]);
  ssd1306_SetCursor(0, 73);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

//------------------------------

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "5-BT Unpair All");
  ssd1306_SetCursor(0, 88);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

//------------------------------
  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "6-Mount USB");
  ssd1306_SetCursor(0, 102);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);
//---------------

  ssd1306_Line(0,115,128,115,White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "FW V%d.%d.%d", fw_version_major, fw_version_minor, fw_version_patch);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 118);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
  // ssd1306_take_screenshot();
}

static const char* exe_error_string(uint8_t err_code)
{
  switch (err_code) {
    case EXE_UNKNOWN_OPCODE:          return "Unknown Opcode";
    case EXE_DSB_INCOMPATIBLE_VERSION:return "Unsupported VM";
    case EXE_DSB_FOPEN_FAIL:          return "File Open Fail";
    case EXE_DSB_FREAD_ERROR:         return "File Read Fail";
    case EXE_STACK_OVERFLOW:          return "Stack Overflow";
    case EXE_STACK_UNDERFLOW:         return "Stack Underflow";
    case EXE_DIVISION_BY_ZERO:        return "Division by Zero";
    default:                          return "Unknown";
  }
}


void draw_wrong_vmver()
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Incompatible");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Script File!");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 15);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_Line(0,28,128,28,White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Update to latest");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 37);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "FW & PC App");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 52);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Load & Save to");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 75);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Refresh Files");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 90);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 117);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}

void draw_exe_error(uint8_t err_code, uint16_t pc)
{
  if(err_code == EXE_DSB_INCOMPATIBLE_VERSION)
  {
    draw_wrong_vmver();
    return;
  }

  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Runtime Error");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_Line(0,10,128,10,White);

  const char *err_msg = exe_error_string(err_code);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "%s", err_msg);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 40);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "PC: 0x%X", pc);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 60);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 115);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}

void draw_msc_mode(void)
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "USB Storage Mode");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 40);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Long Press +/-");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 70);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "To Exit");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 85);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}

void get_clean_fw_name(char* fw_str, char* result_buf, uint8_t result_buf_size)
{
  const char* fw_prefix = "/sdcard/DPP_FW_";
  const char* suffix = "_";

  char* start = strstr(fw_str, fw_prefix);
  if (start == NULL)
    return;

  start += strlen(fw_prefix);

  char* end = strstr(start, suffix);
  if (end == NULL)
    return;

  memset(result_buf, 0, result_buf_size);
  strncpy(result_buf, start, end - start);
}

void draw_fw_update_ask(char* fw_path_str)
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Firmware Found");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_Line(0,10,128,10,White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Current FW:");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 20);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "%d.%d.%d", fw_version_major, fw_version_minor, fw_version_patch);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 32);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "New FW:");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 60);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  get_clean_fw_name(fw_path_str, oled_line_buf, OLED_LINE_BUF_SIZE);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 72);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 100);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "to Update");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 112);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);
  
  // ssd1306_take_screenshot();
  ssd1306_UpdateScreen();
}

void draw_fw_crc_error(char* fw_path_str)
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "FW File CRC ERROR:");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 20);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  get_clean_fw_name(fw_path_str, temp_buf, TEMP_BUFSIZE);
  sprintf(oled_line_buf, "FW_%s.bin", temp_buf);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 50);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 80);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "to Continue");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 95);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);
  
  ssd1306_UpdateScreen();
}

void oled_say(char* what)
{
  ssd1306_Fill(Black);

  ssd1306_SetCursor(center_line(strlen(what), 7, SSD1306_WIDTH), 60);
  ssd1306_WriteString(what, Font_7x10, White);

  ssd1306_UpdateScreen();
}

#define BT_ICON_HEIGHT (9)
#define BT_ICON_WIDTH (6)
const unsigned char bt_icon[] = {
    0x30, 0x28, 0xA4, 0x68, 0x30, 0x68, 0xA4, 0x28, 0x30,
};

#define RTC_ICON_HEIGHT (9)
#define RTC_ICON_WIDTH (9)
const unsigned char rtc_icon[] = {
    0x1C, 0x00, 0x6B, 0x00, 0x49, 0x00, 0x88, 0x80, 0x8E, 0x80, 0x80, 0x80,
    0x41, 0x00, 0x63, 0x00, 0x1C, 0x00,
};

void draw_bt_icon(uint8_t origx, uint8_t origy, uint8_t this_bt_stat, uint8_t update_screen)
{
  if(this_bt_stat == BT_DISABLED)
    return;
  ssd1306_DrawBitmap(origx, origy, bt_icon, BT_ICON_WIDTH, BT_ICON_HEIGHT, White);

  ssd1306_SetCursor(origx+7, origy+2);
  if(this_bt_stat == BT_DISCOVERABLE)
    ssd1306_WriteString("?", Font_6x8, White);
  else
    ssd1306_WriteString(" ", Font_6x8, White);

  if(update_screen)
    ssd1306_UpdateScreen();
}

void draw_rtc_icon(uint8_t origx, uint8_t origy, uint8_t update_screen)
{
  ssd1306_DrawBitmap(origx, origy, rtc_icon, RTC_ICON_WIDTH, RTC_ICON_HEIGHT, White);
  if(update_screen)
    ssd1306_UpdateScreen();
}

void draw_status_icon(uint8_t origx, uint8_t origy, uint8_t update_screen)
{
  if(is_rtc_valid)
    draw_rtc_icon(origx, origy, update_screen);
  else
    draw_bt_icon(origx, origy, bluetooth_status, update_screen);
}

uint8_t last_icon_stat = 255;
void update_status_icon()
{
  uint8_t this_icon_stat = bluetooth_status + is_rtc_valid * 4;
  if(this_icon_stat == last_icon_stat)
    return;
  draw_status_icon(0, 0, 1);
  last_icon_stat = this_icon_stat;
}

void draw_kbled_icon(uint8_t this_led_state, uint8_t update_screen)
{
  SSD1306_COLOR color;
  // numlock
  color = (this_led_state & 0x1) ? White : Black;
  ssd1306_FillCircle(118, 4, 1, color);
  // caps lock
  color = (this_led_state & 0x2) ? White : Black;
  ssd1306_FillCircle(122, 4, 1, color);
  // scroll lock
  color = (this_led_state & 0x4) ? White : Black;
  ssd1306_FillCircle(126, 4, 1, color);
  ssd1306_Line(117, 0, 119, 0, White);
  ssd1306_Line(121, 0, 123, 0, White);
  ssd1306_Line(125, 0, 127, 0, White);
  if(update_screen)
    ssd1306_UpdateScreen();
}

uint8_t last_led_state = 255;
void update_kbled_icon(uint8_t this_led_state)
{
  if(this_led_state == last_led_state)
    return;
  draw_kbled_icon(this_led_state, 1);
  last_led_state = this_led_state;
}

uint32_t last_bt_pin;
void draw_bt_pin(uint32_t this_bt_pin)
{
  if(this_bt_pin == last_bt_pin)
    return;

  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Bluetooth PIN:");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 20);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "%06ld", this_bt_pin);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 11, SSD1306_WIDTH), 50);
  ssd1306_WriteString(oled_line_buf, Font_11x18, White);

  CLEAR_TEMP_BUF();
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 100);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
  last_bt_pin = this_bt_pin;

  for (size_t i = 0; i < 5; i++)
  {
    neopixel_off();
    delay_ms(200);
    neopixel_fill(0, 0, 255);
    delay_ms(200);
  }
  block_until_anykey(SW_EVENT_SHORT_PRESS);
  goto_profile(current_profile_number);
}

void draw_nvm_erase()
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "All BT pairings");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 15);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "have been erased");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 27);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Remember to unpair");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 55);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "on PC side too");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 67);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  CLEAR_TEMP_BUF();
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 100);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}

void draw_no_usb_activity()
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "No USB Activity!");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 50);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Using Bluetooth");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 65);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}
