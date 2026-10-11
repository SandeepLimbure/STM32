/*
 * 001ledblink.c
 *
 *  Created on: Jul 20, 2026
 *      Author: LIMBUSM
 */


#include "stm32f103xx.h"

void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_PP;
	//GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PULL_UP;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA, 5);
		delay();
	}
	return 0;
}
