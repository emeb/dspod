# dspod_u3c5 Firmware

This directory contains various makefile-based projects for testing out the dspod_h7r3. There are projects, some with overlapping functionality:

- adc - testing out the 4-channel ADC inputs.

- app - a multi-effects application with a number of simple gain, filter and delay algorithms.

- blinky - the traditional blinking LED for initial proof-of-life after assembly.

- encoder - quick test of the rotary encoder & button driver.

- hsp_eng_tst - test of the HSP Engine middleware

- hsp_raw_tst - test of the raw HAL interface to the HSP

- i2s_codec - testing out the I2S I/O channel, I2C control and the NAU88C22 codec.

- lcd - test of the LCD SPI and graphics driver.

- ospi_psram - testing out the 8MB PSRAM attached to the OSPI port.

- pdm - test of the PDM microphone input port, as well as the ADF peripheral used to convert PDM signals into PCM samples.

- si5351 - test of the SI5351 clock generator driver.

## Prerequisites

You'll need to have installed a working Arm GCC toolchain in order to use these projects, as well as a version of Black Magic Probe or OpenOCD that supports the STM32U3C5 for loading code into flash. The Makefiles in each project directory will have to be edited to point to these tools.

### GCC

I use the Arm GCC toolchain from this site: [GCC at Arm.com](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)

### Blackmagic Debug

This is the primary tool that I used for download and debug during this project. It consists of a hardware USB interface to JTAG/SWD that operates over a USB ACM port and interfaces directly with Arm GDB. Find out more about it these sites:

-  [Blackmagic Debug at Codeberg](https://codeberg.org/blackmagic-debug/blackmagic)

- [Blackmagic Probe at 1bitsquared](https://1bitsquared.com/products/black-magic-probe)

### OpenOCD

OpenOCD can used to communicate with SWD port on the STM32U3C5 MCU in order to load binary code into the on-chip flash memory. The mainline tool is fairly slow to change and can take several years to add support for new devices, so the STM32H7R3 is not yet in the database for mainline. ST however provides a forked version of OpenOCD that quickly adds their own products and is available in sevral ways:

- Directly from their GitHub repository: [GitHub - STMicroelectronics/OpenOCD: STMicroelectronics customized version of OpenOCD supporting STM32 MCUs and MPUs](https://github.com/STMicroelectronics/OpenOCD)

- With the installation of their proprietary [STM32CubeIDE | Software - STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeide.html) 

## Building

The following steps should be sufficient to build each project:

```
cd [your chosen project]
make
make gdb_flash
```
