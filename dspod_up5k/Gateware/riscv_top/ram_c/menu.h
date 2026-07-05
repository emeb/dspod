/*
 * menu.h - UI menu logic for dspod_up5k
 * 06-16-2026 E. Brombaugh
 */
#ifndef __menu__
#define __menu__

#include "main.h"

void menu_splash(const char *swVersionStr, const char *bdate, const char *btime);
void menu_init(void);
void menu_process(void);

#endif
