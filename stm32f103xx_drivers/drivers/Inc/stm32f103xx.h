/*
 * stm32f103xx.h
 *
 *  Created on: Jul 7, 2026
 *      Author: LIMBUSM
 */

#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_

#include "stdint.h"
#include "stddef.h"

#define  __vo volatile
#define  __weak __attribute__((weak))

/************************START:Processor Specific Details*****************************/
/*
 *  ARM Cortex Mx Processor NVIC ISERx register Address
 */

#define NVIC_ISER0                  ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1                  ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2                  ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3                  ((__vo uint32_t*)0xE000E10C)


/*
 *  ARM Cortex Mx Processor NVIC ICERx register Address
 */

#define NVIC_ICER0                  ((__vo uint32_t*)0xE000E100)
#define NVIC_ICER1                  ((__vo uint32_t*)0xE000E104)
#define NVIC_ICER2                  ((__vo uint32_t*)0xE000E108)
#define NVIC_ICER3                  ((__vo uint32_t*)0xE000E10C)


/*
 *  ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR           ((__vo uint32_t*)0xE000E400)


#define NO_PR_BITS_IMPLEMENTED      4


/*
 * base address of flash and SRAM memories
 */

#define FLASH_BASEADDR              0x08000000U
#define SRAM1_BASEADDR              0x20000000U
#define ROM_BASEADDR                0x1FFFF000U
#define SRAM                        SRAM1_BASEADDR

/*
 * AHB and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASE                 0x40000000U
#define APB1PERIPH_BASE             PERIPH_BASE
#define APB2PERIPH_BASE             0x40010000U
#define AHBPERIPH_BASE              0x40018000U

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */

#define RCC_BASEADDR                (AHBPERIPH_BASE + 0x9000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */

#define GPIOA_BASEADDR              (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASEADDR              (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASEADDR              (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASEADDR              (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASEADDR              (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASEADDR              (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASEADDR              (APB2PERIPH_BASE + 0x2000)

#define SPI1_BASEADDR               (APB2PERIPH_BASE + 0x2000)

#define ADC1_BASEADDR               (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASEADDR               (APB2PERIPH_BASE + 0x2800)
#define ADC3_BASEADDR               (APB2PERIPH_BASE + 0x3C00)

#define USART1_BASEADDR             (APB2PERIPH_BASE + ox3800)

#define EXTI_BASEADDR               (APB2PERIPH_BASE + 0x0400)

#define AFIO_BASEADDR               (APB2PERIPH_BASE + 0x0000)


/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */

#define I2C1_BASEADDR               (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR               (APB1PERIPH_BASE + 0x5800)

#define SPI2_BASEADDR               (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR               (APB1PERIPH_BASE + 0x3C00)

#define UART4_BASEADDR              (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR              (APB1PERIPH_BASE + 0x5000)

#define USART3_BASEADDR             (APB1PERIPH_BASE + 0x4800)
#define USART2_BASEADDR             (APB1PERIPH_BASE + 0x4400)


/*****************************peripheral register definition structure **********************************************/


/*
 *  Note : Registers of a peripheral are specified to MCU
 */

typedef struct
{
	__vo uint32_t CRL;                       /*  GPIO Port configuration register low         Address offset: 0x00 */
	__vo uint32_t CRH;                       /*  GPIO Port configuration register high        Address offset: 0x04 */
	__vo uint32_t IDR;                       /*  GPIO Port input data register                Address offset: 0x08 */
	__vo uint32_t ODR;                       /*  GPIO Port Output data register               Address offset: 0x0C */
	__vo uint32_t BSRR;                      /*  GPIO Port bit set/reset register             Address offset: 0x10 */
	__vo uint32_t BRR;                       /*  GPIO Port bit reset register                 Address offset: 0x14 */
	__vo uint32_t LCKR;	                    /*  GPIO Port configuration lock register        Address offset: 0x18 */
}GPIO_RegDef_t;


/* RCC Register Definition Structure */
typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t APB1RSTR;
	__vo uint32_t AHBENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t APB1ENR;
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
}RCC_RegDef_t;


/*
 * peripheral register definition structure for SPI
 */
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_RegDef_t;


/* EXTI Register Definition Structure */
typedef struct {
    __vo uint32_t IMR;   // Interrupt Mask Register
    __vo uint32_t EMR;   // Event Mask Register
    __vo uint32_t RTSR;  // Rising Trigger Selection Register
    __vo uint32_t FTSR;  // Falling Trigger Selection Register
    __vo uint32_t SWIER; // Software Interrupt Event Register
    __vo uint32_t PR;    // Pending Register
} EXTI_RegDef_t;


/* AFIO Register Definition Structure */
typedef struct {
	__vo uint32_t EVCR;
	__vo uint32_t MAPR;
	__vo uint32_t EXTICR[4]; // External Interrupt Configuration Registers 1-4
	__vo uint32_t MAPR2;
} AFIO_RegDef_t;

/*
 * peripheral register definition structure for I2C
 */
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t OAR1;
	__vo uint32_t OAR2;
	__vo uint32_t DR;
	__vo uint32_t SR1;
	__vo uint32_t SR2;
	__vo uint32_t CCR;
	__vo uint32_t TRISE;
}I2C_RegDef_t;

