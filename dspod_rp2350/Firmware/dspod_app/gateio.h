/*
 * gateio.h - gate IO driver for dspod
 * 08-28-25 E. Brombaugh
 */
#ifndef __gateio__
#define __gateio__

#include "main.h"

void gate_init(void);
void set_gate(bool gate_on);

#endif
