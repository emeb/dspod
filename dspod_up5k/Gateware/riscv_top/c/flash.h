/*
 * flash.h - flash memory driver
 * 07-03-19 E. Brombaugh
 */

#ifndef __flash__
#define __flash__

#include "main.h"

/* flash commands */
#define FLASH_WRPG 0x02 // write page
#define FLASH_READ 0x03 // read data
#define FLASH_RSR1 0x05 // read status reg 1
#define FLASH_RSR2 0x35 // read status reg 2
#define FLASH_RSR3 0x15 // read status reg 3
#define FLASH_WSR1 0x01 // write status reg 1
#define FLASH_WSR2 0x31 // write status reg 2
#define FLASH_WSR3 0x11 // write status reg 3
#define FLASH_WEN  0x06 // write enable
#define FLASH_EB32 0x52 // erase block 32k
#define FLASH_GBUL 0x98 // global unlock
#define FLASH_WKUP 0xAB // wakeup
#define FLASH_ERST 0x66 // enable reset
#define FLASH_RST  0x99 // reset
#define FLASH_ID   0x9f // get ID bytes

void flash_init(SPI_TypeDef *s);
void flash_read(SPI_TypeDef *s, uint8_t *dst, uint32_t addr, uint32_t len);
uint8_t flash_rdreg(SPI_TypeDef *s, uint8_t cmd);
uint8_t flash_status(SPI_TypeDef *s);
void flash_busy_wait(SPI_TypeDef *s);
void flash_eraseblk(SPI_TypeDef *s, uint32_t addr);
void flash_write(SPI_TypeDef *s, uint8_t *src, uint32_t addr, uint32_t len);
uint32_t flash_id(SPI_TypeDef *s);

#endif

