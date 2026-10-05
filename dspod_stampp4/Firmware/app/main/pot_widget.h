/*
 * pot_widget.h - UI graphics widget for potentiometers
 * 08-15-26 E. Brombaugh
 */

#ifndef __pot_widget__
#define __pot_widget__

#include <stdint.h>
#include "gfx.h"

typedef struct
{
	GFX_RECT rect;
	int16_t tcolor;
	uint16_t range;
	uint8_t idx, vis;
	uint16_t cx, cy, ro, ri;
	int a2, a3;
	char label[2][32];
	uint16_t curr_val;
} pot_widget_blk;

void pot_widget_create(pot_widget_blk *pw, uint8_t idx, GFX_RECT *loc,
	GFX_COLOR trkcol, uint16_t ro, uint16_t ri, uint16_t range);
void pot_widget_redraw(pot_widget_blk *pw, char *label0, char *label1,
	uint8_t vis);
void pot_widget_update(pot_widget_blk *pw, uint16_t value, char *valtxt);

#endif