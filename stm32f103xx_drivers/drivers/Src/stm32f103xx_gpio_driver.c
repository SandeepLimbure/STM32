/*
 * stm32f103xx_gpio_driver.c
 *
 *  Created on: Jul 8, 2026
 *      Author: LIMBUSM
 */

#include "stm32f103xx_gpio_driver.h"


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
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
	}else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
	}

}

/*
 * Init and De-Init
 */

/*****************************************************************
 * @fn                  -  GPIO_Init
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
/* ==================================================================================
 * @fn              - GPIO_Init
 * @brief           - Fully configures CRL/CRH, ODR (Pull-up/down), AFIO, and EXTI
 *                    covering 100% of STM32F103 GPIO pin modes.
 * ================================================================================== */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint8_t pinNumber = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
    uint8_t bitOffset = (pinNumber % 8) * 4;
    uint32_t cnf_mode_val = 0;

    //enable the peripheral clock

    GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

    // ------------------------------------------------------------------
    // 1. NON-INTERRUPT MODES (Input, Output, Alternate Function)
    // ------------------------------------------------------------------
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ALTFN)
    {
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IN)
        {
            // MODE bits = 00
            if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_ANALOG_MODE)
            {
                cnf_mode_val = (0x0 << 2) | 0x0; // CNF = 00, MODE = 00
            }
            else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_NO_PULL)
            {
                cnf_mode_val = (0x1 << 2) | 0x0; // CNF = 01 (Floating), MODE = 00
            }
            else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PULL_UP)
            {
                cnf_mode_val = (0x2 << 2) | 0x0; // CNF = 10 (PUPD), MODE = 00
                pGPIOHandle->pGPIOx->ODR |= (1 << pinNumber); // Write 1 to ODR for Pull-Up
            }
            else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PULL_DOWN)
            {
                cnf_mode_val = (0x2 << 2) | 0x0; // CNF = 10 (PUPD), MODE = 00
                pGPIOHandle->pGPIOx->ODR &= ~(1 << pinNumber); // Write 0 to ODR for Pull-Down
            }
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_OUT)
        {
            // MODE bits = GPIO_PinSpeed (10MHz=1, 2MHz=2, 50MHz=3)
            uint8_t cnfBits = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOutputType == GPIO_OP_TYPE_OD) ? 0x1 : 0x0;
            cnf_mode_val = (cnfBits << 2) | (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed & 0x3);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
        {
            // MODE bits = GPIO_PinSpeed
            uint8_t cnfBits = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOutputType == GPIO_OP_TYPE_OD) ? 0x3 : 0x2;
            cnf_mode_val = (cnfBits << 2) | (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed & 0x3);
        }

        // Apply configuration to CRL (Pins 0-7) or CRH (Pins 8-15)
        if (pinNumber < 8)
        {
            pGPIOHandle->pGPIOx->CRL &= ~(0xF << bitOffset);
            pGPIOHandle->pGPIOx->CRL |=  (cnf_mode_val << bitOffset);
        }
        else
        {
            pGPIOHandle->pGPIOx->CRH &= ~(0xF << bitOffset);
            pGPIOHandle->pGPIOx->CRH |=  (cnf_mode_val << bitOffset);
        }
    }
    // ------------------------------------------------------------------
    // 2. INTERRUPT MODES (GPIO_MODE_IT_FT, GPIO_MODE_IT_RT, GPIO_MODE_IT_RFT)
    // ------------------------------------------------------------------
    else
    {
        // A. Set pin as Input in CRL/CRH
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PULL_UP)
        {
            cnf_mode_val = (0x2 << 2) | 0x0;
            pGPIOHandle->pGPIOx->ODR |= (1 << pinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PULL_DOWN)
        {
            cnf_mode_val = (0x2 << 2) | 0x0;
            pGPIOHandle->pGPIOx->ODR &= ~(1 << pinNumber);
        }
        else
        {
            cnf_mode_val = (0x1 << 2) | 0x0; // Default: Floating Input
        }

        if (pinNumber < 8)
        {
            pGPIOHandle->pGPIOx->CRL &= ~(0xF << bitOffset);
            pGPIOHandle->pGPIOx->CRL |=  (cnf_mode_val << bitOffset);
        }
        else
        {
            pGPIOHandle->pGPIOx->CRH &= ~(0xF << bitOffset);
            pGPIOHandle->pGPIOx->CRH |=  (cnf_mode_val << bitOffset);
        }

        // B. Enable AFIO Clock
        AFIO_PCLK_EN();

        // C. Route GPIO Port to EXTI Line in AFIO->EXTICR
        uint8_t exticr_index = pinNumber / 4;
        uint8_t exticr_shift = (pinNumber % 4) * 4;

        uint8_t portCode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

        AFIO->EXTICR[exticr_index] &= ~(0xF << exticr_shift);
        AFIO->EXTICR[exticr_index] |=  (portCode << exticr_shift);

        // D. Configure Edge Triggers in EXTI
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            EXTI->FTSR |=  (1 << pinNumber);
            EXTI->RTSR &= ~(1 << pinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            EXTI->RTSR |=  (1 << pinNumber);
            EXTI->FTSR &= ~(1 << pinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            EXTI->FTSR |= (1 << pinNumber);
            EXTI->RTSR |= (1 << pinNumber);
        }

        // E. Enable the EXTI Interrupt Line (Unmask IMR)
        EXTI->IMR |= (1 << pinNumber);
    }
}
/*****************************************************************
 * @fn                  -  GPIO_DeInit
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
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if (pGPIOx == GPIOA)      { GPIOA_REG_RESET(); }
    else if (pGPIOx == GPIOB) { GPIOB_REG_RESET(); }
    else if (pGPIOx == GPIOC) { GPIOC_REG_RESET(); }
    else if (pGPIOx == GPIOD) { GPIOD_REG_RESET(); }
    else if (pGPIOx == GPIOE) { GPIOE_REG_RESET(); }
}

/*
 * Data Read and Write
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
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t value;
    value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
    return value;
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
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value;
    value = (uint16_t)pGPIOx->IDR;
    return value;
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
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
    if (Value == GPIO_PIN_SET)
    {
        // Write to lower 16 bits of BSRR to set pin HIGH
        pGPIOx->BSRR = (1 << PinNumber);
    }
    else
    {
        // Write to BRR or upper 16 bits of BSRR to set pin LOW
        pGPIOx->BRR = (1 << PinNumber);
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
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
    pGPIOx->ODR = Value;
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
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1 << PinNumber);
}

/*
 * IRQ Configuration and Handling
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
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t  EnorDi)
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
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//1.first find out the ipr register
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
void GPIO_IRQHandling(uint8_t PinNumber)
{
	// clear the exti pr register corresponding to the pin number
	if(EXTI->PR & ( 1 << PinNumber))
	{
		// clear
		EXTI->PR |= ( 1 << PinNumber);
	}

}
