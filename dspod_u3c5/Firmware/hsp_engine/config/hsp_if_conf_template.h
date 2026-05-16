/**
  ******************************************************************************
  * @file    hsp_if_conf_template.h
  * @brief   Header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HSP_IF_CONF_TEMPLATE_H
#define HSP_IF_CONF_TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32xxxx_hal.h" /* Select the hal file corresponding to the device in use (i.e. stm32f3xx_hal.h, stm32f0xx_hal.h, ...) */
#include "hsp_api_def.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_IF_CONF
  * @{
  */

/** @defgroup HSP_IF_CONF_Exported_Constants
  * @{
  */
/* Defines for TRGIN signals -------------------------------------------------*/
/*
  For ADF: #define HSP_TRGIN_ADF_x  HAL_HSP_TRGIN_ADFxxxxx
    #define HSP_TRGIN_ADF_0  HAL_HSP_TRGIN_ADF1_SAD_DET

  For DMA Transfer Complete: #define HSP_TRGIN_DMA_TC_x  HAL_HSP_TRGIN_<G|H|LP>DMA<y>_CH<z>_TC
    ex: #define HSP_TRGIN_DMA_TC_5  HAL_HSP_TRGIN_HDMA2_CH12_TC

  For EXTI: #define HSP_TRGIN_EXTI_x  HAL_HSP_TRGIN_EXT<y>
    ex: #define HSP_TRGIN_EXTI_5  HAL_HSP_TRGIN_EXTI0

  For LPTIM: #define HSP_TRGIN_LPTIM_x  HAL_HSP_TRGIN_LPTIM<y>_CH<z>
    ex: #define HSP_TRGIN_LPTIM_17  HAL_HSP_TRGIN_LPTIM2_CH2

  For MDF Filter RX Interrupt: #define HSP_TRGIN_MDF_FLT_RX_IT_x  HAL_HSP_TRGIN_MDF_FLT<y>_RX_IT
    ex: #define HSP_TRGIN_MDF_FLT_RX_IT_3 HAL_HSP_TRGIN_MDF_FLT1_RX_IT

  For NPU : #define HSP_TRGIN_NPU_x  HAL_HSP_TRGIN_NPU_<literal><[0..9]*>
    ex: #define HSP_TRGIN_NPU_0  HAL_HSP_TRGIN_NPU_HOST_IN3

  For TIM TRGO: #define HSP_TRGIN_TIM_TRGO_x  HAL_HSP_TRGIN_TIM<y>_TRGO<empty | [0..9]>
    ex: #define HSP_TRGIN_TIM_TRGO_1 HAL_HSP_TRGIN_TIM7_TRGO
    ex: #define HSP_TRGIN_TIM_TRGO_5 HAL_HSP_TRGIN_TIM25_TRGO2

  For PLAY OUT: #define HSP_TRGIN_PLAY_OUT_x  HAL_HSP_TRGIN_PLAY<y>_OUT<z>
    ex: #define HSP_TRGIN_PLAY_OUT_0 HAL_HSP_TRGIN_PLAY1_OUT6

  */
#define HSP_TRGIN_DMA_TC_0   (uint32_t)HAL_HSP_TRGIN_DMA1_CH1_TC
#define HSP_TRGIN_DMA_TC_1   (uint32_t)HAL_HSP_TRGIN_DMA1_CH2_TC
#define HSP_TRGIN_DMA_TC_2   (uint32_t)HAL_HSP_TRGIN_DMA1_CH3_TC
#define HSP_TRGIN_DMA_TC_3   (uint32_t)HAL_HSP_TRGIN_DMA1_CH4_TC

#define HSP_TRGIN_EXTI_0     (uint32_t)HAL_HSP_TRGIN_EXTI0
#define HSP_TRGIN_EXTI_1     (uint32_t)HAL_HSP_TRGIN_EXTI1

#define HSP_TIM_TRGO_0       (uint32_t)HAL_HSP_TRGIN_TIM1_TRGO
#define HSP_TIM_TRGO_1       (uint32_t)HAL_HSP_TRGIN_TIM1_TRGO2
#define HSP_TIM_TRGO_2       (uint32_t)HAL_HSP_TRGIN_TIM2_TRGO
#define HSP_TIM_TRGO_3       (uint32_t)HAL_HSP_TRGIN_TIM3_TRGO
#define HSP_TIM_TRGO_4       (uint32_t)HAL_HSP_TRGIN_TIM6_TRGO
#define HSP_TIM_TRGO_5       (uint32_t)HAL_HSP_TRGIN_TIM7_TRGO
#define HSP_TIM_TRGO_6       (uint32_t)HAL_HSP_TRGIN_TIM8_TRGO
#define HSP_TIM_TRGO_7       (uint32_t)HAL_HSP_TRGIN_TIM8_TRGO2
#define HSP_TIM_TRGO_8       (uint32_t)HAL_HSP_TRGIN_TIM15_TRGO

#define HSP_LPTIM_TRGO_0     (uint32_t)HAL_HSP_TRGIN_LPTIM1_CH1
#define HSP_LPTIM_TRGO_1     (uint32_t)HAL_HSP_TRGIN_LPTIM1_CH2
#define HSP_LPTIM_TRGO_2     (uint32_t)HAL_HSP_TRGIN_LPTIM2_CH1
#define HSP_LPTIM_TRGO_3     (uint32_t)HAL_HSP_TRGIN_LPTIM2_CH2

#define HSP_ADF_0            (uint32_t)HAL_HSP_TRGIN_ADF1_SAD_DET

/**
  * @}
  */

/** @defgroup HSP_IF_CONF_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup HSP_IF_CONF_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup HSP_IF_CONF_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup HSP_IF_CONF_Exported_FunctionsPrototype
  * @{
  */
void *HSP_Engine_IF_Init(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_IF_CONF_TEMPLATE_H */
