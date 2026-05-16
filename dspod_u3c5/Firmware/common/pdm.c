/*
 * pdm.h - dspod_u3c5 PDM setup
 * 05-06-26 E. Brombaugh
 */

#include "pdm.h"
#include "cyclesleep.h"

/* uncomment to enable IRQ diag on RX pin */
#define DIAG

#ifdef DIAG
#define DIAG_LOW()	(GPIOC->BSRR=(GPIO_PIN_8<<16))
#define DIAG_HIGH()	(GPIOC->BSRR=GPIO_PIN_8)
#else
#define DIAG_LOW()
#define DIAG_HIGH()
#endif

MDF_HandleTypeDef AdfHandle0;
MDF_FilterConfigTypeDef AdfFilterConfig0;
DMA_NodeTypeDef Node_GPDMA1_Channel3;
DMA_QListTypeDef List_GPDMA1_Channel3;
DMA_HandleTypeDef handle_GPDMA1_Channel3;

int32_t pdm_buffer[PDM_BUFSZ], pdm_max = 0, pdm_cnt, pdm_sample[PDM_SAMPSZ];

/*
 * Initialize the PDM input with the ADF periph
 */
HAL_StatusTypeDef PDMInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	pdm_cnt = 0;

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADF1;
	//PeriphClkInit.Adf1ClockSelection = RCC_ADF1CLKSOURCE_HCLK;
	PeriphClkInit.Adf1ClockSelection = RCC_ADF1CLKSOURCE_MSIK;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		return HAL_ERROR;
	}

	/* Enable Clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
	
    /**ADF1 GPIO Configuration
    PB3 (JTDO/TRACESWO)     ------> ADF1_CCK0
    PB4 (NJTRST)     ------> ADF1_SDI0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_ADF1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
#ifdef DIAG
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/* Configure diagnostic output pin on PC8 ------------------------*/
	GPIO_InitStruct.Pin =  GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
	
	__HAL_RCC_ADF1_CLK_ENABLE();

	/*
	 * AdfHandle0 structure initialization and HAL_MDF_Init function call
	 */
	AdfHandle0.Instance = ADF1_Filter0;
	AdfHandle0.Init.CommonParam.ProcClockDivider = 1;
	AdfHandle0.Init.CommonParam.OutputClock.Activation = ENABLE;
	AdfHandle0.Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
	AdfHandle0.Init.CommonParam.OutputClock.Divider = 4;
	AdfHandle0.Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
	AdfHandle0.Init.SerialInterface.Activation = ENABLE;
	AdfHandle0.Init.SerialInterface.Mode = MDF_SITF_NORMAL_SPI_MODE;
	AdfHandle0.Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
	AdfHandle0.Init.SerialInterface.Threshold = 4;
	AdfHandle0.Init.FilterBistream = MDF_BITSTREAM0_FALLING;
	if (HAL_MDF_Init(&AdfHandle0) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	/*
	 * AdfFilterConfig0 structure initialization
	 */
	AdfFilterConfig0.DataSource = MDF_DATA_SOURCE_BSMX;
	AdfFilterConfig0.Delay = 0;
	AdfFilterConfig0.CicMode = MDF_ONE_FILTER_SINC5;
	AdfFilterConfig0.DecimationRatio = 16;
	AdfFilterConfig0.Gain = 1; // 1 = +3.5dB
	AdfFilterConfig0.ReshapeFilter.Activation = ENABLE;
	AdfFilterConfig0.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;
	AdfFilterConfig0.HighPassFilter.Activation = ENABLE;
	AdfFilterConfig0.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;
	AdfFilterConfig0.SoundActivity.Activation = DISABLE;
	AdfFilterConfig0.AcquisitionMode = MDF_MODE_ASYNC_CONT;
	AdfFilterConfig0.FifoThreshold = MDF_FIFO_THRESHOLD_NOT_EMPTY;
	AdfFilterConfig0.DiscardSamples = 0;
	
    /* ADF1 DMA Init */
	__HAL_RCC_GPDMA1_CLK_ENABLE();
	
    /* GPDMA1_REQUEST_ADF1_FLT0 Init */
	DMA_NodeConfTypeDef NodeConfig;
    NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    NodeConfig.Init.Request = GPDMA1_REQUEST_ADF1_FLT0;
    NodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    NodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
    NodeConfig.Init.SrcInc = DMA_SINC_FIXED;
    NodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
    NodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
    NodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    NodeConfig.Init.SrcBurstLength = 1;
    NodeConfig.Init.DestBurstLength = 1;
    NodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
    NodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    NodeConfig.Init.Mode = DMA_NORMAL;
    NodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    NodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    NodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    NodeConfig.SrcAddress = (uint32_t)&((AdfHandle0.Instance)->DFLTDR);
    NodeConfig.DstAddress = (uint32_t)pdm_buffer;
    NodeConfig.DataSize = PDM_BUFSZ*sizeof(int32_t);	// BYTES!!!
    if (HAL_DMAEx_List_BuildNode(&NodeConfig, &Node_GPDMA1_Channel3) != HAL_OK)
    {
		return HAL_ERROR;
    }

    if (HAL_DMAEx_List_InsertNode(&List_GPDMA1_Channel3, NULL, &Node_GPDMA1_Channel3) != HAL_OK)
    {
		return HAL_ERROR;
    }

    if (HAL_DMAEx_List_SetCircularMode(&List_GPDMA1_Channel3) != HAL_OK)
    {
		return HAL_ERROR;
    }

	/* suggested by docs - needed? */
    if (HAL_DMAEx_List_ConvertQToDynamic(&List_GPDMA1_Channel3) != HAL_OK)
    {
      return HAL_ERROR;
    }

    handle_GPDMA1_Channel3.Instance = GPDMA1_Channel3;
    handle_GPDMA1_Channel3.InitLinkedList.Priority = DMA_LOW_PRIORITY_MID_WEIGHT;
    handle_GPDMA1_Channel3.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    handle_GPDMA1_Channel3.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
    handle_GPDMA1_Channel3.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel3.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
    if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel3) != HAL_OK)
    {
		return HAL_ERROR;
    }

    if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel3, &List_GPDMA1_Channel3) != HAL_OK)
    {
      return HAL_ERROR;
    }

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel3, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
		return HAL_ERROR;
    }

	/* Point DMA chl at the linked list */
	uint32_t cllr_mask = DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;
	handle_GPDMA1_Channel3.Instance->CLBAR = ((uint32_t)handle_GPDMA1_Channel3.LinkedListQueue->Head & DMA_CLBAR_LBA);
	handle_GPDMA1_Channel3.Instance->CLLR  = ((uint32_t)handle_GPDMA1_Channel3.LinkedListQueue->Head & DMA_CLLR_LA) | cllr_mask;

	/* enable TC/HT IRQ on chl 2 */
	__HAL_DMA_ENABLE_IT(&handle_GPDMA1_Channel3, DMA_IT_TC | DMA_IT_HT);
	
    HAL_NVIC_SetPriority(GPDMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel3_IRQn);
	
    __HAL_DMA_ENABLE(&handle_GPDMA1_Channel3);
	
	/* start the filter */
	if(HAL_MDF_AcqStart(&AdfHandle0, &AdfFilterConfig0) == HAL_OK)
	{
		/* stop & restart ADF to enable DMA (silly HAL) */
		ADF1_Filter0_NS->DFLTCR &= ~MDF_DFLTCR_DFLTEN;
		while(ADF1_Filter0_NS->DFLTCR & 0xC0000000);	// wait for stop
		ADF1_Filter0_NS->DFLTCR |= MDF_DFLTCR_DMAEN;
		ADF1_Filter0_NS->DFLTCR |= MDF_DFLTCR_DFLTEN;
		ADF1->GCR |= MDF_GCR_TRGO;

		return HAL_OK;
	}
	else
		return HAL_ERROR;
}

