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
 * initialize the AP6404L PSRAM for Quad mode indirect operation
 */
void xspi_psram_init(void)
{
	/* XSPI1 parameter configuration*/
	hxspi1.Instance = XSPI1;
	hxspi1.Init.FifoThresholdByte = 1;
	hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
	hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM;
	hxspi1.Init.MemorySize = HAL_XSPI_SIZE_64MB;	// bits!
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
	hxspi1.Init.MemorySelect = XSPI_PSRAM_NCS;
	if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
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
	printf("xspi_psram_init: enabling x4 QSPI mode.\n\r");
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

/*
 * initialize the AP6404L PSRAM for Quad mode indirect operation
 */
void xspi_flash_init(void)
{
	/* XSPI1 parameter configuration*/
	hxspi1.Instance = XSPI1;
	hxspi1.Init.FifoThresholdByte = 1;
	hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
	hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM;
	hxspi1.Init.MemorySize = HAL_XSPI_SIZE_32MB;	// bits!
	hxspi1.Init.ChipSelectHighTimeCycle = 2;	// 27ns @ 75MHz (min is 18ns)
	hxspi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
	hxspi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
	hxspi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
	hxspi1.Init.ClockPrescaler = 4;	// 75MHz (max is 80MHz for single mode)
	hxspi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
	hxspi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
	hxspi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
	hxspi1.Init.MaxTran = 225;	// 3us @ 75MHz (max is 3us)
	hxspi1.Init.Refresh = 0;
	hxspi1.Init.MemorySelect = XSPI_FLASH_NCS;
	if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
	{
		Error_Handler();
	}
	
	/* default cmd conditions */
	sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_1_LINE; // on this device all instr are single
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;
	
	/* send reset enable */
	xspi_flash_1bcmd(0x66);

	/* send reset */
	xspi_flash_1bcmd(0x99);
	
	/* check status */
	uint8_t data[3];
	data[0] = xspi_flash_read_status(0);
	data[1] = xspi_flash_read_status(1);
	data[2] = xspi_flash_read_status(2);
	printf("xspi_flash_init: status [1 2 3]: 0x%02X 0x%02X 0x%02X\n\r", data[0], data[1], data[2]);

	/* enable quad mode */
	if((data[1] & 0x02) != 0x02)
	{
		printf("QE not set - setting...\n\r");
		xspi_flash_write_status(1, data[1] | 0x02);	// set QE
		HAL_Delay(1);
		data[1] = xspi_flash_read_status(1);
		printf("xspi_flash_init: status [1 2 3]: 0x%02X 0x%02X 0x%02X\n\r", data[0], data[1], data[2]);
		if((data[1] & 0x02) != 0x02)
			printf("set QE failed!\n\r");
		else
			printf("set QE successful\n\r");
	}
	else
		printf("QE is set\n\r");

}	

/*
 * initialize xspi port
 */
void xspi_init(uint8_t type)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
	PO1     ------> XSPIM_P1_NCS2
	PO0     ------> XSPIM_P1_NCS1
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P1;
	HAL_GPIO_Init(GPIOP, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_1|GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P1;
	HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);
	
	if(type == XSPI_PSRAM_TYPE)
		xspi_psram_init();
	else if(type == XSPI_FLASH_TYPE)
		xspi_flash_init();
	else
		printf("xspi_init: unknown type (PSRAM/Flash)\n\r");		
}

/* original pure HAL with error checking */
/*
 * writing to xspi must be handled in indirect mode
 */
void xspi_psram_writebytes(uint32_t addr, uint8_t *data, uint32_t sz)
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
void xspi_psram_readbytes(uint32_t addr, uint8_t *data, uint32_t sz)
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
 * put xspi into memory mapped read mode for psram
 */
void xspi_psram_memmap(uint8_t enable)
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
 * send 1-byte cmd
 */
void xspi_flash_1bcmd(uint8_t cmd)
{
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Instruction        = cmd;
	sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
	sCommand.DataMode           = HAL_XSPI_DATA_NONE;
	sCommand.DummyCycles        = 0;
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_1bcmd: Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}
}

/*
 * Read a status byte from flash
 */
uint8_t xspi_flash_read_status(uint8_t byte_num)
{
	uint8_t result;
	
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	switch(byte_num)
	{
		default:
		case 0:
			sCommand.Instruction        = 0x05;	// reg 1 s[7:0]
			break;
		case 1:
			sCommand.Instruction        = 0x35;	// reg 2 s[15:8]
			break;
		case 2:
			sCommand.Instruction        = 0x15;	// reg 3 s[23:16]
			break;
	}
	sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
	sCommand.DataMode			= HAL_XSPI_DATA_1_LINE;
	sCommand.DummyCycles		= 0;
	sCommand.DataLength			= 1;

	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_XSPI_Receive(&hxspi1, &result, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_read_status: Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}
	
	return result;
}

