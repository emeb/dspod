/*
 * xspi.c - external xspi PSRAM setup for H7R3 w/ XSPI port
 * 11-04-2025 E. Brombaugh
 */

#ifndef __xspi__
#define __xspi__

#include "stm32h7rsxx_hal.h"

#define XSPI_ADDR_BITS 23
#define XSPI_SIZE (1<<XSPI_ADDR_BITS)

void xspi_init(void);
void xspi_writebytes(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_readbytes(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_memmap(uint8_t enable);
uint8_t xspi_test(uint8_t testflags);

#endif
