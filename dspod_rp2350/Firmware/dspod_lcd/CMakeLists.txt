cmake_minimum_required(VERSION 3.13...3.27)

# to build for RP2350 ARM you must do this before including pico_sdk_import
set(PICO_PLATFORM rp2350)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# since the SDK can't find picotool installed in /usr/local
#set(picotool_DIR /home/ericb/picotool_dir/picotool)

project(dspod_lcd)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(dspod_lcd
        main.c
		cyclesleep.c
		gfx.c
		st7789.c
)

#pico_enable_stdio_usb(dspod_lcd 1)
pico_enable_stdio_uart(dspod_lcd 1)

# Adjust system clock setup for 159.75MHz which gives nearly 48kHz sample rate
target_compile_definitions(dspod_lcd PRIVATE
	PLL_SYS_REFDIV=2
	PLL_SYS_VCO_FREQ_HZ=1278000000
	PLL_SYS_POSTDIV1=4
	PLL_SYS_POSTDIV2=2
	SYS_CLK_HZ=159750000
)

# pull in common dependencies
target_link_libraries(dspod_lcd
	pico_stdlib
	pico_unique_id
	hardware_spi
	pico_multicore
	cmsis_core
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dspod_lcd)


