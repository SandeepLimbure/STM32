/*
 * led_button.c
 *
 *  Created on: Jul 20, 2026
 *      Author: LIMBUSM
 */


/*
 * 001ledblink.c
 *
 *  Created on: Jul 20, 2026
 *      Author: LIMBUSM
 */


#include "stm32f103xx.h"

#define LOW           0
#define BTN_PRESSED   LOW

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_PP;
	//GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PULL_UP;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);

	GpioBtn.pGPIOx = GPIOC;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;

	GPIO_PeriClockControl(GPIOC, ENABLE);

	GPIO_Init(&GpioBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, 5);
		}
	}
	return 0;
}
