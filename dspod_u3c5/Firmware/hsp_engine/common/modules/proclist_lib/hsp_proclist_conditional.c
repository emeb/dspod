/**
  ******************************************************************************
  * @file    hsp_proclist_conditional.c
  * @brief   This file implements the HSP CONDITIONAL Processing functions
  *          used to record a processing list
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
#include "hsp_proclist_conditional.h"
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

/** @addtogroup HSP_MODULES_PROCLIST_CONDITIONAL_LIBRARY
  * @{
  */

/* Private defines -----------------------------------------------------------*/
/** @defgroup HSP_CONDITIONAL_Private_Defines HSP Conditional Private Defines
  * @{
  */
#define TYPE_FLOAT 0U
#define TYPE_UINT  1U
#define TYPE_SINT  2U
#define TYPE_ERROR 0xFFFFFFFFU

/**
  * @}
  */

/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup HSP_Conditional_Private_Functions HSP Conditional Private Functions
  * @{
  */
static uint32_t build_ifCmd(hsp_proclist_cond_cmd_t cmd, uint32_t typeOp);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_Conditional_Exported_Functions
  * @{
  */
/**
  * @brief IF command: add to the current processing list a conditional expression between a floating-point variable
  *        and an immediate floating-point value.
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Immediate value of condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfIFloat(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, float32_t *var1,
                                   float32_t var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr;
  hsp_u32_f32_t val;

  val.f32 = var2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if ((ouCmd = build_ifCmd(cmd, TYPE_FLOAT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, val.u32);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between a uint32 variable
  *        and a uint32 immediate value
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Immediate value of condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfIUint32(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, uint32_t *var1,
                                    uint32_t var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if ((ouCmd = build_ifCmd(cmd, TYPE_UINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, var2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between a int32 variable
  *        and a int32 immediate value
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Immediate value of condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfIInt32(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, int32_t *var1,  int32_t var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if ((ouCmd = build_ifCmd(cmd, TYPE_SINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, (uint32_t)var2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between 2 floating-point variables
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Pointer on second variable of the condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfFloat(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, float32_t *var1,
                                  float32_t *var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr1) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var2, &ouAddr2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if ((ouCmd = build_ifCmd(cmd, TYPE_FLOAT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between 2 unsigned int32 variables
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Pointer on second variable of the condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfUint32(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, uint32_t *var1,
                                   uint32_t *var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr1) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var2, &ouAddr2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if ((ouCmd = build_ifCmd(cmd, TYPE_UINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between 2 signed int32 variables
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var1        Pointer on first variable of the condition
  * @param var2        Pointer on second variable of the condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfInt32(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, int32_t *var1,  int32_t *var2)
{
  uint32_t ouCmd;;
  uint32_t ouAddr1;
  uint32_t ouAddr2;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var1, &ouAddr1) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var2, &ouAddr2) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }

  if ((ouCmd = build_ifCmd(cmd, TYPE_SINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, ouAddr1);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF command: add to the current processing list a conditional expression between a HSP global counter
  *        and a uint32 immediate value
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param idx         Global counter index (must be less than maximum number of global counter)
  * @param var2        Immediate uint32_t value for if condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfCnt(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, uint32_t idx,  uint32_t var2)
{
  uint32_t ouCmd;

  /* Check if maximal number of counter is reached */
  if (idx >= HSP_CMD_COUNT_MAX_NB_COUNTIF)
  {
    return HSP_CORE_ERROR;
  }
  if ((ouCmd = build_ifCmd(cmd, TYPE_UINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, idx);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, var2);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE_CNT) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF_ELSE command: add to the current processing list a command indicating "else" sequence
  * @param hmw         HSP middleware context.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfElse(hsp_engine_context_t *hmw)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_IFELSE);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief IF_ENDIF command: add to the current processing list a command closing current conditional "if" sequence
  * @param hmw         HSP middleware context.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfEndif(hsp_engine_context_t *hmw)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_IFENDIF);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief Loop command: add to the current processing list a command indicating Loop sequence
  * @param hmw         HSP middleware context.
  * @param count       Number of iteration.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_Loop(hsp_engine_context_t *hmw, uint32_t count)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_LOOPSTART);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM1, count);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief End loop command: add to the current processing list a command closing current LOOP sequence
  * @param hmw         HSP middleware context.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_LoopEnd(hsp_engine_context_t *hmw)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_LOOPEND);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief If loop command: add to the current processing list a conditional expression between implicit loop counter
  *        and variable
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param var         Pointer on variable of the condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfLoop(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, uint32_t *var)
{
  uint32_t ouCmd;;
  uint32_t ouAddr;

  /* First check and translate if necessary parameter */
  if (HSP_UTILITIES_ToBramABAddress(hmw, (uint32_t)var, &ouAddr) == HSP_IF_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  if ((ouCmd = build_ifCmd(cmd, TYPE_UINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, ouAddr);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief If Loop command: add to the current processing list a conditional expression between implicit loop counter
  *        and immediate value
  * @param hmw         HSP middleware context.
  * @param cmd         Type of condition:
  *                           - HSP_SEQ_CMP_IFEQ    : check equal
  *                           - HSP_SEQ_CMP_IFNE    : check not equal
  *                           - HSP_SEQ_CMP_IFGT    : check greater than
  *                           - HSP_SEQ_CMP_IFLT    : check less than
  *                           - HSP_SEQ_CMP_IFGE    : check greater equal
  *                           - HSP_SEQ_CMP_IFLE    : check less equal
  * @param val         Immediate value of the condition
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfILoop(hsp_engine_context_t *hmw, hsp_proclist_cond_cmd_t cmd, uint32_t val)
{
  uint32_t ouCmd;

  if ((ouCmd = build_ifCmd(cmd, TYPE_UINT)) == TYPE_ERROR)
  {
    return HSP_CORE_ERROR;
  }
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, ouCmd);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM2, val);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 1U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief if Loop command: add to the current processing list a conditional expression to check if implicit
  *        loop counter is odd
  * @param hmw         HSP middleware context.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfLoopOdd(hsp_engine_context_t *hmw)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_IFODD);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @brief if Loop command: add to the current processing list a conditional expression to check if implicit
  *        loop counter is even
  * @param hmw         HSP middleware context.
  * @retval            Core status.
  */
hsp_core_status_t HSP_SEQ_IfLoopEven(hsp_engine_context_t *hmw)
{
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM0, HSP_CMP_IFEVEN);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM3, 0U);
  HSP_HW_IF_WriteParameter(hmw->hdriver, HSP_HW_IF_PARAM15, 0U);

  if (HSP_HW_IF_SendCommand(hmw->hdriver, HSP_CMD_IFE_LOOP) != HSP_IF_OK)
  {
    return HSP_CORE_ERROR;
  }
  return HSP_CORE_OK;
}

