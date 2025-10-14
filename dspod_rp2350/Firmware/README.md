# dspod_rp2350 Firmware

This directory contains various RPi Pico SDK projects for testing out the dspod_rp2350. There are six separate projects, some with overlapping functionality:

- dspod_app - the primary application that ties everything together to realize a multi-effects processor with stereo audio I/O, 4 CV inputs, an LCD + encoder UI and a small selection of effects. This release includes a limited set of effects including a gain control, several filters and a simple delay.

- dspod_blinky - the traditional blinking LED for initial proof-of-life after assembly.

- dspod_adc - testing out the 4-channel ADC inputs.

- dspod_i2s - testing out the PIO-based I2S I/O channel and the NAU88C22 codec.

- dspod_lcd - testing out the 320x170  IPS LCD on the dspod_module.

- dspod_psram - testing out the 8MB PSRAM attached to the RP2350.

- dspod_test - combination of all basic drivers for constructing an audio application.

## Prerequisites

You'll need to have installed the RPi Pico C SDK in order to use these projects. Find out more about installation and use of that toolchain here: [GitHub - raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)

## Building

Once the Pico SDK is installed and properly set up the following steps should be sufficient to build each project:

```
cd [your chosen project]
mkdir build
cd build
cmake ..
make
```
