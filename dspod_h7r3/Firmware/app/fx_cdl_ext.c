/*
 * fx_cdl.c -  Clean Delay effect with PSRAM buffer for dspod_h7r3
 * 11-19-25 E. Brombaugh
 */
 
#include "fx_cdl_ext.h"
#include "xspi.h"

/* uncomment this for psram buffering */
#define USE_PSRAMBUF

#ifdef USE_PSRAMBUF
#include "psram_buff.h"
#endif

#define XFADE_BITS 11

typedef struct 
{
#ifdef USE_PSRAMBUF
	psbuf_struct wpsb, r1psb, r2psb;	/* PSRAM buffers */
#endif
	uint8_t rng;			/* short/med/long range */
	uint16_t rng_raw;		/* raw range from ADC param */
	uint32_t len;			/* buffer length */
	uint8_t init;			/* initial pass flag for cleanout */
	uint32_t wptr;			/* write pointer */
	uint32_t roff1, roff2;	/* read offsets - main and xfade */
	uint16_t xflen, xfcnt;	/* Cross-fade length and counter */
	int16_t dly;			/* delay value w/ hysteresis */
	int32_t dcb[2];			/* dc block on feedback */
	int16_t fb[2];
} fx_cdx_blk;

const char *cdx_param_names[] =
{
	"DlyAmt",
	"Feedbk",
	"Range ",
};

const char *cdx_ranges[] =
{
	"Short",
	"Medium",
	"Long",
};

/*
 * Clean Delay common init
 */
void * fx_cdx_Init(uint32_t *mem)
{
	/* set up instance in mem area provided */
	fx_cdx_blk *blk = (fx_cdx_blk *)mem;
	mem += sizeof(fx_cdx_blk)/sizeof(uint32_t);

#ifdef USE_PSRAMBUF
	/* initialize PSRAM buffers for write, read & crossfade */
#define PSRAM_BUFSZ 64
	mem += psbuf_init(&blk->wpsb, mem, PSRAM_BUFSZ, 0, XSPI_SIZE);
	mem += psbuf_init(&blk->r1psb, mem, PSRAM_BUFSZ, 0, XSPI_SIZE);
	mem += psbuf_init(&blk->r2psb, mem, PSRAM_BUFSZ, 0, XSPI_SIZE);
#endif
	
	/* set type / range */
	blk->rng = 2;
	blk->rng_raw = 0;
	
	/* init delay buffering */
	blk->len = XSPI_SIZE / (2*sizeof(int16_t));	// length in samples
	blk->init = 1;
	blk->wptr = 0;
#ifdef USE_PSRAMBUF
	blk->roff1 = PSRAM_BUFSZ;
#else
	blk->roff1 = 1;
#endif
	blk->roff2 = 0;
	blk->xfcnt = 0;
	blk->xflen = 1<<XFADE_BITS;
	blk->dly = 0;
	blk->dcb[0] = blk->dcb[1] = 0;
	blk->fb[0] = blk->fb[1] = 0;
	
	printf("fx_cdx_Init -------\n\r");
	printf("\tXSPI_SIZE = %d, len = 0x%08X\n\r", XSPI_SIZE, blk->len);
	for(int i=0;i<3;i++)
		printf("\trng = %d, maxlen =  0x%08X\n\r", i, 4095<<(5+2*i));

	/* return pointer */
	return (void *)blk;
}

/*
 * Clean Delay audio process
 */
