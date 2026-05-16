/**
  ******************************************************************************
  * @file hsp_dsp.h
  * @brief API for STM32 HSP DSP function
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

#ifndef HSP_DSP_H
#define HSP_DSP_H

/* Includes ------------------------------------------------------------------*/
#include "hsp_direct_command.h"


/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @defgroup HSP_MODULES_DSP HSP Modules DSP
  * @{
  * @details
  * This section includes a set of signal processing functions compatible with CMSIS-DSP functions,
  * enabling acceleration of standard 32-bit floating point signal processing operations.
  * All functions are executed in the accelerator mode of the HSP.
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup HSP_MODULES_DSP_Exported_Types HSP Modules DSP Exported Types
  * @{
  */
/**
  * @brief Error status returned by some functions in the library.
  */
typedef enum
{
  STM32_HSP_MATH_SUCCESS                 =  0,        /*!<  No error */
  STM32_HSP_MATH_ARGUMENT_ERROR          = -1,        /*!<  One or more arguments are incorrect */
  STM32_HSP_MATH_LENGTH_ERROR            = -2,        /*!<  Length of data buffer is incorrect */
  STM32_HSP_MATH_SIZE_MISMATCH           = -3,        /*!<  Size of matrices is not compatible with the operation */
  STM32_HSP_MATH_NANINF                  = -4,        /*!<  Not-a-number (NaN) or infinity is generated */
  STM32_HSP_MATH_SINGULAR                = -5,        /*!<  Input matrix is singular and cannot be inverted */
  STM32_HSP_MATH_TEST_FAILURE            = -6,        /*!<  Test Failed */
  STM32_HSP_MATH_DECOMPOSITION_FAILURE   = -7         /*!<  Decomposition Failed */
} stm32_hsp_status;

/**
  * @brief Instance structure for the floating-point CFFT/CIFFT function.
  */
typedef struct
{
  uint16_t fftLen;                   /*!<  length of the FFT. */
  const float32_t *pTwiddle;         /*!<  points to the Twiddle factor table. */
  const uint16_t *pBitRevTable;      /*!<  points to the bit reversal table. */
  uint16_t bitRevLength;             /*!<  bit reversal table length. */
#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
  const uint32_t *rearranged_twiddle_tab_stride1_arr;        /*!<  Per stage reordered twiddle pointer (offset 1) */
  const uint32_t *rearranged_twiddle_tab_stride2_arr;        /*!<  Per stage reordered twiddle pointer (offset 2) */
  const uint32_t *rearranged_twiddle_tab_stride3_arr;        /*!<  Per stage reordered twiddle pointer (offset 3) */
  const float32_t *rearranged_twiddle_stride1; /*!<  reordered twiddle offset 1 storage */
  const float32_t *rearranged_twiddle_stride2; /*!<  reordered twiddle offset 2 storage */
  const float32_t *rearranged_twiddle_stride3;
#endif /* ARM_MATH_MVEF */
} stm32_hsp_cfft_instance_f32;

/**
  * @brief Instance structure for the floating-point RFFT/RIFFT function.
  */
typedef struct
{
  stm32_hsp_cfft_instance_f32 Sint;     /*!<  Internal CFFT structure. */
  uint16_t fftLenRFFT;            /*!<  length of the real sequence */
  const float32_t *pTwiddleRFFT;  /*!<  Twiddle factors real stage  */
} stm32_hsp_rfft_fast_instance_f32;

/**
  * @brief Instance structure for the floating-point FIR filter.
  */
typedef struct
{
  uint16_t numTaps;     /*!<  number of filter coefficients in the filter. */
  float32_t *pState;    /*!<  points to the state variable array. The array is of length 1. */
  const float32_t *pCoeffs;   /*!<  points to the coefficient array. The array is of length 1. */
} stm32_hsp_fir_instance_f32;

/**
  @brief Instance structure for floating-point FIR decimator.
  */
typedef struct
{
  uint8_t M;                  /*!<  decimation factor. */
  uint16_t numTaps;           /*!<  number of coefficients in the filter. */
  const float32_t *pCoeffs;   /*!<  points to the coefficient array. The array is of length numTaps.*/
  float32_t *pState;          /*!<  points to the state variable array. The array is of length 1. */
} stm32_hsp_fir_decimate_instance_f32;

/**
  * @brief Instance structure for the floating-point Biquad cascade filter.
  */
typedef struct
{
  uint32_t numStages;            /*!<  number of 2nd order stages in the filter.  Overall order is 2*numStages. */
  float32_t *pState;             /*!<  Points to the array of state coefficients.  The array is of length 1. */
  const float32_t *pCoeffs;      /*!<  Points to the array of coefficients.  The array is of length 5*numStages. */
} stm32_hsp_biquad_casd_df1_inst_f32;

/**
  * @brief Instance structure for the floating-point transposed direct form II Biquad cascade filter.
  */
typedef struct
{
  uint8_t numStages;              /*!<  number of 2nd order stages in the filter.  Overall order is 2*numStages. */
  float32_t *pState;              /*!<  points to the array of state coefficients.  The array is of length 1. */
  const float32_t *pCoeffs;       /*!<  points to the array of coefficients.  The array is of length 5*numStages. */
} stm32_hsp_biquad_cascade_df2T_instance_f32;

/**
  * @brief Instance structure for the floating-point LMS filter.
  */
typedef struct
{
  uint16_t numTaps;    /*!<  number of coefficients in the filter. */
  float32_t *pState;   /*!<  points to the state variable array. The array is of length 1. */
  float32_t *pCoeffs;  /*!<  points to the coefficient array. The array is of length numTaps. */
  float32_t mu;        /*!<  step size that controls filter coefficient updates. */
} stm32_hsp_lms_instance_f32;

/**
  * @brief Instance structure for the floating-point IIR lattice filter.
  */
typedef struct
{
  uint16_t numStages;                  /*!<  number of stages in the filter. */
  float32_t *pState;                   /*!<  points to the state variable array. The array is of length 1. */
  float32_t *pkCoeffs;                 /*!<  points to the reflection coefficient array. The array is of length numStages. */
  float32_t *pvCoeffs;                 /*!<  points to the ladder coefficient array. The array is of length numStages+1. */
} stm32_hsp_iir_lattice_instance_f32;

/**
  * @brief Instance structure for the floating-point matrix structure.
  */
typedef struct
{
  uint16_t numRows;     /*!<  number of rows of the matrix.     */
  uint16_t numCols;     /*!<  number of columns of the matrix.  */
  float32_t *pData;     /*!<  points to the data of the matrix. */
} stm32_hsp_matrix_instance_f32;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/** @defgroup HSP_MODULES_DSP_Exported_Variables HSP Modules DSP Exported Variables
  * @{
  */
extern hsp_engine_context_t *stm32_cmsis_handle;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup HSP_MODULES_DSP_Exported_Functions HSP Modules DSP Exported Functions
  * @{
  */

/** @defgroup HSP_MODULES_DSP_Transform_Functions HSP Modules DSP Transform Functions
  * @{
  */
/**
  * @brief Processing function to perform an in-place floating-point FFT.
  * @param  rfft_instance   points to an stm32_hsp_rfft_fast_instance_f32 structure
  * @param  input           points to input buffer. Processing occurs in-place
  * @param  output          PARAMETER NOT USED because processing occurs in-place
  * @param  ifftFlag:
  *          - value = 0: execute a RFFT
  *          - value = 1: execute an inverse RFFT
  */
__STATIC_FORCEINLINE void stm32_hsp_rfft_fast_f32(stm32_hsp_rfft_fast_instance_f32 *rfft_instance, float32_t *input,
                                                  float32_t *output, uint8_t ifftFlag)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_RFFT_F32);
  HSP_ACC_Rfft_WriteParam(stm32_cmsis_handle, input, (rfft_instance)->Sint.fftLen, ifftFlag, 1U, HSP_RFFT_TYPE_2);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform an in-place floating-point complex FFT.
  * @param  cfft_instance  points to an instance of the floating-point CFFT structure
  * @param  input          points to the complex data buffer of size 2*fftLen. Processing occurs in-place
  * @param  ifftFlag       flag that selects transform direction
  *          - value = 0: forward transform
  *          - value = 1: inverse transform
  * @param  bitReverseFlag flag that enables / disables bit reversal of output
  *          - value = 0: disables bit reversal of output
  *          - value = 1: enables bit reversal of output
  */
__STATIC_FORCEINLINE void stm32_hsp_cfft_f32(stm32_hsp_cfft_instance_f32 *cfft_instance, float32_t *input,
                                             uint8_t ifftFlag,
                                             uint8_t bitReverseFlag)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_FFT_F32);
  HSP_ACC_Fft_WriteParam(stm32_cmsis_handle, input, (cfft_instance)->fftLen, ifftFlag, bitReverseFlag);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @}
  */

