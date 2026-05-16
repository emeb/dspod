/**
  ******************************************************************************
  * @file    hsp_proclist_scalar.c
  * @brief   This file implements the HSP SCALAR Processing functions used to
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
#include "hsp_proclist_scalar.h"
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

/** @addtogroup HSP_MODULES_PROCLIST_SCALAR_LIBRARY
  * @{
  */
/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Scalar_Exported_Functions
  * @{
  */
/** @addtogroup HSP_Scalar_Exported_Functions_Group3
  * @{
  */
/**
  * @brief Add a function to the current processing list to compute the sine of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSin_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SIN_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the cosine of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_COS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute the sine and cosine of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address (outBuff[0]=cosine, outBuff[1]=sine)
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSinCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                        uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SINCOS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the arctangent2 of
  *        a real scalar X coordinate and a real scalar Y coordinate
  * @param hmw        HSP middleware context
  * @param inABuff    Input A Buffer
  * @param inBBuff    Input B Buffer
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaAtan2_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                       float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1,
                               (uint32_t)inBBuff, &ouAddr2, (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ATAN2_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast computation of the sine
  *        of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFSin_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_FSIN_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast computation of the cosine
  *       of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFCos_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_FCOS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of arctangent2
  *        of a real scalar X coordinate and a real scalar Y coordinate
  * @param hmw        HSP middleware context
  * @param inABuff    Input A Buffer
  * @param inBBuff    Input B Buffer
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFAtan2_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                        float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1,
                               (uint32_t)inBBuff, &ouAddr2, (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_FATAN2_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

#if defined(__HSP_DMA__)
/**
  * @brief Add a function to the current processing list to compute the sine and cosine from an index as input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIdxSinCos_f32(hsp_engine_context_t *hmw, uint32_t *inBuff, float32_t *outBuff,
                                           uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_IDX_SINCOS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @}
  */

#if defined(__HSP_DMA__)
/** @addtogroup HSP_Scalar_Exported_Functions_Group4
  * @{
  */
/**
  * @brief Add a function to the current processing list to compute a Clarke transform
  * @param hmw        HSP middleware context
  * @param inBuff     Input three-phase coordinate Buffer address [Ia, Ib]
  * @param outBuff    Points to output two-phase orthogonal vector axis Buffer address [pIalpha, pIbeta]
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaClarke_f32(hsp_engine_context_t *hmw, hsp_i_a_b_t *inBuff,
                                        hsp_i_alpha_beta_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_CLARKE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute a Floating-point Inverse Clarke transform
  * @param hmw        HSP middleware context
  * @param inBuff     Input two-phase orthogonal vector Buffer address [pIalpha, pIbeta]
  * @param outBuff    Points to output three-phase coordinate Buffer address [Ia, Ib]
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIClarke_f32(hsp_engine_context_t *hmw, hsp_i_alpha_beta_t *inBuff,
                                         hsp_i_a_b_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ICLARKE_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute a Park transform: transforms stator values
  *        alpha and beta, which belong to a stationary qd reference frame, to a rotor flux synchronous reference
  *        frame (properly oriented), so as q and d.
  *                   d = alpha *sin(theta) + beta *cos(Theta)
  *                   q = alpha *cos(Theta) - beta *sin(Theta)
  * @param hmw        HSP middleware context
  * @param thetaBuff  Input angle rotating frame angular position is in float Buffer
  * @param abBuff     Input stator values alpha and beta [alpha, beta] in float Buffer  [alpha, beta]
  * @param outBuff    Output Stator values q and d [q, d] in float format Buffer
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaPark_f32(hsp_engine_context_t *hmw, float32_t *thetaBuff,
                                      hsp_v_alpha_beta_t *abBuff, hsp_v_q_d_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)thetaBuff, &ouAddr1,
                               (uint32_t)abBuff, &ouAddr2, (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_PARK_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute a Ipark transform: Transforms stator voltage
  *        qVq and qVd, that belong to a rotor flux synchronous rotating frame, to a stationary reference frame,
  *        so as to obtain qValpha and qVbeta:
  *        Valfa = Vq*Cos(theta) + Vd*Sin(theta)
  *        Vbeta =-Vq*Sin(theta) + Vd*Cos(theta)
  * @param hmw        HSP middleware context
  * @param thetaBuff  Input rotating frame angular position in float Buffer
  * @param qdBuff     stator voltage Vq and Vd in float format [q, d] Buffer
  * @param outBuff    Output Stator voltage Valpha and Vbeta [alpha, beta] in float Buffer
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIPark_f32(hsp_engine_context_t *hmw, float32_t *thetaBuff,
                                       hsp_v_q_d_t *qdBuff, hsp_v_alpha_beta_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)thetaBuff, &ouAddr1,
                               (uint32_t)qdBuff, &ouAddr2, (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_IPARK_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to compute pid
  * @param hmw         HSP middleware context
  * @param inABuff     Measured value Buffer
  * @param inBBuff     Set point value Buffer
  * @param cfgBuff     PID config Buffer
  * @param outBuff     Output Buffer address
  * @param satMod      Saturation mode (0 for static, 1 for dynamic)
  * @param satFl       Saturation detected flag index
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaPid_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     hsp_pid_buff_cfg_t *cfgBuff, float32_t *outBuff,
                                     uint32_t satMod, uint32_t satfl, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t ouAddr4;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1,
                               (uint32_t)inBBuff, &ouAddr2, (uint32_t)cfgBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)outBuff, &ouAddr4) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, (uint32_t)satfl);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, (uint32_t)satMod);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_PID_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
/**
  * @}
  */
#endif /* __HSP_DMA__ */

/** @addtogroup HSP_Scalar_Exported_Functions_Group1
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform the addition of a real scalar with an
  *        immediate real value
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer
  * @param valueToAdd Immediate value to add
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIAdd_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToAdd,
                                      float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  hsp_u32_f32_t val;

  val.f32 = valueToAdd;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)&valueToAdd, NULL, (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ADD_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the subtraction of a real scalar with
  *       an immediate real value
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer
  * @param valueToSub Immediate value to sub
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaISub_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToSub,
                                      float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  hsp_u32_f32_t val;

  val.f32 = valueToSub;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)&valueToSub, NULL, (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SUB_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the multiplication of a real scalar with
  *        an immediate real value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer
  * @param valueToMult Immediate value to multiply
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIMul_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t valueToMult,
                                      float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  hsp_u32_f32_t val;

  val.f32 = valueToMult;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)&valueToMult, NULL, (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MUL_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the subtraction of a integer scalar with
  *        an immediate integer value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Input Buffer value to subtract
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaISub_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, (uint32_t)imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SUB_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the subtraction of two integer scalar inputs
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffSub   Input Value Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSub_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffSub, int32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffSub, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SUB_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the multiplication of a integer scalar with
  *        an immediate integer value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Input Buffer value to multiply
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIMul_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, (uint32_t)imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MUL_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the multiplication of two integer scalar inputs
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffMul   Input Value Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaMul_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffMul, int32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffMul, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MUL_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the addition of a integer scalar with
  *        an immediate integer value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Input Buffer value to add
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIAdd_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t imm, int32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, (uint32_t)imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ADD_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the addition of two integer scalar inputs
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffAdd   Input Value Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaAdd_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *inBuffAdd, int32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffAdd, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ADD_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the subtraction of two real scalar inputs
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffSub   Input Value Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSub_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *inBuffSub,
                                     float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffSub,
                               &ouAddr2, (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SUB_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the square-root of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer value
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSqrt_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SQRT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the multiplication of two real scalar inputs
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaMul_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MUL_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the division of two real scalar inputs
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaDiv_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_DIV_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the addition of two real scalar inputs
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaAdd_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ADD_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the absolute value of a real scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer address
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
 */
hsp_core_status_t HSP_SEQ_ScaAbs_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_ABS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a saturation of an input real value
  *        using two programmable thresholds.
  * @param hmw         HSP middleware context
  * @param pSrc        Input buffer address
  * @param pSat        Pointer on saturation limit [loTh, hiTh]
  * @param pDst        Output buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSat_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pSat, float32_t *pDst,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pSat, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SAT_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}


