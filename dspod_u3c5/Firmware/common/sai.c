/*
 * sai.c - sai driver for dspod_u3c5
 * 05-05-26 E. Brombugh
 */

#include <string.h>
#include "sai.h"
#include "printf.h"

#define DMA_TX_CHL GPDMA1_Channel0
#define DMA_RX_CHL GPDMA1_Channel1
#define DMA_TX_CHL_IRQn GPDMA1_Channel0_IRQn
#define DMA_RX_CHL_IRQn GPDMA1_Channel1_IRQn
#define DMA_TX_CHL_IRQHandler GPDMA1_Channel0_IRQHandler
#define DMA_RX_CHL_IRQHandler GPDMA1_Channel1_IRQHandler

/* uncomment to enable IRQ diag on RX pin */
#define DIAG

#ifdef DIAG
/* diag on dspod gate-out */
#define DIAG_LOW()	(GPIOC->BSRR=(GPIO_PIN_7<<16))
#define DIAG_HIGH()	(GPIOC->BSRR=GPIO_PIN_7)
#else
#define DIAG_LOW()
#define DIAG_HIGH()
#endif

/* data structures used by HAL for tracking HW state */
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;

/* structures used by linked-list mode which is essential for circular DMA */
DMA_NodeTypeDef Node_GPDMA1_Channel1;
DMA_QListTypeDef List_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_NodeTypeDef Node_GPDMA1_Channel0;
DMA_QListTypeDef List_GPDMA1_Channel0;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* DMA buffers */
int16_t tx_buffer[SAI_BUFSZ], rx_buffer[SAI_BUFSZ], in_buffer[SAI_BUFSZ/2];

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void)
{
	printf("!!! sai error handler !!!\n\r");
	
	while(1) {}
}

/*
 * setup I2S w/ full duplex, DMA and IRQ
 */
void sai_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
		
#if 0
	/* preload buffer w/ static ramp */
	for(int16_t i=0;i<SAI_BUFSZ/2;i++)
	{
		tx_buffer[2*i] = i*(32768/(SAI_BUFSZ/2))-16384;
		tx_buffer[2*i+1] = 16383-i*(32768/(SAI_BUFSZ/2));
	}
