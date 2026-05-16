/**
  ******************************************************************************
  * @file    hsp_utilities.c
  * @brief   utilities for HSP
  *          This file provides a set of functions to manage address for HSP
  *          programming
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
#include "hsp_utilities.h"
#include "hsp_hw_if.h"
#include "hsp_bram_if.h"
#include "hsp_conf.h"
#include "hsp_if_conf.h"
#include "hsp_fw_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_INTERFACES
  * @{
  */

/** @addtogroup HSP_INTERFACE_UTILITIES
  * @{
  */
/* Private variables ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup HSP_INTERFACE_UTILITIES_Private_Constants HSP Interface Utilities Private Constants
  * @{
  */
#if defined(__HSP_DMA__)
#define UTILITIES_USE_AP_PARAM0  0x000F0000
#define UTILITIES_USE_AP_PARAM1  0x00F00000
#define UTILITIES_USE_AP_PARAM2  0x0F000000
#endif /* __HSP_DMA__ */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup HSP_UTILITIES_Private_Functions HSP_UTILITIES Private Functions
  * @{
  */
#if defined(__HSP_DMA__)
hsp_if_status_t HSP_UTILITIES_ToAPddress(hsp_engine_context_t *hmw, uint32_t inIoType, uint32_t inAddr,
                                         uint32_t *ouAddr,
                                         uint32_t pix);
#endif /* __HSP_DMA__ */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_UTILITIES_Exported_Functions HSP_UTILITIES Exported Functions
  * @{
  */
/**
  * @brief  Translate host address in HSP address.
  * @param hmw     HSP middleware context.
  * @param addr    Address to be translate
  * @param ouAddr  Translated address
  * @retval        HSP_IF_OK   translation success
  * @retval        HSP_IF_ERROR  translation failure
  */
hsp_if_status_t HSP_UTILITIES_ToBramABAddress(hsp_engine_context_t *hmw, uint32_t addr, uint32_t *ouAddr)
{
  hsp_bram_handle_t *hhsp_bram = &hmw->hbram;
  if ((addr >= hhsp_bram->baseSharedAddr) && (addr <= hhsp_bram->topSharedAddr))
  {
    /* Calculate HSP shared memory address */
    *ouAddr = (uint32_t)((int32_t)hhsp_bram->bramOffset + (int32_t)addr);
    return HSP_IF_OK;
  }
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
  if (addr == (uint32_t)HSP_HW_IF_SEQ_STREAM_GetBufferAddress(hmw->hdriver, 0U))
  {
    *ouAddr = (uint32_t)HSP_REG_SPE_BUFF0DR;
    return HSP_IF_OK;
  }
  if (addr == (uint32_t)HSP_HW_IF_SEQ_STREAM_GetBufferAddress(hmw->hdriver, 1U))
  {
    *ouAddr = (uint32_t)HSP_REG_SPE_BUFF1DR;
    return HSP_IF_OK;
  }
  if (addr == (uint32_t)HSP_HW_IF_SEQ_STREAM_GetBufferAddress(hmw->hdriver, 2U))
  {
    *ouAddr = (uint32_t)HSP_REG_SPE_BUFF2DR;
    return HSP_IF_OK;
  }
  if (addr == (uint32_t)HSP_HW_IF_SEQ_STREAM_GetBufferAddress(hmw->hdriver, 3U))
  {
    *ouAddr = (uint32_t)HSP_REG_SPE_BUFF3DR;
    return HSP_IF_OK;
  }
#endif /* USE_HSP_ENGINE_STREAM_BUFFER*/
  return HSP_IF_ERROR;
}

/**
  * @brief  Translate host address in HSP address
  * @param hmw       HSP middleware context.
  * @param inIoType  User iotype information
  * @param ouIoType  Internal iotype
  * @param inAddr0   First address to be check and translate
  * @param ouAddr0   Translated address
  * @param inAddr1   Second address to be check and translate
  * @param ouAddr1   Translated address
  * @param inAddr2   Third address to be check and translate
  * @param ouAddr2   Translated address
  * @param nbParam   Number of address to translate (1 or 2 or 3)
  * @retval          HSP_IF_OK   translation success
  * @retval          HSP_IF_ERROR  translation failure
  */
hsp_if_status_t HSP_UTILITIES_BuildParam(hsp_engine_context_t *hmw,
                                         uint32_t inIoType, uint32_t *ouIoType,
                                         uint32_t inAddr0, uint32_t *ouAddr0,
                                         uint32_t inAddr1, uint32_t *ouAddr1,
                                         uint32_t inAddr2, uint32_t *ouAddr2,
                                         uint32_t nbParam)
{
  if ((nbParam == 0) || (nbParam > 3))
  {
    return HSP_IF_ERROR;
  }

  *ouIoType = 0;

  /* inAddr0 */
  if ((inIoType & HSP_SEQ_IOTYPE_IMM_0) == HSP_SEQ_IOTYPE_IMM_0)
  {
    *ouIoType |= HSP_IOTYPE_IMM;
  }
  else
  {
    if ((inIoType & HSP_SEQ_IOTYPE_POP_0) == HSP_SEQ_IOTYPE_POP_0)
    {
      *ouIoType |= HSP_IOTYPE_POP0;
    }
#if defined(__HSP_DMA__)
    if ((inIoType & HSP_SEQ_IOTYPE_EXT_0) == HSP_SEQ_IOTYPE_EXT_0)
    {
      *ouAddr0 = inAddr0;
      *ouIoType |= HSP_IOTYPE_EXT0;
    }
    else
    {
      if (inIoType & UTILITIES_USE_AP_PARAM0)
      {
        /* Calculate offset in buffer */
        if (HSP_UTILITIES_ToAPddress(hmw, inIoType, inAddr0, ouAddr0, UTILITIES_USE_AP_PARAM0) == HSP_IF_ERROR)
        {
          return HSP_IF_ERROR;
        }
      }
      else
      {
        if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr0, ouAddr0) == HSP_IF_ERROR)
        {
          return HSP_IF_ERROR;
        }
      }
    }
#endif /* __HSP_DMA__ */
    if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr0, ouAddr0) == HSP_IF_ERROR)
    {
      return HSP_IF_ERROR;
    }
  }
  /* inAddr1 */
  if (nbParam >= 2)
  {
    if ((inIoType & HSP_SEQ_IOTYPE_IMM_1) == HSP_SEQ_IOTYPE_IMM_1)
    {
      /* No conversion required */
      *ouIoType |= HSP_IOTYPE_IMM;
    }
    else
    {
      if ((inIoType & HSP_SEQ_IOTYPE_POP_1) == HSP_SEQ_IOTYPE_POP_1)
      {
        *ouIoType |= HSP_IOTYPE_POP1;
      }
#if defined(__HSP_DMA__)
      if ((inIoType & HSP_SEQ_IOTYPE_EXT_1) == HSP_SEQ_IOTYPE_EXT_1)
      {
        *ouAddr1 = inAddr1;
        *ouIoType |= HSP_IOTYPE_EXT1;
      }
      else
      {
        if (inIoType & UTILITIES_USE_AP_PARAM1)
        {
          /* Calculate offset in buffer */
          if (HSP_UTILITIES_ToAPddress(hmw, inIoType, inAddr1, ouAddr1, UTILITIES_USE_AP_PARAM1) == HSP_IF_ERROR)
          {
            return HSP_IF_ERROR;
          }
        }
        else
        {
          if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr1, ouAddr1) == HSP_IF_ERROR)
          {
            return HSP_IF_ERROR;
          }
        }
      }
