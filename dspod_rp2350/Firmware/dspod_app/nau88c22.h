/*
 * NAU88C22.h - NAU88C22 codec I2C control port driver for RP2040
 * 05-21-25 E. Brombaugh
 */

#ifndef __NAU88C22__
#define __NAU88C22__

int32_t NAU88C22_WriteRegister(uint16_t Reg, uint16_t Data);
int32_t NAU88C22_ReadRegister(uint16_t Reg, uint16_t *Data);
int32_t NAU88C22_Reset(void);
int32_t NAU88C22_Dump_Regs(void);
int32_t NAU88C22_Init(void);

#endif
