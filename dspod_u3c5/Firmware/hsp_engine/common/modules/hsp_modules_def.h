/**
  ******************************************************************************
  * @file    hsp_modules_def.h
  * @author  MCD Application Team
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
#ifndef HSP_MODULES_DEF_H
#define HSP_MODULES_DEF_H


#include "hsp_conf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#if defined(USE_HSP_MEMCPY) && (USE_HSP_MEMCPY == 1)
#include "hsp_memcpy.h"
#endif /* USE_HSP_MEMCPY */

#if defined(USE_HSP_MODULES_DIRECT_LIB) && (USE_HSP_MODULES_DIRECT_LIB == 1)
#include "hsp_direct_command.h"
#endif /* USE_HSP_MODULES_DIRECT_LIB */

#if defined(USE_HSP_MODULES_DSP_LIB) && (USE_HSP_MODULES_DSP_LIB == 1)
#include "hsp_dsp.h"
#endif /* USE_HSP_MODULES_DSP_LIB */

#if defined(USE_HSP_MODULES_CNN_LIB) && (USE_HSP_MODULES_CNN_LIB == 1)
#include "hsp_cnn.h"
#endif /* USE_HSP_MODULES_CNN_LIB */

#if defined(USE_HSP_MODULES_PROCLIST_COMPLEX) && (USE_HSP_MODULES_PROCLIST_COMPLEX == 1)
#include "hsp_proclist_complex.h"
#endif /* USE_HSP_MODULES_PROCLIST_COMPLEX */

#if defined(USE_HSP_MODULES_PROCLIST_CONDITIONAL) && (USE_HSP_MODULES_PROCLIST_CONDITIONAL == 1)
#include "hsp_proclist_conditional.h"
#endif /* USE_HSP_MODULES_PROCLIST_CONDITIONAL */

#if defined(USE_HSP_MODULES_PROCLIST_FILTER) && (USE_HSP_MODULES_PROCLIST_FILTER == 1)
#include "hsp_proclist_filter.h"
#endif /* USE_HSP_MODULES_PROCLIST_FILTER */

#if defined(USE_HSP_MODULES_PROCLIST_MATRIX) && (USE_HSP_MODULES_PROCLIST_MATRIX == 1)
#include "hsp_proclist_matrix.h"
#endif /* USE_HSP_MODULES_PROCLIST_MATRIX */

#if defined(USE_HSP_MODULES_PROCLIST_SCALAR) && (USE_HSP_MODULES_PROCLIST_SCALAR == 1)
#include "hsp_proclist_scalar.h"
#endif /* USE_HSP_MODULES_PROCLIST_SCALAR */

#if defined(USE_HSP_MODULES_PROCLIST_TRANSFORM) && (USE_HSP_MODULES_PROCLIST_TRANSFORM == 1)
#include "hsp_proclist_transform.h"
#endif /* USE_HSP_MODULES_PROCLIST_TRANSFORM */

#if defined(USE_HSP_MODULES_PROCLIST_VECTOR) && (USE_HSP_MODULES_PROCLIST_VECTOR == 1)
#include "hsp_proclist_vector.h"
#endif /* USE_HSP_MODULES_PROCLIST_VECTOR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_MODULES_DEF_H */
