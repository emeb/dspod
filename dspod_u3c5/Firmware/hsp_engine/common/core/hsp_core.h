/**
  ******************************************************************************
  * @file    hsp_core.h
  * @brief   Header file for hsp_core.c
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
#ifndef HSP_CORE_H
#define HSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_conf.h"
#include "hsp_if_conf.h"
#include "hsp_api_def.h"
#include "hsp_hw_if.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_CORE HSP Core
  * @{
  */

/** @defgroup HSP_CORE_Manager HSP Core Manager
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup HSP_CORE_Exported_Constants HSP Core Exported Constants
  * @{
  */

/** @defgroup HSP_CORE_Error_Code  HSP Error Code
  * @{
  */
#define HSP_CORE_ERROR_NONE           (0UL)            /*!< HSP no error */
#define HSP_CORE_ERROR_INIT           (0x01UL << 1U)   /*!< HSP init failed as it was already done */
#define HSP_CORE_ERROR_INVALID_PARAM  (0x01UL << 2U)   /*!< HSP invalid parameter */

/**
  * @}
  */

/**
  * @}
  */

/* Exported Types ------------------------------------------------------------*/
/** @addtogroup HSP_CORE_Exported_Types
  * @{
  */

/** @defgroup HSP_CORE_SmartClocking  HSP Core SmartClocking Enumerations
  * @{
  */
typedef enum
{
  HSP_CORE_SMART_CLOCKING_SPE,
  HSP_CORE_SMART_CLOCKING_MMC,
  HSP_CORE_SMART_CLOCKING_CTRL
} hsp_core_smart_clocking_t;

typedef enum
{
  HSP_CORE_SMART_CLOCKING_DISABLED = 0U,
  HSP_CORE_SMART_CLOCKING_ENABLED = 1U,
} hsp_core_smart_clocking_status_t;

/**
  * @}
  */

/**
  * @}
  */

/* Exported Functions --------------------------------------------------------*/
/** @defgroup HSP_CORE_Exported_Functions HSP Core Exported Functions
  * @{
  */

/** @defgroup HSP_CORE_Exported_Functions_Group1 HSP Init functions
  * @{
  */
hsp_core_status_t HSP_CORE_Init(hsp_engine_context_t *hmw, void *hdriver);
void HSP_CORE_DeInit(hsp_engine_context_t *hmw);

/**
  * @}
  */

/** @defgroup HSP_CORE_Exported_Functions_Group2 HSP Output Trigger Management
  * @{
  */
hsp_core_status_t HSP_CORE_OUTPUT_SetConfig(hsp_engine_context_t *hmw, uint32_t trgo_id,
                                            uint32_t source);
hsp_core_status_t HSP_CORE_OUTPUT_Enable(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_CORE_OUTPUT_Disable(hsp_engine_context_t *hmw);
uint32_t HSP_CORE_OUTPUT_IsEnabled(hsp_engine_context_t *hmw);

/**
  * @}
  */

/** @defgroup HSP_CORE_Exported_Functions_Group3 HSP Smart Clocking Management
  * @{
  */
hsp_core_status_t HSP_CORE_EnableSmartClocking(hsp_engine_context_t *hmw, hsp_core_smart_clocking_t clock);
hsp_core_status_t HSP_CORE_DisableSmartClocking(hsp_engine_context_t *hmw, hsp_core_smart_clocking_t clock);
hsp_core_smart_clocking_status_t HSP_CORE_GetSmartClockingStatus(hsp_engine_context_t *hmw,
                                                                 hsp_core_smart_clocking_t clock);

/**
  * @}
  */

/** @defgroup HSP_CORE_Exported_Functions_Group4 HSP Protection
  * @{
  */
hsp_core_status_t HSP_CORE_Lock(hsp_engine_context_t *hmw);

/**
  * @}
  */

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
hsp_core_status_t HSP_CORE_EnableProclistEventsIT(hsp_engine_context_t *hmw, uint32_t interrupts);
hsp_core_status_t HSP_CORE_DisableProclistEventsIT(hsp_engine_context_t *hmw, uint32_t interrupts);
#endif /* USE_HSP_MODULES_PROCLIST */

/** @defgroup HSP_CORE_Exported_Functions_Group10 Generic and common functions
  * @{
  */
hsp_core_state_t HSP_CORE_GetState(const hsp_engine_context_t *hmw);

/**
  * @}
  */

/** @defgroup HSP_CORE_Exported_Functions_Group20  HSP Register Callback functions
  * @{
  */
hsp_core_status_t HSP_CORE_RegisterErrorCallback(hsp_engine_context_t *hmw, p_hsp_engine_two_params_cb_t p_callback);

#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
hsp_core_status_t HSP_CORE_RegisterStreamBufferWriteCpltCallback(hsp_engine_context_t *hmw,
                                                                 p_hsp_engine_one_param_cb_t p_callback);
hsp_core_status_t HSP_CORE_RegisterStreamBufferReadCpltCallback(hsp_engine_context_t *hmw,
                                                                p_hsp_engine_one_param_cb_t p_callback);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

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

#endif /* HSP_CORE_H */
