# dspod_up5k Gateware

This directory contains various makefile-based projects for testing out the dspod_up5k:

- blinky - the traditional blinking LED for initial proof-of-life after assembly.

- riscv_top - A MIDIVerb emulator with full LCD/Encoder-based GUI running on a RISC-V soft CPU with C firmware.

## Prerequisites

The build process depends on `make` and the YosysHQ OSS CAD Suite. The RISC-V toolchain is also needed for the firmware in the riscv_top project:

- [YosysHQ oss-cad-suite](https://github.com/YosysHQ/oss-cad-suite-build)

- [RISC-V GCC pre-built toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain/releases)

## Building

Directions for building vary across the projects - check the README files for details.

## Installation

Loading the gateware onto the dspod up5k requires a SPI flash programmer. I've built a custom programmer specifically for my cabling / connector setup:

- USB to SPI programmer: [SPI Fiend](https://github.com/emeb/spi_fiend)

- 0.1 inch header to 0.127mm 10-pin cable adapter: [ice-prog adapt](https://github.com/emeb/ice-dongle/tree/main/Hardware/ice-prog-adapt)
