/**
  ******************************************************************************
  * @file    app_hsp_bram_allocate_template.c
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

#include "app_hsp_bram_allocate.h"
#include "hsp_bram.h"

/* Exported variables ------------------------------------------------------- */
/* BRAM Resources Static Allocation ----------------------------------------- */
/* Example of static allocations
static float32_t my_vect1[HSP_BRAM_RSRC_VECTOR1_SIZE] __attribute__((section("HSP_DATA_BRAM"))) = {0.1f, 1.2f, -0.2f, -1000.0f, -8502.f, -123.545f, 5.0f, .035456464f, 1.54564646f, 798.22545f};
static float32_t my_vect2[HSP_BRAM_RSRC_VECTOR2_SIZE] __attribute__((section("HSP_DATA_BRAM"))) = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static int32_t my_vect3[HSP_BRAM_RSRC_VECTOR3_SIZE] __attribute__((section("HSP_DATA_BRAM")));
*/

/* Private variables -------------------------------------------------------- */
static hsp_bram_resources_t g_bram_resources;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint32_t MX_HSP_BRAM_Allocate(hsp_core_handle_t *hmw)
{
  uint32_t error = 0UL;

  /* Example of code for static allocation */

  /* Example of code for dynamic allocation */
  /*
  g_bram_resources.fir_state_id = HSP_BRAM_MallocStateBuffer_Fir(hmw, HSP_BRAM_RSRC_FIR_STATE_TAPS_NBR, HSP_BRAM_RSRC_FIR_SAMPLES_IN_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.fir_state_id == 0UL) error++;

  g_bram_resources.p_buff_in = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_FIR_SAMPLES_IN_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_buff_in == NULL) error++;

  g_bram_resources.p_coef = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_FIR_STATE_TAPS_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_coef == NULL) error++;

  g_bram_resources.p_buff_out = (float32_t *)HSP_BRAM_Malloc(hmw, HSP_BRAM_RSRC_RESULTS_NBR, HSP_BRAM_ALLOCATION_DEFAULT);
  if (g_bram_resources.p_buff_out == NULL) error++;
  */

  return error;
}

