/*
 * fx.c - Algorithm access for dspod rp2350
 * 10-7-25 E. Brombaugh
 */
 
#include <string.h>
#include "main.h"
#include "audio.h"
#include "fx.h"
#include "fx_vca.h"
#include "fx_filters.h"
#include "fx_cdl.h"
#include "fx_cdl_ext.h"

/* 256kB pre-allocated internal memory for DSP */
size_t fx_int_sz = FX_MAX_MEM/sizeof(uint32_t);
uint32_t fx_mem[FX_MAX_MEM/sizeof(uint32_t)] __attribute__ ((section (".ram_data")));

/* pointer to the fx data structure is void and recast inside the fns */
void *fx;

/* currently active algo */
uint8_t fx_algo;

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
void __attribute__ ((section (".itcm_code"))) fx_bypass_Proc(void *dummy, int16_t *dst, int16_t *src, uint16_t sz)
{
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
	&fx_lpf_struct,
	&fx_hpf_struct,
	&fx_bpf_struct,
	&fx_cdr_struct,
	&fx_cdx_struct,
};

/*
 * initialize the effects library
 */
uint8_t fx_init(void)
{	
	/* start off with bypass algo */
	fx_algo = 0;
	fx = effects[fx_algo]->init(fx_mem);
	
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
	
	/* cleanup previous effect */
	effects[prev_fx_algo]->cleanup(fx);
		
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
void fx_proc(int16_t *dst, int16_t *src, uint16_t sz)
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



