/**
  ******************************************************************************
  * @file    hsp_proclist_scalar.h
  * @brief   Header file for hsp_proclist_scalar.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef HSP_PROCLIST_SCALAR_H
#define HSP_PROCLIST_SCALAR_H

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

/** @defgroup HSP_MODULES_PROCLIST_SCALAR_LIBRARY HSP Proclist Scalar Functions
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_Scalar_Exported_Functions HSP Scalar Exported Functions
  * @{
  */
/** @defgroup HSP_Scalar_Exported_Functions_Group1  Arithmetic Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_ScaIAdd_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToAdd,
                                      float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaISub_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToSub,
                                      float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIMul_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToMult,
                                      float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaISub_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSub_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffSub, int32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIMul_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaMul_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffMul, int32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIAdd_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaAdd_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffAdd, int32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaMac_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSub_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *inBuffSub,
                                     float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSqrt_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaMul_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaDiv_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaAdd_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaAbs_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSat_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pSat, float32_t *pDst,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaNeg_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaInc_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff, uint32_t ioType);

/**
  * @}
  */


/** @defgroup HSP_Scalar_Exported_Functions_Group2  Exponential & Logarithmic Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_ScaExp10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                       uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaExp_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaLog10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                       uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaLn_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastExp2_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                          uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastExp10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastExpe_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                          uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastExpdB_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastLog2_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                          uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastLog10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastLogdB_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFastLn_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                        uint32_t ioType);

/**
  * @}
  */
/** @defgroup HSP_Scalar_Exported_Functions_Group3  Trigonometric Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_ScaSin_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSinCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                        uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaAtan2_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                       float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFSin_f32(hsp_engine_context_t *hmw, float32_t *inBuffId, float32_t *outBuffId,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaFAtan2_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                        float32_t *outBuff, uint32_t ioType);
#if defined(__HSP_DMA__)
hsp_core_status_t HSP_SEQ_ScaIdxSinCos_f32(hsp_engine_context_t *hmw, uint32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType);
#endif /* __HSP_DMA__ */

/**
  * @}
  */

#if defined(__HSP_DMA__)
/** @defgroup HSP_Scalar_Exported_Functions_Group4  Controller Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_ScaClarke_f32(hsp_engine_context_t *hmw, hsp_i_a_b_t *inBuff,
                                        hsp_i_alpha_beta_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIClarke_f32(hsp_engine_context_t *hmw, hsp_i_alpha_beta_t *inBuffId,
                                         hsp_i_a_b_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaPark_f32(hsp_engine_context_t *hmw, float32_t *thetaBuff,
                                      hsp_v_alpha_beta_t *abBuff, hsp_v_q_d_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIPark_f32(hsp_engine_context_t *hmw, float32_t *thetaBuff,
                                       hsp_v_q_d_t *qdBuff, hsp_v_alpha_beta_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaPid_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     hsp_pid_buff_cfg_t *cfgBuff, float32_t *outBuff,
                                     uint32_t satMod, uint32_t satfl, uint32_t ioType);
/**
  * @}
  */
#endif /* __HSP_DMA__ */


/** @defgroup HSP_Scalar_Exported_Functions_Group5  Support Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_ScaQ312F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaF2Q31(hsp_engine_context_t *hmw, float32_t *inBuff, int32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Sca24S2F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaU2F(hsp_engine_context_t *hmw, uint32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaF2U(hsp_engine_context_t *hmw, float32_t *inBuff, uint32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaI2F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaF2I(hsp_engine_context_t *hmw, float32_t *inBuff, int32_t *outBuff, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSet_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaISet_f32(hsp_engine_context_t *hmw, float32_t valueToSet, float32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaSet_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaISet_u32(hsp_engine_context_t *hmw, uint32_t valueToSet, uint32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_Sca2Vect_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pIdx, float32_t *pDst,
                                       uint32_t nbSamples, uint32_t ioType);
/**
  * @}
  */

/** @defgroup HSP_Scalar_Exported_Functions_Group6  Logic & Bitwise Functions
  * @{
  */

hsp_core_status_t HSP_SEQ_ScaIShift_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t iShift, int32_t *outBuff,
                                        uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaShift_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *pShift, int32_t *outBuff,
                                       uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIAnd_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaAnd_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffAnd,
                                     uint32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIOr_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaOr_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffOr, uint32_t *outBuff,
                                    uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaIXor_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                      uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaXor_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffXor,
                                     uint32_t *outBuff,
                                     uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaNot_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff,
                                     uint32_t ioType);
/**
  * @}
  */

/** @defgroup HSP_Scalar_Exported_Functions_Group7  Miscellaneous Functions
  * @{
  */
hsp_core_status_t HSP_SEQ_GetSatFlag(hsp_engine_context_t *hmw, uint32_t *pDst, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ClrSatFlag(hsp_engine_context_t *hmw);
hsp_core_status_t HSP_SEQ_CounterAdd(hsp_engine_context_t *hmw, uint8_t cntIdx, uint32_t val);
hsp_core_status_t HSP_SEQ_CounterSet(hsp_engine_context_t *hmw, uint8_t cntIdx, uint32_t val);
hsp_core_status_t HSP_SEQ_CounterSet(hsp_engine_context_t *hmw, uint8_t cntIdx, uint32_t val);
hsp_core_status_t HSP_SEQ_SendEvt(hsp_engine_context_t *hmw, uint32_t listNumber, uint32_t itf);
hsp_core_status_t HSP_SEQ_SetFlag(hsp_engine_context_t *hmw, uint32_t flag);
hsp_core_status_t HSP_SEQ_SetTrgo(hsp_engine_context_t *hmw, uint32_t val);
hsp_core_status_t HSP_SEQ_SetGpo(hsp_engine_context_t *hmw, uint32_t fieldMask, uint32_t fieldVal);
hsp_core_status_t HSP_SEQ_WaitCond(hsp_engine_context_t *hmw, uint32_t *regBuff, uint32_t fieldMask,
                                   uint32_t fieldVal, uint32_t diffCond, uint32_t timeout, uint32_t timeoutFl);
#if defined(__HSP_DMA__)
hsp_core_status_t HSP_SEQ_SetAdcSrc(hsp_engine_context_t *hmw, uint32_t *pAdcBufNb, uint32_t *pAdcSrc,
                                    uint32_t *pAdcFlt, uint32_t ioType);
hsp_core_status_t HSP_SEQ_Crc32(hsp_engine_context_t *hmw, uint32_t *pState,  uint32_t *pCRC,  uint32_t blockSize,
                                uint32_t posFEOR, uint32_t posFEOB, hsp_crc_mem_type_cmd_t memType, uint32_t ioType);
#endif /* __HSP_DMA__ */
hsp_core_status_t HSP_SEQ_SetBits(hsp_engine_context_t *hmw, uint32_t *regBuff, uint32_t fieldMask, uint32_t fieldVal);
hsp_core_status_t HSP_SEQ_ScaIModAdd_u32(hsp_engine_context_t *hmw, uint32_t *addr, uint32_t imm, uint32_t base,
                                         uint32_t size, uint32_t ioType);
hsp_core_status_t HSP_SEQ_ScaCmpCnt_f32(hsp_engine_context_t *hmw, float32_t *pSrc, int32_t *pDst,
                                        hsp_cmp_cnt_lim_t *pLim, hsp_cmp_cnt_cnt_t *pMax,
                                        hsp_cmp_cnt_cnt_t *pCnt, uint32_t ioType);

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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_PROCLIST_SCALAR_H */