/**
  * @brief Add a function to the current processing list to perform the negation of a scalar float value
  * @param hmw         HSP middleware context
  * @param pSrc        Input Scalar address
  * @param pDst        Output Scalar address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaNeg_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;
  uint32_t fixIotype = HSP_SEQ_IOTYPE_DEFAULT;

  if ((ioType & HSP_SEQ_IOTYPE_POP_0) == HSP_SEQ_IOTYPE_POP_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_POP_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_POP_1) == HSP_SEQ_IOTYPE_POP_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_POP_2;
  }
  if ((ioType & HSP_SEQ_IOTYPE_RXTX_0) == HSP_SEQ_IOTYPE_RXTX_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_RXTX_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_RXTX_1) == HSP_SEQ_IOTYPE_RXTX_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_RXTX_2;
  }
#if defined(__HSP_DMA__)
  if ((ioType & HSP_SEQ_IOTYPE_EXT_0) == HSP_SEQ_IOTYPE_EXT_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_EXT_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_EXT_1) == HSP_SEQ_IOTYPE_EXT_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_EXT_2;
  }
  if ((ioType & HSP_SEQ_IOTYPE_ADC_0) == HSP_SEQ_IOTYPE_ADC_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_ADC_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_ADC_1) == HSP_SEQ_IOTYPE_ADC_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_ADC_2;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_0_0) == HSP_SEQ_IOTYPE_AP_0_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_1_0;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_0_1) == HSP_SEQ_IOTYPE_AP_0_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_1_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_0_2) == HSP_SEQ_IOTYPE_AP_0_2)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_1_2;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_0_3) == HSP_SEQ_IOTYPE_AP_0_3)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_1_3;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_0_4) == HSP_SEQ_IOTYPE_AP_0_4)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_1_4;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_1_0) == HSP_SEQ_IOTYPE_AP_1_0)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_2_0;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_1_1) == HSP_SEQ_IOTYPE_AP_1_1)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_2_1;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_1_2) == HSP_SEQ_IOTYPE_AP_1_2)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_2_2;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_1_3) == HSP_SEQ_IOTYPE_AP_1_3)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_2_3;
  }
  if ((ioType & HSP_SEQ_IOTYPE_AP_1_4) == HSP_SEQ_IOTYPE_AP_1_4)
  {
    fixIotype |= HSP_SEQ_IOTYPE_AP_2_4;
  }
#endif /* __HSP_DMA__ */
  if (HSP_UTILITIES_BuildParam(hmw, fixIotype, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pSrc, &ouAddr2, (uint32_t) pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_NEG_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the unsigned increment of a scalar by 1
  * @param hmw         HSP middleware context
  * @param inBuff      Input Scalar address
  * @param outBuff     Output Scalar address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaInc_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_INC_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the (MAC) multiplication-accumulation
  *        of 3 scalars: outBuff = inABuff * inBBuff + outBuff
  * @param hmw         HSP middleware context
  * @param inABuff     Input A Buffer address
  * @param inBBuff     Input B Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaMac_f32(hsp_engine_context_t *hmw, float32_t *inABuff, float32_t *inBBuff,
                                     float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inABuff, &ouAddr1, (uint32_t)inBBuff, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MAC_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Scalar_Exported_Functions_Group2
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform the 10th power of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaExp10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                       uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
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

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_EXPS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_EXP10_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */

  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the exponential of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaExp_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
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

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_EXPS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_EXP_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the decimal (base-10) logarithm
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaLog10_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                       uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
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

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_LOGS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_LOG10_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to calculate the natural logarithm of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaLn_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
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
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ln_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_LOGS_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#else /* HSP_DMA__ */

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_LN_F32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
#endif /* HSP_DMA__ */
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of exponential
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFastExpe_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexpe_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexpe_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexpe_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexpe_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-2 exponential
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFastExp2_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexp2_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexp2_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexp2_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexp2_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-10 exponential
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFastExp10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexp10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexp10_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexp10_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexp10_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-dB exponential
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param pSrc        Input Buffer address
  * @param pDst        Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaFastExpdB_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fexpdb_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fexpdb_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fexpdb_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fexpdb_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FEXPS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of natural logarithm
  *        of a real scalar input
  * @param hmw        HSP middleware context
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_ScaFastLn_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, fln_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, fln_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, fln_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, fln_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of decimal (base-10) logarithm
  *       of a real scalar input
  * @param hmw        HSP middleware context
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_ScaFastLog10_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flog10_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flog10_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flog10_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flog10_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of base-2 logarithm
  *        of a real scalar input
  * @param hmw        HSP middleware context
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_ScaFastLog2_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst, uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flog2_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flog2_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flog2_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flog2_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the fast calculation of dB (decibel)
  *         based logarithm of a real scalar input
  * @param hmw        HSP middleware context
  * @param pSrc       Input Buffer address
  * @param pDst       Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status
  */
hsp_core_status_t HSP_SEQ_ScaFastLogdB_f32(hsp_engine_context_t *hmw, float32_t *pSrc, float32_t *pDst,
                                           uint32_t ioType)
{
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

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, 1U);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, flogdb_val_a.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, flogdb_val_b.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, flogdb_val_c.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM6, flogdb_val_d.u32);

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  uint32_t cmdid = HSP_CMD_SCA_FLOGS_F32;

  if (HSP_HW_IF_SendCommand(hmw->hdriver, cmdid) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Scalar_Exported_Functions_Group5
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform the conversion of Q31 integers to float32
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaQ312F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_Q312F) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of float32 integers to Q31
  *        of a real scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaF2Q31(hsp_engine_context_t *hmw, float32_t *inBuff, int32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_F2Q31) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of 24-bit signed integer
  *        to float32 of a scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Sca24S2F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_24S2F) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of 32-bit unsigned integer
  *        into float32 of a scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaU2F(hsp_engine_context_t *hmw, uint32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_U2F) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of float32 into unsigned 32-bit
  *        of a scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaF2U(hsp_engine_context_t *hmw, float32_t *inBuff, uint32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_F2U) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of signed 32-bit integer
  *         into float32 of a scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaI2F(hsp_engine_context_t *hmw, int32_t *inBuff, float32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_I2F) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the conversion of float32 into signed 32-bit integer
  *        of a scalar input
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaF2I(hsp_engine_context_t *hmw, float32_t *inBuff, int32_t *outBuff, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) inBuff, &ouAddr1,
                               (uint32_t) outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_F2I) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to set a scalar with an unsigned 32-bit value
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer value
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSet_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SET) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the set value for a scalar input
  * @param hmw        HSP middleware context
  * @param inBuff     Input Buffer value
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaSet_f32(hsp_engine_context_t *hmw, float32_t *inBuff, float32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1,
                               (uint32_t)outBuff, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SET) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the set value for a scalar input
  *        with a real immediate value
  * @param hmw        HSP middleware context
  * @param valueToSet Immediate value to set
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaISet_f32(hsp_engine_context_t *hmw, float32_t valueToSet, float32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  hsp_u32_f32_t val;

  val.f32 = valueToSet;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_0), &encoded, (uint32_t)&valueToSet, NULL,
                               (uint32_t)outBuff, &ouAddr1, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SET) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the set value for a scalar input
  *        with an immediate unsigned 32-bit value
  * @param hmw        HSP middleware context
  * @param valueToSet Immediate value to set
  * @param outBuff    Output Buffer address
  * @param ioType     User iotype information
  * @retval           Core status.
  */
