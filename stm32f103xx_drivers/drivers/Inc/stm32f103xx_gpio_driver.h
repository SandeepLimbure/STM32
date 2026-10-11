/*
 * stm32f103xx_gpio_driver.h
 *
 *  Created on: Jul 8, 2026
 *      Author: LIMBUSM
 */

#ifndef INC_STM32F103XX_GPIO_DRIVER_H_
#define INC_STM32F103XX_GPIO_DRIVER_H_

#include "stm32f103xx.h"

/* ==================================================================================
 * 1. GPIO Pin Numbers
 * ================================================================================== */
#define GPIO_PIN_NO_0                   0
#define GPIO_PIN_NO_1                   1
#define GPIO_PIN_NO_2                   2
#define GPIO_PIN_NO_3                   3
#define GPIO_PIN_NO_4                   4
#define GPIO_PIN_NO_5                   5
#define GPIO_PIN_NO_6                   6
#define GPIO_PIN_NO_7                   7
#define GPIO_PIN_NO_8                   8
#define GPIO_PIN_NO_9                   9
#define GPIO_PIN_NO_10                  10
#define GPIO_PIN_NO_11                  11
#define GPIO_PIN_NO_12                  12
#define GPIO_PIN_NO_13                  13
#define GPIO_PIN_NO_14                  14
#define GPIO_PIN_NO_15                  15

/* ==================================================================================
 * 2. GPIO Pin Modes (MODE[1:0] bits in CRL/CRH + EXTI Modes)
 * ================================================================================== */
#define GPIO_MODE_IN                    0   /* Input mode (reset state) */
#define GPIO_MODE_OUT                   1   /* Output mode */
#define GPIO_MODE_ALTFN                 2   /* Alternate Function mode */
#define GPIO_MODE_IT_FT                 3   /* Interrupt Falling Edge Trigger */
#define GPIO_MODE_IT_RT                 4   /* Interrupt Rising Edge Trigger */
#define GPIO_MODE_IT_RFT                5   /* Interrupt Rising & Falling Edge Trigger */

/* ==================================================================================
 * 3. GPIO Pin Output Speeds (MODE[1:0] bits when MODE > 0)
 * ================================================================================== */
#define GPIO_SPEED_10MHZ                1   /* Output mode, max speed 10 MHz (01b) */
#define GPIO_SPEED_2MHZ                 2   /* Output mode, max speed 2 MHz  (10b) */
#define GPIO_SPEED_50MHZ                3   /* Output mode, max speed 50 MHz (11b) */

/* ==================================================================================
 * 4. GPIO Pin Output Types (CNF[1:0] bits when Output/AF)
 * ================================================================================== */
#define GPIO_OP_TYPE_PP                 0   /* Push-Pull */
#define GPIO_OP_TYPE_OD                 1   /* Open-Drain */

/* ==================================================================================
 * 5. GPIO Pin Pull-Up / Pull-Down Control (CNF[1:0] bits when Input)
 * ================================================================================== */
#define GPIO_NO_PULL                    0   /* Floating Input / Analog */
#define GPIO_PIN_PULL_UP                1   /* Internal Pull-Up */
#define GPIO_PIN_PULL_DOWN              2   /* Internal Pull-Down */
#define GPIO_ANALOG_MODE                3   /* Analog Mode */

typedef struct
{
    uint8_t GPIO_PinNumber;      /* GPIO_PIN_NO_0 to GPIO_PIN_NO_15 */
    uint8_t GPIO_PinMode;        /* INPUT, OUTPUT, ALT_FN, IT_FT, IT_RT, IT_RFT */
    uint8_t GPIO_PinSpeed;       /* SPEED_2MHZ, SPEED_10MHZ, SPEED_50MHZ */
    uint8_t GPIO_PinOutputType;  /* PUSH_PULL, OPEN_DRAIN */
    uint8_t GPIO_PinPuPdControl; /* NO_PULL, PULL_UP, PULL_DOWN */
} GPIO_PinConfig_t;

/*
 * This is a handle structure for GPIO pin
 */

typedef struct
{
	GPIO_RegDef_t *pGPIOx;  // This holds the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;

}GPIO_Handle_t;

/************************************************************************************
 *                        APIs supported by this driver
 ************************************************************************************/

/*
 * Peripheral Clock Setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Init and De-Init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and Handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t  EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_STM32F103XX_GPIO_DRIVER_H_ */
