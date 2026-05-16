/**
  ******************************************************************************
  * @file    hsp_proclist_vector.c
  * @brief   This file implements the HSP VECTOR Processing functions used to
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
#include "hsp_proclist_vector.h"
#include "hsp_hw_if.h"
#include "hsp_utilities.h"
#include "hsp_private_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST_VECTOR_LIBRARY
  * @{
  */
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Vector_Exported_Functions
  * @{
  */
/** @addtogroup HSP_Vector_Exported_Functions_Group1
  * @{
  */
/**
  * @brief Add a function to the current processing list to compute the element-wise absolute value of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectAbs_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_ABS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_ABS_F32;
  }

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;

}

/**
  * @brief Add a function to the current processing list to perform the addition of two real vectors
  * @param hmw          HSP middleware context.
  * @param pSrcA        Input A Buffer address
  * @param pSrcB        Input B Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectAdd_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_ADD_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_ADD_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the average value of a real vector
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectAvg_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
                                      uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_AVG_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the element related division of two real vectors
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Input B Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectDiv_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_DIV_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_DIV_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the dot product of two real vectors
  * @param hmw         HSP middleware context.
  * @param pSrcA       Input A Buffer address
  * @param pSrcB       Input B Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status
  */
hsp_core_status_t HSP_SEQ_VectDotProd_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_DOTPROD_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_MUL_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the decimation of a real vector
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param decim        Decimator factor
  * @param pDst         Output Buffer address
  * @param sizeOu       Output vector size
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectDecim_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t decim,
                                        float32_t *pDst, uint32_t sizeOu, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(decim > 0U);
  HSP_ASSERT_DBG_PARAM(sizeOu > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr3;
  ioType |= HSP_SEQ_IOTYPE_IMM_1;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               0U, NULL, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  /* Second param is not used */
  encoded = encoded & ~HSP_IOTYPE_IMM;
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, decim);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, sizeOu);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_DECIM) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to add to each real vector element
  *        a immediate scalar real value
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param offset       Immediate real value offset to add Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectIOffset_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t offset,
                                          float32_t *pDst, uint32_t nbSamples, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr3;
  hsp_u32_f32_t val;

  val.f32 = offset;
  ioType |= HSP_SEQ_IOTYPE_IMM_1;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               0U, NULL, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_OFFSET_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to multiply each real vector element
  *        by a immediate scalar real value
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param scale        Scale immediate scalar value
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectIScale_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t scale, float32_t *pDst,
                                         uint32_t nbSamples, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr3;
  hsp_u32_f32_t val;

  val.f32 = scale;
  ioType |= HSP_SEQ_IOTYPE_IMM_1;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               0U, NULL, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_SCALE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to add to each real vector element the offset real value
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pOffset    Offset Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectOffset_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pOffset,
                                         float32_t *pDst, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pOffset, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_OFFSET_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to multiply each real vector element by the real scale value
  * @param hmw        HSP middleware context.
  * @param pSrc       Input A Buffer
  * @param pScale     Scale Buffer
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectScale_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pScale,
                                        float32_t *pDst, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pScale, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_SCALE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the element related product of two real vectors
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Input B Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectMul_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_MUL_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_MUL_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the Root Mean Square (RMS) value of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectRms_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
                                      uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_RMS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the element related square-root of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectSqrt_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_SQRT_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SQRT_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the element related subtraction of two real vectors
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Input B Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectSub_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_SUB_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SUB_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to find value and position of the absolute biggest element
  *         of a real vector
  * @param hmw       HSP middleware context.
  * @param pSrc      Input Buffer address
  * @param outVal    Output max_value address
  * @param outIdx    Output max_pos address
  * @param nbSamples Number of float elements to proceed
  * @param ioType    User iotype information
  * @retval          Core status
  */
