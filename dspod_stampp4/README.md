# dspod_stampp4

![dspod_esp32s3 daughterboard](./doc/stampp4_front.jpg)



The dspod_stampp4 is an audio daughterboard based on the M5Stack Stamp P4 which provides n ESP32 P4 MCU with 4MB Flash, 32MB PSRAM, USB and GPIO. The M5Stack module is used because the bare ESP32P4 devices were not available from the usual hobbyist distribution channels at the time of development.

## Abstract

This board is a small 32-pin device with the following features:

* M5Stack Stamp P4 module containing 
  - ESP32S3H4R2 MCU
    - 68-pin QFN package
    - Dual-core RISC V CPUs 
    - 512kB SRAM
    - 32MB PSRAM in package
    - USB, I2C, SPI, ADC, GPIO, etc on-chip
  - 16MB QSPI flash
  - USB-C Connector for development
  - Various connectors for MIPI, SD
* Nuvoton NAU88C22 stereo codec
* Misc GPIO
  - SPI
  - I2C
  - GPIO
* Four channels of 3.3V multiplexed A/D input

## Design Materials

* [Schematic](./doc/dspod_stampp4_sch.pdf)

## Hardware

The hardware design is provided in Kicad 9.x format in the [Hardware](./Hardware) directory.

## Firmware

Firmware is available in the [Firmware](./Firmware) directory.

## Results

The hardware implementation is very similar to the [dspod_esp32s3](../dspod_esp32s3/README.md) so there weren't any big surprises in the overall performance. The encoder/button/LCD-based UI is virtually identical to that on the other three dspod projects so it feels pretty much the same regardless of which instance is in use. The basic effects - filters, delays, reverbs, pitch, frequency and phase shifts - all work the same way and use roughly the right amount of CPU based on their complexity. Some minor changes were required to the ADC and I2S hardware driver code due to the differences between ESP-IDF V5.x and V6.x, as well as migrating from Extensa to RISC-V CPU architecture.

## Going Further

While I've explored most of the highlights, there are a few aspects of this MCU I'd like to explore further:

#### Algorithms

The current complement of effects algorithms are fairly lightweight so I'm looking forward to trying out some more complex things, and while spectral processing is probably not practical, there is CPU bandwidth and RAM capacity for more complex time domain effects.
