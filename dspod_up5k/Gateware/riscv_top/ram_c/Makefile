# Makefile for RISC-V toolchain targting picorv32

#CROSS = /opt/riscv-none-gcc/8.1.0-2-20181019-0952/bin/riscv-none-embed-
CROSS = /opt/riscv-gcc/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
ICEPROG = iceprog
HEXDUMP = hexdump
HEXDUMP_ARGS = -v -e '1/4 "%08x" "\n"'
CDCPROG = cdc_prog

#CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -nostartfiles -flto
CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -flto -nostartfiles -fomit-frame-pointer

HEADER = main.h menu.h fx.h widgets.h dsp_lib.h \
		../c/riscv_top.h ../c/acia.h ../c/spi.h ../c/flash.h ../c/clkcnt.h \
		../c/gfx.h ../c/st7789.h ../c/i2c.h ../c/printf.h ../c/psram.h \
		../c/codec.h ../c/si5351.h ../c/debounce.h ../c/button.h ../c/led.h

SOURCES = start.S main.c menu.c fx.c widgets.c dsp_lib.c \
		../c/acia.c ../c/spi.c ../c/flash.c ../c/clkcnt.c \
		../c/gfx.c ../c/st7789.c ../c/i2c.c ../c/printf.c ../c/psram.c \
		../c/codec.c ../c/si5351.c ../c/debounce.c ../c/button.c ../c/led.c 

main.elf: lnk-app.lds $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS)  -Wl,-Bstatic,-T,lnk-app.lds,--strip-debug,--print-memory-usage,-Map=main.map -I../c -o $@ $(SOURCES)

disassemble: main.elf
	$(OBJDUMP) -S main.elf > main.dis

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	
prog: main.bin
	$(CDCPROG) -w -a 0x100000 -p /dev/ttyACM0 $<

flash_algos: mv_roms.bin
	$(CDCPROG) -w -a 0x110000 -p /dev/ttyACM0 $<

%.hex: %.bin
	$(HEXDUMP) $(HEXDUMP_ARGS) $< >$@

clean:
	rm -f main.bin *.hex *.elf *.dis *.map