/** @defgroup HSP_MODULES_DSP_Filter_Functions HSP Modules DSP Filter Functions
  * @{
  */
/**
  * @brief Processing function to perform a floating-point FIR filter.
  * @param  fir_instance  points to an instance of the floating-point FIR structure.
  *                       Coeff are stored in reverse order than ARM CMSIS
  * @param  pSrc          points to the block of input data.
  * @param  pDst          points to the block of output data.
  * @param  blockSize     number of samples to process.
  */
__STATIC_FORCEINLINE void stm32_hsp_fir_f32(stm32_hsp_fir_instance_f32 *fir_instance, float32_t *pSrc, float32_t *pDst,
                                            uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR((uint32_t)((hsp_hw_if_filter_state_t *)((uint32_t) * ((uint32_t *)(&
                                                                                (fir_instance)->pState[0]))))->dirCmd);
  HSP_ACC_Filter_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((fir_instance)->pCoeffs),
                            ((hsp_filter_state_identifier_t) * ((uint32_t *)(&(fir_instance)->pState[0]))),
                            (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a floating-point FIR decimator filter.
  * @param  fir_instance  points to an instance of the floating-point FIR decimator structure
  * @param  pSrc          points to the block of input data
  * @param  pDst          points to the block of output data
  * @param  blockSize     number of samples to process
  */
__STATIC_FORCEINLINE void stm32_hsp_fir_decimate_f32(stm32_hsp_fir_decimate_instance_f32 *fir_instance, float32_t *pSrc,
                                                     float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR((uint32_t)((hsp_hw_if_fir_decimate_filter_state_t *)((uint32_t) * ((uint32_t *)(&
                                                                               (fir_instance)->pState[0]))))->dirCmd);
  HSP_ACC_FirDecimate_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((fir_instance)->pCoeffs),
                                 ((hsp_filter_state_identifier_t) * ((uint32_t *)(&(fir_instance)->pState[0]))),
                                 (uint32_t)pDst, blockSize, (uint32_t)((fir_instance)->M));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a floating-point Biquad cascade DF1 filter.
  * @param  biq_instance  points to an instance of the floating-point Biquad cascade structure.
  * @param  pSrc          points to the block of input data.
  * @param  pDst          points to the block of output data.
  * @param  blockSize     number of samples to process.
  */
__STATIC_FORCEINLINE void stm32_hsp_biquad_cascade_df1_f32(stm32_hsp_biquad_casd_df1_inst_f32 *biq_instance,
                                                           float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR((uint32_t)((hsp_hw_if_filter_state_t *)((uint32_t) * ((uint32_t *)(&
                                                                                (biq_instance)->pState[0]))))->dirCmd);
  HSP_ACC_Filter_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((biq_instance)->pCoeffs),
                            ((hsp_filter_state_identifier_t) * ((uint32_t *)(&(biq_instance)->pState[0]))),
                            (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a floating-point transposed direct form II (DF2T) Biquad cascade filter.
  * @param  biq_instance  points to an instance of the filter data structure
  * @param  pSrc          points to the block of input data
  * @param  pDst          points to the block of output data
  * @param  blockSize     number of samples to process.
  */
__STATIC_FORCEINLINE void stm32_hsp_biquad_cascade_df2T_f32(stm32_hsp_biquad_cascade_df2T_instance_f32 *biq_instance,
                                                            float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR((uint32_t)((hsp_hw_if_filter_state_t *)((uint32_t) * ((uint32_t *)(&
                                                                                (biq_instance)->pState[0]))))->dirCmd);
  HSP_ACC_Filter_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((biq_instance)->pCoeffs),
                            ((hsp_filter_state_identifier_t) * ((uint32_t *)(&(biq_instance)->pState[0]))),
                            (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a convolution of floating-point sequences.
  * @param  pSrcA    points to the first input sequence.
  * @param  srcALen  length of the first input sequence.
  * @param  pSrcB    points to the second input sequence.
  * @param  srcBLen  length of the second input sequence.
  * @param  pDst     points to the location where the output result is written.
  */
__STATIC_FORCEINLINE void stm32_hsp_conv_f32(float32_t *pSrcA, uint32_t srcALen, float32_t *pSrcB, uint32_t srcBLen,
                                             float32_t *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CONV_F32);
  HSP_ACC_VectIIOSS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, srcALen, srcBLen);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a correlation of floating-point sequences.
  * @param  pSrcA    points to the first input sequence.
  * @param  srcALen  length of the first input sequence.
  * @param  pSrcB    points to the second input sequence.
  * @param  srcBLen  length of the second input sequence.
  * @param  pDst     points to the location where the output result is written.
  */
__STATIC_FORCEINLINE void stm32_hsp_correlate_f32(float32_t *pSrcA, uint32_t srcALen, float32_t *pSrcB,
                                                  uint32_t srcBLen, float32_t *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CORR_F32);
  HSP_ACC_VectIIOSS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, srcALen, srcBLen);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a floating-point LMS (Least Mean Square) filter.
  * @param  lms_instance  points to an instance of the floating-point LMS filter structure.
  * @param  pSrc          points to the block of input data.
  * @param  pRef          points to the block of reference data.
  * @param  pOut          points to the block of output data.
  * @param  pErr          points to the block of error data.
  * @param  blockSize     number of samples to process.
  */
__STATIC_FORCEINLINE void stm32_hsp_lms_f32(stm32_hsp_lms_instance_f32 *lms_instance, float32_t *pSrc, float32_t *pRef,
                                            float32_t *pOut, float32_t *pErr, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_LMS_F32);
  HSP_ACC_Lms_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((lms_instance)->pCoeffs),
                         ((hsp_filter_state_identifier_t) * ((uint32_t *)(&(lms_instance)->pState[0]))), (uint32_t)pOut,
                         (uint32_t)pRef, (uint32_t)pErr, blockSize, ((lms_instance)->mu));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform a floating-point IIR lattice filter.
  * @param  iir_instance   points to an instance of the floating-point IIR lattice structure.
  * @param  pSrc           points to the block of input data.
  * @param  pDst           points to the block of output data.
  * @param  blockSize      number of samples to process.
  */
__STATIC_FORCEINLINE void stm32_hsp_iir_lattice_f32(stm32_hsp_iir_lattice_instance_f32 *iir_instance, float32_t *pSrc,
                                                    float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_IIR_LATTICE_F32);
  HSP_ACC_IirLattice_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)((iir_instance)->pkCoeffs),
                                (uint32_t)((iir_instance)->pvCoeffs),
                                ((uint32_t) * ((uint32_t *)(&(iir_instance)->pState[0]))), (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @}
  */

/** @defgroup HSP_MODULES_DSP_Complex_Functions HSP Modules DSP Complex Functions
  * @{
  */
/**
  * @brief Processing function to compute floating-point complex conjugate.
  * @param  pSrc         points to the input vector
  * @param  pDst         points to the output vector
  * @param  numSamples   number of complex samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_conj_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_CONJ_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, (numSamples * 2U));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point complex dot product
  * @param  pSrcA        points to the first input vector
  * @param  pSrcB        points to the second input vector
  * @param  numSamples   number of complex samples in each vector
  * @param  realResult   real part of the result returned here
  * @param  imagResult   imaginary part of the result returned here
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB, uint32_t numSamples,
                                                       float32_t *realResult, float32_t *imagResult)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_CMSIS_DOTPROD_F32);
  HSP_ACC_VectIIOOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)realResult,
                               (uint32_t)imagResult, (numSamples * 2));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point complex magnitude
  * @param  pSrc        points to the complex input vector
  * @param  pDst        points to the real output vector
  * @param  numSamples  number of complex samples in the input vector
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_mag_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_MAG_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, (numSamples * 2U));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point complex magnitude squared
  * @param  pSrc        points to the complex input vector
  * @param  pDst        points to the real output vector
  * @param  numSamples  number of complex samples in the input vector
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_mag_squared_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_MAGSQUARED_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, (numSamples * 2U));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point complex-by-complex multiplication
  * @param  pSrcA        points to the first input vector
  * @param  pSrcB        points to the second input vector
  * @param  pDst         points to the output vector
  * @param  numSamples   number of complex samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_mult_cmplx_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                                                         uint32_t numSamples)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_MUL_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, (numSamples * 2U));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point complex-by-real multiplication
  * @param  pSrcCmplx    points to the complex input vector
  * @param  pSrcReal     points to the real input vector
  * @param  pCmplxDst    points to the complex output vector
  * @param  numSamples   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_cmplx_mult_real_f32(float32_t *pSrcCmplx, float32_t *pSrcReal, float32_t *pCmplxDst,
                                                        uint32_t numSamples)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_CMPLX_RMUL_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcCmplx, (uint32_t)pSrcReal, (uint32_t)pCmplxDst,
                              (numSamples * 2U));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @}
  */

/** @defgroup HSP_MODULES_DSP_Vector_Functions HSP Modules DSP Vector Functions
  * @{
  */
/**
  * @brief Processing function to compute floating-point vector absolute value.
  * @param  pSrc        points to the input buffer
  * @param  pDst        points to the output buffer
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_abs_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_ABS_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute floating-point vector addition.
  * @param  pSrcA       points to the first input vector
  * @param  pSrcB       points to the second input vector
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_add_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_ADD_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute the mean value of a floating-point vector.
  * @param  pSrc        points to the input vector
  * @param  blockSize   number of samples to process
  * @param  pResult     points to the output vector
  */
__STATIC_FORCEINLINE void stm32_hsp_mean_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_AVG_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pResult, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to copy the elements of a floating-point vector.
  * @param  pSrc       points to the input vector
  * @param  pDst       points to the output vector
  * @param  blockSize  number of samples to process
  */
__STATIC_FORCEINLINE void stm32_hsp_copy_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_COPY);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute dot product of floating-point vectors.
  * @param  pSrcA      points to the first input vector
  * @param  pSrcB      points to the second input vector
  * @param  blockSize  number of samples in each vector
  * @param  result     output result returned here
  */
__STATIC_FORCEINLINE void stm32_hsp_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB, uint32_t blockSize,
                                                 float32_t *result)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_DOTPROD_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)result, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to find value and position of the absolute biggest element of a floating-point vector.
  * @param  pSrc       points to the input buffer
  * @param  blockSize  number of samples to process
  * @param  pResult    maximum value returned here
  * @param  pIndex     index of maximum value returned here
  */
