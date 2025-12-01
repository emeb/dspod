/*
 * xspi.c - external xspi PSRAM setup for H730 w/ OctoSPI port
 * 12-08-2020 E. Brombaugh
 */

#include "xspi.h"
#include <string.h>
#include "printf.h"

/* uncomment this to run the memory in QUAD mode */
#define QUAD_MODE

XSPI_HandleTypeDef hxspi1;
XSPI_RegularCmdTypeDef sCommand;
XSPI_MemoryMappedTypeDef sMemMappedCfg;

/* Error handler - lock up here */
static void Error_Handler(void)
{
	printf("!!!WARNING - In XSPI Error Handler!!!\n\r");
	while(1)
	{
	}
}

/* dump XSPI1 regs for diags */
void dump_regs(void)
{
#if 0
	uint32_t *regptr = (uint32_t *)XSPI1;
	for(int i=0;i<0x228;i+=16)
	{
		printf("%08X: ", regptr);
		for(int j=0;j<16;j+=4)
		{
			printf("%08X ", *regptr++);
		}
		printf("\n\r");
	}
#else
	printf("XSPI1->CCR : 0x%08X\n\r", XSPI1->CCR);
	printf("XSPI1->TCR : 0x%08X\n\r", XSPI1->TCR);
	printf("XSPI1->IR  : 0x%08X\n\r", XSPI1->IR);
	printf("XSPI1->ABR : 0x%08X\n\r", XSPI1->ABR);
	printf("--------\n\r");
	printf("XSPI1->WCCR : 0x%08X\n\r", XSPI1->WCCR);
	printf("XSPI1->WTCR : 0x%08X\n\r", XSPI1->WTCR);
	printf("XSPI1->WIR  : 0x%08X\n\r", XSPI1->WIR);
	printf("XSPI1->WABR : 0x%08X\n\r", XSPI1->WABR);
	printf("===========================\n\r");
#endif
}

/*
 * initialize xspi for memory mapped access
 */
void xspi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	XSPIM_CfgTypeDef sXspiManagerCfg = {0};

	/* Enable the XSPIM_P1 interface */
	HAL_PWREx_EnableXSPIM1();

	/* Initializes the peripherals clock */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_XSPI1;
	PeriphClkInit.Xspi1ClockSelection = RCC_XSPI1CLKSOURCE_HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	/* Peripheral clock enable */
	__HAL_RCC_XSPIM_CLK_ENABLE();
	__HAL_RCC_XSPI1_CLK_ENABLE();

	__HAL_RCC_GPIOP_CLK_ENABLE();
	__HAL_RCC_GPIOO_CLK_ENABLE();
	/**XSPI1 GPIO Configuration
	PP2     ------> XSPIM_P1_IO2
	PP3     ------> XSPIM_P1_IO3
	PO4     ------> XSPIM_P1_CLK
	PP0     ------> XSPIM_P1_IO0
	PP1     ------> XSPIM_P1_IO1
	PO0     ------> XSPIM_P1_NCS1
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P1;
	HAL_GPIO_Init(GPIOP, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P1;
	HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

	/* XSPI1 parameter configuration*/
	hxspi1.Instance = XSPI1;
	hxspi1.Init.FifoThresholdByte = 1;
	hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
	hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM;
	hxspi1.Init.MemorySize = HAL_XSPI_SIZE_64MB;
	hxspi1.Init.ChipSelectHighTimeCycle = 2;	// 27ns @ 75MHz (min is 18ns)
	hxspi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
	hxspi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
	hxspi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
	hxspi1.Init.ClockPrescaler = 2;	// 75MHz (max is 85MHz for linear bursts)
	hxspi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
	hxspi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
	hxspi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
	hxspi1.Init.MaxTran = 225;	// 3us @ 75MHz (max is 3us)
	hxspi1.Init.Refresh = 0;
	hxspi1.Init.MemorySelect = HAL_XSPI_CSSEL_NCS1;
	if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
	{
		Error_Handler();
	}
	sXspiManagerCfg.nCSOverride = HAL_XSPI_CSSEL_OVR_NCS1;
	sXspiManagerCfg.IOPort = HAL_XSPIM_IOPORT_1;
	if (HAL_XSPIM_Config(&hxspi1, &sXspiManagerCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* reset qspi memory */
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_4_LINES;
	sCommand.Instruction        = 0xf5;	// exit quad mode
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode           = HAL_XSPI_DATA_NONE;
	sCommand.DummyCycles        = 0;
	sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	sCommand.InstructionMode   = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = 0x66;	// reset enable
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	sCommand.Instruction       = 0x99;	// reset
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

#ifdef QUAD_MODE
	/* Put it in qspi mode */
	printf("xspi_init: enabling x4 QSPI mode.\n\r");
	sCommand.Instruction       = 0x35;	// enter quad mode
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
			Error_Handler();
	}

	sCommand.InstructionMode	= HAL_XSPI_INSTRUCTION_4_LINES;	// xspi mode uses 4 line instr
	sCommand.AddressMode		= HAL_XSPI_ADDRESS_4_LINES;
	sCommand.DataMode			= HAL_XSPI_DATA_4_LINES;
#else
	printf("xspi_init: using x1 SPI mode.\n\r");
	sCommand.AddressMode		= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.DataMode			= HAL_XSPI_DATA_1_LINE;
#endif

	/* prep for subsequent operations */
	sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;
}

