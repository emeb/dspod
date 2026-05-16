/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   app_hsp_engine_seq_template.c
  * @brief  This file implements the application using Processing List
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
/* USER CODE END Header */

#include "app_hsp_engine_seq.h"
#include "app_hsp_bram_alloc.h"
#include <string.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Exported variables ------------------------------------------------------- */

/* Access to external variables --------------------------------------------- */
extern hsp_core_handle_t hmw;
extern void Error_Handler(void);

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes ---------------------------------------------- */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* -----------------------------------------------------------------------------
   Example of function to record a Processing List no attached to any event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name> (hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   attached to CSEG event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  /* Record the Processing List */
  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR_EVT_CSEG);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id2, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  /* Configure the Event Synchro */
  mw_status = HSP_SEQ_EVENT_SetConfig_CSEG(hmw, HSP_SEQ_PL_ID_FIR_EVT_CSEG, HSP_SEQ_EVENT_SYNC_TCU_DISABLED);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   attached to SPE event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  /* Record the Processing List */
  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR_EVT_SPE);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id2, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  /* Configure the Event Synchro */
  mw_status = HSP_SEQ_EVENT_SetConfig_SPE(hmw, HSP_SEQ_PL_ID_FIR_EVT_SPE,HSP_SEQ_EVENT_SYNC_TCU_DISABLED);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   attached to TRGIN event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  /* Record the Processing List */
  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR_EVT_TRGIN);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id2, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  /* Configure the Event */
  hsp_seq_event_trigger_config_t trigger_cfg;
  trigger_cfg.polarity = HSP_SEQ_TRGIN_POLARITY_RISING;
  trigger_cfg.source = HSP_TRGIN_DMA_TC_0;
  trigger_cfg.evt_sync = HSP_SEQ_EVENT_SYNC_TCU_DISABLED;

  mw_status = HSP_SEQ_EVENT_SetConfig_Trigger(hmw, HSP_SEQ_PL_ID_FIR_EVT_TRGIN, &trigger_cfg);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   attached to STREAM RX event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  /* Record the Processing List */
  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR_EVT_STREAM_IN);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id2, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  /* Configure the Event */
  hsp_seq_event_stream_buffer_config_t stream_cfg;
  stream_cfg.instance = HSP_SEQ_STREAM_BUFFER_1;
  stream_cfg.direction = HSP_SEQ_STREAM_BUFFER_RX;
  stream_cfg.sync = HSP_SEQ_STREAM_BUFFER_SYNC_DISABLE;
  stream_cfg.evt_sync = HSP_SEQ_EVENT_SYNC_TCU_DISABLED;

  mw_status = HSP_SEQ_EVENT_SetConfig_StreamBuffer(hmw, HSP_SEQ_PL_ID_FIR_EVT_STREAM_IN, &stream_cfg);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   attached to STREAM TX event
   -------------------------------------------------------------------------- */
/**
  * @brief Record <PL name> Processing list
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  /* Record the Processing List */
  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_FIR_EVT_STREAM_OUT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_Fir_f32(hmw, p_buff_in, p_coef, fir_state_id2, p_buff_out, FIR_SAMPLES_IN_NBR, HSP_SEQ_IOTYPE_DEFAULT);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  /* Configure the Event */
  hsp_seq_event_stream_buffer_config_t stream_cfg;
  stream_cfg.instance = HSP_SEQ_STREAM_BUFFER_2;
  stream_cfg.direction = HSP_SEQ_STREAM_BUFFER_TX;
  stream_cfg.sync = HSP_SEQ_STREAM_BUFFER_SYNC_ENABLE;
  stream_cfg.evt_sync = HSP_SEQ_EVENT_SYNC_TCU_DISABLED;

  mw_status = HSP_SEQ_EVENT_SetConfig_StreamBuffer(hmw, HSP_SEQ_PL_ID_FIR_EVT_STREAM_OUT, &stream_cfg);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* -----------------------------------------------------------------------------
   Example of function to record and configure a Processing List
   using HSP_SEQ_SetGpo and/or HSP_SEQ_SetTrgo
   -------------------------------------------------------------------------- */
/**
  * @brief Record Processing list to trig a TRGO and a GPO
  * @param hsp_core_handle_t
  * @retval uint32_t
  */
/*
uint32_t MX_HSP_SEQ_Record_PL_<PL name>(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;
  hsp_core_status_t mw_status;

  mw_status = HSP_SEQ_StartRecordPL(hmw, HSP_SEQ_PL_ID_TRIG_OUTPUT);
  if (mw_status != HSP_CORE_OK) error++;

  /* Trig a pulse on both Outputs HSP_TRGO_DMA1_TRIG_34 & HSP_TRGO_DMA1_TRIG_37 */
  uint32_t trgos_mask = HSP_TRGO_DMA1_TRIG_34 | HSP_TRGO_DMA1_TRIG_37;
  mw_status = HSP_SEQ_SetTrgo(hmw, HSP_TRGO_DMA1_TRIG_34 | HSP_TRGO_DMA1_TRIG_37);
  if (mw_status != HSP_CORE_OK) error++;

  /* Set the output HSP_GPO_DMA1_TRIG_38 to HIGH level */
  /* Set the output HSP_GPO_DMA1_TRIG_39 to LOW level */
  uint32_t gpos_mask = HSP_GPO_DMA1_TRIG_38 | HSP_GPO_DMA1_TRIG_39;
  uint32_t gpos_level = HSP_GPO_DMA1_TRIG_38;
  mw_status =  HSP_SEQ_SetGpo(hmw, gpos_mask, gpos_level);
  if (mw_status != HSP_CORE_OK) error++;

  mw_status = HSP_SEQ_StopRecordPL(hmw);
  if (mw_status != HSP_CORE_OK) error++;

  return error;
}
*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

