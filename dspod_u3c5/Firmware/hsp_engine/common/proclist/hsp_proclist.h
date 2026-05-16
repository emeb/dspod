/**
  ******************************************************************************
  * @file    hsp_proclist.h
  * @brief   Header file for hsp_proclist.c
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
#ifndef HSP_PROCLIST_H
#define HSP_PROCLIST_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_conf.h"
#include "hsp_if_conf.h"
#include "hsp_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_PROCLIST HSP Processing List Management
  * @{
  */

/** @defgroup HSP_PROCLIST_Exported_Defines HSP Processing List Exported Constants
  * @{
  */
#define HSP_SEQ_STREAM_BUFFER_0  0U
#define HSP_SEQ_STREAM_BUFFER_1  1U
#define HSP_SEQ_STREAM_BUFFER_2  2U
#define HSP_SEQ_STREAM_BUFFER_3  3U

#define HSP_SEQ_TASK_COMPARATOR_0  0U
#define HSP_SEQ_TASK_COMPARATOR_1  1U
#define HSP_SEQ_TASK_COMPARATOR_2  2U
#define HSP_SEQ_TASK_COMPARATOR_3  3U

#define HSP_TRGO_0  (0x01UL << 0U)
#define HSP_TRGO_1  (0x01UL << 1U)
#define HSP_TRGO_2  (0x01UL << 2U)
#define HSP_TRGO_3  (0x01UL << 3U)

/**
  * @}
  */

/** @defgroup HSP_PROCLIST_Exported_Macros HSP Processing List Exported Macros
  * @{
  */
/**
  * @brief   Convert an event id number to a bit mask value
  * @param   evt_id  value in range [0..31]
  * @return  bitmask value
  */
#define HSP_SEQ_EVENT_ID_TO_BITMASK(evt_id)  (0x1UL << (evt_id))

/**
  * @}
  */

/** @defgroup HSP_PROCLIST_Exported_Types HSP Processing List Exported Types
  * @{
  */
typedef enum
{
  HSP_SEQ_EVENT_1 = 1U,
  HSP_SEQ_EVENT_2 = 2U,
  HSP_SEQ_EVENT_3 = 3U,
  HSP_SEQ_EVENT_4 = 4U,
  HSP_SEQ_EVENT_5 = 5U,
  HSP_SEQ_EVENT_6 = 6U,
  HSP_SEQ_EVENT_7 = 7U,
  HSP_SEQ_EVENT_8 = 8U,
  HSP_SEQ_EVENT_9 = 9U,
  HSP_SEQ_EVENT_10 = 10U,
  HSP_SEQ_EVENT_11 = 11U,
  HSP_SEQ_EVENT_12 = 12U,
  HSP_SEQ_EVENT_13 = 13U,
  HSP_SEQ_EVENT_14 = 14U,
  HSP_SEQ_EVENT_15 = 15U,
  HSP_SEQ_EVENT_16 = 16U,
  HSP_SEQ_EVENT_17 = 17U,
  HSP_SEQ_EVENT_18 = 18U,
  HSP_SEQ_EVENT_19 = 19U,
  HSP_SEQ_EVENT_20 = 20U,
  HSP_SEQ_EVENT_21 = 21U,
  HSP_SEQ_EVENT_22 = 22U,
} hsp_seq_event_t;

typedef enum
{
  HSP_SEQ_EVENT_SYNC_TCU_0 = 0U,
  HSP_SEQ_EVENT_SYNC_TCU_1 = 1U,
  HSP_SEQ_EVENT_SYNC_TCU_2 = 2U,
  HSP_SEQ_EVENT_SYNC_TCU_3 = 3U,
  HSP_SEQ_EVENT_SYNC_TCU_DISABLED = 0xFFU
} hsp_seq_event_sync_tcu_t;

typedef enum
{
  HSP_SEQ_TRGIN_POLARITY_RISING = 0U,
  HSP_SEQ_TRGIN_POLARITY_FALLING = 1U,
} hsp_seq_trgin_polarity_t;

