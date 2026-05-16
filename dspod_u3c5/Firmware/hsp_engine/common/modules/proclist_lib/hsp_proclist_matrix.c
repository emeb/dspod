/**
  ******************************************************************************
  * @file    hsp_proclist_matrix.c
  * @brief   This file implements the HSP MATRIX Processing functions used to
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
#include "hsp_proclist_matrix.h"
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

/** @addtogroup HSP_MODULES_PROCLIST_MATRIX_LIBRARY
  * @{
  */
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Matrix_Exported_Functions
  * @{
  */
/**
  * @brief Add a function to the current processing list to compute the absolute value of each element of a matrix
  * @param hmw       HSP middleware context.
  * @param pSrc      Input Buffer address
  * @param pDst      Output Buffer address
  * @param nCols     Matrix columns number
  * @param nRows     Matrix rows number
  * @param ioType    User iotype information
  * @retval          Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixAbs_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                        uint32_t nCols, uint32_t nRows, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

  uint32_t nbSamples = nCols * nRows;
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
  * @brief Add a function to the current processing list to compute the addition of two matrices
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Input B Buffer address
  * @param pDst       Output Buffer address
  * @param nCols      Matrix columns number
  * @param nRows      Matrix rows number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixAdd_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                        float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

  uint32_t nbSamples = nCols * nRows;
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
  * @brief Add a function to the current processing list to compute the multiplication of two matrices
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Input B Buffer address
  * @param pDst       Output Buffer address
  * @param nRowsA     Matrix A rows number
  * @param nColsA     Matrix A columns number
  * @param nRowsB     Matrix B rows number
  * @param nColsB     Matrix B columns number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixMult_f32(hsp_engine_context_t *hmw,
                                         float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                                         uint32_t nRowsA, uint32_t nColsA, uint32_t nRowsB, uint32_t nColsB,
                                         uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nColsA > 0U);
  HSP_ASSERT_DBG_PARAM(nRowsA > 0U);
  HSP_ASSERT_DBG_PARAM(nColsB > 0U);
  HSP_ASSERT_DBG_PARAM(nRowsB > 0U);
  HSP_ASSERT_DBG_PARAM(nColsA == nRowsB);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nRowsA);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nColsA);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, nRowsB);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, nColsB);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_MAT_MUL_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to add a scalar real value to each element of a matrix
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Offset to add Buffer address
  * @param pDst       Output Buffer address
  * @param nCols      Matrix columns number
  * @param nRows      Matrix rows number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixOffset_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                           float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

  uint32_t nbSamples = nCols * nRows;
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
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_OFFSET_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to multiply each element of a matrix by a scalar real value
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Scale to add Buffer address
  * @param pDst       Output Buffer address
  * @param nCols      Matrix columns number
  * @param nRows      Matrix rows number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixScale_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                          float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

  uint32_t nbSamples = nCols * nRows;
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
  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_VEC_SCALE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the subtraction of two matrices
  * @param hmw        HSP middleware context.
  * @param pSrcA      Input A Buffer address
  * @param pSrcB      Scale to add Buffer address
  * @param pDst       Output Buffer address
  * @param nCols      Matrix columns number
  * @param nRows      Matrix rows number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixSub_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                        float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

  uint32_t nbSamples = nCols * nRows;
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
  * @brief Add a function to the current processing list to compute matrix transpose
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nRows      Matrix rows number
  * @param nCols      Matrix columns number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixTrans_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                          uint32_t nRows, uint32_t nCols, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nCols > 0U);
  HSP_ASSERT_DBG_PARAM(nRows > 0U);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nRows);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nCols);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_MAT_TRANS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute matrix inverse
  * @param hmw        HSP middleware context.
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param nCols      Matrix columns number
  * @param nRows      Matrix rows number
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_MatrixInv_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                        uint32_t nRows, uint32_t nCols, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(nRows > 1U);
  HSP_ASSERT_DBG_PARAM(nCols > 1U);

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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nRows);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, nCols);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_MAT_INV_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the copy of NROW elements of a vector into
  *        a column of a NROW-by-NCOL
  *        Each call to this function copy only one matrix column in destination vector area
  * @param hmw          HSP middleware context.
  * @param pSrc         Input vector address
  * @param pCurIdx      Points to the current column index
  * @param pDst         Output matrix address
  * @param matHeight    Matrix height = number of samples in each vector
  * @param matWidth     Matrix width: jump between 2 matrix column elements
  * @param incBetCol    Write pDst increment between 2 consecutives calls of vect2mat functions
  *                     (should be less than matWidth)
  * @param ioType       User iotype information
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_Vector2MatCol_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pCurIdx,
                                            float32_t *pDst, uint32_t matHeight, uint32_t matWidth, uint32_t incBetCol,
                                            uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(matHeight > 0U);
  HSP_ASSERT_DBG_PARAM(matWidth > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pCurIdx, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, matHeight);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, matWidth);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, incBetCol);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_V2M) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the copy of a column of a NROW-by-NCOL matrix
  *        into a vector.
  *        Each call to this function copy only one matrix column in destination vector area
  * @param hmw          HSP middleware context.
  * @param pSrc         Input matrix address (float32_t *)
  * @param pCurIdx      Points to the current column index (uint32_t *)
  * @param pDst         Output vector address (float32_t *)
  * @param matHeight    Matrix height = number of samples in each vector
  * @param matWidth     Matrix width: jump between 2 matrix column elements
  * @param incBetCol    Write pDst increment between 2 consecutives calls of vect2mat functions
  *                     (should be less than matWidth)
  * @param ioType       User iotype information
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_MatCol2Vector_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pCurIdx,
                                            float32_t *pDst, uint32_t matHeight, uint32_t matWidth, uint32_t incBetCol, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(matHeight > 0U);
  HSP_ASSERT_DBG_PARAM(matWidth > 0U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pCurIdx, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, matHeight);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, matWidth);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, incBetCol);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_M2V) != HSP_IF_OK)
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
