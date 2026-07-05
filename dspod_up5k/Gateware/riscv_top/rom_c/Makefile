# Makefile for RISC-V toolchain targting picorv32

#CROSS = /opt/riscv-none-gcc/8.1.0-2-20181019-0952/bin/riscv-none-embed-
CROSS = /opt/riscv-gcc/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
ICEPROG = iceprog
HEXDUMP = hexdump
HEXDUMP_ARGS = -v -e '1/4 "%08x" "\n"'

#CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -nostartfiles -flto
CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -flto -nostartfiles -fomit-frame-pointer

HEADER = 	../c/riscv_top.h ../c/acia.h ../c/spi.h ../c/flash.h ../c/printf.h \
			../c/clkcnt.h

SOURCES = 	start.S main.c ../c/acia.c ../c/spi.c ../c/flash.c ../c/printf.c \
			../c/clkcnt.c

main.elf: lnk-app.lds $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS)  -Wl,-Bstatic,-T,lnk-app.lds,--strip-debug,--print-memory-usage,-Map=main.map -I../c -o $@ $(SOURCES)

disassemble: main.elf
	$(OBJDUMP) -d main.elf > main.dis

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.hex: %.bin
	$(HEXDUMP) $(HEXDUMP_ARGS) $< >$@

clean:
	rm -f *.bin *.hex *.elf *.dis *.map
