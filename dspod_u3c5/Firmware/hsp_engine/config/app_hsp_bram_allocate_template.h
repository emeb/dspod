/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/app_hsp_bram_allocate.h
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_HSP_BRAM_ALLOCATE_H
#define APP_HSP_BRAM_ALLOCATE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp.h"

/** @addtogroup APP_HSP_ENGINE
  * @{
  */

/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Constants Application HSP BRAM Allocation Constants
  * @{
  */
/* Each define related to a BRAM resource must have the prefix "HSP_BRAM_RSRC_"
#define HSP_BRAM_RSRC_VECTOR1_SIZE  10U
#define HSP_BRAM_RSRC_VECTOR2_SIZE  10U
#define HSP_BRAM_RSRC_VECTOR3_SIZE  32U
#define HSP_BRAM_RSRC_FIR_STATE_TAPS_NBR  51U
#define HSP_BRAM_RSRC_FIR_SAMPLES_IN_NBR  256U
*/

/**
  * @}
  */

/** @defgroup HSP_BRAM_DYNAMIC_Resources HSP BRAM Dynamic Resources
  * @{
  */
/* All BRAM resources are declared in the unique structure hsp_bram_resources_t like this:
typedef struct {
 float32_t *p_vect1;
 float32_t *p_vect2;
 int32_t *p_vect3;

 hsp_filter_state_identifier_t fir_state_id;
 float32_t *p_buff_in;
 float32_t *p_results;

} hsp_bram_resources_t;
*/

/**
  * @}
  */

/** @defgroup APP_HSP_BRAM_ALLOC_Exported_Functions Application HSP BRAM Exported Functions
  * @{
  */

uint32_t MX_HSP_BRAM_Allocation(hsp_engine_context_t *hmw);

hsp_bram_resources_t *MX_HSP_BRAM_GetResources(void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APP_HSP_BRAM_ALLOCATE_H */
