/*
 * main.c - dspod_u3c5 blinky test program
 * 05-04-26 E. Brombaugh
 ******************************************************************************
 * Changelog
 *
 * date: 2026-05-04 V0.0
 * Initial creation - copied from u3c5_nucleo
 *
 */

#include "main.h"
#include "usart.h"
#include "cyclesleep.h"
#include "led.h"
#include "shared_i2c.h"
#include "si5351.h"

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
#if 1
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC1;
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
#else
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
#endif
}

/*
 * format MHz to string
 */
void MHz2str(char *buffer, uint32_t freq)
{
    uint32_t MHz, kHz, Hz;

    /* get MHz */
    MHz = freq / 1000000;
    
    /* get kHz */
    freq = freq - 1000000 * MHz;
    kHz = freq / 1000;
    
    /* get Hz */
    Hz = freq - 1000 * kHz;
    
    /* format into string */
    sprintf(buffer, "%3d.%03d,%03d", MHz, kHz, Hz);
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
	printf("\n\n\rdspod U3C5 si5351 test\n\r");
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
		
	/* initialize MCO */
	shared_i2c_init();
	printf("I2C initialized\n\r");
	
	/* init the SI5351 */
	if(si5351_init() == HAL_OK)
	{
		printf("SI5351 inititialized.\n\r");
		
		uint32_t frq;
		char number_str[16];
		
#if 0
		/* Set chl 0 freq */
		frq = 16000000; 
		si5351_set_output_chl(0, frq, 0);
		MHz2str(number_str, frq);
		printf("Channel 0 (HSE alternate) set to %s MHz\n\r", number_str);
		MHz2str(number_str, si5351_get_vco_freq(0));
		printf("Channel 0 vco %s MHz\n\r", number_str);
#endif		
		/* set chl 1 frq */
		frq = 12288000;
		si5351_set_output_chl(1, frq, 0);
		MHz2str(number_str, frq);
		printf("Channel 1 (Audio clock) set to %s MHz\n\r", number_str);
		MHz2str(number_str, si5351_get_vco_freq(1));
		printf("Channel 1 vco %s MHz\n\r", number_str);
		
	}
	else
		printf("SI5351 init failed.\n\r");

		
    /* Infinite loop */
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