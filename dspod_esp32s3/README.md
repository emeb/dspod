# dspod_rp2350

![dspod_cv1800b daughterboard](./doc/dspod_esp32s3.png)

The dspod_esp32s3 is a daughterboard comprising an ESP32 S3 MCU with 4MB Flash, 2MB PSRAM, USB and GPIO + audio I/O.

## Abstract

This board is a small 32-pin device with the following features:

* ESP32 S3 MCU
  - 56-pin QFN package
  - Dual-core Extensa LX7 CPUs 
  - 512kB SRAM
  - USB, I2C, SPI, ADC, GPIO, WiFi, etc on-chip
* USB-C connector, full-speed host/device
* Nuvoton NAU88C22 stereo codec
* Misc GPIO
  - SPI
  - I2C
  - GPIO
* Four channels of 3.3V multiplexed A/D input
* RF connector for off-board antenna

## Design Materials

* [Schematic](./doc/dspod_esp32s3_sch.pdf)

## Hardware

The hardware design is provided in Kicad 9.x format in the [Hardware](./Hardware) directory.

## Firmware

Firmware is TBD but will be derived from prior work on the [S3GTA](https://github.com/emeb/S3GTA) module.
