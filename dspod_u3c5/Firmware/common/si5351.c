/*
 * si5351.c - SI5351 Clock Generator driver for dspod_u3c5
 * 01-08-2017 E. Brombaugh
 * 05-06-2026 E. Brombaugh - updated for stm32u3c5
 */

#include "si5351.h"
#include <math.h>
#include "printf.h"
#include "shared_i2c.h"

/*
 * I2C stuff
 */
#define SI5351_I2C_ADDRESS   0xC0	// 110000+SA0+RW - 0x60 or 0x61, shifted left
#define I2C_TIMEOUT          100

/*
 * Freq calcs
 */
#define XTAL_FREQ 25000000
#define C_MAX ((1<<20)-1)

/* register definitions */
#define SI5351_0_Device_Status 0
#define SI5351_1_Interrupt_Status_Sticky 1
#define SI5351_2_Interrupt_Status_Mask 2
#define SI5351_3_Output_Enable_Control 3
#define SI5351_9_OEB_Pin_Enable_Control_Mask 9
#define SI5351_15_PLL_Input_Source 15
#define SI5351_16_CLK0_Control 16
#define SI5351_17_CLK1_Control 17
#define SI5351_18_CLK2_Control 18
#define SI5351_19_CLK3_Control 19
#define SI5351_20_CLK4_Control 20
#define SI5351_21_CLK5_Control 21
#define SI5351_22_CLK6_Control 22
#define SI5351_23_CLK7_Control 23
#define SI5351_24_CLK3_0_Disable_State 24
#define SI5351_25_CLK7_4_Disable_State 25
#define SI5351_26_Multisynth_NA_Parameters_0 26
#define SI5351_27_Multisynth_NA_Parameters_1 27
#define SI5351_28_Multisynth_NA_Parameters_2 28
#define SI5351_29_Multisynth_NA_Parameters_3 29
#define SI5351_30_Multisynth_NA_Parameters_4 30
#define SI5351_31_Multisynth_NA_Parameters_5 31
#define SI5351_32_Multisynth_NA_Parameters_6 32
#define SI5351_33_Multisynth_NA_Parameters_7 33
#define SI5351_34_Multisynth_NB_Parameters_0 34
#define SI5351_35_Multisynth_NB_Parameters_1 35
#define SI5351_36_Multisynth_NB_Parameters_2 36
#define SI5351_37_Multisynth_NB_Parameters_3 37
#define SI5351_38_Multisynth_NB_Parameters_4 38
#define SI5351_39_Multisynth_NB_Parameters_5 39
#define SI5351_40_Multisynth_NB_Parameters_6 40
#define SI5351_41_Multisynth_NB_Parameters_7 41
#define SI5351_42_Multisynth0_Parameters_0 42
#define SI5351_43_Multisynth0_Parameters_1 43
#define SI5351_44_Multisynth0_Parameters_2 44
#define SI5351_45_Multisynth0_Parameters_3 45
#define SI5351_46_Multisynth0_Parameters_4 46
#define SI5351_47_Multisynth0_Parameters_5 47
#define SI5351_48_Multisynth0_Parameters_6 48
#define SI5351_49_Multisynth0_Parameters_7 49
#define SI5351_50_Multisynth1_Parameters_0 50
#define SI5351_51_Multisynth1_Parameters_1 51
#define SI5351_52_Multisynth1_Parameters_2 52
#define SI5351_53_Multisynth1_Parameters_3 53
#define SI5351_54_Multisynth1_Parameters_4 54
#define SI5351_55_Multisynth1_Parameters_5 55
#define SI5351_56_Multisynth1_Parameters_6 56
#define SI5351_57_Multisynth1_Parameters_7 57
#define SI5351_58_Multisynth2_Parameters_0 58
#define SI5351_59_Multisynth2_Parameters_1 59
#define SI5351_60_Multisynth2_Parameters_2 60
#define SI5351_61_Multisynth2_Parameters_3 61
#define SI5351_62_Multisynth2_Parameters_4 62
#define SI5351_63_Multisynth2_Parameters_5 63
#define SI5351_64_Multisynth2_Parameters_6 64
#define SI5351_65_Multisynth2_Parameters_7 65
#define SI5351_66_Multisynth3_Parameters_0 66
#define SI5351_67_Multisynth3_Parameters_1 67
#define SI5351_68_Multisynth3_Parameters_2 68
#define SI5351_69_Multisynth3_Parameters_3 69
#define SI5351_70_Multisynth3_Parameters_4 70
#define SI5351_71_Multisynth3_Parameters_5 71
#define SI5351_72_Multisynth3_Parameters_6 72
#define SI5351_73_Multisynth3_Parameters_7 73
#define SI5351_74_Multisynth4_Parameters_0 74
#define SI5351_75_Multisynth4_Parameters_1 75
#define SI5351_76_Multisynth4_Parameters_2 76
#define SI5351_77_Multisynth4_Parameters_3 77
#define SI5351_78_Multisynth4_Parameters_4 78
#define SI5351_79_Multisynth4_Parameters_5 79
#define SI5351_80_Multisynth4_Parameters_6 80
#define SI5351_81_Multisynth4_Parameters_7 81
#define SI5351_82_Multisynth5_Parameters_0 82
#define SI5351_83_Multisynth5_Parameters_1 83
#define SI5351_84_Multisynth5_Parameters_2 84
#define SI5351_85_Multisynth5_Parameters_3 85
#define SI5351_86_Multisynth5_Parameters_4 86
#define SI5351_87_Multisynth5_Parameters_5 87
#define SI5351_88_Multisynth5_Parameters_6 88
#define SI5351_89_Multisynth5_Parameters_7 89
#define SI5351_90_Multisynth6_Parameters_0 90
#define SI5351_91_Multisynth7_Parameters_0 91
#define SI5351_92_Clock_6_and_7_Output_Divider 92
#define SI5351_149_Spread_Spectrum_Parameters_0 149
#define SI5351_150_Spread_Spectrum_Parameters_1 150
#define SI5351_151_Spread_Spectrum_Parameters_2 151
#define SI5351_152_Spread_Spectrum_Parameters_3 152
#define SI5351_153_Spread_Spectrum_Parameters_4 153
#define SI5351_154_Spread_Spectrum_Parameters_5 154
#define SI5351_155_Spread_Spectrum_Parameters_6 155
#define SI5351_156_Spread_Spectrum_Parameters_7 156
#define SI5351_157_Spread_Spectrum_Parameters_9 157
#define SI5351_158_Spread_Spectrum_Parameters_A 158
#define SI5351_159_Spread_Spectrum_Parameters_B 159
#define SI5351_160_Spread_Spectrum_Parameters_C 160
#define SI5351_161_Spread_Spectrum_Parameters_D 161
#define SI5351_162_VCXO_Parameter_0 162
#define SI5351_163_VCXO_Parameter_1 163
#define SI5351_164_VCXO_Parameter_2 164
#define SI5351_165_CLK0_Initial_Phase_Offset 165
#define SI5351_166_CLK1_Initial_Phase_Offset 166
#define SI5351_167_CLK2_Initial_Phase_Offset 167
#define SI5351_168_CLK3_Initial_Phase_Offset 168
#define SI5351_169_CLK4_Initial_Phase_Offset 169
#define SI5351_170_CLK5_Initial_Phase_Offset 170
#define SI5351_177_PLL_Reset 177
#define SI5351_183_Crystal_Internal_Load_Capacitance 183
#define SI5351_187_Fanout_Enable 187

