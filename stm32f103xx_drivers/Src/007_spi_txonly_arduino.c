/*
 * 006spi_tx_testing.c
 *
 *  Created on: Jul 25, 2026
 *      Author: sande
 */

#include "stm32f103xx.h"
#include "string.h"

/*
 *   AF          SPI1_REMAP = 0        SPI1_REMAP = 1
 * SPI1_NSS  -->     PA4                   PA15
 * SPI1_SCK  -->     PA5                   PB3
 * SPI1_MISO -->     PA6                   PB4
 * SPI1_MOSI -->     PA7                   PB5
 */

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

void SPI1_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOA;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinOutputType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed  = GPIO_SPEED_50MHZ;


	// NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
	GPIO_Init(&SPIPins);

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&SPIPins);

	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&SPIPins);

}

void SPI1_Inits(void)
{
	SPI_Handle_t SPI1handle;

	SPI1handle.pSPIx = SPI1;
	SPI1handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI1handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // generates SCLK of 8Mhz
	SPI1handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI1handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI1handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI1handle.SPIConfig.SPI_SSM  = SPI_SSM_DI;  // Hardware slave management enable for NSS pin

	SPI_Init(&SPI1handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBtn;

	GpioBtn.pGPIOx = GPIOC;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;

	GPIO_Init(&GpioBtn);
}

int main(void)
{
	char user_data[] = "Hello World";

	GPIO_ButtonInit();

	// This function is used to initialize the GPIO pins to behave as SPI1 Pins
	SPI1_GPIOInits();

	// This function is used to initialize the SPI1 peripheral parameters
	SPI1_Inits();

    /*
     * making SSOE 1 does NSS output enable
     * The NSS pin is automatically managed by the hardware
     * i.e when SPE=1, NSS will be pulled to low
     * and NSS pin will be high when SPE=0.
     */
	SPI_SSOEConfig(SPI1 , ENABLE);

	while(1)
	{
		while(!(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)));

		delay();

		//enable the SPI1 peripheral
		SPI_PeriClockControl(SPI2, ENABLE);

		//first send length information
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI1, &dataLen , 1);

		//to send data
		SPI_SendData(SPI1, (uint8_t*)user_data, strlen(user_data));

		// confirming SPI is not busy
		while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

		//disable the SPI1 peripheral
		SPI_PeriClockControl(SPI2, DISABLE);

	}

	return 0;
}
