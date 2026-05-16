/*
 * HspNgn.c - top-level wrapper for HSP Engine setup
 * 05-12-2026 E. Brombaugh
 */

#ifndef __hspngn__
#define __hspngn__

#include "main.h"
#include "hsp.h"

extern hsp_engine_context_t hmw;

HAL_StatusTypeDef HspNgn_Init(void);

#endif