/* original pure HAL with error checking */
/*
 * writing to xspi must be handled in indirect mode
 */
void xspi_writebytes(uint32_t addr, uint8_t *data, uint32_t sz)
{
	HAL_StatusTypeDef stat;

	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Address			= addr;
	sCommand.Instruction 		= 0x38;		// SPI QUAD WRITE
	sCommand.DummyCycles		= 0;
	sCommand.DataLength			= sz;

	if ((stat = HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)) != HAL_OK)
	{
		printf("xspi_writebytes: HAL_XSPI_Command status = 0x%08X\n\r", stat);
		Error_Handler();
	}

	if ((stat = HAL_XSPI_Transmit(&hxspi1, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)) != HAL_OK)
	{
		printf("xspi_writebytes: HAL_XSPI_Transmit status = 0x%08X\n\r", stat);
		Error_Handler();
	}
}

/*
 * reading xspi in batch mode
 */
void xspi_readbytes(uint32_t addr, uint8_t *data, uint32_t sz)
{
	HAL_StatusTypeDef stat;

	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Address			= addr;
	sCommand.Instruction 		= 0xEB;		// SPI QUAD READ
	sCommand.DummyCycles		= 6;
	sCommand.DataLength			= sz;

	if ((stat = HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)) != HAL_OK)
	{
		printf("xspi_readbytes: HAL_XSPI_Command status = 0x%08X\n\r", stat);
		Error_Handler();
	}

	if ((stat = HAL_XSPI_Receive(&hxspi1, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)) != HAL_OK)
	{
		printf("xspi_readbytes: HAL_XSPI_Receive status = 0x%08X\n\r", stat);
		Error_Handler();
	}
}

/*
 * put xspi into memory mapped read mode
 */
