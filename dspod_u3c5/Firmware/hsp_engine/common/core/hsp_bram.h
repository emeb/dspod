/**
  ******************************************************************************
  * @file    hsp_bram.h
  * @brief   Header file for hsp_bram.c
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
#ifndef HSP_BRAM_H
#define HSP_BRAM_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_conf.h"
#include "hsp_api_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_CORE HSP Core
  * @{
  */

/** @defgroup HSP_BRAM HSP BRAM Manager
  * @{
  */

/** @defgroup HSP_BRAM_Exported_Types HSP BRAM Exported Types
  * @{
  */

/** BRAM Sharing Access Arbitration between CPU & SPE */
typedef enum
{
  HSP_BRAM_ARBITRATION_LATENCY_16_CYCLES,
  HSP_BRAM_ARBITRATION_LATENCY_8_CYCLES,
  HSP_BRAM_ARBITRATION_LATENCY_4_CYCLES
} hsp_bram_arbitration_t;

/**
  * @}
  */

/** @defgroup HSP_BRAM_Exported_Functions HSP Exported Functions
  * @{
  */

/** @defgroup HSP_BRAM_Exported_Functions_Group1 HSP BRAM Allocation Management
  * @{
  */
void *HSP_BRAM_Malloc(hsp_engine_context_t *hmw, uint32_t size_in_word, hsp_bram_allocation_t area);
uint32_t HSP_BRAM_GetFreeSize(hsp_engine_context_t *hmw, hsp_bram_allocation_t area);
hsp_core_status_t HSP_BRAM_SetPopAddress(hsp_engine_context_t *hmw, uint32_t *pop, uint32_t addr);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_Fir(hsp_engine_context_t *hmw, uint32_t nbTaps,
                                                             uint32_t nbSamples, hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_FirDecimate(hsp_engine_context_t *hmw, uint32_t nbTaps,
                                                                     uint32_t nbSamples, uint32_t decimFactor,
                                                                     hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_BiquadCascadeDf1(hsp_engine_context_t *hmw, uint32_t nbStages,
                                                                          hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_BiquadCascadeDf2t(hsp_engine_context_t *hmw, uint32_t nbStages,
                                                                           hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_Lms(hsp_engine_context_t *hmw, uint32_t nbTaps,
                                                             hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_IirLattice(hsp_engine_context_t *hmw, uint32_t nbStages,
                                                                    hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_IirDf1(hsp_engine_context_t *hmw, uint32_t nbStages,
                                                                hsp_bram_allocation_t area);
#if defined(__HSP_DMA__)
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_Iir4p4z(hsp_engine_context_t *hmw, hsp_bram_allocation_t area);
#endif /* __HSP_DMA__ */
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_Iir3p3z(hsp_engine_context_t *hmw, hsp_bram_allocation_t area);
hsp_filter_state_identifier_t HSP_BRAM_MallocStateBuffer_Iir2p2z(hsp_engine_context_t *hmw, hsp_bram_allocation_t area);

/**
  * @}
 */

/** @defgroup HSP_BRAM_Exported_Functions_Group2 HSP BRAM Access Management
  * @{
  */
hsp_core_status_t HSP_BRAM_EnableConflictAccessCounter(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_BRAM_DisableConflictAccessCounter(hsp_engine_context_t *hmw);
uint32_t HSP_BRAM_GetConflictAccessCounter(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_BRAM_SetBandwidthArbitration(hsp_engine_context_t *hmw, hsp_bram_arbitration_t mode);
hsp_bram_arbitration_t HSP_BRAM_GetBandwidthArbitration(hsp_engine_context_t *hmw);

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
#endif  /* __cplusplus */

#endif /* HSP_BRAM_H */