/*
 * local vars
 */
uint32_t si5351_vco_freq[2];
uint8_t si5351_databuffer[8];

/*
 * exception handler for I2C timeout
 */
uint32_t SI5351_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/*
 * Send a block of data to the Si5351 via I2C
 */
HAL_StatusTypeDef si5351_i2c_write(uint8_t reg, uint8_t *data, uint8_t sz)
{
	uint8_t i, i2c_msg[9];
	HAL_StatusTypeDef status = HAL_OK;
	
	/* Assemble the message */
	i2c_msg[0] = reg;
	for(i=0;i<sz;i++)
		i2c_msg[1+i] = data[i];
  
	status = HAL_I2C_Master_Transmit(&i2c_handler, SI5351_I2C_ADDRESS,
										i2c_msg, sz+1, I2C_TIMEOUT);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* reset I2C communication bus */
		shared_i2c_reset();
	}
	
	return status;
}

/*
 * Get a block of data bytes from the Si5351 via I2C
 */
HAL_StatusTypeDef si5351_i2c_read(uint8_t reg, uint8_t *data, uint8_t sz)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* send register */
	status = HAL_I2C_Master_Transmit(&i2c_handler, SI5351_I2C_ADDRESS,
										&reg, 1, I2C_TIMEOUT);
	
	/* get data */
	if(status == HAL_OK)
		status = HAL_I2C_Master_Receive(&i2c_handler, SI5351_I2C_ADDRESS,
										data, sz, I2C_TIMEOUT);
	
	if(status != HAL_OK)
	{
		/* reset I2C communication bus */
		shared_i2c_reset();
	}
	
	return status;
}

