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
#include "string.h"

#define LOW           0
#define BTN_PRESSED   LOW

void EXTI15_10_IRQHandler(void);

void delay(void)
{
    for(uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;
    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GpioBtn, 0, sizeof(GpioBtn));

    // 1. Enable Peripheral Clocks (Include AFIO for STM32F1 EXTI!)
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_PeriClockControl(GPIOC, ENABLE);
    AFIO_PCLK_EN(); // <--- CRITICAL for STM32F1 EXTI mapping!

    // 2. Configure LED Pin (PA5)
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
    GpioLed.GPIO_PinConfig.GPIO_PinOutputType  = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PULL_UP;
    GPIO_Init(&GpioLed);

    // 3. Configure Button Pin (PC13) with Falling-Edge Interrupt
    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IT_FT;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_2MHZ;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PULL_UP;
    GPIO_Init(&GpioBtn);

    // 4. Configure NVIC Priority and Enable IRQ
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRIO15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

    // 5. CRITICAL FIX: Infinite Loop to keep CPU alive waiting for interrupts
    while(1)
    {
        // CPU waits here; ISR handles button presses asynchronously
    }

    return 0;
}

// Interrupt Handler Routine
void EXTI15_10_IRQHandler(void)
{
    GPIO_IRQHandling(GPIO_PIN_NO_13);          // Clear pending EXTI interrupt bit first
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5); // Toggle LED state

}
