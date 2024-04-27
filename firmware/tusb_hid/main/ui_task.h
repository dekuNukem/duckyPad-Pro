#ifndef __OLED_TASK_H
#define __OLED_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define OLED_MOSI 9
#define OLED_CS 12
#define OLED_DC 11
#define OLED_SCK 8

void oled_init(void);

void write_test(void);


#ifdef __cplusplus
}
#endif

#endif


