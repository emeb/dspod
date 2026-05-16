/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   app_hsp_engine_process_template.c
  * @brief  This file implements the HSP_Engine Process functions
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

#include "main.h"
#include "app_hsp_engine_process.h"
#include "app_hsp_engine_seq.h"
#include "app_hsp_bram_alloc.h"

#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* Add #include related to the HSP Modules required by the Application */
/*
  ex: "#include "hsp_direct_command.h" for processing by HSP Direct Command
  ex: "#include "hsp_dsp.h" for processing by HSP DSP functions
  ...
 */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* Application functions prototypes *******/
extern void Error_Handler(void);
extern hsp_core_handle_t hmw;
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief Run Biquad Cascade DF1 filter with HSP Engine
  * @param None
  * @retval 0 if success, other failed
  */
/*uint32_t APP_HSP_Engine_Process_Direct_BiquadCascadeDF1(void)
{
  ...
  return 0;
}
*/

/* USER CODE END 0 */
