/*
 * NAU88C22.c - NAU88C22 codec I2C control port driver for rp2040_audio
 * 05-21-25 E. Brombaugh
 */

#include <stdio.h>
#include "main.h"
#include "hardware/i2c.h"
#include "nau88c22.h"

#define I2C_PORT i2c0
#define I2C_SDA_PIN 20
#define I2C_SCL_PIN 21

/* The 7 bits NAU88C22 address (sent through I2C interface) */
#define NAU88C22_I2C_ADDR 0x1A

/* Initialization data */
static uint16_t codec_settings[] = 
{
	// Reset and power-up
	0,		0x000,	// Software Reset
	1,		0x0CD,	// aux mixers, internal tie-off enable & 80k impedance for slow charge
	69,		0x000,	// low voltage bias
	127,	250,	// Wait 250ms
	
	// Input routing & ADC setup
	2,		0x03F,	// ADC, PGA, Mix/Boost inputs powered up
	//14,		0x108,	// HPF, 128x
	14,		0x008,	// DC, 128x
	
	44,		0x044,	// PGA input - select line inputs
	45, 	0x010,	// LPGA 0dB, unmuted, immediate, no ZC
	46,		0x010,	// RPGA 0dB, unmuted, immediate, no ZC
	47,		0x030,	// Lchl line in 0dB, no boost
	48,		0x030,	// Rchl line in 0dB, no boost
	
	// Output routing & DAC setup
	3,		0x18F,	// DACs and aux outputs enabled
	10,		0x008,	// 128x rate
//	10,		0x000,	// 64x rate
	49,		0x002,	// thermal shutdown only (default)
	50,		0x001,	// L main mixer input from LDAC (default) NEEDED!
	51,		0x001,	// R main mixer input from RDAC (default) NEEDED!
	56,		0x001,	// LDAC to AUX2 (default) NEEDED!
	57,		0x001,	// RDAC to AUX1 (default) NEEDED!
	
	// Format & clock
	4, 		0x010,	// 16-bit I2S
#if 1
	// No PLL
	6,		0x000,	// MCLK, no PLL, 1x division, FS, BCLK inputs
	7,		0x000,	// 4wire off, 48k, no timer (default)
#else
	// PLL setting for IMCLK = 12.5MHz from 12.5MHz input
	6,		0x140,	// PLL, 2x division, FS, BCLK inputs (default)
	7,		0x000,	// 4wire off, 48k, no timer (default)
	36,		0x008,	// PLL D = 1, N = 8
	37,		0x000,	// K (high) = 0
	38,		0x000,	// K (mid) = 0
	39,		0x000,	// K (low) = 0
	8,		0x034,	// CSB pin is PLL/16
	1,		0x0ED,	// enable PLL
#endif

	255,	0x000,	// EOF
};


/**
  * @brief  Writes a Byte to a given register into the NAU88C22
			through the control interface (I2C)
  * @param  Reg: The 7-bit address of the register to be written.
  * @param  Data: the 9-bit value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
int32_t NAU88C22_WriteRegister(uint16_t Reg, uint16_t Data)
{
	int32_t status;
	uint8_t i2c_msg[2];

	/* Assemble 2-byte data in NAU88C22 format */
    i2c_msg[0] = ((Reg&0x7F)<<1) | ((Data>>8)&1);
	i2c_msg[1] = Data&0xFF;

	status = i2c_write_timeout_us(I2C_PORT, NAU88C22_I2C_ADDR, i2c_msg, 2, false, 10000);

	/* Check the communication status */
	if(status != 2)
	{
		/* Reset the I2C communication bus */
		//printf("NAU88C22_WriteRegister: write to DevAddr 0x%02X / Reg %3d failed - resetting.\n\r",
		//	NAU88C22_I2C_ADDR, Reg);
		
		i2c_deinit(I2C_PORT);
		i2c_init(I2C_PORT, 100*1000);
		
		return 1;
	}
	
	//printf("NAU88C22_WriteRegister: write to DevAddr 0x%02X / Reg %3d = 0x%03X\n\r",
	//	NAU88C22_I2C_ADDR, Reg, Data);

	return 0;
}

