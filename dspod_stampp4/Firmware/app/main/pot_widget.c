/*
 * widgets.c - UI graphics widgets for dspod
 * 03-20-22 E. Brombaugh
 */
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "gfx.h"
#include "menu.h"
#include "pot_widget.h"
#include "int_math.h"

/*
 * initialize a pot widget location, style, etc.
 */
void pot_widget_create(pot_widget_blk *pw, uint8_t idx, GFX_RECT *loc,
	GFX_COLOR trkcol, uint16_t ro, uint16_t ri, uint16_t range)
{
	/* set the index */
	pw->idx = idx;
	
	/* copy the location rect */
	memcpy(&(pw->rect), loc, sizeof(GFX_RECT));
	
	/* the pot track background color - converted to HW color */
	pw->tcolor = gfx_getcolor(trkcol);
	
	/* save the range */
	pw->range = range;
	
	/* init the track angles */
	pw->a2 = 1*INT_2PI/10;
	pw->a3 = 9*INT_2PI/10;
	
	/* initial state is invisible */
	pw->vis = 0;
	
	/* compute the arc params */
	pw->cx = (pw->rect.x0+pw->rect.x1)/2;
	pw->cy = (pw->rect.y0+pw->rect.y1)/2 - 4;
	pw->ro = ro;
	pw->ri = ri;
}

/*
 * change a pot widget labels, visibility, etc.
 */
void pot_widget_redraw(pot_widget_blk *pw, char *label0, char *label1,
	uint8_t vis)
{
	
	/* erase the whole widget */
	gfx_clrrect(&pw->rect);
	
	/* save visibility */
	pw->vis = vis;
	
	/* save labels */
	strncpy(pw->label[0], label0, 31);
	strncpy(pw->label[1], label1, 31);
	
	/* draw the labels */
	if(pw->vis)
	{
		/* bottom label */
		if(strlen(pw->label[0]))
		{
			gfx_drawstrctr(pw->cx, pw->rect.y1-4, pw->label[0]);
		}
		
		/* units render below the center text value */
		if(strlen(pw->label[1]))
		{
			gfx_drawstrctr(pw->cx, pw->cy+8, pw->label[1]);
		}
	}
	
	/* set current value out of range to update on first call */
	pw->curr_val = pw->range+1;
}

/*
 * update a pot widget value2
 */
void pot_widget_update(pot_widget_blk *pw, uint16_t value, char *valtxt)
{
	if((pw->vis) && (value != pw->curr_val))
	{
		pw->curr_val = value;
		gfx_drawstrctr(pw->cx, pw->cy, valtxt);
		
		int a0 = pw->a3 - ((value * 8*INT_2PI/10) / pw->range);
		int a1 = pw->a3;
#if 0
		gfx_draw_arc_segment(pw->cx, pw->cy, pw->ri, pw->ro, a0, a1);
#else
		gfx_draw_arc_segment2(pw->cx, pw->cy, pw->ri, pw->ro, a0, a1,
			pw->a2, pw->a3, pw->tcolor);
#endif
	}
}