hsp_core_status_t HSP_SEQ_VectAbsmax_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *outVal,
                                         uint32_t *outIdx, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) outVal, &ouAddr2, (uint32_t) outIdx, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_ABSMAX_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the product of each element
  *        of a real vector and a cosine signal
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param startBuff    Start buffer address (I/O: input is start index and output is nextIdx)
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param step         Step value between 2 cos number in ROM
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectMulCos_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *startBuff,
                                         float32_t *pDst,
                                         uint32_t nbSamples, int32_t step, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) startBuff, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, step);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_MUL_COS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the product of each element
  *        of a real vector and a sine signal
  * and an exponential complex signal
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param startBuff    Start buffer address (I/O: input is start index and output is nextIdx)
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param step         Step value between 2 sin number in ROM
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectMulSin_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *startBuff,
                                         float32_t *pDst,
                                         uint32_t nbSamples, int32_t step, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) startBuff, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, step);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_MUL_SIN_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to find value and position of the biggest element
  *         of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param outVal     Output max_value address
  * @param outIdx     Output max_pos address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectMax_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *outVal,
                                      uint32_t *outIdx, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) outVal, &ouAddr2, (uint32_t) outIdx, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_MAX_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to find value and position of the smallest element
  *         of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param outVal     Output min_value address
  * @param outIdx     Output min_pos address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectMin_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *outVal,
                                      uint32_t *outIdx, uint32_t nbSamples, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) outVal, &ouAddr2, (uint32_t) outIdx, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_MIN_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a saturation of a real vector or scalar
  *        using two programmable thresholds. When a saturation is performed the flag FPUSATF of HSP_EVT_ISR
  *        register is set to 1, and a variable (pRes) is updated.
  * @param hmw          HSP middleware context.
  * @param pSrc         Input buffer address
  * @param pSat         Pointer on saturation limit [loTh, hiTh] (float)
  * @param pDst         Output buffer address
  * @param nbSamples    Number of samples to proceed
  * @param pRes         Res value address (1 if saturation)
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectSat_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pSat, float32_t *pDst,
                                      uint32_t nbSamples, uint32_t *pRes, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;
  uint32_t ouAddr4;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pSat, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) pRes, &ouAddr4) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_SAT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Vector_Exported_Functions_Group2
  * @{
  */

/**
  * @brief Add a function to the current processing list to calculate the exponential of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectExp_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
#if defined(__HSP_DMA__)
  hsp_u32_f32_t expe_val_a;
  expe_val_a.f32 = HSP_EXPE_VAL_A;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, expe_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_EXPS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_EXPS_F32;
  }
  else
  {
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_EXP_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_EXP_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the 10th power of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectExp10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
#if defined(__HSP_DMA__)
  hsp_u32_f32_t exp10_val_a;
  exp10_val_a.f32 = HSP_EXP10_VAL_A;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, exp10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_EXPS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_EXPS_F32;
  }
  else
  {
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_EXP10_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_EXP10_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the natural logarithm
  *        of each elements of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectLn_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
#if defined(__HSP_DMA__)
  hsp_u32_f32_t ln_val_a;
  hsp_u32_f32_t ln_val_b;
  ln_val_a.f32 = HSP_LN_VAL_A;
  ln_val_b.f32 = HSP_LN_VAL_B;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ln_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_LOGS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_LOGS_F32;
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ln_val_b.u32);
  }
  else
  {
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, ln_val_b.u32);
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

#else /* HSP_DMA__ */

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_LN_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_LN_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the decimal (base-10) logarithm
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectLog10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
#if defined(__HSP_DMA__)
  hsp_u32_f32_t log10_val_a;
  hsp_u32_f32_t log10_val_b;
  log10_val_a.f32 = HSP_LOG10_VAL_A;
  log10_val_b.f32 = HSP_LOG10_VAL_B;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, log10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, log10_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_LOGS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_LOGS_F32;
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, log10_val_b.u32);
  }
  else
  {
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
    HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, log10_val_b.u32);
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

