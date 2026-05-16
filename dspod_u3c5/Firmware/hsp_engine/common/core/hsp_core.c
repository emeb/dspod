/**
  ******************************************************************************
  * @file    hsp_core.c
  * @brief   This file implements the HSP Core functions.
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

/* Includes ------------------------------------------------------------------*/
#include "hsp_core.h"
#include "hsp_bram_if.h"
#include "hsp_hw_if.h"
#include "hsp_fw_ram.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_Engine_Configuration_Table HSP Engine Configuration Table
  * @{
  # Configuration inside the HSP Engine middleware

Software configuration defined in hsp_conf.h:
Prepocessor flags                      | Default value | Comment
-------------------------------------- | ------------- | ------------------------------------------------
USE_HSP_MODULES_DIRECT_LIB             |       1       | Enable HSP Engine Direct Command module
USE_HSP_MODULES_DSP_LIB                |       0       | Enable HSP Engine DSP module (ARM-like API)
USE_HSP_MODULES_CNN_LIB                |       0       | Enable HSP Engine CNN module
USE_HSP_MODULES_PROCLIST_COMPLEX       |       0       | Enable HSP Engine Proclist Complex module
USE_HSP_MODULES_PROCLIST_CONDITIONAL   |       0       | Enable HSP Engine Proclist Conditional module
USE_HSP_MODULES_PROCLIST_FILTER        |       0       | Enable HSP Engine Proclist Filter module
USE_HSP_MODULES_PROCLIST_MATRIX        |       0       | Enable HSP Engine Proclist Matrix module
USE_HSP_MODULES_PROCLIST_SCALAR        |       0       | Enable HSP Engine Proclist Scalar module
USE_HSP_MODULES_PROCLIST_TRANSFORM     |       0       | Enable HSP Engine Proclist Transform module
USE_HSP_MODULES_PROCLIST_VECTOR        |       0       | Enable HSP Engine Proclist Vector module
USE_HSP_MEMCPY                         |       1       | Enable HSP Engine memcopy function
USE_HSP_ACC_DIRECT_COMMAND_SYNCHRONOUS |       1       | Enable HSP Engine Direct Command synchronous mode
USE_HSP_ENGINE_STREAM_BUFFER           |       0       | Enable HSP Engine STREAM Buffer feature
ENABLE_HSP_PERFORMANCE_MONITOR         |       0       | Enable the HSP Performance Monitor counter
HSP_BRAM_AI_SIZE                       |       0       | Configure the BRAMAB size reserved for CNN processing
USE_HSP_PLUGIN                         |       0       | Enable the HSP Engine Plugin feature

Software configuration defined in preprocessor environment:
Prepocessor flags              |   Default value   | Comment
------------------------------ | ----------------- | ------------------------------------------------
USE_ASSERT_DBG_PARAM           |    Not defined    | Enable check param for MW
USE_ASSERT_DBG_STATE           |    Not defined    | Enable check state for MW

  */

/**
  * @}
  */

/** @addtogroup HSP_CORE
  * @{
  */

/** @addtogroup HSP_CORE_Manager
  * @{
  */

/** @defgroup HSP_CORE_Init_Private_Functions HSP Core Private Functions
  * @{
  */
static hsp_core_status_t HSP_CORE_LoadFW(hsp_engine_context_t *hmw);
#if defined(USE_HSP_PLUGIN) && (USE_HSP_PLUGIN == 1)
static hsp_core_status_t HSP_CORE_LoadPlugins(hsp_engine_context_t *hmw);
#endif /* USE_HSP_PLUGIN */

/**
  * @}
  */

/* Exported Functions --------------------------------------------------------*/
/** @addtogroup HSP_CORE_Exported_Functions
  * @{
  */

/** @defgroup HSP_CORE_Exported_Functions_Group1 HSP Init functions
  * @{
  */
/**
  * @brief  Initialize the MW HSP Engine according to the associated driver.
  * @param  hmw      Pointer to a @ref hsp_engine_context_t.
  * @param  hdriver  Pointer to a HSP driver.
  * @retval HSP_CORE_OK            Operation successful.
  * @retval HSP_CORE_INVALID_PARAM invalid parameter.
  * @retval HSP_CORE_ERROR         Operation failed.
  */
hsp_core_status_t HSP_CORE_Init(hsp_engine_context_t *hmw, void *hdriver)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

#if defined(USE_HSP_CHECK_PARAM) && (USE_HSP_CHECK_PARAM == 1)
  /* Check whether the HSP handle is valid */
  if (hmw == NULL)
  {
    return HSP_CORE_INVALID_PARAM;
  }

  if (hdriver == NULL)
  {
    return HSP_CORE_INVALID_PARAM;
  }
#endif /* USE_HSP_CHECK_PARAM */

  /* Link the HSP Driver to the MW Context -----------------------------------*/
  hmw->hdriver = hdriver;

  /* Interfaces Initialization -----------------------------------------------*/
  HSP_HW_IF_Init(hmw->hdriver, 0U);

  /* Initialize the BRAM memory interface */
  if (HSP_BRAM_IF_Init(&(hmw->hbram), hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  /* Load the Firmware & Plugin ----------------------------------------------*/
  /* Call the private function to load Firmware */
  if (HSP_CORE_LoadFW(hmw) != HSP_CORE_OK)
  {
    return HSP_CORE_ERROR;
  }

#if defined(USE_HSP_PLUGIN) && (USE_HSP_PLUGIN == 1)
  /* Call the weak function to load Plugins */
  if (HSP_CORE_LoadPlugins(hmw) != HSP_CORE_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* USE_HSP_PLUGIN */

  /* Change HSP Middleware state */
  hmw->global_state = HSP_CORE_STATE_READY;

  /* Boot the HSP */
  if (HSP_HW_IF_Boot(hmw->hdriver, ENABLE_HSP_PERFORMANCE_MONITOR) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  /* Change HSP Middleware state */
  hmw->global_state = HSP_CORE_STATE_IDLE;

  return HSP_CORE_OK;
}

/**
  * @brief  DeInitialize the MW HSP Engine.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  */
void HSP_CORE_DeInit(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  /* DeInit the Hardware interface */
  HSP_HW_IF_DeInit(hmw->hdriver);

  /* DeInit the BRAM Memory interface */
  HSP_BRAM_IF_DeInit(&(hmw->hbram));
}

/**
  * @}
  */

/** @addtogroup HSP_CORE_Exported_Functions_Group2
  * @{
  */
/**
  * @brief Configure a TRGO.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  trgo_id  TRGO ID
  * @param  source   Trigger source
  * @note literals are defined in hsp_if_conf.h
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_OUTPUT_SetConfig(hsp_engine_context_t *hmw, uint32_t trgo_id, uint32_t source)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_OUTPUT_SetConfig(hmw->hdriver, trgo_id, source) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Enable the HSP Outputs.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_OUTPUT_Enable(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_OUTPUT_Enable(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Disable the HSP Outputs.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_OUTPUT_Disable(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_OUTPUT_Disable(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Get the HSP Outputs enabling status.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
uint32_t HSP_CORE_OUTPUT_IsEnabled(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  return HSP_HW_IF_OUTPUT_IsEnabled(hmw->hdriver);
}

/**
  * @}
 */

/** @addtogroup HSP_CORE_Exported_Functions_Group3
  * @{
  */
/**
  * @brief Enable the SmartClocking of HSP block.
  * @param  hmw    Pointer to a @ref hsp_engine_context_t.
  * @param  clock  Block clock
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_EnableSmartClocking(hsp_engine_context_t *hmw, hsp_core_smart_clocking_t clock)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_EnableSmartClocking(hmw->hdriver, (uint32_t)clock) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Disable the SmartClocking of HSP block.
  * @param  hmw    Pointer to a @ref hsp_engine_context_t.
  * @param  clock  Block clock
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_DisableSmartClocking(hsp_engine_context_t *hmw, hsp_core_smart_clocking_t clock)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_DisableSmartClocking(hmw->hdriver, (uint32_t)clock) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Get the SmartClocking of HSP block status.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  clock  Block clock.
  * @retval HSP_CORE_SMART_CLOCKING_DISABLED  SmartClocking is disabled.
  * @retval HSP_CORE_SMART_CLOCKING_ENABLED   SmartClocking is enabled.
  */
hsp_core_smart_clocking_status_t HSP_CORE_GetSmartClockingStatus(hsp_engine_context_t *hmw,
                                                                 hsp_core_smart_clocking_t clock)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  return HSP_CORE_SMART_CLOCKING_ENABLED;
}

/**
  * @}
 */

/** @addtogroup HSP_CORE_Exported_Functions_Group4
  * @{
  */
/**
  * @brief Lock the HSP configuration.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_Lock(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_Lock(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_CORE_Exported_Functions_Group10
  * @{
  */
/**
  * @brief Retrieve the Middleware state.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_STATE_RESET
  * @retval HSP_CORE_STATE_FW_LOADED
  * @retval HSP_CORE_STATE_READY
  * @retval HSP_CORE_STATE_IDLE
  * @retval HSP_CORE_STATE_PROCLIST_RECORDING
  * @retval HSP_CORE_STATE_CNN_ACTIVE
  * @retval HSP_CORE_STATE_FAULT
  */
hsp_core_state_t HSP_CORE_GetState(const hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  return hmw->global_state;
}

/**
  * @}
  */

/** @addtogroup HSP_CORE_Exported_Functions_Group20
  * @{
  */
/**
  * @brief  Register a User HSP callback for Error interrupt.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_RegisterErrorCallback(hsp_engine_context_t *hmw, p_hsp_engine_two_params_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  if (HSP_HW_IF_RegisterErrorCallback(hmw->hdriver, p_callback) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/**
  * @brief  Register a User HSP callback for STREAM Buffer Write complete interrupt.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_RegisterStreamBufferWriteCpltCallback(hsp_engine_context_t *hmw,
                                                                 p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  if (HSP_HW_IF_RegisterStreamBufferWriteCpltCallback(hmw->hdriver, p_callback) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief  Register a User HSP callback for STREAM Buffer Read complete interrupt.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_CORE_RegisterStreamBufferReadCpltCallback(hsp_engine_context_t *hmw,
                                                                p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  if (HSP_HW_IF_RegisterStreamBufferReadCpltCallback(hmw->hdriver, p_callback) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @}
  */

/** @addtogroup HSP_CORE_Init_Private_Functions
  * @{
  */
/**
  * @brief  Load the HSP Firmware.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_BUSY   Wrong state
  * @retval HSP_CORE_ERROR  Operation failed.
  */
static hsp_core_status_t HSP_CORE_LoadFW(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_RESET);

  if (HSP_HW_IF_LoadFW(hmw->hdriver, hsp_fw_getcram(), hsp_fw_getdram()) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  HSP_CHECK_UPDATE_STATE(hmw, global_state, HSP_CORE_STATE_RESET, HSP_CORE_STATE_FW_LOADED);
  return HSP_CORE_OK;
}

#if defined(USE_HSP_PLUGIN) && (USE_HSP_PLUGIN == 1)
/**
  * @brief  Load the HSP Plugins.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
static hsp_core_status_t HSP_CORE_LoadPlugins(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  /* Load a set of Plugins */
  for (uint32_t id = 0UL; id < HSP_PLUGIN_ARRAY_SIZE; id++)
  {
    hsp_core_plugin_t plugin = HSP_PLUGIN_ARRAY_NAME[id];
    if (HSP_HW_IF_LoadPlugin(hmw->hdriver, plugin.p_bin, plugin.size_byte) != HSP_IF_OK)
    {
      return HSP_CORE_ERROR;
    }
  }
  return HSP_CORE_OK;
}
#endif /* USE_HSP_PLUGIN */

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
