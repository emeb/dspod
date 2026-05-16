/**
  ******************************************************************************
  * @file    hsp_direct_command.c
  * @brief   This file implements the HSP Direct Command functions
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
#include "hsp_direct_command.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_DIRECT_COMMAND
  * @{
  */

/** @addtogroup HSP_MODULES_DIRECT_COMMAND_Private_Functions
  * @{
  */
/**
  * Get FFT attribute, used by the FFT to control the load operations and bit reverse addressing
  * @param log2Nbp  FFT size log2
  * @param bitrev   FFT bit reverse
  * @param ifftFlag FFT inverse flag
  */
static uint32_t get_fft_attr(uint32_t log2Nbp, uint32_t bitrev, uint32_t ifftFlag)
{
  /*
  fft_attr used by the FFT to control the load operations and bit reverse addressing.
  fft_attr has different fields:
  fft_attr[3:0] : contains the log2(Size of the FFT), used for bit reverse addressing mode.
                  Ex: 7 for a FFT 128 points, 8 for a FFT 256. It can support FFT up to 32768 points.
  fft_attr[4]   : used to compute real FFT (ie : imaginary part of data load from AMEM is overwritten by 0)
  fft_attr[5]   : used to compute inverse FFT (ie : imaginary part of data load from AMEM is negate)
  fft_attr[7:6] : reserved.
  fft_attr[19:8]: used for bit reverse addressing mode to store the address of the current element read.
  */
  uint32_t fftAttr = 0U;
  if (bitrev)
  {
    fftAttr += log2Nbp;
  }
  if (ifftFlag)
  {
    /* Inverse FFT */
    fftAttr += 0x20;
  }
  return fftAttr;
}

/**
  * @brief Write parameters for executing RFFT transform
  * @param hmw          HSP middleware context.
  * @param buff         Input and output Buffer
  * @param log2Nbp      log2(number of FFT point)
  * @param ifftFlag     Inverse FFT flag
  * @param bitrev       Bit reverse flag
  * @param fftVariant   Type of FFT
  */
void HSP_ACC_Rfft_WriteParam(hsp_engine_context_t *hmw, float32_t *buff, uint32_t log2Nbp, uint8_t ifftFlag,
                             uint8_t bitrev, uint8_t fftVariant)
{
  HSP_HW_IF_WRITE_PARAMR2((get_fft_attr(log2Nbp - 1U, bitrev, ifftFlag)));
  HSP_HW_IF_WRITE_PARAMR3(((uint32_t)(1U << (log2Nbp - 1U))));
  HSP_HW_IF_WRITE_PARAMR4(((uint32_t)log2Nbp - 1U));
  HSP_HW_IF_WRITE_PARAMR5(((uint32_t)ifftFlag));
  HSP_HW_IF_WRITE_PARAMR6(((uint32_t)fftVariant));
  HSP_HW_IF_WRITE_DCMDPTR0(((uint32_t)buff));
}

/**
  * @brief Write parameters for executing FFT transform
  * @param hmw          HSP middleware context.
  * @param buff         Input and output Buffer
  * @param log2Nbp      log2(number of FFT point)
  * @param ifftFlag     Inverse FFT flag
  * @param bitrev       Bit reverse flag
  */
void HSP_ACC_Fft_WriteParam(hsp_engine_context_t *hmw, float32_t *buff, uint32_t log2Nbp, uint8_t ifftFlag,
                            uint8_t bitrev)
{
  HSP_HW_IF_WRITE_PARAMR2((get_fft_attr(log2Nbp, bitrev, ifftFlag)));
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)(1U << log2Nbp));
  HSP_HW_IF_WRITE_PARAMR4((uint32_t)log2Nbp);
  HSP_HW_IF_WRITE_PARAMR5((uint32_t)ifftFlag);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)buff);
}

/**
  * @brief Write parameters for executing DCT transform
  * @param hmw          HSP middleware context.
  * @param buff         Input and output Buffer
  * @param log2Nbp      log2(number of FFT point)
  */
