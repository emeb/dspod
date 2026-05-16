# GDB command file for testing BMDA

file main.elf

target extended-remote /dev/ttyACM0
#target extended-remote :2000

mon swd_scan

attach 1