/**
  * @brief  Writes a Byte to a given register into the NAU88C22
			through the control interface (I2C)
  * @param  Reg: The 7-bit address of the register to be written.
  * @param  Data: the 9-bit value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
int32_t NAU88C22_ReadRegister(uint16_t Reg, uint16_t *Data)
{
	int32_t status;
	uint8_t i2c_msg[2];

	/* send register address in one byte with nostop */
    i2c_msg[0] = ((Reg&0x7F)<<1);

	status = i2c_write_timeout_us(I2C_PORT, NAU88C22_I2C_ADDR, i2c_msg, 1, true, 10000);

	/* Check the communication status */
	if(status != 1)
	{
		/* Reset the I2C communication bus */
		//printf("NAU88C22_ReadRegister: send reg to DevAddr 0x%02X / Reg %3d failed - resetting.\n\r",
		//	NAU88C22_I2C_ADDR, Reg);
		
		i2c_deinit(I2C_PORT);
		i2c_init(I2C_PORT, 100*1000);
		
		return 1;
	}
	
	/* get 2-byte read data */
	status = i2c_read_timeout_us(I2C_PORT, NAU88C22_I2C_ADDR, i2c_msg, 2, false, 10000);

	/* Check the communication status */
	if(status != 2)
	{
		/* Reset the I2C communication bus */
		//printf("NAU88C22_ReadRegister: get data from DevAddr 0x%02X failed - resetting.\n\r",
		//	NAU88C22_I2C_ADDR);
		
		i2c_deinit(I2C_PORT);
		i2c_init(I2C_PORT, 100*1000);
		
		return 1;
	}

	/* assemble 9-bit result */
	*Data = ((i2c_msg[0]&1)<<8) | i2c_msg[1];
	
	//printf("NAU88C22_ReadRegister: read from DevAddr 0x%02X / Reg %3d = 0x%03X\n\r",
	//	NAU88C22_I2C_ADDR, Reg, *Data);

	return 0;
}

/**
  * @brief  Resets the audio NAU88C22. It restores the default configuration of the
  *         NAU88C22 (this function shall be called before initializing the NAU88C22).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
int32_t NAU88C22_Reset(void)
{
	uint8_t idx = 0, reg, tries;
	uint16_t data;
	int32_t result = 0;
	
	while((reg = codec_settings[2*idx]) < 0x80)
	{
		data = codec_settings[2*idx + 1];
		if(reg < 127)
		{
			/* valid register - write data */
			/*
			 * NOTE: Sometimes writing reg 1 fails. Seems to succeed on 2nd try.
			 */
			tries = 0;
			while((NAU88C22_WriteRegister(reg, data)!=0) && (tries++ <5));
			if(tries > 4)
			{
				printf("NAU88C22_Reset: write to Reg %3d failed after %d tries.\n\r",
					reg, tries);
				result++;
			}
		}
		else
		{
			/* time delay */
			//printf("NAU88C22_Reset: delay %d ms\n\r", data);
			sleep_ms(data);
		}
		
		idx++;
	}

	return result;
}

/*
 * diagnostic to spew all the registers
 */
int32_t NAU88C22_Dump_Regs(void)
{
	uint8_t reg = 0;
	uint16_t data;
	printf("Dumping NAU88C22 registers...\n");
	while(reg < 82)
	{
		NAU88C22_ReadRegister(reg, &data);
		printf("Reg %3d = 0x%03X\n\r", reg, data);
		reg++;
	}
}

/*
 * Do all hardware setup for NAU88C22 including reset & config
 */
int32_t NAU88C22_Init(void)
{
	/* setup i2c */
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    i2c_init(I2C_PORT, 100*1000);
	
	return NAU88C22_Reset();
}

