/*
 * codec.h - driver for NAU88C22 codec I2C control port
 * 06-08-2026 E. Brombaugh
 */
#ifndef __codec__
#define __codec__

#include "main.h"

#define CODEC_MUTE_ON 1
#define CODEC_MUTE_OFF 0

int8_t Codec_WriteRegister(uint8_t RegAddr, uint16_t RegValue);
int8_t Codec_ReadRegister(uint8_t RegAddr, uint16_t *RegValue);
uint8_t Codec_Init(void);
void Codec_Mute(int8_t ena);

#endif
