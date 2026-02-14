# dspod_rp2350

![dspod_rp2350 daughterboard](./doc/dspod_rp2350.jpg)

The dspod_rp2350 is a daughterboard comprising an RP2350A MCU with 4MB Flash, 8MB PSRAM, USB and GPIO + audio I/O.

## Abstract

This board is a small 32-pin device with the following features:

* RP2350 MCU
  - 68-pin QFN package
  - Dual-core Arm Cortex M33 or Hazard3 RISC-V CPUs 
  - 520kB SRAM
  - USB, I2C, SPI, ADC, GPIO, PIO, etc on-chip
* USB-C connector, full-speed host/device
* Nuvoton NAU88C22 stereo codec
* Misc GPIO
  - SPI
  - I2C
  - GPIO
* Four channels of 3.3V multiplexed A/D input

## Design Materials

* [Schematic](./doc/dspod_rp2350_sch.pdf)
* [BOM Spreadsheet](./doc/dspod_rp2350_BOM.ods)

## Hardware

The hardware design is provided in Kicad 9.x format in the [Hardware](./Hardware) directory.

## Firmware

A collection of test programs for exercising all the peripherals of the dspod_rp2350 can be found in the [Firmware](./Firmware) directory.

## Results

Bringing up the hardware drivers and DSP effects application on the RP2350 was remarkably pain-free and performance is reasonable given the manufacturer's published constraints. Following are some observations:

#### I2S Audio

The PIO peripheral works well for implementing I2S formatted serial I/O with DMA into on-chip SRAM buffers. I use a slightly modified version of the original output-only example code that was provided by the manufacturer and was adapted for full-duplex operation by [Jonathan Brodsky](https://github.com/jonbro). 

#### Multicore processing

This is well-supported by the Pico SDK and straightforward to implement. In the DSP effects application the code was split between the foreground UI and ADC polling running on core 0 and the DSP running on core 1 which worked nicely.

#### PSRAM

The original RP2040 relies on external QSPI flash for nonvolatile storage of code and data and allows execute in-place (XIP) access which maps the external QSPI flash into the CPU address space. This works very well and for the RP2350 a second chip select line and write capability was added that supports RAM. This board uses that capability for audio data buffering and it functions well.

#### Loadable code

An experiment in the effects application was the use of loadable code sections via "overlays". This requires some fancy tricks in the linker and function attributes to pull chunks of executable code from off-chip flash into on-chip CCMRAM on demand in order to improve code execution speed while limiting the resources required for large code bases. This worked well and allows maximum code execution speed while not impacting RAM usage heavily.

#### Clock Speed

As of February 2026, the RP2350 is specified to run at a maximum clock speed of 150MHz with a VREG voltage of 1.1V. Experiments in the user community have shown however that it can be reliably run at much higher speeds if the VREG setting is increased and proper cooling is provided - see this article for details: [Overclocking Raspberry Pi Pico 2](https://learn.pimoroni.com/article/overclocking-the-pico-2). By default, the dspod_rp2350 is set to operate at 159.750MHz - slightly overclocked to provide a nearly perfect 48001Hz audio sample rate, but by changing a few settings in the CMakelists.txt file it's possible to double the clock rate to nearly 320MHz with only a slight increase in the VREG setting (firmware controlled) which significantly improves the CPU loading of the more complex DSP. Further increases may be possible, and if rates near 500MHz are acheivable then FFT-based spectral effects may be feasible on this platform. Further experiments are in process to see where the limits are. 

#### Performance

Overall, the RP2350 platform provides a lot of powerful options for audio DSP, especially if pushed beyond the conservative limits specified by the manufacturer. The MCU itself is one of the least expensive ones tested on the dspod ecosystem despite providing two CPU cores and lots of on-chip SRAM and it has a lot of potential that hasn't been fully explored.
