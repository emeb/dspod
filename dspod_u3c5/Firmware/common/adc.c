/*
 * adc.c - dspod_u3c5 4 channel adc setup
 * 05-05-2026 E. Brombaugh
 */

#include <string.h>
#include "adc.h"
#include "printf.h"

/* uncomment to enable IRQ diag on RX pin */
#define DIAG

#ifdef DIAG
#define DIAG_LOW()	(GPIOC->BSRR=(GPIO_PIN_8<<16))
#define DIAG_HIGH()	(GPIOC->BSRR=GPIO_PIN_8)
#else
#define DIAG_LOW()
#define DIAG_HIGH()
#endif

ADC_HandleTypeDef hadc1;
DMA_NodeTypeDef Node_GPDMA1_Channel2;
DMA_QListTypeDef List_GPDMA1_Channel2;
DMA_HandleTypeDef handle_GPDMA1_Channel2;

int16_t adc_buffer[ADC_BUFSZ];
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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	DMA_NodeConfTypeDef NodeConfig;
	ADC_ChannelConfTypeDef sConfig = {0};
	
	/* turn on analog power */
	HAL_PWREx_EnableVddA();
	
	/* Hook up ADC clock */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
    PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_MSIK;
    PeriphClkInit.AdcDacClockDivider = RCC_ADCDACCLK_DIV2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

	/* Peripheral clock enable */
	__HAL_RCC_ADC12_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA1     ------> ADC1_IN4
    PA2     ------> ADC1_IN5
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN1
    PC1     ------> ADC1_IN2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
#ifdef DIAG
	/* Configure diagnostic output pin on PC8 ------------------------*/
	GPIO_InitStruct.Pin =  GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
	
	/*
	* Common config
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = ADC_BUFSZ;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_46CYCLES_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_4;
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

	/* Perform ADC calibration */
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		/* Calibration Error */
		Error_Handler();
	}

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
		
		/* grab rx from previous */
		//memcpy(adc_val, adc_buffer, ADC_BUFSZ*sizeof(int16_t));
		for(int i=0;i<ADC_BUFSZ;i++)
		{
			/* invert & scale to full 12-bit range */
			int32_t temp = (4095-adc_buffer[i]) * 4099;
			
			/* normalize & saturate */
			ADC_val[i] = __USAT((temp >> 12), 12);
		}
	}
    	
	/* Lower activity flag */
	DIAG_LOW();
}
