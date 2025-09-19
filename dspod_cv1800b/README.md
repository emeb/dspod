# dspod_cv1800b

![dspod_cv1800b daughterboard](./doc/dspod_cv1800b.png)

The dspod_cv1800b is a daughterboard comprising a Sophon/CVItek CV1800B Linux SoC
with 64MB RAM, SD card interface, USB and GPIO + audio I/O.

## Abstract

This board is a small 32-pin device with the following features:

* CV1800B Linux SoC
  - 1GHz RISC-V primary CPU
  - 700MHz RISC-V secondary CPU
  - 8051 low-power CPU
  - 64MB SDRAM
  - 0.5 TOPS NPU
  - 68-pin QFN package
* Micro SD card slot
* USB-C connector
* Nuvoton NAU88C22 stereo codec
* Misc GPIO
  - SPI
  - I2C
  - GPIO
* Four channels of 3.3V multiplexed A/D input

## Design Materials

* [Schematic](./doc/dspod_cv1800b_schematic.pdf)

## Software

The CV1800B SoC runs a Buildroot / Busybox version of Linux based on a 5.10 kernel
with vendor-customized extensions (not upstreamed). The vendor SDK is freely
available here [Milk-V SDK V2](https://github.com/milkv-duo/duo-buildroot-sdk-v2)

I've customized the kernel with drivers and device-tree configurations that are
specific to this board. My fork of the SDK is here
[Customize kernel SDK](https://github.com/emeb/duo-buildroot-sdk-v2/tree/audio_board)

Additionally, there is a user-space application that provides the UI for the board
along with drivers for the audio codec and 4-channel ADC - find the source here
[dspod app](https://github.com/emeb/duo-examples/tree/main/dspod_app)
