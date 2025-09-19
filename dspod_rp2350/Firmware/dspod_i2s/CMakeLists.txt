cmake_minimum_required(VERSION 3.13...3.27)

# to build for RP2350 ARM you must do this before including pico_sdk_import
set(PICO_PLATFORM rp2350)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# since the SDK can't find picotool installed in /usr/local
#set(picotool_DIR /home/ericb/picotool_dir/picotool)

# board is PICO PLUS 2
# use `cmake -DPICO_BOARD=pimoroni_pico_plus2_rp2350 ..` when starting

# programming
# either reset to bootloader and copy rp2350_psram.uf2 to RP2350 device or
# pico_flash_rp2350 rp2350_i2s.elf

project(dspod_i2s)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(dspod_i2s
        main.c
		cyclesleep.c
		i2s_fulldup.c
		audio.c
		nau88c22.c
)

#pico_enable_stdio_usb(dspod_i2s 1)
pico_enable_stdio_uart(dspod_i2s 1)

# build PIO stuff
pico_generate_pio_header(dspod_i2s ${CMAKE_CURRENT_LIST_DIR}/i2s_fulldup.pio)

# Adjust system clock setup for 159.75MHz which gives nearly 48kHz sample rate
target_compile_definitions(dspod_i2s PRIVATE
	PLL_SYS_REFDIV=2
	PLL_SYS_VCO_FREQ_HZ=1278000000
	PLL_SYS_POSTDIV1=4
	PLL_SYS_POSTDIV2=2
	SYS_CLK_HZ=159750000
)

# pull in common dependencies
target_link_libraries(dspod_i2s
	pico_stdlib
	pico_unique_id
	hardware_pio
	hardware_dma
	hardware_i2c
	pico_multicore
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dspod_i2s)


