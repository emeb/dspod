/*
 * led.h - dspod_u3c5 LED setup
 * 05-04-26 E. Brombaugh
 */

#include "led.h"

#define LED1_PIN                                GPIO_PIN_15
#define LED1_GPIO_PORT                          GPIOB
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/*
 * Initialize the nucleo LEDs
 */
void LEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable Clock */
	LED1_GPIO_CLK_ENABLE();
	
	/* Enable Ports/Pins */
	GPIO_InitStructure.Pin =  LED1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * Turn on LED
 */
void LEDOn(void)
{
	LED1_GPIO_PORT->BSRR = LED1_PIN;
}

/*
 * Turn off LED
 */
void LEDOff(void)
{
	LED1_GPIO_PORT->BSRR = LED1_PIN<<16;
}

/*
 * Toggle LED
 */
void LEDToggle(void)
{
			LED1_GPIO_PORT->ODR ^= LED1_PIN;
}

