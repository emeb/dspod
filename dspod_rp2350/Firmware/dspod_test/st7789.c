/*
 * ST7789.c - interface routines for ST7789 LCD on dspod_module.
 * shamelessly ganked from Adafruit_ST7789 library
 * 08-12-19 E. Brombaugh
 * 10-21-20 E. Brombaugh - updated for f405_codec_v2
 * 10-28-20 E. Brombaugh - updated for f405 feather + tftwing
 * 10-11-21 E. Brombaugh - updated for RP2040
 * 09-12-22 E. Brombaugh - updated for 01Space LCD
 * 08-28-25 E. Brombaugh - updated for dspod_module
 */

#include "st7789.h"
#include "RP2350.h"

/* ----------------------- I/O definitions ----------------------- */

#define ST7789_SC_PIN 6
#define ST7789_SD_PIN 7
#define ST7789_BL_PIN 2
#define ST7789_CS_PIN 5
#define ST7789_DC_PIN 4
#define ST7789_RS_PIN 3

#define ST7789_CS_LOW()    gpio_put(ST7789_CS_PIN,0)
#define ST7789_CS_HIGH()   gpio_put(ST7789_CS_PIN,1)
#define ST7789_DC_CMD()    gpio_put(ST7789_DC_PIN,0)
#define ST7789_DC_DATA()   gpio_put(ST7789_DC_PIN,1)
#define ST7789_RS_LOW()    gpio_put(ST7789_RS_PIN,0)
#define ST7789_RS_HIGH()   gpio_put(ST7789_RS_PIN,1)
#define ST7789_BL_LOW()    gpio_put(ST7789_BL_PIN,0)
#define ST7789_BL_HIGH()   gpio_put(ST7789_BL_PIN,1)

#define ST_CMD            0x100
#define ST_CMD_DELAY      0x200
#define ST_CMD_END        0x400

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_COLMOD     0x3A
#define ST77XX_MADCTL     0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x08
#define ST77XX_MADCTL_MH  0x04


#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD


/* high level driver interface */
GFX_DRIVER ST7789_drvr =
{
	ST7789_TFTHEIGHT,
	ST7789_TFTWIDTH,
	ST7789_init,
	ST7789_setRotation,
    ST7789_Color565,
    ST7789_ColorRGB,
	ST7789_fillRect,
	ST7789_drawPixel,
	ST7789_bitblt
};

/* ----------------------- private variables ----------------------- */
// Initialization command sequence
const static uint16_t
  initlst[] = {
    ST77XX_SWRESET | ST_CMD,        //  1: Software reset, no args, w/delay
    ST_CMD_DELAY | 150,             //  150 ms delay
    ST77XX_SLPOUT | ST_CMD ,        //  2: Out of sleep mode, no args, w/delay
	ST_CMD_DELAY | 500,             //  500 ms delay
    ST77XX_COLMOD | ST_CMD ,        //  3: Set color mode
      0x55,                         //     16-bit color
	ST_CMD_DELAY | 10,              //     10 ms delay
    ST77XX_MADCTL | ST_CMD ,        //  4: Mem access ctrl (directions), 1 arg:
      0x00,                         //     Row/col addr, bottom-top refresh
    ST77XX_CASET | ST_CMD  ,        //  5: Column addr set, 4 args, no delay:
      0x00,
      0,                            //     XSTART = 0
      0,
      170,                          //     XEND = 240
    ST77XX_RASET | ST_CMD  ,        //  6: Row addr set, 4 args, no delay:
      0x00,
      0,                            //     YSTART = 0
      320>>8,
      320&0xff,                     //     YEND = 320
    ST77XX_INVON | ST_CMD  ,        //  7: hack
    ST_CMD_DELAY | 10,              //  10 ms
    ST77XX_NORON | ST_CMD  ,        //  8: Normal display on, no args, w/delay
    ST_CMD_DELAY | 10,              //  10 ms delay
    ST77XX_DISPON | ST_CMD ,        //  9: Main screen turn on, no args, delay
    ST_CMD_DELAY | 500,             //  500 ms delay
	ST_CMD_END                      //  END OF LIST
};

