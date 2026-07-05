/*
 * mk_mvucode_flash.c - parse raw microcode ROMs into binary for flash
 * 09-04-2021 E. Brombaugh
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ROMS 4

int main(int argc, char **argv)
{
	char *uname[NUM_ROMS] = {
		"midifverb.bin",
		"midifex.bin",
		"mv_special_rom.bin",
		"mv2_rom.bin"
	};
	uint8_t num_algos[NUM_ROMS] = {
		64, 64, 64, 99
	};
	uint16_t offset[NUM_ROMS] = {
		0, 0, 0, 0x1d00
	};
	
	char *oname = "mv_roms.bin";
	FILE *ufile, *ofile;
	uint8_t rom[256*99], prog, op;
	uint16_t r, i, addr, instr;
	
	/* open output file */
	if(!(ofile = fopen(oname, "wb")))
	{
		fprintf(stderr, "Couldn't open output file %s for write\n", oname);
		exit(1);
	}
	
	/* loop over 3 roms */
	for(r=0;r<NUM_ROMS;r++)
	{
		/* load microcode */
		if(!(ufile = fopen(uname[r], "rb")))
		{
			fprintf(stderr, "Couldn't open microcode file %s for read\n", uname[r]);
			exit(1);
		}
		
		/* skip non-algo data */
		fseek(ufile, offset[r], SEEK_SET);
		
		/* load algo data */
		uint32_t szbytes = num_algos[r]*256;
		if(fread(rom, sizeof(uint8_t), szbytes, ufile) != szbytes)
		{
			fprintf(stderr, "Unexepected EOF in microcode file %s.\n", uname[r]);
			fclose(ufile);
			exit(1);
		}
		fclose(ufile);

		/* loop over all programs except last which is just a dummy/disable */
		for(prog=0;prog<num_algos[r];prog++)
		{
			/* loop over all instructions */
			for(i=0;i<128;i++)
			{
				/* build instruction */
				if(r<3)
				{
					/* MV1 format */
					op = (rom[(prog<<8) + ((i*2-3)&0xff)] >> 6) & 0x03;
					addr = rom[(prog<<8) + ((i*2-2)&0xff)] +
							((rom[(prog<<8) + ((i*2-1)&0xff)] & 0x3f) << 8);
				}
				else
				{
					/* MV2 format */
					op = (rom[(prog<<8) + ((i*2-2)&0xff)] >> 6) & 0x03;
					addr = rom[(prog<<8) + ((i*2-1)&0xff)] +
							((rom[(prog<<8) + ((i*2)&0xff)] & 0x3f) << 8);
				}
				instr = (op<<14) + addr;
				fwrite(&instr, sizeof(uint16_t), 1, ofile);
			}
		}
	}
	
	fclose(ofile);
}

	