#else /* HSP_DMA__ */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_LOG10_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_LOG10_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of natural logarithm
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastLn_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                         uint32_t nbSamples,
                                         uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t fln_val_a;
  hsp_u32_f32_t fln_val_b;
  hsp_u32_f32_t fln_val_c;
  hsp_u32_f32_t fln_val_d;

  fln_val_a.f32 = HSP_FLN_VAL_A;
  fln_val_b.f32 = HSP_FLN_VAL_B;
  fln_val_c.f32 = HSP_FLN_VAL_C;
  fln_val_d.f32 = HSP_FLN_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fln_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fln_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fln_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fln_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of decimal (base-10) logarithm
  *       of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastLog10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                            uint32_t nbSamples,
                                            uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t flog10_val_a;
  hsp_u32_f32_t flog10_val_b;
  hsp_u32_f32_t flog10_val_c;
  hsp_u32_f32_t flog10_val_d;

  flog10_val_a.f32 = HSP_FLOG10_VAL_A;
  flog10_val_b.f32 = HSP_FLOG10_VAL_B;
  flog10_val_c.f32 = HSP_FLOG10_VAL_C;
  flog10_val_d.f32 = HSP_FLOG10_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flog10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flog10_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flog10_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flog10_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-2 logarithm
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastLog2_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t nbSamples,
                                           uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t flog2_val_a;
  hsp_u32_f32_t flog2_val_b;
  hsp_u32_f32_t flog2_val_c;
  hsp_u32_f32_t flog2_val_d;

  flog2_val_a.f32 = HSP_FLOG2_VAL_A;
  flog2_val_b.f32 = HSP_FLOG2_VAL_B;
  flog2_val_c.f32 = HSP_FLOG2_VAL_C;
  flog2_val_d.f32 = HSP_FLOG2_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flog2_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flog2_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flog2_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flog2_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the fast calculation of dB (decibel)
  *        based logarithm of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastLogdB_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                            uint32_t nbSamples,
                                            uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t flogdb_val_a;
  hsp_u32_f32_t flogdb_val_b;
  hsp_u32_f32_t flogdb_val_c;
  hsp_u32_f32_t flogdb_val_d;

  flogdb_val_a.f32 = HSP_FLOGDB_VAL_A;
  flogdb_val_b.f32 = HSP_FLOGDB_VAL_B;
  flogdb_val_c.f32 = HSP_FLOGDB_VAL_C;
  flogdb_val_d.f32 = HSP_FLOGDB_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flogdb_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flogdb_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flogdb_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flogdb_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of exponential
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastExpe_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t nbSamples,
                                           uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t fexpe_val_a;
  hsp_u32_f32_t fexpe_val_b;
  hsp_u32_f32_t fexpe_val_c;
  hsp_u32_f32_t fexpe_val_d;

  fexpe_val_a.f32 = HSP_FEXPE_VAL_A;
  fexpe_val_b.f32 = HSP_FEXPE_VAL_B;
  fexpe_val_c.f32 = HSP_FEXPE_VAL_C;
  fexpe_val_d.f32 = HSP_FEXPE_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexpe_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexpe_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexpe_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexpe_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-10 exponential
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastExp10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                            uint32_t nbSamples,
                                            uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t fexp10_val_a;
  hsp_u32_f32_t fexp10_val_b;
  hsp_u32_f32_t fexp10_val_c;
  hsp_u32_f32_t fexp10_val_d;

  fexp10_val_a.f32 = HSP_FEXP10_VAL_A;
  fexp10_val_b.f32 = HSP_FEXP10_VAL_B;
  fexp10_val_c.f32 = HSP_FEXP10_VAL_C;
  fexp10_val_d.f32 = HSP_FEXP10_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexp10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexp10_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexp10_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexp10_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-2 exponential
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastExp2_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t nbSamples,
                                           uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t fexp2_val_a;
  hsp_u32_f32_t fexp2_val_b;
  hsp_u32_f32_t fexp2_val_c;
  hsp_u32_f32_t fexp2_val_d;

  fexp2_val_a.f32 = HSP_FEXP2_VAL_A;
  fexp2_val_b.f32 = HSP_FEXP2_VAL_B;
  fexp2_val_c.f32 = HSP_FEXP2_VAL_C;
  fexp2_val_d.f32 = HSP_FEXP2_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexp2_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexp2_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexp2_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexp2_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-dB exponential
  *        of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectFastExpdB_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                            uint32_t nbSamples,
                                            uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  hsp_u32_f32_t fexpdb_val_a;
  hsp_u32_f32_t fexpdb_val_b;
  hsp_u32_f32_t fexpdb_val_c;
  hsp_u32_f32_t fexpdb_val_d;

  fexpdb_val_a.f32 = HSP_FEXPDB_VAL_A;
  fexpdb_val_b.f32 = HSP_FEXPDB_VAL_B;
  fexpdb_val_c.f32 = HSP_FEXPDB_VAL_C;
  fexpdb_val_d.f32 = HSP_FEXPDB_VAL_D;

  if (nbSamples == 1U)
  {
    return HSP_CORE_ERROR;
  }
  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexpdb_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexpdb_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexpdb_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexpdb_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Vector_Exported_Functions_Group3
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform the cosine of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectCos_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_COS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_COS_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the sine of each element of a real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectSin_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_SIN_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SIN_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the sine and cosine of each element of real vector
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address (pDst[i]=cosine, pDst[i+1]=sine)
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectSinCos_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_SINCOS_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SINCOS_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the arctangent2 of a real vector
  *        containing the X coordinates and an real vector containing the Y coordinates
  * @param hmw          HSP middleware context.
  * @param pSrcA        Input A Buffer address (contains x coordinates)
  * @param pSrcB        Input B Buffer address (contains y coordinates)
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectAtan2_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_ATAN2_F32;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_ATAN2_F32;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Vector_Exported_Functions_Group4
  * @{
  */
/**
  * @brief Add a function to the current processing list to convert each element of a vector
  *        from float32 to signed 32-bit integer
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectF2I(hsp_engine_context_t *hmw, float32_t *pSrc, int32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_F2I;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_F2I;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert a vector, where each element consists of
  *        two contiguous float32 numbers into two Q15 numbers packed into the same 32-bit location
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectF2Q15(hsp_engine_context_t *hmw, float32_t *pSrc, int32_t *pDst, uint32_t nbSamples,
                                    uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 3U);
  HSP_ASSERT_DBG_PARAM((nbSamples & 1U) == 0U);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_F2Q15;
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector from float32 to Q31
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectF2Q31(hsp_engine_context_t *hmw, float32_t *pSrc, int32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_F2Q31;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_F2Q31;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector
  *        from float32 into unsigned 32-bit
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectF2U(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_F2U;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_F2U;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert a vector containing elements packed
  *         as two Q15 numbers in the same 32-bit location into two contiguous float32 numbers
  * @param hmw         HSP middleware context.
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status
  */
hsp_core_status_t HSP_SEQ_VectQ152F(hsp_engine_context_t *hmw, int32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
                                    uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 1U);
  HSP_ASSERT_DBG_PARAM((nbSamples & 1U) == 0U);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_Q152F;
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector from
  *        Q31 integers to float32
  * @param hmw         HSP middleware context.
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param nbSamples   Number of float elements to proceed
  * @param ioType      User iotype information
  * @retval            Core status
  */
hsp_core_status_t HSP_SEQ_VectQ312F(hsp_engine_context_t *hmw, int32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_Q312F;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_Q312F;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector from
  *       signed 32-bit integer into float32
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectI2F(hsp_engine_context_t *hmw, int32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_I2F;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_I2F;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector from
  *        32-bit unsigned integer into float32
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectU2F(hsp_engine_context_t *hmw, uint32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_U2F;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_U2F;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to convert each element of a vector from
  *        24-bit signed integer to float32
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_Vect24s2F(hsp_engine_context_t *hmw, int32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_24S2F;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_24S2F;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to set each element of a real vector to a value
  * @param hmw        HSP middleware context.
  * @param pSrc       Buffer address of the set value
  * @param pDst       Output Buffer address
  * @param nbSamples  Number of float elements to proceed
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_VectSet_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_SET;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SET;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to set each element of a real vector to a immediate value
  * @param hmw          HSP middleware context.
  * @param value        Value to set in vector
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectISet_f32(hsp_engine_context_t *hmw, float32_t value, float32_t *pDst, uint32_t nbSamples,
                                       uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr2;
  hsp_u32_f32_t val;

  val.f32 = value;
  ioType |= HSP_SEQ_IOTYPE_IMM_0;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, 0U, NULL,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_VEC_SET;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SET;
  }

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the copy of the vector source
  *        to the vector destination
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param pDst         Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectCopy_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbSamples,
                                       uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  uint32_t cmdid = HSP_CMD_VEC_COPY;
  if (nbSamples == 1U)
  {
    cmdid = HSP_CMD_SCA_SET;
  }
  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform zero insertion of a real vector
  * @param hmw          HSP middleware context.
  * @param pSrc         Input Buffer address
  * @param znb          Number of inserted zeros
  * @param pDst         Output Buffer address
  * @param sizeIn       Input vector size
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectZins_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t znb, float32_t *pDst,
                                       uint32_t sizeIn, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(znb > 0);
  HSP_ASSERT_DBG_PARAM(sizeIn > 1);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr3;
  ioType |= HSP_SEQ_IOTYPE_IMM_1;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               0U, NULL, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  /* Second param is not used */
  encoded = encoded & ~HSP_IOTYPE_IMM;
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, znb);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, sizeIn);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_ZINS) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a comparison of an input vector to a threshold
  * @param hmw          HSP middleware context.
  * @param pSrc         Input vector Buffer address
  * @param pLim         Scalar buffer address
  * @param pRes         Vector comparison result flag: 1 if threshold is hit, 0 otherwise
  * @param nbSamples    Number of samples to proceed
  * @param cmpType      Comparison type: Greater, Greater Equal, Less, Less equal, Equal
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_VectCmp_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pLim, uint32_t *pRes,
                                      uint32_t nbSamples, hsp_proclist_cmp_vect_cond_cmd_t cmpType, uint32_t ioType)
{
  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pLim, &ouAddr2, (uint32_t) pRes, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, cmpType);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPB_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}