/*
 * Initialize the SSD1306
 */
HAL_StatusTypeDef si5351_init(void)
{
	uint8_t i;
    HAL_StatusTypeDef status = HAL_OK;
	
	/* Disable all outputs */
	si5351_databuffer[0] = 0xFF;
	status = si5351_i2c_write(SI5351_3_Output_Enable_Control, si5351_databuffer, 1);
	
	/* Power down output drivers */
	for(i=0;i<8;i++)
		si5351_databuffer[i] = 0x80;
	status = si5351_i2c_write(SI5351_16_CLK0_Control, si5351_databuffer, 8);

	/* Set XTAL load */
	//si5351_databuffer[0] = 0x52;	// 6pf
	si5351_databuffer[0] = 0x92;	// 8pf - seems most accurate (per SA)
	//si5351_databuffer[0] = 0xD2;	// default 10pf
	status = si5351_i2c_write(SI5351_183_Crystal_Internal_Load_Capacitance, si5351_databuffer, 1);	

	/* init pll vco values */
	si5351_vco_freq[0] = 0;
	si5351_vco_freq[1] = 0;
	
	return status;
}

/*
 * compute values for multisynths in PLLs and outputs
 */
void si5351_multisynth_set(uint8_t *buffer, uint8_t a, uint32_t b, uint32_t c)
{
	uint32_t P1, P2, P3;
	
	/* Set the main Px config registers */
	if(!b)
	{
		/* Integer mode */
		P1 = (a << 7) - 512;
		P2 = b;
		P3 = c;
	}
	else
	{
        /* fixed point fractional calculations */
        uint64_t idiv;
        idiv = ((uint64_t)a<<7) + ((((uint64_t)b<<20)/C_MAX)>>(20-7))-512;
        P1 = idiv;
        idiv = ((uint64_t)b<<7) - C_MAX * ((((uint64_t)b<<20)/C_MAX)>>(20-7));
        P2 = idiv;
		P3 = c;
	}

	/* break the regs down as the hardware expects */
	buffer[0] = (P3 & 0x0000FF00) >> 8;
	buffer[1] = P3 & 0x000000FF;
	buffer[2] = (P1 & 0x00030000) >> 16;
	buffer[3] = (P1 & 0x0000FF00) >> 8;
	buffer[4] = P1 & 0x000000FF;
	buffer[5] = ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16);
	buffer[6] = (P2 & 0x0000FF00) >> 8;
	buffer[7] = P2 & 0x000000FF;
}

/*
 * set integer divide bits for output multisynths only
 */
void si5351_oms_div_bits(uint8_t *buffer, uint8_t odiv, uint8_t ms_div)
{
	buffer[2] |= ((odiv & 0x7) << 4);
	
	if(ms_div == 4)
		buffer[2] |= 0xC;
}

/*
 * set output channel 0 or 1 freq - this assumes:
 * - plla -> chl 0, pllb -> chl 1
 * - integer output division for best jitter
 */
