/**
  ******************************************************************************
  * @file    hsp_hw_if.h
  * @brief   Header file for hsp_hw_if.c.
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

#ifndef HSP_HW_IF_H
#define HSP_HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_conf.h"
#include "hsp_if_conf.h"
#include "interface.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_INTERFACES HSP Interfaces
  * @{
  */

/** @defgroup HSP_HW_IF HSP Hardware Interface
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup HSP_HW_IF_Exported_Constants HSP_HW_IF Exported Constants
  * @{
  */
#if defined(USE_HAL_HSP_CUBE1_LEGACY)
#define hal_hsp_handle_t  HSP_HandleTypeDef
#define hal_hsp_bram_arbitration_t  HAL_HSP_BRAM_ArbitrationTypeDef
#define hal_hsp_boot_config_t  HAL_HSP_Boot_ConfigTypeDef
#define hal_hsp_perf_monitor_t  HAL_HSP_PERF_MONITOR_StateTypeDef
#define hal_hsp_fw_version_t  HAL_HSP_FW_VersionTypeDef
#define hal_hsp_task_t    HAL_HSP_TaskTypeDef
#define hal_hsp_event_sync_t  HAL_HSP_EVENT_SyncTypeDef
#define hal_hsp_event_config_t  HAL_HSP_EVENT_ConfigTypeDef
#define hal_hsp_trgin_source_t  HAL_HSP_TRGIN_SourceTypeDef
#define hal_hsp_trgin_config_t  HAL_HSP_TRGIN_ConfigTypeDef
#define hal_hsp_stream_buffer_config_t  HAL_HSP_STREAM_Buffer_ConfigTypeDef
#define hal_hsp_output_trigger_source_t  HAL_HSP_OUTPUT_TRIGGER_SourceTypeDef
#define hal_hsp_output_trigger_t  HAL_HSP_OUTPUT_TriggerTypeDef
#define hal_hsp_stream_buffer_direction_t  HAL_HSP_STREAM_Buffer_DirectionTypeDef
#define hal_hsp_stream_buffer_t  HAL_HSP_STREAM_BufferTypeDef
#define hal_hsp_trgin_polarity_t  HAL_HSP_TRGIN_PolarityTypeDef
#define hal_hsp_event_source_t  HAL_HSP_EVENT_SourceTypeDef
#define hal_hsp_param_t  HAL_HSP_PARAMTypeDef
#define hal_hsp_perf_monitor_state_t  HAL_HSP_PERF_MONITOR_StateTypeDef
#define hal_hsp_task_comparator_t  HAL_HSP_TASK_ComparatorTypeDef
#define hal_hsp_tsc_config_t  HAL_HSP_TSC_ConfigTypeDef
#define hal_hsp_tsc_prescaler_t  HAL_HSP_TSC_PrescalerTypeDef
#define hal_hsp_capabilities_t HAL_HSP_CapabilitiesTypeDef
#endif /* USE_HAL_HSP_CUBE1_LEGACY */

#define  HSP_HW_IF_SEQ_EVENT_SYNC_DISABLED  0xFFU

#define HSP_HW_IF_PARAM0   HAL_HSP_PARAM_0
#define HSP_HW_IF_PARAM1   HAL_HSP_PARAM_1
#define HSP_HW_IF_PARAM2   HAL_HSP_PARAM_2
#define HSP_HW_IF_PARAM3   HAL_HSP_PARAM_3
#define HSP_HW_IF_PARAM4   HAL_HSP_PARAM_4
#define HSP_HW_IF_PARAM5   HAL_HSP_PARAM_5
#define HSP_HW_IF_PARAM6   HAL_HSP_PARAM_6
#define HSP_HW_IF_PARAM7   HAL_HSP_PARAM_7
#define HSP_HW_IF_PARAM8   HAL_HSP_PARAM_8
#define HSP_HW_IF_PARAM9   HAL_HSP_PARAM_9
#define HSP_HW_IF_PARAM10  HAL_HSP_PARAM_10
#define HSP_HW_IF_PARAM11  HAL_HSP_PARAM_11
#define HSP_HW_IF_PARAM12  HAL_HSP_PARAM_12
#define HSP_HW_IF_PARAM13  HAL_HSP_PARAM_13
#define HSP_HW_IF_PARAM14  HAL_HSP_PARAM_14
#define HSP_HW_IF_PARAM15  HAL_HSP_PARAM_15

