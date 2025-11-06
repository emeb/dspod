/*
 * shared_i2c.h - shared I2C bus basic routines
 * 07-12-19 E. Brombaugh
 */

#ifndef __shared_i2c__
#define __shared_i2c__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7rsxx_hal.h"

extern I2C_HandleTypeDef i2c_handler;

void shared_i2c_init(void);
void shared_i2c_reset(void);

#ifdef __cplusplus
}
#endif

#endif
