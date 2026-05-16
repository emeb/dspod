/*
 * shared_i2c.h - shared I2C bus basic routines for dspod_u3c5
 * 05-05-2026 E. Brombaugh
 */

#ifndef __shared_i2c__
#define __shared_i2c__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern I2C_HandleTypeDef i2c_handler;

void shared_i2c_init(void);
void shared_i2c_reset(void);

#ifdef __cplusplus
}
#endif

#endif