#define HSP_HW_IF_WRITE_PARAMR0(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR0_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR1(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR1_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR2(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR2_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR3(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR3_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR4(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR4_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR5(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR5_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR6(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR6_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR7(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR7_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR8(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR8_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR9(val)  (*((volatile uint32_t *)HAL_HSP1_PARAMR9_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR10(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR10_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR11(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR11_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR12(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR12_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR13(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR13_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR14(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR14_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_PARAMR15(val) (*((volatile uint32_t *)HAL_HSP1_PARAMR15_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_DCMDPTR0(val) (*((volatile uint32_t *)HAL_HSP1_DCMDPTR0_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_DCMDPTR1(val) (*((volatile uint32_t *)HAL_HSP1_DCMDPTR1_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_DCMDPTR2(val) (*((volatile uint32_t *)HAL_HSP1_DCMDPTR2_ADDRESS) = (val))
#define HSP_HW_IF_WRITE_DCMDIDR(val)  (*((volatile uint32_t *)HAL_HSP1_DCMDIDR_ADDRESS) = (val))

#define HSP_HW_IF_READ_PARAMR0()  (*((volatile uint32_t *)HAL_HSP1_PARAMR0_ADDRESS))
#define HSP_HW_IF_READ_PARAMR1()  (*((volatile uint32_t *)HAL_HSP1_PARAMR1_ADDRESS))
#define HSP_HW_IF_READ_PARAMR2()  (*((volatile uint32_t *)HAL_HSP1_PARAMR2_ADDRESS))
#define HSP_HW_IF_READ_PARAMR3()  (*((volatile uint32_t *)HAL_HSP1_PARAMR3_ADDRESS))
#define HSP_HW_IF_READ_PARAMR4()  (*((volatile uint32_t *)HAL_HSP1_PARAMR4_ADDRESS))
#define HSP_HW_IF_READ_PARAMR5()  (*((volatile uint32_t *)HAL_HSP1_PARAMR5_ADDRESS))
#define HSP_HW_IF_READ_PARAMR6()  (*((volatile uint32_t *)HAL_HSP1_PARAMR6_ADDRESS))
#define HSP_HW_IF_READ_PARAMR7()  (*((volatile uint32_t *)HAL_HSP1_PARAMR7_ADDRESS))
#define HSP_HW_IF_READ_PARAMR8()  (*((volatile uint32_t *)HAL_HSP1_PARAMR8_ADDRESS))
#define HSP_HW_IF_READ_PARAMR9()  (*((volatile uint32_t *)HAL_HSP1_PARAMR9_ADDRESS))
#define HSP_HW_IF_READ_PARAMR10() (*((volatile uint32_t *)HAL_HSP1_PARAMR10_ADDRESS))
#define HSP_HW_IF_READ_PARAMR11() (*((volatile uint32_t *)HAL_HSP1_PARAMR11_ADDRESS))
#define HSP_HW_IF_READ_PARAMR12() (*((volatile uint32_t *)HAL_HSP1_PARAMR12_ADDRESS))
#define HSP_HW_IF_READ_PARAMR13() (*((volatile uint32_t *)HAL_HSP1_PARAMR13_ADDRESS))
#define HSP_HW_IF_READ_PARAMR14() (*((volatile uint32_t *)HAL_HSP1_PARAMR14_ADDRESS))
#define HSP_HW_IF_READ_PARAMR15() (*((volatile uint32_t *)HAL_HSP1_PARAMR15_ADDRESS))

