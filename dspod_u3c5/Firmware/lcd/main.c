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
#include "st7789.h"

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
 * enter here
 */
int main(void)
{
    /* HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
    HAL_Init();
	
	__HAL_RCC_PWR_CLK_ENABLE();
	
	/* Configure the system clock to 96 MHz */
    SystemClock_Config();
	
	/* init the UART for diagnostics */
	setup_usart();
	init_printf(0,usart_putc);
	printf("\n\n\rdspod U3C5 LCD test\n\r");
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
		
	/* set up LCD SPI & GPIO */
	gfx_init(&ST7789_drvr);
	start_meas();
	gfx_clrscreen();
	end_meas();
	uint32_t act, tot;
	get_meas(&act, &tot);
	printf("Clearscreen CPU cycles: %d (%d us)\n\r", act, act/(HAL_RCC_GetSysClockFreq()/1000000));
	ST7789_backlight(1);
	printf("LCD & GFX initialized\n\r");
	
#if 0
	/* check overhead */
	printf("Looping forever on clrscreen()\n\r");
	while(1)
			gfx_clrscreen();
#endif
	
	/* gfx tests */
#if 0
	printf("Test clr, text, circles \n\r");
	gfx_drawstr(0, 0, "Hello World!");
	gfx_set_forecolor(GFX_RED);
	gfx_fillcircle(40, 60, 30);
	gfx_set_forecolor(GFX_GREEN);
	gfx_fillcircle(70, 90, 30);
	gfx_set_forecolor(GFX_BLUE);
	gfx_fillcircle(100, 120, 30);
#endif
#if 0
	printf("Test offsets.\n\r");
	gfx_drawline(0, 0, 319, 169);
	gfx_drawline(319, 0, 0, 169);
	gfx_drawstr(0, 0, "0, 0");
	gfx_drawstr(160, 85, "160, 85");
	gfx_drawstr(255, 161, "255, 161");
#endif
#if 1
	printf("Rounded rects\n\r");
	gfx_set_forecolor(GFX_WHITE);
	GFX_RECT rect = {2,2,317,167};
	gfx_fillroundedrect(&rect, 20);
	
	rect.x0 = 50;
	rect.y0 = 50;
	rect.x1 = 150;
	rect.y1 = 150;
	gfx_set_forecolor(GFX_CYAN);
	gfx_fillroundedrect(&rect, 20);
	
	rect.x0 = 200;
	rect.y0 = 20;
	rect.x1 = 250;
	rect.y1 = 160;
	gfx_set_forecolor(GFX_MAGENTA);
	gfx_fillroundedrect(&rect, 50);
	
	rect.x0 = 20;
	rect.y0 = 10;
	rect.x1 = 170;
	rect.y1 = 40;
	gfx_set_forecolor(GFX_BLUE);
	gfx_fillroundedrect(&rect, 5);
#endif

#if 0
#define SLPTM 1000
	printf("looping on sleep tests...\n\r");
	while(1)
	{
		gfx_set_backcolor(GFX_BLACK);
		printf("Bk ");
		gfx_clrscreen();
		HAL_Delay(SLPTM);
		gfx_set_backcolor(GFX_RED);
		printf("Rd ");
		gfx_clrscreen();
		HAL_Delay(SLPTM);
		gfx_set_backcolor(GFX_GREEN);
		printf("Gn ");
		gfx_clrscreen();
		HAL_Delay(SLPTM);
		gfx_set_backcolor(GFX_BLUE);
		printf("Bl ");
		gfx_clrscreen();
		HAL_Delay(SLPTM);
		gfx_set_backcolor(GFX_WHITE);
		printf("Wt\n\r");
		gfx_clrscreen();
		HAL_Delay(SLPTM);
	}
#endif

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