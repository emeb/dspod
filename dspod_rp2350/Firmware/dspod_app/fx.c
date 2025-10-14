/*
 * fx.c - Algorithm access for dspod rp2350
 * 10-7-25 E. Brombaugh
 */
 
#include <string.h>
#include "main.h"
#include "hardware/structs/xip.h"
#include "audio.h"
#include "fx.h"
#include "fx_vca.h"
#include "fx_cdl.h"
#include "fx_filters.h"

/*
 * overlay addresses from the linker
 * Effects are assigned to different overlays by type:
 * Overlay 0: Filter effects FX_OVL_IDX_FILT
 *  - vca
 *  - filters
 *  - freq shift
 *  - phaser
 * Overlay 1: Delay effects FX_OVL_IDX_DLY
 *  - clean delay
 */
extern uint32_t _ovly_table[3][3];

/* external memory buffer */
int16_t *fx_ext_buffer;
size_t fx_ext_sz;

/* 256kB pre-allocated internal memory for DSP */
size_t fx_int_sz;
uint32_t *fx_mem;

/* pointer to the fx data structure is void and recast inside the fns */
void *fx;

/* currently active algo */
uint8_t fx_algo;

/* currently active overlay */
uint8_t fx_curr_ovly;

/* interlock for overlay safety */
volatile uint32_t fx_ovl_interlock;


/**************************************************************************/
/******************* Bypass algo definition *******************************/
/**************************************************************************/

const char *bypass_param_names[] =
{
	"CV1",
	"CV2",
	"CV3",
};

/*
 * Bypass init
 */
void * fx_bypass_Init(uint32_t *mem)
{
	/* no init - just return pointer */
	return (void *)mem;
}

/*
 * Bypass Cleanup
 */
void fx_bypass_Cleanup(void *dummy)
{
	/* does nothing */
}

/*
 * Bypass audio process is just in-out loopback / bypass
 */
void fx_bypass_Proc(void *dummy, int16_t *dst, int16_t *src, uint16_t sz)
{
	/* interlock w/ fx foreground to indicate safe to modify overlay */
	if(fx_ovl_interlock == 0xDEADBEEF)
		fx_ovl_interlock = 0xABADCAFE;
	
	/* just copy in to out */
	while(sz--)
	{
		*dst++ = *src++;
		*dst++ = *src++;
	}
}

/*
 * Bypass render parameter is just simple percentage
 */
void fx_bypass_Render_Parm(void *vblk, uint8_t idx, GFX_RECT *rect, uint8_t init)
{
	static int16_t prev_val[3] = {-1, -1, -1};
	int16_t val;
	char *pname = fx_get_parm_name(idx), txtbuf[32];
	
	/* only first 3 ADC values are for general use */
	if(idx > 2)
		return;

	if(init)
	{
		/* clear param region and update param name */
		gfx_clrrect(rect);
		if(strlen(pname))
		{
			gfx_drawstrctr((rect->x0+rect->x1)/2, rect->y1-16, pname);
		}
		prev_val[0] = -1;
		prev_val[1] = -1;
		prev_val[2] = -1;
	}
	else if(strlen(pname))
	{
		val = ADC_val[idx]/41;
		if(val != prev_val[idx])
		{
			sprintf(txtbuf, "%2d%% ", val);
			prev_val[idx] = val;
			gfx_drawstrctr((rect->x0+rect->x1)/2, rect->y1-6, txtbuf);
		}
	}
}

/*
 * Bypass init
 */
const fx_struct fx_bypass_struct =
{
	"Bypass",
	0,
	bypass_param_names,
	FX_OVL_IDX_NONE,
	fx_bypass_Init,
	fx_bypass_Cleanup,
	fx_bypass_Proc,
	fx_bypass_Render_Parm,
};


/**************************************************************************/

/* array of effect structures */
const fx_struct *effects[FX_NUM_ALGOS] = {
	&fx_bypass_struct,
	&fx_vca_struct,
	&fx_cdr_struct,
	&fx_lpf_struct,
	&fx_hpf_struct,
	&fx_bpf_struct,
};

/*
 * load overlay to CCMRAM
 */