void HSP_ACC_Dct_WriteParam(hsp_engine_context_t *hmw, float32_t *buff, uint32_t log2Nbp)
{
  HSP_HW_IF_WRITE_PARAMR2((get_fft_attr(log2Nbp, 1U, 0U)));
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)(1U << log2Nbp));
  HSP_HW_IF_WRITE_PARAMR4((uint32_t)log2Nbp);
  HSP_HW_IF_WRITE_PARAMR5((uint32_t)0U);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)buff);
}

/**
  * @brief Write parameters for executing IDCT transform
  * @param hmw          HSP middleware context.
  * @param buff         Input and output Buffer
  * @param log2Nbp      log2(number of FFT point)
  */
void HSP_ACC_IDct_WriteParam(hsp_engine_context_t *hmw, float32_t *buff, uint32_t log2Nbp)
{
  HSP_HW_IF_WRITE_PARAMR2((get_fft_attr(log2Nbp, 1U, 1U)));
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)(1U << log2Nbp));
  HSP_HW_IF_WRITE_PARAMR4((uint32_t)log2Nbp);
  HSP_HW_IF_WRITE_PARAMR5((uint32_t)1U);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)buff);
}

/**
  * @brief Write parameter for function with 1 input param, 1 output param and size param
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address value
  * @param outBuff      Output Buffer address value
  * @param nbSamples    Number of float elements to proceed
  */
void HSP_ACC_VectIOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t outBuff,
                                uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR2(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}

/**
  * @brief Write parameter for function with 2 input param, 1 output param and size param
  * @param hmw          HSP middleware context.
  * @param inABuff      First Input Buffer address
  * @param inBBuff      Second Input Buffer address
  * @param outBuff      Output Buffer address
  * @param nbSamples    Number of float elements to proceed
  */
void HSP_ACC_VectIIOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inABuff, uint32_t inBBuff,
                                 uint32_t outBuff, uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR3(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inABuff);
  HSP_HW_IF_WRITE_DCMDPTR1(inBBuff);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameter for function with 2 input param, 1 output param and size param
  * @param hmw          HSP middleware context.
  * @param inABuff      First Input Buffer address
  * @param inBBuff      Second Input Buffer address
  * @param outBuffA     Output Buffer A address
  * @param outBuffB     Output Buffer B address
  * @param nbSamples    Number of float elements to proceed
  */
void HSP_ACC_VectIIOOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inABuff, uint32_t inBBuff,
                                  uint32_t outBuffA, uint32_t outBuffB, uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR3(outBuffB);
  HSP_HW_IF_WRITE_PARAMR4(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inABuff);
  HSP_HW_IF_WRITE_DCMDPTR1(inBBuff);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuffA);
}

/**
  * @brief Write parameter for function with 1 input param, 1 int value param, 1 output param and 1 size param
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param uValue       Unsigned integer value
  * @param outBuff      Output Buffer address
  * @param nbSamples    Number of float elements in inBuff to proceed
  */
void HSP_ACC_VectIIVOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t uValue, uint32_t outBuff,
                                  uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR1(uValue);
  HSP_HW_IF_WRITE_PARAMR3(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}

/**
  * @brief Write parameter for function with 2 input param, 1 output param and 2 size param
  * @param hmw          HSP middleware context.
  * @param inABuff      First Input Buffer address
  * @param inBBuff      Second Input Buffer address
  * @param outBuff      Output Buffer address
  * @param nbSamplesA   Number of float elements in inABuff to proceed
  * @param nbSamplesB   Number of float elements in inBBuff to proceed
  */
