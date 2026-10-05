/*
 * gc9a01_drvr.h - interface routines for st7789 LCD.
 * 01-17-26 E. Brombaugh - ported from GC9A01_drvr
 */

#ifndef __st7789_drvr__
#define __st7789_drvr__

#include "gfx.h"

// dimensions for LCD on tiny TFT wing
#define ST7789_TFTWIDTH 320
#define ST7789_TFTHEIGHT 170

extern GFX_DRIVER st7789_drvr;

void st7789_init(void);
void st7789_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void st7789_drawPixel(int16_t x, int16_t y, uint16_t color);
void st7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	uint16_t color);
uint16_t st7789_Color565(uint32_t rgb24);
uint32_t st7789_ColorRGB(uint16_t color16);
void st7789_bitblt(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *buf);
void st7789_setRotation(uint8_t m);
void st7789_setBacklight(uint8_t ena);

#endif
