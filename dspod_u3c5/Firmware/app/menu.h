/*
 * menu.h - menuing UI for dspod_rp2350
 * 10-06-25 E. Brombaugh copied from dspod_cv1800b
 */

#ifndef __menu__
#define __menu__

void menu_splash(const char *swVersionStr, const char *bdate, const char *btime);
void menu_init(void);
void menu_process(void);

#endif
