/**
  ******************************************************************************
  * @file    hsp_proclist_filter.c
  * @brief   This file implements the HSP Filter Processing functions used to
  *          record a processing list
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
#include "hsp_proclist_filter.h"
#include "hsp_hw_if.h"
#include "hsp_utilities.h"
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

/** @addtogroup HSP_MODULES_PROCLIST_FILTER_LIBRARY
  * @{
  */
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Filter_Exported_Functions
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform an optimal signal filtering
  *        using a finite impulse response (FIR) filter structure
  * @code
  * y[n] = b0 * x[n] + b1 * x[n-1] + ... + bk * x[n-k]
  * @endcode
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Fir_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                  hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                  uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FIR_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform filtering based on a FIR filter,
  *        followed by decimation, in an optimal way.
  *        To be efficient, this function computes only the output samples from the decimation
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param decimFactor Decimation factor
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_FirDecimate_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                          hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                          uint32_t nbSamples,
                                          uint32_t decimFactor, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);
  HSP_ASSERT_DBG_PARAM(decimFactor > 1U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  hsp_hw_if_fir_decimate_filter_state_t *tmp = (hsp_hw_if_fir_decimate_filter_state_t *)staBuff;

  ouAddr4 = (uint32_t)(tmp->addrHsp);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, decimFactor);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FIRDEC_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a Biquad cascade DF1.
  *        This filter is implemented as a cascade of second-order Biquad sections.
  *        Direct Form 1 Biquad sections are used.
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_BiquadCascadeDf1_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                               hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                               uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_BQ_CAS_DF1_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a Biquad cascade DF2T.
  *        This filter is implemented as a cascade of second-order Biquad sections.
  *        Direct Form 2 Transposed Biquad sections are used.
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_BiquadCascadeDf2T_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                                hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                                uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_BQ_CAS_DF2T_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the convolution between two vectors
  * @code
  * y[n] = a[n] * b[n]
  * @endcode
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param sizeA       Number of float elements in vectA
  * @param sizeB       Number of float elements in vectB
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Conv_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                   float32_t *outBuff,
                                   uint32_t sizeA, uint32_t sizeB, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  HSP_ASSERT_DBG_PARAM(sizeA > 0U);
  HSP_ASSERT_DBG_PARAM(sizeB > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, sizeA);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, sizeB);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CONV_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform correlation between two vectors
  * @code
  * y[n] = a[n] * b[-n]
  * @endcode
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param sizeA       Number of float elements in vectA
  * @param sizeB       Number of float elements in vectB
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Correlate_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                        float32_t *outBuff, uint32_t sizeA, uint32_t sizeB, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  HSP_ASSERT_DBG_PARAM(sizeA > 0U);
  HSP_ASSERT_DBG_PARAM(sizeB > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, sizeA);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, sizeB);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);


  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CORR_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the sum of products of a bank of filters
  *        with the power spectrum of the input signal
  * @param hmw         HSP middleware context
  * @param spectrCol   Input spectrogram slice of length FFTLen / 2 Buffer address
  * @param startIdx    FLTBANK filter pCoefficients start indexes Buffer address
  * @param idxSize     FLTBANK filter pCoefficients size indexes Buffer address
  * @param coef        FLTBANK filter weights Buffer address
  * @param fltbankCol  Output fltbank energies in each filterbank Buffer address
  * @param nFltbanks   Number of Fltbank bands to generate
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_FltBank_f32(hsp_engine_context_t *hmw, float32_t *spectrCol, uint32_t *startIdx,
                                      uint32_t *idxSize, float32_t *coef, float32_t *fltbankCol,
                                      uint32_t nFltbanks, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr0;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nFltbanks > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) spectrCol, &ouAddr0, (uint32_t) fltbankCol, &ouAddr1,
                               0, NULL, 2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) startIdx, &ouAddr2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) idxSize, &ouAddr3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) coef, &ouAddr4) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr0); /* spectrCol */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1); /* fltbankCol */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2); /* startIdx */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr3); /* idxSize */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, ouAddr4); /* coef */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nFltbanks);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FLTBANK_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

