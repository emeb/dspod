/**
  ******************************************************************************
  * @file hsp_private_def.h
  * @brief Private defines for STM32 HSP DSP function
  *
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
#ifndef HSP_PRIVATE_DEF_H
#define HSP_PRIVATE_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @defgroup HSP_MODULES  HSP Modules
  * @details
  * This section includes all functions and definitions related to the processing operations available
  * in HSP.
  * @{
  */

/* Private defines -----------------------------------------------------------*/
/** @defgroup HSP_MODULES_Private_Defines  HSP Modules Private Defines
  * @{
  */
#define HSP_EXP10_VAL_A           3.32192802429199218750f
#define HSP_EXPE_VAL_A         1.44269502162933349609375f
#define HSP_LOG10_VAL_A     0.43429449200630187988281250f
#define HSP_LOG10_VAL_B           0.69314718246459960937f
#define HSP_LN_VAL_A                                 1.0f
#define HSP_LN_VAL_B              0.69314718246459960937f
#define HSP_FEXPE_VAL_A                1.442695040888963f
#define HSP_FEXPE_VAL_B                        0.3397100f
#define HSP_FEXPE_VAL_C                        8388608.0f
#define HSP_FEXPE_VAL_D                            127.0f
#define HSP_FEXP2_VAL_A                              1.0f
#define HSP_FEXP2_VAL_B                        0.3397100f
#define HSP_FEXP2_VAL_C                        8388608.0f
#define HSP_FEXP2_VAL_D                            127.0f
#define HSP_FEXP10_VAL_A               3.321928094887362f
#define HSP_FEXP10_VAL_B                       0.3397100f
#define HSP_FEXP10_VAL_C                       8388608.0f
#define HSP_FEXP10_VAL_D                           127.0f
#define HSP_FEXPDB_VAL_A               0.166096404744368f
#define HSP_FEXPDB_VAL_B                       0.3397100f
#define HSP_FEXPDB_VAL_C                       8388608.0f
#define HSP_FEXPDB_VAL_D                           127.0f
#define HSP_FLN_VAL_A           0.0000001192092895507812f
#define HSP_FLN_VAL_B                             -127.0f
#define HSP_FLN_VAL_C                           0.346607f
#define HSP_FLN_VAL_D                  0.693147180559945f
#define HSP_FLOG10_VAL_A         .0000001192092895507812f
#define HSP_FLOG10_VAL_B                          -127.0f
#define HSP_FLOG10_VAL_C                        0.346607f
#define HSP_FLOG10_VAL_D               0.301029995663981f
#define HSP_FLOG2_VAL_A         0.0000001192092895507812f
#define HSP_FLOG2_VAL_B                           -127.0f
#define HSP_FLOG2_VAL_C                         0.346607f
#define HSP_FLOG2_VAL_D                              1.0f
#define HSP_FLOGDB_VAL_A        0.0000001192092895507812f
#define HSP_FLOGDB_VAL_B                          -127.0f
#define HSP_FLOGDB_VAL_C                        0.346607f
#define HSP_FLOGDB_VAL_D               6.020599913279624f
#define HSP_VAL_FLOAT_NULL                           0.0f

/**
  * @}
  */

/* Private Types -------------------------------------------------------------*/
/** @defgroup HSP_MODULES_Private_Types  HSP Modules Private Types
  * @{
  */
typedef union
{
  uint32_t u32;
  float32_t f32;
} hsp_u32_f32_t;

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* HSP_PRIVATE_DEF_H */
