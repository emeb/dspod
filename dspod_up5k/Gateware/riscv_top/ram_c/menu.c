/*
 * menu.c - UI menu logic for dspod_up5k
 * 06-16-2026 E. Brombaugh
 */
#include "menu.h"
#include "button.h"
#include "st7789.h"
#include "fx.h"
#include "widgets.h"
#include "dsp_lib.h"
#include <string.h>

#define MENU_XMAX 319
#define MENU_YMAX 169
#define MENU_CV_WIDTH 50
#define MENU_VU_WIDTH 50

const char *cv_labels[4] =
{
	"ROM",
	"---",
	"---",
	"Mix",
};

const char *rom_lables[4] = 
{
	"MV1",
	"MFx",
	"Spc",
	"MV2"
};

const int16_t rom_algos[4] =
{
	64,
	64,
	64,
	99,
};

char textbuf[32];
uint16_t prev_cv[4], rom;
static uint8_t menu_reset;
static int16_t menu_next_algo, menu_curr_algo, base_algo;

/*
 * Draw splash screen
 */
void menu_splash(const char *swVersionStr, const char *bdate, const char *btime)
{
	GFX_RECT rect;

	rect.x0 = 2;
	rect.y0 = 2;
	rect.x1 = 317;
	rect.y1 = 167;
	gfx_fillroundedrect(&rect, 20);
	rect.x0 = 40;
	rect.y0 = 40;
	rect.x1 = 279;
	rect.y1 = 130;
	gfx_set_forecolor(GFX_BLUE);
	gfx_fillroundedrect(&rect, 20);
	gfx_set_backcolor(GFX_BLUE);
	gfx_set_forecolor(GFX_WHITE);
	gfx_set_txtscale(2);
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2-32, "DSPOD");
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2-8, "UP5k");
	gfx_set_txtscale(1);
	sprintf(textbuf, "Version %s", swVersionStr);
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2+16, textbuf);
	sprintf(textbuf, "%s %s", bdate, btime);
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2+32, textbuf);
}

/*
 * redraw the menu
 */
void menu_render(void)
{
	//uint8_t i;
	GFX_RECT rect;
	char textbuf[32];
	
	/* refresh static items */
	if(menu_reset)
	{
		menu_reset = 0;
		
		/* current algo name box */
		gfx_set_forecolor(GFX_BLUE);
		rect.x0 = MENU_XMAX/2 - 120;
		rect.y0 = 36;
		rect.x1 = MENU_XMAX/2 + 120;
		rect.y1 = 60;
		gfx_fillroundedrect(&rect, 24);
		gfx_set_forecolor(GFX_WHITE);
		gfx_set_backcolor(GFX_BLUE);
		gfx_set_txtscale(2);
		sprintf(textbuf, "%2u: %s", fx_get_algo() - base_algo + 1, fx_get_curr_algo_name());
		gfx_drawstrctr(MENU_XMAX/2, 48, textbuf);
		gfx_set_txtscale(1);

		/* set constants */
		gfx_set_backcolor(GFX_DGRAY);
		
		/* vu meters labels an */
		gfx_drawstr(10, 141, "il");
		gfx_drawstr(10, 151, "ir");
		gfx_drawstr(MENU_XMAX-10-16, 141, "ol");
		gfx_drawstr(MENU_XMAX-10-16, 151, "or");
	}
	
	/* update CVs */
	gfx_set_backcolor(GFX_DGRAY);
	for(int i=0;i<4;i++)
	{
		uint16_t val = ADC_get_chl(i);
		if(val != prev_cv[i])
		{
			prev_cv[i] = val;
			if(i==0)
			{
				/* quantize CV to number of ROMs */
				dsp_ratio_hyst_arb(&rom, val, 3);
				strcpy(textbuf, rom_lables[rom]);
				
				/* compute algo offset for ROM select */
				base_algo = 0;
				for(int j=0;j<rom;j++)
					base_algo += rom_algos[j];
			}
			else
				sprintf(textbuf, "%2d", val/41);
			
			int16_t x = i*80 + 40;
			gfx_drawarc(x, 100, 30, 512, 512+((val*6)>>3));
			gfx_set_txtscale(2);
			gfx_drawstrctr(x, 100, textbuf);
			gfx_set_txtscale(1);
			gfx_drawstrctr(x, 130, (char *)cv_labels[i]);
		}
	}
	
	/* update levels */
	widg_bargraphHG(30, 140, MENU_VU_WIDTH, 8, LVL->L_RX/328);
	widg_bargraphHG(MENU_XMAX-10-16-6-MENU_VU_WIDTH, 140, MENU_VU_WIDTH, 8, LVL->L_TX/328);
	widg_bargraphHG(30, 150, MENU_VU_WIDTH, 8, LVL->R_RX/328);
	widg_bargraphHG(MENU_XMAX-10-16-6-MENU_VU_WIDTH, 150, MENU_VU_WIDTH, 8, LVL->R_TX/328);
}

/*
 * init the menu state
 */
void menu_init(void)
{
	/* wipe screen */
	gfx_set_backcolor(GFX_DGRAY);
	gfx_set_forecolor(GFX_WHITE);
	gfx_clrscreen();
	
	/* set state */
	menu_reset = 1;
	menu_curr_algo = menu_next_algo = 0;
	rom = 0;
	base_algo = 0;
	for(int i=0;i<4;i++)
		prev_cv[i] = 4096;
	
	/* create VU gradient */
	widg_gradient_init(MENU_VU_WIDTH);
	
	/* first draw */
	menu_render();
}

/*
 * process menu events
 */
void menu_process(void)
{
	int16_t enc_val;
	uint8_t enc_btn;
	GFX_RECT rect;
	char textbuf[32];
	
	// detect encoder changes
	if(encoder_poll(&enc_val, &enc_btn))
	{
		if(enc_val)
		{
			menu_next_algo += enc_val;
			menu_next_algo = menu_next_algo < 0 ? 0 : menu_next_algo;
			menu_next_algo = menu_next_algo >= rom_algos[rom] ? rom_algos[rom]-1 : menu_next_algo;
		
			/* next algo box */
			gfx_set_forecolor(GFX_MAGENTA);
			rect.x0 = MENU_XMAX/2 - 60;
			rect.y0 = 16;
			rect.x1 = MENU_XMAX/2 + 60;
			rect.y1 = 32;
			gfx_fillroundedrect(&rect, 16);
						
			/* algo selection */
			gfx_set_forecolor(GFX_WHITE);
			gfx_set_backcolor(GFX_MAGENTA);
			sprintf(textbuf, "%2u: %s", menu_next_algo + 1, fx_get_algo_name(base_algo+menu_next_algo));
			gfx_set_txtscale(1);
			gfx_drawstrctr(MENU_XMAX/2, 24, textbuf);
			gfx_set_backcolor(GFX_DGRAY);
		}
		
		if(enc_btn == 1)
		{
			/* erase next algo box */
			gfx_set_forecolor(GFX_DGRAY);
			rect.x0 = MENU_XMAX/2 - 60;
			rect.y0 = 16;
			rect.x1 = MENU_XMAX/2 + 60;
			rect.y1 = 32;
			gfx_fillrect(&rect);
			gfx_set_forecolor(GFX_WHITE);
			
			/* update current algo & redraw */
			menu_reset = 1;
			menu_curr_algo = menu_next_algo;
			fx_select_algo(base_algo + menu_next_algo);
			printf("menu_process: switched to fx algo %d\n\r", base_algo + menu_next_algo);
		}
	}
	
	// update display
	menu_render();
}