#define HSP_HW_IF_SMARTCLOCKING_CTRL    0U
#define HSP_HW_IF_SMARTCLOCKING_SPE     1U
#define HSP_HW_IF_SMARTCLOCKING_MMC     2U
#define HSP_HW_IF_SMARTCLOCKING_HSPDMA  3U


#define HSP_HW_IF_LOG2NBP_32    5  /*!< Value of Log2(32) for FFT 32 points     */
#define HSP_HW_IF_LOG2NBP_64    6  /*!< Value of Log2(64) for FFT 64 points     */
#define HSP_HW_IF_LOG2NBP_128   7  /*!< Value of Log2(128) for FFT 128 points   */
#define HSP_HW_IF_LOG2NBP_256   8  /*!< Value of Log2(256) for FFT 256 points   */
#define HSP_HW_IF_LOG2NBP_512   9  /*!< Value of Log2(512) for FFT 512 points   */
#define HSP_HW_IF_LOG2NBP_1024  10 /*!< Value of Log2(1024) for FFT 1024 points */
#define HSP_HW_IF_LOG2NBP_2048  11 /*!< Value of Log2(2048) for FFT 2048 points */
#define HSP_HW_IF_LOG2NBP_4096  12 /*!< Value of Log2(4096) for FFT 4096 points */

#define HSP_HW_IF_SEQ_EVENT_SYNC_DISABLE  HAL_HSP_EVENT_SYNC_DISABLE
#define HSP_HW_IF_SEQ_EVENT_SYNC_ENABLE  HAL_HSP_EVENT_SYNC_ENABLE

#define  HSP_HW_IF_STREAM_BUFFER_DIRECTION_IN  HAL_HSP_STREAM_DIRECTION_CPU_TO_HSP
#define  HSP_HW_IF_STREAM_BUFFER_DIRECTION_OUT HAL_HSP_STREAM_DIRECTION_HSP_TO_CPU

#define HSP_HW_IF_OUTPUT_TRIGGER_NONE    HAL_HSP_OUTPUT_TRIGGER_NONE
#define HSP_HW_IF_OUTPUT_TRIGGER_STREAM  HAL_HSP_OUTPUT_TRIGGER_STREAM
#define HSP_HW_IF_OUTPUT_TRIGGER_CORE    HAL_HSP_OUTPUT_TRIGGER_CORE
#define HSP_HW_IF_OUTPUT_TRIGGER_TIMESTAMPCAPTURE  HAL_HSP_OUTPUT_TRIGGER_TIMESTAMPCAPTURE

#define HSP_HW_IF_BRAM_ARBITRATION_LATENCY_16_CYCLES (uint32_t)HAL_HSP_BRAM_ARBITRATION_LATENCY_16_CYCLES
#define HSP_HW_IF_BRAM_ARBITRATION_LATENCY_8_CYCLES  (uint32_t)HAL_HSP_BRAM_ARBITRATION_LATENCY_8_CYCLES
#define HSP_HW_IF_BRAM_ARBITRATION_LATENCY_4_CYCLES  (uint32_t)HAL_HSP_BRAM_ARBITRATION_LATENCY_4_CYCLES

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/** @defgroup  HSP_HW_IF_Exported_Types HSP_HW_IF Exported Types
  * @{
  */
#define hsp_hw_if_firmware_t  hsp_fw_firmware_t

/**
  * @}
  */

/** @defgroup HSP_HW_IF_Driver_Aliases HSP_HW_IF Driver Aliases
  * @{
  */
#define HSP_HW_IF_WriteDirectCommand(hdriver, cmd_id)  \
  HAL_HSP_DIRECT_WriteCommand((hal_hsp_handle_t *)(hdriver), cmd_id)

#define HSP_HW_IF_WaitEndOfDirectCommand(hdriver)  \
  while (((HSP_TypeDef *)(((hal_hsp_handle_t *)(hdriver))->Instance))->DCMDSR)

