/*
 * ospi.c - external ospi PSRAM setup for dspod_u3c5
 * 05-06-2025 E. Brombaugh
 */

#ifndef __ospi__
#define __ospi__

#include "main.h"

#define OSPI_PSRAM_ADDR_BITS 23
#define OSPI_PSRAM_SIZE (1<<OSPI_PSRAM_ADDR_BITS)

void ospi_init(void);
void ospi_psram_init(void);
void ospi_flash_init(void);
void ospi_psram_writebytes(uint32_t addr, uint8_t *data, uint32_t sz);
void ospi_psram_readbytes(uint32_t addr, uint8_t *data, uint32_t sz);
void ospi_psram_memmap(uint8_t enable);
uint8_t ospi_psram_test(uint8_t testflags);

#endif
