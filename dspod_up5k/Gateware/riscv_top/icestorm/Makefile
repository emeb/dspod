# Makefile for icestorm tools + yosys + nextpnr
# Modified from examples in icestorm distribution
# 01-16-18 E. Brombaugh

# src directory
VPATH = ../src

SRC =	../src/riscv_top.v ../src/system.v ../src/spram_16kx32.v \
		../src/acia.v ../src/acia_rx.v ../src/acia_tx.v ../src/ps_input.v \
		../src/debounce.v ../src/rotary_decode.v ../src/wd_mix.v ../src/sat.v \
		../src/level_det.v ../src/mv.v \
		../src/wb_bus.v ../src/wb_master.v ../src/adc.v ../src/i2s_inout.v \
		../src/picorv32.v 

# preparing the machine code
FAKE_HEX =	rom.hex
REAL_HEX =  code.hex
HEX_DIR = ../rom_c

# project stuff
PROJ = riscv_top
PIN_DEF = ../src/dspod_up5k.pcf
SDC = ../src/riscv_top.sdc
DEVICE = up5k
PACKAGE = sg48

# tools
TOOLS = /opt/openfpga/fpga-toolchain
#TOOLS = /opt/openfpga/oss-cad-suite_06-14-2024
#TOOLS = /opt/openfpga/oss-cad-suite_02-03-2026
#TOOLS = /opt/openfpga/oss-cad-suite_06-10-2026
#TOOLS = /opt/openfpga/oss-cad-suite_06-18-2026
YOSYS = $(TOOLS)/bin/yosys
YOSYS_SYNTH_ARGS = -dsp -relut -dffe_min_ce_use 4
NEXTPNR = $(TOOLS)/bin/nextpnr-ice40
NEXTPNR_ARGS = --pre-pack $(SDC) --placer heap
ICEPACK = $(TOOLS)/bin/icepack
ICETIME = $(TOOLS)/bin/icetime
ICEPROG = $(TOOLS)/bin/iceprog
ICEBRAM = $(TOOLS)/bin/icebram
CDCPROG = cdc_prog
DFU_UTIL = dfu-util
VERILATOR = verilator
TECH_LIB = $(TOOLS)/share/yosys/ice40/cells_sim.v

all: $(PROJ).bin

$(FAKE_HEX):
	$(ICEBRAM) -g 32 2048 > $(FAKE_HEX)

%.json: $(SRC) $(FAKE_HEX)
	$(YOSYS) -p 'synth_ice40 -dsp -top $(PROJ) -json $@' $(SRC)

%.asc: %.json $(PIN_DEF) 
	$(NEXTPNR) $(NEXTPNR_ARGS) --$(DEVICE) --json $< --pcf $(PIN_DEF) --package $(PACKAGE) --asc $@

$(REAL_HEX):
	$(MAKE) -C $(HEX_DIR) main.hex
	cp $(HEX_DIR)/main.hex ./$(REAL_HEX)
		
%.bin: %.asc $(REAL_HEX)
	$(ICEBRAM) $(FAKE_HEX) $(REAL_HEX) < $< > temp.asc
	$(ICEPACK) temp.asc $@

%.rpt: %.asc
	$(ICETIME) -d $(DEVICE) -mtr $@ $<

# Note: This loads direct to the FPGA, not into flash
prog: $(PROJ).bin
	$(CDCPROG) -p /dev/ttyACM0 $<

# Note: This will update the firmware in the ROM and reload the FPGA
recode:
	rm -f $(REAL_HEX) $(PROJ).bin
	$(MAKE) prog

# Note: This is a "raw" flash that only works on a device w/o the NO2 Bootloader
#       If the bootloader is present then you must use the 'flash_dfu0' or 'dfu' targets
#		because low flash is protected.
flash: $(PROJ).bin
	$(CDCPROG) -w -p /dev/ttyACM0 $<

# Note: This writes direct to flash in the region reserved by the bootloader for the config
#       binary
flash_dfu0: $(PROJ).bin
	$(CDCPROG) -w -a 0x80000 -p /dev/ttyACM0 $<

# Note: This requires the NO2 Bootloader to be installed and active. You must
#       reset the board while holding the BOOT button, then wait for the red/greem
#       LED pulsation.
dfu: $(PROJ).bin
	$(DFU_UTIL) -d1d50:6146 -a0 -R -D $<

lint: $(SRC)
	$(VERILATOR) --lint-only -Wall -DNO_ICE40_DEFAULT_ASSIGNMENTS --top-module $(PROJ) $(TECH_LIB) $(SRC)

clean:
	$(MAKE) -C $(HEX_DIR) clean
	rm -f *.json *.asc *.rpt *.bin *.hex

.SECONDARY:
.PHONY: all prog clean