__STATIC_FORCEINLINE void stm32_hsp_absmax_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult,
                                               uint32_t *pIndex)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_ABSMAX_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pResult, (uint32_t)pIndex, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to find value and position of the biggest element of a floating-point vector.
  * @param  pSrc        points to the input buffer
  * @param  blockSize   length of the input vector
  * @param  pResult     maximum value returned here
  * @param  pIndex      index of maximum value returned here
  */
__STATIC_FORCEINLINE void stm32_hsp_max_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult, uint32_t *pIndex)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_MAX_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pResult, (uint32_t)pIndex, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to find value and position of the smallest element of a floating-point vector.
  * @param  pSrc        is input pointer
  * @param  blockSize   is the number of samples to process
  * @param  pResult     is output pointer
  * @param  pIndex      is the array index of the minimum value in the input buffer.
  */
__STATIC_FORCEINLINE void stm32_hsp_min_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult, uint32_t *pIndex)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_MIN_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pResult, (uint32_t)pIndex, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform element-wise multiplication of two floating-point vectors.
  * @param  pSrcA       points to the first input vector
  * @param  pSrcB       points to the second input vector
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_mult_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_MUL_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to add an offset floating-point value to each element of a floating-point vector
  * @param  pSrc         points to the input vector
  * @param  offset       is the offset to be added
  * @param  pDst         points to the output vector
  * @param  blockSize    number of samples in the vector
  */
