/**
  ******************************************************************************
  * @file    hsp_proclist_complex.c
  * @brief   This file implements the HSP COMPLEX Processing functions used to
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
#include "hsp_proclist_complex.h"
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

/** @addtogroup HSP_MODULES_PROCLIST_COMPLEX_LIBRARY
  * @{
  */
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_COMPLEX_Exported_Functions
  * @{
  */
/**
  * @brief Add a function to the current processing list to compute the conjugate of each vector element.
  *        Each vector element is a complex number stored in interleaved format: (real, imag)
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of complex elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxConj_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                        uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_CONJ_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the dot product of two complex vectors.
  *        Each vector element is a complex number stored in interleaved format: (real, imag)
  * @param hmw         HSP middleware context.
  * @param pSrcA       Input A Buffer address
  * @param pSrcB       Input B Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxDotProd_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                           float32_t *pDst, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrcA, &ouAddr1,
                               (uint32_t) pSrcB, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_DOTPROD_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the magnitude of a complex vector.
  *        Each input vector element is a complex number stored in interleaved format: (real, imag)
  *        Each output vector element is a real number
  * @param hmw         HSP middleware context.
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxMag_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
                                       uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_MAG_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the fast magnitude of a complex vector.
  *        Each input vector element is a complex number stored in interleaved format: (real, imag)
  *        Each output vector element is a real number
  * @param hmw         HSP middleware context.
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxFastMag_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_FMAG_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute magnitude squared of a complex vector.
  *        Each input vector element is a complex number stored in interleaved format: (real, imag)
  *        Each output vector element is a real number
  * @param hmw         HSP middleware context.
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxMagSquared_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                              uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_MAGSQUARED_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the  multiplication of 2 complex vector element.
  *        Each vector element is a complex number stored in interleaved format: (real, imag)
  * @param hmw         HSP middleware context.
  * @param pSrcA       Input A Buffer address
  * @param pSrcB       Input B Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxMul_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                                       uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrcA, &ouAddr1,
                               (uint32_t) pSrcB, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_MUL_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to multiply a complex vector by a real vector.
  *        The first input vector consists of complex numbers stored in interleaved format: (real, imag).
  *        The second input vector consists of real numbers.
  * @param hmw         HSP middleware context.
  * @param pSrcA       Input A Buffer address
  * @param pSrcB       Input B Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of complex elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxRMul_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                                        uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrcA, &ouAddr1,
                               (uint32_t) pSrcB, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_RMUL_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the complex multiplication of
  *        one complex vector element and an exponential complex signal. This function takes advantage
  *        of the exponential function already implemented in the DROM.
  *        Each vector element is a complex number stored in interleaved format: (real, imag)
  * @param hmw            HSP middleware context.
  * @param pSrc           Input Buffer address
  * @param pStart         Start buffer address (I/O: input is start index and output is nextIdx)
  * @param pDst           Output Buffer address
  * @param nbSamples      Number of complex elements to proceed
  * @param step           Step value between 2 exponential number in ROM
  * @param ioType         User iotype information
  * @retval               Core status.
  */
hsp_core_status_t HSP_SEQ_CmplxMulExp_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pStart,
                                          float32_t *pDst, uint32_t nbSamples, int32_t step, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pStart, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, (2U * nbSamples));
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, step);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPLX_MUL_EXP_F32) != HSP_IF_OK)
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
