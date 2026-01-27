/*
 * psram_buff.h - read & write buffering for the xspi psram indirect mode
 * 11-22-2025 E. Brombaugh
 */
#include "psram_buff.h"
#include "xspi.h"

/*
 * initialize a buffer structure - this works for both read and write
 * pbf is pointer to psram buffer object
 * mem is pointer to local buffer in internal SRAM
 * bufsz is length of local buffer in internal SRAM 
 * psram_start is beginning of circular buffer in psram
 * psram_sz is length of circular buffer in psram
 * returns size of internal buffer in words
 */
uint32_t psbuf_init(psbuf_struct *pbf, uint32_t *mem, uint32_t bufsz,
	uint32_t psram_start, uint32_t psram_sz)
{
	/* init the struct */
	pbf->buf = (uint8_t *)mem;
	pbf->bufsz = bufsz;
	pbf->psram_start = psram_start;
	pbf->psram_sz = psram_sz;
	pbf->begin_addr = 0;
	pbf->next_addr = 0;
	pbf->fill_cnt = 0;
	pbf->idx = 0;
	
	/* round up buffer length to words */
	return (bufsz + (sizeof(uint32_t)-1))/sizeof(uint32_t);
}

/*
 * write bytes to the buffer and flush to PSRAM when buffer is full
 * pbf is pointer to psram buffer object
 * addr is location in psram - if not contiguous then buffer is flushed first
 * data is pointer to array of bytes to put into local buffer
 * sz is length of array to put into local buffer
 */
void psbuf_writebytes(psbuf_struct *pbf, uint32_t addr, uint8_t *data,
	uint32_t sz)
{
	/* flush first? */
	if((addr != pbf->next_addr) && (pbf->fill_cnt != 0))
	{
		/* non-contiguous address with data already waiting */
		xspi_psram_writebytes(pbf->psram_start + pbf->begin_addr, pbf->buf,
			pbf->fill_cnt);
		pbf->fill_cnt = 0;
		pbf->next_addr = addr;
	}
	
	/* iterate over input array */
	while(sz--)
	{
		/* set start addr at beginning of buffer */
		if(pbf->fill_cnt == 0)
		{
			pbf->begin_addr = addr;
			pbf->next_addr = addr;
		}
			
		/* put data in buffer & update addrs */
		pbf->buf[pbf->fill_cnt++] = *data++;
		pbf->next_addr++;
		addr++;
		
		/* check if time to flush */
		if(pbf->fill_cnt == pbf->bufsz)
		{
			uint32_t psram_addr = pbf->psram_start + pbf->begin_addr;
			
			/* check for wrap */
			if((psram_addr + pbf->fill_cnt) < (pbf->psram_start + pbf->psram_sz))
			{
				/* write w/o wrap */
				xspi_psram_writebytes(psram_addr, pbf->buf, pbf->fill_cnt);
				
				/* reset fill count - address will update at next iteration */
				pbf->fill_cnt = 0;
			}
			else
			{
				/* write only to end of psram */
				uint32_t tlen = pbf->psram_sz - psram_addr;
				xspi_psram_writebytes(psram_addr, pbf->buf, tlen);
				
				/* adjust tracking */
				pbf->begin_addr += tlen;
				pbf->fill_cnt -= tlen;
				
				/* move remaining data down to beginning */
				for(int i=0;i<pbf->fill_cnt;i++)
					pbf->buf[i] = pbf->buf[i+tlen];
			}
		}	/* end of flush check */
	}	/* end of while(sz--) */
}	/* end function */

/*
 * read bytes from buffer and fetch from PSRAM with readahead when empty
 * pbf is pointer to psram buffer object
 * addr is location in psram - can be discontinuous but will trigger a read
 * data is pointer to array of bytes to get from local buffer
 * sz is length of array to get from local buffer
 */
void psbuf_readbytes(psbuf_struct *pbf, uint32_t addr, uint8_t *data,
	uint32_t sz)
{
	/* fetch first? */
	if((addr != pbf->next_addr) || (pbf->fill_cnt == 0))
	{
		pbf->begin_addr = addr;
		pbf->next_addr = addr;
		pbf->idx = 0;
		
		uint32_t psram_addr = pbf->psram_start + pbf->begin_addr;
		
		/* wrap? */
		if((psram_addr+pbf->bufsz) < (pbf->psram_start + pbf->psram_sz))
		{
			/* no wrap */
			xspi_psram_readbytes(psram_addr, pbf->buf, pbf->bufsz);
			pbf->fill_cnt = pbf->bufsz;
			pbf->begin_addr += pbf->bufsz;
		}
		else
		{
			/* read only to end of psram */
			uint32_t tlen = pbf->psram_sz - psram_addr;
			xspi_psram_readbytes(psram_addr, pbf->buf, tlen);
			pbf->fill_cnt = tlen;
			pbf->begin_addr += tlen;
		}
				
		/* reset begin addr? */
		if(pbf->begin_addr == pbf->psram_start + pbf->psram_sz)
			pbf->begin_addr = pbf->psram_start;
	}
	
	/* iterate over output array */
	while(sz--)
	{
		/* get data from local buffer */
		*data++ = pbf->buf[pbf->idx++];
		pbf->next_addr++;
		pbf->fill_cnt--;
		
		/* time to fetch? */
		if(pbf->fill_cnt == 0)
		{
			uint32_t psram_addr = pbf->psram_start + pbf->begin_addr;
			
			pbf->idx = 0;
			
			/* wrap? */
			if((psram_addr+pbf->bufsz) < (pbf->psram_start + pbf->psram_sz))
			{
				/* no wrap */
				xspi_psram_readbytes(psram_addr, pbf->buf, pbf->bufsz);
				pbf->fill_cnt = pbf->bufsz;
				pbf->begin_addr += pbf->bufsz;
			}
			else
			{
				/* read only to end of psram */
				uint32_t tlen = pbf->psram_sz - psram_addr;
				xspi_psram_readbytes(psram_addr, pbf->buf, tlen);
				pbf->fill_cnt = tlen;
				pbf->begin_addr += tlen;
			}
				
			/* reset begin addr? */
			if(pbf->begin_addr == pbf->psram_start + pbf->psram_sz)
				pbf->begin_addr = pbf->psram_start;
		}	/* end of fetch check */
	}	/* end of while(sz--) */
}	/* end function */
