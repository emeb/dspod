/*
 * i2s.c - spi-i2s driver for H503 I2S Audio
 * 09-25-21 E. Brombugh
 */

#include <string.h>
#include "i2s.h"
#include "printf.h"
#include "stm32h7rsxx_ll_rcc.h"

/* choose the DMA channels */
#if 0
/* deeper FIFOs on chls 4-7 */
#define DMA_TX_CHL GPDMA1_Channel4
#define DMA_RX_CHL GPDMA1_Channel5
#define DMA_TX_CHL_IRQn GPDMA1_Channel4_IRQn
#define DMA_RX_CHL_IRQn GPDMA1_Channel5_IRQn
#define DMA_TX_CHL_IRQHandler GPDMA1_Channel4_IRQHandler
#define DMA_RX_CHL_IRQHandler GPDMA1_Channel5_IRQHandler
#else
/* shorter FIFOs on chls 0-3 */
#define DMA_TX_CHL GPDMA1_Channel0
#define DMA_RX_CHL GPDMA1_Channel1
#define DMA_TX_CHL_IRQn GPDMA1_Channel0_IRQn
#define DMA_RX_CHL_IRQn GPDMA1_Channel1_IRQn
#define DMA_TX_CHL_IRQHandler GPDMA1_Channel0_IRQHandler
#define DMA_RX_CHL_IRQHandler GPDMA1_Channel1_IRQHandler
#endif

/* uncomment to enable IRQ diag on RX pin */
//#define DIAG

#ifdef DIAG
#define DIAG_LOW()	(GPIOB->BSRR=(GPIO_PIN_7<<16))
#define DIAG_HIGH()	(GPIOB->BSRR=GPIO_PIN_7)
#else
#define DIAG_LOW()
#define DIAG_HIGH()
#endif

/* data structures used by HAL for tracking HW state */
I2S_HandleTypeDef hi2s6;

/* structures used by linked-list mode which is essential for circular DMA */
DMA_NodeTypeDef Node_GPDMA1_Channel1;
DMA_QListTypeDef List_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_NodeTypeDef Node_GPDMA1_Channel0;
DMA_QListTypeDef List_GPDMA1_Channel0;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* DMA buffers */
int16_t tx_buffer[I2S_BUFSZ] __attribute__ ((aligned (8))),
		rx_buffer[I2S_BUFSZ] __attribute__ ((aligned (8))),
		in_buffer[I2S_BUFSZ/2];

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void)
{
	printf("!!! i2s error handler !!!\n\r");
	
	while(1) {}
}

/*
 * setup I2S w/ full duplex, DMA and IRQ
 */
void i2s_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
#if 0
	/* preload buffer w/ static ramp */
	for(int16_t i=0;i<I2S_BUFSZ/2;i++)
	{
		tx_buffer[2*i] = i*(32768/(I2S_BUFSZ/2))-16384;
		tx_buffer[2*i+1] = 16383-i*(32768/(I2S_BUFSZ/2));
	}
