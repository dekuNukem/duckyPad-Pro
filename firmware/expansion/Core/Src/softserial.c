#include "softserial.h"
#include "delay_us.h"

#define BIT_DELAY_9600 104
#define BIT_DELAY_38400 26
#define BIT_DELAY_57600 16

#define SOFT_UART_SPEED BIT_DELAY_57600

GPIO_TypeDef* ss_port;
uint16_t ss_pin;

void softserial_init(GPIO_TypeDef* port, uint16_t pin)
{
	ss_port = port;
	ss_pin = pin;
	HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_SET);
}

void softserial_putc(char c)
{
	HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_RESET);
	delay_us(SOFT_UART_SPEED);
	for (int i = 0; i < 8; ++i)
	{
		if((c >> i) & 0x1)
			HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_RESET);
		delay_us(SOFT_UART_SPEED);
	}
	HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_SET);
	delay_us(SOFT_UART_SPEED);
}