/* SPI port instance */
#define SPI_PORT spi0

/* LCD state */
uint8_t rowstart, colstart;
uint16_t _width, _height, rotation;

/* ----------------------- Private functions ----------------------- */
/*
 * Initialize SPI interface to LCD
 */
void ST7789_SPI_Init(void)
{
	/* set GPIO for LCD */
    gpio_set_function(ST7789_SC_PIN, GPIO_FUNC_SPI);
    gpio_set_function(ST7789_SD_PIN, GPIO_FUNC_SPI);
	gpio_init(ST7789_CS_PIN);
	gpio_set_dir(ST7789_CS_PIN, GPIO_OUT);
	gpio_put(ST7789_CS_PIN, 1);
	gpio_init(ST7789_DC_PIN);
	gpio_set_dir(ST7789_DC_PIN, GPIO_OUT);
	gpio_put(ST7789_DC_PIN, 0);
	gpio_init(ST7789_RS_PIN);
	gpio_set_dir(ST7789_RS_PIN, GPIO_OUT);
	gpio_put(ST7789_RS_PIN, 1);
	gpio_init(ST7789_BL_PIN);
	gpio_set_dir(ST7789_BL_PIN, GPIO_OUT);
	gpio_put(ST7789_BL_PIN, 0);
	
	/* init desired SPI port to 20MHz */
	spi_init(SPI_PORT, 20000000);
	
	/* set up formatting */
	spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

/*
 * send single byte via SPI - cmd or data depends on bit 8
 */
void ST7789_write_byte(uint16_t dat)
{
	uint8_t dat8;
	
	if((dat & ST_CMD) == ST_CMD)
		ST7789_DC_CMD();
	else
		ST7789_DC_DATA();

	ST7789_CS_LOW();

	dat8 = dat&0xff;
	spi_write_blocking(SPI_PORT, &dat8, 1);

	ST7789_CS_HIGH();
}

/* ----------------------- Public functions ----------------------- */
// Initialization for ST7789R red tab screens
void ST7789_init(void)
{
	// turn off the backlight
	ST7789_BL_LOW();
	
	// init the SPI port
	ST7789_SPI_Init();
	
#if 0
	// test spi formatting
	{
		uint16_t halfword = 0x1234;
		while(1)
			spi_write_blocking(SPI_PORT, (uint8_t *)&halfword, 2);
	}
#endif
	
	// Reset it
	ST7789_RS_LOW();
	sleep_ms(10);
	ST7789_RS_HIGH();
	sleep_ms(10);

	// Send init command list
	uint16_t *addr = (uint16_t *)initlst, ms;
	while(*addr != ST_CMD_END)
	{
		if((*addr & ST_CMD_DELAY) != ST_CMD_DELAY)
			ST7789_write_byte(*addr++);
		else
		{
			ms = (*addr++)&0x1ff;        // strip delay time (ms)
			sleep_ms(ms);
		}
	}
}

// opens a window into display mem for bitblt
void ST7789_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint8_t tx_buf[4];
	uint16_t sum;

	ST7789_write_byte(ST77XX_CASET | ST_CMD); // Column addr set
	sum = x0+colstart;
	tx_buf[0] = sum>>8;
	tx_buf[1] = sum&0xff;
	sum = x1+colstart;
	tx_buf[2] = sum>>8;
	tx_buf[3] = sum&0xff;
	ST7789_DC_DATA();
	ST7789_CS_LOW();
	spi_write_blocking(SPI_PORT, tx_buf, 4);
	ST7789_CS_HIGH();

	ST7789_write_byte(ST77XX_RASET | ST_CMD); // Row addr set
	sum = y0+rowstart;
	tx_buf[0] = sum>>8;
	tx_buf[1] = sum&0xff;
	sum = y1+rowstart;
	tx_buf[2] = sum>>8;
	tx_buf[3] = sum&0xff;
	ST7789_DC_DATA();
	ST7789_CS_LOW();
	spi_write_blocking(SPI_PORT, tx_buf, 4);
	ST7789_CS_HIGH();

	ST7789_write_byte(ST77XX_RAMWR | ST_CMD); // write to RAM
}

