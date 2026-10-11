/*
 * stm32f103xx_spi_driver.c
 *
 *  Created on: Jul 22, 2026
 *      Author: sande
 */


#include "stm32f103xx_spi_driver.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);



/*
 * Peripheral Clock Setup
 */

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_EN();
			}else if(pSPIx == SPI2)
			{
				SPI2_PCLK_EN();
			}else if(pSPIx == SPI3)
			{
				SPI3_PCLK_EN();
			}
		}else
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}else if(pSPIx == SPI2)
			{
				SPI2_PCLK_DI();
			}else if(pSPIx == SPI3)
			{
				SPI3_PCLK_DI();
			}
		}
}



/*
 * Init and De-Init
 */

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	// peripheral clock enable

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE );


	// first configure the SPI_CR1register
    uint32_t tempreg = 0;

    //1. configure the device mode
    tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

    //2. configure the bus config
    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
    	//BIDI mode should be cleared
    	tempreg &= ~(1 << SPI_CR1_BIDIMODE);

    }else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
    	//BIDI mode should be set
    	tempreg |= (1 << SPI_CR1_BIDIMODE);

    }else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
    	//BIDI mode should be cleared
    	tempreg &= ~(1 << SPI_CR1_BIDIMODE);
    	//RXONLY bit must be set
    	tempreg |= (1 << SPI_CR1_RXONLY);
    }

    //3. Configure the SPI serial clock speed(baud rate)
    tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

    //4. configure the DFF
    tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

    //5. configure the CPOL
    tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

    //6. configure the CPOL
    tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

    pSPIHandle->pSPIx->CR1 = tempreg;
}


/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1)      { SPI1_REG_RESET(); }
	else if (pSPIx == SPI2) { SPI2_REG_RESET(); }
	else if (pSPIx == SPI3) { SPI3_REG_RESET(); }
}


/*
 * Data send and Receive
 */

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  This is a blocking call(polling based)
 *
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->CR1 & ( 1 << SPI_CR1_DFF)))
		{
			//16-bit DFF
			//1. load the data into DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			//8-bit DFF
			//1.load the data into DR
			pSPIx->DR = *(pTxBuffer);
			Len--;
			pTxBuffer++;
		}
	}


}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

			//2. check the DFF bit in CR1
			if((pSPIx->CR1 & ( 1 << SPI_CR1_DFF)))
			{
				//16-bit DFF
				//1. load the data from DR to Rxbuffer address.
				*((uint16_t*)pRxBuffer) = pSPIx->DR;
				Len--;
				Len--;
				(uint16_t*)pRxBuffer++;
			}else
			{
				//8-bit DFF
				//1.load the data from DR to Rxbuffer address.
				*(pRxBuffer) = pSPIx->DR;
				Len--;
				pRxBuffer++;
			}
		}
}



/*
 * IRQ Configuration and ISR Handling
 */

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t  EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			// program ISER0 Register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// program ISER1 register
			*NVIC_ISER1 |= ( 1 << IRQNumber % 32 );

		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// program ISER2 register
			*NVIC_ISER2 |= ( 1 << IRQNumber % 64 );

		}
	}else
	{
		if(IRQNumber <= 31)
		{
			// program ICER0 Register
			*NVIC_ICER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// program ICER1 register
			*NVIC_ICER1 |= ( 1 << IRQNumber % 32 );

		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// program ICER2 register
			*NVIC_ICER2 |= ( 1 << IRQNumber % 64 );

		}
	}

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount  = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + (iprx * 4)) |= (IRQPriority << shift_amount );

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1 , temp2;
	// Lets check for TXE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_TXE );
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE );

	if(temp1 && temp2)
	{
		// Handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// check for RXNE
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_RXNE );
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE );

	if(temp1 && temp2)
	{
		// Handle TXE
		spi_rxne_interrupt_handle(pHandle);
	}

	// check for overrun flag
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_OVR );
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE );

	if(temp1 && temp2)
	{
		// Handle TXE
		spi_ovr_err_interrupt_handle(pHandle);
	}

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//2. check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF)))
	{
		//16-bit DFF
		//1. load the data into DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8-bit DFF
		//1.load the data into DR
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		// TxLen is Zero , so close the SPI transmission and inform the application
		// that TX is over

		// this prevents interrupts from setting up of TXE flag
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);

	}

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if((pSPIHandle->pSPIx->CR1 & ( 1 << 11)))
		{
			//16-bit DFF
			//1. load the data into DR
		    *((uint16_t*)pSPIHandle->pTxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->RxLen -= 2;
			pSPIHandle->pRxBuffer--;
			pSPIHandle->pRxBuffer--;
		}else
		{
			//8-bit DFF
			//1.load the data into DR
			*((uint16_t*)pSPIHandle->pTxBuffer) = pSPIHandle->pSPIx->DR;
			pSPIHandle->RxLen--;
			pSPIHandle->pRxBuffer--;
		}

		if(! pSPIHandle->RxLen)
		{
			// Reception is complete
			//lets turn off the rxneie interrupt

			SPI_CloseReception(pSPIHandle);
			SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);

		}

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	//1.clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	//2.inform the application
	SPI_ApplicationEvenetCallback(pSPIHandle, SPI_EVENT_OVR_ERR);

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1. Save the Tx Buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen  = Len;

		//2. Mark the SPI state as busy in transmission so that
		//no other code can take over same SPI peripheral until transmission is over.
		pSPIHandle->TxState  = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrput whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2  |= (1<<SPI_CR2_TXEIE);

	}

	//4. Data Transmission will be handled by the ISR code

	return state;

}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save the Tx Buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen  = Len;

		//2. Mark the SPI state as busy in transmission so that
		//no other code can take over same SPI peripheral until transmission is over.
		pSPIHandle->RxState  = SPI_BUSY_IN_RX;

		//3. Enable the TXEIE control bit to get interrput whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2  |= (1<<SPI_CR2_RXNEIE);

	}

	//4. Data Transmission will be handled by the ISR code

	return state;
}


/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}


/*****************************************************************
 * @fn                  -  GPIO_PeriClockControl
 *
 * @brief               -  This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]           -  base address of the GPIO peripheral
 * @param[in]           -  ENABLE or DISABLE macros
 * @param[in]           -
 *
 * @return              -  none
 *
 * @Note                -  none
 *
 */

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR2_SSOE);
	}
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
    (void)temp;

}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer  = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer  = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;

}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{

}