void si5351_set_output_chl(uint8_t chl, uint32_t freq, uint8_t odiv)
{
    int8_t ms_div, a, reg;
    uint32_t Ftarget, b;
    uint64_t idiv;
    
	/* only good for first 2 chls */
	if(chl > 1)
		return;
	
	/* estimate rough divide ratio */
    ms_div = 0xFE&(900000000/freq);
    
    /* Desired VCO freq */
    Ftarget = freq * ms_div;
    
    /* fixed point ratio */
    idiv = ((uint64_t)Ftarget<<21) / XTAL_FREQ;
    a = idiv >> 21;
    b = ((idiv & ((1<<21)-1)) + 1)>>1;

    /* update pll vco freq */
    idiv = (XTAL_FREQ*(((uint64_t)b<<20)/C_MAX+((uint64_t)a<<20))+(1<<19))>>20;
    si5351_vco_freq[chl] = idiv; 

    /* compute pll multisync & send */
    si5351_multisynth_set(si5351_databuffer, a, b, C_MAX);
    reg = chl ? SI5351_34_Multisynth_NB_Parameters_0 : SI5351_26_Multisynth_NA_Parameters_0;
    si5351_i2c_write(reg, si5351_databuffer, 8);
    
    /* compute output multisync & send */
    si5351_multisynth_set(si5351_databuffer, ms_div, 0, 1);
    si5351_oms_div_bits(si5351_databuffer, odiv, ms_div);
    reg = chl ? SI5351_50_Multisynth1_Parameters_0 : SI5351_42_Multisynth0_Parameters_0;
    si5351_i2c_write(reg, si5351_databuffer, 8);
	
	/* reset the PLL */
	if(chl)
	{
		/* PLL B */
		si5351_databuffer[0] = 0x80;
	}
	else
	{
		/* PLL A */
		si5351_databuffer[0] = 0x20;
	}
	si5351_i2c_write(SI5351_177_PLL_Reset, si5351_databuffer, 1);
   
    /* set output channel parameters */
    if(chl)
    {
        /* powerup,integer,PLLB,uninverted,MS1,8ma */
        si5351_databuffer[0] = 0x6F;
        reg = SI5351_17_CLK1_Control;
    }
    else
    {
        /* powerup,integer,PLLA,uninverted,MS0,8ma */
        si5351_databuffer[0] = 0x4F;
        reg = SI5351_16_CLK0_Control;
    }
    si5351_i2c_write(reg, si5351_databuffer, 1);
    
    /* enable output channel */
	si5351_oena(chl, 1);
}

/*
 * set output channel 2 freq - must be even integer divide of PLLB
 * div may range [4,6,8...254]
 */
void si5351_set_output_3(uint8_t idiv, uint8_t odiv)
{
    /* compute output multisync & send */
    si5351_multisynth_set(si5351_databuffer, idiv, 0, 1);
    si5351_oms_div_bits(si5351_databuffer, odiv, idiv);
    si5351_i2c_write(SI5351_58_Multisynth2_Parameters_0, si5351_databuffer, 8);
	
    /* set output channel parameters */
	/* powerup,integer,PLLB,uninverted,MS2,8ma */
	si5351_databuffer[0] = 0x6F;
    si5351_i2c_write(SI5351_18_CLK2_Control, si5351_databuffer, 1);
	
    /* enable output channel */
	si5351_oena(2, 1);
}

/*
 * output control
 */
void si5351_oena(uint8_t chl, uint8_t state)
{
	uint8_t bit = 1<<chl;
	
	/* get current value */
    si5351_i2c_read(SI5351_3_Output_Enable_Control, si5351_databuffer, 1);
	
	/* on or off? */
	if(state)
		si5351_databuffer[0] &= ~bit;	// on
	else
		si5351_databuffer[0] |= bit;	// off
	
	/* send new value */
    si5351_i2c_write(SI5351_3_Output_Enable_Control, si5351_databuffer, 1);
}

/*
 * get vco freq - chl 1 or 2 only
 */
uint32_t si5351_get_vco_freq(uint8_t chl)
{
	/* only good for first 2 chls */
	if(chl > 1)
		return 0;
	else
		return si5351_vco_freq[chl];
}