void __attribute__ ((section (".itcm_code"))) fx_cdx_Proc(void *vblk, int16_t *dst, int16_t *src, uint16_t sz)
{
	fx_cdx_blk *blk = vblk;
	uint16_t i;
	int16_t fb_lvl;
	int32_t rptr;
	int32_t mix;
	int16_t rl_out[2];
	uint8_t chl;
	
	/* update delay parameters if not already crossfading */
	if(!blk->xfcnt)
	{
		uint8_t rng_upd = 0;
		
		/* set range */
		rng_upd = dsp_ratio_hyst_arb(&blk->rng_raw, ADC_val[2], 2);
		blk->rng = 5+2*blk->rng_raw;
		
		/* get raw delay value and apply hysteresis */
		if(dsp_gethyst(&blk->dly, ADC_val[0]) || rng_upd)
		{
			/* compute next delay and start crossfade */
			blk->roff2 = (blk->dly<<blk->rng) + 1;
#ifdef USE_PSRAMBUF
			blk->roff2 = blk->roff2 <= PSRAM_BUFSZ ? PSRAM_BUFSZ : blk->roff2;
#endif
			blk->roff2 = blk->roff2 > blk->len-2 ? blk->len-2 : blk->roff2;
			blk->xfcnt = blk->xflen;
		}
	}
	
	/* get the feedback value */
	fb_lvl = ADC_val[1];
	
	/* loop over the buffers */
	for(i=0;i<sz;i++)
	{
		/* mix fb to input and write to buffer */
		for(chl=0;chl<2;chl++)
		{
			mix = (*(src++)<<12) + blk->fb[chl] * fb_lvl;
			rl_out[chl] = dsp_ssat16(mix>>12);
		}
#ifdef USE_PSRAMBUF
		psbuf_writebytes(&blk->wpsb, 4*blk->wptr, (uint8_t *)rl_out,
			2*sizeof(int16_t));
#else
		xspi_writebytes(4*blk->wptr, (uint8_t *)rl_out, 2*sizeof(int16_t));
#endif
		
		/* get main tap */
		rptr = blk->wptr-blk->roff1;
		rptr = rptr < 0 ? blk->len + rptr : rptr;
		rptr = rptr >= blk->len ? rptr - blk->len : rptr;
		if(!blk->init || (rptr < blk->wptr))
#ifdef USE_PSRAMBUF
		psbuf_readbytes(&blk->r1psb, 4*rptr, (uint8_t *)&rl_out,
			2*sizeof(int16_t));
#else
			xspi_readbytes(4*rptr, (uint8_t *)&rl_out, 2*sizeof(int16_t));
#endif
		else
			rl_out[0] = rl_out[1] = 0;
		
		/* process crossfade */
		if(blk->xfcnt)
		{
			/* do crossfade mix */
			rptr = blk->wptr-blk->roff2;
			rptr = rptr < 0 ? blk->len + rptr : rptr;
			rptr = rptr >= blk->len ? rptr - blk->len : rptr;
			if(!blk->init || (rptr < blk->wptr))
			{
				/* valid data from buffer so mix */
				int16_t rl_xf[2];
#ifdef USE_PSRAMBUF
				psbuf_readbytes(&blk->r2psb, 4*rptr, (uint8_t *)&rl_xf,
					2*sizeof(int16_t));
#else
				xspi_readbytes(4*rptr, (uint8_t *)rl_xf, 2*sizeof(int16_t));
#endif
				for(chl=0;chl<2;chl++)
				{
					mix = (int32_t)rl_out[chl] * blk->xfcnt;
					mix += rl_xf[chl] * (blk->xflen - blk->xfcnt);
					rl_out[chl] = dsp_ssat16(mix>>XFADE_BITS);
				}
			}
			else
			{
				/* no valid data in buffer so just fade */
				for(chl=0;chl<2;chl++)
				{
					mix = (int32_t)rl_out[chl] * blk->xfcnt;
					rl_out[chl] = dsp_ssat16(mix>>XFADE_BITS);
				}
			}
			
			/* update crossfade */
			blk->xfcnt--;
			if(blk->xfcnt == 0)
			{
				/* update current delay */
				blk->roff1 = blk->roff2;
			}
		}
			
		/* dc block on feedback */
		for(chl=0;chl<2;chl++)
		{
			mix = (int32_t)rl_out[chl] - (blk->dcb[chl]>>8); 
			blk->dcb[chl] += mix;
			blk->fb[chl] = dsp_ssat16(mix);
			
			/* output */
			*dst++ = rl_out[chl];
		}
	
		/* update write pointer */
		rptr = blk->wptr;
		blk->wptr = (blk->wptr + 1) % blk->len;
		
		/* clear init flag if write ptr wrapped */
		if(rptr > blk->wptr)
			blk->init = 0;
	}
}

/*
 * Render parameter for clean delay - either delay in ms or feedback %
 */
void fx_cdx_Render_Parm(void *vblk, uint8_t idx, GFX_RECT *rect, uint8_t init)
{
	fx_cdx_blk *blk = vblk;
	char txtbuf[32];
	uint32_t ms;
	static uint32_t prev_ms = 0xffffffff;
	uint8_t update = 0;
	static int16_t prev_fb = -1;
	int16_t fb;
	static uint8_t prev_rng = 255;
	
	if(init)
	{
		/* clear param region and update param name */
		gfx_clrrect(rect);
		gfx_drawstrctr((rect->x0+rect->x1)/2, rect->y1-16, fx_get_parm_name(idx));
		prev_ms = 0;
		prev_fb = -1;
		prev_rng = 255;
	}
	else
	{
		/* update param value */
		switch(idx)
		{
			case 0:	// Delay
				ms = (blk->dly<<blk->rng) + 1;
				ms = ms > blk->len-2 ? blk->len-2 : ms;
				ms = ms / (SAMPLE_RATE/1000);
				if(ms != prev_ms)
				{
					sprintf(txtbuf, "%6u ms ", ms);
					prev_ms = ms;
					update = 1;
				}
				break;
			
			case 1:	// Feedback
				fb = ADC_val[idx]/41;
				if(fb != prev_fb)
				{
					sprintf(txtbuf, "%2d%% ", fb);
					prev_fb = fb;
					update = 1;
				}
				break;
			
			case 2: // Range
				if(blk->rng_raw != prev_rng)
				{
					sprintf(txtbuf, " %s ", cdx_ranges[blk->rng_raw]);
					prev_rng = blk->rng_raw;
					update = 1;
				}
				//fx_cdl_Render_Parm(vblk, 1, rect);	// update Delay too
				break;
			
			default:
				return;
		}
		
		if(update)
		{
			gfx_drawstrctr((rect->x0+rect->x1)/2, rect->y1-6, txtbuf);
		}
	}
}

/*
 * clean delay range struct
 */
fx_struct fx_cdx_struct =
{
	"ClnDlyExt",
	3,
	cdx_param_names,
	FX_OVL_IDX_DLY,
	fx_cdx_Init,
	fx_bypass_Cleanup,
	fx_cdx_Proc,
	fx_cdx_Render_Parm,
};

