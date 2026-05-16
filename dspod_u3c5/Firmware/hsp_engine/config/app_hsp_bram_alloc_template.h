/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_hsp_bram_alloc_template.h
  * @brief   Header file
  ******************************************************************************
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef APP_HSP_BRAM_ALLOC_TEMPLATE_H
#define APP_HSP_BRAM_ALLOC_TEMPLATE_H
/* USER CODE END Header */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ----------------------------------------------------------------- */
#include "hsp.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup APP_HSP_ENGINE
  * @{
  */
/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Constants Application HSP BRAM Allocation Constants
  * @{
  */
/* Declare #define related to the buffer dimensions */
/*
#define VECTOR_SIZE  10U
#define FIR_SAMPLES_IN_NBR  32U
#define FIR_COEF_NBR  6U
#define FIR_STATE_TAPS_NBR  6U
#define FIR_RESULTS_NBR  32U
...
*/

/* USER CODE BEGIN BRAM Constants */

/* USER CODE END BRAM Constants */

/**
  * @}
  */

/** @defgroup HSP_BRAM_STATIC_Resources HSP BRAM Static Resources
  * @{
  */
/* Externalize any buffer/variable required to be use with any HSP Processing functions */
/*
extern hsp_filter_state_identifier_t fir_state_id;  ...
extern hsp_filter_state_identifier_t fir_state_id2;  ...
extern float32_t *p_buff_in;  ...
extern float32_t *p_coef;  ...
extern float32_t *p_buff_out;  ...
...
*/
/* USER CODE BEGIN BRAM Static Allocation */

/* USER CODE END BRAM Static Allocation */

/**
  * @}
  */

/** @defgroup HSP_BRAM_DYNAMIC_Resources HSP BRAM Dynamic Resources
  * @{
  */
/* Externalize any buffer/variable required to be use with any HSP Processing functions */
/*
extern hsp_filter_state_identifier_t fir_state_id;  ...
extern float32_t *p_buff_in;  ...
extern float32_t *p_coef;  ...
extern float32_t *p_buff_out;  ...
*/
/* USER CODE BEGIN BRAM Dynamic Allocation */

/* USER CODE END BRAM Dynamic Allocation */

/**
  * @}
  */

/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Functions Application HSP BRAM Exported Functions
  * @{
  */
uint32_t MX_HSP_BRAM_Allocation(hsp_core_handle_t *hmw);

/* USER CODE BEGIN Exported Functions */

/* USER CODE END Exported Functions */

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

#endif /* APP_HSP_BRAM_ALLOC_TEMPLATE_H */
