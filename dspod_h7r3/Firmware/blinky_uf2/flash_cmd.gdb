file main.elf

#target extended-remote /dev/ttyACM0
target extended-remote :2000

mon swd_scan

attach 1

mon erase_mass
#mon erase 0x08000000 8192

load

compare-sections

#dump binary memory h7.bin 0x08000000 0x08002000

quit