void fx_load_overlay(int idx)
{
	printf("fx_load_overlay: Loading Overlay %d from 0x%08X to 0x%08X, sz 0x%08x\n\r", 
		idx, _ovly_table[idx][2], _ovly_table[idx][0], _ovly_table[idx][1]);

	memcpy((uint8_t *)_ovly_table[idx][0], (uint8_t *)_ovly_table[idx][2], _ovly_table[idx][1]);
}
/*
 * initialize the effects library
 */
uint8_t fx_init(void)
{
	/* allocate 129kB internal buffer memory */
	printf("fx_init: attempt to allocate %d bytes internal RAM for audio...", FX_MAX_MEM);
	fx_mem = malloc(FX_MAX_MEM);
	if(fx_mem)
		printf("Success!\n");
	else
	{
		printf("Failed!!!\n");
		while(1){}
	}
	
	/* set up 8MB external buffer memory */
    gpio_set_function(8, GPIO_FUNC_XIP_CS1); // CS for PSRAM on dspod
    xip_ctrl_hw->ctrl|=XIP_CTRL_WRITABLE_M1_BITS;
	printf("PSRAM CS enabled and set writable.\n\r");
	fx_ext_sz = FX_EXT_MEM;
	fx_ext_buffer = (int16_t *)0x11000000;
	
	/* start off with bypass algo */
	fx_algo = 0;
	fx = effects[fx_algo]->init(fx_mem);
	fx_curr_ovly = FX_OVL_IDX_NONE;
	fx_ovl_interlock = 0;
	
	return 0;
}

/*
 * switch algorithms
 */
void fx_select_algo(uint8_t algo)
{
	uint8_t prev_fx_algo;
	
	/* only legal algorithms */
	if((algo >= FX_NUM_ALGOS) || (algo == fx_algo))
		return;
	
	/* mute during transition */
	Audio_Mute(1);
	
	/* bypass during init */
	prev_fx_algo = fx_algo;
	fx_algo = 0;
	
	/* start interlock & wait for bypass to start */
	fx_ovl_interlock = 0xDEADBEEF;
	while(fx_ovl_interlock != 0xABADCAFE) {}
	
	/* cleanup previous effect */
	effects[prev_fx_algo]->cleanup(fx);
	
	/* check if new effect is in overlay and fetch it */
	if(effects[algo]->overlay_index != FX_OVL_IDX_NONE)
	{
		/* don't reload current overlay */
		if(fx_curr_ovly != effects[algo]->overlay_index)
		{
			fx_load_overlay(effects[algo]->overlay_index);
			fx_curr_ovly = effects[algo]->overlay_index;
		}
	}
	
	/* init next effect from effect array */
	fx = effects[algo]->init(fx_mem);
		
	/* switch to next effect */
	fx_algo = algo;
	
	/* unmute */
	Audio_Mute(0);
}

/*
 * process audio through current effect
 */
void __not_in_flash_func(fx_proc)(int16_t *dst, int16_t *src, uint16_t sz)
{
	/* use effect structure function pointers */
	effects[fx_algo]->proc(fx, dst, src, sz);
}

/*
 * get current algorithm
 */
uint8_t fx_get_algo(void)
{
	return fx_algo;
}

/*
 * get number of params
 */
uint8_t fx_get_num_parms(void)
{
	return effects[fx_algo]->parms;
}

/*
 * get name of any effect
 */
char * fx_get_algo_name(uint8_t algo_num)
{
	return (char *)effects[algo_num]->name;
}

/*
 * get name of current effect
 */
char * fx_get_curr_algo_name(void)
{
	return (char *)effects[fx_algo]->name;
}

/*
 * get name of param by index
 */
char * fx_get_parm_name(uint8_t idx)
{
	return (char *)effects[fx_algo]->parm_names[idx];
}

/*
 * render parameter parts
 */
void fx_render_parm(uint8_t idx, uint8_t init)
{
	char *pname = fx_get_parm_name(idx);
	
	/* four 80x80 rects spaced across the display */
	GFX_RECT rect =
	{
		.x0 = idx*80,
		.y0 = 70,
		.x1 = idx*80 + 79,
		.y1 = 129
	};
		
	effects[fx_algo]->render_parm(fx, idx, &rect, init);
}



