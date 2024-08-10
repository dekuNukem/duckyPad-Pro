#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/*
2024 03 20
Adjust screen size in ssd1306.h
make clean; make; ./my_program.exe ; python ./vis.py ./OLED_128_128.bin
*/

#define FILENAME_BUF_SIZE 64
char filename_buf[FILENAME_BUF_SIZE];
#define OLED_LINE_BUF_SIZE 64
char oled_line_buf[OLED_LINE_BUF_SIZE];
#define TEMP_BUFSIZE 64
char temp_buf[TEMP_BUFSIZE];

uint8_t center_line(uint8_t line_len, uint8_t char_width_pixels, uint8_t oled_width_pixels)
{
  int16_t start_pixel = (oled_width_pixels - line_len * char_width_pixels) / 2;
  if(start_pixel < 0)
    start_pixel = 0;
  return start_pixel;
}

int main(int argc, char *argv[])
{

  // ----------------------------------------
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "All BT Pairing");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 15);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "has been forgotten");
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

  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 100);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  // ------------------------------

	sprintf(filename_buf, "OLED_%d_%d.bin", SSD1306_WIDTH, SSD1306_HEIGHT);

	FILE* file = fopen(filename_buf, "wb");
  if (file == NULL) {
      printf("Failed to open file\n");
      return 1;
  }

  size_t written = fwrite(SSD1306_Buffer, sizeof(uint8_t), SSD1306_BUFFER_SIZE, file);
  if (written < SSD1306_BUFFER_SIZE)
  {
      printf("Failed to write the complete buffer to file\n");
      return 2;
  }

  fclose(file);
  printf("done!\n");
}

