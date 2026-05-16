/*
 * pdm.h - dspod_u3c5 PDM setup
 * 05-06-26 E. Brombaugh
 */

#ifndef __pdm__
#define __pdm__

#include "main.h"

#define PDM_BUFSZ 128
#define PDM_SAMPSZ 512

extern int32_t pdm_sample[];

HAL_StatusTypeDef PDMInit(void);
int32_t PDM_GetMax(void);
int32_t PDM_ChkSamp(void);
void PDM_StartSamp(void);

#endif
