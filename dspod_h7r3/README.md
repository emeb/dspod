# dspod_h7r3

![dspod_cv1800b daughterboard](./doc/dspod_h7r3.png)

The dspod_h7r3 is a daughterboard comprising an STM32H7R3V8 MCU with a choice of PSRAM or Flash, USB and GPIO + audio I/O.

## Abstract

This board is a small 32-pin device with the following features:

* STM32H7R3V8 MCU
  - 100-pin LQFP package
  - Arm Cortex M7 CPU at 600MHz max clock
  - 620kB SRAM
  - 64kB Flash
  - USB, I2C, SPI, ADC, GPIO, etc on-chip
* USB-C connector, high-speed or full-speed host/device
* SOIC-8 flash or PSRAM (or both!)
* Nuvoton NAU88C22 stereo codec
* Activity LED
* 3.3V level serial for debug / status
* SWD for flashing / debug
* Misc GPIO
  - SPI
  - I2C
  - GPIO
* Four channels of 3.3V multiplexed A/D input

This board is test to see how well this new, faster version of the STM32H7 performs - with a clock speed of 600MHz max and plenty of on-chip SRAM it may work well for 'bare-metal' audio DSP. The downside is that it has very limited flash memory which will put a squeeze on the amount of code that can run on-chip. To compensate, a single channel of QSPI to an external device is provided which can host either a large flash memory (up to 128MB is available in this package) or an 8MB PSRAM which can be used for audio buffering. An optional chip select is brought out for experiments with stacking both flash and PSRAM - more on this after hardware is on the bench.

* ## Design Materials

* [Schematic](./doc/dspod_h7r3_sch.pdf)

## Hardware

The hardware design is provided in Kicad 9.x format in the [Hardware](./Hardware) directory.

## Firmware

Firmware is TBD.
