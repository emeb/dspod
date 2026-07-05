/*
 * fx.h - Algorithm access for dspod up5k
 * 06-18-26 E. Brombaugh
 */

#ifndef __fx__
#define __fx__

#include "main.h"

#define FX_NUM_ALGOS  	(4*63)

uint8_t fx_init(void);
void fx_select_algo(uint16_t algo);
uint16_t fx_get_algo(void);
char * fx_get_algo_name(uint16_t algo_num);
char * fx_get_curr_algo_name(void);

#endif