void xspi_memmap(uint8_t enable)
{
	if((hxspi1.State != HAL_XSPI_STATE_BUSY_MEM_MAPPED) && enable)
	{
		/* set write & read ops & turn on mem mapped mode */
		sCommand.OperationType		= HAL_XSPI_OPTYPE_WRITE_CFG;
		sCommand.Instruction 		= 0x38;		// SPI QUAD WRITE
		sCommand.DummyCycles		= 0;
		sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE; // ERRATUM 2.4.1
		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			Error_Handler();
		}
		
		sCommand.OperationType		= HAL_XSPI_OPTYPE_READ_CFG;
		sCommand.Instruction 		= 0xEB;		// SPI QUAD READ
		sCommand.DummyCycles		= 6;
		sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;	// return to correct value
		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			Error_Handler();
		}

		sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_ENABLE;
		sMemMappedCfg.TimeoutPeriodClock      = 0x34;
		if (HAL_XSPI_MemoryMapped(&hxspi1, &sMemMappedCfg) != HAL_OK)
		{
			Error_Handler();
		}
		
		/* wait a bit for things to settle */
		HAL_Delay(1);
	}
	else if((hxspi1.State == HAL_XSPI_STATE_BUSY_MEM_MAPPED) && !enable)
	{
		/* turn off mem mapped mode */
		uint32_t Tickstart = HAL_GetTick();

		/* Update xspi state */
		hxspi1.State = HAL_XSPI_STATE_ABORT;

		/* Configure xspi: CR register with Abort request */
		SET_BIT(hxspi1.Instance->CR, XSPI_CR_ABORT);

		/* wait for not busy */
		while(READ_BIT(hxspi1.Instance->SR, XSPI_SR_BUSY))
		{
			if((HAL_GetTick() - Tickstart) > HAL_XSPI_TIMEOUT_DEFAULT_VALUE)
			{
				Error_Handler();
			}
		}

		/* disable mem map mode */
		MODIFY_REG(hxspi1.Instance->CR, XSPI_CR_FMODE, 0);

		/* Update xspi state */
		hxspi1.State = HAL_XSPI_STATE_READY;
	}
}

/*
 * tests for xspi
 */

//#define xspi_TEST_LEN xspi_SIZE/4
#define xspi_TEST_LEN (1<<11)

/* PRN generator info */
#define NOISE_POLY_TAP0 31
#define NOISE_POLY_TAP1 21
#define NOISE_POLY_TAP2 1
#define NOISE_POLY_TAP3 0

uint32_t prn_lfsr;
uint8_t txbuffer[xspi_TEST_LEN], rxbuffer[xspi_TEST_LEN];
uint32_t wordbuf[xspi_TEST_LEN];

/**
  * @brief  PRN into destination
  * @param  seed - seed for noise generator
  * @retval none
  */
void prn_seed(uint32_t seed)
{
	prn_lfsr = seed;
}

/**
  * @brief  32-bit PRN
  * @retval 32-bit unsigned result
  */
uint32_t prn_gen(int8_t nbits)
{
	uint8_t bit;
	uint8_t new_data;
	uint32_t mask = (1<<(nbits+1))-1;

	/* generate new bits */
	for(bit=0;bit<nbits;bit++)
	{
		new_data = ((prn_lfsr>>NOISE_POLY_TAP0) ^
					(prn_lfsr>>NOISE_POLY_TAP1) ^
					(prn_lfsr>>NOISE_POLY_TAP2) ^
					(prn_lfsr>>NOISE_POLY_TAP3));
		prn_lfsr = (prn_lfsr<<1) | (new_data&1);
	}

	return prn_lfsr & mask;
}

/*
 * test various xspi access methods
 */
uint8_t xspi_test(uint8_t testflags)
{
	uint32_t i, err, *memword = (uint32_t *)XSPI1_BASE;
	uint8_t retval = 0, *membyte = (uint8_t *)XSPI1_BASE;
	
	/* Indirect mode, 8-bit count sequence */
	if(testflags & 0x01)
	{
		printf("Indirect mode, counting\n\r");
		printf("Writing %d\n\r", xspi_TEST_LEN);
		for(i=0;i<xspi_TEST_LEN;i++)
			txbuffer[i] = i;

		xspi_writebytes(0, txbuffer, xspi_TEST_LEN);

		printf("Reading %d\n\r", xspi_TEST_LEN);
		xspi_readbytes(0, rxbuffer, xspi_TEST_LEN);

		err = 0;
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			if(txbuffer[i] != rxbuffer[i])
			{
				err++;
				if(testflags & 0x80)
					printf("%2d : %02X -> %02X\n\r", i, txbuffer[i], rxbuffer[i]);
			}
		}
		if(err==0)
			printf("Passed.\n\r");
		else
		{
			retval++;
			printf("Failed %d.\n\r", err);
		}
	}
	
	/* Memmap mode, 8-bit count sequence */
	if(testflags & 0x02)
	{
		printf("Memmap mode, counting\n\r");
		xspi_memmap(1);
		
#if 1
		printf("Writing %d\n\r", xspi_TEST_LEN);
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			txbuffer[i] = i+0x10;
			//printf("%1d : %02X\n\r", i, txbuffer[i]);
			//membyte[i] = txbuffer[i];
		}
		memcpy(membyte, txbuffer, sizeof(txbuffer));
