/*
 * 006spi_tx_testing.c
 *
 *  Created on: Jul 25, 2026
 *      Author: sande
 */

#include "stm32f103xx.h"
#include "string.h"
#include "stdio.h"

extern void initialise_monitor_handles();

//command code
#define COMMAND_LED_CTRL               0x50
#define COMMAND_SENSOR_READ            0x51
#define COMMAND_LED_READ               0x52
#define COMMAND_PRINT                  0x53
#define COMMAND_ID_READ                0x54

#define LED_ON                         1
#define LED_OFF                        0


//arduino analog pins
#define ANALOG_PIN0                    0
#define ANALOG_PIN1                    1
#define ANALOG_PIN2                    2
#define ANALOG_PIN3                    3
#define ANALOG_PIN4                    4

//arduino led
#define LED_PIN                        9

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
	GPIO_Handle_t GpioBtn,GpioLed;

	//this is button gpio configuration
	GpioBtn.pGPIOx = GPIOC;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;

	GPIO_Init(&GpioBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
	GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;

	GPIO_Init(&GpioLed);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		// ack
		return 1;
	}
	return 0;
}

int main(void)
{
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
	//char user_data[] = "Hello World";

	initialise_monitor_handles();

	printf("Application is running\n");

	GPIO_ButtonInit();

	// This function is used to initialize the GPIO pins to behave as SPI1 Pins
	SPI1_GPIOInits();

	// This function is used to initialize the SPI1 peripheral parameters
	SPI1_Inits();

	printf("SPI Initialization done\n");

    /*
     * making SSOE 1 does NSS output enable
     * The NSS pin is automatically managed by the hardware
     * i.e when SPE=1, NSS will be pulled to low
     * and NSS pin will be high when SPE=0.
     */
	SPI_SSOEConfig(SPI1 , ENABLE);

	while(1)
	{
		// wait till button is pressed
		while(!(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)));

		// to avoid button de-bouncing related issues 200ms of delay
		//delay();

		//enable the SPI1 peripheral
		SPI_PeriClockControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL  <pin no(1)>    <value(1)>

		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		//send command
		SPI_SendData(SPI1,&commandcode,1);

		// do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI1, &dummy_read, 1);

		//insert some delay so that slave can ready with the data
		delay();

	    //send some dummy bits (1 byte) to fetch the response from the slave.
		SPI_SendData(SPI1, &dummy_write, 1);

		//read the ack byte received
		SPI_ReceiveData(SPI1,&ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			args[1] = LED_ON;

			//send arguments
			SPI_SendData(SPI1, args, 2);
			printf("COMMAND_LED_CTRL Executed.\n");

		}
		//end of COMMAND_LED_CTRL

		//2. CMD_SENSOR_READ    <analog pin number(1)>

		// wait till button is pressed
		while(!(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)));

		// to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_SENSOR_READ;

		//send command
		SPI_SendData(SPI1,&commandcode,1);

		// do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI1, &dummy_read, 1);

		//insert some delay so that slave can ready with the data
		delay();

	    //send some dummy bits (1 byte) to fetch the response from the slave.
		SPI_SendData(SPI1, &dummy_write, 1);

	    //read the ack byte received
		SPI_ReceiveData(SPI1,&ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte))
		{
			args[0] = ANALOG_PIN0;

			//send arguments
			SPI_SendData(SPI1, args, 1);

			// do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI1, &dummy_read, 1);

			//insert some delay so that slave can ready with the data
			delay();

			//send some dummy bits (1 byte) to fetch the response from the slave.
			SPI_SendData(SPI1, &dummy_write, 1);

			uint8_t analog_read;
			SPI_ReceiveData(SPI1, &analog_read, 1);

			printf("CMD_SENSOR_READ %d\n", analog_read);
		}
		//end of CMD_SENSOR_READ

		//3.  CMD_LED_READ 	 <pin no(1) >

		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_LED_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

	   //do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;

			//send arguments
			SPI_SendData(SPI2,args,1); //sending one byte of

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2,&dummy_read,1);

			//insert some delay so that slave can ready with the data
			delay();

			//Send some dummy bits (1 byte) fetch the response from the slave
			SPI_SendData(SPI2,&dummy_write,1);

			uint8_t led_status;
			SPI_ReceiveData(SPI2,&led_status,1);
			printf("COMMAND_READ_LED %d\n",led_status);

		}

		//4. CMD_PRINT 		<len(2)>  <message(len) >

		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_PRINT;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t message[] = "Hello ! How are you ??";
		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = strlen((char*)message);

			//send arguments
			SPI_SendData(SPI2,args,1); //sending length

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2,&dummy_read,1);

			delay();

			//send message
			for(int i = 0 ; i < args[0] ; i++){
				SPI_SendData(SPI2,&message[i],1);
				SPI_ReceiveData(SPI2,&dummy_read,1);
			}

			printf("COMMAND_PRINT Executed \n");

		}

		//5. CMD_ID_READ
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_ID_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t id[11];
		uint32_t i=0;
		if( SPI_VerifyResponse(ackbyte))
		{
			//read 10 bytes id from the slave
			for(  i = 0 ; i < 10 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_SendData(SPI2,&dummy_write,1);
				SPI_ReceiveData(SPI2,&id[i],1);
			}

			id[10] = '\0';

			printf("COMMAND_ID : %s \n",id);

		}


		// confirming SPI is not busy
		while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

		//disable the SPI1 peripheral
		SPI_PeriClockControl(SPI2, DISABLE);

		printf("SPI Communication Closed\n");

	}

	return 0;
}
