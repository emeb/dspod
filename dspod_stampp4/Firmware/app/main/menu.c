/*
 * menu.c - menuing UI for dspod_esp32s3
 * 10-06-25 E. Brombaugh copied from dspod_rp2350
 */

#include <stdio.h>
#include "main.h"
#include "menu.h"
#include "gfx.h"
#include "widgets.h"
#include "pot_widget.h"
#include "audio.h"
#include "encoder.h"
#ifdef MULTICORE
#include "multicore_audio.h"
#endif
#include "fx.h"

#define MENU_XMAX 319
#define MENU_YMAX 169
#define MENU_CV_WIDTH 50
#define MENU_VU_WIDTH 50

static uint8_t menu_reset;
static int8_t menu_next_algo, menu_curr_algo;
static pot_widget_blk pot_widgets[4];

char txtbuf[32];

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
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2-8, "ESP32S3");
	gfx_set_txtscale(1);
	sprintf(txtbuf, "Version %s", swVersionStr);
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2+16, txtbuf);
	sprintf(txtbuf, "%s %s", bdate, btime);
	gfx_drawstrctr((rect.x0+rect.x1)/2, (rect.y0+rect.y1)/2+32, txtbuf);
}

/*
 * display CPU load on change
 */
void menu_show_cpuload(void)
{
	static uint8_t prev_load = 255;
	uint8_t curr_load = Audio_cpu_load();
	
	if(curr_load != prev_load)
	{
		sprintf(txtbuf, "Load: %2u%% ", curr_load);
		gfx_drawstr(10, 4, txtbuf);
		prev_load = curr_load;
	}
}

/* CV bargraph coords */
int16_t cv_coords[] =
{
	MENU_XMAX/2-5-MENU_CV_WIDTH, 140,
	MENU_XMAX/2-5-MENU_CV_WIDTH, 150,
	MENU_XMAX/2+5, 140,
	MENU_XMAX/2+5, 150,
};

/*
 * render CV indicators on change
 */
void menu_render_cvs(void)
{
	static int16_t prev_cv[4] = {-1, -1, -1, -1};
	int16_t curr_cv, i;
	
	for(i=0;i<4;i++)
	{
		curr_cv = ADC_val[i]/41;
		if(curr_cv != prev_cv[i])
		{
			widg_bargraphH(cv_coords[2*i], cv_coords[2*i+1], MENU_CV_WIDTH, 8, curr_cv);		
			prev_cv[i] = curr_cv;
		}
	}
}

/*
 * render W/D indicator on change
 */
void menu_render_wetdry(void)
{
	sprintf(txtbuf, "%2d%%", ADC_val[3]/41);
	pot_widget_update(&pot_widgets[3], ADC_val[3], txtbuf);
}

/* VU meter coords */
int16_t vu_coords[] = 
{
	30, 140,
	30, 150,
	MENU_XMAX-10-16-6-MENU_VU_WIDTH, 140,
	MENU_XMAX-10-16-6-MENU_VU_WIDTH, 150,
};

/*
 * render VU meters on change
 */
void menu_render_vu(void)
{
	static int16_t prev_vu[4] = {-1, -1, -1, -1};
	int16_t curr_vu, i;
	
	for(i=0;i<4;i++)
	{
		curr_vu = Audio_get_level(i)/328;
		if(curr_vu != prev_vu[i])
		{
			widg_bargraphHG(vu_coords[2*i], vu_coords[2*i+1], MENU_VU_WIDTH, 8, curr_vu);
			prev_vu[i] = curr_vu;
		}
	}
}

/*
 * redraw the menu
 */
