/**
  ******************************************************************************
  * @file    hsp_memcpy.h
  * @author  MCD Application Team
  * @brief   Header file
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
#ifndef HSP_MEMCPY_H
#define HSP_MEMCPY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_CORE HSP Core
  * @{
  */

/** @defgroup HSP_UTILITIES  HSP Utilities
  * @{
  */

/** @defgroup HSP_MEMCPY_Exported_Functions HSP MEMCPY Exported Functions
  * @{
  */

void *stm32_hsp_memcpy(void *pDst, const void *pSrc, uint32_t blockSize);

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

#endif /* HSP_MEMCPY_H */