hsp_core_status_t HSP_SEQ_ScaISet_u32(hsp_engine_context_t *hmw, uint32_t valueToSet, uint32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_0), &encoded, (uint32_t)&valueToSet, NULL,
                               (uint32_t)outBuff, &ouAddr1, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, valueToSet);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SET) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to write a scalar value into a vector to build up a vector
  * @param hmw         HSP middleware context
  * @param pSrc        Input scalar Buffer address
  * @param pIdx        Current index to start writing
  * @param pDst        Output vector Buffer address
  * @param nbSamples   Number of samples to proceed
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Sca2Vect_f32(hsp_engine_context_t *hmw, float32_t *pSrc, uint32_t *pIdx, float32_t *pDst,
                                       uint32_t nbSamples, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* Check command size */
  HSP_ASSERT_DBG_PARAM(nbSamples > 0U);

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)pSrc, &ouAddr1, (uint32_t)pIdx, &ouAddr2,
                               (uint32_t)pDst, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  /* Reset current index */
  *pIdx = 0;
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, nbSamples);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA2VECT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
/**
  * @}
  */

/** @addtogroup HSP_Scalar_Exported_Functions_Group6
  * @{
  */
/**
  * @brief Add a function to the current processing list to perform the shift with an immediate scalar value
  *        of an integer.
  *        If iShift is positive, then the integer is shifted to the left.
  *        If iShift is negative then the integer is shifted to the right.
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param iShift      Immediate value of shift
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIShift_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t iShift, int32_t *outBuff,
                                        uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, (uint32_t)iShift);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SHIFT_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the shift of an integer with an scalar value.
  *        If pShift is positive, then the integer is shifted to the left.
  *        If pShift is negative then the integer is shifted to the right.
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param pShift      Input Value Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaShift_i32(hsp_engine_context_t *hmw, int32_t *inBuff, int32_t *pShift, int32_t *outBuff,
                                       uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)pShift, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_SHIFT_I32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise AND between
  *        an 32-bit unsigned integer and an immediate 32-bit unsigned value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Immediate value to and
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIAnd_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_AND_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise AND between
  *        two 32-bit unsigned integers
  * @param hmw        HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffAnd   Input Buffer value to and
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaAnd_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffAnd,
                                     uint32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffAnd, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_AND_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise OR between
  *        an 32-bit unsigned integer and an immediate 32-bit unsigned value
  * @param hmw        HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Immediate value to or
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIOr_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_OR_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise OR between
  *        two 32-bit unsigned integers
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffOr    Input Buffer value to or
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaOr_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffOr, uint32_t *outBuff,
                                    uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffOr, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_OR_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise XOR between
  *        an 32-bit unsigned integer and an immediate 32-bit unsigned value
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param imm         Immediate value to or
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIXor_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t imm, uint32_t *outBuff,
                                      uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)inBuff, &ouAddr1, 0U, NULL,
                               (uint32_t)outBuff, &ouAddr2, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_XOR_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise OR between
  *        two 32-bit unsigned integers
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param inBuffXor   Input Buffer value to xor
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaXor_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *inBuffXor,
                                     uint32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)inBuffXor, &ouAddr2,
                               (uint32_t)outBuff, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_XOR_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the logical bitwise NOT of 32-bit unsigned integer
  * @param hmw         HSP middleware context
  * @param inBuff      Input Buffer address
  * @param outBuff     Output Buffer address
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaNot_u32(hsp_engine_context_t *hmw, uint32_t *inBuff, uint32_t *outBuff,
                                     uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t)inBuff, &ouAddr1, (uint32_t)outBuff, &ouAddr2,
                               0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_NOT_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/** @addtogroup HSP_Scalar_Exported_Functions_Group7
  * @{
  */
