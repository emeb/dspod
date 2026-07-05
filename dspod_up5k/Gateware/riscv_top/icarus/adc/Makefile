# Makefile for Icarus Verilog simulation
# 02-11-2019 E. Brombaugh

# sources
SOURCES = 	tb_adc.v ../../src/adc.v 

# top level
TOP = tb_adc
			
# Executables
TOOLS = /opt/openfpga/fpga-toolchain
VLOG = iverilog
WAVE = gtkwave
TECH_LIB = $(TOOLS)/share/yosys/ice40/cells_sim.v

# targets
all: $(TOP).vcd

wave: $(TOP).vcd $(TOP).gtkw
	$(WAVE) $(TOP).gtkw
	
$(TOP).vcd: $(TOP)
	./$(TOP)

$(TOP): $(SOURCES) $(HEX)
	$(VLOG) -D icarus -DNO_ICE40_DEFAULT_ASSIGNMENTS -l $(TECH_LIB) -o $(TOP) $(SOURCES)
	
clean:
	rm -rf a.out *.obj $(HEX) $(RPT) $(TOP) $(TOP).vcd
	