void menu_render(void)
{
	uint8_t i;
	GFX_RECT rect;
	
	/* refresh static items */
	if(menu_reset)
	{
		menu_reset = 0;
		
		/* current algo name box */
		gfx_set_forecolor(GFX_BLUE);
		rect.x0 = MENU_XMAX/2 - 120;
		rect.y0 = 20;
		rect.x1 = MENU_XMAX/2 + 120;
		rect.y1 = 44;
		gfx_fillroundedrect(&rect, 24);
		gfx_set_forecolor(GFX_WHITE);
		gfx_set_backcolor(GFX_BLUE);
		gfx_set_txtscale(2);
		sprintf(txtbuf, "%2u: %s", fx_get_algo(), fx_get_curr_algo_name());
		gfx_drawstrctr(MENU_XMAX/2, 32, txtbuf);
		gfx_set_txtscale(1);

		/* set constants */
		gfx_set_backcolor(GFX_DGRAY);
		
		/* init fx params */
		for(i=0;i<3;i++)
		{
			fx_render_parm(&pot_widgets[i], 1);
		}
		
		/* wet/dry label */
		pot_widget_redraw(&pot_widgets[3], "W/D Mix", "", 1);

		/* CV indicators */
		gfx_drawstr(MENU_XMAX/2-MENU_CV_WIDTH-6-16, 141, "C0");
		gfx_drawstr(MENU_XMAX/2-MENU_CV_WIDTH-6-16, 151, "C1");
		gfx_drawstr(MENU_XMAX/2+5+MENU_CV_WIDTH+2, 141, "C2");
		gfx_drawstr(MENU_XMAX/2+5+MENU_CV_WIDTH+2 , 151, "C3");
	
		/* vu meters labels and boxes */
		gfx_drawstr(10, 141, "il");
		gfx_drawstr(10, 151, "ir");
		gfx_drawstr(MENU_XMAX-10-16, 141, "ol");
		gfx_drawstr(MENU_XMAX-10-16, 151, "or");
	}
	
	/* update dynamic items */
	menu_show_cpuload();

	/* update algo params */
	gfx_set_backcolor(GFX_DGRAY);
	gfx_set_txtscale(1);
	for(i=0;i<3;i++)
	{
		fx_render_parm(&pot_widgets[i], 0);
	}
			
	menu_render_wetdry();
	menu_render_cvs();
	menu_render_vu();
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
	
	/* create VU gradient */
	widg_gradient_init(MENU_VU_WIDTH);
	
	menu_reset = 1;
	menu_curr_algo = menu_next_algo = 0;
	
	/* four pot widgets in 80x90 rects spaced across the display */
	for(int i=0;i<4;i++)
	{
		GFX_RECT rect;
		rect.x0 = i*80;
		rect.y0 = 54;
		rect.x1 = i*80 + 79;
		rect.y1 = 133;
		
		pot_widget_create(&pot_widgets[i], i, &rect, GFX_LGRAY, 35, 25, 4096);
	};
		
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
	
	// detect encoder changes
	if(encoder_poll(&enc_val, &enc_btn))
	{
		if(enc_val)
		{
			menu_next_algo += enc_val;
			menu_next_algo = menu_next_algo < 0 ? 0 : menu_next_algo;
			menu_next_algo = menu_next_algo >= FX_NUM_ALGOS ? FX_NUM_ALGOS-1 : menu_next_algo;
		
			/* next algo box */
			gfx_set_forecolor(GFX_MAGENTA);
			rect.x0 = MENU_XMAX/2 - 60;
			rect.y0 = 0;
			rect.x1 = MENU_XMAX/2 + 60;
			rect.y1 = 16;
			gfx_fillroundedrect(&rect, 16);
			
			/* algo selection */
			gfx_set_forecolor(GFX_WHITE);
			gfx_set_backcolor(GFX_MAGENTA);
			sprintf(txtbuf, "%2u: %s", menu_next_algo, fx_get_algo_name(menu_next_algo));
			gfx_set_txtscale(1);
			gfx_drawstrctr(MENU_XMAX/2, 8, txtbuf);
			gfx_set_backcolor(GFX_DGRAY);
		}
		
		if(enc_btn == 1)
		{
			/* erase next algo box */
			gfx_set_forecolor(GFX_DGRAY);
			rect.x0 = MENU_XMAX/2 - 60;
			rect.y0 = 0;
			rect.x1 = MENU_XMAX/2 + 60;
			rect.y1 = 16;
			gfx_fillrect(&rect);
			gfx_set_forecolor(GFX_WHITE);
			
			/* update current algo & redraw */
			menu_reset = 1;
			menu_curr_algo = menu_next_algo;
#ifdef MULTICORE
			multicore_audio_select_algo(menu_next_algo);
#else
			fx_select_algo(menu_next_algo);
#endif
			printf("menu_process: switched to fx algo %d\n\r", menu_next_algo);
		}
	}
	
	// update display
	menu_render();
}
