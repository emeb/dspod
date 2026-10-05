/*
 * menu.h - menuing UI for dspod_esp32s3
 * 10-06-25 E. Brombaugh copied from dspod_rp2350
 */

#ifndef __menu__
#define __menu__

extern char txtbuf[32];

void menu_splash(const char *swVersionStr, const char *bdate, const char *btime);
void menu_init(void);
void menu_process(void);

#endif