#endif

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2S6 GPIO Configuration
    PA3     ------> I2S6_MCK
    PA4     ------> I2S6_WS
    PA5     ------> I2S6_CK
    PA6     ------> I2S6_SDI
    PA7     ------> I2S6_SDO
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#ifdef DIAG
	/* Configure diagnostic output pin on PB7 ------------------------*/
	GPIO_InitStruct.Pin =  GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
	
    /* SPI6/I2S6 Init */
	// Use this for PLL1Q based on SYSCLK rate  - gives 48.8k sample rate
	// otherwise uses PLL2P defined in main
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
    PeriphClkInit.Spi6ClockSelection = RCC_SPI6CLKSOURCE_PLL2Q;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
		Error_Handler();
    }

    __HAL_RCC_SPI6_CLK_ENABLE();

	hi2s6.Instance = SPI6;
	hi2s6.Init.Mode = I2S_MODE_MASTER_FULLDUPLEX;
	hi2s6.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s6.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s6.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s6.Init.AudioFreq = I2S_AUDIOFREQ_48K;
	hi2s6.Init.CPOL = I2S_CPOL_LOW;
	hi2s6.Init.FirstBit = I2S_FIRSTBIT_MSB;
	hi2s6.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
	hi2s6.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
	hi2s6.Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;
	if (HAL_I2S_Init(&hi2s6) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* turn on TX/RX DMA req */
    SET_BIT(hi2s6.Instance->CFG1, (SPI_CFG1_TXDMAEN|SPI_CFG1_RXDMAEN));   

    /* I2S6 DMA Init */
	__HAL_RCC_GPDMA1_CLK_ENABLE();
	
	/* CubeMX generated code uses LL mode w/ one node */
	DMA_NodeConfTypeDef NodeConfig;
	
	/* GPDMA1_REQUEST_SPI1_TX Init */
    NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    NodeConfig.Init.Request = GPDMA1_REQUEST_SPI6_TX;
    NodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    NodeConfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
    NodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
    NodeConfig.Init.DestInc = DMA_DINC_FIXED;
    NodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    NodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    NodeConfig.Init.SrcBurstLength = 1;
    NodeConfig.Init.DestBurstLength = 1;
    NodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
    NodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    NodeConfig.Init.Mode = DMA_NORMAL;
    NodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    NodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    NodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    NodeConfig.SrcAddress = (uint32_t)tx_buffer;
    NodeConfig.DstAddress = (uint32_t)&((hi2s6.Instance)->TXDR);
    NodeConfig.DataSize = I2S_BUFSZ*sizeof(int16_t);	// BYTES!!!
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
    NodeConfig.Init.Request = GPDMA1_REQUEST_SPI6_RX;
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
    NodeConfig.SrcAddress = (uint32_t)&((hi2s6.Instance)->RXDR);
    NodeConfig.DstAddress = (uint32_t)rx_buffer;
    NodeConfig.DataSize = I2S_BUFSZ*sizeof(int16_t);	// BYTES!!!
	
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
 * start the I2S port running. Assumes all GPIO, I2S & DMA stuff already set up
 */
void i2s_start(void)
{
	/* turn on the I2S */
	__HAL_I2S_ENABLE(&hi2s6);
	
	/* start transmission (new bit needed for H5) */
	SET_BIT(hi2s6.Instance->CR1, SPI_CR1_CSTART);
}

/*
 * compute I2S sample rate
 */
int32_t i2s_get_fsamp(void)
{
	int32_t i2s_div = (hi2s6.Instance->I2SCFGR&SPI_I2SCFGR_I2SDIV)>>SPI_I2SCFGR_I2SDIV_Pos;
	i2s_div <<= 1;
	if(hi2s6.Instance->I2SCFGR&SPI_I2SCFGR_ODD)
		i2s_div++;
	
	int32_t in_clk;
	LL_PLL_ClocksTypeDef PLL_Clocks;
	
	LL_RCC_GetPLL2ClockFreq(&PLL_Clocks);
	in_clk = PLL_Clocks.PLL_Q_Frequency;

		
	return in_clk / i2s_div / 256;
}

/**
  * GPDMA1 Channel0 is I2S TX TC/HT
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
		memcpy(in_buffer, &rx_buffer[I2S_BUFSZ/2], I2S_BUFSZ/2*sizeof(int16_t));

		/* load the first half of the buffers */
		Audio_Proc(&tx_buffer[0], in_buffer, I2S_BUFSZ/2);
	}
	
	/* Transfer complete interrupt */
	if(DMA_TX_CHL->CSR&DMA_CSR_TCF)
	{
		/* Clear the Interrupt flag */
		DMA_TX_CHL->CFCR = DMA_CFCR_TCF;
		
		/* grab rx from previous */
		memcpy(in_buffer, &rx_buffer[0], I2S_BUFSZ/2*sizeof(int16_t));

		/* load the 2nd half of the buffer */
		Audio_Proc(&tx_buffer[I2S_BUFSZ/2], in_buffer, I2S_BUFSZ/2);
	}
    	
	/* Lower activity flag */
	DIAG_LOW();
}