/**
  * @}
  */

/**
  * @addtogroup HSP_Conditional_Private_Functions
  * @{
  */
/**
  * Build IF command according type of operand
  * return cmd otherwise TYPE_ERROR
  */
static uint32_t build_ifCmd(hsp_proclist_cond_cmd_t cmd, uint32_t typeOp)
{
  if (typeOp == TYPE_FLOAT)
  {
    switch (cmd)
    {
      case HSP_SEQ_CMP_IFEQ:
        return HSP_CMP_IFEQ_F32;
        break;
      case HSP_CMP_IFNE:
        return HSP_CMP_IFNE_F32;
        break;
      case HSP_SEQ_CMP_IFGT:
        return HSP_CMP_IFGT_F32;
        break;
      case HSP_SEQ_CMP_IFLT:
        return HSP_CMP_IFLT_F32;
        break;
      case HSP_SEQ_CMP_IFGE:
        return HSP_CMP_IFGE_F32;
        break;
      case HSP_CMP_IFLE:
        return HSP_CMP_IFLE_F32;
        break;
      default:
        return (TYPE_ERROR);
        break;
    }
  }
  else
  {
    if (typeOp == TYPE_UINT)
    {
      switch (cmd)
      {
        case HSP_SEQ_CMP_IFEQ:
          return HSP_CMP_IFEQ_INT;
          break;
        case HSP_CMP_IFNE:
          return HSP_CMP_IFNE_INT;
          break;
        case HSP_SEQ_CMP_IFGT:
          return HSP_CMP_IFGT_U32;
          break;
        case HSP_SEQ_CMP_IFLT:
          return HSP_CMP_IFLT_U32;
          break;
        case HSP_SEQ_CMP_IFGE:
          return HSP_CMP_IFGE_U32;
          break;
        case HSP_CMP_IFLE:
          return HSP_CMP_IFLE_U32;
          break;
        default:
          return (TYPE_ERROR);
          break;
      }
    }
    else
    {
      if (typeOp == TYPE_SINT)
      {
        switch (cmd)
        {
          case HSP_SEQ_CMP_IFEQ:
            return HSP_CMP_IFEQ_INT;
            break;
          case HSP_CMP_IFNE:
            return HSP_CMP_IFNE_INT;
            break;
          case HSP_SEQ_CMP_IFGT:
            return HSP_CMP_IFGT_I32;
            break;
          case HSP_SEQ_CMP_IFLT:
            return HSP_CMP_IFLT_I32;
            break;
          case HSP_SEQ_CMP_IFGE:
            return HSP_CMP_IFGE_I32;
            break;
          case HSP_CMP_IFLE:
            return HSP_CMP_IFLE_I32;
            break;
          default:
            return (TYPE_ERROR);
            break;
        }
      }
      else
      {
        return (TYPE_ERROR);
      }
    }
  }
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
