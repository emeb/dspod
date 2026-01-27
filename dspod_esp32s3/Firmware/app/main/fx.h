/*
 * fx.h - Algorithm access for dspod h7r3
 * 11-15-25 E. Brombaugh
 */

#ifndef __fx__
#define __fx__

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "dsp_lib.h"
#include "eb_adc.h"
#include "gfx.h"

#define SAMPLE_RATE     (48000)

#ifdef SPECT
#define FRAMESZ			(512)
#define FX_NUM_ALGOS  2
#else
#define FRAMESZ			(64)
#define FX_NUM_ALGOS  6
#endif
#define FX_MAX_PARAMS 3
#define FX_MAX_MEM (129*1024)


/* this is currently unused but left here for the fx structs */
enum overlays
{
	FX_OVL_IDX_FILT = 0x0,
	FX_OVL_IDX_DLY = 0x1,
	FX_OVL_IDX_MVB = 0x2,
	FX_OVL_IDX_NONE = 0xff
};

/*
 * structure containing algorithm access info
 */
typedef struct
{
	const char *name;
	uint8_t parms;
	const char **parm_names;
	uint8_t overlay_index;	// CURRENTLY UNUSED
	void * (*init)(uint32_t *mem);
	void (*cleanup)(void *blk);
	void (*proc)(void *blk, int16_t *dst, int16_t *src, uint16_t sz);
	void (*render_parm)(void *blk, uint8_t idx, GFX_RECT *rect, uint8_t init);
} fx_struct;

/* array of ptrs to effects structs */
extern const fx_struct *effects[FX_NUM_ALGOS];
extern size_t fx_ext_sz;
extern int16_t *fx_ext_buffer;
extern size_t fx_int_sz;
extern uint32_t *fx_mem;

void fx_bypass_Cleanup(void *dummy);
void fx_bypass_Render_Parm(void *blk, uint8_t idx, GFX_RECT *rect, uint8_t init);

uint8_t fx_init(void);
void fx_select_algo(uint8_t algo);
void fx_proc(int16_t *dst, int16_t *src, uint16_t sz);
uint8_t fx_get_algo(void);
uint8_t fx_get_num_parms(void);
char * fx_get_algo_name(uint8_t algo_num);
char * fx_get_curr_algo_name(void);
char * fx_get_parm_name(uint8_t idx);
void fx_render_parm(uint8_t idx, uint8_t init);

#endif

