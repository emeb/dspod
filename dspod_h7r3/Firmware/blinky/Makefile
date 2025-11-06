# Makefile for H7R3 with openocd
# 10-14-2025 E. Brombaugh

# sub directories
VPATH = .:../common:../CMSIS:../HAL

# Object files
OBJECTS = 	startup_stm32h7r3xx.o system_stm32h7rsxx.o main.o led.o usart.o printf.o \
			stm32h7rsxx_hal.o stm32h7rsxx_hal_cortex.o stm32h7rsxx_hal_gpio.o \
            stm32h7rsxx_hal_rcc.o stm32h7rsxx_hal_rcc_ex.o stm32h7rsxx_hal_pwr_ex.o \
            stm32h7rsxx_hal_usart.o \

# Linker script
LDSCRIPT = stm32h7r3xx_flash.ld

# dependency dir
DEPDIR = dep

# Compiler Flags
CFLAGS  = -g -O3 -ffunction-sections -std=gnu99 -Wall -flto
CFLAGS += -I. -I../common -I../CMSIS -I../HAL -I../DSP -I../shimmer
CFLAGS += -DARM_MATH_CM7 -DUSE_HAL_DRIVER
CFLAGS += -D'HSE_VALUE=((uint32_t)12000000)' -DSTM32H7R3xx
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m7 
CFLAGS += -mfloat-abi=hard -mfpu=fpv5-sp-d16

# Compiler flags to generate dependency files:
CFLAGS += -MMD -MP -MF $(DEPDIR)/$(@F).d

AFLAGS  = -mlittle-endian -mthumb -mcpu=cortex-m7 
LFLAGS  = $(CFLAGS) -nostartfiles -T $(LDSCRIPT) -Wl,-Map=main.map
LFLAGS += -Wl,--gc-sections -Wl,--print-memory-usage
#LFLAGS += --specs=nano.specs
CPFLAGS = --output-target=binary
ODFLAGS	= -x --syms
BASEADD = 0x08000000

# Executables
#ARCH = arm-none-eabi
ARCH = /opt/launchpad/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi
CC = $(ARCH)-gcc
LD = $(ARCH)-ld -v
AS = $(ARCH)-as
OBJCPY = $(ARCH)-objcopy
OBJDMP = $(ARCH)-objdump
GDB = $(ARCH)-gdb

## Mainline OpenOCD installed in /usr/local
#OPENOCD = openocd

## ST's fork of OpenOCD - need to 
## export OPENOCD_SCRIPTS=/home/ericb/build/openocd/stm32oocd/OpenOCD/tcl/
## before using
#OPENOCD = ~/build/openocd/stm32oocd/OpenOCD/src/openocd

## OOCD from STM32CubeIDE - need to
## export OPENOCD_SCRIPTS=/home/ericb/st/stm32cubeide_1.19.0/plugins/com.st.stm32cube.ide.mcu.debug.openocd_2.3.100.202501240831/resources/openocd/st_scripts
## before using
OPENOCD = /home/ericb/st/stm32cubeide_1.19.0/plugins/com.st.stm32cube.ide.mcu.externaltools.openocd.linux64_2.4.200.202505051030/tools/bin/openocd

CRYPT = ../crc_calc/crc_calc
DFU = dfu-util
DFU_SUFFIX = dfu-suffix

CPFLAGS = --output-target=binary
ODFLAGS	= -x --syms

# Targets
all: main.bin

clean:
	rm -f $(OBJECTS) crt.lst *.lst *.elf *.bin *.map *.dmp
	rm -rf $(DEPDIR)
	
#flash: gdb_flash
flash: oocd_stlink_flash_elf

oocd_stlink_flash_elf: main.elf
	$(OPENOCD) -f openocd_stlink.cfg -c "program main.elf verify reset exit"

oocd_stlink_flash_bin: main.bin
	$(OPENOCD) -f openocd_stlink.cfg -c "program main.bin verify reset exit $(BASEADD)"

oocd_stlink_dump:
	$(OPENOCD) -f openocd_stlink.cfg -c "dump_image dump.bin $(BASEADD) 0x10000"

gdb_flash: main.elf
	$(GDB) -x flash_cmd.gdb -batch
	stty sane

main.dfu: main.bin
	cp main.bin main.dfu
	$(DFU_SUFFIX) -v 0483 -p df11 -a main.dfu

dfu_flash: main.dfu
	$(DFU) -a 0 -s 0x8000000 -D main.dfu

disassemble: main.elf
	$(OBJDMP) -d main.elf > main.dis
	
dist:
	tar -c *.h *.c *.s Makefile *.cmd *.cfg openocd_doflash | gzip > minimal_hello_world.tar.gz

main.ihex: main.elf
	$(OBJCPY) --output-target=ihex main.elf main.ihex

main.bin: main.elf 
	$(OBJCPY) $(CPFLAGS) main.elf main.bin
	$(OBJDMP) $(ODFLAGS) main.elf > main.dmp
	ls -l main.elf main.bin

main.elf: $(DEPDIR) $(OBJECTS) $(LDSCRIPT)
	$(CC) $(LFLAGS) -o main.elf $(OBJECTS) -lnosys -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# needed for that one file with warnings
stm32h7rsxx_hal_spi.o: stm32h7rsxx_hal_spi.c stm32h7rsxx_hal_spi.h
	$(CC) $(CFLAGS) -fno-strict-aliasing -c -o $@ $<

# the dependency dir
$(DEPDIR):
	@mkdir -p $@

# Include the dependency files. This should come last!!!
-include $(wildcard $(DEPDIR)/*.d)