void HSP_ACC_VectIIOSS_WriteParam(hsp_engine_context_t *hmw, uint32_t inABuff, uint32_t inBBuff,
                                  uint32_t outBuff, uint32_t nbSamplesA, uint32_t nbSamplesB)
{
  HSP_HW_IF_WRITE_PARAMR3(nbSamplesA);
  HSP_HW_IF_WRITE_PARAMR4(nbSamplesB);
  HSP_HW_IF_WRITE_DCMDPTR0(inABuff);
  HSP_HW_IF_WRITE_DCMDPTR1(inBBuff);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameter for function with 1 input param, 1 float value param, 1 output param and 1 size param
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param fValue       float value
  * @param outBuff      Output Buffer address
  * @param nbSamples    Number of float elements in inBuff to proceed
  */
void HSP_ACC_VectIFVOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, float32_t fValue,
                                  uint32_t outBuff, uint32_t nbSamples)
{
  hsp_u32_f32_t val;

  val.f32 = fValue;
  HSP_HW_IF_WRITE_PARAMR1(val.u32);
  HSP_HW_IF_WRITE_PARAMR3(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}

/**
  * @brief Write parameters for executing matrix inverse function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param outBuff      Output Buffer address
  * @param nRows        Matrix rows number
  * @param nCols        Matrix columns number
  */
void HSP_ACC_MatInv_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t outBuff, uint32_t nRows,
                               uint32_t nCols)
{
  HSP_HW_IF_WRITE_PARAMR0(nRows);
  HSP_HW_IF_WRITE_PARAMR1(nCols);
  HSP_HW_IF_WRITE_PARAMR3(nRows);
  HSP_HW_IF_WRITE_PARAMR4(nCols);
  HSP_HW_IF_WRITE_PARAMR6(1U);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}

/**
  * @brief Write parameters for executing matrix multiplication function
  * @param hmw          HSP middleware context.
  * @param inABuff      Input Buffer address
  * @param inBBuff      Input Buffer address
  * @param outBuff      Output Buffer address
  * @param nRowsA       First matrix rows number
  * @param nColsA       First matrix columns number
  * @param nRowsB       Second matrix rows number
  * @param nColsB       Second matrix columns number
  */
void HSP_ACC_MatMult_WriteParam(hsp_engine_context_t *hmw, uint32_t inABuff, uint32_t inBBuff,
                                uint32_t outBuff, uint32_t nRowsA, uint32_t nColsA,
                                uint32_t nRowsB, uint32_t nColsB)
{
  HSP_HW_IF_WRITE_PARAMR0(nRowsA);
  HSP_HW_IF_WRITE_PARAMR1(nColsA);
  HSP_HW_IF_WRITE_PARAMR3(nRowsB);
  HSP_HW_IF_WRITE_PARAMR4(nColsB);
  HSP_HW_IF_WRITE_PARAMR6(nRowsA);
  HSP_HW_IF_WRITE_PARAMR7(nColsB);
  HSP_HW_IF_WRITE_DCMDPTR0(inABuff);
  HSP_HW_IF_WRITE_DCMDPTR1(inBBuff);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameters for executing matrix transpose function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param outBuff      Output Buffer address
  * @param nRows        Input matrix rows number
  * @param nCols        Input matrix columns number
  */
void HSP_ACC_MatTrans_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t outBuff, uint32_t nRows,
                                 uint32_t nCols)
{
  HSP_HW_IF_WRITE_PARAMR0(nRows);
  HSP_HW_IF_WRITE_PARAMR1(nCols);
  HSP_HW_IF_WRITE_PARAMR3(nCols);
  HSP_HW_IF_WRITE_PARAMR4(nRows);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}

/**
  * @brief Write parameters for executing FIR function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param coefBBuff    Coefficients Buffer
  * @param stateId      State Buffer identifier
  * @param outBuff      Output Buffer
  * @param nbSamples    Number of float elements to proceed
  */
void HSP_ACC_Filter_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t coefBBuff,
                               hsp_filter_state_identifier_t stateId, uint32_t outBuff, uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR4(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(coefBBuff);
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)((hsp_hw_if_filter_state_t *)stateId)->addrHsp);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);

}

/**
  * @brief Write parameters for executing FIR Decimate function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer
  * @param coefBBuff    Coefficients Buffer
  * @param stateId      State Buffer identifier
  * @param outBuff      Output Buffer
  * @param nbSamples    Number of float elements to proceed
  * @param decimFactor  Decimation factor
  */
