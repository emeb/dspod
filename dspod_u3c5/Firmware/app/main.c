/*
 * main.c - dspod_u3c5 effects app
 * 05-09-2026 E. Brombaugh
 ******************************************************************************
 * Changelog
 *
 * date: 2026-05-09 V0.0
 * Initial creation
 *
 */

#include "main.h"
#include "systick.h"
#include "usart.h"
#include "led.h"
#include "cyclesleep.h"
#include "adc.h"
#include "ospi.h"
#include "st7789.h"
#include "audio.h"
#include "sai.h"
#include "codec.h"
#include "shared_i2c.h"
#include "menu.h"
#define DELAY 200

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
	printf("!!! main error handler !!!\n\r");
	
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

/*
 * enter here
 */
int main(void)
{
	/* init encoder & button polling prior to HAL */
	systick_init();
	
	/* init HAL API */
    HAL_Init();
	
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_ICACHE_Enable();

	/* Configure the system clock to 600 MHz */
    SystemClock_Config();
	
	/* init the UART for diagnostics */
	HAL_Delay(100);
	setup_usart();
	init_printf(0,usart_putc);
	printf("\n\n\rdspod_u3c5 FX App\n\r");
	printf("CPUID: 0x%08X\n\r", SCB->CPUID);
	printf("IDCODE: 0x%08X\n\r", DBGMCU->IDCODE);
	printf("Version: %s\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
	printf("SYSCLK = %d\n\r", HAL_RCC_GetSysClockFreq());
	printf("\n");
	
	/* initialize LEDs */
	LEDInit();
	printf("LED initialized\n\r");
		
	/* start cycle counter */
	cyccnt_enable();
	printf("cyclesleep initialized\n\r");
	
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
		printf("SAI1 CLK = %d\n\r", HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SAI1));
	}

	/* initialize the XSPI interface w/ PSRAM */
	ospi_init();
	printf("OSPI initialized\n\r");

	/* set up LCD SPI & GPIO */
	gfx_init(&ST7789_drvr);
	gfx_clrscreen();
	menu_splash(fwVersionStr, bdate, btime);
	ST7789_backlight(1);
	printf("LCD & GFX initialized\n\r");
	
	/* initialize ADC */
	ADC_Init();
	printf("ADC initialized\n\r");
	
	/* initialize audio handler */
	Audio_Init(48000);
	printf("Audio initialized\n\r");
	
	/* start I2S */
	sai_init();
	printf("SAI initialized\n\r");
	printf("	Fsamp = %d\n\r", sai_get_fsamp());

	/* init I2C */
	shared_i2c_init();
	printf("I2C initialized\n\r");

	/* init codec */
	Codec_Init();
	printf("Codec initialized\n\r");

	/* start I2S running */
	sai_start();
	printf("I2S started\n\r");

	/* unmute */
	Audio_Mute(0);

	/* splash delay & init menu */
	HAL_Delay(2000);
	menu_init();
	printf("Menu intialized.\n\r");
	
	/* Infinite loop */
	printf("Looping...\n\r");
	uint32_t led_goal = cyclegoal_ms(250);
	uint32_t menu_goal = cyclegoal_ms(33);
	while(1)
	{
		if(!cyclecheck(led_goal))
		{
			led_goal = cyclegoal_ms(250);		
			LEDToggle();
		}
		
		if(!cyclecheck(menu_goal))
		{
			/* handle menu */
			menu_process();
			
			menu_goal = cyclegoal_ms(33);
		}
	}
	
	/* should never get here */
	return 0;
}


/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
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

/**
  * @brief This function handles Secure fault.
  */
void SecureFault_Handler(void)
{
  /* USER CODE BEGIN SecureFault_IRQn 0 */

  /* USER CODE END SecureFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_SecureFault_IRQn 0 */
    /* USER CODE END W1_SecureFault_IRQn 0 */
  }
}
