/**
  ******************************************************************************
  * @file    hsp.h
  * @brief   Header file
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
#ifndef HSP_H
#define HSP_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "hsp_conf.h"
#include "hsp_if_conf.h"
#include "hsp_core.h"
#include "hsp_bram.h"
#if (USE_HSP_MODULES_PROCLIST_COMPLEX == 1)   || (USE_HSP_MODULES_PROCLIST_CONDITIONAL == 1)  \
    || (USE_HSP_MODULES_PROCLIST_FILTER == 1)  || (USE_HSP_MODULES_PROCLIST_MATRIX == 1)      \
    || (USE_HSP_MODULES_PROCLIST_SCALAR == 1) || (USE_HSP_MODULES_PROCLIST_TRANSFORM ==1 )    \
    || (USE_HSP_MODULES_PROCLIST_VECTOR == 1)
#include "hsp_proclist.h"
#endif /* USE_HSP_MODULES_PROCLIST_xxx == 1 */
#include "hsp_modules_def.h"
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_H */
