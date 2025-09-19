cmake_minimum_required(VERSION 3.13...3.27)

# to build for RP2350 ARM you must do this before including pico_sdk_import
set(PICO_PLATFORM rp2350)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# since the SDK can't find picotool installed in /usr/local
set(picotool_DIR /home/ericb/picotool_dir/picotool)


project(dspod_blink)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(dspod_blink
        dspod_blink.c
)

# pull in common dependencies
target_link_libraries(dspod_blink pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(dspod_blink)


