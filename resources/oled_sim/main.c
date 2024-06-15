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

uint8_t center_line(uint8_t line_len, uint8_t char_width_pixels, uint8_t oled_width_pixels)
{
  int16_t start_pixel = (oled_width_pixels - line_len * char_width_pixels) / 2;
  if(start_pixel < 0)
    start_pixel = 0;
  return start_pixel;
}

uint8_t fw_version_major;
uint8_t fw_version_minor;
uint8_t fw_version_patch;

int main(int argc, char *argv[])
{

    //--------------------------------


	ssd1306_Fill(Black);

    ssd1306_SetCursor(0,0);
    ssd1306_WriteString("abcdefghijklmn", Font_6x8, White);
    ssd1306_SetCursor(0,10);
    ssd1306_WriteString("opqrstuvwxyz", Font_6x8, White);

    ssd1306_SetCursor(0,20);
    ssd1306_WriteString("1234567890", Font_6x8, White);

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