/*
 * Read a status byte from flash
 */
void xspi_flash_write_status(uint8_t byte_num, uint8_t data)
{
	/* Set write enable */
	xspi_flash_1bcmd(0x06);
	
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	switch(byte_num)
	{
		default:
		case 0:
			sCommand.Instruction        = 0x01;	// reg 1 s[7:0]
			break;
		case 1:
			sCommand.Instruction        = 0x31;	// reg 2 s[15:8]
			break;
		case 2:
			sCommand.Instruction        = 0x11;	// reg 3 s[23:16]
			break;
	}
	sCommand.AddressMode        = HAL_XSPI_ADDRESS_NONE;
	sCommand.DataMode			= HAL_XSPI_DATA_1_LINE;
	sCommand.DummyCycles		= 0;
	sCommand.DataLength			= 1;

	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_XSPI_Transmit(&hxspi1, &data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_write_status: Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}
}

/*
 * erase sectors or blocks in flash - sz in bytes results in proper num 
 */
void xspi_flash_erase(uint8_t type, uint32_t addr, uint32_t sz)
{
	uint8_t cmd;
	uint32_t esz, to, timeout;
	
	switch(type)
	{
		default:
		case 0:	// sector
			cmd = 0x20;		// command byte
			esz = 4096;		// erase size
			to = 500;		// timeout in ms
			break;
		
		case 1: // 32k block
			cmd = 0x52;
			esz = 32768;
			to = 2000;
			break;
		
		case 2: // 64k block
			cmd = 0x68;
			esz = 65536;
			to = 3000;
			break;
	}
	
	/* get num sectors to erase as ceil(sz/esz) */
	sz  = (sz + (esz-1)) / esz;
	
	if(sz == 0)
		return;
	
	while(sz--)
	{
		/* Set write enable */
		xspi_flash_1bcmd(0x06);
		
		/* send sector erase cmd */
		sCommand.AddressMode		= HAL_XSPI_ADDRESS_1_LINE;
		sCommand.DataMode			= HAL_XSPI_DATA_NONE;
		sCommand.Address			= addr;
		sCommand.Instruction        = cmd;
		sCommand.DummyCycles		= 0;
		sCommand.DataLength			= 0;
		sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;
		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			printf("xspi_flash_erase_sect: Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
			Error_Handler();
		}
		
		/* wait for finish or timeout */
		timeout = to;
		while((xspi_flash_read_status(0) & 1) && (timeout != 0))
		{
			timeout--;
			HAL_Delay(1);
		}
		
		/* next address */
		addr += esz;
	}
}

/*
 * write a buffer of data to flash. max 256 bytes. Note that data
 * beyond end of starting page wraps back to beginning of start page!
 */
uint8_t xspi_flash_writepg(uint32_t addr, uint8_t *data, uint32_t sz)
{
	if(sz>256)
		return 1;
	
	/* Set write enable */
	xspi_flash_1bcmd(0x06);
	
	/* quad page write - uses single mode for addr */ 
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.AddressMode		= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.Address			= addr;
	sCommand.Instruction 		= 0x32;	// SPI QUAD Page Prog
	sCommand.DummyCycles		= 0;
	sCommand.DataLength			= sz;
	sCommand.DataMode			= HAL_XSPI_DATA_4_LINES;

	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_writepg: Command Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}

	if (HAL_XSPI_Transmit(&hxspi1, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_writepg: Transmit Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}
	
	/* wait for finish or timeout */
	uint32_t timeout = 1000;
	while((xspi_flash_read_status(0) & 1) && (timeout != 0))
	{
		timeout--;
		HAL_Delay(1);
	}
	
	printf("xspi_flash_writepg: stat = 0x%02X, timeout = %d\n\r", xspi_flash_read_status(0), timeout);
	
	/* return error if timeout reached zero */
	return (timeout == 0);
}

/*
 * read a buffer of data from flash
 */