#define HSP_HW_IF_MSGBOX_IsMsgAvailable(hdriver)  HAL_HSP_MSGBOX_IsMsgAvailable((hal_hsp_handle_t *)(hdriver))
#define HSP_HW_IF_MSGBOX_ReleaseSemaphore(hdriver)  HAL_HSP_MSGBOX_ReleaseSemaphore((hal_hsp_handle_t *)(hdriver))

#define HSP_HW_IF_SEQ_EVENT_GetStatus(hdriver)  HAL_HSP_EVENT_GetStatus((hal_hsp_handle_t *)(hdriver))

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_HW_IF_Exported_Functions HSP_HW_IF Exported Functions
  * @{
  */
void HSP_HW_IF_Init(void *hdriver, uint32_t mw_id);
void HSP_HW_IF_DeInit(void *hdriver);

/* API to manage the FW & Plugin ---------------------------------------------*/
hsp_if_status_t HSP_HW_IF_LoadFW(void *hdriver, const void *p_fw_cram, const void *p_fw_dram);
hsp_if_status_t HSP_HW_IF_LoadPlugin(void *hdriver, const uint8_t *p_bin, uint32_t size_byte);

/* API to boot the HSP           ---------------------------------------------*/
hsp_if_status_t HSP_HW_IF_Boot(void *hdriver, uint32_t enableCycleCount);

/* API to manage the SmartClocking -------------------------------------------*/
hsp_if_status_t HSP_HW_IF_EnableSmartClocking(void *hdriver, uint32_t clock_id);
hsp_if_status_t HSP_HW_IF_DisableSmartClocking(void *hdriver, uint32_t clock_id);

/* API to manage Command ----------------------------------------------*/
hsp_if_status_t HSP_HW_IF_SendCommand(void *hdriver, uint32_t command_id);
void HSP_HW_IF_WriteParameter(void *hdriver, uint32_t param_id, uint32_t value);
uint32_t HSP_HW_IF_ClearFWERR(void *hdriver);

/* API to configure STI AP Port  ---------------------------------------------*/
hsp_if_status_t HSP_HW_IF_STI_SetPortAddress(void *hdriver, uint32_t port_id, uint32_t block_address);

/* API to manage OUTPUT ------------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_OUTPUT_SetConfig(void *hdriver, uint32_t trgo_id, uint32_t source);

hsp_if_status_t HSP_HW_IF_OUTPUT_Enable(void *hdriver);
hsp_if_status_t HSP_HW_IF_OUTPUT_Disable(void *hdriver);
hsp_if_activation_status_t HSP_HW_IF_OUTPUT_IsEnabled(void *hdriver);

/* API to manage Processing List ---------------------------------------------*/
hsp_if_status_t HSP_HW_IF_SEQ_StartRecord(void *hdriver, uint32_t pl_id);
hsp_if_status_t HSP_HW_IF_SEQ_StopRecord(void *hdriver);
hsp_if_status_t HSP_HW_IF_SEQ_ResetPL(void *hdriver);

hsp_if_status_t HSP_HW_IF_SEQ_RunTask(void *hdriver, uint32_t task_id, uint32_t timeout_ms);

/* API to manage Trigger & Event configuration -------------------------------*/
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_StreamBuffer(void *hdriver, uint32_t event_id,
                                                           uint32_t stream_buffer_id,
                                                           uint32_t direction, uint32_t buffer_sync,
                                                           uint32_t tcu_sync);

hsp_if_status_t HSP_HW_IF_SEQ_STREAM_EnableIntToFloat(void *hdriver);
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_DisableIntToFloat(void *hdriver);

hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Enable(void *hdriver);
hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Disable(void *hdriver);
volatile uint32_t *HSP_HW_IF_SEQ_STREAM_GetBufferAddress(void *hdriver, uint32_t stream_buffer_id);


hsp_if_status_t HSP_HW_IF_SEQ_STREAM_Buffer_Start(void *hdriver, uint32_t stream_buffer_id, const int32_t *p_buffer,
                                                  uint32_t buffer_size);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_Trigger(void *hdriver, uint32_t evt_id,
                                                      uint32_t trigger_source, uint32_t polarity,
                                                      uint32_t event_sync);
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_CSEG(void *hdriver, uint32_t evt_id, uint32_t p_sync);
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_SetConfig_SPE(void *hdriver, uint32_t evt_id, uint32_t p_sync);

hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Enable(void *hdriver, uint32_t events_mask);
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Disable(void *hdriver, uint32_t events_mask);

hsp_if_status_t HSP_HW_IF_SEQ_EVENT_Trig_CSEG(void *hdriver, uint32_t events_mask);
hsp_if_status_t HSP_HW_IF_SEQ_EVENT_PollForPending(void *hdriver, uint32_t events_mask, uint32_t timeout_ms);

/* API to BRAM Bandwidth Access control --------------------------------------*/
hsp_if_status_t HSP_HW_IF_BRAM_EnableConflictAccessCounter(void *hdriver);
hsp_if_status_t HSP_HW_IF_BRAM_DisableConflictAccessCounter(void *hdriver);
uint32_t HSP_HW_IF_BRAM_GetConflictAccessCounter(void *hdriver);
hsp_if_status_t HSP_HW_IF_BRAM_SetBandwidthArbitration(void *hdriver, uint32_t mode);
uint32_t HSP_HW_IF_BRAM_GetBandwidthArbitration(void *hdriver);

/* API for secure Access -----------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_Lock(void *hdriver);

/* API to get the value of Performance counter */
uint32_t HSP_HW_IF_GetPerformanceCounter(void *hdriver);

/* API for TimeStamp Capture -------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_TSC_EnableSPEBreak(void *hdriver);
hsp_if_status_t HSP_HW_IF_TSC_EnablePFCT(void *hdriver, uint32_t prescaler, uint32_t task_id);
hsp_if_status_t HSP_HW_IF_TSC_Enable(void *hdriver);
hsp_if_status_t HSP_HW_IF_TSC_Disable(void *hdriver);

/* API for Task Comparator ---------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_EnableTaskComparator(void *hdriver, uint32_t tcu_id, uint32_t task_id);
hsp_if_status_t HSP_HW_IF_DisableTaskComparator(void *hdriver, uint32_t tcu_id);
hsp_if_status_t HSP_HW_IF_EnableTaskComparator_IT(void *hdriver, uint32_t tcu_id, uint32_t task_id);
hsp_if_status_t HSP_HW_IF_DisableTaskComparator_IT(void *hdriver, uint32_t tcu_id);

uint32_t HSP_HW_IF_GetFirmwareError(void *hdriver);

hsp_if_status_t HSP_HW_IF_WaitAndReleaseSemaphore(void *hdriver);
hsp_if_status_t HSP_HW_IF_WaitSemaphore(void *hdriver);
void HSP_HW_IF_ReleaseSemaphore(void *hdriver);

/* API for CNN ---------------------------------------------------------------*/
hsp_if_status_t HSP_HW_IF_CNN_Begin(void *hdriver, uint32_t cmd_id);
hsp_if_status_t HSP_HW_IF_CNN_StartOfProcess(void *hdriver, uint32_t cnn_cmd_id);
hsp_if_status_t HSP_HW_IF_CNN_WaitEndOfProcess(void *hdriver);
hsp_if_status_t HSP_HW_IF_CNN_End(void *hdriver);

/* API Register MW Callbacks ------------------------------------------------ */
hsp_if_status_t HSP_HW_IF_RegisterErrorCallback(void *hdriver, p_hsp_engine_two_params_cb_t p_callback);
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
hsp_if_status_t HSP_HW_IF_RegisterStreamBufferWriteCpltCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback);
hsp_if_status_t HSP_HW_IF_RegisterStreamBufferReadCpltCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback);
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */
hsp_if_status_t HSP_HW_IF_RegisterTCU_TaskStartedCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback);
hsp_if_status_t HSP_HW_IF_RegisterTCU_TaskEndedCallback(void *hdriver, p_hsp_engine_one_param_cb_t p_callback);

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

#endif /* HSP_HW_IF_H */
