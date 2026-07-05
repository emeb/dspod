# Makefile for microcode parser
# 05-01-2021 E. Brombaugh

CC = gcc

# output binary names
MKUC = mk_mvucode_vlog
MKF = mk_mvucode_flash

# Targets
all: $(MKF)

$(MKUC): $(MKUC).c
	$(CC) -g -o $@ $<

$(MKF): $(MKF).c
	$(CC) -g -o $@ $<
	
clean:
	rm -f *.o $(MKUC) $(MKF) mv_roms.bin mv_ucode.hex
	
