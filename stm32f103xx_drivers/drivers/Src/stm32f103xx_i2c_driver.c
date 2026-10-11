/*
 * stm32f103xx_i2c_driver.c
 *
 *  Created on: Aug 6, 2026
 *      Author: sande
 */

#include "stm32f103xx_i2c_driver.h"



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

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if(pSPIx == I2C2)
		{
			I2C2_PCLK_EN();
		}
	}else
	{
		if(pSPIx == I2C1)
		{
			I2C1_PCLK_DI();
		}else if(pSPIx == I2C2)
		{
			I2C2_PCLK_DI();
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

void I2C_Init(I2C_Handle_t *pI2CHandle)
{


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

void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{

}