#endif

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SAI1_A_Block_A,B GPIO Configuration
    PC3     ------> SAI1_SD_A
    PB5     ------> SAI1_SD_B
    PB10     ------> SAI1_SCK_A
    PB8     ------> SAI1_MCLK_A
    PB9     ------> SAI1_FS_A
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_SAI1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#ifdef DIAG
	/* Configure diagnostic output pin on PA0 (gate-out) --------------------*/
	GPIO_InitStruct.Pin =  GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
	
    /* SAI Init */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_MSIK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
		Error_Handler();
    }

	__HAL_RCC_SAI1_CLK_ENABLE();

	hsai_BlockA1.Instance = SAI1_Block_A;
	hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
	hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
	hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
	hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
	hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
	hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
	hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
	hsai_BlockA1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
	hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
	hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
	hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
	if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
	{
		Error_Handler();
	}
	hsai_BlockB1.Instance = SAI1_Block_B;
	hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_RX;
	hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
	hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
	hsai_BlockB1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
	hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
	hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
	hsai_BlockB1.Init.MckOutput = SAI_MCK_OUTPUT_ENABLE;
	hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
	hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
	hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
	if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* turn on TX/RX DMA req */
    SET_BIT(hsai_BlockA1.Instance->CR1, SAI_xCR1_DMAEN);   
    SET_BIT(hsai_BlockB1.Instance->CR1, SAI_xCR1_DMAEN);   

    /* DMA Init */
	__HAL_RCC_GPDMA1_CLK_ENABLE();
	
	/* CubeMX generated code uses LL mode w/ one node */
	DMA_NodeConfTypeDef NodeConfig;
	
	/* GPDMA1_REQUEST_SPI1_TX Init */
    NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    NodeConfig.Init.Request = GPDMA1_REQUEST_SAI1_A;
    NodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    NodeConfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
    NodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
    NodeConfig.Init.DestInc = DMA_DINC_FIXED;
    NodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    NodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    NodeConfig.Init.SrcBurstLength = 1;
    NodeConfig.Init.DestBurstLength = 1;
    NodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
    NodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    NodeConfig.Init.Mode = DMA_NORMAL;
    NodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    NodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    NodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    NodeConfig.SrcAddress = (uint32_t)tx_buffer;
    NodeConfig.DstAddress = (uint32_t)&((hsai_BlockA1.Instance)->DR);
    NodeConfig.DataSize = SAI_BUFSZ*sizeof(int16_t);	// BYTES!!!
    if (HAL_DMAEx_List_BuildNode(&NodeConfig, &Node_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_InsertNode(&List_GPDMA1_Channel0, NULL, &Node_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_SetCircularMode(&List_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

	/* suggested by docs - needed? */
    if (HAL_DMAEx_List_ConvertQToDynamic(&List_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    handle_GPDMA1_Channel0.Instance = DMA_TX_CHL;
    handle_GPDMA1_Channel0.InitLinkedList.Priority = DMA_HIGH_PRIORITY;
    handle_GPDMA1_Channel0.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    handle_GPDMA1_Channel0.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
    handle_GPDMA1_Channel0.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel0.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
    if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel0, &List_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel0, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
      Error_Handler();
    }
		
	/* GPDMA1_REQUEST_SPI1_RX Init */
    NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    NodeConfig.Init.Request = GPDMA1_REQUEST_SAI1_B;
    NodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    NodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
    NodeConfig.Init.SrcInc = DMA_SINC_FIXED;
    NodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
    NodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    NodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    NodeConfig.Init.SrcBurstLength = 1;
    NodeConfig.Init.DestBurstLength = 1;
    NodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    NodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    NodeConfig.Init.Mode = DMA_NORMAL;
    NodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    NodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    NodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    NodeConfig.SrcAddress = (uint32_t)&((hsai_BlockB1.Instance)->DR);
    NodeConfig.DstAddress = (uint32_t)rx_buffer;
    NodeConfig.DataSize = SAI_BUFSZ*sizeof(int16_t);	// BYTES!!!
	
    if (HAL_DMAEx_List_BuildNode(&NodeConfig, &Node_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_InsertNode(&List_GPDMA1_Channel1, NULL, &Node_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_SetCircularMode(&List_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }

	/* suggested by docs - needed? */
    if (HAL_DMAEx_List_ConvertQToDynamic(&List_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }
	
    handle_GPDMA1_Channel1.Instance = DMA_RX_CHL;
    handle_GPDMA1_Channel1.InitLinkedList.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
    handle_GPDMA1_Channel1.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    handle_GPDMA1_Channel1.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
    handle_GPDMA1_Channel1.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel1.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
    if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel1, &List_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel1, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
      Error_Handler();
    }
	
	/* start up DMA */	
	uint32_t cllr_mask = DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;
	handle_GPDMA1_Channel0.Instance->CLBAR = ((uint32_t)handle_GPDMA1_Channel0.LinkedListQueue->Head & DMA_CLBAR_LBA);
	handle_GPDMA1_Channel0.Instance->CLLR  = ((uint32_t)handle_GPDMA1_Channel0.LinkedListQueue->Head & DMA_CLLR_LA) | cllr_mask;
	handle_GPDMA1_Channel1.Instance->CLBAR = ((uint32_t)handle_GPDMA1_Channel1.LinkedListQueue->Head & DMA_CLBAR_LBA);
	handle_GPDMA1_Channel1.Instance->CLLR  = ((uint32_t)handle_GPDMA1_Channel1.LinkedListQueue->Head & DMA_CLLR_LA) | cllr_mask;
	
	/* enable TC/HT IRQs on chl 0 (TX) */
	__HAL_DMA_ENABLE_IT(&handle_GPDMA1_Channel0, (DMA_IT_TC | DMA_IT_HT));
	
	/* enable IRQ for GPDMA1 Chl0 (TX) */
	HAL_NVIC_SetPriority(DMA_TX_CHL_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA_TX_CHL_IRQn);

	/* enable both channels */
    __HAL_DMA_ENABLE(&handle_GPDMA1_Channel0);
    __HAL_DMA_ENABLE(&handle_GPDMA1_Channel1);	
}

/*
 * start the SAI port running. Assumes all GPIO, SAI & DMA stuff already set up
 */
void sai_start(void)
{
	/* turn on the SAI blocks */
	__HAL_SAI_ENABLE(&hsai_BlockA1);
	__HAL_SAI_ENABLE(&hsai_BlockB1);
}

/*
 * compute I2S sample rate
 */
int32_t sai_get_fsamp(void)
{
	uint32_t freq;
	
	freq = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SAI1);
	freq /= ((hsai_BlockA1.Instance)->CR1 & SAI_xCR1_MCKDIV_Msk)>>SAI_xCR1_MCKDIV_Pos;
	
	if(!((hsai_BlockA1.Instance)->CR1 & SAI_xCR1_NODIV_Msk))
	{
		/* MCLK enabled */
		if((hsai_BlockA1.Instance)->CR1 & SAI_xCR1_OSR_Msk)
		{
			freq /= 512;
		}
		else
		{
			freq /= 256;
		}
	}
	
	return freq;
}

/**
  * GPDMA1 Channel0 is SAI TX TC/HT
  */
void DMA_TX_CHL_IRQHandler(void)
{
	/* Raise activity flag */
	DIAG_HIGH();

	/* Half-Transfer interrupt */
	if(DMA_TX_CHL->CSR&DMA_CSR_HTF)
	{
		/* Clear the Interrupt flag */
		DMA_TX_CHL->CFCR = DMA_CFCR_HTF;
		
		/* grab rx from previous */
		memcpy(in_buffer, &rx_buffer[SAI_BUFSZ/2], SAI_BUFSZ/2*sizeof(int16_t));

		/* load the first half of the buffers */
		Audio_Proc(&tx_buffer[0], in_buffer, SAI_BUFSZ/2);
	}
	
	/* Transfer complete interrupt */
	if(DMA_TX_CHL->CSR&DMA_CSR_TCF)
	{
		/* Clear the Interrupt flag */
		DMA_TX_CHL->CFCR = DMA_CFCR_TCF;
		
		/* grab rx from previous */
		memcpy(in_buffer, &rx_buffer[0], SAI_BUFSZ/2*sizeof(int16_t));

		/* load the 2nd half of the buffer */
		Audio_Proc(&tx_buffer[SAI_BUFSZ/2], in_buffer, SAI_BUFSZ/2);
	}
    	
	/* Lower activity flag */
	DIAG_LOW();
}
