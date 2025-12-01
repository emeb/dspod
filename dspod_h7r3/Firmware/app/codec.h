/*
 * codec.h - codec I2C control port driver for NAU88C22
 * 11-03-25 E. Brombaugh
 */

#ifndef __codec__
#define __codec__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

extern I2C_HandleTypeDef i2c_handler;

HAL_StatusTypeDef Codec_WriteRegister(uint8_t RegAddr, uint16_t RegValue);
HAL_StatusTypeDef Codec_ReadRegister(uint8_t RegAddr, uint16_t *RegValue);
void Codec_Reset(void);
void Codec_Init(void);

#ifdef __cplusplus
}
#endif

#endif
