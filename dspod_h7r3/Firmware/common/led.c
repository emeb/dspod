/*
 * led.c - dspod_h7r3 LED setup
 * 10-14-25 E. Brombaugh
 */

#include "led.h"

#define LD0_Pin GPIO_PIN_12
#define LD0_GPIO_Port GPIOB

/*
 * Initialize the dspod_h7r3 LED
 */
void LEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO B Clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Enable LD0 for output */
	GPIO_InitStructure.Pin =  LD0_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(LD0_GPIO_Port, &GPIO_InitStructure);
}

/*
 * Turn on LED
 */
void LEDOn(void)
{
	LD0_GPIO_Port->BSRR = LD0_Pin;
}

/*
 * Turn off LED
 */
void LEDOff(void)
{
	LD0_GPIO_Port->BSRR = LD0_Pin<<16;
}

/*
 * Toggle LED
 */
void LEDToggle(void)
{
	LD0_GPIO_Port->ODR ^= LD0_Pin;
}

