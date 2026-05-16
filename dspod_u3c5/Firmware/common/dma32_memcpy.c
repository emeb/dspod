/*
 * dma32_memcpy.c - dma version of memcpy for 32-bit aligned values
 * 05-14-2026 E. Brombaugh
 */

#include "dma32_memcpy.h"

DMA_HandleTypeDef handle_GPDMA1_Channel4;

/*
 * init the DMA channel for mem2mem 
 */
HAL_StatusTypeDef dma32_memcpy_init(void)
{
	__HAL_RCC_GPDMA1_CLK_ENABLE();

	handle_GPDMA1_Channel4.Instance = GPDMA1_Channel4;
	handle_GPDMA1_Channel4.Init.Request = DMA_REQUEST_SW;
	handle_GPDMA1_Channel4.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
	handle_GPDMA1_Channel4.Init.Direction = DMA_MEMORY_TO_MEMORY;
	handle_GPDMA1_Channel4.Init.SrcInc = DMA_SINC_INCREMENTED;
	handle_GPDMA1_Channel4.Init.DestInc = DMA_DINC_INCREMENTED;
	handle_GPDMA1_Channel4.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
	handle_GPDMA1_Channel4.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
	handle_GPDMA1_Channel4.Init.Priority = DMA_HIGH_PRIORITY;
	handle_GPDMA1_Channel4.Init.SrcBurstLength = 1;
	handle_GPDMA1_Channel4.Init.DestBurstLength = 1;
	handle_GPDMA1_Channel4.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
	handle_GPDMA1_Channel4.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
	handle_GPDMA1_Channel4.Init.Mode = DMA_NORMAL;
	if (HAL_DMA_Init(&handle_GPDMA1_Channel4) != HAL_OK)
	{
		return HAL_ERROR;
	}
	if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel4, DMA_CHANNEL_NPRIV) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

/*
 * do a copy 
 */
void dma32_memcpy(void *dst, void *src, uint32_t len)
{
#if 0
	/* kick off a move */
	HAL_DMA_Start(&handle_GPDMA1_Channel4, (uint32_t)src, (uint32_t)dst, len);
	
	/* wait for it to finish */
	HAL_DMA_PollForTransfer(&handle_GPDMA1_Channel4, HAL_DMA_FULL_TRANSFER, 10);
#else
	/* get the channel */
	DMA_Channel_TypeDef *chl = handle_GPDMA1_Channel4.Instance;
	
	/* Configure the DMA channel data size (bytes!) */
	chl->CBR1 = len & DMA_CBR1_BNDT;
	
	/* Clear all interrupt flags */
	chl->CFCR = DMA_FLAG_TC | DMA_FLAG_HT | DMA_FLAG_DTE | DMA_FLAG_ULE | DMA_FLAG_USE | DMA_FLAG_SUSP |
					   DMA_FLAG_TO;
	
	/* Configure DMA channel source address */
	chl->CSAR = (uint32_t)src;

	/* Configure DMA channel destination address */
	chl->CDAR = (uint32_t)dst;
	
	/* start it */
	chl->CCR |=  DMA_CCR_EN;
	
	/* wait for transfer complete */
	while((chl->CSR & DMA_FLAG_IDLE) == 0U);
#endif
}

