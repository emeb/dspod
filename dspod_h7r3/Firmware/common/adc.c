/*
 * adc.c - h7r3 adc setup
 * E. Brombaugh 11-03-2025
 */

#include <string.h>
#include "adc.h"
#include "printf.h"

/* uncomment to enable IRQ diag on RX pin */
//#define DIAG

#ifdef DIAG
/* diag on dspod gate-out */
#define DIAG_LOW()	(GPIOA->BSRR=(GPIO_PIN_0<<16))
#define DIAG_HIGH()	(GPIOA->BSRR=GPIO_PIN_0)
#else
#define DIAG_LOW()
#define DIAG_HIGH()
#endif

ADC_HandleTypeDef hadc1 ;//__attribute__ ((section (".ram_data")));
DMA_NodeTypeDef Node_GPDMA1_Channel2 __attribute__ ((section (".sramahb_data"))) ;//__attribute__ ((aligned (32)));
DMA_QListTypeDef List_GPDMA1_Channel2 ;//__attribute__ ((section (".sramahb_data")));
DMA_HandleTypeDef handle_GPDMA1_Channel2 ;//__attribute__ ((section (".sramahb_data")));

int16_t adc_buffer[ADC_BUFSZ] __attribute__ ((section (".sramahb_data"))) ;//__attribute__ ((aligned (32)));
volatile int16_t ADC_val[ADC_BUFSZ];

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void)
{
	printf("!!! adc error handler !!!\n\r");
	
	while(1) {}
}

/*
 * Initialize the breakout board LED
 */
void ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};
	DMA_NodeConfTypeDef NodeConfig;
	
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	/* enable the AHB SRAMs for DMA buffers */
	__HAL_RCC_SRAM1_CLK_ENABLE();
	__HAL_RCC_SRAM2_CLK_ENABLE();

    /* Peripheral clock enable */
    __HAL_RCC_ADC12_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA1     ------> ADC1_INP1
    PA2     ------> ADC1_INP14
    PB0     ------> ADC1_INP9
    PB1     ------> ADC1_INP5
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
#ifdef DIAG
	/* Configure diagnostic output pin on PA0 (gate-out) --------------------*/
	GPIO_InitStruct.Pin =  GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
	
	/*
	* Common config
	* clocking, oversampling and sample time combine for ~4kHz ISR rate
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = ADC_BUFSZ;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = ENABLE;
	hadc1.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_16;
	hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
	hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
	
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	*/
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSign = ADC_OFFSET_SIGN_NEGATIVE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = ADC_REGULAR_RANK_4;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* turn on DMA req */
	SET_BIT(hadc1.Instance->CFGR, ADC_CFGR_DMAEN);

	/* ADC1 DMA Init */
	__HAL_RCC_GPDMA1_CLK_ENABLE();

	/* GPDMA1_REQUEST_ADC1 Init */
	NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
	NodeConfig.Init.Request = GPDMA1_REQUEST_ADC1;
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
    NodeConfig.SrcAddress = (uint32_t)&((hadc1.Instance)->DR);
    NodeConfig.DstAddress = (uint32_t)adc_buffer;
    NodeConfig.DataSize = ADC_BUFSZ*sizeof(int16_t);	// BYTES!!!
	if (HAL_DMAEx_List_BuildNode(&NodeConfig, &Node_GPDMA1_Channel2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DMAEx_List_InsertNode(&List_GPDMA1_Channel2, NULL, &Node_GPDMA1_Channel2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DMAEx_List_SetCircularMode(&List_GPDMA1_Channel2) != HAL_OK)
	{
		Error_Handler();
	}

	/* suggested by docs - needed? */
    if (HAL_DMAEx_List_ConvertQToDynamic(&List_GPDMA1_Channel2) != HAL_OK)
    {
      Error_Handler();
    }
	
#ifdef USE_CACHE
#ifndef USE_MPU
	/* flush linked list? */
	uint32_t *clean_addr = (uint32_t *)((uint32_t)&Node_GPDMA1_Channel2 & ~0x1f);
	SCB_CleanDCache_by_Addr(clean_addr, sizeof(DMA_NodeTypeDef)+0x20);
#endif
#endif
	
	handle_GPDMA1_Channel2.Instance = GPDMA1_Channel2;
	handle_GPDMA1_Channel2.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
	handle_GPDMA1_Channel2.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
	handle_GPDMA1_Channel2.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
	handle_GPDMA1_Channel2.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
	handle_GPDMA1_Channel2.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
	if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel2, &List_GPDMA1_Channel2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel2, DMA_CHANNEL_NPRIV) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* Point DMA chl at the linked list */
	uint32_t cllr_mask = DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;
	handle_GPDMA1_Channel2.Instance->CLBAR = ((uint32_t)handle_GPDMA1_Channel2.LinkedListQueue->Head & DMA_CLBAR_LBA);
	handle_GPDMA1_Channel2.Instance->CLLR  = ((uint32_t)handle_GPDMA1_Channel2.LinkedListQueue->Head & DMA_CLLR_LA) | cllr_mask;

	/* enable TC IRQ on chl 2 */
	__HAL_DMA_ENABLE_IT(&handle_GPDMA1_Channel2, DMA_IT_TC);
	
	/* enable IRQ for GPDMA1 Chl2 */
	HAL_NVIC_SetPriority(GPDMA1_Channel2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel2_IRQn);

	/* enable channel */
    __HAL_DMA_ENABLE(&handle_GPDMA1_Channel2);
	
	/* Start ADC */
	HAL_ADC_Start(&hadc1);
}

/*
 *
 */
int16_t ADC_GetChl(uint8_t chl)
{
	return ADC_val[chl];
}

/**
  * GPDMA1 Channel2 IRQ is ADC TC
  */
void GPDMA1_Channel2_IRQHandler(void)
{
	/* Raise activity flag */
	DIAG_HIGH();
	
	/* Transfer complete interrupt */
	if(GPDMA1_Channel2->CSR&DMA_CSR_TCF)
	{
		/* Clear the Interrupt flag */
		GPDMA1_Channel2->CFCR = DMA_CFCR_TCF;
		
#ifdef USE_CACHE
#ifndef USE_MPU
		/* invalidate Dcache of ADC buffer */
		uint32_t *inval_addr = (uint32_t *)((uint32_t)adc_buffer & ~0x1f);
		SCB_InvalidateDCache_by_Addr(inval_addr, ADC_BUFSZ+32);
#endif
#endif
		
		/* grab rx from previous, invert, etc */
		//memcpy((int16_t *)ADC_val, adc_buffer, ADC_BUFSZ*sizeof(int16_t));
		for(int i=0;i<ADC_BUFSZ;i++)
		{
			/* invert & scale to full 12-bit range */
			int32_t temp = (4095-adc_buffer[i]) * 4172;
			
			/* normalize & saturate */
			ADC_val[i] = __USAT((temp >> 12), 12);
		}
	}
    	
	/* Lower activity flag */
	DIAG_LOW();
}
