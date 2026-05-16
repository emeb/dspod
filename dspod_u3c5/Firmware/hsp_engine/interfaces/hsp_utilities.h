/**
  ******************************************************************************
  * @file    hsp_utilities.h
  * @brief   Header file for hsp_utilities.c.
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

#ifndef HSP_UTILITIES_H
#define HSP_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "interface.h"
#include "hsp_def.h"
#include <stdint.h>

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_INTERFACES HSP Interfaces
  * @{
  */

/** @defgroup HSP_INTERFACE_UTILITIES HSP Interface Utilities
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/* Exported types -------------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/** @defgroup HSP_UTILITIES_Exported_Functions HSP_UTILITIES Exported Functions
  * @{
  */
hsp_if_status_t HSP_UTILITIES_ToBramABAddress(hsp_engine_context_t *hmw, uint32_t addr, uint32_t *addrOut);
hsp_if_status_t HSP_UTILITIES_BuildParam(hsp_engine_context_t *hmw,
                                         uint32_t inIoType, uint32_t *ouIoType,
                                         uint32_t inAddr0, uint32_t *ouAddr0,
                                         uint32_t inAddr1, uint32_t *ouAddr1,
                                         uint32_t inAddr2, uint32_t *ouAddr2,
                                         uint32_t nbParam);

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

#endif /* HSP_UTILITIES_H */
