/**
  ******************************************************************************
  * @file    hsp_proclist.c
  * @brief   This file implements the functions for the proclist management
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
#include "hsp_proclist.h"
#include "hsp_hw_if.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_PROCLIST
  * @{
  */

/** @addtogroup HSP_PROCLIST_Exported_Functions
  * @{
  */

/** @addtogroup HSP_PROCLIST_Exported_Functions_Group1
  * @{
  */
/**
  * @brief Start the record of a processing list.
  * @param hmw    Pointer to a @ref hsp_engine_context_t.
  * @param pl_id  ID of the new processing list.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_BUSY   Wrong state.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_StartRecordPL(hsp_engine_context_t *hmw, uint32_t pl_id)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_CHECK_UPDATE_STATE(hmw, global_state, HSP_CORE_STATE_IDLE, HSP_CORE_STATE_PROCLIST_RECORDING);

  if (HSP_HW_IF_SEQ_StartRecord(hmw->hdriver, pl_id) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Stop the record of a processing list.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_BUSY   Wrong state.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_StopRecordPL(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_CHECK_UPDATE_STATE(hmw, global_state, HSP_CORE_STATE_PROCLIST_RECORDING, HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_StopRecord(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Run a processing list with the lowest priority.
  * @param  hmw        Pointer to a @ref hsp_engine_context_t.
  * @param pl_id       ID of the processing list
  * @param timeout_ms  Timeout in millisecond
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_RunTask(hsp_engine_context_t *hmw, uint32_t pl_id, uint32_t timeout_ms)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_RunTask(hmw->hdriver, pl_id, timeout_ms) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Reset (remove) all the processing list in HSP.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_ResetPL(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HAL_HSP_STATE_READY);

  if (HSP_HW_IF_SEQ_ResetPL(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}
/**
  * @}
  */

/** @addtogroup HSP_PROCLIST_Exported_Functions_Group2
  * @{
  */
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
/* Event configuration -------------------------------------------------------*/
/**
  * @brief Configure a HSP Event with a STREAM Buffer as trigger source.
  * @param  hmw       Pointer to a @ref hsp_engine_context_t.
  * @param  evt_id    HSP Event ID.
  * @param  p_config  Pointer to a hsp_seq_event_stream_buffer_config_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_StreamBuffer(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                                       const hsp_seq_event_stream_buffer_config_t *p_config)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_PARAM((p_config != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_SetConfig_StreamBuffer(hmw->hdriver, (uint32_t)evt_id,
                                                 p_config->instance, (uint32_t)p_config->direction, p_config->sync,
                                                 p_config->evt_sync) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Get the Stream Buffer Address.
  * @param  hmw               Pointer to a @ref hsp_engine_context_t.
  * @param  stream_buffer_id  Stream Buffer ID.
  * @return Stream Buffer Address
  */
volatile uint32_t *HSP_SEQ_STREAM_GetBufferAddress(hsp_engine_context_t *hmw, uint32_t stream_buffer_id)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  return HSP_HW_IF_SEQ_STREAM_GetBufferAddress(hmw->hdriver, stream_buffer_id);
}

/**
  * @brief Transmit the Application RX/TX buffer to the MW to link to a STREAM Buffer.
  * @param  hmw               Pointer to a @ref hsp_engine_context_t.
  * @param  stream_buffer_id  Stream Buffer ID.
  * @param  p_buffer          Pointer to the application buffer.
  * @param  buffer_size       Size of the buffer in word.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_STREAM_Buffer_Start(hsp_engine_context_t *hmw, uint32_t stream_buffer_id,
                                              const int32_t *p_buffer, uint32_t buffer_size)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_PARAM((p_buffer != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  HSP_HW_IF_SEQ_STREAM_Buffer_Start(hmw->hdriver, stream_buffer_id, p_buffer, buffer_size);

  return HSP_CORE_OK;
}

/**
  * @brief Enable Integer to Float data conversion for STREAM Buffers.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_STREAM_EnableIntToFloat(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_STREAM_EnableIntToFloat(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Disable Integer to Float data conversion for STREAM Buffers.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_STREAM_DisableIntToFloat(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_STREAM_DisableIntToFloat(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Enable all the STREAM Buffers.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_STREAM_Enable(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_STREAM_Enable(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Disable all the STREAM Buffers.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_STREAM_Disable(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_STREAM_Disable(hmw->hdriver) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

/**
  * @brief Configure a HSP Event with a TRGIN as trigger source.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  evt_id    HSP Event ID.
  * @param  p_config  Pointer to a hsp_seq_event_trigger_config_t.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_Trigger(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                                  const hsp_seq_event_trigger_config_t *p_config)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_PARAM((p_config != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_SetConfig_Trigger(hmw->hdriver, (uint32_t)evt_id, /*p_config->instance, */p_config->source,
                                            p_config->polarity, p_config->evt_sync) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Configure a HSP Event with a CSEG trigger.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  evt_id    HSP Event ID.
  * @param  tcu_sync  Synchronisation from the end of task
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_CSEG(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                               hsp_seq_event_sync_tcu_t tcu_sync)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_SetConfig_CSEG(hmw->hdriver, (uint32_t)evt_id, (uint32_t)tcu_sync) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Configure a HSP Event with a HSEG trigger.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  evt_id    HSP Event ID.
  * @param  tcu_sync  Synchronisation from the end of task
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_SetConfig_SPE(hsp_engine_context_t *hmw, hsp_seq_event_t evt_id,
                                              hsp_seq_event_sync_tcu_t tcu_sync)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_SetConfig_SPE(hmw->hdriver, (uint32_t)evt_id, (uint32_t)tcu_sync) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/* Event Control -------------------------------------------------------------*/
/**
  * @brief Enable a set of HSP Events.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  events_mask  Mask of HSP events.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_Enable(hsp_engine_context_t *hmw, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_Enable(hmw->hdriver, events_mask) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Disable a set of HSP Events.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  events_mask  Mask of HSP events.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_Disable(hsp_engine_context_t *hmw, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_Disable(hmw->hdriver, events_mask) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Get the enabled status of all HSP Events.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @return Mask of enabled HSP Events.
  */
uint32_t HSP_SEQ_EVENT_GetStatus(hsp_engine_context_t *hmw)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));

  return HSP_HW_IF_SEQ_EVENT_GetStatus(hmw->hdriver);
}

/**
  * @brief Trig an HSP Event configured with CSEG interface.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  events_mask  Mask of HSP events.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_Trig_CSEG(hsp_engine_context_t *hmw, uint32_t events_mask)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_Trig_CSEG(hmw->hdriver, events_mask) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Poll on HSP event in Pending status.
  * @param  hmw  Pointer to a @ref hsp_engine_context_t.
  * @param  events_mask  Mask of HSP events.
  * @param  timeout_ms  Timeout in millisecond.
  * @retval HSP_CORE_OK     Operation successful.
  * @retval HSP_CORE_ERROR  Operation failed.
  */
hsp_core_status_t HSP_SEQ_EVENT_PollForPending(hsp_engine_context_t *hmw, uint32_t events_mask, uint32_t timeout_ms)
{
  HSP_ASSERT_DBG_PARAM((hmw != NULL));
  HSP_ASSERT_DBG_STATE(hmw->global_state, (uint32_t)HSP_CORE_STATE_IDLE);

  if (HSP_HW_IF_SEQ_EVENT_PollForPending(hmw->hdriver, events_mask, timeout_ms) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

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
