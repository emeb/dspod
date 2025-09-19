cmake_minimum_required(VERSION 3.13...3.27)

# to build for RP2350 ARM you must do this before including pico_sdk_import
set(PICO_PLATFORM rp2350)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# since the SDK can't find picotool installed in /usr/local
#set(picotool_DIR /home/ericb/picotool_dir/picotool)

# programming
# either reset to bootloader and copy dspod_adc.uf2 to RP2350 device or
# pico_flash_rp2350 dspod_adc.elf

project(dspod_adc)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(dspod_adc
        dspod_adc.c
		cyclesleep.c
		adc.c
)

#pico_enable_stdio_usb(dspod_adc 1)
pico_enable_stdio_uart(dspod_adc 1)

# pull in common dependencies
target_link_libraries(dspod_adc
	pico_stdlib
	pico_unique_id
	hardware_adc
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dspod_adc)