__STATIC_FORCEINLINE void stm32_hsp_offset_f32(float32_t *pSrc, float32_t offset, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_OFFSET_I_F32);
  HSP_ACC_VectIFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, offset, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to compute the RMS (Root Mean Square) value of a floating-point vector
  * @param  pSrc        is input pointer
  * @param  blockSize   is the number of samples to process
  * @param  pResult     is output value.
  */
__STATIC_FORCEINLINE void stm32_hsp_rms_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_RMS_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pResult, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to multiply each element of a floating-point vector by a floating-point scale value
  * @param  pSrc        points to the input vector
  * @param  scale       scale factor to be applied
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in the vector
  */
__STATIC_FORCEINLINE void stm32_hsp_scale_f32(float32_t *pSrc,  float32_t scale, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SCALE_I_F32);
  HSP_ACC_VectIFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, scale, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to fill a constant value into a floating-point vector.
  * @param  value       input value to be filled
  * @param  pDst        output pointer
  * @param  blockSize   number of samples to process
  */
__STATIC_FORCEINLINE void stm32_hsp_fill_f32(float32_t value, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SET_I);
  HSP_ACC_VectIFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)pDst, value, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform element-wise subtraction of a floating-point vector.
  * @param  pSrcA       points to the first input vector
  * @param  pSrcB       points to the second input vector
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_sub_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SUB_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrcA, (uint32_t)pSrcB, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to convert the elements of the Q31 vector to floating-point vector.
  * @param  pSrc        is input pointer
  * @param  pDst        is output pointer
  * @param  blockSize   is the number of samples to process
  */
