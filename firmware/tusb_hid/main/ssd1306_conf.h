/**
 * Private configuration file for the SSD1306 library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

// Mirror the screen if needed
#define SSD1306_MIRROR_VERT
#define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
#define SSD1306_INVERSE_COLOR

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
// #define SSD1306_INCLUDE_FONT_11x18
// #define SSD1306_INCLUDE_FONT_16x26
// #define SSD1306_INCLUDE_FONT_16x24

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 128

#endif /* __SSD1306_CONF_H__ */
