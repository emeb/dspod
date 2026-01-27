# dspod_h7r3 Firmware

This directory contains various makefile-based projects for testing out the dspod_h7r3. There are projects, some with overlapping functionality:

- blinky - the traditional blinking LED for initial proof-of-life after assembly.

- blinky_uf2 - the blinking LED demo but designed to be flashed into external QSPI flash memory with the Adafruit TinyUF2 bootloader (see below).

- adc - testing out the 4-channel ADC inputs.

- i2s_codec - testing out the I2S I/O channel, I2C control and the NAU88C22 codec.

- psram - testing out the 8MB PSRAM attached to the XSPI port with the first chip select.

- app - a multi-effects application with a number of simple gain, filter and delay algorithms.

- flash - a test of external QSPI flash memory attached to the XSPI port with the second chip select.

## Prerequisites

You'll need to have installed a working Arm GCC toolchain in order to use these projects, as well as a version of OpenOCD or Black Magic Probe that supports the STM32H7R3 for loading code into flash. The Makefiles in each project directory will have to be edited to point to these tools.

### GCC

I use the Arm GCC toolchain from this site: https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain

### OpenOCD

OpenOCD is the tool used to communicate with SWD port on the STM32H7R3 MCU in order to load binary code into the on-chip flash memory. The mainline tool is fairly slow to change and can take several years to add support for new devices, so the STM32H7R3 is not yet in the database for mainline. ST however provides a forked version of OpenOCD that quickly adds their own products and is available in sevral ways:

- Directly from their GitHub repository: [GitHub - STMicroelectronics/OpenOCD: STMicroelectronics customized version of OpenOCD supporting STM32 MCUs and MPUs](https://github.com/STMicroelectronics/OpenOCD)

- With the installation of their proprietary [STM32CubeIDE | Software - STMicroelectronics](https://www.st.com/en/development-tools/stm32cubeide.html) 

#### Github

This is the version my Makefiles are set up for - it works but requires building from scratch. Clone the repository and make sure to select the `openocd-cubeide-r6` branch, then follow the installation instructions included. I found that when running the `configure` script that I had to include the following option to compile successfully: `--disable-werror`

#### CubeIDE

I was able to use this version to successfully program the STM32H7R3 but it does require some fiddling to discover the location of the executable binary and provide it with the location of the necessary scripts

- Install STM32CubeIDE from the link above (warning - it's big)

- Find the executable binary and add the path to the project Makefile. I found mine at`~/st/stm32cubeide_1.19.0/plugins/com.st.stm32cube.ide.mcu.externaltools.openocd.linux64_2.4.200.202505051030/tools/bin/openocd`

- Make sure to set the environment variable that OpenOCD needs to find its scripts. Mine was at `export OPENOCD_SCRIPTS=~/st/stm32cubeide_1.19.0/plugins/com.st.stm32cube.ide.mcu.debug.openocd_2.3.100.202501240831/resources/openocd/st_scripts`

#### TinyUF2

The TinyUF2 bootloader allows executable firmware to be programmed into external QSPI flash memory via an easy-to-use USB mass storage interface. Although the mainline version of TinyUF2 doesn't support the dspod H7R3 board, I've got a fork which does - find it here: [GitHub - emeb/tinyuf2](https://github.com/emeb/tinyuf2)

To use it, follow these steps:

- clone the repository

- switch to the `stm32h7rs` branch

- `cd ports/stm32h7rs`

- `make BOARD=dspod_h7r3 all`

- flash to the board with your favorite tool.

Note that there are two `BOARD` targets - one for full speed USB and another for high-speed. There's no appreciable difference in flashing speed between them since the flash erase & write timing dominates the process, but if your main application requires one or the other then the UF2 bootloader will support either. 

## Building

The following steps should be sufficient to build each project:

```
cd [your chosen project]
make
export OPENOCD_SCRIPTS=[location of the OpenOCD scripts directory]
make flash
```