/*
 * get & reset max val
 */
int32_t PDM_GetMax(void)
{
	int32_t result = pdm_max;
	pdm_max = 0;
	return result;
}

/*
 * check if sample is ready
 */
int32_t PDM_ChkSamp(void)
{
	return pdm_cnt == PDM_SAMPSZ;
}

/*
 * restart sample collection
 */
void PDM_StartSamp(void)
{
	pdm_cnt = 0;
}

/*
 * data handler
 */
void PDM_process(int32_t *src, uint32_t sz)
{
	while(sz--)
	{
		int32_t absval = *src >> 8;
		if(pdm_cnt < PDM_SAMPSZ)
			pdm_sample[pdm_cnt++] = absval;
		
		absval = absval < 0 ? -absval : absval;
		pdm_max = pdm_max > absval ? pdm_max : absval;
		src++;
	}
}

/*
 * GPDMA1 Channel 3 interrupt for MDF/ADF/PCM
 */
void GPDMA1_Channel3_IRQHandler(void)
{
	/* Raise activity flag */
	DIAG_HIGH();
	
	start_meas();

	/* Half transfer interrupt */
	if(GPDMA1_Channel3->CSR&DMA_CSR_HTF)
	{
		/* Clear the Interrupt flag */
		GPDMA1_Channel3->CFCR = DMA_CFCR_HTF;
		
		/* handle the data */
		PDM_process(&pdm_buffer[0], PDM_BUFSZ/2);
	}
	
	/* Transfer complete interrupt */
	if(GPDMA1_Channel3->CSR&DMA_CSR_TCF)
	{
		/* Clear the Interrupt flag */
		GPDMA1_Channel3->CFCR = DMA_CFCR_TCF;
		
		/* handle the data */
		PDM_process(&pdm_buffer[PDM_BUFSZ/2], PDM_BUFSZ/2);
	}
	
	end_meas();
	
	/* Lower activity flag */
	DIAG_LOW();
}

