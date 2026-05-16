/**
  ******************************************************************************
  * @file           : Target/hsp_if_conf_template.c
  * @brief          : This file implements the interface between
  *                   the HSP Engine MW and HSP Peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "hsp_if_conf.h"

/* Private variables ---------------------------------------------------------*/

HSP_HandleTypeDef hhsp1;

/* Private function prototypes -----------------------------------------------*/


/* MSP Init */
void HAL_HSP_MspInit(HSP_HandleTypeDef *hspHandle)
{
  if (hspHandle->Instance == HSP1)
  {
    /* USER CODE BEGIN HSP1_MspInit 0 */

    /* USER CODE END HSP1_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_HSP1_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(HSP1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(HSP1_IRQn);
    /* USER CODE BEGIN HSP1_MspInit 1 */

    /* USER CODE END HSP1_MspInit 1 */
  }
}

void HAL_HSP_MspDeInit(HSP_HandleTypeDef *hspHandle)
{
  if (hspHandle->Instance == HSP1)
  {
    /* USER CODE BEGIN HSP1_MspDeInit 0 */

    /* USER CODE END HSP1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_HSP1_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(HSP1_IRQn);

    /* USER CODE BEGIN HSP1_MspDeInit 1 */

    /* USER CODE END HSP1_MspDeInit 1 */
  }
}

/*******************************************************************************
                       HAL Driver Interface (HSP Engine Library --> HSP)
  *******************************************************************************/

/**
  * @brief  Initializes the low level portion of the driver.
  * @param  hmw  HSP Engine handle
  * @retval hsp_if_status_t status
  */
void *HSP_Engine_IF_Init(void)
{
  /* Init HSP Driver. */
  hhsp1.Instance = HSP1;
  hhsp1.global_state = HAL_HSP_STATE_RESET;
  if (HAL_HSP_Init(&hhsp1) != HAL_OK)
  {
    return NULL;
  }

  return (void *)&hhsp1;
}
