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
  printf("start pixel: %d\n", start_pixel);
  return start_pixel;
}

void draw_bluetooth_icon(uint8_t origx, uint8_t origy)
{
  ssd1306_DrawPixel(origx+2, origy+0, White);
  ssd1306_DrawPixel(origx+2, origy+1, White);
  ssd1306_DrawPixel(origx+3, origy+1, White);
  ssd1306_DrawPixel(origx+2, origy+2, White);
  ssd1306_DrawPixel(origx+4, origy+2, White);
  ssd1306_DrawPixel(origx+0, origy+3, White);
  ssd1306_DrawPixel(origx+2, origy+3, White);
  ssd1306_DrawPixel(origx+5, origy+3, White);
  ssd1306_DrawPixel(origx+1, origy+4, White);
  ssd1306_DrawPixel(origx+2, origy+4, White);
  ssd1306_DrawPixel(origx+4, origy+4, White);
  ssd1306_DrawPixel(origx+2, origy+5, White);
  ssd1306_DrawPixel(origx+3, origy+5, White);
  ssd1306_DrawPixel(origx+1, origy+6, White);
  ssd1306_DrawPixel(origx+2, origy+6, White);
  ssd1306_DrawPixel(origx+4, origy+6, White);
  ssd1306_DrawPixel(origx+0, origy+7, White);
  ssd1306_DrawPixel(origx+2, origy+7, White);
  ssd1306_DrawPixel(origx+5, origy+7, White);
  ssd1306_DrawPixel(origx+2, origy+8, White);
  ssd1306_DrawPixel(origx+4, origy+8, White);
  ssd1306_DrawPixel(origx+2, origy+9, White);
  ssd1306_DrawPixel(origx+3, origy+9, White);
  ssd1306_DrawPixel(origx+2, origy+10, White);
  ssd1306_SetCursor(origx+7, origy+2);
  ssd1306_WriteString("?", Font_6x8, White);
}


int main(int argc, char *argv[])
{
  ssd1306_Fill(Black);
  // ----------------------------------------
  draw_bluetooth_icon(10,10);
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