/**
  * @brief Add a function to the current processing list to get HSP saturation flag
  * @param hmw         HSP middleware context
  * @param pDst        Result flag (uint32_t) address (0x40 for saturation)
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_GetSatFlag(hsp_engine_context_t *hmw, uint32_t *pDst, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pDst, &ouAddr,
                               (uint32_t) pDst, &ouAddr, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_GET_SATF) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to clear HSP saturation flag
  * @param hmw         HSP middleware context
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ClrSatFlag(hsp_engine_context_t *hmw)
{
  uint32_t encoded = 0U;
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CLR_SATF) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to add an immediate value to a global counter
  * @param hmw         HSP middleware context
  * @param cntIdx      Counter index [0,15]
  * @param val         Value to add to counter
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CounterAdd(hsp_engine_context_t *hmw, uint8_t cntIdx, uint32_t val)
{
  if (val > HSP_CMD_COUNT_MAX_NB_COUNTIF)
  {
    /* max counter number is reached */
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, cntIdx);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, HSP_CMD_COUNT_MODE_ADD);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_COUNT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to set an immediate value to a global counter
  * @param hmw         HSP middleware context
  * @param cntIdx      Counter index [0,15]
  * @param val         Value to set to counter
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_CounterSet(hsp_engine_context_t *hmw, uint8_t cntIdx, uint32_t val)
{
  if (val > HSP_CMD_COUNT_MAX_NB_COUNTIF)
  {
    /* max counter number is reached */
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, cntIdx);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, val);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, HSP_CMD_COUNT_MODE_SET);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_COUNT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to send an event to trig a processing list
  * @param hmw           HSP middleware context
  * @param listNumber    Processing list number
  * @param itf           Event generated by HDEG (0) or HSEG (1) interface
  * @retval              Core status.
  */
