# dspod_h7r3 Firmware

This directory contains various makefile-based projects for testing out the dspod_h7r3. There are projects, some with overlapping functionality:

- blinky - the traditional blinking LED for initial proof-of-life after assembly.

- adc - testing out the 4-channel ADC inputs.

- i2s_codec - testing out the I2S I/O channel, I2C control and the NAU88C22 codec.

- psram - testing out the 8MB PSRAM attached to the XSPI port.

## Prerequisites

You'll need to have installed a working Arm GCC toolchain in order to use these projects, as well as a version of OpenOCD or Black Magic Probe that supports the STM32H7R3 for loading code into flash. The Makefiles in each project directory will have to be edited to point to these tools.



## Building

The following steps should be sufficient to build each project:

```
cd [your chosen project]
make
export OPENOCD_SCRIPTS=[location of the OpenOCD scripts directory]
make flash
```