/*
 * Peripheral definitions (Peripheral base addresses typecasted to GPIOx_RegDef_t
 */

#define GPIOA          ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB          ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC          ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD          ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE          ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF          ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG          ((GPIO_RegDef_t*)GPIOG_BASEADDR)

#define RCC            ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI           ((EXTI_RegDef_t*) EXTI_BASEADDR)
#define AFIO           ((AFIO_RegDef_t*) AFIO_BASEADDR)


#define SPI1           ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2           ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3           ((SPI_RegDef_t*)SPI3_BASEADDR)

#define I2C1           ((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2           ((I2C_RegDef_t*)I2C2_BASEADDR)
/*
 * Clock Enable MAcros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()     ( RCC->APB2ENR |= (1 << 2))
#define GPIOB_PCLK_EN()     ( RCC->APB2ENR |= (1 << 3))
#define GPIOC_PCLK_EN()     ( RCC->APB2ENR |= (1 << 4))
#define GPIOD_PCLK_EN()     ( RCC->APB2ENR |= (1 << 5))
#define GPIOE_PCLK_EN()     ( RCC->APB2ENR |= (1 << 6))
#define GPIOF_PCLK_EN()     ( RCC->APB2ENR |= (1 << 7))
#define GPIOG_PCLK_EN()     ( RCC->APB2ENR |= (1 << 8))

/*
 * Clock Enable MAcros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()      ( RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()      ( RCC->APB1ENR |= (1 << 22))

/*
 * Clock Enable MAcros for SPIx peripherals
 */

#define SPI1_PCLK_EN()      ( RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()      ( RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()      ( RCC->APB1ENR |= (1 << 15))

/*
 * Clock Enable MAcros for USARTx peripherals
 */

#define USART1_PCLK_EN()      ( RCC->APB2ENR |= (1 << 14))
#define USART2_PCLK_EN()      ( RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()      ( RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()       ( RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()       ( RCC->APB1ENR |= (1 << 20))

/*
 * Clock Enable MAcros for USARTx peripherals
 */

#define AFIO_PCLK_EN()        ( RCC->APB2ENR |= (1 << 0))


/*
 * Clock Disable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 2))
#define GPIOB_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 3))
#define GPIOC_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 4))
#define GPIOD_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 5))
#define GPIOE_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 6))
#define GPIOF_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 7))
#define GPIOG_PCLK_DI()     ( RCC->APB2ENR &= ~(1 << 8))

/*
 * Clock Disable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 22))

/*
 * Clock Disable Macros for SPIx peripherals
 */

#define SPI1_PCLK_DI()      ( RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 15))

/*
 * Clock Disable Macros for USARTx peripherals
 */

#define USART1_PCLK_DI()      ( RCC->APB2ENR &= ~(1 << 14))
#define USART2_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()      ( RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()       ( RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()       ( RCC->APB1ENR &= ~(1 << 20))

/*
 * Clock Disable Macros for USARTx peripherals
 */

#define AFIO_PCLK_DI()        ( RCC->APB2ENR &= ~(1 << 0))

#define GPIOA_REG_RESET()   do { RCC->APB2RSTR |= (1 << 2); RCC->APB2RSTR &= ~(1 << 2); } while(0)
#define GPIOB_REG_RESET()   do { RCC->APB2RSTR |= (1 << 3); RCC->APB2RSTR &= ~(1 << 3); } while(0)
#define GPIOC_REG_RESET()   do { RCC->APB2RSTR |= (1 << 4); RCC->APB2RSTR &= ~(1 << 4); } while(0)
#define GPIOD_REG_RESET()   do { RCC->APB2RSTR |= (1 << 5); RCC->APB2RSTR &= ~(1 << 5); } while(0)
#define GPIOE_REG_RESET()   do { RCC->APB2RSTR |= (1 << 6); RCC->APB2RSTR &= ~(1 << 6); } while(0)


#define GPIO_BASEADDR_TO_CODE(x)          ((x == GPIOA) ? 0 :\
		                                  (x == GPIOB) ? 1 :\
		                                  (x == GPIOC) ? 2 :\
		                                  (x == GPIOD) ? 3 :\
		                                  (x == GPIOE) ? 4 :0 )

/*
 * IRQ(Interrupt Request) Number of STM32F103RB MCU
 */

#define IRQ_NO_EXTI0         13
#define IRQ_NO_EXTI1         14
#define IRQ_NO_EXTI2         15
#define IRQ_NO_EXTI3         16
#define IRQ_NO_EXTI4         17
#define IRQ_NO_EXTI9_5       30
#define IRQ_NO_EXTI15_10     47
#define IRQ_NO_SPI1          42
#define IRQ_NO_SPI2          43
#define IRQ_NO_SPI3          58


/*
 * Macros for NVIC interrupt priority
 */

