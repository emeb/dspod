/*
 * fx.c - Algorithm access for dspod up5k
 * 06-18-26 E. Brombaugh
 */
 
#include "fx.h"
#include "codec.h"
#include "clkcnt.h"
#include "flash.h"
#include "mv_names.h"

/* persistent vars */
uint16_t fx_algo;	// currently active algo
uint8_t fx_buf;		// which buffer of microcode is in use

/* temp microcode buffer */
uint16_t uc_buff[128];

/*
 * initialize the effects library
 */
uint8_t fx_init(void)
{	
	/* start off with bypass algo */
	fx_algo = 0;
	fx_buf = 0;
	
	flash_init(SPI0);	// wake up the flash chip
	
	return 0;
}

/*
 * switch algorithms
 */
void fx_select_algo(uint16_t algo)
{
	/* mute */
	Codec_Mute(CODEC_MUTE_ON);
	clkcnt_delayms(10);
	
	/* fetch desired algo into temp buffer */
	flash_read(SPI0, (uint8_t *)uc_buff, 0x110000 + 256*algo, 256);
	
	/* diag - print first few half-words in temp buffer for check */
	//for(int i=0;i<8;i++)
	//	printf("%02X : %04X\n\r", i, uc_buff[i]);
	
	/* copy algo into unused page of uc store */
	uint8_t next_buf = fx_buf ^ 1;
	uint32_t *ucode = (uint32_t *)(MVB_BASE + (next_buf*sizeof(uint32_t)*128));
	for(int i=0;i<128;i++)
		*ucode++ = uc_buff[i];
	
	/* switch buffers and update algo */
	fx_buf = next_buf;
	gp_out = (gp_out & ~GP_OUT_ALGO) | (fx_buf & GP_OUT_ALGO);
	
	/* wait for current prog to update from HW */
	while(((gp_in>>GP_IN_HW_PROG_POS)&GP_IN_HW_PROG_MASK) != fx_buf);
	fx_algo = algo;
	
	/* umute */
	clkcnt_delayms(10);
	Codec_Mute(CODEC_MUTE_OFF);
}

/*
 * get current algorithm
 */
uint16_t fx_get_algo(void)
{
	return fx_algo;
}

/*
 * get name of any effect
 */
char * fx_get_algo_name(uint16_t algo_num)
{
	return (char *)mv_prog_names[algo_num];
}

/*
 * get name of current effect
 */
char * fx_get_curr_algo_name(void)
{
	return fx_get_algo_name(fx_algo);
}

