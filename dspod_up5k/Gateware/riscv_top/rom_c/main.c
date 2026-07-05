/*
 * main.c - top level of picorv32 firmware for dspod_up5k ROM loader
 * 06-04-26 E. Brombaugh
 */
#include "main.h"
#include "acia.h"
#include "spi.h"
#include "flash.h"
#include "clkcnt.h"

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;
const char *fwVersionStr = "V0.1";

uint8_t dest[32768] __attribute__ ((section (".ram1_data")));

/*
 * hack to view the current SP
 */
void print_stack_pointer() {
  void* p = NULL;
  printf("Current Stack Pointer: 0x%08X\n\r", (void*)&p);
}

/*
 * main
 */
void main()
{
	init_printf(0, acia_printf_putc);
	printf("\n\n\r-----------------------------\n\r");
	printf("dspod_up5k ROM loader - starting up\n\r");
    printf("Version: %s\n\r", fwVersionStr);
    printf("Build Date: %s\n\r", bdate);
    printf("Build Time: %s\n\r", btime);
	printf("-----------------------------\n\r");

	print_stack_pointer();

	/* test both SPI ports */
	spi_init(SPI0);
	
	/* copy flash to RAM */
	flash_init(SPI0);	// wake up the flash chip
	flash_read(SPI0, dest, 0x100000, 0x8000);
#if 0
	printf("Jumping to loaded code @ 0x%08X...\r\n\n\n", loaded_code);
	void (*loaded_code)(void) = dest;
	loaded_code();	// never returns
#else
	printf("Remapping and Resetting CPU\n\r");
	sysctrl_reg = SYSCTRL_CPURST | SYSCTRL_ROMREMAP;
#endif
}
