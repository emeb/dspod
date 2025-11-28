/*
 * shared_spi.h - interface routines for shared SPI port.
 * 04-19-16 E. Brombaugh
 * 08-31-17 E. Brombaugh - updated for F303
 * 04-01-20 E. Brombaugh - updated for H750
 * 12-05-20 E. Brombaugh - updated for H730
 * 11-13-25 E. Brombaugh - updated for H7R3
 */

#ifndef __SHARED_SPI__
#define __SHARED_SPI__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

uint8_t Shared_SPI_ReadWriteByte(uint8_t tx);
void Shared_SPI_WriteByte(uint8_t Data);
void Shared_SPI_WriteBytes(uint8_t *pData, uint16_t size);
void Shared_SPI_WriteWord(uint16_t Data);
void Shared_SPI_Blocking_PIO_WriteBytes(uint8_t *pData, uint32_t Size);
void Shared_SPI_Blocking_PIO_WriteWord(uint16_t Data, uint32_t Size);
void Shared_SPI_start_DMA_WriteBytes(uint8_t *buffer, int32_t count);
void Shared_SPI_end_DMA_WriteBytes(void);
void Shared_SPI_Init(void);

#ifdef __cplusplus
}
#endif

#endif
