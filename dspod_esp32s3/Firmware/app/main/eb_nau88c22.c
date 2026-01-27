/*
 * eb_nau88c22.c - my high-level nau88c22 codec driver.
 * 01-16-26 E. Brombaugh
 */
#include <stdio.h>
#include "main.h"
#include "driver/i2c_master.h"
#include "eb_nau88c22.h"

static const char *TAG = "eb_nau88c22";

#define I2C_MASTER_SCL_IO           16		/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO			17		/*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM				0		/*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ			100000	/*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE	0		/*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE	0		/*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS	1000

/* The 7 bits Codec address (sent through I2C interface) */
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

/* codec device handle */
static i2c_master_dev_handle_t codec_dev_handle;

/*
 * top-level codec setup
 */
esp_err_t eb_nau88c22_Init(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
		.i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .glitch_ignore_cnt = 7,
		.flags.enable_internal_pullup = true
    };

	i2c_master_bus_handle_t bus_handle;
	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

	i2c_device_config_t dev_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = NAU88C22_I2C_ADDR,
		.scl_speed_hz = 100000,
	};

	return i2c_master_bus_add_device(bus_handle, &dev_cfg, &codec_dev_handle);
}

/*
 * reset I2C bus
 */
void eb_nau88c22_i2c_reset(void)
{
	/* doesn't do anything ATM */
}

/*
 * codec write access
 */
esp_err_t eb_nau88c22_write(uint8_t addr, uint16_t data)
{
    esp_err_t status = ESP_OK;
    uint8_t i2c_msg[2];

	/* Assemble 2-byte data in nau88c22 format */
    i2c_msg[0] = ((addr<<1)&0xFE) | ((data>>8)&0x01);
	i2c_msg[1] = data&0xFF;

	/* send it */
    status = i2c_master_transmit(codec_dev_handle, i2c_msg, sizeof(i2c_msg), -1);

	/* Check the communication status */
	if(status != ESP_OK)
	{
		/* Reset the I2C communication bus */
		eb_nau88c22_i2c_reset();
	}

	return status;
}

/*
 * codec read access
 */
esp_err_t eb_nau88c22_read(uint8_t addr, uint16_t *data)
{
    esp_err_t status = ESP_OK;
    uint8_t i2c_msg[1];

	/* Assemble 1-byte address in nau88c22 format */
    i2c_msg[0] = ((addr<<1)&0xFE);

	/* send addr & receive data */
    status = i2c_master_transmit_receive(codec_dev_handle, i2c_msg,
		sizeof(i2c_msg), (uint8_t *)data, sizeof(uint16_t), -1);

	/* Check the communication status */
	if(status != ESP_OK)
	{
		/* Reset the I2C communication bus */
		eb_nau88c22_i2c_reset();
	}

	return status;
}

/*
 * codec reset
 */
int32_t eb_nau88c22_Reset(void)
{
	int32_t result = 0;
	uint16_t idx = 0, reg, val;

	/* Load default values from init data table */
	while((reg = codec_settings[2*idx]) != 0x00FF)
	{
		val = codec_settings[2*idx + 1];
		if(eb_nau88c22_write(reg, val) != ESP_OK)
		{
			ESP_LOGI(TAG, "eb_nau88c22_Reset(): Write Addr 0x%02X failed", reg);
			result++;
		}
		else
		{
			ESP_LOGI(TAG, "Codec_Config(): Write Addr 0x%02X data 0x%03X", reg, val);
		}
		idx++;
	}
	
	return result;
}