#endif /* __HSP_DMA__ */
      if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr1, ouAddr1) == HSP_IF_ERROR)
      {
        return HSP_IF_ERROR;
      }
    }
  }
  /* inAddr2 */
  if (nbParam >= 3)
  {
    if ((inIoType & HSP_SEQ_IOTYPE_POP_2) == HSP_SEQ_IOTYPE_POP_2)
    {
      *ouIoType |= HSP_IOTYPE_POP2;
    }
#if defined(__HSP_DMA__)
    if ((inIoType & HSP_SEQ_IOTYPE_EXT_2) == HSP_SEQ_IOTYPE_EXT_2)
    {
      *ouAddr2 = inAddr2;
      *ouIoType |= HSP_IOTYPE_EXT2;
    }
    else
    {
      if (inIoType & UTILITIES_USE_AP_PARAM2)
      {
        /* Calculate offset in buffer */
        if (HSP_UTILITIES_ToAPddress(hmw, inIoType, inAddr2, ouAddr2, UTILITIES_USE_AP_PARAM2) == HSP_IF_ERROR)
        {
          return HSP_IF_ERROR;
        }
      }
      else
      {
        if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr2, ouAddr2) == HSP_IF_ERROR)
        {
          return HSP_IF_ERROR;
        }
      }
    }
