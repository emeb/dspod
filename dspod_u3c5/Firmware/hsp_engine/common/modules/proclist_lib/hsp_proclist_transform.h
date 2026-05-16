/**
  ******************************************************************************
  * @file    hsp_proclist_transform.h
  * @brief   Header file for hsp_proclist_transform.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef HSP_PROCLIST_TRANSFORM_H
#define HSP_PROCLIST_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_proclist_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST
  * @{
  */

/** @defgroup HSP_MODULES_PROCLIST_TRANSFORM_LIBRARY HSP Proclist Transform Functions
  * @{
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_Transform_Exported_Functions HSP Transform Exported Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_Fft_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                  uint8_t ifftFlag, uint8_t bitrev, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Rfft_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                   uint8_t ifftFlag, uint8_t bitrev, hsp_type_rfft_cmd_t fftVariant,
                                   uint32_t ioType);
hsp_core_status_t HSP_SEQ_Dct_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                  uint32_t ioType);
hsp_core_status_t HSP_SEQ_IDct_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                   uint32_t ioType);

/**
  * @}
  */

/**
  * @}
  */

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

#endif /* HSP_PROCLIST_TRANSFORM_H */
