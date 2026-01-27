# dspod_esp32s3 Firmware

This directory contains various projects for testing out the dspod_esp32s3. There are projects, some with overlapping functionality:

- blinky - the traditional blinking LED for initial proof-of-life after assembly.

- adc - testing out the 4-channel ADC inputs.

- i2s_codec - testing out the I2S I/O channel, I2C control and the NAU88C22 codec.

- encoder - testing out the rotary encoder and buttons.

- lcd - testing out the SPI LCD display and graphics primitives.

- app - a multi-effects application with a number of simple gain, filter and delay algorithms.

## Prerequisites

You'll need to have installed the Espressif toolchain known as 'esp-idf'. These projects were all built with version 5.5.2 which was the most recent stable version at the time of this writing. You can find the installation instructions here:

[ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/index.html)

## Building

The following steps should be sufficient to build each project:

```
cd [your chosen project]
idf.py build
```

## Installing

Flashing the application code to the hardware is fairly easy due to the built-in USB connectivity on the ESP32S3. Connect the dspod_esp32s3 USB jack to your host machine and use the following command from within the project directory:

```
idf.py -p /dev/ttyACM0 flash monitor
```

This should download the code to the device and present you with a monitor of the logs. When you're ready to exit the monitor just type `ctrl-[`
