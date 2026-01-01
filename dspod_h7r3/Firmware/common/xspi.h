/*
 * xspi.c - external xspi PSRAM setup for H7R3 w/ XSPI port
 * 11-04-2025 E. Brombaugh
 */

#ifndef __xspi__
#define __xspi__

#include "main.h"

#define XSPI_PSRAM_TYPE 0
#define XSPI_PSRAM_ADDR_BITS 23
#define XSPI_PSRAM_SIZE (1<<XSPI_PSRAM_ADDR_BITS)
#define XSPI_PSRAM_NCS HAL_XSPI_CSSEL_NCS1
#define XSPI_PSRAM_OVR HAL_XSPI_CSSEL_OVR_NCS1

#define XSPI_FLASH_TYPE 1
#define XSPI_FLASH_ADDR_BITS 22
#define XSPI_FLASH_SIZE (1<<XSPI_FLASH_ADDR_BITS)
#define XSPI_FLASH_NCS HAL_XSPI_CSSEL_NCS2
#define XSPI_FLASH_OVR HAL_XSPI_CSSEL_OVR_NCS2

void xspi_init(uint8_t type);
void xspi_psram_init(void);
void xspi_flash_init(void);
void xspi_psram_writebytes(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_psram_readbytes(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_psram_memmap(uint8_t enable);
void xspi_flash_1bcmd(uint8_t cmd);
uint8_t xspi_flash_read_status(uint8_t byte_num);
void xspi_flash_write_status(uint8_t byte_num, uint8_t data);
void xspi_flash_erase(uint8_t type, uint32_t addr, uint32_t sz);
uint8_t xspi_flash_writepg(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_flash_readbytes(uint32_t addr, uint8_t *data, uint32_t sz);
void xspi_flash_memmap(uint8_t enable);
uint8_t xspi_psram_test(uint8_t testflags);
uint8_t xspi_flash_test(uint8_t testflags);

#endif