#if defined(__HSP_DMA__)
/**
  * @brief Add a function to the current processing list to perform the sum of products of a bank of filters
  *        with the power spectrum of the input signal. Coefficient buffer is external and internal DMA is used
  *        to get external coefficient buffer
  * @param hmw         HSP middleware context
  * @param spectrCol   Input spectrogram slice of length FFTLen / 2 Buffer address
  * @param startIdx    FLTBANK filter pCoefficients start indexes Buffer address
  * @param idxSize     FLTBANK filter pCoefficients size indexes Buffer address
  * @param coef        FLTBANK filter weights Buffer address
  * @param fltbankCol  Output fltbank energies in each filterbank Buffer address
  * @param nFltbanks   Number of Fltbank bands to generate
  * @param dmaAdd      FLTBANK DMA Buffer address (must be max filter size x2 for pingpong)
  * @param dmaSize     FLTBANK DMA Buffer size (full DMA buffer size (ping + pong))
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_FltBankExtC_f32(hsp_engine_context_t *hmw, float32_t *spectrCol, uint32_t *startIdx,
                                          uint32_t *idxSize, float32_t *coef, float32_t *fltbankCol,
                                          uint32_t nFltbanks,
                                          float32_t *dmaAdd, uint32_t dmaSize, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr0;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr6;

  HSP_ASSERT_DBG_PARAM(nFltbanks > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) spectrCol, &ouAddr0, (uint32_t) fltbankCol, &ouAddr1,
                               0, NULL, 2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) startIdx, &ouAddr2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) idxSize, &ouAddr3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) dmaAdd, &ouAddr6) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr0); /* spectrCol */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1); /* fltbankCol */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2); /* startIdx */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr3); /* idxSize */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, (uint32_t) coef);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nFltbanks);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, ouAddr6);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, dmaSize);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_FLTBANK_EXTC_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @brief Add a function to the current processing list to perform LMS (Least Mean Square) filtering
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param refBuff     Reference Buffer address
  * @param errBuff     Error Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param mu          Adaptative factor
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Lms_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                  hsp_filter_state_identifier_t staBuff,
                                  float32_t *outBuff, float32_t *refBuff,
                                  float32_t *errBuff, uint32_t nbSamples, float32_t mu, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1; /* inBuff */
  uint32_t ouAddr2; /* coefBuff */
  uint32_t ouAddr3; /* outBuff */
  uint32_t ouAddr4; /* state buffer */
  uint32_t ouAddr5; /* refBuff */
  uint32_t ouAddr6; /* errBuff */
  hsp_u32_f32_t val;

  val.f32 = mu;

  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) refBuff, &ouAddr5) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) errBuff, &ouAddr6) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, ouAddr5);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, ouAddr6);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_LMS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform IIR filtering using the Lattice structure
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coeffsk     Coefficients Buffer address
  * @param coeffsv     Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IirLattice_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coeffsk,
                                         float32_t *coeffsv, hsp_filter_state_identifier_t staBuff,
                                         float32_t *outBuff,
                                         uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;
  uint32_t ouAddr5;

  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coeffsk, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) coeffsv, &ouAddr5) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, ouAddr5);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IIR_LATTICE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform optimal IIR filtering using Direct Form 1
  * @code
  * IIR coeffs are stored interleaved and in reversed order if K nb stages:
  * B[k-1] A[k-1] B[k-2] A[k-2] ... B[1] A[1] B[0]
  * @endcode
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IirDf1_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                     hsp_filter_state_identifier_t staBuff, float32_t *outBuff,
                                     uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IIR_DF1_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

#if defined(__HSP_DMA__)
/**
  * @brief Add a function to the current processing list to perform the basic and enhanced forms
  *        of a 4-pole, 4-zero Direct Form 1 IIR filter
  * @code
  *        e[n] = (Ref - x[n]) * KE
  *        GL = (KL * y[n-1]) + C
  *        y[n] = GL * (b0 * e[n] + b1 * e[n-1] + b2 * e[n-2] + b3 * e[n-3] + b4 * e[n-4]) +
  *               a1 * y[n-1] + a2 * y[n-2] + a3 * y[n-3] + a4 * y[n-4] + Ffd
  *
  * @endcode
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Iir4p4z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IIR_4P4Z_1S_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @brief Add a function to the current processing list to perform the basic and enhanced forms
  *        of a 3-pole, 3-zero Direct Form 1 IIR filter
  * @code
  *        e[n] = (Ref - x[n]) * KE
  *        GL = (KL * y[n-1]) + C
  *        y[n] = GL * (b0 * e[n] + b1 * e[n-1] + b2 * e[n-2] + b3 * e[n-3]) +
  *               a1 * y[n-1] + a2 * y[n-2] + a3 * y[n-3] + Ffd
  *        Coefficients order is:
  *                     KL, C,  b3, a3, b2, a2,  b1, a1, Ref, KE, b0, Ffd, TicksMax, MinDuty
  * @endcode
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Iir3p3z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IIR_3P3Z_1S_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the basic and enhanced forms
  *       of a 2-pole, 2-zero Direct Form 1 IIR filter
  * @code
  *        e[n] = (Ref - x[n]) * KE
  *        GL = (KL * y[n-1]) + C
  *        y[n] = GL * (b0 * e[n]+ b1 * e[n-1] + b2 * e[n-2]) +
  *              a1 * y[n-1] + a2 * y[n-2] + Ffd
  *        coefficients order is:
  *                     KL, C, b2, a2,  b1, a1, Ref, KE, b0, Ffd, TicksMax, MinDuty
  * @endcode
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param coefBuff    Coefficients Buffer address
  * @param staBuff     State Buffer
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Iir2p2z_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *coefBuff,
                                      hsp_filter_state_identifier_t staBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)coefBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  ouAddr4 = (uint32_t)((hsp_hw_if_filter_state_t *)staBuff)->addrHsp;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IIR_2P2Z_1S_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform element-wise product between a data vector
  *        and a periodic window template
  * @param hmw         HSP middleware context
  * @param inABuff     Input Buffer address
  * @param inBBuff     Input window Buffer address
  * @param outBuff     Output Buffer address
  * @param sizeW       Number of float elements in input vectA (= Window size)
  * @param sizeD0      Number of extra dest value pad to 0
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_WinSym_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff, uint32_t sizeW, uint32_t sizeD0, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  HSP_ASSERT_DBG_PARAM(sizeW > 1U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, sizeW);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, sizeD0);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_WINSYM_F32) != HSP_IF_OK)
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
