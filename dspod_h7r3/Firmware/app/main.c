/*
 * main.c - dspod_h7r3 effects app
 * 11-15-25 E. Brombaugh
 ******************************************************************************
 * Changelog
 *
 * date: 2025-11-15 V0.0
 * Initial creation
 *
 */

#include "main.h"
#include "systick.h"
#include "usart.h"
#include "led.h"
#include "cyclesleep.h"
#include "xspi.h"
#include "adc.h"
#include "audio.h"
#include "i2s.h"
#include "codec.h"
#include "shared_i2c.h"
#include "st7789.h"
#include "menu.h"

#define DELAY 200

/* build version in simple format */
const char *fwVersionStr = "V0.0";

/* build time */
const char *bdate = __DATE__;
const char *btime = __TIME__;

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /* Disables all MPU regions */
  for(uint8_t i=0; i<__MPU_REGIONCOUNT; i++)
  {
    HAL_MPU_DisableRegion(i);
  }

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* enable R/W access to XSPI1 memmap */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = XSPI1_BASE;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);  

#ifdef USE_CACHE
  /* disable cache on AHB SRAM so it can be used for DMA */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);  
#endif

  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL1.PLLM = 3;
  RCC_OscInitStruct.PLL1.PLLN = 300;
  RCC_OscInitStruct.PLL1.PLLP = 2;
  RCC_OscInitStruct.PLL1.PLLQ = 2;
  RCC_OscInitStruct.PLL1.PLLR = 2;
  RCC_OscInitStruct.PLL1.PLLS = 2;
  RCC_OscInitStruct.PLL1.PLLT = 2;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL2.PLLM = 3;
  RCC_OscInitStruct.PLL2.PLLN = 196;
  RCC_OscInitStruct.PLL2.PLLP = 16;
  RCC_OscInitStruct.PLL2.PLLQ = 2;
  RCC_OscInitStruct.PLL2.PLLR = 2;
  RCC_OscInitStruct.PLL2.PLLS = 2;
  RCC_OscInitStruct.PLL2.PLLT = 2;
  RCC_OscInitStruct.PLL2.PLLFractional = 4981;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL3.PLLM = 3;
  RCC_OscInitStruct.PLL3.PLLN = 128;
  RCC_OscInitStruct.PLL3.PLLP = 2;
  RCC_OscInitStruct.PLL3.PLLQ = 16;
  RCC_OscInitStruct.PLL3.PLLR = 2;
  RCC_OscInitStruct.PLL3.PLLS = 2;
  RCC_OscInitStruct.PLL3.PLLT = 2;
  RCC_OscInitStruct.PLL3.PLLFractional = 0;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK4|RCC_CLOCKTYPE_PCLK5;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/*
 * Test AHB memory
 */
void test_mem(void)
{
#define TST_LEN (1<<5)
	uint32_t *tstbuffer = (uint32_t *)0x30004000;
	int32_t i, err;
	
	printf("Testing AHB SRAM\n\r");
	printf("Writing %d\n\r", TST_LEN);
	for(i=0;i<TST_LEN;i++)
	{
		tstbuffer[i] = i;
	}

	printf("Reading %d\n\r", TST_LEN);
	err = 0;
	for(i=0;i<TST_LEN;i++)
	{
		if(tstbuffer[i] != i)
		{
			err++;
			printf("%6d : %08X -> %08X\n\r", i, i, tstbuffer[i]);
		}
	}
	
	if(err==0)
		printf("Passed.\n\r");
	else
	{
		printf("Failed %d.\n\r", err);
	}
}

/*
 * enter here
 */
int main(void)
{
	/* Cache Configuration---------------------------------------------------*/
#ifdef USE_CACHE
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
#endif
	
	/* MPU Configuration-----------------------------------------------------*/
#ifdef USE_MPU
	MPU_Config();
#else
	HAL_MPU_Disable();
#endif
	
    /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
	
	/* init encoder & button polling prior to HAL */
	systick_init();
	
	/* init HAL API */
    HAL_Init();
	
	/* this board uses only the LDO - disable the SMPS stepdown */
	if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK)
	{
		/* Initialization error */
		Error_Handler();
	}

	/* Configure the system clock to 600 MHz */
    SystemClock_Config();
	
	/* init the UART for diagnostics */
	HAL_Delay(100);
	setup_usart();
	init_printf(0,usart_putc);
	printf("\n\n\rdspod_h7r3 fx app\n\r");
	printf("CPUID: 0x%08X\n\r", SCB->CPUID);
	printf("IDCODE: 0x%08X\n\r", DBGMCU->IDCODE);
	printf("Version: %s\n\r", fwVersionStr);
	printf("Build Date: %s\n\r", bdate);
	printf("Build Time: %s\n\r", btime);
	printf("\n");
#ifdef USE_CACHE
	printf("Cache Enabled\n\r", btime);
#else
	printf("Cache Disabled\n\r", btime);
#endif
#ifdef USE_MPU
	printf("MPU Enabled\n\r", btime);
#else
	printf("MPU Disabled\n\r", btime);
#endif
	printf("\n");
	printf("SYSCLK = %d\n\r", HAL_RCC_GetSysClockFreq());
	printf("\n");
	
	/* initialize LEDs */
	LEDInit();
	printf("LED initialized\n\r");
		
	/* start cycle counter */
	cyccnt_enable();
	printf("cyclesleep initialized\n\r");
	
	/* initialize the XSPI interface w/ PSRAM */
	xspi_init();
	printf("XSPI initialized\n\r");

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
	i2s_init();
	printf("I2S initialized\n\r");
	printf("	Fsamp = %d\n\r", i2s_get_fsamp());
		
	/* init I2C */
	shared_i2c_init();
	printf("I2C initialized\n\r");

	/* init codec */
	Codec_Init();
	printf("Codec initialized\n\r");
	
	/* start I2S running */
	i2s_start();
	printf("I2S started\n\r");
		
#if 1
	xspi_test(0x8f);
	printf("Testing XSPI timing...\n\r");
#if 0
#define TSTSZ 10
	while(1)
	{
		uint32_t i, act, tot, sum = 0, cnt = 0;
		uint8_t buf[TSTSZ];
		
		for(i=0;i<16;i+=2)
		{
			start_meas();
			xspi_writebytes(i, buf, TSTSZ);
			end_meas();
			get_meas(&act, &tot);
			sum += act;
			cnt++;
		}
		printf("Avg Write time = %d (%d us)\n\r", sum/cnt, (sum/cnt)/600);
		HAL_Delay(100);
	}
#endif
#endif

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

/*
 * for debug
 */
void HardFault_Handler(void)
{
	printf("Hardfault handler\n\r");

	Error_Handler();
}

void MemManage_Handler(void)
{
	printf("MemManage handler\n\r");

	Error_Handler();
}

void BusFault_Handler(void)
{
	printf("BusFault handler\n\r");

	Error_Handler();
}
