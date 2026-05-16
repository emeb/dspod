/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_hsp_bram_alloc_template.c
  * @brief   This file contains resources allocated in HSP BRAM region memory
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
/* USER CODE END Header */

#include "app_hsp_bram_alloc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Exported variables ------------------------------------------------------- */
/* BRAM Resources Static Allocation ----------------------------------------- */
/* Example of static allocations
static float32_t my_vect1[VECTOR1_SIZE] __attribute__((section("HSP_DATA_BRAM"))) = {0.1f, 1.2f, -0.2f, -1000.0f, -8502.f, -123.545f, 5.0f, .035456464f, 1.54564646f, 798.22545f};
static float32_t my_vect2[VECTOR2_SIZE] __attribute__((section("HSP_DATA_BRAM"))) = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static int32_t my_vect3[VECTOR2_SIZE] __attribute__((section("HSP_DATA_BRAM")));
*/

/* USER CODE BEGIN BRAM Static Allocation */

/* USER CODE END BRAM Static Allocation */

/* BRAM Resources Dynamic Allocation ---------------------------------------- */
/* Example of dynamic allocations
hsp_filter_state_identifier_t fir_state_id;
hsp_filter_state_identifier_t biquad_state_id;

float32_t *p_buff_in;
float32_t *p_coef;
float32_t *p_buff_out;
*/

/* USER CODE BEGIN BRAM Dynamic Allocation */

/* USER CODE END BRAM Dynamic Allocation */

/* Access to external variables --------------------------------------------- */
extern hsp_core_handle_t hmw;

/* USER CODE BEGIN PV */
/* Private variables -------------------------------------------------------- */

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

uint32_t MX_HSP_BRAM_Allocation(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;

  /* Example of code for dynamic allocation */
  /*
  fir_state_id = HSP_BRAM_MallocStateBuffer_Fir(hmw, FIR_STATE_TAPS_NBR, FIR_SAMPLES_IN_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (fir_state_id == 0UL) error++;

  biquad_state_id = HSP_BRAM_MallocStateBuffer_BiquadCascadeDf1(hmw, FIR_STATE_TAPS_NBR, FIR_SAMPLES_IN_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (biquad_state_id == 0UL) error++;

  p_buff_in = (float32_t *)HSP_BRAM_Malloc(hmw, FIR_SAMPLES_IN_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (p_buff_in == NULL) error++;

  p_coef = (float32_t *)HSP_BRAM_Malloc(hmw, FIR_COEF_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (p_coef == NULL) error++;

  p_buff_out = (float32_t *)HSP_BRAM_Malloc(hmw, FIR_RESULTS_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (p_buff_out == NULL) error++;
  */

  /* USER CODE BEGIN BRAM_ALLOC */

  /* USER CODE END BRAM_ALLOC */

  return error;
}


/**
  * @}
  */

/**
  * @}
  */