// draw single pixel
void ST7789_drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

	ST7789_setAddrWindow(x,y,x+1,y+1);

	ST7789_DC_DATA();
	ST7789_CS_LOW();
	
	spi_write_blocking(SPI_PORT, (uint8_t *)&color, 2);

	ST7789_CS_HIGH();
}

// fill a rectangle
void ST7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	uint16_t color)
{
	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= _width) || (y >= _height)) return;
	if((x + w - 1) >= _width)  w = _width  - x;
	if((y + h - 1) >= _height) h = _height - y;

	ST7789_setAddrWindow(x, y, x+w-1, y+h-1);
	
	w*=h;
	
	/* prep tos end data */
	ST7789_DC_DATA();
	ST7789_CS_LOW();

	/* faster version keeps pipes full */
	while(w--)
		spi_write_blocking(SPI_PORT, (uint8_t *)&color, 2);

	ST7789_CS_HIGH();
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t ST7789_Color565(uint32_t rgb24)
{
	uint16_t color16;
	color16 = 	(((rgb24>>16) & 0xF8) << 8) |
				(((rgb24>>8) & 0xFC) << 3) |
				((rgb24 & 0xF8) >> 3);
	
	return __REVSH(color16);
}

// Pass 16-bit packed color, get back 8-bit (each) R,G,B in 32-bit
uint32_t ST7789_ColorRGB(uint16_t color16)
{
    uint32_t r,g,b;

	color16 = __REVSH(color16);
	
    r = (color16 & 0xF800)>>8;
    g = (color16 & 0x07E0)>>3;
    b = (color16 & 0x001F)<<3;
	return (r<<16) | (g<<8) | b;
}

// bitblt a region to the display
void ST7789_bitblt(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *buf)
{
	ST7789_setAddrWindow(x, y, x+w-1, y+h-1);

	ST7789_DC_DATA();
	ST7789_CS_LOW();

	/* PIO buffer send */
	spi_write_blocking(SPI_PORT, (uint8_t *)buf, 2*w*h);

	ST7789_CS_HIGH();
}

// set orientation of display
void ST7789_setRotation(uint8_t m)
{
	ST7789_write_byte(ST77XX_MADCTL | ST_CMD);
	rotation = m % 4; // can't be higher than 3
	switch (rotation)
	{
		case 0:
			ST7789_write_byte(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY );
			_width  = ST7789_TFTWIDTH;
			_height = ST7789_TFTHEIGHT;
			rowstart = ST7789_ROWSTART;
			colstart = ST7789_COLSTART;
			break;

		case 1:
			ST7789_write_byte(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV );
			_width  = ST7789_TFTHEIGHT;
			_height = ST7789_TFTWIDTH;
			rowstart = ST7789_COLSTART;
			colstart = ST7789_ROWSTART;
			break;

		case 2:
			ST7789_write_byte(0);
			_width  = ST7789_TFTWIDTH;
			_height = ST7789_TFTHEIGHT;
			rowstart = ST7789_ROWSTART;
			colstart = ST7789_COLSTART;
			break;

		case 3:
			ST7789_write_byte(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV );
			_width  = ST7789_TFTHEIGHT;
			_height = ST7789_TFTWIDTH;
			rowstart = ST7789_COLSTART;
			colstart = ST7789_ROWSTART;
			break;
	}
	
	/* adjust the limits */
	ST7789_drvr.xmax = _width;
	ST7789_drvr.ymax = _height;
}

// backlight on/off
void ST7789_setBacklight(uint8_t ena)
{
    gpio_put(ST7789_BL_PIN, ena);
}
