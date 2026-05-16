/**
  ******************************************************************************
  * @file    hsp_proclist_complex.h
  * @brief   Header file for hsp_proclist_complex.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef HSP_PROCLIST_COMPLEX_H
#define HSP_PROCLIST_COMPLEX_H

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

/** @defgroup HSP_MODULES_PROCLIST HSP Modules Processing List
  * @details
  * This section includes all functions and definitions related to the sequencer (SEQ) mode of the HSP
  * @{
  */

/** @defgroup HSP_MODULES_PROCLIST_COMPLEX_LIBRARY HSP Proclist Complex Functions
  * @{
  */

/** @defgroup HSP_COMPLEX_Exported_Functions HSP COMPLEX Exported Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_CmplxConj_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                        uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxDotProd_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                           float32_t *outBuff, uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxMag_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                       uint32_t nbSamples,
                                       uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxFastMag_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                           uint32_t nbSamples,
                                           uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxMagSquared_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                              uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxMul_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                       float32_t *outBuff,
                                       uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxRMul_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                        float32_t *outBuff,
                                        uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_CmplxMulExp_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *startBuff,
                                          float32_t *outBuff, uint32_t nbSamples, int32_t step, uint32_t ioType);

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

#endif /* HSP_PROCLIST_COMPLEX_H */
