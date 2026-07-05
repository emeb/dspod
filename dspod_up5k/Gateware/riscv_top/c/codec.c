/*
 * codec.c - driver for NAU88C22 codec I2C control port
 * 06-08-2026 E. Brombaugh
 */
#include "codec.h"
#include "i2c.h"

/* The 7 bits NAU88C22 address (sent through I2C interface) */
#define NAU88C22_I2C_ADDR 0x1A

/* registers / bits we may use */
#define NAU88C22_REG_DACCTRL 0x0A
#define NAU88C22_BIT_DACCTRL_SOFTMUTE (1<<6)

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
	10,		0x048,	// 128x rate, muted
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

/*
 * send reg addr + data to codec
 */
int8_t Codec_WriteRegister(uint8_t RegAddr, uint16_t RegValue)
{
	uint8_t i2c_msg[2];

	/* Assemble 2-byte data in NAU88C22 format */
    i2c_msg[0] = ((RegAddr&0x7F)<<1) | ((RegValue>>8)&1);
	i2c_msg[1] = RegValue&0xFF;
	return i2c_mtx(I2C0, NAU88C22_I2C_ADDR, i2c_msg, 2);
}

/*
 * get reg addr + data from codec
 */
int8_t Codec_ReadRegister(uint8_t RegAddr, uint16_t *RegValue)
{
	uint8_t tx_msg = (RegAddr&0x7F)<<1;
	uint8_t rx_msg[2];
	uint8_t status = i2c_mtxrx(I2C0, NAU88C22_I2C_ADDR, &tx_msg, 1, rx_msg, 2);
	*RegValue = (rx_msg[0] << 8) | rx_msg[1];
	return status;
}

/*
 * initialize the codec via the I2C port
 */
uint8_t Codec_Init(void)
{
	uint16_t idx = 0, reg, val, stat = 0;
	
	while((reg = codec_settings[2*idx]) != 0x00FF)
	{
		val = codec_settings[2*idx + 1];
		if((stat |= Codec_WriteRegister(reg, val)) == 0)
		{
			//printf("Codec_Config(): Write Addr %3d = 0x%03X\n\r", reg, val);
		}
		else
			printf("Codec_Config(): Write Addr %3d failed\n\r", reg);
		idx++;
	}
	
	return stat;
}

/*
 * Codec DAC mute
 */
void Codec_Mute(int8_t ena)
{
	uint16_t RegValue;
	
	/* get R10 current value */
	Codec_ReadRegister(NAU88C22_REG_DACCTRL, &RegValue);
	
	/* set/clear SoftMute bit */
	if(ena)
		RegValue |= NAU88C22_BIT_DACCTRL_SOFTMUTE;
	else
		RegValue &= ~NAU88C22_BIT_DACCTRL_SOFTMUTE;
	
	/* write new value */
	Codec_WriteRegister(NAU88C22_REG_DACCTRL, RegValue);
}