__STATIC_FORCEINLINE void stm32_hsp_q31_to_float(int32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_Q312F);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to convert the elements of the floating-point vector to Q31 vector.
  * @param  pSrc        points to the floating-point input vector
  * @param  pDst        points to the Q31 output vector
  * @param  blockSize   length of the input vector
  */
__STATIC_FORCEINLINE void stm32_hsp_float_to_q31(float32_t *pSrc, int32_t *pDst, uint32_t blockSize)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_F2Q31);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @brief Processing function to perform the natural logarithm of each element of a floating-point vector.
  * @param  pSrc        points to the input vector
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_vlog_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
#if defined(__HSP_DMA__)
  const float32_t LN_VAL_A = 1.0f;
  const float32_t LN_VAL_B = 0.69314718246459960937f;
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_LOGS_F32);
  HSP_ACC_VectIFVFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, LN_VAL_A, LN_VAL_B, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(hmw);
#else /* __HSP_DMA__ */
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_LN_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
#endif /* __HSP_DMA__ */
}

/**
  * @brief Processing function to perform the exponential of each element of a floating-point vector.
  * @param  pSrc        points to the input vector
  * @param  pDst        points to the output vector
  * @param  blockSize   number of samples in each vector
  */
__STATIC_FORCEINLINE void stm32_hsp_vexp_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
#if defined(__HSP_DMA__)
  const float32_t EXPE_VAL_A = 1.44269502162933349609375f;
  const float32_t EXPE_VAL_B = 0.0f;
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_EXPS_F32);
  HSP_ACC_VectIFVFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, EXPE_VAL_A, EXPE_VAL_B,
                                 (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(hmw);
#else /* __HSP_DMA__ */
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_EXP_F32);
  HSP_ACC_VectIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pDst, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
