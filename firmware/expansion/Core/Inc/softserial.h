#ifndef __SOFTSERIAL_H
#define __SOFTSERIAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx_hal.h"

void softserial_init(GPIO_TypeDef* port, uint16_t pin);
void softserial_putc(char c);

#ifdef __cplusplus
}
#endif

#endif