#define NVIC_IRQ_PRIO0       0
#define NVIC_IRQ_PRIO1       1
#define NVIC_IRQ_PRIO2       2
#define NVIC_IRQ_PRIO3       3
#define NVIC_IRQ_PRIO4       4
#define NVIC_IRQ_PRIO5       5
#define NVIC_IRQ_PRIO6       6
#define NVIC_IRQ_PRIO7       7
#define NVIC_IRQ_PRIO8       8
#define NVIC_IRQ_PRIO9       9
#define NVIC_IRQ_PRIO10      10
#define NVIC_IRQ_PRIO11      11
#define NVIC_IRQ_PRIO12      12
#define NVIC_IRQ_PRIO13      13
#define NVIC_IRQ_PRIO14      14
#define NVIC_IRQ_PRIO15      15

/*
 * Some Generic Macros
 */
#define ENABLE           1
#define DISABLE          0
#define SET              ENABLE
#define RESET            DISABLE
#define GPIO_PIN_SET     SET
#define GPIO_PIN_RESET   RESET
#define FLAG_RESET       RESET
#define FLAG_SET         SET

/***************************************************************************
 *             Bit position definitions of SPI peripheral
 ***************************************************************************/

/*
 * Bit position definitions for SPI_CR1
 */
#define SPI_CR1_CPHA           0
#define SPI_CR1_CPOL           1
#define SPI_CR1_MSTR           2
#define SPI_CR1_BR             3
#define SPI_CR1_SPE            6
#define SPI_CR1_LSBFIRST       7
#define SPI_CR1_SSI            8
#define SPI_CR1_SSM            9
#define SPI_CR1_RXONLY         10
#define SPI_CR1_DFF            11
#define SPI_CR1_CRCNEXT        12
#define SPI_CR1_CRCEN          13
#define SPI_CR1_BIDIOE         14
#define SPI_CR1_BIDIMODE       15


/*
 * Bit position definitions for SPI_CR2
 */
#define SPI_CR2_RXDMAEN        0
#define SPI_CR2_TXDMAEN        1
#define SPI_CR2_SSOE           2
#define SPI_CR2_FRF            4
#define SPI_CR2_ERRIE          5
#define SPI_CR2_RXNEIE         6
#define SPI_CR2_TXEIE          7

/*
 * Bit position definitions for SPI_SR
 */
#define SPI_SR_RXNE            0
#define SPI_SR_TXE             1
#define SPI_SR_CHSIDE          2
#define SPI_SR_UDR             3
#define SPI_SR_CRCERR          4
#define SPI_SR_MODF            5
#define SPI_SR_OVR             6
#define SPI_SR_BSY             7
#define SPI_SR_FRE             8

#define SPI1_REG_RESET()   do { RCC->APB2RSTR |= (1 << 12); RCC->APB2RSTR &= ~(1 << 12); } while(0)
#define SPI2_REG_RESET()   do { RCC->APB1RSTR |= (1 << 14); RCC->APB1RSTR &= ~(1 << 14); } while(0)
#define SPI3_REG_RESET()   do { RCC->APB1RSTR |= (1 << 15); RCC->APB1RSTR &= ~(1 << 15); } while(0)


/***********************************************************************************
 *   Bit Position definitions of I2C peripheral
 **********************************************************************************/

/*
 * Bit position definitions for I2C_CR1
 */
#define I2C_CR1_PE             0
#define I2C_CR1_NOSTRETCH      7
#define I2C_CR1_START          8
#define I2C_CR1_STOP           9
#define I2C_CR1_ACK            10
#define I2C_CR1_SWRST          15


/*
 * Bit position definitions for I2C_CR2
 */
#define I2C_CR2_FREQ           0
#define I2C_CR2_ITERREN        8
#define I2C_CR2_ITEVTEN        9
#define I2C_CR2_ITBUFEN        10


/*
 * Bit position definitions for I2C_OAR1
 */
#define I2C_OAR1_ADD0             0
#define I2C_OAR1_ADD71            1
#define I2C_OAR1_ADD98            8
#define I2C_OAR1_ADDMOD           15

/*
 * Bit position definitions for I2C_SR1
 */
#define I2C_SR1_SB             0
#define I2C_SR1_ADDR           1
#define I2C_SR1_BTF            2
#define I2C_SR1_ADD10          3
#define I2C_SR1_STOPF          4
#define I2C_SR1_RXNE           6
#define I2C_SR1_TXE            7
#define I2C_SR1_BERR           8
#define I2C_SR1_ARLO           9
#define I2C_SR1_AF             10
#define I2C_SR1_OVR            11
#define I2C_SR1_TIMEOUT        14

/*
 * Bit position definitions for I2C_SR2
 */
#define I2C_SR2_MSL            0
#define I2C_SR2_BUSY           1
#define I2C_SR2_TRA            2
#define I2C_SR2_GENCALL        3
#define I2C_SR2_DUALF          4

/*
 * Bit position definitions for I2C_CCR
 */
#define I2C_CCR_CCR            0
#define I2C_CCR_DUTY           14
#define I2C_CCR_FS             15


#include "stm32f103xx_gpio_driver.h"
#include "stm32f103xx_spi_driver.h"
#include "stm32f103xx_i2c_driver.h"


#endif /* INC_STM32F103XX_H_ */
