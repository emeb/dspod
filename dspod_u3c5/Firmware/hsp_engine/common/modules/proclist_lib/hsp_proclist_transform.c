/**
  ******************************************************************************
  * @file    hsp_proclist_transform.c
  * @brief   This file implements the HSP TRANSFORM Processing functions used to
  *          record a processing list
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
#include "hsp_proclist_transform.h"
#include "hsp_hw_if.h"
#include "hsp_utilities.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST_TRANSFORM_LIBRARY
  * @{
  */

/** @defgroup HSP_TRANSFORM_Private_Defines HSP Transform Private Defines
  * @{
  */
/* Private defines -----------------------------------------------------------*/
#define HSP_FFT_LENGTH_32      32U
#define HSP_FFT_LENGTH_64      64U
#define HSP_FFT_LENGTH_128    128U
#define HSP_FFT_LENGTH_256    256U
#define HSP_FFT_LENGTH_512    512U
#define HSP_FFT_LENGTH_1024  1024U
#define HSP_FFT_LENGTH_2048  2048U
#define HSP_FFT_LENGTH_4096  4096U

/**
  * @}
  */

/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Transform_Exported_Functions
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform an in-place FFT transform of a complex vector
  * @param hmw          HSP middleware context
  * @param buff         Input and output Buffer address (must be in HSP memory)
  * @param log2Nbp      log2(number of FFT complex point)
  * @param ifftFlag     Inverse FFT flag
  * @param bitrev       Bit reverse flag
  * @param ioType       User iotype information
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_Fft_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                  uint8_t ifftFlag, uint8_t bitrev, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr;

  HSP_ASSERT_DBG_PARAM((log2Nbp == HSP_LOG2NBP_32) || (log2Nbp == HSP_LOG2NBP_64) || (log2Nbp == HSP_LOG2NBP_128) ||
                       (log2Nbp == HSP_LOG2NBP_256) || (log2Nbp == HSP_LOG2NBP_512) || (log2Nbp == HSP_LOG2NBP_1024) ||
                       (log2Nbp == HSP_LOG2NBP_2048) || (log2Nbp == HSP_LOG2NBP_4096));

  /*  Input buffer must be in shared memory */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) buff, &ouAddr, 0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ifftFlag);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (1U << (uint32_t)log2Nbp));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, HSP_TYPE_FFT_COMPLEX);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, bitrev);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FFT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform an in-place FFT transform of real vector
  * @param hmw          HSP middleware context
  * @param buff         Input and output Buffer address (must be in HSP memory)
  * @param log2Nbp      log2(number of FFT point)
  * @param ifftFlag     Inverse FFT flag: 0: not inverse, 1: inverse
  * @param bitrev       Bit reverse flag: 0: not reverse, 1 reverse
  * @param fftVariant   Type of FFT: HSP_RFFT_TYPE_1, HSP_RFFT_TYPE_2, HSP_RFFT_TYPE_3
  * @param ioType       User iotype information
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_Rfft_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                   uint8_t ifftFlag, uint8_t bitrev, hsp_type_rfft_cmd_t fftVariant,
                                   uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr;

  HSP_ASSERT_DBG_PARAM((log2Nbp == HSP_LOG2NBP_64) || (log2Nbp == HSP_LOG2NBP_128) || (log2Nbp == HSP_LOG2NBP_256) ||
                       (log2Nbp == HSP_LOG2NBP_512) || (log2Nbp == HSP_LOG2NBP_1024) ||
                       (log2Nbp == HSP_LOG2NBP_2048) || (log2Nbp == HSP_LOG2NBP_4096));

  /* Input buffer must be in shared memory */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) buff, &ouAddr, 0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ifftFlag);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (1U << (uint32_t)log2Nbp));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, (uint32_t)fftVariant);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, bitrev);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FFT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform an in-place DCT transform of a real vector
  * @param hmw          HSP middleware context
  * @param buff         Input and output Buffer address (must be in HSP memory)
  * @param log2Nbp      log2(number of FFT point)
  * @param ioType       User iotype information
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_Dct_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                  uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr;

  HSP_ASSERT_DBG_PARAM((log2Nbp == HSP_LOG2NBP_32) || (log2Nbp == HSP_LOG2NBP_64) || (log2Nbp == HSP_LOG2NBP_128) ||
                       (log2Nbp == HSP_LOG2NBP_256) || (log2Nbp == HSP_LOG2NBP_512) || (log2Nbp == HSP_LOG2NBP_1024) ||
                       (log2Nbp == HSP_LOG2NBP_2048) || (log2Nbp == HSP_LOG2NBP_4096));

  /* Input buffer must be in shared memory */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) buff, &ouAddr, 0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (1U << (uint32_t)log2Nbp));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_DCT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform an in-place inverse DCT transform of a real vector
  * @param hmw        HSP middleware context
  * @param buff       Input and output Buffer address (must be in HSP memory)
  * @param log2Nbp    log2(number of FFT point)
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_IDct_f32(hsp_engine_context_t *hmw, float32_t *buff, hsp_ftt_lognbp_cmd_t log2Nbp,
                                   uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr;

  HSP_ASSERT_DBG_PARAM((log2Nbp == HSP_LOG2NBP_32) || (log2Nbp == HSP_LOG2NBP_64) || (log2Nbp == HSP_LOG2NBP_128) ||
                       (log2Nbp == HSP_LOG2NBP_256) || (log2Nbp == HSP_LOG2NBP_512) || (log2Nbp == HSP_LOG2NBP_1024) ||
                       (log2Nbp == HSP_LOG2NBP_2048) || (log2Nbp == HSP_LOG2NBP_4096));

  /* Input buffer must be in shared memory */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) buff, &ouAddr, 0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (1U << (uint32_t)log2Nbp));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IDCT_F32) != HSP_IF_OK)
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

/**
  * @}
  */
