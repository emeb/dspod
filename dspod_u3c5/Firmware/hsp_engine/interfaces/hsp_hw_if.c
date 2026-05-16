/**
  ******************************************************************************
  * @file    hsp_hw_if.c
  * @brief   This file implements the service provide by the HSP Hardware
  *          Interface
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

/* Includes ------------------------------------------------------------------*/
#include "hsp_hw_if.h"
#include "hsp_fw_def.h"
#include "hsp_fw_ram.h"


/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_INTERFACES
  * @{
  */

/** @addtogroup HSP_HW_IF
  * @{
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup HSP_HW_IF_Private_Constants HSP_HW_IF Private Constants
  * @{
  */
#define HSP_ENGINE_INSTANCE_COUNT 1U

#define GET_HAL_HSP_TRGIN_ID(trgin_id)  \
  ((trgin_id == 0U)?HAL_HSP_TRGIN_0:    \
   (trgin_id == 1U)?HAL_HSP_TRGIN_1:    \
   (trgin_id == 2U)?HAL_HSP_TRGIN_2:    \
   (trgin_id == 3U)?HAL_HSP_TRGIN_3:    \
   (trgin_id == 4U)?HAL_HSP_TRGIN_4:    \
   (trgin_id == 5U)?HAL_HSP_TRGIN_5:    \
   (trgin_id == 6U)?HAL_HSP_TRGIN_6:    \
   (trgin_id == 7U)?HAL_HSP_TRGIN_7:    \
   (trgin_id == 8U)?HAL_HSP_TRGIN_8:    \
   (trgin_id == 9U)?HAL_HSP_TRGIN_9:0xFFFFU)

#define HSP_EVENT_GET_TRGINS_MASK(evt_id)                                                                  \
  ((((evt_id) == 1U) || ((evt_id) == 6U)  || ((evt_id) == 11U) || ((evt_id) == 16U) || ((evt_id) == 21U))? \
   (uint32_t)((1UL << 3U) | (1UL << 8U)):                                                                  \
   (((evt_id) == 2U) || ((evt_id) == 7U)  || ((evt_id) == 12U) || ((evt_id) == 17U) || ((evt_id) == 22U))? \
   (uint32_t)((1UL << 4U) | (1UL << 9U)):                                                                  \
   (((evt_id) == 3U) || ((evt_id) == 8U)  || ((evt_id) == 13U) || ((evt_id) == 18U))?                      \
   (uint32_t)((1UL << 0U) | (1UL << 5U)):                                                                  \
   (((evt_id) == 4U) || ((evt_id) == 9U)  || ((evt_id) == 14U) || ((evt_id) == 19U))?                      \
   (uint32_t)((1UL << 1U) | (1UL << 6U)):                                                                  \
   (((evt_id) == 5U) || ((evt_id) == 10U) || ((evt_id) == 15U))?(uint32_t)((1UL << 2U) | (1UL << 7U)):0xFFFFU)

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup HSP_HW_IF_Private_Macros HSP_HW_IF Private Macros
  * @{
  */
#define POSITION_FIRST_ZERO(VAL) (__CLZ(__RBIT(~(VAL))))
#if defined(HAL_HSP_SMARTCLOCKING_HSPDMA)
#define HSP_HW_IF_GET_CLOCK(clock_id)                                             \
  (((clock_id) == HSP_HW_IF_SMARTCLOCKING_CTRL)?HAL_HSP_SMARTCLOCKING_CTRL:       \
   ((clock_id) == HSP_HW_IF_SMARTCLOCKING_SPE)?HAL_HSP_SMARTCLOCKING_SPE:         \
   ((clock_id) == HSP_HW_IF_SMARTCLOCKING_MMC)?HAL_HSP_SMARTCLOCKING_MMC:         \
   ((clock_id) == HSP_HW_IF_SMARTCLOCKING_HSPDMA)?HAL_HSP_SMARTCLOCKING_HSPDMA:0U)
#else /* HAL_HSP_SMARTCLOCKING_HSPDMA */
#define HSP_HW_IF_GET_CLOCK(clock_id)                                        \
  (((clock_id) == HSP_HW_IF_SMARTCLOCKING_CTRL)?HAL_HSP_SMARTCLOCKING_CTRL:  \
   ((clock_id) == HSP_HW_IF_SMARTCLOCKING_SPE)?HAL_HSP_SMARTCLOCKING_SPE:    \
   ((clock_id) == HSP_HW_IF_SMARTCLOCKING_MMC)?HAL_HSP_SMARTCLOCKING_MMC:0U)
#endif /* HAL_HSP_SMARTCLOCKING_HSPDMA */

/**
  * @brief Get the MW HSP Engine ID from the handle.
  * @param hhsp Handle of the HSP.
  * @param mw_id  MW HSP Engine ID to be returned.
  */
#define GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id)                                   \
  do                                                                             \
  {                                                                              \
    for (uint8_t index = 0; index < (uint8_t)HSP_ENGINE_INSTANCE_COUNT; index++) \
    {                                                                            \
      if (hsp_hw_if_ctx[index].hal_hsp == (hhsp))                                \
      {                                                                          \
        (mw_id) = index;                                                         \
        break;                                                                   \
      }                                                                          \
    }                                                                            \
  } while(0);

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/** @defgroup HSP_HW_IF_Private_Types HSP_HW_IF Private Types
  * @{
  */
typedef struct
{
  uint32_t *p_data;
  uint32_t data_counter;
} hsp_hw_if_buffer_t;

typedef struct
{
  hal_hsp_handle_t *hal_hsp;

  void (* ErrorCallback)(uint32_t mw_id, uint32_t fw_error_code, uint32_t spe_error_inf);

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
  void (* ProcListEventsCallback)(uint32_t mw_id, uint32_t proclist_events);
#endif  /* USE_HSP_MODULES_PROCLIST */
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
  void (* StreamBufferWriteCpltCallback)(uint32_t mw_id, uint32_t stream_buffer_ids);
  void (* StreamBufferReadCpltCallback)(uint32_t mw_id, uint32_t stream_buffer_ids);

  hsp_hw_if_buffer_t stream_buffer_input[4U];
  hsp_hw_if_buffer_t stream_buffer_output[4U];
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

  uint32_t trgin_instances; /* bitfields to identify which TRGIN are free or configured */

  void (* TCU_TaskStartedCallback)(uint32_t mw_id, uint32_t tcu_ids);
  void (* TCU_TaskEndedCallback)(uint32_t mw_id, uint32_t tcu_ids);
} hsp_hw_if_ctx_t;

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup HSP_ENGINE_HW_IF_Private_Variables HSP_ENGINE_HW_IF Private Variables
  * @{
  */
hsp_hw_if_ctx_t hsp_hw_if_ctx[HSP_ENGINE_INSTANCE_COUNT];

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup HSP_ENGINE_HW_IF_Private_Functions HSP_ENGINE_HW_IF Private Functions
  * @{
  */
/* Overridden HAL Callbacks --------------------------------------------------*/
static void HSP_HW_IF_ErrorCallback(hal_hsp_handle_t *hhsp, uint32_t fw_error_code, uint32_t spe_error_inf);
#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
static void HSP_HW_IF_PROCLIST_EventsCallback(hal_hsp_handle_t *hhsp, uint32_t events_mask);
#endif  /* USE_HSP_MODULES_PROCLIST */
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
static void HSP_HW_IF_STREAM_BufferEmptyCallback(hal_hsp_handle_t *hhsp, uint32_t buffers_mask);
static void HSP_HW_IF_STREAM_BufferFullCallback(hal_hsp_handle_t *hhsp, uint32_t buffers_mask);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */
static void HSP_HW_IF_TCU_TaskStartedCallback(hal_hsp_handle_t *hhsp, uint32_t tcu_ids);
static void HSP_HW_IF_TCU_TaskEndedCallback(hal_hsp_handle_t *hhsp, uint32_t tcu_ids);

static uint32_t HSP_HW_IF_Retrieve_TRGIN_HAL_Event_Source_Enum(uint32_t event_id, uint32_t trgin_id,
                                                               hal_hsp_event_source_t *enum_value);
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
static uint32_t HSP_HW_IF_Retrieve_STREAM_HAL_Event_Source_Enum(uint32_t event_id, hal_hsp_event_source_t *enum_value);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_HW_IF_Exported_Functions
  * @{
  */
/**
  * @brief  Deinit the interface
  * @param  hdriver   HSP Driver handle.
  * @param  mw_id     Id for Interface Context
  */
void HSP_HW_IF_Init(void *hdriver, uint32_t mw_id)
{
  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  (void)HAL_HSP_RegisterErrorCallback(hhsp, HSP_HW_IF_ErrorCallback);
#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
  (void)HAL_HSP_RegisterFWEVT_ProcessingEventCallback(hhsp, HSP_HW_IF_PROCLIST_EventsCallback);
#endif /* USE_HSP_MODULES_PROCLIST */
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
  (void)HAL_HSP_RegisterSTREAM_BufferEmptyCallback(hhsp, HSP_HW_IF_STREAM_BufferEmptyCallback);
  (void)HAL_HSP_RegisterSTREAM_BufferFullCallback(hhsp, HSP_HW_IF_STREAM_BufferFullCallback);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

  (void)HAL_HSP_RegisterTCU_TaskStartedCallback(hhsp, HSP_HW_IF_TCU_TaskStartedCallback);
  (void)HAL_HSP_RegisterTCU_TaskEndedCallback(hhsp, HSP_HW_IF_TCU_TaskEndedCallback);

  /* Link the hdriver to the MW Ctx */
  hsp_hw_if_ctx[mw_id].hal_hsp = hhsp;
}

/**
  * @brief  Deinit the interface
  * @param  hdriver   HSP Driver handle.
  */
void HSP_HW_IF_DeInit(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

}

/**
  * @brief  Load a Firmware OpCode & data respectively in CRAM & DRAM
  * @param  hdriver    HSP Driver handle.
  * @param  p_fw_cram  Pointer to CRAM Firmware structure
  * @param  p_fw_dram  Pointer to DRAM Firmware structure
  * @retval HSP_IF_OK     Load complete successfully
  * @retval HSP_IF_ERROR  Load failed
  */
hsp_if_status_t HSP_HW_IF_LoadFW(void *hdriver, const void *p_fw_cram, const void *p_fw_dram)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_fw_cram != NULL);
  HSP_ASSERT_DBG_PARAM(p_fw_dram != NULL);

  hsp_fw_descriptor_t *p_cram = (hsp_fw_descriptor_t *)p_fw_cram;
  hsp_fw_descriptor_t *p_dram = (hsp_fw_descriptor_t *)p_fw_dram;

  if (HAL_HSP_LoadFirmware((hal_hsp_handle_t *)(hdriver),
                           p_cram->p_binary, p_cram->size_byte,
                           p_dram->p_binary, p_dram->size_byte) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief  Load a Plugin in CRAM.
  * @param  hdriver    HSP Driver handle.
  * @param  p_bin      Pointer to the Plugin binary.
  * @param  size_byte  Size in byte of Plugin binary.
  * @retval HSP_IF_OK     Load complete successfully
  * @retval HSP_IF_ERROR  Load failed
  */
hsp_if_status_t HSP_HW_IF_LoadPlugin(void *hdriver, const uint8_t *p_bin, uint32_t size_byte)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_bin != NULL);

  if (HAL_HSP_LoadPlugin((hal_hsp_handle_t *)(hdriver), p_bin, size_byte) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Boot the HSP.
  * @param  hdriver          HSP Driver handle.
  * @param enableCycleCount  Enable cycle count on HSP side
  * @retval HSP_IF_OK     Boot complete successfully
  * @retval HSP_IF_ERROR  Boot failed
  */
hsp_if_status_t HSP_HW_IF_Boot(void *hdriver, uint32_t enableCycleCount)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  hal_hsp_boot_config_t boot_cfg;
  boot_cfg.boot_cmd_id = HSP_CMD_FW_START;
  boot_cfg.boot_success_code = HSP_BSTAT_BOOTOK;
  boot_cfg.perf_mon = (hal_hsp_perf_monitor_state_t)enableCycleCount;
  boot_cfg.perf_counter_offset = HSP_DRAM_CC_ADDR;

  if (HAL_HSP_Boot(hhsp, &boot_cfg, HSP_BOOT_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
#if 0
  hal_hsp_fw_version_t rom_version;
  if (HAL_HSP_GetCROMVersion(&(h_hw_hsp_if[hsp_id].hhsp), &rom_version) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  if (rom_version.major != HSP_FW_DEF_ROM_VERSION_MAJOR)
  {
    return HSP_IF_ERROR;
  }

  if (rom_version.minor != HSP_FW_DEF_ROM_VERSION_MINOR)
  {
    return HSP_IF_ERROR;
  }

  hal_hsp_fw_version_t ram_version;
  if (HAL_HSP_GetCRAMVersion(&(h_hw_hsp_if[hsp_id].hhsp), &ram_version) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  if (ram_version.major != HSP_FW_DEF_RAM_VERSION_MAJOR)
  {
    return HSP_IF_ERROR;
  }

  if (ram_version.minor != HSP_FW_DEF_RAM_VERSION_MINOR)
  {
    return HSP_IF_ERROR;
  }
#endif /* if 0 */
  return HSP_IF_OK;
}

/* API to get the value of Performance counter */
/**
  * @brief Get the HSP Performance counter.
  * @param  hdriver   HSP Driver handle.
  * @note  The counter must be enabled during the HSP Boot.
  * @return counter value
  */
uint32_t HSP_HW_IF_GetPerformanceCounter(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  return HAL_HSP_GetPerformanceCycleCount(hhsp);
}

/* API to manage the SmartClocking -------------------------------------------*/
/**
  * @brief Enable SmartClocking capabilities of HSP blocks.
  * @param  hdriver   HSP Driver handle.
  * @param clock_id   Clock ID
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_EnableSmartClocking(void *hdriver, uint32_t clock_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  uint32_t clock = HSP_HW_IF_GET_CLOCK(clock_id);
  if (clock == 0U)
  {
    return HSP_IF_ERROR;
  }

  if (HAL_HSP_SMARTCLOCKING_Enable(hhsp, clock) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

/**
  * @brief Disable SmartClocking capabilities of HSP blocks.
  * @param  hdriver   HSP Driver handle.
  * @param  clock_id  Clock ID
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_DisableSmartClocking(void *hdriver, uint32_t clock_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  uint32_t clock = HSP_HW_IF_GET_CLOCK(clock_id);
  if (clock == 0U)
  {
    return HSP_IF_ERROR;
  }

  if (HAL_HSP_SMARTCLOCKING_Disable(hhsp, clock) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

/* API to configure STI AP Port  ---------------------------------------------*/
hsp_if_status_t HSP_HW_IF_STI_SetPortAddress(void *hdriver, uint32_t port_id, uint32_t block_address)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
#if (USE_HSP_STI_AP_PORT)
  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_STI_SetPortAddress(hhsp, (hal_hsp_sti_ap_port_t)port_id, block_address) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
#else /* USE_HSP_STI_AP_PORT */
  return HSP_IF_ERROR;
#endif /* USE_HSP_STI_AP_PORT */
}

/* API to manage OUTPUT ------------------------------------------------------*/
/**
  * @brief Configure the source of a TRGO output.
  * @param  hdriver   HSP Driver handle.
  * @param  trgo_id   TRGO output ID.
  * @param  source    Source value.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_OUTPUT_SetConfig(void *hdriver, uint32_t trgo_id, uint32_t source)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_OUTPUT_SetConfigTrigger(hhsp, (hal_hsp_output_trigger_t)trgo_id,
                                      (hal_hsp_output_trigger_source_t)source) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

/**
  * @brief Enable GPO & TRGO outputs.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_OUTPUT_Enable(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_OUTPUT_Enable(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

/**
  * @brief Enable GPO & TRGO outputs.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_OUTPUT_Disable(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_OUTPUT_Disable(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Return the enabling status of HSP Outputs
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_ACTIVATION_DISABLED  HSP Outputs are disabled
  * @retval HSP_IF_ACTIVATION_ENABLED   HSP Outputs are enabled
  */
hsp_if_activation_status_t  HSP_HW_IF_OUTPUT_IsEnabled(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_OUTPUT_IsEnabled(hhsp) == HAL_HSP_OUTPUT_DISABLED)
  {
    return HSP_IF_ACTIVATION_DISABLED;
  }
  return HSP_IF_ACTIVATION_ENABLED;
}

/* API to manage Direct Command ----------------------------------------------*/
/**
  * @brief Send a command with MsgBox Interface.
  * @param  hdriver     HSP Driver handle.
  * @param  command_id  Command ID.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SendCommand(void *hdriver, uint32_t command_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_MSGBOX_SendCommand(hhsp, command_id, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Send a command with MsgBox Interface.
  * @param  hdriver   HSP Driver handle.
  * @param  param_id  Parameter ID.
  * @param  value     Value to set.
  */
void HSP_HW_IF_WriteParameter(void *hdriver, uint32_t param_id, uint32_t value)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  (void) HAL_HSP_WriteParameter(hhsp, (hal_hsp_param_t)param_id, value);
}

/**
  * @brief Clear the Firmware Error Code.
  * @param  hdriver   HSP Driver handle.
  * @return  Firmware Error Code.
  */
uint32_t HSP_HW_IF_ClearFWERR(void *hdriver)
{

  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  HAL_HSP_DIRECT_WriteCommand(hhsp, HSP_DIRECT_CMD_CLR_FWERR);
  while (HAL_HSP_DIRECT_IsCommandReady(hhsp) != HAL_HSP_DIRECT_CMD_READY);

  return HAL_HSP_ReadParameter(hhsp, HAL_HSP_PARAM_0);
}

/* API to manage Processing List ---------------------------------------------*/
/**
  * @brief  Start the record of a Processing List
  * @param  hdriver  HSP Driver handle.
  * @param  pl_id    Processing List ID
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_StartRecord(void *hdriver, uint32_t pl_id)
{

  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TASK_Run(hhsp, HAL_HSP_TASK_SUPERVISOR, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  HAL_HSP_WriteParameter(hhsp, HAL_HSP_PARAM_0, pl_id);

  if (HAL_HSP_MSGBOX_SendCommand(hhsp, HSP_CMD_PRO_CFG_START, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

/**
  * @brief  Stop the record of the Processing List
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_StopRecord(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_MSGBOX_SendCommand(hhsp, HSP_CMD_PRO_CFG_END, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief  Reset all Processing List
  * @retval HSP_IF_OK success
  *         HSP_IF_ERROR failure
  */
hsp_if_status_t HSP_HW_IF_SEQ_ResetPL(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TASK_Run(hhsp, HAL_HSP_TASK_SUPERVISOR, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  HAL_HSP_WriteParameter(hhsp, HAL_HSP_PARAM_1, ENABLE_HSP_PERFORMANCE_MONITOR);
  if (HAL_HSP_MSGBOX_SendCommand(hhsp, HSP_CMD_PL_RESET, HSP_TIMEOUT_MS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}

hsp_if_status_t HSP_HW_IF_SEQ_RunTask(void *hdriver, uint32_t task_id, uint32_t timeout_ms)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TASK_Run(hhsp, (hal_hsp_task_t)task_id, timeout_ms) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/* API to manage Trigger & Event configuration -------------------------------*/
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/**
  * @brief Configure an event with a STREAM Buffer as source.
  * @param  hdriver           HSP Driver handle.
  * @param  event_id          Event ID.
  * @param  stream_buffer_id  STREAM Buffer Identifier.
  * @param  direction         STREAM Buffer Direction (0:RX, 1:TX)
  * @param  buffer_sync       Synchronization with the adjacent STREAM Buffer
  * @param  tcu_sync          Task Comparator Synchronization
  * @note  The enable of STREAM is not perform at the end of the configuration otherwise it will be impossible
  *        to configure the next Stream Buffer.
  *        The application must do it after all STREAM Buffer configurations.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_StreamBuffer(void *hdriver, uint32_t event_id,
                                                           uint32_t stream_buffer_id,
                                                           uint32_t direction, uint32_t buffer_sync,
                                                           uint32_t tcu_sync)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Config the stream */
  hal_hsp_stream_buffer_config_t stream_cfg;
  stream_cfg.direction = (hal_hsp_stream_buffer_direction_t)direction;

  if (buffer_sync == 0U)
  {
    stream_cfg.sync_state = HAL_HSP_STREAM_BUFFER_SYNC_DISABLE;
  }
  else
  {
    stream_cfg.sync_state = HAL_HSP_STREAM_BUFFER_SYNC_ENABLE;
  }

  if (HAL_HSP_STREAM_SetConfig(hhsp, (hal_hsp_stream_buffer_t)(stream_buffer_id), &stream_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Configure the Event */
  hal_hsp_event_config_t evt_cfg;
  if (HSP_HW_IF_Retrieve_STREAM_HAL_Event_Source_Enum(event_id, &evt_cfg.source) != 0U)
  {
    return HSP_IF_ERROR;
  }

  if (tcu_sync == HSP_HW_IF_SEQ_EVENT_SYNC_DISABLED)
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_DISABLE;
    evt_cfg.sync.tcu = HAL_HSP_TASK_COMPARATOR_0;
  }
  else
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_ENABLE;
    evt_cfg.sync.tcu = (hal_hsp_task_comparator_t)tcu_sync;
  }

  if (HAL_HSP_EVENT_SetConfig(hhsp, &evt_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Enable the Integer To Float conversion for all STREAM Buffers.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_EnableIntToFloat(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_STREAM_EnableIntToFloat(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable the Integer To Float conversion for all STREAM Buffers.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_DisableIntToFloat(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_STREAM_DisableIntToFloat(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Enable all STREAM Buffers.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Enable(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  /* Build STREAM Buffer IT to enable */
  uint32_t evt_its = 0U;
  uint32_t err_its = 0U;
  for (uint32_t i = 0U; i < 4U; i++)
  {
    hsp_hw_if_buffer_t *p_hw_if_stream_buffer = &(hsp_hw_if_ctx[mw_id].stream_buffer_input[i]);
    if (p_hw_if_stream_buffer->data_counter > 0U)
    {
      evt_its |= (HAL_HSP_IT_EVT_STREAM0_READY << i);
      err_its |= (HAL_HSP_IT_ERR_STREAM0_OVR << i);
    }
  }

  if (HAL_HSP_STREAM_Start(hhsp, evt_its, err_its) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable all STREAM Buffers.
  * @param  hdriver   HSP Driver handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Disable(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_STREAM_Stop(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief  Get a STREAM Buffer address.
  * @param  hdriver  Pointer to a driver.
  * @param  stream_buffer_id  Stream buffer id.
  * @retval Buffer Address
  */
volatile uint32_t *HSP_HW_IF_SEQ_STREAM_GetBufferAddress(void *hdriver, uint32_t stream_buffer_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  return HAL_HSP_STREAM_GetBufferAddress(hhsp, (hal_hsp_stream_buffer_t)stream_buffer_id);
}

/**
  * @brief  Start the write/read of data to/from a STREAM Buffer in IT mode.
  *         The Write or Read is depend of the Stream Buffer Direction.
  * @param  hdriver  Pointer to a driver.
  * @param  stream_buffer_id  Stream buffer id.
  * @param  p_buffer Data buffer
  * @param  buffer_size Number of data
  */
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Buffer_Start(void *hdriver, uint32_t stream_buffer_id, const int32_t *p_buffer,
                                                  uint32_t buffer_size)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_buffer != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  /* Store the data buffer in the Interface Context */
  hsp_hw_if_buffer_t *p_hw_if_stream_buffer = &(hsp_hw_if_ctx[mw_id].stream_buffer_input[stream_buffer_id]);
  p_hw_if_stream_buffer->p_data = (uint32_t *)p_buffer;
  p_hw_if_stream_buffer->data_counter = buffer_size;

  return HSP_IF_OK;
}
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @brief Configure an event with a TRGIN trigger as source.
  * @param  hdriver         HSP Driver handle.
  * @param  event_id        Event ID.
  * @param  trigger_source  TRGIN Source
  * @param  polarity        Rising or Falling edge
  * @param  event_sync      Enable Synchronisation with End Of Task.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_Trigger(void *hdriver, uint32_t event_id,
                                                      uint32_t trigger_source, uint32_t polarity,
                                                      uint32_t event_sync)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  uint32_t mw_id;

  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  /* Get the TRGIN used status */
  uint32_t trgin_instances = hsp_hw_if_ctx[mw_id].trgin_instances;

  uint32_t evt_trgins_msk = HSP_EVENT_GET_TRGINS_MASK(event_id);

  /* Check if one is available */
  uint32_t trgin_available = (evt_trgins_msk & ~trgin_instances);
  if (trgin_available == 0U)
  {
    return HSP_IF_ERROR;
  }

  /* Find the first TRGIN available */
  trgin_available = POSITION_FIRST_ZERO(~trgin_available);

  /* Book the TRGIN instance*/
  hsp_hw_if_ctx[mw_id].trgin_instances = (trgin_instances | (1UL << trgin_available));

  /* Get the HAL HSP TRGIN define value */
  uint32_t hal_trgin_id = GET_HAL_HSP_TRGIN_ID(trgin_available);

  /* Config the TRGINx */
  hal_hsp_trgin_config_t trgin_cfg;
  trgin_cfg.polarity = (hal_hsp_trgin_polarity_t)polarity;
  trgin_cfg.source = (hal_hsp_trgin_source_t)trigger_source;

  if (HAL_HSP_TRGIN_SetConfig(hhsp, hal_trgin_id, &trgin_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Configure the Event */
  hal_hsp_event_config_t evt_cfg;
  if (HSP_HW_IF_Retrieve_TRGIN_HAL_Event_Source_Enum(event_id, trgin_available, &evt_cfg.source) != 0U)
  {
    return HSP_IF_ERROR;
  }

  if (event_sync == HSP_HW_IF_SEQ_EVENT_SYNC_DISABLED)
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_DISABLE;
    evt_cfg.sync.tcu = HAL_HSP_TASK_COMPARATOR_0;
  }
  else
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_ENABLE;
    evt_cfg.sync.tcu = (hal_hsp_task_comparator_t)event_sync;
  }

  if (HAL_HSP_EVENT_SetConfig(hhsp, &evt_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Enable the TRGINx instance */
  if (HAL_HSP_TRGIN_Enable(hhsp, hal_trgin_id) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Configure an event with the CSEG interface as source.
  * @param  hdriver   HSP Driver handle.
  * @param  event_id  Event ID.
  * @param  tcu_sync  Task Comparator Synchronization
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_CSEG(void *hdriver, uint32_t event_id, uint32_t tcu_sync)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  hal_hsp_event_config_t evt_cfg;

  evt_cfg.source = HAL_HSP_EVENT_SOURCE_GET_ENUM_FOR_CSEG(event_id);

  if (tcu_sync == HSP_HW_IF_SEQ_EVENT_SYNC_DISABLED)
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_DISABLE;
    evt_cfg.sync.tcu = HAL_HSP_TASK_COMPARATOR_0;
  }
  else
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_ENABLE;
    evt_cfg.sync.tcu = (hal_hsp_task_comparator_t)tcu_sync;
  }

  if (HAL_HSP_EVENT_SetConfig(hhsp, &evt_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Configure an event with the HSEG interface as source.
  * @param  hdriver   HSP Driver handle.
  * @param  event_id  Event ID.
  * @param  tcu_sync  Task Comparator Synchronization
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_SPE(void *hdriver, uint32_t event_id, uint32_t tcu_sync)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  hal_hsp_event_config_t evt_cfg;

  evt_cfg.source = HAL_HSP_EVENT_SOURCE_GET_ENUM_FOR_HSEG(event_id);

  if (tcu_sync == HSP_HW_IF_SEQ_EVENT_SYNC_DISABLED)
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_DISABLE;
    evt_cfg.sync.tcu = HAL_HSP_TASK_COMPARATOR_0;
  }
  else
  {
    evt_cfg.sync.state = HAL_HSP_EVENT_SYNC_ENABLE;
    evt_cfg.sync.tcu = (hal_hsp_task_comparator_t)tcu_sync;
  }

  if (HAL_HSP_EVENT_SetConfig(hhsp, &evt_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Enable HSP events attached to Processing Lists.
  * @param  hdriver   HSP Driver handle.
  * @param  events_mask  Mask of Events to enable.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Enable(void *hdriver, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_EVENT_Enable(hhsp, events_mask) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable HSP events attached to Processing Lists.
  * @param  hdriver   HSP Driver handle.
  * @param  events_mask  Mask of Events to disable.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Disable(void *hdriver, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_EVENT_Disable(hhsp, events_mask) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Request execution of Processing Lists attached to CSEG interface.
  * @param  hdriver   HSP Driver handle.
  * @param  events_mask  Mask of Events to trig.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Trig_CSEG(void *hdriver, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_EVENT_RequestSWTrigger(hhsp, events_mask) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Poll until the Request execution is taken into account.
  * @param  hdriver   HSP Driver handle.
  * @param  events_mask  Mask of Events to poll.
  * @param  timeout_ms   Timeout in millisecond
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_PollForPending(void *hdriver, uint32_t events_mask, uint32_t timeout_ms)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  uint32_t event_status = 0U;

  if (HAL_HSP_EVENT_PollForPending(hhsp, HAL_HSP_POLLING_FOR_ALL_EVENT,
                                   events_mask, timeout_ms, &event_status) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/* API to BRAM Access control ------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_BRAM_EnableConflictAccessCounter(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_BRAM_EnableConflictAccessCounter(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

hsp_if_status_t HSP_HW_IF_BRAM_DisableConflictAccessCounter(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_BRAM_DisableConflictAccessCounter(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

uint32_t HSP_HW_IF_BRAM_GetConflictAccessCounter(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  return HAL_HSP_BRAM_GetConflictAccessCounter(hhsp);
}

hsp_if_status_t HSP_HW_IF_BRAM_SetBandwidthArbitration(void *hdriver, uint32_t mode)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_BRAM_SetBandwidthArbitration(hhsp, (hal_hsp_bram_arbitration_t) mode) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

uint32_t HSP_HW_IF_BRAM_GetBandwidthArbitration(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  return (uint32_t)HAL_HSP_BRAM_GetBandwidthArbitration(hhsp);
}

/* API for secure Access -----------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_Lock(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_Lock(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
hsp_if_status_t HSP_HW_IF_SEQ_EnableProclistEventsIT(void *hdriver, uint32_t interrupts)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_PROCLIST_EnableIT(hhsp, interrupts) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

hsp_if_status_t HSP_HW_IF_SEQ_DisableProclistEventsIT(void *hdriver, uint32_t interrupts)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_PROCLIST_DisableIT(hhsp, interrupts) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}
#endif /* USE_HSP_MODULES_PROCLIST */

/* API to manage Task Comparator Unit ----------------------------------------*/

/**
  * @brief Configure and enable a Task Comparator.
  * @param  hdriver  Pointer to a HSP handle.
  * @param  tcu_id   ID of the Task Comparator Unit.
  * @param  task_id  Task ID to monitor.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_EnableTaskComparator(void *hdriver, uint32_t tcu_id, uint32_t task_id)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TCU_StartComparator(hhsp,
                                  (hal_hsp_task_comparator_t)tcu_id, (hal_hsp_task_t)task_id, 0U) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable a Task Comparator.
  * @param  hdriver  Pointer to a HSP handle.
  * @param  tcu_id   ID of the Task Comparator Unit.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_DisableTaskComparator(void *hdriver, uint32_t tcu_id)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TCU_StopComparator(hhsp, (hal_hsp_task_comparator_t)tcu_id) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Configure and enable a Task Comparator.
  * @param  hdriver  Pointer to a HSP handle.
  * @param  tcu_id   ID of the Task Comparator Unit.
  * @param  task_id  Task ID to monitor.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_EnableTaskComparator_IT(void *hdriver, uint32_t tcu_id, uint32_t task_id)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  uint32_t its = ((HAL_HSP_IT_EVT_TCU0_TASK_STARTED | HAL_HSP_IT_EVT_TCU0_TASK_ENDED) << tcu_id);

  if (HAL_HSP_TCU_StartComparator(hhsp,
                                  (hal_hsp_task_comparator_t)tcu_id, (hal_hsp_task_t)task_id,
                                  its) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable a Task Comparator.
  * @param  hdriver  Pointer to a HSP handle.
  * @param  tcu_id   ID of the Task Comparator Unit.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_DisableTaskComparator_IT(void *hdriver, uint32_t tcu_id)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TCU_StopComparator(hhsp, (hal_hsp_task_comparator_t)tcu_id) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/* API for TimeStamp Capture -------------------------------------------------*/
/**
  * @brief Enable the TimeStamp Capture in xxx mode.
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_TSC_EnableSPEBreak(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Set FRCNTEN to 0 */
  /* Set CAPMOD to 0 */
  hal_hsp_tsc_config_t tsc_cfg;
  tsc_cfg.prescaler = HAL_HSP_TSC_PRESCALER_BYPASSED;
  tsc_cfg.counter = HAL_HSP_TSC_FREE_COUNTER_DISABLE;
  tsc_cfg.filter = HAL_HSP_TSC_TASK_FILTER_NONE;
  if (HAL_HSP_TSC_SetConfig(hhsp, &tsc_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Enable all sources of BKITF */

  /* Set CAPMOD to 1 */
  if (HAL_HSP_TSC_Enable(hhsp, HAL_HSP_TSC_CAPTURE_MODE_SPE_BREAKS) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Enable the TimeStamp Capture in Task mode.
  * @param  hdriver    Pointer to a HSP handle.
  * @param  prescaler  Prescaler for Free Counter (if enabled)
  * @param  task_id    ID of the Task Comparator Unit.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_TSC_EnablePFCT(void *hdriver, uint32_t prescaler, uint32_t task_id)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);
  hal_hsp_tsc_config_t tsc_cfg;

  if (task_id == 0U/* HSP_TSC_TASK_NOT_SPECIFIED*/)
  {
    tsc_cfg.filter = HAL_HSP_TSC_TASK_FILTER_NONE;
  }
  else
  {
    tsc_cfg.filter = HAL_HSP_TSC_TASK_FILTER_TSKCMP0;
  }

  tsc_cfg.prescaler = (hal_hsp_tsc_prescaler_t)prescaler;
  tsc_cfg.counter = HAL_HSP_TSC_FREE_COUNTER_ENABLE;
  if (HAL_HSP_TSC_SetConfig(hhsp, &tsc_cfg) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Set CAPMOD to 3 */
  if (HAL_HSP_TSC_Enable(hhsp, HAL_HSP_TSC_CAPTURE_MODE_PFCT_NBR_CHANGES) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Disable the TimeStamp Capture.
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_TSC_Disable(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_TSC_Disable(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Retrieve the HSP Firmware Error Code.
  * @param  hdriver  Pointer to a HSP handle.
  * @return FW Error Code
  */
uint32_t HSP_HW_IF_GetFirmwareError(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  return HAL_HSP_GetFirmwareError(hhsp);
}

/**
  * @brief Wait semaphore and release semaphore.
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  */
hsp_if_status_t HSP_HW_IF_WaitAndReleaseSemaphore(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Wait direct command done */
  while (HAL_HSP_MSGBOX_IsMsgAvailable(hhsp) == HAL_HSP_MSGBOX_SEMAPHORE_NO_MSG);

  /* Clear Semahore */
  HAL_HSP_MSGBOX_ReleaseSemaphore(hhsp);

  return HSP_IF_OK;
}

/**
  * @brief Wait semaphore and release semaphore.
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  */
hsp_if_status_t HSP_HW_IF_WaitSemaphore(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Wait direct command done */
  while (HAL_HSP_MSGBOX_IsMsgAvailable(hhsp) == HAL_HSP_MSGBOX_SEMAPHORE_NO_MSG);

  return HSP_IF_OK;
}

/**
  * @brief Release Semaphore.
  * @param  hdriver  Pointer to a HSP handle.
  */
void HSP_HW_IF_ReleaseSemaphore(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Clear Semaphore */
  HAL_HSP_MSGBOX_ReleaseSemaphore(hhsp);
}

/* API for CNN ---------------------------------------------------------------*/
/**
  * @brief Prepare the HSP to execute the CNN.
  *   The function disables all HSP Event & ITs to avoid any interruption during the CNN processing.
  * @param  hdriver  Pointer to a HSP handle.
  * @param  cmd_id   CNN Command ID.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_CNN_Begin(void *hdriver, uint32_t cmd_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Send the CNN Direct Command */
  (void) HAL_HSP_DIRECT_WriteCommand(hhsp, cmd_id);

  if (HAL_HSP_CNN_EnterSafeMode(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/**
  * @brief Run the computing of a CNN.
  * @param  hdriver     Pointer to a HSP handle.
  * @param  cnn_cmd_id  CNN Command ID.
  * @retval HSP_IF_OK   Action complete successfully
  */
hsp_if_status_t HSP_HW_IF_CNN_StartOfProcess(void *hdriver, uint32_t cnn_cmd_id)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  (void)HAL_HSP_CNN_Run(hhsp, cnn_cmd_id);

  return HSP_IF_OK;
}

/**
  * @brief Wait End of CNN .
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  */
hsp_if_status_t HSP_HW_IF_CNN_WaitEndOfProcess(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  /* Wait direct command done */
  while (HAL_HSP_MSGBOX_IsMsgAvailable(hhsp) == HAL_HSP_MSGBOX_SEMAPHORE_NO_MSG);

  /* Clear Semahore */
  HAL_HSP_MSGBOX_ReleaseSemaphore(hhsp);

  return HSP_IF_OK;
}

/**
  * @brief End the process of a CNN.
  * @param  hdriver  Pointer to a HSP handle.
  * @retval HSP_IF_OK     Action complete successfully
  * @retval HSP_IF_ERROR  Action failed
  */
hsp_if_status_t HSP_HW_IF_CNN_End(void *hdriver)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);

  hal_hsp_handle_t *hhsp = (hal_hsp_handle_t *)(hdriver);

  if (HAL_HSP_CNN_ExitSafeMode(hhsp) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  return HSP_IF_OK;
}

/* API Register MW Callbacks ------------------------------------------------ */
/**
  * @brief  Register a User HSP callback for any Error Event.
  * @param  hdriver        Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK      Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterErrorCallback(void *hdriver, p_hsp_engine_two_params_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].ErrorCallback = p_callback;

  return HSP_IF_OK;
}

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
/**
  * @brief  Register a User HSP callback for any Processing List Event.
  * @param  hdriver     Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK       Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterProcListEventsCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].ProcListEventsCallback = p_callback;

  return HSP_IF_OK;
}
#endif /* USE_HSP_MODULES_PROCLIST */

#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/**
  * @brief  Register a User HSP callback for any Stream Buffer Write Complete Event.
  * @param  hdriver        Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK      Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterStreamBufferWriteCpltCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].StreamBufferWriteCpltCallback = p_callback;

  return HSP_IF_OK;
}

/**
  * @brief  Register a User HSP callback for any Stream Buffer Read Complete Event.
  * @param  hdriver     Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK      Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterStreamBufferReadCpltCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].StreamBufferReadCpltCallback = p_callback;

  return HSP_IF_OK;
}
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @brief  Register a User HSP callback for Task Comparator Started IT.
  * @param  hdriver     Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK      Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterTCU_TaskStartedCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].TCU_TaskStartedCallback = p_callback;

  return HSP_IF_OK;
}

/**
  * @brief  Register a User HSP callback for Task Comparator Ended IT.
  * @param  hdriver     Pointer to a HSP handle
  * @param  p_callback  Pointer to the callback function.
  * @retval HSP_IF_OK      Registering completed successfully.
  */
hsp_if_status_t HSP_HW_IF_RegisterTCU_TaskEndedCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback)
{
  HSP_ASSERT_DBG_PARAM(hdriver != NULL);
  HSP_ASSERT_DBG_PARAM(p_callback != NULL);

  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hdriver, mw_id);

  /* Store the callback function within handle */
  hsp_hw_if_ctx[mw_id].TCU_TaskEndedCallback = p_callback;

  return HSP_IF_OK;
}

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup HSP_ENGINE_HW_IF_Private_Functions
  * @{
  */
/**
  * @brief  Error callback.
  * @param  fw_error_code  FW Error Code value
  * @param  spe_error_inf  SPE Error information value
  * @param  hhsp Pointer to a @ref hal_hsp_handle_t.
  */
static void HSP_HW_IF_ErrorCallback(hal_hsp_handle_t *hhsp, uint32_t fw_error_code, uint32_t spe_error_inf)
{
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  hsp_hw_if_ctx[mw_id].ErrorCallback(mw_id, fw_error_code, spe_error_inf);
}

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
/**
  * @brief  Proccesing List Events callback.
  * @param  hhsp Pointer to a @ref hal_hsp_handle_t.
  */
static void HSP_HW_IF_PROCLIST_EventsCallback(hal_hsp_handle_t *hhsp, uint32_t events_mask)
{
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  hsp_hw_if_ctx[mw_id].ProcListEventsCallback(mw_id, events_mask);
}
#endif /* USE_HSP_MODULES_PROCLIST */

#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/**
  * @brief  Stream Buffer Empty callback.
  * @param  hhsp          Pointer to a @ref hal_hsp_handle_t.
  * @param  buffers_mask  Mask of Empty STREAM Buffers.
  */
static void HSP_HW_IF_STREAM_BufferEmptyCallback(hal_hsp_handle_t *hhsp, uint32_t buffers_mask)
{
  uint32_t buffer_cplt = 0U;
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  for (uint8_t id = 0U; id < 4U; id++)
  {
    if ((buffers_mask & (1UL << id)) != 0U)
    {
      /* The STREAM Buffer <id> is empty */
      hsp_hw_if_buffer_t *p_hw_if_stream_buffer = &(hsp_hw_if_ctx[mw_id].stream_buffer_input[id]);
      HAL_HSP_STREAM_WriteBufferData(hhsp, (hal_hsp_stream_buffer_t) id, *(p_hw_if_stream_buffer->p_data));

      p_hw_if_stream_buffer->p_data++;
      p_hw_if_stream_buffer->data_counter--;
      if (p_hw_if_stream_buffer->data_counter == 0U)
      {
        buffer_cplt |= (1UL << id);
      }
      else
      {
        /* RE enable the IT which are automatically disabled by the HAL IRQ Handler */
        HAL_HSP_STREAM_Start(hhsp, HAL_HSP_IT_EVT_STREAM0_READY << id, HAL_HSP_IT_ERR_STREAM0_OVR << id);
      }
    }
  }

  if (buffer_cplt != 0U)
  {
    hsp_hw_if_ctx[mw_id].StreamBufferWriteCpltCallback(mw_id, buffer_cplt);
  }
}

/**
  * @brief  Stream Buffer Full callback.
  * @param  hhsp Pointer to a @ref hal_hsp_handle_t.
  * @param  buffers_mask  Mask of Full STREAM Buffers.
  */
static void HSP_HW_IF_STREAM_BufferFullCallback(hal_hsp_handle_t *hhsp, uint32_t buffers_mask)
{
  uint32_t buffer_cplt = 0U;
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  for (uint8_t id = 0U; id < 4U; id++)
  {
    if ((buffers_mask & 0x1U) != 0U)
    {
      /* The STREAM Buffer <id> is empty */
      hsp_hw_if_buffer_t *p_hw_if_stream_buffer = &(hsp_hw_if_ctx[mw_id].stream_buffer_output[id]);
      *(p_hw_if_stream_buffer->p_data) = HAL_HSP_STREAM_ReadBufferData(hhsp, (hal_hsp_stream_buffer_t) id);

      p_hw_if_stream_buffer->p_data++;
      p_hw_if_stream_buffer->data_counter--;
      if (p_hw_if_stream_buffer->data_counter == 0U)
      {
        buffer_cplt |= (1UL << id);
      }
      else
      {
        /* RE enable the IT which are automatically disabled by the HAL IRQ Handler */
        HAL_HSP_STREAM_Start(hhsp, HAL_HSP_IT_EVT_STREAM0_READY << id, HAL_HSP_IT_ERR_STREAM0_OVR << id);
      }
    }
  }

  if (buffer_cplt != 0U)
  {
    hsp_hw_if_ctx[mw_id].StreamBufferReadCpltCallback(mw_id, buffers_mask);
  }
}
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @brief  TCU Started Task callback.
  * @param  hhsp    Pointer to a @ref hal_hsp_handle_t.
  * @param tcu_ids  TCU IDs mask.
  */
static void HSP_HW_IF_TCU_TaskStartedCallback(hal_hsp_handle_t *hhsp, uint32_t tcu_ids)
{
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  hsp_hw_if_ctx[mw_id].TCU_TaskStartedCallback(mw_id, tcu_ids);
}

/**
  * @brief  TCU Ended Task callback.
  * @param  hhsp    Pointer to a @ref hal_hsp_handle_t.
  * @param tcu_ids  TCU IDs mask.
  */
static void HSP_HW_IF_TCU_TaskEndedCallback(hal_hsp_handle_t *hhsp, uint32_t tcu_ids)
{
  uint32_t mw_id = 0U;
  GET_MW_ID_FROM_HSP_HANDLE(hhsp, mw_id);

  hsp_hw_if_ctx[mw_id].TCU_TaskEndedCallback(mw_id, tcu_ids);
}

/**
  * @brief  Retrieve the HAL enum value of Event TRGIN source related to event ID and TRGIN ID.
  * @param  event_id  Event ID.
  * @param  trgin_id  TRGIN ID.
  * @param  enum_value Pointer to hal_hsp_event_source_t to be filled.
  * @retval 0 Success.
  * @retval 1 Error.
  */
static uint32_t HSP_HW_IF_Retrieve_TRGIN_HAL_Event_Source_Enum(uint32_t event_id, uint32_t trgin_id,
                                                               hal_hsp_event_source_t *enum_value)
{
  switch (event_id)
  {
    case 1U:
    {
      if (trgin_id == 3U) { *enum_value = HAL_HSP_EVENT_1_TRGIN3; }
      else if (trgin_id == 8U) { *enum_value = HAL_HSP_EVENT_1_TRGIN8; }
      else return 1;
      break;
    }
    case 2U:
    {
      if (trgin_id == 4U) { *enum_value = HAL_HSP_EVENT_2_TRGIN4; }
      else if (trgin_id == 9U) { *enum_value = HAL_HSP_EVENT_2_TRGIN9; }
      else return 1;
      break;
    }
    case 3U:
    {
      if (trgin_id == 0U) { *enum_value = HAL_HSP_EVENT_3_TRGIN0; }
      else if (trgin_id == 5U) { *enum_value = HAL_HSP_EVENT_3_TRGIN5; }
      else return 1;
      break;
    }
    case 4U:
    {
      if (trgin_id == 1U) { *enum_value = HAL_HSP_EVENT_4_TRGIN1; }
      else if (trgin_id == 6U) { *enum_value = HAL_HSP_EVENT_4_TRGIN6; }
      else return 1;
      break;
    }
    case 5U:
    {
      if (trgin_id == 2U) { *enum_value = HAL_HSP_EVENT_5_TRGIN2; }
      else if (trgin_id == 7U) { *enum_value = HAL_HSP_EVENT_5_TRGIN7; }
      else return 1;
      break;
    }
    case 6U:
    {
      if (trgin_id == 3U) { *enum_value = HAL_HSP_EVENT_6_TRGIN3; }
      else if (trgin_id == 8U) { *enum_value = HAL_HSP_EVENT_6_TRGIN8; }
      else return 1;
      break;
    }
    case 7U:
    {
      if (trgin_id == 4U) { *enum_value = HAL_HSP_EVENT_7_TRGIN4; }
      else if (trgin_id == 9U) { *enum_value = HAL_HSP_EVENT_7_TRGIN9; }
      else return 1;
      break;
    }
    case 8U:
    {
      if (trgin_id == 0U) { *enum_value = HAL_HSP_EVENT_8_TRGIN0; }
      else if (trgin_id == 5U) { *enum_value = HAL_HSP_EVENT_8_TRGIN5; }
      else return 1;
      break;
    }
    case 9U:
    {
      if (trgin_id == 1U) { *enum_value = HAL_HSP_EVENT_9_TRGIN1; }
      else if (trgin_id == 6U) { *enum_value = HAL_HSP_EVENT_9_TRGIN6; }
      else return 1;
      break;
    }
    case 10U:
    {
      if (trgin_id == 2U) { *enum_value = HAL_HSP_EVENT_10_TRGIN2; }
      else if (trgin_id == 7U) { *enum_value = HAL_HSP_EVENT_10_TRGIN7; }
      else return 1;
      break;
    }
    case 11U:
    {
      if (trgin_id == 3U) { *enum_value = HAL_HSP_EVENT_11_TRGIN3; }
      else if (trgin_id == 8U) { *enum_value = HAL_HSP_EVENT_11_TRGIN8; }
      else return 1;
      break;
    }
    case 12U:
    {
      if (trgin_id == 4U) { *enum_value = HAL_HSP_EVENT_12_TRGIN4; }
      else if (trgin_id == 9U) { *enum_value = HAL_HSP_EVENT_12_TRGIN9; }
      else return 1;
      break;
    }
    case 13U:
    {
      if (trgin_id == 0U) { *enum_value = HAL_HSP_EVENT_13_TRGIN0; }
      else if (trgin_id == 5U) { *enum_value = HAL_HSP_EVENT_13_TRGIN5; }
      else return 1;
      break;
    }
    case 14U:
    {
      if (trgin_id == 1U) { *enum_value = HAL_HSP_EVENT_14_TRGIN1; }
      else if (trgin_id == 6U) { *enum_value = HAL_HSP_EVENT_14_TRGIN6; }
      else return 1;
      break;
    }
    case 15U:
    {
      if (trgin_id == 2U) { *enum_value = HAL_HSP_EVENT_15_TRGIN2; }
      else if (trgin_id == 7U) { *enum_value = HAL_HSP_EVENT_15_TRGIN7; }
      else return 1;
      break;
    }
    case 16U:
    {
      if (trgin_id == 3U) { *enum_value = HAL_HSP_EVENT_16_TRGIN3; }
      else if (trgin_id == 8U) { *enum_value = HAL_HSP_EVENT_16_TRGIN8; }
      else return 1;
      break;
    }
    case 17U:
    {
      if (trgin_id == 4U) { *enum_value = HAL_HSP_EVENT_17_TRGIN4; }
      else if (trgin_id == 9U) { *enum_value = HAL_HSP_EVENT_17_TRGIN9; }
      else return 1;
      break;
    }
    case 18U:
    {
      if (trgin_id == 0U) { *enum_value = HAL_HSP_EVENT_18_TRGIN0; }
      else if (trgin_id == 5U) { *enum_value = HAL_HSP_EVENT_18_TRGIN5; }
      else return 1;
      break;
    }
    case 19U:
    {
      if (trgin_id == 1U) { *enum_value = HAL_HSP_EVENT_19_TRGIN1; }
      else if (trgin_id == 6U) { *enum_value = HAL_HSP_EVENT_19_TRGIN6; }
      else return 1;
      break;
    }
    case 20U:
    {
      if (trgin_id == 2U) { *enum_value = HAL_HSP_EVENT_20_TRGIN2; }
      else if (trgin_id == 7U) { *enum_value = HAL_HSP_EVENT_20_TRGIN7; }
      else return 1;
      break;
    }
    case 21U:
    {
      if (trgin_id == 3U) { *enum_value = HAL_HSP_EVENT_21_TRGIN3; }
      else if (trgin_id == 8U) { *enum_value = HAL_HSP_EVENT_21_TRGIN8; }
      else return 1;
      break;
    }
    case 22U:
    {
      if (trgin_id == 4U) { *enum_value = HAL_HSP_EVENT_22_TRGIN4; }
      else if (trgin_id == 9U) { *enum_value = HAL_HSP_EVENT_22_TRGIN9; }
      else return 1;
      break;
    }
  }

  return 0;
}

#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/**
  * @brief  Retrieve the HAL enum value of Event STREAM source related to event ID.
  * @param  event_id  Event ID.
  * @param  enum_value Pointer to hal_hsp_event_source_t to be filled.
  * @retval 0 Success.
  * @retval 1 Error.
  */
static uint32_t HSP_HW_IF_Retrieve_STREAM_HAL_Event_Source_Enum(uint32_t event_id, hal_hsp_event_source_t *enum_value)
{
  switch (event_id)
  {
    case 1U:
    {
      *enum_value = HAL_HSP_EVENT_1_STREAM0;
      break;
    }
    case 2U:
    {
      *enum_value = HAL_HSP_EVENT_2_STREAM1;
      break;
    }
    case 3U:
    {
      *enum_value = HAL_HSP_EVENT_3_STREAM2;
      break;
    }
    case 4U:
    {
      *enum_value = HAL_HSP_EVENT_4_STREAM3;
      break;
    }
    case 5U:
    {
      *enum_value = HAL_HSP_EVENT_5_STREAM0;
      break;
    }
    case 6U:
    {
      *enum_value = HAL_HSP_EVENT_6_STREAM1;
      break;
    }
    case 7U:
    {
      *enum_value = HAL_HSP_EVENT_7_STREAM2;
      break;
    }
    case 8U:
    {
      *enum_value = HAL_HSP_EVENT_8_STREAM3;
      break;
    }
    case 9U:
    {
      *enum_value = HAL_HSP_EVENT_9_STREAM0;
      break;
    }
    case 10U:
    {
      *enum_value = HAL_HSP_EVENT_10_STREAM1;
      break;
    }
    case 11U:
    {
      *enum_value = HAL_HSP_EVENT_11_STREAM2;
      break;
    }
    case 12U:
    {
      *enum_value = HAL_HSP_EVENT_12_STREAM3;
      break;
    }
    case 13U:
    {
      *enum_value = HAL_HSP_EVENT_13_STREAM0;
      break;
    }
    case 14U:
    {
      *enum_value = HAL_HSP_EVENT_14_STREAM1;
      break;
    }
    case 15U:
    {
      *enum_value = HAL_HSP_EVENT_15_STREAM2;
      break;
    }
    case 16U:
    {
      *enum_value = HAL_HSP_EVENT_16_STREAM3;
      break;
    }
    case 17U:
    {
      *enum_value = HAL_HSP_EVENT_17_STREAM0;
      break;
    }
    case 18U:
    {
      *enum_value = HAL_HSP_EVENT_18_STREAM1;
      break;
    }
    case 19U:
    {
      *enum_value = HAL_HSP_EVENT_19_STREAM2;
      break;
    }
    case 20U:
    {
      *enum_value = HAL_HSP_EVENT_20_STREAM3;
      break;
    }
    case 21U:
    {
      *enum_value = HAL_HSP_EVENT_21_STREAM0;
      break;
    }
    case 22U:
    {
      *enum_value = HAL_HSP_EVENT_22_STREAM1;
      break;
    }
  }

  return 0;
}
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
