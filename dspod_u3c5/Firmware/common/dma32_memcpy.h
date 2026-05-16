/*
 * dma32_memcpy.c - dma version of memcpy for 32-bit aligned values
 * 05-14-2026 E. Brombaugh
 */

#ifndef __dma32_memcpy__
#define __dma32_memcpy__

#include "main.h"

HAL_StatusTypeDef dma32_memcpy_init(void);
void dma32_memcpy(void *dst, void *src, uint32_t len);

#endif