void HSP_ACC_FirDecimate_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t coefBBuff,
                                    hsp_filter_state_identifier_t stateId, uint32_t outBuff,
                                    uint32_t nbSamples, uint32_t decimFactor)
{
  HSP_HW_IF_WRITE_PARAMR4(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(coefBBuff);
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)((hsp_hw_if_fir_decimate_filter_state_t *)stateId)->addrHsp);
  HSP_HW_IF_WRITE_PARAMR5(decimFactor);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameters for executing CMSIS Convolution
  * @param hmw          HSP middleware context.
  * @param inABuff      Input A Buffer
  * @param inBBuff      Input B Buffer
  * @param outBuff      Output Buffer
  * @param sizeA        Number of float elements in vectA
  * @param sizeB        Number of float elements in vectB
  */
void HSP_ACC_Conv_WriteParam(hsp_engine_context_t *hmw, uint32_t inABuff, uint32_t inBBuff,
                             uint32_t outBuff, uint32_t sizeA, uint32_t sizeB)
{
  HSP_HW_IF_WRITE_PARAMR3(sizeA);
  HSP_HW_IF_WRITE_PARAMR4(sizeB);
  HSP_HW_IF_WRITE_DCMDPTR0(inABuff);
  HSP_HW_IF_WRITE_DCMDPTR1(inBBuff);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameters for executing FLTBANK filter
  * @param hmw          HSP middleware context.
  * @param spectrCol    Input spectrogram slice of length FFTLen / 2 Buffer
  * @param startIdx     FLTBANK filter pCoefficients start indexes Buffer
  * @param idxSize      FLTBANK filter pCoefficients size indexes Buffer
  * @param coef         FLTBANK filter weights Buffer
  * @param fltbankCol   Output fltbank energies in each filterbank Buffer
  * @param nFltbanks    Number of Fltbank bands to generate
  */
void HSP_ACC_FltBank_WriteParam(hsp_engine_context_t *hmw, uint32_t spectrCol, uint32_t startIdx,
                                uint32_t idxSize, uint32_t coef, uint32_t fltbankCol,
                                uint32_t nFltbanks)
{
  HSP_HW_IF_WRITE_PARAMR5(nFltbanks);
  HSP_HW_IF_WRITE_DCMDPTR0(spectrCol);
  HSP_HW_IF_WRITE_DCMDPTR1(coef);
  HSP_HW_IF_WRITE_PARAMR2(startIdx);
  HSP_HW_IF_WRITE_PARAMR3(idxSize);
  HSP_HW_IF_WRITE_DCMDPTR2(fltbankCol);
}

#if defined(__HSP_DMA__)
/**
  * @brief Write parameters for executing FLTBANK filter with external coefficients
  *        (internal DMA is used to get external coef in dmaBuffId pingpoing buffer)
  * @param hmw          HSP middleware context.
  * @param spectrCol    Input spectrogram slice of length FFTLen / 2 Buffer
  * @param startIdx     FLTBANK filter pCoefficients start indexes Buffer
  * @param idxSize      FLTBANK filter pCoefficients size indexes Buffer
  * @param coef         FLTBANK filter weights Buffer
  * @param fltbankCol   Output fltbank energies in each filterbank Buffer
  * @param nFltbanks    Number of Fltbank bands to generate
  * @param dmaAdd       FLTBANK DMA Buffer address (must be max filter size x2 for pingpong)
  * @param dmaSize      FLTBANK DMA Buffer size (full DMA buffer size (ping + pong))
  */
void HSP_ACC_FltBankExtC_WriteParam(hsp_engine_context_t *hmw, uint32_t spectrCol, uint32_t startIdx,
                                    uint32_t idxSize, uint32_t coef, uint32_t fltbankCol,
                                    uint32_t nFltbanks, uint32_t dmaAdd, uint32_t dmaSize)
{
  HSP_HW_IF_WRITE_PARAMR5(nFltbanks);
  HSP_HW_IF_WRITE_DCMDPTR0(spectrCol);
  HSP_HW_IF_WRITE_PARAMR6(dmaAdd);
  HSP_HW_IF_WRITE_DCMDPTR1(coef);
  HSP_HW_IF_WRITE_PARAMR2(startIdx);
  HSP_HW_IF_WRITE_PARAMR3(idxSize);
  HSP_HW_IF_WRITE_PARAMR7(dmaSize / 2U);
  HSP_HW_IF_WRITE_DCMDPTR2(fltbankCol);
}
#endif /* __HSP_DMA__ */

/**
  * @brief Write parameters for executing LMS filter function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer
  * @param coefBBuff    Coefficients Buffer
  * @param stateId      State Buffer identifier
  * @param outBuff      Output Buffer
  * @param refBuff      Reference Buffer
  * @param errBuff      Error Buffer
  * @param nbSamples    Number of float elements to proceed
  * @param mu           Adaptative factor
  */
void HSP_ACC_Lms_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t coefBBuff,
                            hsp_filter_state_identifier_t stateId, uint32_t outBuff, uint32_t refBuff,
                            uint32_t errBuff, uint32_t nbSamples, float32_t mu)
{
  hsp_u32_f32_t val;
  val.f32 = mu;

  HSP_HW_IF_WRITE_PARAMR6(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)((hsp_hw_if_filter_state_t *)stateId)->addrHsp);
  HSP_HW_IF_WRITE_DCMDPTR1(coefBBuff);
  HSP_HW_IF_WRITE_PARAMR4(refBuff);
  HSP_HW_IF_WRITE_PARAMR5(errBuff);
  HSP_HW_IF_WRITE_PARAMR7(val.u32);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Write parameters for executing IIR Lattice filter function
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer
  * @param skCoeff      Coefficients Buffer
  * @param svCoeff      Coefficients Buffer
  * @param stateId      State Buffer identifier
  * @param outBuff      Output Buffer
  * @param nbSamples    Number of float elements to proceed
  */