void xspi_flash_readbytes(uint32_t addr, uint8_t *data, uint32_t sz)
{
	sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.AddressMode		= HAL_XSPI_ADDRESS_4_LINES;
	sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.Address			= addr;
	sCommand.Instruction 		= 0xEB;	// SPI QUAD READ
	sCommand.DummyCycles		= 6;	// 2 mode + 4 dummy
	sCommand.DataLength			= sz;
	sCommand.DataMode			= HAL_XSPI_DATA_4_LINES;

	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_readbytes: Command Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}

	if (HAL_XSPI_Receive(&hxspi1, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("xspi_flash_readbytes: Receive Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
		Error_Handler();
	}
}

/*
 * put xspi into memory mapped read mode for flash
 */
void xspi_flash_memmap(uint8_t enable)
{
	if((hxspi1.State != HAL_XSPI_STATE_BUSY_MEM_MAPPED) && enable)
	{
		/* set write & read ops & turn on mem mapped mode */
		/* write mode should not be used on flash but it needs to be set up anyway */
		sCommand.OperationType		= HAL_XSPI_OPTYPE_WRITE_CFG;
		sCommand.AddressMode		= HAL_XSPI_ADDRESS_4_LINES;
		sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;
		sCommand.Instruction 		= 0x32;		// SPI QUAD WRITE
		sCommand.DummyCycles		= 0;
		sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE; // ERRATUM 2.4.1
		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			Error_Handler();
		}
		
		/* set read ops & turn on mem mapped mode */
		sCommand.OperationType		= HAL_XSPI_OPTYPE_READ_CFG;
		sCommand.Instruction 		= 0xEB;		// SPI QUAD READ
		sCommand.DummyCycles		= 6;
		sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;	// return to correct value
		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			printf("xspi_flash_memmap: Command Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
			Error_Handler();
		}

		sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_ENABLE;
		sMemMappedCfg.TimeoutPeriodClock      = 0x34;
		if (HAL_XSPI_MemoryMapped(&hxspi1, &sMemMappedCfg) != HAL_OK)
		{
			printf("xspi_flash_memmap: MemoryMapped Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
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
				printf("xspi_flash_memmap: timeout for not busy\n\r");
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
uint8_t xspi_psram_test(uint8_t testflags)
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

		xspi_psram_writebytes(0, txbuffer, xspi_TEST_LEN);

		printf("Reading %d\n\r", xspi_TEST_LEN);
		xspi_psram_readbytes(0, rxbuffer, xspi_TEST_LEN);

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
		xspi_psram_memmap(1);
		
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
		
		xspi_psram_memmap(0);
	}
	
	/* Indirect mode, 8-bit PRN sequence */
	if(testflags & 0x04)
	{
		printf("Indirect mode, PRN\n\r");
		printf("Writing %d\n\r", xspi_TEST_LEN);
		prn_seed(0x12345678);
		for(i=0;i<xspi_TEST_LEN;i++)
			txbuffer[i] = prn_gen(8);
		xspi_psram_writebytes(0, txbuffer, xspi_TEST_LEN);

		printf("Reading %d\n\r", xspi_TEST_LEN);
		xspi_psram_readbytes(0, rxbuffer, xspi_TEST_LEN);

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
		xspi_psram_memmap(1);
		
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
		
		xspi_psram_memmap(0);
	}
	
	return 0;
}

/*
 * test various xspi access methods
 */
uint8_t xspi_flash_test(uint8_t testflags)
{
	uint8_t result = 0;
	uint8_t data[256];
	
	if(testflags & 0x01)
	{
		/* get mfg/dev id */
		sCommand.AddressMode		= HAL_XSPI_ADDRESS_1_LINE;
		sCommand.DataMode			= HAL_XSPI_DATA_1_LINE;
		sCommand.Address			= 0;
		sCommand.Instruction        = 0x90;	// read mfg/dev ID (single)
		sCommand.DummyCycles		= 0;
		sCommand.DataLength			= 2;
		sCommand.AddressWidth		= HAL_XSPI_ADDRESS_24_BITS;

		if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_XSPI_Receive(&hxspi1, data, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			printf("xspi_flash_test: Error = 0x%08X\n\r", HAL_XSPI_GetError(&hxspi1));
			Error_Handler();
		}
		printf("xspi_flash_test: mfg = 0x%02X, dev = 0x%02X\n\r", data[0], data[1]);
	}
	
	if(testflags & 0x02)
	{
		/* test qspi flash program */
		
		for(int i=0;i<256;i++)
			data[i] = i;
		
		//printf("Erasing...\n\r");
		//xspi_flash_eraseblock(0, 256);
		
		printf("Writing...\n\r");
		if(xspi_flash_writepg(0, data, 256))
		{
			printf("Write error\n\r");
			result++;
		}
		else
			printf("Write OK\n\r");
	}
	
	if(testflags & 0x04)
	{
		/* test qspi flash read */
		printf("Reading...\n\r");
		xspi_flash_readbytes(0, data, 256);
		for(int i=0;i<256;i+=16)
		{
			printf("%06X:", i);
			for(int j=0;j<16;j++)
				printf(" %02X", data[i+j]);
			printf("\n\r");
		}
	}
	
	if(testflags & 0x08)
	{
		/* test qspi flash erase */
		printf("Erase Sector...\n\r");
		xspi_flash_erase(0, 0, 256);
	}
	
	if(testflags & 0x10)
	{
		/* test memory mapped read */
		printf("Mem map read...\n\r");
		xspi_flash_memmap(1);
		uint8_t *membyte = (uint8_t *)XSPI1_BASE;
		
		for(int i=0;i<512;i+=16)
		{
			printf("%06X:", i);
			for(int j=0;j<16;j++)
				printf(" %02X", membyte[i+j]);
			printf("\n\r");
		}
		xspi_flash_memmap(0);
	}
	
	return result;
}
