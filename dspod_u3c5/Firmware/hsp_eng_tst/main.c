/*
 * main.c - dspod_u3c5 HSP Engine test program
 * 05-11-26 E. Brombaugh
 ******************************************************************************
 * Changelog
 *
 * date: 2026-05-10 V0.0
 * Initial creation - copied from u3c5_nucleo
 *
 */

#include "main.h"
#include "usart.h"
#include "cyclesleep.h"
#include "led.h"
#include "HspNgn.h"

/* build version in simple format */
const char *fwVersionStr = "V0.0";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Hang forever */
  while(1)
  {
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable Epod Booster
  */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
  */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_3);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/*
 * enter here
 */
int main(void)
{
    HAL_Init();
	
	__HAL_RCC_PWR_CLK_ENABLE();
	
	/* Configure the system clock to 96 MHz */
    SystemClock_Config();
	
	/* init the UART for diagnostics */
	setup_usart();
	init_printf(0,usart_putc);
	printf("\n\n\rdspod U3C5 HSP Engine (Middleware) test\n\r");
	printf("CPUID: 0x%08X\n\r", SCB->CPUID);
	printf("IDCODE: 0x%08X\n\r", DBGMCU->IDCODE);
	printf("Version: %s\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	printf("SYSCLK = %d\n\r", HAL_RCC_GetSysClockFreq());
	printf("Voltage Range = %d\n\r", HAL_PWREx_GetVoltageRange());
	printf("\n");
	
	/* initialize LEDs */
	cyccnt_enable();
	printf("cyccnt initialized\n\r");
		
	/* initialize LEDs */
	LEDInit();
	printf("LED initialized\n\r");
	
	if(HspNgn_Init() != HAL_OK)
	{
		printf("HspNgn_Init() failed\n\r");
		Error_Handler();
	}
	printf("HspNgn_Init() OK\n\r");
	
	/* create buffers */
#define MAX_BLOCKSIZE 1024
	printf("HSP test - blocksize = %d\n\r", MAX_BLOCKSIZE);
	float32_t *PP_Buffer = (float32_t *)HSP_BRAM_Malloc(&hmw, (MAX_BLOCKSIZE * 2),
		HSP_BRAM_ALLOCATION_DEFAULT);
	if(PP_Buffer == NULL)
	{
		printf("PP_Buffer create failed\n\r");
		Error_Handler();
	}
	else
		printf("PP_Buffer allocated OK\n\r");
	
	float32_t *Window = (float32_t *)HSP_BRAM_Malloc(&hmw, MAX_BLOCKSIZE,
		HSP_BRAM_ALLOCATION_DEFAULT);
	if(Window == NULL)
	{
		printf("Window create failed\n\r");
		Error_Handler();
	}
		printf("Window allocated OK\n\r");
	
#if 1
	/* This stuff is the DSP_Lib - need some other things compiled to use */
	printf("Testing DSP Lib stuff\n\r");
	// prep fft
	stm32_hsp_cfft_instance_f32 Acc_Cfft;
	stm32_hsp_cfft_init_f32(&Acc_Cfft, MAX_BLOCKSIZE);
	
	start_meas();
	
	// Step 1: Apply window (element-wise multiply)
	stm32_hsp_mult_f32(PP_Buffer, Window, PP_Buffer, MAX_BLOCKSIZE);
	
	// Step 2: Perform complex FFT
	stm32_hsp_cfft_f32(&Acc_Cfft, PP_Buffer, 1, 1);
	
	// Step 3: Compute power spectrum (magnitude squared)
	stm32_hsp_cmplx_mag_squared_f32(PP_Buffer, PP_Buffer, (MAX_BLOCKSIZE >> 1));    /* Infinite loop */

	end_meas();
	uint32_t act, tot;
	get_meas(&act, &tot);
	printf("CPU cycles: %d (%d us)\n\r", act, act/(HAL_RCC_GetSysClockFreq()/1000000));
#endif
	
	printf("Looping...\n\r");
	while(1)
	{
		LEDToggle();
		HAL_Delay(100);
	}
	
	/* should never get here */
	return 0;
}

/*
 * SysTick IRQ handler runs at 1000Hz
 */
void SysTick_Handler(void)
{
	/* Needed by HAL! */
	HAL_IncTick();
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}