#endif
		printf("Reading %d\n\r", xspi_TEST_LEN);
		err = 0;
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			if(txbuffer[i] != membyte[i])
			{
				err++;
				if(testflags & 0x80)
					printf("%2d : %02X -> %02X\n\r", i, txbuffer[i], membyte[i]);
			}
		}
		
		if(err==0)
			printf("Passed.\n\r");
		else
		{
			retval++;
			printf("Failed %d.\n\r", err);
		}
		
		xspi_memmap(0);
	}
	
	/* Indirect mode, 8-bit PRN sequence */
	if(testflags & 0x04)
	{
		printf("Indirect mode, PRN\n\r");
		printf("Writing %d\n\r", xspi_TEST_LEN);
		prn_seed(0x12345678);
		for(i=0;i<xspi_TEST_LEN;i++)
			txbuffer[i] = prn_gen(8);
		xspi_writebytes(0, txbuffer, xspi_TEST_LEN);

		printf("Reading %d\n\r", xspi_TEST_LEN);
		xspi_readbytes(0, rxbuffer, xspi_TEST_LEN);

		err = 0;
		prn_seed(0x12345678);
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			if(txbuffer[i] != rxbuffer[i])
			{
				if(testflags & 0x80)
					printf("%1d : %02X -> %02X\n\r", i, txbuffer[i], rxbuffer[i]);
				err++;
			}
		}
		
		if(err==0)
			printf("Passed.\n\r");
		else
		{
			retval++;
			printf("Failed %d.\n\r", err);
		}
	}
	
	/* Memmap mode, 8-bit PRN sequence */
	if(testflags & 0x08)
	{
		printf("Memmap mode, PRN\n\r");
		xspi_memmap(1);
		
#if 0
		printf("32 bits\n\r");
#if 1
		printf("Writing %d\n\r", xspi_TEST_LEN);
		prn_seed(0xdeadbeef);
		for(i=0;i<xspi_TEST_LEN/4;i++)
		{
			wordbuf[i] = prn_gen(32);
		}
		memcpy(memword, wordbuf, sizeof(wordbuf));
#endif
		
		printf("Reading %d\n\r", xspi_TEST_LEN);
		err = 0;
		//SCB_InvalidateDCache_by_Addr((uint32_t *)membyte, xspi_TEST_LEN);
		prn_seed(0xdeadbeef);
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			uint32_t tstdat = prn_gen(32);
			if(tstdat != memword[i])
			{
				err++;
				if(testflags & 0x80)
					printf("%1d : %08X -> %08X\n\r", i, tstdat, memword[i]);
			}
		}
#else
		printf("8 bits\n\r");
#if 1
		printf("Writing %d\n\r", xspi_TEST_LEN);
		prn_seed(0xabadcafe);
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			txbuffer[i] = prn_gen(8);
			//printf("%1d : %02X\n\r", i, tstdat);
			//membyte[i] = txbuffer[i];
		}
		memcpy(membyte, txbuffer, sizeof(txbuffer));
#endif
		
		printf("Reading %d\n\r", xspi_TEST_LEN);
		err = 0;
		//SCB_InvalidateDCache_by_Addr((uint32_t *)membyte, xspi_TEST_LEN);
		prn_seed(0xabadcafe);
		for(i=0;i<xspi_TEST_LEN;i++)
		{
			if(txbuffer[i] != membyte[i])
			{
				err++;
				if(testflags & 0x80)
					printf("%1d : %02X -> %02X\n\r", i, txbuffer[i], membyte[i]);
			}
		}
#endif

		if(err==0)
			printf("Passed.\n\r");
		else
		{
			retval++;
			printf("Failed %d.\n\r", err);
		}
		
		xspi_memmap(0);
	}
	
	return 0;
}