#endif /* __HSP_DMA__ */
}

/**
  * @brief Processing function to perform the sum of the squares of the elements of a floating-point vector.
  * @param  pSrc        is input pointer
  * @param  blockSize   is the number of samples to process
  * @param  pResult     is output value.
  */
__STATIC_FORCEINLINE void stm32_hsp_power_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_DOTPROD_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)pSrc, (uint32_t)pSrc, (uint32_t)pResult, blockSize);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
}

/**
  * @}
  */

/** @defgroup HSP_MODULES_DSP_Matrix_Functions HSP Modules DSP Matrix Functions
  * @{
  */
/**
  * @brief Processing function to compute the addition of two floating-point matrix.
  * @param  pSrcA   points to the first input matrix structure
  * @param  pSrcB   points to the second input matrix structure
  * @param  pDst    points to output matrix structure
  * @return STM32_HSP_MATH_SIZE_MISMATCH or STM32_HSP_MATH_SUCCESS based on the outcome of size checking.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_add_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                            stm32_hsp_matrix_instance_f32 *pSrcB,
                                                            stm32_hsp_matrix_instance_f32 *pDst)
{
  if (((pSrcA)->numRows != (pSrcB)->numRows) || ((pSrcA)->numCols != (pSrcB)->numCols) ||
      ((pSrcA)->numRows != (pDst)->numRows) || ((pSrcA)->numCols != (pDst)->numCols))
  {
    return STM32_HSP_MATH_SIZE_MISMATCH;
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_ADD_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                              (uint32_t)((pDst)->pData), (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_add_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                            stm32_hsp_matrix_instance_f32 *pSrcB,
                                                            stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_ADD_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                              (uint32_t)((pDst)->pData), (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @brief Processing function to compute the subtraction of two floating-point matrix
  * @param  pSrcA   points to the first input matrix structure
  * @param  pSrcB   points to the second input matrix structure
  * @param  pDst    points to output matrix structure
  * @return STM32_HSP_MATH_SIZE_MISMATCH or STM32_HSP_MATH_SUCCESS based on the outcome of size checking.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_sub_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                            stm32_hsp_matrix_instance_f32 *pSrcB,
                                                            stm32_hsp_matrix_instance_f32 *pDst)
{
  if (((pSrcA)->numRows != (pSrcB)->numRows) || ((pSrcA)->numCols != (pSrcB)->numCols) ||
      ((pSrcA)->numRows != (pDst)->numRows) || ((pSrcA)->numCols != (pDst)->numCols))
  {
    return STM32_HSP_MATH_SIZE_MISMATCH;
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SUB_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                              (uint32_t)((pDst)->pData), (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_sub_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                            stm32_hsp_matrix_instance_f32 *pSrcB,
                                                            stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SUB_F32);
  HSP_ACC_VectIIOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                              (uint32_t)((pDst)->pData), (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @brief Processing function to compute floating-point matrix inverse.
  * @param  pSrc    points to the instance of the input floating-point matrix structure.
  * @param  pDst    points to the instance of the output floating-point matrix structure.
  * @return The function returns STM32_HSP_MATH_SIZE_MISMATCH, if the dimensions do not match.
  *         If the input matrix is singular (does not have an inverse), then the algorithm terminates and returns
  *         error status STM32_HSP_MATH_SINGULAR.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_inverse_f32(stm32_hsp_matrix_instance_f32 *pSrc,
                                                                stm32_hsp_matrix_instance_f32 *pDst)
{
  if ((pSrc->numRows != pSrc->numCols) || (pDst->numRows != pDst->numCols) || (pSrc->numRows != pDst->numRows))
  {
    return STM32_HSP_MATH_SIZE_MISMATCH;
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_INV_F32);
  HSP_ACC_MatInv_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), (uint32_t)((pDst)->pData), ((pDst)->numCols),
                            ((pDst)->numRows));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  if (HSP_HW_IF_READ_PARAMR0() != 0)
  {
    return STM32_HSP_MATH_SINGULAR;
  }
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_inverse_f32(stm32_hsp_matrix_instance_f32 *pSrc,
                                                                stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_INV_F32);
  HSP_ACC_MatInv_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), (uint32_t)((pDst)->pData), ((pDst)->numCols),
                            ((pDst)->numRows));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  if (HSP_HW_IF_READ_PARAMR0() != 0)
  {
    return STM32_HSP_MATH_SINGULAR;
  }
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @brief Processing function to compute floating-point matrix transpose.
  * @param  pSrc  points to the input matrix
  * @param  pDst  points to the output matrix
  * @return The function returns either STM32_HSP_MATH_SIZE_MISMATCH or STM32_HSP_MATH_SUCCESS based on
  *         the outcome of size checking.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_trans_f32(stm32_hsp_matrix_instance_f32 *pSrc,
                                                              stm32_hsp_matrix_instance_f32 *pDst)
{
  if (((pSrc)->numRows != (pDst)->numCols) || ((pSrc)->numCols != (pDst)->numRows))
  {
    return STM32_HSP_MATH_SIZE_MISMATCH;
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_TRANS_F32);
  HSP_ACC_MatTrans_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), (uint32_t)((pDst)->pData),
                              ((pDst)->numCols), ((pDst)->numRows));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_trans_f32(stm32_hsp_matrix_instance_f32 *pSrc,
                                                              stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_TRANS_F32);
  HSP_ACC_MatTrans_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), (uint32_t)((pDst)->pData),
                              ((pDst)->numCols), ((pDst)->numRows));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @brief Processing function to compute the multiplication of two floating-point matrix
  * @param  pSrcA  points to the first input matrix structure
  * @param  pSrcB  points to the second input matrix structure
  * @param  pDst   points to output matrix structure
  * @return The function returns either STM32_HSP_MATH_SIZE_MISMATCH or STM32_HSP_MATH_SUCCESS based on
  *         the outcome of size checking.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_mult_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                             stm32_hsp_matrix_instance_f32 *pSrcB,
                                                             stm32_hsp_matrix_instance_f32 *pDst)
{
  if (((pSrcA)->numCols != (pSrcB)->numRows) || ((pSrcA)->numRows != (pDst)->numRows)
      || ((pSrcB)->numCols != (pDst)->numCols))
  {
    return (STM32_HSP_MATH_SIZE_MISMATCH);
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_MUL_F32);
  HSP_ACC_MatMult_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                             (uint32_t)((pDst)->pData),
                             ((pSrcA)->numRows), ((pSrcA)->numCols), ((pSrcB)->numRows), ((pSrcB)->numCols));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_mult_f32(stm32_hsp_matrix_instance_f32 *pSrcA,
                                                             stm32_hsp_matrix_instance_f32 *pSrcB,
                                                             stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_MAT_MUL_F32);
  HSP_ACC_MatMult_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrcA)->pData), (uint32_t)((pSrcB)->pData),
                             (uint32_t)((pDst)->pData),
                             ((pSrcA)->numRows), ((pSrcA)->numCols), ((pSrcB)->numRows), ((pSrcB)->numCols));
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @brief Processing function to multiply each element of a matrix by a scalar floating-point value.
  * @param  pSrc   points to the input matrix
  * @param  scale  scale factor
  * @param  pDst   points to the output matrix
  * @return The function returns either STM32_HSP_MATH_SIZE_MISMATCH or STM32_HSP_MATH_SUCCESS based on
  *         the outcome of size checking.
  */