void HSP_ACC_IirLattice_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, uint32_t skCoeff,
                                   uint32_t svCoeff, hsp_filter_state_identifier_t stateId,
                                   uint32_t outBuff, uint32_t nbSamples)
{
  HSP_HW_IF_WRITE_PARAMR4(nbSamples);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(skCoeff);
  HSP_HW_IF_WRITE_PARAMR5(svCoeff);
  HSP_HW_IF_WRITE_PARAMR3((uint32_t)((hsp_hw_if_filter_state_t *)stateId)->addrHsp);
  HSP_HW_IF_WRITE_DCMDPTR2(outBuff);
}

/**
  * @brief Generic function to write parameters for reset, set or get a filter state
  * @param hmw          HSP middleware context.
  * @param stateId      Identifier of the filter state
  * @param mulSize      Size multiplier for state buffer
  * @param addSize      Size addition for state buffer
  * @param buffer       Buffer to reset, set or get
  */
void HSP_ACC_StateBuffer_WriteParam(hsp_engine_context_t *hmw, hsp_filter_state_identifier_t stateId,
                                    uint32_t mulSize, uint32_t addSize, uint32_t buffer)
{
  hsp_hw_if_filter_state_t *pFilter = (hsp_hw_if_filter_state_t *)stateId;
  HSP_HW_IF_WRITE_PARAMR1((uint32_t)pFilter->addrHsp);
  HSP_HW_IF_WRITE_PARAMR2((uint32_t)((pFilter->size) * mulSize) + addSize);
  HSP_HW_IF_WRITE_DCMDPTR0(buffer);
}

/**
  * @brief Function to write parameters for reset, set or get of FIR Decimate filter state
  * @param hmw          HSP middleware context.
  * @param stateId      Identifier of the filter state
  * @param buffer       Buffer to reset, set or get
  */
void HSP_ACC_FirDecimateStateBuffer_WriteParam(hsp_engine_context_t *hmw, uint32_t stateId,
                                               uint32_t buffer)
{
  hsp_hw_if_fir_decimate_filter_state_t *pFilter = (hsp_hw_if_fir_decimate_filter_state_t *)stateId;
  HSP_HW_IF_WRITE_PARAMR1((uint32_t)pFilter->addrHsp);
  HSP_HW_IF_WRITE_PARAMR2((uint32_t)pFilter->size);
  HSP_HW_IF_WRITE_DCMDPTR0(buffer);
}

#if defined(__HSP_DMA__)

