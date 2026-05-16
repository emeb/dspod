/**
  ******************************************************************************
  * @file    interface.h
  * @brief   global Header file for all "interface" .c files .
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

#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hsp_api_def.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_INTERFACES
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  HSP_INTERFACES_Exported_Types HSP Interfaces Exported Types
  * @{
  */
typedef enum
{
  HSP_IF_OK,
  HSP_IF_ERROR
} hsp_if_status_t;

typedef enum
{
  HSP_IF_ACTIVATION_DISABLED = 0U,
  HSP_IF_ACTIVATION_ENABLED  = 1U
} hsp_if_activation_status_t;

/**
  * @brief  Filter state structure
  */
typedef struct
{
  uint32_t dirCmd;  /**< Direct command */
  uint32_t addrHsp; /**< State buffer address in HSP memory */
  float32_t *addr;  /**< Buffer address */
  float32_t *ptr;   /**< Current pointer position */
  uint32_t size;    /**< Contains information according filter type */
} hsp_hw_if_filter_state_t;

/**
  * @brief  FIR Decimate Filter state structure
  */
typedef struct
{
  uint32_t dirCmd;     /**< Direct command */
  uint32_t addrHsp;    /**< State buffer address in HSP memory */
  float32_t *addr;     /**< Buffer address */
  float32_t *ptr;      /**< Current pointer position */
  uint32_t size;       /**< Contains information according filter type */
  uint32_t firstLoop;  /**<  Number of iteration to compute state values */
  uint32_t secondLoop; /**< Number of iteration to compute remaining data */
} hsp_hw_if_fir_decimate_filter_state_t;

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

#endif /* INTERFACE_H */