#endif /* __HSP_DMA__ */
    if (HSP_UTILITIES_ToBramABAddress(hmw, inAddr2, ouAddr2) == HSP_IF_ERROR)
    {
      return HSP_IF_ERROR;
    }
  }
  return HSP_IF_OK;
}

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup HSP_UTILITIES_Private_Functions
  * @{
  */
#if defined(__HSP_DMA__)
hsp_if_status_t HSP_UTILITIES_ToAPddress(hsp_engine_context_t *hmw, uint32_t inIoType, uint32_t inAddr,
                                         uint32_t *ouAddr,
                                         uint32_t pix)
{
  HSP_TypeDef *p_hspx = (HSP_TypeDef *)(((hal_hsp_handle_t *)(hmw->hdriver))->instance);

  if (pix == UTILITIES_USE_AP_PARAM0)
  {
    if ((inIoType & UTILITIES_USE_AP_PARAM0) == HSP_SEQ_IOTYPE_AP_0_0)
    {
      *ouAddr = HSP_STI_AP0_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[0]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM0) == HSP_SEQ_IOTYPE_AP_0_1)
    {
      *ouAddr = HSP_STI_AP1_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[1]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM0) == HSP_SEQ_IOTYPE_AP_0_2)
    {
      *ouAddr = HSP_STI_AP2_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[2]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM0) == HSP_SEQ_IOTYPE_AP_0_3)
    {
      *ouAddr = HSP_STI_AP3_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[3]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM0) == HSP_SEQ_IOTYPE_AP_0_4)
    {
      *ouAddr = HSP_STI_AP4_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[4]));
    }
    else
    {
      return HSP_IF_ERROR;
    }
  }
  else if (pix == UTILITIES_USE_AP_PARAM1)
  {
    if ((inIoType & UTILITIES_USE_AP_PARAM1) == HSP_SEQ_IOTYPE_AP_1_0)
    {
      *ouAddr = HSP_STI_AP0_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[0]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM1) == HSP_SEQ_IOTYPE_AP_1_1)
    {
      *ouAddr = HSP_STI_AP1_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[1]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM1) == HSP_SEQ_IOTYPE_AP_1_2)
    {
      *ouAddr = HSP_STI_AP2_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[2]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM1) == HSP_SEQ_IOTYPE_AP_1_3)
    {
      *ouAddr = HSP_STI_AP3_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[3]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM1) == HSP_SEQ_IOTYPE_AP_1_4)
    {
      *ouAddr = HSP_STI_AP4_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[4]));
    }
    else
    {
      return HSP_IF_ERROR;
    }
  }
  else if (pix == UTILITIES_USE_AP_PARAM2)
  {
    if ((inIoType & UTILITIES_USE_AP_PARAM2) == HSP_SEQ_IOTYPE_AP_2_0)
    {
      *ouAddr = HSP_STI_AP0_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[0]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM2) == HSP_SEQ_IOTYPE_AP_2_1)
    {
      *ouAddr = HSP_STI_AP1_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[1]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM2) == HSP_SEQ_IOTYPE_AP_2_2)
    {
      *ouAddr = HSP_STI_AP2_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[2]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM2) == HSP_SEQ_IOTYPE_AP_2_3)
    {
      *ouAddr = HSP_STI_AP3_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[3]));
    }
    else if ((inIoType & UTILITIES_USE_AP_PARAM2) == HSP_SEQ_IOTYPE_AP_2_4)
    {
      *ouAddr = HSP_STI_AP4_BUFF_ADDR + (inAddr - (p_hspx->STIAPADDR[4]));
    }
    else
    {
      return HSP_IF_ERROR;
    }
  }
  else
  {
    return HSP_IF_ERROR;
  }
  return HSP_IF_OK;
}
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
