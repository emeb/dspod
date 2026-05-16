/*
 * mco.h - dspod_u3c5 Master Clock Output setup
 * 05-04-26 E. Brombaugh
 */

#include "led.h"

#define MCO1_PIN                                GPIO_PIN_8
#define MCO1_GPIO_PORT                          GPIOA
#define MCO1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

/*
 * Initialize the MCO
 */
void MCOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable Clock */
	MCO1_GPIO_CLK_ENABLE();
	
	/* Enable Ports/Pins */
	GPIO_InitStructure.Pin =  MCO1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF0_MCO;
	HAL_GPIO_Init(MCO1_GPIO_PORT, &GPIO_InitStructure);
	
	/* set MCO1 source and divisor */
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

}