#if defined(STM32_HSP_MATH_MATRIX_CHECK)
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_scale_f32(stm32_hsp_matrix_instance_f32 *pSrc, float32_t scale,
                                                              stm32_hsp_matrix_instance_f32 *pDst)
{
  if (((pSrc)->numRows != (pDst)->numRows) || ((pSrc)->numCols != (pDst)->numCols))
  {
    return (STM32_HSP_MATH_SIZE_MISMATCH);
  }
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SCALE_I_F32);
  HSP_ACC_VectIFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), scale, (uint32_t)((pDst)->pData),
                               (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#else /* STM32_HSP_MATH_MATRIX_CHECK */
__STATIC_FORCEINLINE stm32_hsp_status stm32_hsp_mat_scale_f32(stm32_hsp_matrix_instance_f32 *pSrc, float32_t scale,
                                                              stm32_hsp_matrix_instance_f32 *pDst)
{
  HSP_HW_IF_WRITE_DCMDIDR(HSP_DIRECT_CMD_VEC_SCALE_I_F32);
  HSP_ACC_VectIFVOS_WriteParam(stm32_cmsis_handle, (uint32_t)((pSrc)->pData), scale, (uint32_t)((pDst)->pData),
                               (pDst)->numCols * (pDst)->numRows);
  HSP_ACC_WAIT_END_OF_DIRECT_COMMAND(stm32_cmsis_handle);
  return STM32_HSP_MATH_SUCCESS;
}
#endif /* STM32_HSP_MATH_MATRIX_CHECK */

/**
  * @}
  */

/** @defgroup HSP_MODULES_DSP_Init_Functions HSP Modules DSP Initialization Functions
  * @{
  */
stm32_hsp_status stm32_hsp_init(hsp_engine_context_t *hmw);
stm32_hsp_status stm32_hsp_rfft_fast_init_f32(stm32_hsp_rfft_fast_instance_f32 *S, uint16_t fftLen);
stm32_hsp_status stm32_hsp_cfft_init_f32(stm32_hsp_cfft_instance_f32 *S, uint16_t fftLen);
void stm32_hsp_fir_init_f32(stm32_hsp_fir_instance_f32 *S, uint16_t numTaps, const float32_t *pCoeffs,
                            float32_t *pState, uint32_t blockSize);
stm32_hsp_status stm32_hsp_fir_decimate_init_f32(stm32_hsp_fir_decimate_instance_f32 *S, uint16_t numTaps, uint8_t M,
                                                 const float32_t *pCoeffs, float32_t *pState, uint32_t blockSize);
void stm32_hsp_biquad_cascade_df1_init_f32(stm32_hsp_biquad_casd_df1_inst_f32 *S, uint8_t numStages,
                                           const float32_t *pCoeffs,
                                           float32_t *pState);
void stm32_hsp_biquad_cascade_df2T_init_f32(stm32_hsp_biquad_cascade_df2T_instance_f32 *S, uint8_t numStages,
                                            const float32_t *pCoeffs, float32_t *pState);
void stm32_hsp_lms_init_f32(stm32_hsp_lms_instance_f32 *S, uint16_t numTaps, float32_t *pCoeffs, float32_t *pState,
                            float32_t mu, uint32_t blockSize);
void stm32_hsp_iir_lattice_init_f32(stm32_hsp_iir_lattice_instance_f32 *S, uint16_t numStages, float32_t *pkCoeffs,
                                    float32_t *pvCoeffs, float32_t *pState, uint32_t blockSize);
void stm32_hsp_mat_init_f32(stm32_hsp_matrix_instance_f32 *S, uint16_t nRows, uint16_t nColumns, float32_t *pData);

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

#endif /* HSP_DSP_H */
