/*
 * shared_i2c.c - shared I2C bus basic routines
 * 07-12-19 E. Brombaugh
 */
 
#include "shared_i2c.h"
#include "printf.h"

/* ST's crazy I2C timing register for 100kHz (from CubeMX) */
#define I2Cx_TIMING ((uint32_t)0x60808CD3)  

I2C_HandleTypeDef i2c_handler;

/*
 * initialize shared I2C bus
 */
void shared_i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	/* init the peripheral clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C23;
    PeriphClkInit.I2c23ClockSelection = RCC_I2C23CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
		printf("shared_i2c_init: PeriphCLKConfig failed\n\r");
		while(1){}
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* reset the I2C bus */
	shared_i2c_reset();
}

void shared_i2c_reset(void)
{
	/* Enable the I2C1 peripheral clock & reset it */
    __HAL_RCC_I2C2_CLK_ENABLE();
	__HAL_RCC_I2C2_FORCE_RESET();
	__HAL_RCC_I2C2_RELEASE_RESET();
	
	/* point at the right interface */
	i2c_handler.Instance = I2C2;
	
	/* De-initialize the I2C communication bus */
	HAL_I2C_DeInit(&i2c_handler);

	i2c_handler.Init.Timing = 0x20C0EDFF;
	i2c_handler.Init.OwnAddress1 = 0;
	i2c_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c_handler.Init.OwnAddress2 = 0;
	i2c_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2c_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c_handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c_handler);
	HAL_I2CEx_ConfigAnalogFilter(&i2c_handler, I2C_ANALOGFILTER_ENABLE);
	HAL_I2CEx_ConfigDigitalFilter(&i2c_handler, 0);
}
