/*
 * HspNgn.c - top-level wrapper for HSP Engine
 * 05-12-2026 E. Brombaugh
 */

#include "HspNgn.h"

/* the engine middleware */
hsp_engine_context_t hmw;
HSP_HandleTypeDef hhsp1;

/* clock initializer is called by HAL during init */
void HAL_HSP_MspInit(HSP_HandleTypeDef* hspHandle)
{
  if(hspHandle->Instance==HSP1)
  {
    /* Enable Peripheral clock */
    __HAL_RCC_HSP1_CLK_ENABLE();
  }
}

/*
 * boilerplate middleware startup
 */
hsp_if_status_t HSP_Engine_IF_Init(hsp_engine_context_t *hmw)
{
  /* Init HSP Driver. */
  hhsp1.global_state = HAL_HSP_STATE_RESET;

  hhsp1.Instance = HSP1;
  if (HAL_HSP_Init(&hhsp1) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Link the driver to the MW. */
  hmw->hdriver = &hhsp1;

  return HSP_IF_OK;
}

/*
 * this is the whole init for HSP + middleware
 */
HAL_StatusTypeDef HspNgn_Init(void)
{
	/* init HSP engine and hardware */
	if (HSP_Engine_IF_Init(&hmw) != HSP_IF_OK)
	{
		printf("HSP_Engine_IF_Init() failed\n\r");
		return HAL_ERROR;
	}

	if (HSP_CORE_Init(&hmw, hmw.hdriver) != HSP_CORE_OK)
	{
		printf("HSP_CORE_Init() failed\n\r");
		return HAL_ERROR;
	}
	
	if (HSP_CORE_Lock(&hmw) != HSP_CORE_OK)
	{
		printf("HSP_CORE_Lock() failed\n\r");
		return HAL_ERROR;
	}
	
	/* init ARM CMSIS DSP emulation layer */
	if(stm32_hsp_init(&hmw) != STM32_HSP_MATH_SUCCESS)
	{
		printf("stm32_hsp_init() failed\n\r");
		return HAL_ERROR;
	}
	
	return HAL_OK;
}