hsp_core_status_t HSP_SEQ_SendEvt(hsp_engine_context_t *hmw, uint32_t listNumber, uint32_t itf)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, listNumber);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, itf);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SEND_EVT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to set a flag in processing list
  * @param hmw          HSP middleware context
  * @param flag         Flag index
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_SetFlag(hsp_engine_context_t *hmw, uint32_t flag)
{
  if (flag > 31U)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, flag);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SET_FLAG) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to generate a trigger pulse on one of
  *        the hsp_trg_out[3:0] signals by simply
  * writing into the HSP_HYP_TRGOR register
  * @param hmw          HSP middleware context
  * @param val          Integer value to set (must be in [1,15])
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_SetTrgo(hsp_engine_context_t *hmw, uint32_t val)
{
  if ((val == 0U) || (val > 15U))
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, val);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SET_TRGO) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to control the level of signal of a GPO
  * @param hmw          HSP middleware context
  * @param fieldMask    Mask indicating which bits must be checked
  * @param fieldVal     Field value
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_SetGpo(hsp_engine_context_t *hmw, uint32_t fieldMask, uint32_t fieldVal)
{
  fieldVal = ((fieldVal & fieldMask) << 16U); /* Put on the GPO field place */
  fieldMask = fieldMask << 16U; /* Put on the GPO field place */
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, fieldMask);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, fieldVal);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SET_GPO) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to check the content of a bitfield to a reference value
  *        and waits until the condition is false or true. A timeout value forces the function to exit the loop
  * @param hmw          HSP middleware context
  * @param regBuff      Register Buffer address
  * @param fieldMask    Mask indicating which bits must be checked
  * @param fieldVal     Field value
  * @param diffCond     Define the flag position to be set in case of timout (0 for equal, 1 for diff)
  * @param timeout      Timeout value
  * @param timeoutFl    Timeout index
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_WaitCond(hsp_engine_context_t *hmw, uint32_t *regBuff, uint32_t fieldMask,
                                   uint32_t fieldVal, uint32_t diffCond, uint32_t timeout, uint32_t timeoutFl)
{
  uint32_t encoded = 0U;
  uint32_t ioType = 0U;
  uint32_t ouAddr1;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) regBuff, &ouAddr1,
                               0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, fieldMask);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, fieldVal);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, timeoutFl);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, diffCond);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, timeout);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_WAIT_COND) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