/**
  * @brief Direct CRC32 computation
  * @param hmw          HSP middleware context.
  * @param pState       Pointer of memory where pState[0] = CRC pState[1] = offset
  * @param pCRC         Pointer on crc computed
  * @param blockSize    Size of block
  * @param posFEOR      Position of the flag indicating when CRC function reaches the end of the ROM.
  * @param posFEOB      Position of the flag indicating when CRC function reaches the end of data block.
  * @param memType      HAL_HSP_CRC_CROM: to run CRC on CROM,
  *                     HAL_HSP_CRC_DROM: to run CRC on DROM
  */
void HSP_ACC_Crc32_WriteParam(hsp_engine_context_t *hmw, uint32_t pState, uint32_t pCRC, uint32_t blockSize,
                              uint32_t posFEOR, uint32_t posFEOB, uint32_t memType)
{
  HSP_HW_IF_WRITE_PARAMR2(blockSize);
  HSP_HW_IF_WRITE_PARAMR3(memType);
  HSP_HW_IF_WRITE_PARAMR4(posFEOR);
  HSP_HW_IF_WRITE_PARAMR5(posFEOB);
  HSP_HW_IF_WRITE_DCMDPTR0(pState);
  HSP_HW_IF_WRITE_DCMDPTR1(pCRC);
}

/**
  * @brief Add memory transfer function from external to internal memory in the current configuration list.
  * @param hmw          HSP middleware context.
  * @param inAddr       Input Buffer Address
  * @param ouAddr       Output Buffer Address
  * @param nbElem       Number of Elements to proceed
  * @param eltFormat    Destination element format to transfer ({HSP_DMA_ELT_FMT_32B 32-bits}
  *                     {HSP_DMA_ELT_FMT_64B 64-bits} {HSP_DMA_ELT_FMT_U16 uint16_t} {HSP_DMA_ELT_FMT_S16 int16_t})
  * @param pingpong     Dest buffer is in ping-pong or single buffer
  * @param dmaChan      DMA channel number
  * @param perIdx       Peripheral channel One channel among 8 can be selected. Allowed values are from 0 to 7.
  * @param pingPl       Ping processing list (only for pingpong option)
  * @param pongPl       Pong processing list (only for pingpong option)
  */
void HSP_ACC_BackgroundExt2Int_WriteParam(hsp_engine_context_t *hmw, uint32_t inAddr, uint32_t ouAddr,
                                          uint32_t nbElem, uint32_t eltFormat, uint32_t pingpong,
                                          uint32_t dmaChan, uint32_t perIdx, uint32_t pingPl, uint32_t pongPl)
{
  HSP_HW_IF_WRITE_PARAMR0(dmaChan);
  HSP_HW_IF_WRITE_PARAMR1(inAddr);
  HSP_HW_IF_WRITE_PARAMR2(nbElem);
  HSP_HW_IF_WRITE_PARAMR3(eltFormat);
  HSP_HW_IF_WRITE_PARAMR4(pingpong);
  HSP_HW_IF_WRITE_PARAMR5(pingPl);
  HSP_HW_IF_WRITE_PARAMR6(pongPl);
  HSP_HW_IF_WRITE_PARAMR7(perIdx);
  HSP_HW_IF_WRITE_PARAMR8(HSP_DMA_DC_PER2MEM);
  HSP_HW_IF_WRITE_DCMDPTR0(ouAddr);
}

/**
  * @brief Add memory transfer function from internal to external memory in the current configuration list.
  * @param hmw          HSP middleware context.
  * @param inAddr       Input Buffer Address
  * @param ouAddr       Output Buffer Address
  * @param nbElem       Number of Elements to proceed
  * @param eltFormat    Destination element format to transfer ({HSP_DMA_ELT_FMT_32B 32-bits}
  *                     {HSP_DMA_ELT_FMT_64B 64-bits} {HSP_DMA_ELT_FMT_U16 uint16_t} {HSP_DMA_ELT_FMT_S16 int16_t})
  * @param pingpong     Source buffer is in ping-pong or single buffer
  * @param dmaChan      DMA channel number
  * @param perIdx       Peripheral channel One channel among 8 can be selected. Allowed values are from 0 to 7.
  * @param pingPl       Ping processing list (only for pingpong option)
  * @param pongPl       Pong processing list (only for pingpong option)
  */
