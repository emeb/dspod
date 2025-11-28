/*
 * psram_buff.h - read & write buffering for the xspi psram indirect mode
 * 11-22-2025 E. Brombaugh
 */

#ifndef __psram_buff__
#define __psram_buff__

#include "stm32h7rsxx_hal.h"

/*
 * structure for psram buffers (both read and write)
 */
typedef struct
{
	uint8_t *buf;
	uint32_t bufsz;
	uint32_t psram_start;
	uint32_t psram_sz;
	uint32_t begin_addr;
	uint32_t next_addr;
	uint32_t fill_cnt;
	uint32_t idx;
} psbuf_struct;

uint32_t psbuf_init(psbuf_struct *pbf, uint32_t *mem, uint32_t bufsz,
	uint32_t psram_start, uint32_t psram_sz);
void psbuf_writebytes(psbuf_struct *pbf, uint32_t addr, uint8_t *data,
	uint32_t sz);
void psbuf_readbytes(psbuf_struct *pbf, uint32_t addr, uint8_t *data,
	uint32_t sz);

#endif
