/**
  ******************************************************************************************
  * @file hsp_dsp.c
  * @brief API for HSP DSP functions
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

/* Includes ------------------------------------------------------------------*/
#include "hsp_dsp.h"
#include "hsp_bram.h"
#include "hsp_hw_if.h"
#include "hsp_utilities.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_DSP
  * @{
  */

/* Exported variables --------------------------------------------------------*/
/** @addtogroup HSP_MODULES_DSP_Exported_Variables
  * @{
  */
hsp_engine_context_t *stm32_cmsis_handle;

/**
  * @}
  */

/* Private defines -----------------------------------------------------------*/
/** @defgroup HSP_MODULES_DSP_Private_Defines HSP DSP Private Defines
  * @{
  */
#define HSP_FFT_LENGTH_32      32U
#define HSP_FFT_LENGTH_64      64U
#define HSP_FFT_LENGTH_128    128U
#define HSP_FFT_LENGTH_256    256U
#define HSP_FFT_LENGTH_512    512U
#define HSP_FFT_LENGTH_1024  1024U
#define HSP_FFT_LENGTH_2048  2048U
#define HSP_FFT_LENGTH_4096  4096U

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_MODULES_DSP_Exported_Functions
  * @{
  */

/** @addtogroup HSP_MODULES_DSP_Init_Functions
  * @{
  */
stm32_hsp_status stm32_hsp_init(hsp_engine_context_t *hmw)
{
  if (hmw == NULL)
  {
    return STM32_HSP_MATH_ARGUMENT_ERROR;
  }
  stm32_cmsis_handle = hmw;
  return STM32_HSP_MATH_SUCCESS;
}

/**
  * @brief Initialization function for the floating-point real FFT.
  * @param S            points to an arm_rfft_fast_instance_f32 structure
  * @param fftLen       length of the Real Sequence
  * @return execution status
  *                   -  STM32_HSP_MATH_SUCCESS        : Operation successful
  *                   -  STM32_HSP_MATH_ARGUMENT_ERROR : fftLen is not a supported length
  * @details Description
  *                   The parameter fftLen specifies the length of RFFT/CIFFT process.
  *                   Supported RFFT Lengths are 64, 128, 256, 512, 1024, 2048, 4096.
  */
stm32_hsp_status stm32_hsp_rfft_fast_init_f32(stm32_hsp_rfft_fast_instance_f32 *S, uint16_t fftLen)
{
  S->pTwiddleRFFT = NULL;
  S->Sint.pTwiddle = NULL;
  S->Sint.pBitRevTable = NULL;
  S->Sint.bitRevLength = 0;
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
  S->Sint.rearranged_twiddle_tab_stride1_arr = NULL;
  S->Sint.rearranged_twiddle_tab_stride2_arr = NULL;
  S->Sint.rearranged_twiddle_tab_stride3_arr = NULL;
  S->Sint.rearranged_twiddle_stride1 = NULL;
  S->Sint.rearranged_twiddle_stride2 = NULL;
  S->Sint.rearranged_twiddle_stride3 = NULL;
#endif /* ARM_MATH_MVEF */
  S->fftLenRFFT = fftLen;
  /* Set Calculate LOG2NBP according RFFT size */
  if (fftLen == HSP_FFT_LENGTH_64)
  {
    S->Sint.fftLen = HSP_LOG2NBP_64;
  }
  else if (fftLen == HSP_FFT_LENGTH_128)
  {
    S->Sint.fftLen = HSP_LOG2NBP_128;
  }
  else if (fftLen == HSP_FFT_LENGTH_256)
  {
    S->Sint.fftLen = HSP_LOG2NBP_256;
  }
  else if (fftLen == HSP_FFT_LENGTH_512)
  {
    S->Sint.fftLen = HSP_LOG2NBP_512;
  }
  else if (fftLen == HSP_FFT_LENGTH_1024)
  {
    S->Sint.fftLen = HSP_LOG2NBP_1024;
  }
  else if (fftLen == HSP_FFT_LENGTH_2048)
  {
    S->Sint.fftLen = HSP_LOG2NBP_2048;
  }
  else if (fftLen == HSP_FFT_LENGTH_4096)
  {
    S->Sint.fftLen = HSP_LOG2NBP_4096;
  }
  else
  {
    return STM32_HSP_MATH_ARGUMENT_ERROR;
  }
  return STM32_HSP_MATH_SUCCESS;
}

/**
  * @brief Initialization function for the cfft f32 function
  * @param S              points to an instance of the floating-point CFFT structure
  * @param fftLen         fft length (number of complex samples)
  * @return execution status
  *                  - STM32_HSP_MATH_SUCCESS        : Operation successful
  *                  - STM32_HSP_MATH_ARGUMENT_ERROR : an error is detected
  */
stm32_hsp_status stm32_hsp_cfft_init_f32(stm32_hsp_cfft_instance_f32 *S, uint16_t fftLen)
{
  S->pTwiddle = NULL;
  S->pBitRevTable = NULL;
  S->bitRevLength = 0U;
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
  S->rearranged_twiddle_tab_stride1_arr = NULL;
  S->rearranged_twiddle_tab_stride2_arr = NULL;
  S->rearranged_twiddle_tab_stride3_arr = NULL;
  S->rearranged_twiddle_stride1 = NULL;
  S->rearranged_twiddle_stride2 = NULL;
  S->rearranged_twiddle_stride3 = NULL;
#endif /* ARM_MATH_MVEF */
  /* Set Calculate LOG2NBP according RFFT size */
  if (fftLen == HSP_FFT_LENGTH_32)
  {
    S->fftLen = HSP_LOG2NBP_32;
  }
  else if (fftLen == HSP_FFT_LENGTH_64)
  {
    S->fftLen = HSP_LOG2NBP_64;
  }
  else if (fftLen == HSP_FFT_LENGTH_128)
  {
    S->fftLen = HSP_LOG2NBP_128;
  }
  else if (fftLen == HSP_FFT_LENGTH_256)
  {
    S->fftLen = HSP_LOG2NBP_256;
  }
  else if (fftLen == HSP_FFT_LENGTH_512)
  {
    S->fftLen = HSP_LOG2NBP_512;
  }
  else if (fftLen == HSP_FFT_LENGTH_1024)
  {
    S->fftLen = HSP_LOG2NBP_1024;
  }
  else if (fftLen == HSP_FFT_LENGTH_2048)
  {
    S->fftLen = HSP_LOG2NBP_2048;
  }
  else if (fftLen == HSP_FFT_LENGTH_4096)
  {
    S->fftLen = HSP_LOG2NBP_4096;
  }
  else
  {
    return STM32_HSP_MATH_ARGUMENT_ERROR;
  }
  return STM32_HSP_MATH_SUCCESS;
}

/**
  * @brief Initialization function for the floating-point FIR filter.
  * @param S          points to an instance of the floating-point FIR filter structure.
  * @param numTaps    Number of filter coefficients in the filter.
  * @param pCoeffs    points to the filter coefficients.
  * @param pState     points to the state buffer.
  * @param blockSize  number of samples that are processed at a time.
  */
void stm32_hsp_fir_init_f32(stm32_hsp_fir_instance_f32 *S, uint16_t numTaps, const float32_t *pCoeffs,
                            float32_t *pState, uint32_t blockSize)
{
  hsp_u32_f32_t val;
  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_Fir(stm32_cmsis_handle, numTaps, blockSize,
                                                                       HSP_BRAM_ALLOCATION_DEFAULT);
  val.u32 = (uint32_t)state;
  pState[0] = val.f32;
  S->pState = pState;
  S->numTaps = numTaps;
  S->pCoeffs = pCoeffs;
}

/**
  * @brief Initialization function for the floating-point FIR decimator.
  * @param S          points to an instance of the floating-point FIR decimator structure
  * @param numTaps    number of coefficients in the filter
  * @param M          decimation factor
  * @param pCoeffs    points to the filter coefficients
  * @param pState     points to the state buffer
  * @param blockSize  number of input samples to process per call
  * @return execution status
  *                    - STM32_HSP_MATH_SUCCESS      : Operation successful
  *                    - STM32_HSP_MATH_LENGTH_ERROR : blockSize is not a multiple of M
  */
stm32_hsp_status stm32_hsp_fir_decimate_init_f32(stm32_hsp_fir_decimate_instance_f32 *S, uint16_t numTaps, uint8_t M,
                                                 const float32_t *pCoeffs, float32_t *pState, uint32_t blockSize)
{
  hsp_u32_f32_t val;

  /* The size of the input block must be a multiple of the decimation factor */
  if ((blockSize % M) != 0U)
  {
    return STM32_HSP_MATH_LENGTH_ERROR;
  }

  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_FirDecimate(stm32_cmsis_handle, numTaps, blockSize,
                                                                               M, HSP_BRAM_ALLOCATION_DEFAULT);

  if (state == (uint32_t)NULL)
  {
    return (STM32_HSP_MATH_LENGTH_ERROR);
  }

  /* Assign filter taps */
  S->numTaps = numTaps;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Assign state pointer */
  val.u32 = (uint32_t)state;
  pState[0] = val.f32;
  S->pState = pState;

  /* Assign Decimation Factor */
  S->M = M;

  return STM32_HSP_MATH_SUCCESS;
}

/**
  * @brief Initialization function for the floating-point Biquad cascade filter.
  * @param S          points to an instance of the floating-point Biquad cascade structure.
  * @param numStages  number of 2nd order stages in the filter.
  * @param pCoeffs    points to the filter coefficients.
  * @param pState     points to the state buffer.
  */
void stm32_hsp_biquad_cascade_df1_init_f32(stm32_hsp_biquad_casd_df1_inst_f32 *S, uint8_t numStages,
                                           const float32_t *pCoeffs,
                                           float32_t *pState)
{
  hsp_u32_f32_t val;

  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_BiquadCascadeDf1(stm32_cmsis_handle, numStages,
                                        HSP_BRAM_ALLOCATION_DEFAULT);
  val.u32 = (uint32_t)state;

  /* Assign filter stages */
  S->numStages = numStages;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Assign state pointer */
  pState[0] = val.f32;
  S->pState = pState;
}

/**
  * @brief Initialization function for the floating-point transposed direct form II Biquad cascade filter.
  * @param S           points to an instance of the filter data structure.
  * @param numStages   number of 2nd order stages in the filter.
  * @param pCoeffs     points to the filter coefficients.
  * @param pState      points to the state buffer.
  *
  *   @par Coefficient and State Ordering
  *        The coefficients are stored in the array  pCoeffs in the following order
  *        {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...}
  *   @par
  *                    where b1x and a1x are the coefficients for the first stage,
  *                    b2x and a2x are the coefficients for the second stage,
  *                    and so on.  The pCoeffs array contains a total of 5*numStages values.
  *                    and it must be initialized using the function
  *                    arm_biquad_cascade_df2T_compute_coefs_f32 which is taking the
  *                    standard array coefficient as parameters.
  *   @par
  *                    The pState is a pointer to state array.
  *                    Each Biquad stage has 2 state variables d1, and d2.
  *                    The 2 state variables for stage 1 are first, then the 2 state variables for stage 2, and so on.
  *                    The state array has a total length of 2*numStages values.
  *                    The state variables are updated after each block of data is processed; the coefficients are untouched.
 */
void stm32_hsp_biquad_cascade_df2T_init_f32(stm32_hsp_biquad_cascade_df2T_instance_f32 *S, uint8_t numStages,
                                            const float32_t *pCoeffs, float32_t *pState)
{
  hsp_u32_f32_t val;

  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_BiquadCascadeDf2t(stm32_cmsis_handle, numStages,
                                        HSP_BRAM_ALLOCATION_DEFAULT);
  /* How to return error if create state does not succeed ? */

  /* Assign filter stages */
  S->numStages = numStages;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Assign state pointer */
  val.u32 = (uint32_t)state;
  pState[0] = val.f32;
  S->pState = pState;
}

/**
  * @brief Initialization function for floating-point LMS filter.
  * @param S          points to an instance of the floating-point LMS filter structure.
  * @param numTaps    number of filter coefficients.
  * @param pCoeffs    points to the coefficient buffer.
  * @param pState     points to state buffer.
  * @param mu         step size that controls filter coefficient updates.
  * @param blockSize  number of samples to process.
  */
void stm32_hsp_lms_init_f32(stm32_hsp_lms_instance_f32 *S, uint16_t numTaps, float32_t *pCoeffs, float32_t *pState,
                            float32_t mu, uint32_t blockSize)
{
  hsp_u32_f32_t val;

  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_Lms(stm32_cmsis_handle, numTaps,
                                                                       HSP_BRAM_ALLOCATION_DEFAULT);
  /*  How to return error if create state does not succeed ? */

  /* Assign filter taps */
  S->numTaps = numTaps;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Assign state pointer */
  val.u32 = (uint32_t)state;
  pState[0] = val.f32;
  S->pState = pState;

  /* Assign Step size value */
  S->mu = mu;
}


/**
  * @brief Initialization function for the floating-point IIR lattice filter.
  * @param S          points to an instance of the floating-point IIR lattice structure.
  * @param numStages  number of stages in the filter.
  * @param pkCoeffs   points to the reflection coefficient buffer.  The array is of length numStages.
  * @param pvCoeffs   points to the ladder coefficient buffer.  The array is of length numStages+1.
  * @param pState     points to the state buffer.
  * @param blockSize  number of samples to process.
  */
void stm32_hsp_iir_lattice_init_f32(stm32_hsp_iir_lattice_instance_f32 *S, uint16_t numStages, float32_t *pkCoeffs,
                                    float32_t *pvCoeffs, float32_t *pState, uint32_t blockSize)
{
  hsp_u32_f32_t val;

  hsp_filter_state_identifier_t state = HSP_BRAM_MallocStateBuffer_IirLattice(stm32_cmsis_handle, numStages,
                                                                              HSP_BRAM_ALLOCATION_DEFAULT);
  /* How to return error if create state does not succeed ? */

  /* Assign filter taps */
  S->numStages = numStages;

  /* Assign reflection coefficient pointer */
  S->pkCoeffs = pkCoeffs;

  /* Assign ladder coefficient pointer */
  S->pvCoeffs = pvCoeffs;

  /* Assign state pointer */
  val.u32 = (uint32_t)state;
  pState[0] = val.f32;
  S->pState = pState;
}

/**
  * @brief  Floating-point matrix initialization.
  * @param S         points to an instance of the floating-point matrix structure.
  * @param nRows     number of rows in the matrix.
  * @param nColumns  number of columns in the matrix.
  * @param pData     points to the matrix data array.
  */
void stm32_hsp_mat_init_f32(stm32_hsp_matrix_instance_f32 *S, uint16_t nRows, uint16_t nColumns, float32_t *pData)
{
  /* Assign Number of Rows */
  S->numRows = nRows;

  /* Assign Number of Columns */
  S->numCols = nColumns;

  /* Assign Data pointer */
  S->pData = pData;
}

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

/**
  * @}
  */
