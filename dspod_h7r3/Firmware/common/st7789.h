/*
 * ST7789.h - interface routines for ST7789 LCD.
 * shamelessly ganked from Adafruit_ST7789 library
 * 08-12-19 E. Brombaugh
 */

#ifndef __ST7789__
#define __ST7789__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7rsxx_hal.h"
#include "gfx.h"

// dimensions
#define ST7789_TFTWIDTH 170
#define ST7789_TFTHEIGHT 320
#define ST7789_ROWSTART 0
#define ST7789_COLSTART 35

extern GFX_DRIVER ST7789_drvr;

void ST7789_init(void);
void ST7789_backlight(uint8_t state);
void ST7789_setRotation(uint8_t m);
void ST7789_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_fillScreen(uint16_t color);
void ST7789_drawPixel(int16_t x, int16_t y, uint16_t color);
void ST7789_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
	uint16_t color);
uint16_t ST7789_Color565(uint32_t rgb24);
uint32_t ST7789_ColorRGB(uint16_t color16);
void ST7789_bitblt(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *buf);

#ifdef __cplusplus
}
#endif

#endif
