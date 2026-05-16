/**
  ******************************************************************************
  * @file    hsp_proclist_matrix.h
  * @brief   Header file for hsp_proclist_matrix.c
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
#ifndef HSP_PROCLIST_MATRIX_H
#define HSP_PROCLIST_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_proclist_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_PROCLIST
  * @{
  */

/** @defgroup HSP_MODULES_PROCLIST_MATRIX_LIBRARY HSP Proclist Matrix Functions
  * @{
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_Matrix_Exported_Functions HSP Matrix Exported Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_MatrixAbs_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                        uint32_t nCols, uint32_t nRows, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixAdd_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                        float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixMult_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                                         uint32_t nRowsA, uint32_t nColsA, uint32_t nRowsB, uint32_t nColsB,
                                         uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixOffset_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                           float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixScale_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                          float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixSub_f32(hsp_engine_context_t *hmw, float32_t *pSrcA, float32_t *pSrcB,
                                        float32_t *pDst, uint32_t nCols, uint32_t nRows, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixTrans_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                          uint32_t nRows, uint32_t nCols, uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatrixInv_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                        uint32_t nRows, uint32_t nCols, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Vector2MatCol_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pCurIdx,
                                            float32_t *pDst, uint32_t matHeight, uint32_t matWidth, uint32_t incBetCol,
                                            uint32_t ioType);
hsp_core_status_t HSP_SEQ_MatCol2Vector_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pCurIdx,
                                            float32_t *pDst, uint32_t matHeight, uint32_t matWidth, uint32_t incBetCol,
                                            uint32_t ioType);

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_PROCLIST_MATRIX_H */