#if defined(__HSP_DMA__)
/**
  * @brief Add a function to the current processing list to select the ADC source and ADC channel filter
  * @param hmw          HSP middleware context
  * @param pAdcBufNb    Pointer on ADC data buffer number
  * @param pAdcSrc      Pointer on ADC source number
  * @param pAdcFlt      Pointer on ADC channel filter
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_SetAdcSrc(hsp_engine_context_t *hmw, uint32_t *pAdcBufNb, uint32_t *pAdcSrc,
                                    uint32_t *pAdcFlt, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t ouAddr2;
  uint32_t ouAddr3;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pAdcBufNb, &ouAddr1, (uint32_t) pAdcSrc, &ouAddr2,
                               (uint32_t) pAdcFlt, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SET_ADCSRC) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the CRC32 computation of CROM and
  *        DROM.
  * @param hmw            HSP middleware context
  * @param pState         Pointer of memory where pState[0] = CRC pState[1] = offset
  * @param pCRC           Pointer on crc computed
  * @param blockSize      Size of block
  * @param posFEOR        Position of the flag indicating when CRC function reaches the end of the ROM.
  * @param posFEOB        Position of the flag indicating when CRC function reaches the end of data block.
  * @param memType        HSP_CRC_CROM: to run CRC on CROM,
  *                       HSP_CRC_DROM: to run CRC on DROM
  * @param ioType         User iotype information
  * @retval               Core status.
  */
