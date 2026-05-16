/**
  ******************************************************************************
  * @file    hsp_proclist_filter.h
  * @brief   Header file for hsp_proclist_filter.c
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
#ifndef HSP_PROCLIST_FILTER_H
#define HSP_PROCLIST_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_proclist_def.h"
#include "hsp_bram.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST
  * @{
  */

/** @defgroup HSP_MODULES_PROCLIST_FILTER_LIBRARY HSP Proclist Filter Functions
  * @{
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_Filter_Exported_Functions HSP Filter Exported Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_Fir_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                  hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                  uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_FirDecimate_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                          hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                          uint32_t nbSamples,
                                          uint32_t decimFactor, uint32_t ioType);
hsp_core_status_t HSP_SEQ_BiquadCascadeDf1_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                               hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                               uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_BiquadCascadeDf2T_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                                hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                                uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Conv_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                   float32_t *outBuff,
                                   uint32_t sizeA, uint32_t sizeB, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Correlate_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                        float32_t *outBuff, uint32_t sizeA, uint32_t sizeB, uint32_t ioType);
hsp_core_status_t HSP_SEQ_FltBank_f32(hsp_engine_context_t *hmw, float32_t *spectrCol, uint32_t *startIdx,
                                      uint32_t *idxSize, float32_t *coef, float32_t *fltbankCol,
                                      uint32_t nFltbanks, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Lms_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                  hsp_filter_state_identifier_t staBuff,
                                  float32_t *outBuff, float32_t *refBuff,
                                  float32_t *errBuff, uint32_t nbSamples, float32_t mu, uint32_t ioType);
hsp_core_status_t HSP_SEQ_IirLattice_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coeffsk,
                                         float32_t *coeffsv, hsp_filter_state_identifier_t staBuff,
                                         float32_t *outBuff,
                                         uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_IirDf1_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                     hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                     uint32_t nbSamples, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Iir3p3z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Iir2p2z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_WinSym_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff, uint32_t sizeW, uint32_t sizeD0, uint32_t ioType);
#if defined(__HSP_DMA__)
hsp_core_status_t HSP_SEQ_Iir4p4z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_FltBankExtC_f32(hsp_engine_context_t *hmw, float32_t *spectrCol, uint32_t *startIdx,
                                          uint32_t *idxSize, float32_t *coef, float32_t *fltbankCol,
                                          uint32_t nFltbanks,
                                          float32_t *dmaAdd, uint32_t dmaSize, uint32_t ioType);
#endif /* __HSP_DMA__ */

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

#endif /* HSP_PROCLIST_FILTER_H */
