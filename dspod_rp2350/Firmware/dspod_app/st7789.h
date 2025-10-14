/*
 * ST7789.h - interface routines for ST7789 LCD.
 * shamelessly ganked from Adafruit_ST7789 library
 * 08-28-20 E. Brombaugh
 * 10-21-20 E. Brombaugh - updated for f405_codec_v2
 * 10-28-20 E. Brombaugh - updated for f405 feather + tftwing
 * 10-11-21 E. Brombaugh - updated for RP2040
 */

#ifndef __ST7789__
#define __ST7789__

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "gfx.h"

// dimensions for LCD on dsod_module
#define ST7789_TFTWIDTH 170
#define ST7789_TFTHEIGHT 320
#define ST7789_ROWSTART 0
#define ST7789_COLSTART 35

extern GFX_DRIVER ST7789_drvr;

void ST7789_init(void);
void ST7789_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_drawPixel(int16_t x, int16_t y, uint16_t color);
void ST7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	uint16_t color);
uint16_t ST7789_Color565(uint32_t rgb24);
uint32_t ST7789_ColorRGB(uint16_t color16);
void ST7789_bitblt(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *buf);
void ST7789_setRotation(uint8_t m);
void ST7789_setBacklight(uint8_t ena);

#endif
