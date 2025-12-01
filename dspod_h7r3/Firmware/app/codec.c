/*
 * codec.h - codec I2C control port driver for NAU88C22
 * 11-03-25 E. Brombaugh
 */
/**
  * @brief  Initializes the audio codec and all related interfaces (control
  *         interface: I2C and audio interface: I2S)
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */

#include "codec.h"
#include "shared_i2c.h"
#include "printf.h"

/* The 7 bits NAU88C22 address (sent through I2C interface) */
#define NAU88C22_I2C_ADDR (0x1A<<1)

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
  * @brief  Writes to a given register into the TLV320AIC3101 audio codec
			through the control interface (I2C)
  * @param  RegAddr: The address of the register to be written.
  * @param  RegValue: value to be written to register.
  * @retval HAL_OK if success
  */
HAL_StatusTypeDef Codec_WriteRegister(uint8_t RegAddr, uint16_t RegValue)
{
	uint8_t Addr, Data;

	/* Assemble 2-byte data in NAU88C22 format */
    Addr = ((RegAddr&0x7F)<<1) | ((RegValue>>8)&1);
	Data = RegValue&0xFF;
	return HAL_I2C_Mem_Write(&i2c_handler, NAU88C22_I2C_ADDR, Addr, 1,
		&Data, 1, 100);
}

/**
  * @brief  Reads from a given register into the TLV320AIC3101 audio codec
			through the control interface (I2C)
  * @param  RegAddr: The address of the register to be read.
  * @param  RegValue: pointer to value read from register.
  * @retval HAL_OK if success
  */
HAL_StatusTypeDef Codec_ReadRegister(uint8_t RegAddr, uint16_t *RegValue)
{
	/* shift up one bit for formatting */
	RegAddr <<= 1;
	
	return HAL_I2C_Mem_Read(&i2c_handler, NAU88C22_I2C_ADDR, RegAddr, 1,
		(uint8_t *)RegValue, 2, 100);
}

/**
  * @brief  Resets the audio codec by sending an I2C command
  * @param  None
  * @retval None
  */
void Codec_Reset(void)
{
}

/*
 * configure the default settings
 */
void Codec_Config(void)
{
	uint16_t idx = 0, reg, val;
	
	while((reg = codec_settings[2*idx]) != 0x00FF)
	{
		val = codec_settings[2*idx + 1];
		if(Codec_WriteRegister(reg, val) == HAL_OK)
		{
			//printf("Codec_Config(): Write Addr %3d = 0x%02X\n\r", reg, val);
		}
		else
			printf("Codec_Config(): Write Addr %3d failed\n\r", reg);
		idx++;
	}
}

/*
 * Do all hardware setup for Codec except for final reset & config
 */
void Codec_Init(void)
{
	/* Toggle the codec NRST pin */
	Codec_Reset();
	
	/* Set up codec */
	Codec_Config();
	
#if 0
	/* diagnostics */
	uint8_t reg = 0, val[128], sz;
	
#if 0
	/* test write page reg */
	val[0] = 1;
	sz = 1;
	if(Codec_WriteRegister(reg, val, 1) == HAL_OK)
		printf("Write Addr 0x%02X = 0x%02X\n\r", reg, val[0]);
	else
		printf("Write Addr 0x%02X failed\n\r", reg);
#endif

	/* test read */
	sz = 128;
	HAL_StatusTypeDef stat;
	if((stat = Codec_ReadRegister(reg, val, sz)) == HAL_OK)
	{
		for(reg=0;reg<sz;reg++)
			printf("Read Addr %3d = 0x%02X\n\r", reg, val[reg]);
	}
	else
		printf("Read Addr %3d failed, err = %d\n\r", reg, stat);
#endif
}

