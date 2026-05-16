/*
 * sai.c - sai driver for dspod_u3c5
 * 05-05-26 E. Brombugh
 */

#ifndef __sai__
#define __sai__

#include "main.h"
#include "audio.h"

#define SAI_BUFSZ BUFSZ

void sai_init(void);
void sai_start(void);
int32_t sai_get_fsamp(void);

#endif


