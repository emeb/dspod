# dspod_up5k riscv_top Gateware

This directory contains a makefile-based project for a complete GUI-based MIDIVerb emulator.

## Abstract

The MIDIVerb emulator design consists of FPGA gateware and two layers of
embedded firmware that runs on it. The system breaks down as follows:

- OPTIONAL: NO2 Bootloader Gateware. This is Sylvain Munaut's USB DFU bootloader that handles loading firmware when you don't have a SPI flash loader such as the SPI Fiend mentioned earlier. The NO2 bootloader lives in the first configuration slot of the external SPI flash memory and ordinarily just reboots to the Main FPGA gateware (below). It is not required to use this.

- Main FPGA Gateware. This is a combination of a RISC-V softcore (Claire Wolf's PicoRV32) based SoC that includes some ROM, RAM, UART and interface peripherals to connect to the external resources (ADC, Codec, LCD, buttons, etc) as well as the MIDIVerb emulator logic. The ROM contains a small bootloader that copies the larger application into on-chip RAM and restarts. The source code for this is found in the `src` directory and building takes place in the `icestorm` directory.

- Bootloader. This is a small C-based firmware that is permanently stored in the FPGA bitstream file and serves only to fetch the main application code from the external SPI flash memory into a 64kB SPRAM on the FPGA. The source for this is found in the `rom_c` directory and that is also where the build takes place. Some addition common code for peripheral drivers is found in the 'c' directory.

- Main Application. This contains all the code to initialize and operate the GUI, audio codec, control voltage ADCs and manage moving algorithms from SPI
  flash into the MIDIVerb microcode store.

## Building and Installing

Ensure you've got the prerequisites (see Gateware/README.md) then follow
these steps:

- OPTIONAL: build and install the NO2 Bootloader. Details for this can be found at Sylvain's repo here: [NO2 Bootloader](https://github.com/emeb/no2bootloader) Note that when configuring the build you should use `make BOARD=ice-dongle` to ensure compatibility with the LED and Boot button on the dspod_up5k.

- In the `icestorm` directory run `make` to build the ROM firmware and bitstream. If you have the NO2 bootloader installed then run `make flash_dfu0` to place the bitstream at the second configuration slot in SPI flash. If you did not set up the NO2 bootloader then instead run `make flash` which will install in the first slot.

- in the `ram_c` directory run `make` to build the Main Application firmware, then run `make prog` to install it into the correct location in SPI flash. Finally, run `make flash_algos` to install the MIDIVerb DSP microcode into its proper location in the SPI flash.