typedef struct
{
  uint32_t instance; /* deprecated */
  hsp_seq_trgin_polarity_t polarity;
  uint32_t source;
  hsp_seq_event_sync_tcu_t evt_sync;
} hsp_seq_event_trigger_config_t;

typedef enum
{
  HSP_SEQ_STREAM_BUFFER_RX = 0U,
  HSP_SEQ_STREAM_BUFFER_TX = 1U,
} hsp_seq_stream_buffer_direction_t;

typedef enum
{
  HSP_SEQ_STREAM_BUFFER_SYNC_DISABLE,
  HSP_SEQ_STREAM_BUFFER_SYNC_ENABLE,
} hsp_seq_stream_buffer_sync_state_t;

typedef struct
{
  uint32_t instance;
  hsp_seq_stream_buffer_direction_t direction;
  hsp_seq_stream_buffer_sync_state_t sync;
  hsp_seq_event_sync_tcu_t evt_sync;
} hsp_seq_event_stream_buffer_config_t;

/**
  * @}
  */

/** @defgroup HSP_PROCLIST_Exported_Functions HSP Processing List Exported Functions
  * @{
  */
/** @defgroup HSP_PROCLIST_Exported_Functions_Group1 HSP Processing List Recording
  * @{
  */
hsp_core_status_t HSP_SEQ_StartRecordPL(hsp_engine_context_t *hmw, uint32_t pl_id);
hsp_core_status_t HSP_SEQ_StopRecordPL(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_SEQ_RunTask(hsp_engine_context_t *hmw, uint32_t pl_id, uint32_t timeout_ms);
hsp_core_status_t HSP_SEQ_ResetPL(hsp_engine_context_t *hmw);

/**
  * @}
  */

/** @defgroup HSP_PROCLIST_Exported_Functions_Group2 HSP Processing List Event Management
  * @{
  */
/* Event configuration -------------------------------------------------------*/
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_StreamBuffer(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                                       const hsp_seq_event_stream_buffer_config_t *p_config);

volatile uint32_t *HSP_SEQ_STREAM_GetBufferAddress(hsp_engine_context_t *hmw, uint32_t stream_buffer_id);

hsp_core_status_t HSP_SEQ_STREAM_Buffer_Start(hsp_engine_context_t *hmw, uint32_t stream_buffer_id,
                                              const int32_t *p_buffer, uint32_t buffer_size);

hsp_core_status_t HSP_SEQ_STREAM_EnableIntToFloat(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_SEQ_STREAM_DisableIntToFloat(hsp_engine_context_t *hmw);

hsp_core_status_t HSP_SEQ_STREAM_Enable(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_SEQ_STREAM_Disable(hsp_engine_context_t *hmw);

#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

hsp_core_status_t HSP_SEQ_EVENT_SetConfig_Trigger(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                                  const hsp_seq_event_trigger_config_t *p_config);
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_CSEG(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                               hsp_seq_event_sync_tcu_t tcu_sync);
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_SPE(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                              hsp_seq_event_sync_tcu_t tcu_sync);

/* Event Control -------------------------------------------------------------*/
hsp_core_status_t HSP_SEQ_EVENT_Enable(hsp_engine_context_t *hmw, uint32_t events_mask);
hsp_core_status_t HSP_SEQ_EVENT_Disable(hsp_engine_context_t *hmw, uint32_t events_mask);
uint32_t HSP_SEQ_EVENT_GetStatus(hsp_engine_context_t *hmw);

hsp_core_status_t HSP_SEQ_EVENT_Trig_CSEG(hsp_engine_context_t *hmw, uint32_t events_mask);
hsp_core_status_t HSP_SEQ_EVENT_PollForPending(hsp_engine_context_t *hmw, uint32_t events_mask, uint32_t timeout_ms);

/**
  * @}
  */

/** @defgroup HSP_PROCLIST_Exported_Functions_Group3 HSP Processing List Event Callback
  * @{
  */
hsp_core_status_t HSP_SEQ_RegisterProcListEventsCallback(hsp_engine_context_t *hmw,
                                                         p_hsp_engine_one_param_cb_t p_callback);

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

#endif /* HSP_PROCLIST_H */