hsp_core_status_t HSP_SEQ_Crc32(hsp_engine_context_t *hmw, uint32_t *pState,  uint32_t *pCRC,  uint32_t blockSize,
                                uint32_t posFEOR, uint32_t posFEOB, hsp_crc_mem_type_cmd_t memType, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pState, &ouAddr1,
                               (uint32_t) pCRC, &ouAddr2, 0U, NULL, 2U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, blockSize);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, memType);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, posFEOR);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM5, posFEOB);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CRC32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @brief Add a function to the current processing list to set the specified bitfield of a scalar to the specified value
  * @param hmw          HSP middleware context
  * @param regBuff      Register Buffer address
  * @param fieldMask    Mask indicating which bits must be checked
  * @param fieldVal     Field value
  * @retval             Core status.
  */
hsp_core_status_t HSP_SEQ_SetBits(hsp_engine_context_t *hmw, uint32_t *regBuff, uint32_t fieldMask, uint32_t fieldVal)
{
  uint32_t encoded = 0U;
  uint32_t ioType = 0U;
  uint32_t ouAddr1;

  fieldVal &= fieldMask;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) regBuff, &ouAddr1,
                               0U, NULL, 0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, fieldMask);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, fieldVal);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SET_BITS) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform the addition of an immediate value to content
  *        of address with modulus.
  * @param hmw         HSP middleware context
  * @param addr        Input Buffer address (uint32_t)
  * @param imm         Immediate value to add (uint32_t)
  * @param base        Base used for modulo (uint32_t)
  * @param size        Size used for modulo (uint32_t)
  * @param ioType      User iotype information
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_ScaIModAdd_u32(hsp_engine_context_t *hmw, uint32_t *addr, uint32_t imm, uint32_t base,
                                         uint32_t size, uint32_t ioType)
{
  HSP_ASSERT_DBG_PARAM(size > 1U);

  uint32_t encoded = 0U;
  uint32_t ouAddr1;
  uint32_t endAddr;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_BuildParam(hmw, (ioType | HSP_SEQ_IOTYPE_IMM_1), &encoded, (uint32_t)addr, &ouAddr1, 0U, NULL,
                               0U, NULL, 1U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  endAddr = base;
  size = size * 4U;
  imm = imm * 4U;
  endAddr += size;

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, imm);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, endAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, size);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_SCA_MADD_U32) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Add a function to the current processing list to perform a comparison of an input number
  *        to two thresholds (LoLim, HiLim).
  *        If a number of consecutive elements (given by MaxCntHi) are greater than HiLim, pDst is set to +1
  *        If a number of consecutive elements (given by MaxCntLo) are less than LoLim, pDst is set to -1
  *        If none of the previous conditions are met, pDst is set to 0
  * @param hmw          HSP middleware context
  * @param pSrc         Input value address (float)
  * @param pDst         Res value address (int32_t)
  * @param pLim         Pointer on comparison limit [LoLim, HiLim] (float)
  * @param pMax         Pointer on max comparison limit [MaxCntLo, MaxCntHi] (uint32_t)
  * @param pCnt         Pointer on counter struct [locnt, hicnt] (uint32_t)
  * @param ioType       User iotype information
  * @retval             Core status
  */
hsp_core_status_t HSP_SEQ_ScaCmpCnt_f32(hsp_engine_context_t *hmw, float32_t *pSrc, int32_t *pDst,
                                        hsp_cmp_cnt_lim_t *pLim, hsp_cmp_cnt_cnt_t *pMax,
                                        hsp_cmp_cnt_cnt_t *pCnt, uint32_t ioType)
{
  uint32_t encoded = 0U;
  uint32_t ouAddr1, ouAddr2, ouAddr3;
  uint32_t ouAddr4, ouAddr5;

  if (HSP_UTILITIES_BuildParam(hmw, ioType, &encoded, (uint32_t) pSrc, &ouAddr1,
                               (uint32_t) pDst, &ouAddr2, (uint32_t) pLim, &ouAddr3, 3U) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) pMax, &ouAddr4) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t) pCnt, &ouAddr5) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr3);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, ouAddr4);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM4, ouAddr5);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, encoded);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_CMPCNT_F32) != HSP_IF_OK)
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