/**
  * @}
  */

#if defined(__HSP_DMA__)
/** @addtogroup HSP_Vector_Exported_Functions_Group5
@verbatim
  ==============================================================================
                  ##### HSP Memory Transfer functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Adds memory blocking transfer function from external (global) to internal (BRAM) memory
    (+) Adds memory blocking transfer function from internal(BRAM) to external (global) memory
    (+) Adds memory blocking transfer function from internal(BRAM) to internal(BRAM) memory
    (+) Adds memory transfer function from external (global) to internal(BRAM) memory
    (+) Adds memory transfer function from internal(BRAM) to external (global) memory
    (+) Adds memory transfer function from internal(BRAM) to internal (BRAM) memory
    (+) Processing List background block transfer command

@endverbatim
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform a blocking memory transfer from external
  *        (global) to internal (BRAM) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         External input buffer pointer
  * @param pDst         Internal output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Source element format to transfer:
  *                     HSP_DMA_ELT_FMT_32B
  *                     HSP_DMA_ELT_FMT_64B
  *                     HSP_DMA_ELT_FMT_U16
  *                     HSP_DMA_ELT_FMT_S16
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_Ext2Int(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbElems,
                                  uint32_t eltFormat, uint32_t iJump, uint32_t nbIterIn, uint32_t offsetIn,
                                  uint32_t oJump, uint32_t nbIterOu, uint32_t offsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_EXT_0, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, eltFormat);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_EXT2INT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a blocking memory transfer from internal (BRAM)
  *        to external (global) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         Internal input buffer pointer
  * @param pDst         External output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Destination element format to transfer
  *                     HSP_DMA_ELT_FMT_32B
  *                     HSP_DMA_ELT_FMT_64B
  *                     HSP_DMA_ELT_FMT_U16
  *                     HSP_DMA_ELT_FMT_S16
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_Int2Ext(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbElems,
                                  uint32_t eltFormat, uint32_t iJump, uint32_t nbIterIn, uint32_t offsetIn,
                                  uint32_t oJump, uint32_t nbIterOu, uint32_t offsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_EXT_1, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, eltFormat);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_INT2EXT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a blocking memory transfer from internal (BRAM)
  *        to internal (BRAM) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         Internal input buffer pointer
  * @param pDst         Internal output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Element format to transfer:
  *                     HSP_DMA_ELT_FMT_32B 32-bits
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_Int2Int(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t nbElems,
                                  uint32_t eltFormat, uint32_t iJump, uint32_t nbIterIn, uint32_t offsetIn,
                                  uint32_t oJump, uint32_t nbIterOu, uint32_t offsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_DEFAULT, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, eltFormat);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_INT2INT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform memory transfer from external (global)
  *        to internal (BRAM) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         External input buffer pointer
  * @param pDst         Internal output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Source element format to transfer
  *                     HSP_DMA_ELT_FMT_32B
  *                     HSP_DMA_ELT_FMT_64B
  *                     HSP_DMA_ELT_FMT_U16
  *                     HSP_DMA_ELT_FMT_S16
  * @param chanId       Channel index: 1 for channel 1,  2 for channel 2
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @param nextOffsetIn Next input offset used by bgnd next function (source block increment)
  * @param nextOffsetOu Next output offset used by bgnd next function (destination block increment)
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_BgndExt2Int(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                      uint32_t nbElems, uint32_t eltFormat, uint32_t chanId, uint32_t iJump,
                                      uint32_t nbIterIn, uint32_t offsetIn, uint32_t oJump, uint32_t nbIterOu,
                                      uint32_t offsetOu, uint32_t nextOffsetIn, uint32_t nextOffsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  uint32_t tmp32 = eltFormat;
  tmp32 |= chanId;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_EXT_0, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, tmp32);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);
  /* REG_10  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM10, nextOffsetIn);
  /* REG_11  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM11, nextOffsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_EXT2INT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform memory transfer from internal (BRAM)
  *        to external (global) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         Internal input buffer pointer
  * @param pDst         External output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Destination element format to transfer
  *                     HSP_DMA_ELT_FMT_32B
  *                     HSP_DMA_ELT_FMT_64B
  *                     HSP_DMA_ELT_FMT_U16
  *                     HSP_DMA_ELT_FMT_S16
  * @param chanId       Channel index: 1 for channel 1,  2 for channel 2
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @param nextOffsetIn Next input offset used by bgnd next function (source block increment)
  * @param nextOffsetOu Next output offset used by bgnd next function (destination block increment)
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_BgndInt2Ext(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                      uint32_t nbElems, uint32_t eltFormat, uint32_t chanId, uint32_t iJump,
                                      uint32_t nbIterIn, uint32_t offsetIn, uint32_t oJump, uint32_t nbIterOu,
                                      uint32_t offsetOu, uint32_t nextOffsetIn, uint32_t nextOffsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  uint32_t tmp32 = eltFormat;
  tmp32 |= chanId;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_EXT_1, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, tmp32);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);
  /* REG_10  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM10, nextOffsetIn);
  /* REG_11  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM11, nextOffsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_INT2EXT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform memory transfer from internal (BRAM)
  *        to internal (BRAM) memory
  * @param hmw          HSP middleware context.
  * @param pSrc         Internal input buffer pointer
  * @param pDst         Internal output buffer pointer
  * @param nbElems      Number of elements to proceed
  * @param eltFormat    Destination element format to transfer
  *                     HSP_DMA_ELT_FMT_32B
  * @param chanId       Channel index: 1 for channel 1,  2 for channel 2
  * @param iJump        Jump between 2 inputs element read, unit is input element size
  * @param nbIterIn     Number of iterations for input offset application
  * @param offsetIn     Input offset, unit is input element size
  * @param oJump        Jump between 2 outputs element write, unit is output element size
  * @param nbIterOu     Number of iterations for output offset application
  * @param offsetOu     Output offset, unit is output element size
  * @param nextOffsetIn Next input offset used by bgnd next function (source block increment)
  * @param nextOffsetOu Next output offset used by bgnd next function (destination block increment)
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_BgndInt2Int(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                      uint32_t nbElems, uint32_t eltFormat, uint32_t chanId, uint32_t iJump,
                                      uint32_t nbIterIn, uint32_t offsetIn, uint32_t oJump, uint32_t nbIterOu,
                                      uint32_t offsetOu, uint32_t nextOffsetIn, uint32_t nextOffsetOu)
{
  HSP_ASSERT_DBG_PARAM(nbElems > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;
  uint32_t tmp32 = eltFormat;
  tmp32 |= chanId;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, HSP_SEQ_IOTYPE_DEFAULT, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  /* REG_0  Source buffer. Source buffer type must be EXT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  /* REG_1  Destination buffer. Destination buffer type must be INT_BUFF */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  /* REG_2  Increment of the source buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, iJump);
  /* REG_3  Number of iterations for input offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbIterIn);
  /* REG_4  Increment of the destination buffer */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, oJump);
  /* REG_5  Number of iterations for output offset application */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nbIterOu);
  /* REG_6  Format of the data in the source buffer and mode */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, tmp32);
  /* REG_7  Number of elements to proceed */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM7, nbElems);
  /* REG_8  Input offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM8, offsetIn);
  /* REG_9  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM9, offsetOu);
  /* REG_10  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM10, nextOffsetIn);
  /* REG_11  Output offset */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM11, nextOffsetOu);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_INT2INT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to execute background block transfer command
  * @param hmw          HSP middleware context.
  * @param cmd          Background command: 0 for next, 1 for wait, 2 for stop
  * @param chanId       Background channel ID
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_BgndCmd(hsp_engine_context_t *hmw, uint32_t cmd, uint32_t chanId)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, cmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, chanId);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_DMA_BGND_CMD) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */
#endif /* __HSP_DMA__ */

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
