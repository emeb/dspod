/*
 * main.c - dspod_u3c5 pdm test program
 * 05-06-26 E. Brombaugh
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
#include "st7789.h"
#include "pdm.h"

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS|RCC_OSCILLATORTYPE_MSIK|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.MSIKState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKSource = RCC_MSI_RC1;
  RCC_OscInitStruct.MSIKDiv = RCC_MSI_DIV2;
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

int16_t wave[320] = {0};
	
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
	printf("\n\n\rdspod U3C5 blinky\n\r");
	printf("CPUID: 0x%08X\n\r", SCB->CPUID);
	printf("IDCODE: 0x%08X\n\r", DBGMCU->IDCODE);
	printf("Version: %s\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	uint32_t clkfrq = HAL_RCC_GetSysClockFreq();
	printf("SYSCLK = %d\n\r", clkfrq);
	printf("Voltage Range = %d\n\r", HAL_PWREx_GetVoltageRange());
	printf("\n");
	
	/* initialize LEDs */
	cyccnt_enable();
	printf("cyccnt initialized\n\r");
		
	/* initialize LEDs */
	LEDInit();
	printf("LED initialized\n\r");
		
#if 1
	/* Init LCD */
	gfx_init(&ST7789_drvr);
	ST7789_backlight(1);
	gfx_drawstr(0,0, "running");
	printf("LCD & GFX initialized\n\r");
#endif

	/* turn on MSIS PLL mode using HSE */
	RCC_MSIRCxPLLTypeDef MSI_PLL = {0};
	MSI_PLL.State = RCC_MSIRCx_PLL_ON;
	MSI_PLL.InputSrce = RCC_MSIRCx_PLL_INPUT_HSE;
	MSI_PLL.FastMode = RCC_MSIRCx_PLL_FAST_DISABLE;
	//MSI_PLL.MSIRC1PLLN = RCC_MSIRC1_PLLN_732;	// 24.016MHz
	//MSI_PLL.MSIRC1PLLN = RCC_MSIRC1_PLLN_689;	// 22.581MHz
	MSI_PLL.MSIRC1PLLN = RCC_MSIRC1_PLLN_750;	// 24.577MHz
	HAL_StatusTypeDef status;
	if((status = HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC1, &MSI_PLL)) != HAL_OK)
	{
		printf("HAL_RCCEx_MSIRCxPLLModeConfig failed %d\n\r", status);
	}
	else
	{
		printf("MSIS running from HSE in PLL Mode\n\r");
		printf("SYSCLK = %d\n\r", HAL_RCC_GetSysClockFreq());
	}

	/* initialize pdm */
	if(PDMInit() == HAL_OK)
	{
		printf("PDM initialized\n\r");
		printf("ADF1 CLK = %d\n\r", HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_ADF1));
		printf("ADF1_Filter0_NS->DFLTCR = 0x%08X\n\r", ADF1_Filter0_NS->DFLTCR);
	}
	else
		printf("PDM init failed\n\r");

    /* Infinite loop */
	printf("Looping...\n\r");
	uint32_t ledgoal = cyclegoal_ms(100);
	uint32_t act, tot;
	clkfrq = clkfrq/1000000;	// for us
	while(1)
	{
		if(!cyclecheck(ledgoal))
		{
			ledgoal = cyclegoal_ms(100);
			get_meas(&act, &tot);
			printf("max: 0x%08X period %d us, CPU Load %d / %d (%d%%)   \r", PDM_GetMax(), tot/clkfrq, act, tot, (100*act)/tot);
			LEDToggle();
		}
		
		if(PDM_ChkSamp())
		{
			/* redraw waveform on screen */
			for(int16_t x = 0 ; x < 320 ; x++)
			{
				GFX_POINT pt;
				pt.x = x;
				pt.y = wave[x];
				gfx_clrpixel(pt);
				wave[x] = ST7789_TFTWIDTH/2 - (pdm_sample[x]>>8);
				pt.y = wave[x];
				gfx_setpixel(pt);
			}
			
			/* capture another */
			PDM_StartSamp();
		}
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