void HSP_ACC_BackgroundInt2Ext_WriteParam(hsp_engine_context_t *hmw, uint32_t inAddr, uint32_t ouAddr,
                                          uint32_t nbElem, uint32_t eltFormat, uint32_t pingpong,
                                          uint32_t dmaChan, uint32_t perIdx, uint32_t pingPl, uint32_t pongPl)
{
  HSP_HW_IF_WRITE_PARAMR0(dmaChan);
  HSP_HW_IF_WRITE_PARAMR1(ouAddr);
  HSP_HW_IF_WRITE_PARAMR2(nbElem);
  HSP_HW_IF_WRITE_PARAMR3(eltFormat);
  HSP_HW_IF_WRITE_PARAMR4(pingpong);
  HSP_HW_IF_WRITE_PARAMR5(pingPl);
  HSP_HW_IF_WRITE_PARAMR6(pongPl);
  HSP_HW_IF_WRITE_PARAMR7(perIdx);
  HSP_HW_IF_WRITE_PARAMR8(HSP_DMA_DC_MEM2PER);
  HSP_HW_IF_WRITE_DCMDPTR0(inAddr);
}

/**
  * @brief Direct memory transfer function to suspend/resume background transfer data from external to internal memory
  * @param hmw          HSP middleware context.
  * @param dmaChan      DMA channel number
  * @param flag         Indicate if suspend or resume.
  *                     - Flag=0 suspend transfer
  *                     - Flag=1 resume transfer
  */
void HSP_ACC_BackgroundSuspendResumeExt2Int_WriteParam(hsp_engine_context_t *hmw, uint32_t dmaChan, uint32_t flag)
{
  HSP_HW_IF_WRITE_PARAMR0(dmaChan);
  HSP_HW_IF_WRITE_PARAMR1(flag);
  HSP_HW_IF_WRITE_DCMDPTR0(flag);
}

/**
  * @brief Write parameter for function with 1 input param, 2 float value param, 1 output param and 1 size param
  * @param hmw          HSP middleware context.
  * @param inBuff       Input Buffer address
  * @param fValueA      float value
  * @param fValueB      float value
  * @param outBuff      Output Buffer address
  * @param nbSamples    Number of float elements in inABuff to proceed
  */
void HSP_ACC_VectIFVFVOS_WriteParam(hsp_engine_context_t *hmw, uint32_t inBuff, float32_t fValueA,
                                    float32_t fValueB, uint32_t outBuff, uint32_t nbSamples)
{
  hsp_u32_f32_t valA;
  hsp_u32_f32_t valB;

  valA.f32 = fValueA;
  valB.f32 = fValueB;
  HSP_HW_IF_WRITE_PARAMR2(valA.u32);
  HSP_HW_IF_WRITE_PARAMR3(nbSamples);
  HSP_HW_IF_WRITE_PARAMR4(valB.u32);
  HSP_HW_IF_WRITE_DCMDPTR0(inBuff);
  HSP_HW_IF_WRITE_DCMDPTR1(outBuff);
}
#endif /* __HSPDMA__ */

/**
  * @brief Counter kernel function to add immediate to global counter index
  * @param hmw          HSP middleware context.
  * @param cntIdx       Counter index [0,15])
  * @param val          Value to add to counter
  * @param type         Counter mode: HSP_CMD_COUNT_MODE_SET or HSP_CMD_COUNT_MODE_ADD
  */
void HSP_ACC_Counter_WriteParam(hsp_engine_context_t *hmw, uint32_t cntIdx, uint32_t val, uint32_t type)
{
  HSP_HW_IF_WRITE_PARAMR0(cntIdx);
  HSP_HW_IF_WRITE_PARAMR1(val);
  HSP_HW_IF_WRITE_PARAMR2(type);
  HSP_HW_IF_WRITE_DCMDPTR0(0U);
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
