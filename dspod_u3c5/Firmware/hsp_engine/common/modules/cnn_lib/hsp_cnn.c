/**
  ******************************************************************************
  * @file hsp_cnn.c
  * @brief API for HSP CNN functions
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
#include "hsp_conf.h"
#include "hsp_cnn.h"
#include "hsp_def.h"
#include "hsp_hw_if.h"
#include "hsp_bram.h"
#include "hsp_bram_if.h"

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @addtogroup HSP_MODULES_CNN
  * @{
  */

#define HSP_MEMSET memset

/* Private types -----------------------------------------------------------*/
/** @defgroup HSP_CNN_Private_Types HSP CNN Private Types
  * @{
  */
typedef uint8_t uint8_mema_t;
typedef int8_t int8_mema_t;
typedef uint8_t uint8_memb_t;
typedef int8_t int8_memb_t;
typedef uint32_t uint32_mema_t;
typedef int32_t int32_mema_t;
typedef uint32_t uint32_memb_t;
typedef int32_t int32_memb_t;

/**
  * @}
  */

/* Private defines -----------------------------------------------------------*/
/** @defgroup HSP_CNN_Private_Defines HSP CNN Private Defines
  * @{
  */
#define HSP_CNN_CDEG_EVT 27U   /**< CDEG event number used for ARM/HSP sync during CNN direct functions */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup HSP_CNN_Private_Functions HSP CNN Private Functions
  * @{
  */
static void free_all_ai(hsp_bram_handle_t *hhsp_bram);
static int8_memb_t *alloc_in_memB(hsp_bram_handle_t *hhsp_bram, uint32_t size_in_byte);
static int8_mema_t *alloc_in_memA(hsp_bram_handle_t *hhsp_bram, uint32_t size_in_byte);
static void align_factor_cmsisnn_fast_ch_v3(float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                            int32_t *p_out_factor, int32_t *p_out_shift);
static void align_factor_cmsisnn_fast_ch_v2(float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                            int32_t *p_bias_data, uint16_t ch_im_out, int32_t *p_out);
static void align_factor_cmsisnn_fast_ch(float32_t in_scale, float32_t out_scale,
                                         const float32_t *p_wt_scale, uint16_t ch_im_out, int32_t *p_out);
/**
  * @}
  */

/** @addtogroup HSP_MODULES_CNN_Exported_Functions
  * @{
  */
/* Exported functions --------------------------------------------------------*/
#if defined(__HSP_DMA__)
/**
  * @brief Execute pointwise CNN convolution with all necessary data loaded into memory.
  *        Coefficients, input and output data are fully loaded into memory before processing begins.
  * @param hmw             HSP middleware context.
  * @param in_w            Input dimension width
  * @param in_h            Input dimension height
  * @param in_c            Input dimension channel
  * @param ou_w            Output dimension width
  * @param ou_h            Output dimension height
  * @param ou_c            Output dimension channel
  * @param stridex         Stride on X
  * @param stridey         Stride on Y
  * @param p_input_data    Input data pointer, int8_t data type
  * @param p_filter_data   Kernel coefficient pointer, int8_t data type
  * @param p_output_data   Output data pointer, int8_t data type
  * @param p_bias_data     Bias data pointer, int32_t data type
  * @param in_scale        Input scale
  * @param out_scale       Output scale
  * @param p_wt_scale      Pointer in weight scales (one per output channel)
  * @param off_in          Input offset, int32_t data type
  * @param off_ou          Output offset, int32_t data type
  * @param sat_min         Min sat (Relu), int32_t data type
  * @param sat_max         Max sat (Relu), int32_t data type
  * @retval                Core status.
  * @details
    @verbatim
    Coefficients and data are loaded before running convolution on HSP
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * in_w * in_h               : Size (word aligned) in bytes of circular input buffer
        ou_c * ou_w * ou_h               : Size (word aligned) in bytes of circular output buffer
        ou_c * 4                         : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2                     : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        ou_c * (in_c + align4)           : Size (word aligned) in bytes of the coefficient buffer. Number of
                                           input channel are aligned to a multiple of 4 for memory allocation
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvPointwise0_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max)
{
  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVPW_I8);

  int8_mema_t *pDst ;
  int8_mema_t *pSrcA;
  int8_memb_t *pSrcB;
  int32_mema_t *pQS;
  int32_mema_t *pBias;
  uint32_t jump_in = 0U;
  uint32_t moreChanIn = (0x4U - (in_c & 0x3U)) & 0x3U;

  if (stridey != 1U)
  {
    jump_in = (in_c * in_w);
  }
  /* Allocate and copy input data in MemA */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), (in_c * in_w * in_h))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate output data buffer in MemA */
  if ((pDst = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * ou_w * ou_h))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy bias data in MemA */
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy kernel data in MemB */
  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (ou_c * (in_c + moreChanIn)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy quantification data in MemA. interleave quant_params */
  if ((pQS = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  /* Parameter must be written before accessing blocking register */
  HSP_HW_IF_WRITE_PARAMR0(in_w);
  HSP_HW_IF_WRITE_PARAMR1(in_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(ou_w);
  HSP_HW_IF_WRITE_PARAMR4(ou_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_c);
  HSP_HW_IF_WRITE_PARAMR7(stridex);
  HSP_HW_IF_WRITE_PARAMR8(stridey);
  HSP_HW_IF_WRITE_PARAMR9((uint32_t)off_in);
  HSP_HW_IF_WRITE_PARAMR10((uint32_t)off_ou);
  HSP_HW_IF_WRITE_PARAMR11((uint32_t)sat_min);
  HSP_HW_IF_WRITE_PARAMR12((uint32_t)sat_max);
  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQS);
  HSP_HW_IF_WRITE_PARAMR15(HSP_CNN_CFG_MODE_0STEP);

  int8_mema_t *pStartSrc = (int8_mema_t *)pSrcA;
  int8_mema_t *pCurrSrc = pStartSrc;
  int8_mema_t *pStartDst = (int8_mema_t *)pDst;
  int8_mema_t *pCurrDst = pStartDst;

  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  int8_t *pTmpOutput = (int8_t *)p_output_data;
  uint32_t inLineSize = (in_c * in_w);
  int8_t *pInput8 = (int8_t *)p_input_data;

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQS);

  /* Load all coeffs */
  if (moreChanIn)
  {
    int8_memb_t *pDstCoeffTmp = pSrcB;
    int8_t *pInCoeffTmp = p_filter_data;
    int8_t zeroArray[3] = {0, 0, 0};
    for (uint32_t i = 0; i < ou_c; i++)
    {
      /* For each coeff add moreChanIn 0 coefficient */
      HSP_MEMCPY(pDstCoeffTmp, pInCoeffTmp, in_c);
      pDstCoeffTmp += in_c;
      pInCoeffTmp += in_c;
      /* Add extra 0 coefficients */
      HSP_MEMCPY(pDstCoeffTmp, zeroArray, moreChanIn);
      pDstCoeffTmp += moreChanIn;
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (ou_c * in_c));
  }

  HSP_MEMCPY((int8_t *)pBias, (int8_t *)p_bias_data, (ou_c * sizeof(uint32_t)));
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);

  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send CNN event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /* Load only necessary lines (depending of stride) in circular buffer. */
  for (uint32_t i = 0; i < ou_h; i++)
  {
    HSP_MEMCPY(pCurrSrc, pInput8, (in_w * in_c));
    pInput8 +=  inLineSize + jump_in;
    pCurrSrc +=  inLineSize;
  }
  /* Start working: go */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  /*  Wait output line  */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /*  Copy output from HSP to CPU buffer  */
  HSP_MEMCPY(pTmpOutput, pCurrDst, (ou_h * ou_w * ou_c));

  HSP_HW_IF_CNN_End(hmw->hdriver);
  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @brief Execute pointwise CNN convolution in 1 step.
  *        Coefficients are fully loaded into memory, and input and output data are loaded into a circular buffer.
  *        Step: Process input lines using all coefficients while loading the next line into the circular buffer.
  *
  * @param hmw                  HSP middleware context.
  * @param in_w                 Input dimension width
  * @param in_h                 Input dimension height
  * @param in_c                 Input dimension channel
  * @param ou_w                 Output dimension width
  * @param ou_h                 Output dimension height
  * @param ou_c                 Output dimension channel
  * @param stridex              Stride on X
  * @param stridey              Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_filter_data        Kernel coefficient pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param p_bias_data          Bias data pointer, int32_t data type (one per output channel)
  * @param in_scale             Input scale
  * @param out_scale            Output scale
  * @param p_wt_scale           Pointer in weight scales (one per output channel)
  * @param off_in               Input offset, int32_t data type
  * @param off_ou               Output offset, int32_t data type
  * @param sat_min              Min sat (Relu), int32_t data type
  * @param sat_max              Max sat (Relu), int32_t data type
  * @retval                     Core status.
  * @details
  * This variant supports number of channel <= 4
  * Coefficients are loaded before running convolution on HSP.
  * Input and output data are loaded in a circular buffer containing exactly 2 input/output lines
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * in_w * 2        : Size (word aligned) in bytes of circular input buffer
        ou_c * ou_w * 2        : Size (word aligned) in bytes of circular output buffer
        ou_c * 4               : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2           : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        ou_c * (in_c + align4) : Size (word aligned) in bytes of the coefficient buffer. Number of
                                 input channel are aligned to a multiple of 4 for memory allocation
    Constraints:
      1  : Minimal input width
      1  : Minimal input height
      1  : Minimal output width
      1  : Minimal output height
      1  : Minimal number of input channel
      1  : Minimal number of output channel
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvPointwise1_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max)
{
  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVPW_I8);

  int8_mema_t *pDst ;
  int8_mema_t *pSrcA;
  int8_memb_t *pSrcB;
  int32_mema_t *pBias;
  int32_mema_t *pQS;
  uint32_t jump_in = 0U;
  uint32_t moreChanIn = (0x4U - (in_c & 0x3U)) & 0x3U;

  if (stridey != 1U)
  {
    jump_in = in_c * in_w;
  }

  /* Allocate and copy input data in MemA: 2 input lines in circular buffer  */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), ((in_c * in_w)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate output data buffer in MemA: ping pong buffer on destination  */
  if ((pDst = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), ((ou_c * ou_w)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy bias data in MemA */
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy kernel data in MemB */
  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (ou_c * (in_c + moreChanIn)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy quantification data in MemA interleave quant_params */
  if ((pQS = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  /* Parameter must be written before accessing blocking register */
  HSP_HW_IF_WRITE_PARAMR0(in_w);
  HSP_HW_IF_WRITE_PARAMR1(in_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(ou_w);
  HSP_HW_IF_WRITE_PARAMR4(ou_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_c);
  HSP_HW_IF_WRITE_PARAMR7(stridex);
  HSP_HW_IF_WRITE_PARAMR8(stridey);
  HSP_HW_IF_WRITE_PARAMR9((uint32_t)off_in);
  HSP_HW_IF_WRITE_PARAMR10((uint32_t)off_ou);
  HSP_HW_IF_WRITE_PARAMR11((uint32_t)sat_min);
  HSP_HW_IF_WRITE_PARAMR12((uint32_t)sat_max);
  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQS);
  /* Set the circular input and output size buffer */
  HSP_HW_IF_WRITE_PARAMR15((2U << HSP_CNN_CFG_NB_OU_LINE_SHIFT) |
                           (2U << HSP_CNN_CFG_NB_IN_LINE_SHIFT) | HSP_CNN_CFG_MODE_1STEP);
  int8_mema_t *pStartSrc = (int8_mema_t *)pSrcA;
  int8_mema_t *pCurrSrc = pStartSrc; /* input */
  int8_mema_t *pStartDst = (int8_mema_t *)pDst; /* pDst */
  int8_mema_t *pCurrDst = pStartDst;
  int8_mema_t *pEndSrc;
  int8_mema_t *pEndDst;

  pEndSrc = pStartSrc + (2U * (in_c * in_w));
  pEndDst = pStartDst + (2U * (ou_c * ou_w));

  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  int8_t *pTmpOutput = (int8_t *)p_output_data;
  uint32_t inLineSize = (in_c * in_w);
  uint32_t nbLine = (in_h + (stridey - 1U)) / stridey;
  uint32_t outLineSize = (ou_c * ou_w);
  int8_t *pInput8 = (int8_t *)p_input_data;

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQS);

  /* Load all coeffs */
  if (moreChanIn)
  {
    int8_memb_t *pDstCoeffTmp = pSrcB;
    int8_t *pInCoeffTmp = p_filter_data;
    int8_t zeroArray[3] = {0, 0, 0};
    for (uint32_t i = 0; i < ou_c; i++)
    {
      /* For each coeff add moreChanIn 0 coefficient */
      HSP_MEMCPY(pDstCoeffTmp, pInCoeffTmp, in_c);
      pDstCoeffTmp += in_c;
      pInCoeffTmp += in_c;
      /* Add extra 0 coefficients */
      HSP_MEMCPY(pDstCoeffTmp, zeroArray, moreChanIn);
      pDstCoeffTmp += moreChanIn;
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (ou_c  * in_c));
  }

  HSP_MEMCPY((int8_t *)pBias, (int8_t *)p_bias_data, (ou_c * sizeof(uint32_t)));

  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send CNN event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /* Load 2 lines in circular buffer with stride_h respect */
  HSP_MEMCPY(pCurrSrc, pInput8, inLineSize);
  pInput8 +=  inLineSize + jump_in;
  pCurrSrc +=  inLineSize;
  HSP_MEMCPY(pCurrSrc, pInput8, inLineSize);
  pInput8 +=  inLineSize + jump_in;  /* Jump over stride */
  pCurrSrc = pStartSrc;

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  for (uint32_t i = 0; i < nbLine - 1; i++)
  {
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

    /* Load next line in circular buffer while HSP is working */
    HSP_MEMCPY(pCurrSrc, pInput8, (inLineSize));
    pCurrSrc += inLineSize;
    pInput8 +=  inLineSize;
    /* Jump over stride */
    pInput8 += jump_in;
    if (pCurrSrc == pEndSrc)
    {
      pCurrSrc = pStartSrc;
    }
    /* Copy output while HSP is working */
    HSP_MEMCPY(pTmpOutput, pCurrDst, outLineSize);
    pTmpOutput += outLineSize;
    pCurrDst += outLineSize;
    if (pCurrDst == pEndDst)
    {
      /* Go back to start buffer */
      pCurrDst = pStartDst;
    }
  }
  /* Wait output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  /* Copy output from HSP to CPU buffer */
  HSP_MEMCPY(pTmpOutput, pCurrDst, outLineSize);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all CNN allocated memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute pointwise CNN convolution in 2 steps.
  *        Coefficients are fully loaded, input and output data are loaded into a circular buffer.
  *        First step: Load all coefficients, while HSP works on the first output line.
  *        Second step: Process input lines using all coefficients while loading the next line into the circular
  *        buffer.
  *
  * @param hmw                  HSP middleware context.
  * @param in_w                 Input dimension width
  * @param in_h                 Input dimension height
  * @param in_c                 Input dimension channel
  * @param ou_w                 Output dimension width
  * @param ou_h                 Output dimension height
  * @param ou_c                 Output dimension channel
  * @param stridex              Stride on X
  * @param stridey              Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_filter_data        Kernel coefficient pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param p_bias_data          Bias data pointer, int32_t data type
  * @param in_scale             Input scale
  * @param out_scale            Output scale
  * @param p_wt_scale           Pointer in weight scales (one per output channel)
  * @param off_in               Input offset, int32_t data type
  * @param off_ou               Output offset, int32_t data type
  * @param sat_min              Min sat (Relu), int32_t data type
  * @param sat_max              Max sat (Relu), int32_t data type
  * @param nb_line_per_blocks   Nb line to output in buffer
  * @retval                     Core status.
  * @details
    @verbatim
    This variant does not support number of channel <= 4
    Input and output data are loaded in a circular buffer containing exactly 2 input/output lines
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * in_w * 2                  : Size (word aligned) in bytes of circular input buffer
        ou_c * ou_w * nb_line_per_blocks : Size (word aligned) in bytes of circular output buffer
        ou_c * 4                         : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2                     : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        ou_c * (in_c + align4)           : Size (word aligned) in bytes of the coefficient buffer. Number of
                                           input channel are aligned to a multiple of 4 for memory allocation
    Constraints:
      2 : Minimal number of nb_line_per_blocks (exception: for ou_h = 1, nb_line_per_blocks = 1)
      1 : Minimal input width
      1 : Minimal input height
      1 : Minimal output width
      1 : Minimal output height
      5 : Minimal number of input channel
      2 : Minimal number of output channel
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvPointwise2_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t nb_line_per_blocks)
{
  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVPW_I8);

  int8_mema_t *pDst ;
  int8_mema_t *pSrcA;
  int8_memb_t *pSrcB;
  int32_mema_t *pBias;
  int32_mema_t *pQS;

  uint32_t nbinLine = 2U; /* For circular mode */
  uint32_t circiSize = (in_c * in_w) * nbinLine;
  uint32_t moreChanIn = (0x4U - (in_c & 0x3U)) & 0x3U;

  /* Allocate and copy input data in MemA */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate output data buffer in MemA */
  if ((pDst = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * ou_w) * nb_line_per_blocks)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy bias data in MemA */
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy kernel data in MemB */
  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (ou_c * (in_c + moreChanIn)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy quantification data in MemA interleaved quant_params */
  if ((pQS = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  /* Parameter must be written before accessing blocking register */
  HSP_HW_IF_WRITE_PARAMR0(in_w);
  HSP_HW_IF_WRITE_PARAMR1(in_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(ou_w);
  HSP_HW_IF_WRITE_PARAMR4(ou_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_c);
  HSP_HW_IF_WRITE_PARAMR7(stridex);
  HSP_HW_IF_WRITE_PARAMR8(stridey);
  HSP_HW_IF_WRITE_PARAMR9((uint32_t)off_in);
  HSP_HW_IF_WRITE_PARAMR10((uint32_t)off_ou);
  HSP_HW_IF_WRITE_PARAMR11((uint32_t)sat_min);
  HSP_HW_IF_WRITE_PARAMR12((uint32_t)sat_max);
  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQS);
  /* Set circular buffer size and pw cmd mode */
  HSP_HW_IF_WRITE_PARAMR15((nb_line_per_blocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) |
                           (nbinLine << HSP_CNN_CFG_NB_IN_LINE_SHIFT) | HSP_CNN_CFG_MODE_2STEP);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQS);

  HSP_MEMCPY((int8_t *)pBias, (int8_t *)p_bias_data, (ou_c * sizeof(uint32_t)));

  /* Load only first channel out coeffs */
  int8_t *pKe = p_filter_data;

  if (moreChanIn)
  {
    int8_memb_t *pDstCoeffTmp = pSrcB;
    int8_t *pInCoeffTmp = p_filter_data;
    int8_t zeroArray[3] = {0, 0, 0};
    /* Add moreChanIn 0 in first set of coefficient */
    HSP_MEMCPY(pDstCoeffTmp, pInCoeffTmp, in_c);
    pDstCoeffTmp += in_c;
    pInCoeffTmp += in_c;
    /* Add extra 0 coefficients */
    HSP_MEMCPY(pDstCoeffTmp, zeroArray, moreChanIn);
    pDstCoeffTmp += moreChanIn;
  }
  else
  {
    HSP_MEMCPY(pSrcB, pKe, in_c);
  }

  /* Load first input tile */
  int8_t *pIn = p_input_data;
  uint32_t inLineSize8 = in_c * in_w; /* 1 input line */
  HSP_MEMCPY(pSrcA, pIn, (inLineSize8));
  pIn += (inLineSize8  * stridey);
  int8_t *pOu = p_output_data;

  HSP_HW_IF_WaitEndOfDirectCommand(hmw);

  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send CNN event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint8_t *pKerCur = (uint8_t *)pKe + in_c;
  uint8_t *pSrcbCur = (uint8_t *)pSrcB + in_c;
  uint8_t *pSrcEnd = (uint8_t *)pSrcA + (circiSize);
  uint8_t *pSrcCur = (uint8_t *)pSrcA + (inLineSize8);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nb_line_per_blocks * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  /* Loop to load all kernels in BRAM */
  if (moreChanIn)
  {
    int8_memb_t *pDstCoeffTmp = pSrcB  + in_c +  moreChanIn;
    int8_t *pInCoeffTmp = (int8_t *)pKerCur;
    int8_t zeroArray[3] = {0, 0, 0};
    for (uint32_t i = 1; i < ou_c; i++)
    {
      /* For each coeff add moreChanIn 0 coefficient */
      HSP_MEMCPY(pDstCoeffTmp, pInCoeffTmp, in_c);
      pDstCoeffTmp += in_c;
      pInCoeffTmp += in_c;
      /* Add extra 0 coefficients */
      HSP_MEMCPY(pDstCoeffTmp, zeroArray, moreChanIn);
      pDstCoeffTmp += moreChanIn;

      /* Wait output channel done */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */
      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
  }
  else
  {
    for (uint32_t out_num = 1; out_num < ou_c; out_num++)
    {
      /* Copy kernel block */
      HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKerCur, in_c);
      pKerCur += in_c;
      pSrcbCur += in_c;
      /* Wait output channel done */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */
      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
  }

  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)pIn, inLineSize8);
    pIn += (inLineSize8 * stridey);
    pSrcCur += inLineSize8;
    if (pSrcCur == pSrcEnd)
    {
      pSrcCur = (uint8_t *)pSrcA;
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    HSP_MEMCPY((int8_t *)pOu, (int8_t *)pDstCur, outLineSize8);
    pOu += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = (uint8_t *)pDst;
    }
  }
  /* Wait last output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  HSP_MEMCPY((int8_t *)pOu, (int8_t *)pDstCur, outLineSize8);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute pointwise CNN convolution in 3 steps.
  *        Coefficients are loaded into a ping-pong buffer, input and output data are loaded into a circular buffer.
  *        First step: Process all input lines with first coefficient while receiving lines from the ARM,
  *                    synchronize with HSP on the input line.
  *        Second step: Process all channel (excluding the first and last) while loading coefficients
  *                     into the ping-pong buffer.
  *        Third step: Process the last channel, synchronization with HSP is done on output line transfer.
  *        Repeat these 3 steps until all output lines are processed.
  *
  * @param hmw                  HSP middleware context.
  * @param in_w                 Input dimension width
  * @param in_h                 Input dimension height
  * @param in_c                 Input dimension channel
  * @param ou_w                 Output dimension width
  * @param ou_h                 Output dimension height
  * @param ou_c                 Output dimension channel
  * @param stridex              Stride on X
  * @param stridey              Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_filter_data        Kernel coefficient pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param p_bias_data          Bias data pointer, int32_t data type
  * @param in_scale             Input scale
  * @param out_scale            Output scale
  * @param p_wt_scale           Pointer in weight scales (one per output channel)
  * @param off_in               Input offset, int32_t data type
  * @param off_ou               Output offset, int32_t data type
  * @param sat_min              Min sat (Relu), int32_t data type
  * @param sat_max              Max sat (Relu), int32_t data type
  * @param nb_line_per_blocks   Nb line to output in buffer
  * @retval                     Core status.
  * @details
    @verbatim
    This variant does not support number of channel <= 4
    This variant does not support number of channel out <= 2
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * in_w * nb_line_per_blocks : Size (word aligned) in bytes of circular input buffer
        ou_c * 4                         : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2                     : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        ou_c * ou_w * nb_line_per_blocks : Size (word aligned) in bytes of circular output buffer
        ou_c * (in_c + align4)           : Size (word aligned) in bytes of the coefficient buffer. Number of
                                           input channel are aligned to a multiple of 4 for memory allocation
    Constraints:
      2 : Minimal number of nb_line_per_blocks
      2 : Minimal input width
      2 : Minimal input height
      2 : Minimal output width
      2 : Minimal output height
      5 : Minimal number of input channel
      3 : Minimal number of output channel
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvPointwise3_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t nb_line_per_blocks)
{
  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVPW_I8);

  int8_memb_t *pDst;
  int8_mema_t *pSrcA;
  int8_memb_t *pSrcB;
  int32_mema_t *pBias;
  int32_mema_t *pQS;
  uint32_t moreChanIn = (0x4U - (in_c & 0x3U)) & 0x3U;

  uint32_t circiSize = nb_line_per_blocks * (in_c * in_w); /* For circular mode */
  /* Allocate and copy input data in MemA */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy bias data in MemA */
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy kernel data in MemB : Only 2 kernels in ping pong */
  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), ((in_c + moreChanIn) * 2))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate output data buffer in MemB */
  if ((pDst = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (ou_c * ou_w) * nb_line_per_blocks)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Allocate and copy quantification data in MemA interleave quant_params */
  if ((pQS = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(in_w);
  HSP_HW_IF_WRITE_PARAMR1(in_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(ou_w);
  HSP_HW_IF_WRITE_PARAMR4(ou_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_c);
  HSP_HW_IF_WRITE_PARAMR7(stridex);
  HSP_HW_IF_WRITE_PARAMR8(stridey);
  HSP_HW_IF_WRITE_PARAMR9((uint32_t)off_in);
  HSP_HW_IF_WRITE_PARAMR10((uint32_t)off_ou);
  HSP_HW_IF_WRITE_PARAMR11((uint32_t)sat_min);
  HSP_HW_IF_WRITE_PARAMR12((uint32_t)sat_max);
  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQS);
  /* Set circular buffer size and pw cmd mode */
  HSP_HW_IF_WRITE_PARAMR15((nb_line_per_blocks << HSP_CNN_CFG_NB_IN_LINE_SHIFT) | HSP_CNN_CFG_MODE_3STEP);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQS);

  HSP_MEMCPY((int8_t *)pBias, (int8_t *)p_bias_data, (ou_c * sizeof(uint32_t)));
  /* Load only first channel out coeffs */
  int8_t *pKe = p_filter_data;

  int8_t zeroArray[3] = {0, 0, 0};
  if (moreChanIn)
  {
    int8_memb_t *pDstCoeffTmp = pSrcB;
    int8_t *pInCoeffTmp = p_filter_data;
    int8_t zeroArray[3] = {0, 0, 0};
    /* Add moreChanIn 0 in first set of coefficient */
    HSP_MEMCPY(pDstCoeffTmp, pInCoeffTmp, in_c);
    pDstCoeffTmp += in_c;
    pInCoeffTmp += in_c;
    /* Add extra 0 coefficients */
    HSP_MEMCPY(pDstCoeffTmp, zeroArray, moreChanIn);
    pDstCoeffTmp += moreChanIn;
  }
  else
  {
    HSP_MEMCPY(pSrcB, pKe, in_c);
  }
  /* Load first input tile */
  int8_t *pIn = p_input_data;
  uint32_t inLineSize8 = in_c * in_w; /* 1 input line */
  HSP_MEMCPY(pSrcA, pIn, (inLineSize8));
  pIn += (inLineSize8  * stridey);
  int8_t *pOu = p_output_data;

  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send CNN event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  uint8_t *pSrcEnd = (uint8_t *)pSrcA + circiSize;
  uint8_t *pSrcCur = (uint8_t *)pSrcA + inLineSize8;
  uint8_t *pDstCur = (uint8_t *)pDst;
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nb_line_per_blocks * ou_c * ou_w));
  uint8_t *pKerCur = (uint8_t *)pKe + in_c;
  uint8_t *pSrcbCur = (uint8_t *)pSrcB + (in_c + moreChanIn);
  uint8_t *pSrcbEnd = (uint8_t *)pSrcB + ((in_c + moreChanIn) * 2);
  uint32_t nbLinesBlck = 0; /* Init with one block for more simple comparison at end of while */
  uint32_t nbLineLoop = nb_line_per_blocks;

  if (pSrcCur == pSrcEnd)
  {
    pSrcCur = (uint8_t *)pSrcA;
  }
  do
  {
    if (nbLinesBlck)
    {
      if (moreChanIn)
      {
        /* Load all coeffs needed for first channel out */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKe, in_c);
        pKerCur = (uint8_t *)pKe + in_c;
        pSrcbCur += in_c;
        /* Add extra 0 coefficients */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)zeroArray, moreChanIn);
        pSrcbCur += moreChanIn;
      }
      else
      {
        /* Load all coeffs needed for first channel out */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKe, in_c);
        pKerCur = (uint8_t *)pKe + in_c;
        pSrcbCur += in_c;
      }
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = (uint8_t *)pSrcB;
      }

      /* Copy input line in circular buffer before enter loop */
      HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)pIn, inLineSize8);
      pIn += (inLineSize8 * stridey);
      pSrcCur += inLineSize8;
      if (pSrcCur == pSrcEnd)
      {
        pSrcCur = (uint8_t *)pSrcA;
      }
    }
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    nbLinesBlck += nbLineLoop;
    if (nbLinesBlck > ou_h)
    {
      /* Count how may output lines not done by block */
      nbLinesBlck -= nbLineLoop; /* Remove next block size */
      nbLineLoop = ou_h - nbLinesBlck;
      nbLinesBlck += nbLineLoop;
    }
    /* First step: load all input block */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Copy input line in circular buffer */
      HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)pIn, inLineSize8);
      pIn += (inLineSize8 * stridey);
      pSrcCur += inLineSize8;
      if (pSrcCur == pSrcEnd)
      {
        pSrcCur = (uint8_t *)pSrcA;
      }
      /* Wait output line */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */
      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
    /* Whole input block is now in BRAM loop on all channels: Loop on each ping pong kernel */
    for (uint32_t out_num = 1; out_num < ou_c; out_num++)
    {
      if (moreChanIn)
      {
        /* Load all coeffs needed for first channel out */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKerCur, in_c);
        pKerCur += in_c;
        pSrcbCur += in_c;
        /* Add extra 0 coefficients */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)zeroArray, moreChanIn);
        pSrcbCur += moreChanIn;
      }
      else
      {
        /* First of all copy kernel block */
        HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKerCur, in_c);
        pKerCur += in_c;
        pSrcbCur += in_c;
      }
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = (uint8_t *)pSrcB;
      }
      /* Wait output channel done */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */
      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }

    /* Then read output lines in BRAM */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Wait output line */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */
      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
      HSP_MEMCPY((int8_t *)pOu, (int8_t *)pDstCur, outLineSize8);
      pOu += outLineSize8;
      pDstCur += outLineSize8;
      if (pDstCur == pDstEnd)
      {
        pDstCur = (uint8_t *)pDst;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */
    HSP_MEMCPY((int8_t *)pOu, (int8_t *)pDstCur, outLineSize8);
    pOu += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = (uint8_t *)pDst;
    }
  } while (nbLinesBlck < ou_h); /* End of input block loop */

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

#if defined(__HSP_DMA__)
/**
  * @brief Execute conv2D CNN convolution with all necessary data loaded into memory.
  *        Coefficients, input and output data are fully loaded into memory before processing begins.
  *
  * @param hmw                 HSP middleware context.
  * @param in_w                Input dimension width
  * @param in_h                Input dimension height
  * @param in_c                Input dimension channel
  * @param k_w                 Kernel dimension width
  * @param k_h                 Kernel dimension height
  * @param ou_w                Output dimension width
  * @param ou_h                Output dimension height
  * @param ou_c                Output dimension channel
  * @param stridex             Stride on X
  * @param stridey             Stride on Y
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param p_wt_scale          Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @param padx_l              Pad on X left
  * @param padx_r              Pad on X right
  * @param pady_t              Pad on Y top
  * @param pady_b              Pad on Y bottom
  * @retval                    Core status.
  * @details
    @verbatim
    Coefficients and data are loaded before running convolution on HSP
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
        In BRAM_A:
        (in_w + (padx_l + padx_r)) * (in_h + (pady_t + pady_b)) * in_c : Input lines size (word aligned) in bytes
        ou_w * ou_h * ou_c                                             : Output lines size (word aligned) in bytes
        ou_c * 3 * 4                                                   : Quantification and bias size (word aligned)
                                                                         in bytes
        In BRAM_B:
        (ou_c * k_h * ((k_w * in_c) + nbPadK0))                        : Filter coefficients size (word aligned) in bytes
    Constraints:
      Support all filter sizes except WHC: 1x1x1, 1x2x1, 2x1x1, 1x3x1, 3x1x1, 1x4x1
    @endverbatim
 */
hsp_core_status_t HSP_ACC_CnnConv2d0_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c, uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_mema_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONV2D_I8);

  /* Then update all parameters */
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);
  uint32_t k_c = in_c * ou_c;
  uint32_t nbPadK0 = (4U - ((k_w * in_c) & 0x3U)) & 0x3U;

  /* Put kernels in MEMB */
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (ou_c * k_h * ((k_w * in_c) + nbPadK0)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), inp_w * inp_h * in_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMA */
  if ((pDst  = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), ou_w * ou_h * ou_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ  = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), (3 * ou_c) * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t) pBQ);
  HSP_HW_IF_WRITE_PARAMR15(HSP_CNN_CFG_MODE_0STEP);

  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load all coeffs */
  if (nbPadK0)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    int8_t *tmpKe = p_filter_data;
    for (uint32_t i = 0; i < (ou_c * k_h); i++)
    {
      HSP_MEMCPY(pSrcB, tmpKe, (in_c * k_w));
      tmpKe += (in_c * k_w);
      pSrcB += (in_c * k_w);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *pSrcB++ = 0;
      }
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (k_c * k_h * k_w));
  }

  align_factor_cmsisnn_fast_ch_v2(in_scale, out_scale, p_wt_scale, (int32_t *)p_bias_data, (uint16_t)ou_c,
                                  (int32_t *)pBQ);
  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  uint8_mema_t *inPad = pSrcA;
  /* First lines pad */
  /* Pad one full dest line */
  HSP_MEMSET(inPad, val, (pady_t *(in_w + (padx_l + padx_r)) * in_c));
  inPad += (pady_t *(in_w + (padx_l + padx_r)) * in_c);
  /* Middle lines to pad */
  for (uint32_t i = 0; i < in_h; i++)
  {
    /* First column pad */
    HSP_MEMSET(inPad, val, (padx_l * in_c));
    inPad += (padx_l * in_c);
    /* Copy image line */
    HSP_MEMCPY(inPad, p_input_data, (in_w * in_c));
    inPad += (in_w * in_c);
    p_input_data += (in_w * in_c);
    /* Last column pad */
    HSP_MEMSET(inPad, val, (padx_r * in_c));
    inPad += (padx_r * in_c);
  }
  /* Last lines pad */
  HSP_MEMSET(inPad, val, (pady_b * (in_w + (padx_l + padx_r)) * in_c));

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  /* Wait direct command done */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /* Copy result in external memory */
  HSP_MEMCPY(p_output_data, pDst, (ou_c * ou_h * ou_w));

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}
#endif /* __HSP_DMA__ */

/**
  * @brief Execute conv2D CNN convolution in 1 step.
  *        Coefficients are fully loaded into memory, and input and output data are loaded into a circular buffer.
  *        Step: Process input lines using all coefficients while loading the next line into the circular buffer.
  *
  * @param hmw                    HSP middleware context.
  * @param in_w                   Input dimension width
  * @param in_h                   Input dimension height
  * @param in_c                   Input dimension channel
  * @param k_w                    Kernel dimension width
  * @param k_h                    Kernel dimension height
  * @param ou_w                   Output dimension width
  * @param ou_h                   Output dimension height
  * @param ou_c                   Output dimension channel (ou_c > 0)
  * @param stridex                Stride on X
  * @param stridey                Stride on Y
  * @param p_input_data           Input data pointer, int8_t data type
  * @param p_filter_data          Kernel coefficient pointer, int8_t data type
  * @param p_output_data          Output data pointer, int8_t data type
  * @param p_bias_data            Bias data pointer, int32_t data type
  * @param in_scale               Input scale
  * @param out_scale              Output scale
  * @param p_wt_scale             Pointer in weight scales (one per output channel)
  * @param off_in                 Input offset, int32_t data type
  * @param off_ou                 Output offset, int32_t data type
  * @param sat_min                Min sat (Relu), int32_t data type
  * @param sat_max                Max sat (Relu), int32_t data type
  * @param padx_l                 Pad on X left
  * @param padx_r                 Pad on X right
  * @param pady_t                 Pad on Y top
  * @param pady_b                 Pad on Y bottom
  * @param nb_line_per_blocks_m2  Number of output lines per output buffer - 2 (0 is for ping-pong)
  * @retval                       Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * (in_w + (padx_l + padx_r)) * (k_h + stridey)  : Input lines size (word aligned) in bytes
        (nb_line_per_blocks_m2 + 2) * ou_w * ou_c            : Output lines size (word aligned) in bytes
        ou_c * 3 * 4                                         : Quantification and bias size (word aligned) in bytes
      In BRAM_B:
        (ou_c * k_h * ((k_w * in_c) + 3))                    : Filter coefficients size (word aligned) in bytes
    Constraints:
      Support all sizes except WHC: 1x1x1, 1x2x1, 2x1x1, 1x3x1, 3x1x1, 1x4x1
      No restriction on number of output line
      nb_line_per_blocks_m2 is only used to keep severals output lines in BRAM, no perf impact set 0 for standard use
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConv2d1_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks_m2)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_mema_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONV2D_I8);

  uint32_t pad = padx_l + padx_r + pady_t + pady_b;
  /* Then update all parameters */
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);
  uint32_t nbLinePerBlocks = nb_line_per_blocks_m2 + 2U;
  if (nbLinePerBlocks > ou_h)
  {
    nbLinePerBlocks = ou_h;
  }
  uint32_t k_c = in_c * ou_c;
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  uint32_t nbPadK0 = (4U - ((k_w * in_c) & 0x3U)) & 0x3U;

  /* Put kernels in MEMB */
#if defined(__HSP_DMA__)
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (ou_c * k_h * ((k_w * in_c) + nbPadK0)))) == NULL)
#else /* __HSP_DMA__ */
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (ou_c * k_h * ((k_w * in_c) + 3)))) == NULL)
#endif /* __HSP_DMA__ */
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMA */
  if ((pDst = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), nbLinePerBlocks * outLineSize8)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), (3 * ou_c) * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t) pBQ);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nbLinePerBlocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_1STEP);
  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load all coeffs */
  if (nbPadK0)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    int8_t *tmpKe = p_filter_data;
    for (uint32_t i = 0; i < (ou_c * k_h); i++)
    {
      HSP_MEMCPY(pSrcB, tmpKe, (in_c * k_w));
      tmpKe += (in_c * k_w);
      pSrcB += (in_c * k_w);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *pSrcB++ = 0;
      }
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (k_c * k_h * k_w));
  }

  align_factor_cmsisnn_fast_ch_v2(in_scale, out_scale, p_wt_scale, (int32_t *)p_bias_data, (uint16_t)ou_c,
                                  (int32_t *)pBQ);

  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  if (pad)
  {
    /* Need to add padding */
    uint32_t nbPadTop = 0U;
    uint8_mema_t *inPad = pSrcA;
    /* First lines pad */
    HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
    /* Middle lines to pad */
    uint32_t lineToPad = k_h - pady_t;
    if (lineToPad > in_h)
    {
      nbPadTop = lineToPad - in_h;
      lineToPad =  in_h;
    }
    for (uint32_t i = 0; i < lineToPad; i++)
    {
      /* First column pad */
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
      /* Copy image line */
      HSP_MEMCPY(inPad, p_input_data, (in_w * in_c));
      inPad += (in_w * in_c);
      p_input_data += (in_w * in_c);
      /* Last column pad */
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
    /* Add bottom pad if necessary */
    if (nbPadTop)
    {
      /* last lines pad */
      HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
      inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
    }
  }
  else
  {
    /* No padding */
    HSP_MEMCPY(pSrcA, p_input_data, (inLineSize8 * k_h));
    p_input_data += (inLineSize8 * k_h);
  }

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }

  uint8_t *pSrcEnd = pSrcA + (circiSize);
  uint8_t *pSrcCur = pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nbLinePerBlocks * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);
  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    for (uint32_t sidx = 0; sidx < stridey; sidx++)
    {
      if (countlines == countmax)
      {
        /* Last lines pad */
        HSP_MEMSET(pSrcCur, val, inLineSize8);
        pSrcCur += inLineSize8;
      }
      else
      {
        countlines++;
        if (pad)
        {
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
          pSrcCur += xloop;
          p_input_data += xloop;
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
        else
        {
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, inLineSize8);
          p_input_data += inLineSize8;
          pSrcCur += inLineSize8;
        }
        if (pSrcCur == pSrcEnd)
        {
          pSrcCur = pSrcA;
        }
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = pDst;
    }
  }
  /* Wait last output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}


/**
  * @brief Execute conv2D CNN convolution in 2 step.
  *        Coefficients are fully loaded, input and output data are loaded into a circular buffer.
  *        First step: Load all coefficients, while HSP works on the first output line.
  *        Second step: Process input lines using all coefficients while loading the next line into the circular
  *        buffer.
  *
  * @param hmw                    HSP middleware context.
  * @param in_w                   Input dimension width
  * @param in_h                   Input dimension height
  * @param in_c                   Input dimension channel
  * @param k_w                    Kernel dimension width
  * @param k_h                    Kernel dimension height
  * @param ou_w                   Output dimension width
  * @param ou_h                   Output dimension height
  * @param ou_c                   Output dimension channel (ou_c > 1 for CROM 1.1.7)
  * @param stridex                Stride on X
  * @param stridey                Stride on Y
  * @param p_input_data           Input data pointer, int8_t data type
  * @param p_filter_data          Kernel coefficient pointer, int8_t data type
  * @param p_output_data          Output data pointer, int8_t data type
  * @param p_bias_data            Bias data pointer, int32_t data type
  * @param in_scale               Input scale
  * @param out_scale              Output scale
  * @param p_wt_scale             Pointer in weight scales (one per output channel)
  * @param off_in                 Input offset, int32_t data type
  * @param off_ou                 Output offset, int32_t data type
  * @param sat_min                Min sat (Relu), int32_t data type
  * @param sat_max                Max sat (Relu), int32_t data type
  * @param padx_l                 Pad on X left
  * @param padx_r                 Pad on X right
  * @param pady_t                 Pad on Y top
  * @param pady_b                 Pad on Y bottom
  * @param nb_line_per_blocks_m2  Number of output lines per output buffer - 2 (0 is for ping-pong)
  * @retval                       Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        (in_c * (in_w + (padx_l + padx_r))) * (k_h + stridey)  : Input lines size (word aligned) in bytes
        (nb_line_per_blocks_m2 + 2) * ou_w * ou_c              : Output lines size (word aligned) in bytes
        ou_c * 3 * 4                                           : Quantification and bias size (word aligned) in bytes
      In BRAM_B:
        (ou_c * k_h * ((k_w * in_c) + 3))                      : Filter coefficients size (word aligned) in bytes
    Constraints:
      Support all sizes except WHC: 1x1x1, 1x2x1, 2x1x1, 1x3x1, 3x1x1, 1x4x1, 3x3x1, 3x3x3
      Restriction on output line: one line not supported for CROM 1.1.7
      nb_line_per_blocks_m2 is only used to keep severals output lines in BRAM, no perf impact set 0 for standard use
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConv2d2_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks_m2)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_mema_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONV2D_I8);

  uint32_t pad = padx_l + padx_r + pady_t + pady_b; /*  Pad option */
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);
  uint32_t nbLinePerBlocks = nb_line_per_blocks_m2 + 2U;
  if (nbLinePerBlocks > ou_h)
  {
    nbLinePerBlocks = ou_h;
  }
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  uint32_t nbPadK0 = (4U - ((k_w * in_c) & 0x3U)) & 0x3U;

  /* Put kernels in MEMB */
#if defined(__HSP_DMA__)
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (ou_c * k_h * ((k_w * in_c) + nbPadK0)))) == NULL)
#else /* __HSP_DMA__ */
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (ou_c * k_h * ((k_w * in_c) + 3)))) == NULL)
#endif /* __HSP_DMA__ */
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *) alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMA */
  if ((pDst  = (uint8_mema_t *) alloc_in_memA(&(hmw->hbram), nbLinePerBlocks * outLineSize8)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ  = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), (3U * ou_c) * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t) pBQ);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nbLinePerBlocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_2STEP);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load only first channel out coeffs */
  if (nbPadK0)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    int8_t *tmpKe = p_filter_data;
    for (uint32_t i = 0; i < k_h; i++)
    {
      HSP_MEMCPY(pSrcB, tmpKe, (in_c * k_w));
      tmpKe += (in_c * k_w);
      pSrcB += (in_c * k_w);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *pSrcB++ = 0;
      }
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (k_w * k_h * in_c));
    pSrcB += (k_w * k_h * in_c);
  }

  align_factor_cmsisnn_fast_ch_v2(in_scale, out_scale, p_wt_scale, (int32_t *)p_bias_data, (uint16_t)ou_c,
                                  (int32_t *)pBQ);

  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  if (pad)
  {
    uint32_t nbPadTop = 0U;
    uint8_mema_t *inPad = pSrcA;
    /* First lines pad */
    HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
    /* Middle lines to pad */
    uint32_t lineToPad = k_h - pady_t;
    if (lineToPad > in_h)
    {
      nbPadTop = lineToPad - in_h;
      lineToPad =  in_h;
    }
    for (uint32_t i = 0; i < lineToPad; i++)
    {
      /* First column pad */
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
      /* Copy image line */
      HSP_MEMCPY(inPad, p_input_data, (in_w * in_c));
      inPad += (in_w * in_c);
      p_input_data += (in_w * in_c);
      /* Last column pad */
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
    /* Add bottom pad if necessary */
    if (nbPadTop)
    {
      /* last lines pad */
      HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
      inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
    }
  }
  else
  {
    HSP_MEMCPY(pSrcA, p_input_data, (inLineSize8 * k_h));
    p_input_data += (inLineSize8 * k_h);
  }
  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send CNN event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);
  uint8_t *pKerCur = (uint8_t *)(p_filter_data + (k_h * k_w * in_c));
  uint8_t *pSrcbCur = pSrcB;
  uint8_t *pSrcEnd = pSrcA + (circiSize);
  uint8_t *pSrcCur = pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nbLinePerBlocks * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;
#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  /* Loop to load all kernels in BRAM */
  for (uint32_t out_num = 1; out_num < ou_c; out_num++)
  {
    /* Copy kernel block */
    if (nbPadK0)
    {
      /* Not aligned on 4, add extra kernels to 0 */
      for (uint32_t i = 0; i < k_h; i++)
      {
        HSP_MEMCPY(pSrcbCur, pKerCur, (in_c * k_w));
        pKerCur += (in_c * k_w);
        pSrcbCur += (in_c * k_w);
        for (uint32_t kp = 0; kp < nbPadK0; kp++)
        {
          *pSrcbCur++ = 0;
        }
      }
    }
    else
    {
      HSP_MEMCPY(pSrcbCur, pKerCur, (k_w * k_h * in_c));
      pKerCur += (k_w * k_h * in_c);
      pSrcbCur += (k_w * k_h * in_c);
    }
    /* Wait output channel done */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  }

  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    for (uint32_t sidx = 0; sidx < stridey; sidx++)
    {
      if (countlines == countmax)
      {
        /* Last lines pad */
        HSP_MEMSET(pSrcCur, val, inLineSize8);
        pSrcCur += inLineSize8;
      }
      else
      {
        countlines++;
        if (pad)
        {
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
          pSrcCur += xloop;
          p_input_data += xloop;
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
        else
        {
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, inLineSize8);
          p_input_data += inLineSize8;
          pSrcCur += inLineSize8;
        }
        if (pSrcCur == pSrcEnd)
        {
          pSrcCur = pSrcA;
        }
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = pDst;
    }
  }
  /* Wait last output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute conv2D CNN convolution in 3 step.
  *        Coefficients are loaded into a ping-pong buffer, input and output data are loaded into a circular buffer.
  *        First step: Process all input lines with first coefficient while receiving lines from the ARM,
  *                    synchronize with HSP on the input line.
  *        Second step: Process all channel (excluding the first and last) while loading coefficients
  *                     into the ping-pong buffer.
  *        Third step: Process the last channel, synchronization with HSP is done on output line transfer.
  *        Repeat these 3 steps until all output lines are processed.
  *
  * @param hmw                 HSP middleware context.
  * @param in_w                Input dimension width
  * @param in_h                Input dimension height
  * @param in_c                Input dimension channel
  * @param k_w                 Kernel dimension width
  * @param k_h                 Kernel dimension height
  * @param ou_w                Output dimension width
  * @param ou_h                Output dimension height
  * @param ou_c                Output dimension channel (ou_c > 0)
  * @param stridex             Stride on X
  * @param stridey             Stride on Y
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param p_wt_scale          Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @param padx_l              Pad on X left
  * @param padx_r              Pad on X right
  * @param pady_t              Pad on Y top
  * @param pady_b              Pad on Y bottom
  * @param nb_line_per_blocks  Number of output lines per output buffer
  * @retval                    Core status.
  * @details
    @verbatim
    Buffer footprint:
      In BRAM_A:
        ((nb_line_per_blocks * stridey) + k_h - stridey) * (in_w + padx) * in_c  : Input lines size (word aligned)
                                                                                   in bytes
        ou_c * 3 * 4                                                             : Quantification and bias size
                                                                                   (word aligned) in bytes
      In BRAM_B:
        (2 * k_h * (((k_w * in_c) + 3)))                                         : Filter coefficients size
                                                                                   (word aligned) in bytes
        nb_line_per_blocks * ou_w * ou_c                                         : Output linessize (word aligned)
                                                                                   in bytes
    Constraints:
      Support all sizes except: ((k_w * in_c) < 5) WHC: 3x3x1, 3x3x3
      No restriction of on output line
      For best performance, find biggest nb_line_per_blocks according to BRAM area
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConv2d3_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_memb_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONV2D_I8);

  uint32_t pad = padx_l + padx_r + pady_t + pady_b; /*  Pad option */
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);
  uint32_t nbLineInPerBlocks = (nb_line_per_blocks * stridey) + k_h - stridey; /* For circular mode */
  if (nb_line_per_blocks > ou_h)
  {
    nb_line_per_blocks = ou_h;
  }
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = nbLineInPerBlocks; /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  uint32_t nbPadK0 = (4U - ((k_w * in_c) & 0x3U)) & 0x3U;

  /* Put kernels in MEMB */
#if defined(__HSP_DMA__)
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (2 * k_h * ((k_w * in_c) + nbPadK0)))) == NULL)
#else /* __HSP_DMA__ */
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), (2 * k_h * ((k_w * in_c) + 3)))) == NULL)
#endif /* __HSP_DMA__ */
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *) alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMB */
  if ((pDst  = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), nb_line_per_blocks * outLineSize8)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ  = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), (3U * ou_c) * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t) pBQ);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nb_line_per_blocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_3STEP);
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t) pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t) pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t) pDst);

  /* Load all coeffs needed for first channel out */
  if (nbPadK0)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    int8_t *tmpKe = p_filter_data;
    uint8_memb_t *tmpB = pSrcB;
    for (uint32_t i = 0; i < k_h; i++)
    {
      HSP_MEMCPY(tmpB, tmpKe, (in_c * k_w));
      tmpKe += (in_c * k_w);
      tmpB += (in_c * k_w);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *tmpB++ = 0;
      }
    }
  }
  else
  {
    HSP_MEMCPY(pSrcB, p_filter_data, (k_w * k_h * in_c));
  }

  align_factor_cmsisnn_fast_ch_v2(in_scale, out_scale, p_wt_scale, (int32_t *)p_bias_data, (uint16_t)ou_c,
                                  (int32_t *)pBQ);

  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  if (pad)
  {
    uint32_t nbPadTop = 0U;
    uint8_mema_t *inPad = pSrcA;
    /* First lines pad */
    HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
    /* Middle lines to pad */
    uint32_t lineToPad = k_h - pady_t;
    if (lineToPad > in_h)
    {
      nbPadTop = lineToPad - in_h;
      lineToPad =  in_h;
    }
    for (uint32_t i = 0; i < lineToPad; i++)
    {
      /* First column pad */
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
      /* Copy image line */
      HSP_MEMCPY(inPad, p_input_data, (in_w * in_c));
      inPad += (in_w * in_c);
      p_input_data += (in_w * in_c);
      /* Last column pad */
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
    /* Add bottom pad if necessary */
    if (nbPadTop)
    {
      /* last lines pad */
      HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
      inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
    }
  }
  else
  {
    HSP_MEMCPY(pSrcA, p_input_data, (inLineSize8 * k_h));
    p_input_data += (inLineSize8 * k_h);
  }

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  uint8_t *pSrcEnd = pSrcA + (circiSize);
  uint8_t *pSrcCur = pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstCur = pDst;
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nb_line_per_blocks * ou_c * ou_w));
  uint8_t *pKerCur = (uint8_t *)(p_filter_data + (k_w * k_h * in_c));
  uint8_t *pSrcbCur = pSrcB + (k_h * ((in_c * k_w) + nbPadK0));
  uint8_t *pSrcbEnd = pSrcB + (k_h * ((in_c * k_w) + nbPadK0) * 2);
  uint32_t nbLinesBlck = 0U; /* Init with one block for more simple comparison at end of while */
  uint32_t nbLineLoop = nb_line_per_blocks;
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);
  do
  {
    if (nbLinesBlck)
    {
      /* Load all coeffs needed for first channel out */
      pKerCur = (uint8_t *) p_filter_data;
      if (nbPadK0)
      {
        /* Not aligned on 4, add extra kernels to 0 */
        for (uint32_t i = 0; i < k_h; i++)
        {
          HSP_MEMCPY(pSrcbCur, pKerCur, (in_c * k_w));
          pKerCur += (in_c * k_w);
          pSrcbCur += (in_c * k_w);
          for (uint32_t kp = 0; kp < nbPadK0; kp++)
          {
            *pSrcbCur++ = 0;
          }
        }
      }
      else
      {
        HSP_MEMCPY(pSrcbCur, pKerCur, (k_w * k_h * in_c));
        pKerCur += (k_w * k_h * in_c);
        pSrcbCur += (k_w * k_h * in_c);
      }
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = pSrcB;
      }
      /* Copy input line in circular buffer before enter loop */
      for (uint32_t sidx = 0; sidx < stridey; sidx++)
      {
        if (countlines == countmax)
        {
          /* Last lines pad */
          HSP_MEMSET(pSrcCur, val, inLineSize8);
          pSrcCur += inLineSize8;
        }
        else
        {
          countlines++;
          if (pad)
          {
            /* First column pad */
            HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
            pSrcCur += (padx_l * in_c);
            HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
            pSrcCur += xloop;
            p_input_data += xloop;
            /* Last column pad */
            HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
            pSrcCur += (padx_r * in_c);
          }
          else
          {
            HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, inLineSize8);
            p_input_data += inLineSize8;
            pSrcCur += inLineSize8;
          }
          if (pSrcCur == pSrcEnd)
          {
            pSrcCur = pSrcA;
          }
        }
      }
    }
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    nbLinesBlck += nbLineLoop;
    if (nbLinesBlck > ou_h)
    {
      /* Count how may output lines not done by block */
      nbLinesBlck -= nbLineLoop; /* Remove next block size */
      nbLineLoop = ou_h - nbLinesBlck;
      nbLinesBlck += nbLineLoop;
    }
    /* First step: load all input block */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Copy input line in circular buffer */
      for (uint32_t sidx = 0; sidx < stridey; sidx++)
      {
        if (countlines == countmax)
        {
          /* Last lines pad */
          HSP_MEMSET(pSrcCur, val, inLineSize8);
          pSrcCur += inLineSize8;
        }
        else
        {
          countlines++;
          if (pad)
          {
            /* First column pad */
            HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
            pSrcCur += (padx_l * in_c);
            HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
            pSrcCur += xloop;
            p_input_data += xloop;
            /* Last column pad */
            HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
            pSrcCur += (padx_r * in_c);
          }
          else
          {
            HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, inLineSize8);
            p_input_data += inLineSize8;
            pSrcCur += inLineSize8;
          }
          if (pSrcCur == pSrcEnd)
          {
            pSrcCur = pSrcA;
          }
        }
      }
      /* Wait output line */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
    /* Whole input block is now in BRAM loop on all channels: Loop on each ping pong kernel */
    for (uint32_t out_num = 1; out_num < ou_c; out_num++)
    {
      /* First of all copy kernel block */
      if (nbPadK0)
      {
        /* Not aligned on 4, add extra kernels to 0 */
        for (uint32_t i = 0; i < k_h; i++)
        {
          HSP_MEMCPY(pSrcbCur, pKerCur, (in_c * k_w));
          pKerCur += (in_c * k_w);
          pSrcbCur += (in_c * k_w);
          for (uint32_t kp = 0; kp < nbPadK0; kp++)
          {
            *pSrcbCur++ = 0;
          }
        }
      }
      else
      {
        HSP_MEMCPY(pSrcbCur, pKerCur, (k_w * k_h * in_c));
        pKerCur += (k_w * k_h * in_c);
        pSrcbCur += (k_w * k_h * in_c);
      }
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = pSrcB;
      }
      /* Wait output channel done */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }

    /* Then read output lines in BRAM */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Wait output line */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif  /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
      HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
      p_output_data += outLineSize8;
      pDstCur += outLineSize8;
      if (pDstCur == pDstEnd)
      {
        pDstCur = pDst;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */

    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = pDst;
    }
  } while (nbLinesBlck < ou_h); /* End of input block loop */

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute depthwise CNN convolution in 1 step.
  *        Coefficients are fully loaded into memory, and input and output data are loaded into a circular buffer.
  *        Step: Process input lines using all coefficients while loading the next line into the circular buffer.
  *
  * @param hmw                 HSP middleware context.
  * @param in_w                Input dimension width
  * @param in_h                Input dimension height
  * @param in_c                Input dimension channel
  * @param k_w                 Kernel dimension width
  * @param k_h                 Kernel dimension height
  * @param ou_w                Output dimension width
  * @param ou_h                Output dimension height
  * @param ou_c                Output dimension channel
  * @param stridex             Stride on X
  * @param stridey             Stride on Y
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param p_wt_scale          Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @param padx_l              Pad on X left
  * @param padx_r              Pad on X right
  * @param pady_t              Pad on Y top
  * @param pady_b              Pad on Y bottom
  * @retval                    Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
      (in_c * (in_w + (padx_l + padx_r))) * (k_h + stridey) : Size (word aligned) in bytes of circular input buffer
      ou_c * ou_w * 2                                       : Size (word aligned) in bytes of circular output buffer
      ou_c * 4                                              : Size (word aligned) in bytes of bias buffer
      ou_c * 4 * 2                                          : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
      (in_c + align4) * k_h * k_w                           : Size (word aligned) in bytes of the coefficient buffer
                                                              Number of input channel are aligned to a multiple of 4
                                                              for memory allocation
    Constraints:
      2        : Minimal input width
      2        : Minimal input height
      1        : Minimal output width
      1        : Minimal output height
      1        : Minimal number of input channel
      1        : Minimal number of output channel
      2x1, 1x2 : Minimal coefficient size (HxW) supported
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvDepthwise1_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t k_w, uint32_t k_h,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b)
{
  int8_memb_t *pSrcB;
  int8_mema_t *pSrcA;
  int8_mema_t *pDst;
  int32_mema_t *pBias;
  int32_mema_t *pQuant;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVDW_I8);

  /* Then update all parameters */
  uint32_t pad = padx_l + padx_r + pady_t + pady_b;
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8((stridex << 16U) | (stridey & 0x000000ff));
  HSP_HW_IF_WRITE_PARAMR9(off_in);
  HSP_HW_IF_WRITE_PARAMR10(off_ou);
  HSP_HW_IF_WRITE_PARAMR11(sat_min);
  HSP_HW_IF_WRITE_PARAMR12(sat_max);

  uint32_t k_c = in_c;
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  uint32_t nbPadK0 = (4U - (in_c & 0x3U)) & 0x3U;

  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (in_c  + nbPadK0) * k_h * k_w)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Ping pong buffer on destination */
  if ((pDst  = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), (2U * outLineSize8))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pQuant = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias); /* bias followed by quant */
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQuant);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (2U << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_1STEP);

  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load all coeffs */
  /* Depthwise works 4 by 4 then we need to load (4*k_h*k_w) kernel */
  int8_t *ptmp = (int8_t *) p_filter_data;
  int8_memb_t *pSrcB8 = (int8_memb_t *) pSrcB;

  int8_t *ptmpKe = (int8_t *) ptmp;
  for (uint32_t out_num = 0; out_num < (ou_c >> 2U); out_num++)
  {
    ptmpKe = (int8_t *) ptmp;
    for (uint32_t j = 0; j < (k_h * k_w); j++)
    {
      *pSrcB8++ = ptmpKe[0];
      *pSrcB8++ = ptmpKe[1];
      *pSrcB8++ = ptmpKe[2];
      *pSrcB8++ = ptmpKe[3];
      ptmpKe += k_c;
    }
    ptmp += 4; /* next 4 channels */
  }
  if (nbPadK0)
  {
    ptmpKe = (int8_t *) ptmp;
    for (uint32_t j = 0; j < (k_h * k_w); j++)
    {
      *pSrcB8++ = ptmpKe[0];
      if (nbPadK0 == 1)
      {
        *pSrcB8++ = ptmpKe[1];
        *pSrcB8++ = ptmpKe[2];
        *pSrcB8++ = 0;
      }
      if (nbPadK0 == 2)
      {
        *pSrcB8++ = ptmpKe[1];
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
      }
      if (nbPadK0 == 3)
      {
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
      }
      ptmpKe += k_c;
    }
  }

  /* Now copy biais */
  for (uint32_t i = 0; i < ou_c; i++)
  {
    *pBias++ = p_bias_data[i];
  }

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQuant);

  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  uint8_mema_t *inPad = (uint8_mema_t *)pSrcA;
  uint32_t nbPadTop = 0U;

  /* First lines pad */
  HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
  inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
  uint32_t lineToPad = k_h - pady_t;
  if (lineToPad > in_h)
  {
    nbPadTop = lineToPad - in_h;
    lineToPad =  in_h;
  }
  /* Middle lines to pad */
  for (uint32_t i = 0; i < lineToPad; i++)
  {
    /* First column pad */
    if (pad)
    {
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
    }
    /* Copy image line */
    HSP_MEMCPY((int8_t *)inPad, (int8_t *)p_input_data, (in_w * in_c));
    inPad += (in_w * in_c);
    p_input_data += (in_w * in_c);
    /* Last column pad */
    if (pad)
    {
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
  }
  /* Add bottom pad if necessary */
  if (nbPadTop)
  {
    /* last lines pad */
    HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
  }

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint8_t *pSrcEnd = (uint8_t *)pSrcA + (circiSize);
  uint8_t *pSrcCur = (uint8_t *)pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (2U * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    for (uint32_t sidx = 0; sidx <  stridey; sidx++)
    {
      if (countlines == countmax)
      {
        /* Last lines pad */
        HSP_MEMSET(pSrcCur, val, inLineSize8);
        pSrcCur += inLineSize8;
      }
      else
      {
        countlines++;
        if (pad)
        {
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
        }
        HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
        pSrcCur += xloop;
        p_input_data += xloop;
        if (pad)
        {
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
      }
      if (pSrcCur == pSrcEnd)
      {
        pSrcCur = (uint8_t *)pSrcA;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif  /* STM32H7P5xx */
    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = (uint8_t *)pDst;
    }
  }
  /* Wait last output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute depthwise CNN convolution in 2 steps.
  *        Coefficients are fully loaded, input and output data are loaded into a circular buffer.
  *        First step: Load all coefficients, while HSP works on the first output line.
  *        Second step: Process input lines using all coefficients while loading the next line into the circular
  *        buffer.
  *
  * @param hmw                 HSP middleware context.
  * @param in_w                Input dimension width
  * @param in_h                Input dimension height
  * @param in_c                Input dimension channel
  * @param k_w                 Kernel dimension width
  * @param k_h                 Kernel dimension height
  * @param ou_w                Output dimension width
  * @param ou_h                Output dimension height
  * @param ou_c                Output dimension channel
  * @param stridex             Stride on X
  * @param stridey             Stride on Y
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data        Kernel coefficient pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param p_bias_data          Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param p_wt_scale          Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @param padx_l              Pad on X left
  * @param padx_r              Pad on X right
  * @param pady_t              Pad on Y top
  * @param pady_b              Pad on Y bottom
  * @param nb_line_per_blocks  Number of output lines per output buffer
  * @retval                    Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        (in_c * (in_w + (padx_l + padx_r))) * (k_h + stridey) : Size (word aligned) in bytes of circular input buffer
        ou_c * ou_w * nb_line_per_blocks                      : Size (word aligned) in bytes of circular output buffer
        ou_c * 4                                              : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2                                          : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        (in_c + align4) * k_h * k_w                           : Size (word aligned) in bytes of the coefficient buffer
                                                                Number of input channel are aligned to a multiple
                                                                of 4 for memory allocation
    Constraints:
      2        : nb_line_per_blocks (exception: for ou_h = 1, nb_line_per_blocks = 1)
      2        : Minimal input width
      2        : Minimal input height
      1        : Minimal output width
      1        : Minimal output height
      1        : Minimal number of input channel
      1        : Minimal number of output channel
      2x1, 1x2 : Minimal coefficient size (HxW) supported
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnConvDepthwise2_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t k_w, uint32_t k_h,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                               uint32_t nb_line_per_blocks)
{
  int8_memb_t *pSrcB;
  int8_mema_t *pSrcA;
  int8_mema_t *pDst;
  int32_mema_t *pBias;
  int32_mema_t *pQuant;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVDW_I8);

  /* Then update all parameters */
  uint32_t pad = padx_l + padx_r + pady_t + pady_b;
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8((stridex << 16U) | (stridey & 0x000000ff));
  HSP_HW_IF_WRITE_PARAMR9(off_in);
  HSP_HW_IF_WRITE_PARAMR10(off_ou);
  HSP_HW_IF_WRITE_PARAMR11(sat_min);
  HSP_HW_IF_WRITE_PARAMR12(sat_max);

  uint32_t k_c = in_c;
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  uint32_t nbPadK0 = (4U - (in_c & 0x3U)) & 0x3U;

  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (in_c  + nbPadK0) * k_h * k_w)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* 2 input lines in circular buffer */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* ping pong buffer on destination */
  if ((pDst  = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), (nb_line_per_blocks * outLineSize8))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pQuant = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias); /* bias followed by quant */
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQuant);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nb_line_per_blocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_2STEP);

  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t) pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t) pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t) pDst);

  /* Depthwise works  4 by 4 then we need to load (4*k_h*k_w) kernel for first line */
  int8_t *ptmp = (int8_t *)p_filter_data;

  int8_memb_t *pSrcB8 = (int8_memb_t *) pSrcB;
  int8_t *ptmpKe = (int8_t *) ptmp;
  if (nbPadK0 && (ou_c <= 4))
  {
    for (uint32_t j = 0; j < (k_h * k_w); j++)
    {
      *pSrcB8++ = ptmpKe[0];
      if (nbPadK0 == 1)
      {
        *pSrcB8++ = ptmpKe[1];
        *pSrcB8++ = ptmpKe[2];
        *pSrcB8++ = 0;
      }
      if (nbPadK0 == 2)
      {
        *pSrcB8++ = ptmpKe[1];
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
      }
      if (nbPadK0 == 3)
      {
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
        *pSrcB8++ = 0;
      }
      ptmpKe += k_c;
    }
  }
  else
  {
    for (uint32_t j = 0; j < (k_h * k_w); j++)
    {
      *pSrcB8++ = ptmpKe[0];
      *pSrcB8++ = ptmpKe[1];
      *pSrcB8++ = ptmpKe[2];
      *pSrcB8++ = ptmpKe[3];
      ptmpKe += k_c;
    }
  }
  ptmp += 4; /* next 4 channels */

  /* Now copy bias */
  for (uint32_t i = 0; i < ou_c; i++)
  {
    *pBias++ = p_bias_data[i];
  }

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQuant);

  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  uint8_mema_t *inPad = (uint8_mema_t *)pSrcA;
  uint32_t nbPadTop = 0U;

  if (pad)
  {
    /* First lines pad */
    HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
  }
  /* Middle lines to pad */
  uint32_t lineToPad = k_h - pady_t;
  if (lineToPad > in_h)
  {
    nbPadTop = lineToPad - in_h;
    lineToPad =  in_h;
  }
  for (uint32_t i = 0; i < lineToPad; i++)
  {
    if (pad)
    {
      /* First column pad */
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
    }
    /* Copy image line */
    HSP_MEMCPY((int8_t *)inPad, (int8_t *)p_input_data, (in_w * in_c));
    inPad += (in_w * in_c);
    p_input_data += (in_w * in_c);
    if (pad)
    {
      /* Last column pad */
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
  }
  /* Add bottom pad if necessary */
  if (nbPadTop)
  {
    /* last lines pad */
    HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
  }
  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint8_t *pSrcEnd = (uint8_t *)pSrcA + (circiSize);
  uint8_t *pSrcCur = (uint8_t *)pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nb_line_per_blocks * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);

#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  /* Loop to load all remaining kernels minus 4 already loaded in BRAM */
  for (uint32_t out_num = 4; out_num < ou_c; out_num += 4)
  {
    /* Copy 4 kernel by 4 kernel */
    ptmpKe = (int8_t *) ptmp;
    if (nbPadK0 && ((out_num + 4) >= ou_c))
    {
      for (uint32_t j = 0; j < (k_h * k_w); j++)
      {
        *pSrcB8++ = ptmpKe[0];
        if (nbPadK0 == 1)
        {
          *pSrcB8++ = ptmpKe[1];
          *pSrcB8++ = ptmpKe[2];
          *pSrcB8++ = 0;
        }
        if (nbPadK0 == 2)
        {
          *pSrcB8++ = ptmpKe[1];
          *pSrcB8++ = 0;
          *pSrcB8++ = 0;
        }
        if (nbPadK0 == 3)
        {
          *pSrcB8++ = 0;
          *pSrcB8++ = 0;
          *pSrcB8++ = 0;
        }
        ptmpKe += k_c;
      }
    }
    else
    {
      for (uint32_t j = 0; j < (k_h * k_w); j++)
      {
        *pSrcB8++ = ptmpKe[0];
        *pSrcB8++ = ptmpKe[1];
        *pSrcB8++ = ptmpKe[2];
        *pSrcB8++ = ptmpKe[3];
        ptmpKe += k_c;
      }
    }
    ptmp += 4; /* next 4 channels */

    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  }
  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    for (uint32_t sidx = 0; sidx <  stridey; sidx++)
    {
      if (countlines == countmax)
      {
        /* Last lines pad */
        HSP_MEMSET(pSrcCur, val, inLineSize8);
        pSrcCur += inLineSize8;
      }
      else
      {
        countlines++;
        if (pad)
        {
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
        }
        HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
        pSrcCur += xloop;
        p_input_data += xloop;
        if (pad)
        {
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
      }
      if (pSrcCur == pSrcEnd)
      {
        pSrcCur = (uint8_t *)pSrcA;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = (uint8_t *)pDst;
    }
  }
  /* Wait output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute depthwise CNN convolution in 3 steps.
  *        Coefficients are loaded into a ping-pong buffer, input and output data are loaded into a circular buffer.
  *        First step: Process all input lines with first coefficient while receiving lines from the ARM,
  *                    synchronize with HSP on the input line.
  *        Second step: Process all channel (excluding the first and last) while loading coefficients
  *                     into the ping-pong buffer.
  *        Third step: Process the last channel, synchronization with HSP is done on output line transfer.
  *        Repeat these 3 steps until all output lines are processed.
  *
  * @param hmw                 HSP middleware context.
  * @param in_w                Input dimension width
  * @param in_h                Input dimension height
  * @param in_c                Input dimension channel
  * @param k_w                 Kernel dimension width
  * @param k_h                 Kernel dimension height
  * @param ou_w                Output dimension width
  * @param ou_h                Output dimension height
  * @param ou_c                Output dimension channel
  * @param stridex             Stride on X
  * @param stridey             Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param p_wt_scale          Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @param padx_l              Pad on X left
  * @param padx_r              Pad on X right
  * @param pady_t              Pad on Y top
  * @param pady_b              Pad on Y bottom
  * @param nb_line_per_blocks  Nb line to output in buffer
  * @retval                    Core status.
  * @details
    @verbatim
    This variant does not support number of channel <= 8
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        (in_c * (in_w + (padx_l + padx_r))) *
        ((nb_line_per_blocks * stridey) + k_h - stridey) : Size (word aligned) in bytes of circular input buffer
        ou_c * 4                                         : Size (word aligned) in bytes of bias buffer
        ou_c * 4 * 2                                     : Size (word aligned) in bytes of quantification buffer
      In BRAM_B:
        ou_c * ou_w * nb_line_per_blocks                 : Size (word aligned) in bytes of circular output buffer
        (4 * k_h * k_w) * 2)                             : Size (word aligned) in bytes of the coefficient buffer
    Constraints:
      1        : nb_line_per_blocks
      2        : Minimal input width
      2        : Minimal input height
      1        : Minimal output width
      1        : Minimal output height
      9        : Minimal number of input channel
      9        : Minimal number of output channel
      2x1, 1x2 : Minimal coefficient size (HxW) supported
    @endverbatim
 */
hsp_core_status_t HSP_ACC_CnnConvDepthwise3_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t k_w, uint32_t k_h,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                               uint32_t nb_line_per_blocks)
{
  int8_memb_t *pSrcB;
  int8_mema_t *pSrcA;
  int8_memb_t *pDst;
  int32_mema_t *pBias;
  int32_mema_t *pQuant;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, HSP_DIRECT_CMD_CNN_CONVDW_I8);

  /* Then update all parameters */
  uint32_t pad = padx_l + padx_r + pady_t + pady_b;
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);

  uint32_t nbLineInPerBlocks = (nb_line_per_blocks * stridey) + k_h - stridey; /* For circular mode */

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8((stridex << 16U) | (stridey & 0x000000ff));
  HSP_HW_IF_WRITE_PARAMR9(off_in);
  HSP_HW_IF_WRITE_PARAMR10(off_ou);
  HSP_HW_IF_WRITE_PARAMR11(sat_min);
  HSP_HW_IF_WRITE_PARAMR12(sat_max);

  uint32_t k_c = in_c;
  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = nbLineInPerBlocks; /* For circular mode */
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */

  uint32_t nbPadK0 = (4U - (in_c & 0x3U)) & 0x3U;

  if ((pSrcB = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (4U * k_h * k_w) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Nb input lines in buffer */
  if ((pSrcA = (int8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pDst  = (int8_memb_t *)alloc_in_memB(&(hmw->hbram), (outLineSize8 * nb_line_per_blocks))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pBias = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)))) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  if ((pQuant = (int32_mema_t *)alloc_in_memA(&(hmw->hbram), (ou_c * sizeof(uint32_t)) * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR13((uint32_t)pBias); /* bias followed by quant */
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pQuant);
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nb_line_per_blocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_3STEP);

  /* WARNING copy bias and quant before DCMDPTR for FW needs */
  for (uint32_t i = 0; i < ou_c; i++)
  {
    *pBias++ = p_bias_data[i];
  }

  align_factor_cmsisnn_fast_ch(in_scale, out_scale, p_wt_scale, (uint16_t)ou_c, (int32_t *)pQuant);

  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t) pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t) pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t) pDst);

  /* Depthwise works  4 by 4 then we need to load (4*k_h*k_w) kernel for first line */
  int8_t *ptmp = (int8_t *)p_filter_data;
  int8_memb_t *ptmpSrcB = (int8_memb_t *)pSrcB;

  for (uint32_t j = 0; j < (k_h * k_w); j++)
  {
    ptmpSrcB[0] = ptmp[0];
    ptmpSrcB[1] = ptmp[1];
    ptmpSrcB[2] = ptmp[2];
    ptmpSrcB[3] = ptmp[3];
    ptmpSrcB += 4; /* nb kernel: 4 by 4 */
    ptmp += k_c;
  }


  /* Load first input tile */
  int8_t padV = (int8_t) off_in;
  padV = -padV;
  uint8_t val = padV;
  uint8_mema_t *inPad = (uint8_mema_t *)pSrcA;
  uint32_t nbPadTop = 0U;

  /* First lines pad */
  HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
  inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
  uint32_t lineToPad = k_h - pady_t;
  if (lineToPad > in_h)
  {
    nbPadTop = lineToPad - in_h;
    lineToPad =  in_h;
  }
  /* Middle lines to pad */
  for (uint32_t i = 0; i < lineToPad; i++)
  {
    /* First column pad */
    if (pad)
    {
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
    }
    /* Copy image line */
    HSP_MEMCPY((int8_t *)inPad, (int8_t *)p_input_data, (in_w * in_c));
    inPad += (in_w * in_c);
    p_input_data += (in_w * in_c);
    /* Last column pad */
    if (pad)
    {
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
  }
  /* Add bottom pad if necessary */
  if (nbPadTop)
  {
    /* last lines pad */
    HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
  }
  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */


  uint8_t *pSrcEnd = (uint8_t *)pSrcA + (circiSize);
  uint8_t *pSrcCur = (uint8_t *)pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nb_line_per_blocks * ou_c * ou_w));
  uint8_t *pDstCur = (uint8_t *) pDst;
  uint8_t *pSrcbEnd = (uint8_t *)(pSrcB + (4U * k_h * k_w) * 2U); /* Ping Pong coeff buffer */
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);
  uint8_t *pKerCur = (uint8_t *)(p_filter_data + 4U);
  uint8_t *pSrcbCur = (uint8_t *)(pSrcB + (k_h * k_w * 4U));
  uint32_t nbLinesBlck = 0U; /* Init with one block for more simple comparison at end of while */
  uint32_t nbLineLoop = nb_line_per_blocks;

  if (pSrcCur == pSrcEnd)
  {
    pSrcCur = (uint8_t *)pSrcA;
  }

  do
  {
    if (nbLinesBlck)
    {
      pKerCur = (uint8_t *)p_filter_data;
      int8_t *ptmpKe = (int8_t *)p_filter_data;
      int8_memb_t  *ptmpKb = (int8_memb_t *)pSrcbCur;
      for (uint32_t j = 0; j < (k_h * k_w); j++)
      {
        ptmpKb[0] = ptmpKe[0];
        ptmpKb[1] = ptmpKe[1];
        ptmpKb[2] = ptmpKe[2];
        ptmpKb[3] = ptmpKe[3];
        ptmpKb += 4;
        pSrcbCur += 4;
        ptmpKe += k_c;
      }
      pKerCur += 4; /* next 4 channels */
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = (uint8_t *)pSrcB;
      }
      /* Copy input line in circular buffer before enter loop */
      for (uint32_t sidx = 0; sidx <  stridey; sidx++)
      {
        if (countlines == countmax)
        {
          /* Last lines pad */
          HSP_MEMSET(pSrcCur, val, inLineSize8);
          pSrcCur += inLineSize8;
        }
        else
        {
          countlines++;
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
          pSrcCur += xloop;
          p_input_data += xloop;
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
        if (pSrcCur == pSrcEnd)
        {
          pSrcCur = (uint8_t *)pSrcA;
        }
      }
    }
    nbLinesBlck += nbLineLoop;
    if (nbLinesBlck > ou_h)
    {
      /* Count how may output lines not done by block */
      nbLinesBlck -= nbLineLoop; /* Remove next block size */
      nbLineLoop = ou_h - nbLinesBlck;
      nbLinesBlck += nbLineLoop;
    }

    /*Send event through CDEG: WARNING: do we need to disable all other events??? */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    /* First Step: copy all remaining input lines in memory */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Copy input line in circular buffer */
      for (uint32_t sidx = 0; sidx <  stridey; sidx++)
      {
        if (countlines == countmax)
        {
          /* Last lines pad */
          HSP_MEMSET(pSrcCur, val, inLineSize8);
          pSrcCur += inLineSize8;
        }
        else
        {
          countlines++;
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
          pSrcCur += xloop;
          p_input_data += xloop;
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
        if (pSrcCur == pSrcEnd)
        {
          pSrcCur = (uint8_t *)pSrcA;
        }
      }
      /* Send ready to FW */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
    /* All Lines are copied: Loop on each ping pong kernel */
    for (uint32_t out_num = 4; out_num < ou_c; out_num += 4)
    {
      /* First of all copy kernel block */
      if (nbPadK0 && ((out_num + 4) >= ou_c))
      {
        int8_t *ptmpKe = (int8_t *)pKerCur;
        int8_memb_t  *ptmpKb = (int8_memb_t *)pSrcbCur;
        for (uint32_t j = 0; j < (k_h * k_w); j++)
        {
          ptmpKb[0] = ptmpKe[0];
          if (nbPadK0 == 1)
          {
            ptmpKb[1] = ptmpKe[1];
            ptmpKb[2] = ptmpKe[2];
            ptmpKb[3] = 0;
          }
          if (nbPadK0 == 2)
          {
            ptmpKb[1] = ptmpKe[1];
            ptmpKb[2] = 0;
            ptmpKb[3] = 0;
          }
          if (nbPadK0 == 3)
          {
            ptmpKb[1] = 0;
            ptmpKb[2] = 0;
            ptmpKb[3] = 0;
          }
          ptmpKb += 4;
          pSrcbCur += 4;
          ptmpKe += k_c;
        }
        pKerCur += (4 - nbPadK0); /* next 4 channels */
      }
      else
      {
        int8_t *ptmpKe = (int8_t *)pKerCur;
        int8_memb_t  *ptmpKb = (int8_memb_t *)pSrcbCur;
        for (uint32_t j = 0; j < (k_h * k_w); j++)
        {
          ptmpKb[0] = ptmpKe[0];
          ptmpKb[1] = ptmpKe[1];
          ptmpKb[2] = ptmpKe[2];
          ptmpKb[3] = ptmpKe[3];
          ptmpKb += 4;
          pSrcbCur += 4;
          ptmpKe += k_c;
        }
        pKerCur += 4; /* next 4 channels */
      }
      if (pSrcbCur == pSrcbEnd)
      {
        pSrcbCur = (uint8_t *)pSrcB;
      }
      /* Wait output channel done */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    }
    /* Now copy output in destination */
    /* Then read output lines in BRAM */
    for (uint32_t lineIdx = 1; lineIdx < nbLineLoop; lineIdx++)
    {
      /* Wait output line */
      HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
      __DSB();
#endif /* STM32H7P5xx */

      /* Send event through CDEG */
      HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
      HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
      p_output_data += outLineSize8;
      pDstCur += outLineSize8;
      if (pDstCur == pDstEnd)
      {
        pDstCur = (uint8_t *) pDst;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = (uint8_t *)pDst;
    }
  } while (nbLinesBlck < ou_h); /* End of input block loop */


  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute fully connected CNN function with all necessary data loaded into memory.
  *
  * @param hmw                 HSP middleware context.
  * @param in_c                Input dimension channel (in_c > 4)
  * @param ou_c                Output dimension channel (ou_c > 1)
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param wt_scale            Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @retval                    Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c         : Input lines size (word aligned) in bytes
        ou_c         : Output lines size (word aligned) in bytes
        ou_c * 4     : Bias size (word aligned) in bytes
      In BRAM_B:
        in_c * ou_c  : Filter coefficients size (word aligned) in bytes
    Constraints:
      Size limitations: ou_c must be > 1 and in_c > 4
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnFullyConnected0_s8(hsp_engine_context_t *hmw, uint32_t in_c, uint32_t ou_c,
                                                int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                                uint32_t *p_bias_data,
                                                float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                                uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_mema_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, (uint32_t) HSP_DIRECT_CMD_CNN_FC_I8);

  uint32_t k_c = in_c * ou_c;
  /* Put kernels in MEMB */
  if ((pSrcB = (uint8_memb_t *) alloc_in_memB(&(hmw->hbram), k_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *) alloc_in_memA(&(hmw->hbram), in_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMA */
  if ((pDst  = (uint8_mema_t *) alloc_in_memA(&(hmw->hbram), ou_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ  = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  int32_t qMul;
  int32_t qShift;
  align_factor_cmsisnn_fast_ch_v3(in_scale, out_scale, wt_scale, (int32_t *)&qMul, (int32_t *)&qShift);

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(qMul);
  HSP_HW_IF_WRITE_PARAMR9(qShift);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pBQ);
  HSP_HW_IF_WRITE_PARAMR15(HSP_CNN_CFG_MODE_0STEP);
  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load all coeffs */
  uint32_t nbPadK0 = 0U;
  if (in_c & 0x3U)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    uint8_memb_t *tmpB = pSrcB;
    int8_t *tmpKe = p_filter_data;
    nbPadK0 = (4U - in_c) & 0x3U;
    for (uint32_t i = 0; i < ou_c; i++)
    {
      HSP_MEMCPY((int8_t *)tmpB, (int8_t *)tmpKe, (in_c));
      tmpKe += (in_c);
      tmpB += (in_c);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *tmpB++ = 0;
      }
    }
  }
  else
  {
    HSP_MEMCPY((int8_t *)pSrcB, (int8_t *)p_filter_data, (k_c));
  }

  /* Now copy bias */
  HSP_MEMCPY((int8_t *)pBQ, (int8_t *)p_bias_data, (ou_c * 4U));
  /* Load first input tile */
  HSP_MEMCPY((int8_t *)pSrcA, (int8_t *)p_input_data, (in_c));

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  /* Wait direct command done */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */
  /* Copy result in external memory */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDst, (ou_c));

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute fully connected CNN function in 1 step.
  *        Input and output data are fully loaded into memory, while coefficients are loaded into a circular buffer.
  *        Step: Process input lines using the corresponding coefficients while loading the next set of coefficients
  *              into the circular buffer.
  *
  * @param hmw                 HSP middleware context.
  * @param in_c                Input dimension channel (in_c > 4)
  * @param ou_c                Output dimension channel
  * @param p_input_data        Input data pointer, int8_t data type
  * @param p_filter_data       Kernel coefficient pointer, int8_t data type
  * @param p_output_data       Output data pointer, int8_t data type
  * @param p_bias_data         Bias data pointer, int32_t data type
  * @param in_scale            Input scale
  * @param out_scale           Output scale
  * @param wt_scale            Pointer in weight scales (one per output channel)
  * @param off_in              Input offset, int32_t data type
  * @param off_ou              Output offset, int32_t data type
  * @param sat_min             Min sat (Relu), int32_t data type
  * @param sat_max             Max sat (Relu), int32_t data type
  * @retval                    Core status.
  * @details
    @verbatim
    Buffer footprint:
    All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c         : Input lines size (word aligned) in bytes
        ou_c         : Output lines size (word aligned) in bytes
        ou_c * 4     : Bias size (word aligned) in bytes
      In BRAM_B:
        in_c * 2     : Filter coefficients size (word aligned) in bytes
    Constraints:
      Size limitations: in_c > 4
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnFullyConnected1_s8(hsp_engine_context_t *hmw, uint32_t in_c, uint32_t ou_c,
                                                int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                                uint32_t *p_bias_data,
                                                float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                                uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max)
{
  uint8_memb_t *pSrcB;
  uint8_mema_t *pSrcA;
  uint8_mema_t *pDst;
  uint32_mema_t *pBQ;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, (uint32_t) HSP_DIRECT_CMD_CNN_FC_I8);

  /* Force BARB here for the moment */
  HSP_HW_IF_BRAM_SetBandwidthArbitration(hmw->hdriver, HAL_HSP_BRAM_ARBITRATION_LATENCY_4_CYCLES);

  /* Put kernels in MEMB */
  if ((pSrcB = (uint8_memb_t *)alloc_in_memB(&(hmw->hbram), in_c * 2U)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), in_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMA */
  if ((pDst  = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put bias in MEMA */
  if ((pBQ  = (uint32_mema_t *)alloc_in_memA(&(hmw->hbram), ou_c * sizeof(uint32_t))) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  int32_t qMul;
  int32_t qShift;
  align_factor_cmsisnn_fast_ch_v3(in_scale, out_scale, wt_scale, (int32_t *)&qMul, (int32_t *)&qShift);

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(qMul);
  HSP_HW_IF_WRITE_PARAMR9(qShift);
  HSP_HW_IF_WRITE_PARAMR10(off_in);
  HSP_HW_IF_WRITE_PARAMR11(off_ou);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14((uint32_t)pBQ);
  HSP_HW_IF_WRITE_PARAMR15(HSP_CNN_CFG_MODE_FCPP);
  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR1((uint32_t)pSrcB);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load all coeffs */
  uint32_t nbPadK0 = 0U;
  if ((in_c) & 0x3U)
  {
    /* Not aligned on 4, add extra kernels to 0 */
    uint8_memb_t *tmpB = pSrcB;
    int8_t *tmpKe = p_filter_data;
    nbPadK0 = (4U - in_c) & 0x3U;
    HSP_MEMCPY((int8_t *)tmpB, (int8_t *)tmpKe, (in_c));
    tmpKe += (in_c);
    tmpB += (in_c);
    for (uint32_t kp = 0; kp < nbPadK0; kp++)
    {
      *tmpB++ = 0;
    }
  }
  else
  {
    HSP_MEMCPY((int8_t *)pSrcB, (int8_t *)p_filter_data, (in_c));
  }
  /* Now copy bias */
  HSP_MEMCPY((int8_t *)pBQ, (int8_t *)p_bias_data, (ou_c * 4U));
  /* Load first input tile */
  HSP_MEMCPY((int8_t *)pSrcA, (int8_t *)p_input_data, (in_c));

  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

  uint8_t *pKerCur = (uint8_t *)(p_filter_data + (in_c));
  uint8_t *pSrcbCur = pSrcB + (in_c + nbPadK0);
  uint8_t *pSrcbEnd = pSrcB + ((in_c + nbPadK0) * 2U);
#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);

  /* Loop to load all kernels in BRAM */
  for (uint32_t out_num = 1; out_num < ou_c; out_num++)
  {
    /* Copy kernel block */
    if (nbPadK0)
    {
      /* Not aligned on 4, add extra kernels to 0 */
      HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKerCur, (in_c));
      pKerCur += (in_c);
      pSrcbCur += (in_c);
      for (uint32_t kp = 0; kp < nbPadK0; kp++)
      {
        *pSrcbCur++ = 0;
      }
    }
    else
    {
      HSP_MEMCPY((int8_t *)pSrcbCur, (int8_t *)pKerCur, in_c);
      pKerCur += in_c;
      pSrcbCur += in_c;
    }
    if (pSrcbCur == pSrcbEnd)
    {
      pSrcbCur = pSrcB;
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  }
  HSP_HW_IF_BRAM_SetBandwidthArbitration(hmw->hdriver, HAL_HSP_BRAM_ARBITRATION_LATENCY_16_CYCLES);
  /* Wait last output line */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /* Copy result in external memory */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDst, (ou_c));

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute max pool or avg pool CNN function.
  *        Data input are fully loaded into memory before processing begins.
  *
  * @param hmw                  HSP middleware context.
  * @param in_w                 Input dimension width
  * @param in_h                 Input dimension height
  * @param in_c                 Input dimension channel
  * @param k_w                  Kernel dimension width
  * @param k_h                  Kernel dimension height
  * @param ou_w                 Output dimension width
  * @param ou_h                 Output dimension height
  * @param ou_c                 Output dimension channel
  * @param stridex              Stride on X
  * @param stridey              Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param sat_min              Min sat (Relu), int32_t data type
  * @param sat_max              Max sat (Relu), int32_t data type
  * @param pool_type            0: max pool 1: avg pool
  * @retval                     Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_w * in_h * in_c        : Input lines size (word aligned) in bytes
      In BRAM_B:
        ou_w * ou_h * ou_c        : Output lines size (word aligned) in bytes
    Constraints:
      Kernel coefficient 1x1 not supported
    @endverbatim
 */
hsp_core_status_t HSP_ACC_CnnPool0_s8(hsp_engine_context_t *hmw,
                                      uint32_t in_w, uint32_t in_h, uint32_t in_c, uint32_t k_w, uint32_t k_h,
                                      uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                      int8_t *p_input_data, int8_t *p_output_data,
                                      uint32_t sat_min, uint32_t sat_max, uint32_t pool_type)
{
  uint8_mema_t *pSrcA;
  uint8_memb_t *pDst;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, (uint32_t) HSP_DIRECT_CMD_CNN_POOL_I8);

  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t nbLinePerBlocks = ou_h;
  circiLines = in_h;
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), in_w * in_h * in_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMB */
  if ((pDst  = (uint8_memb_t *)alloc_in_memB(&(hmw->hbram), ou_w * ou_h * ou_c)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  /* Then update all parameters */
  HSP_HW_IF_WRITE_PARAMR0(in_w);
  HSP_HW_IF_WRITE_PARAMR1(in_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14(pool_type); /* type; */
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nbLinePerBlocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT));
  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load first input tile */
  HSP_MEMCPY((int8_t *)pSrcA, (int8_t *)p_input_data, (in_c * in_h * in_w));
  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }
  /* Clear sem before send event */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif /* STM32H7P5xx */

  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  /* Wait direct command done */
  HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  /* Copy result in external memory */
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDst, (ou_c * ou_h * ou_w));

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @brief Execute max pool or avg pool CNN function in 1 step.
  *        Input and output data are loaded into a circular buffer.
  *        Step: Process input lines while loading the next line into the circular buffer.
  *
  * @param hmw                  HSP middleware context.
  * @param in_w                 Input dimension width
  * @param in_h                 Input dimension height
  * @param in_c                 Input dimension channel
  * @param k_w                  Kernel dimension width
  * @param k_h                  Kernel dimension height
  * @param ou_w                 Output dimension width
  * @param ou_h                 Output dimension height
  * @param ou_c                 Output dimension channel
  * @param stridex              Stride on X
  * @param stridey              Stride on Y
  * @param p_input_data         Input data pointer, int8_t data type
  * @param p_output_data        Output data pointer, int8_t data type
  * @param sat_min              Min sat (Relu), int32_t data type
  * @param sat_max              Max sat (Relu), int32_t data type
  * @param padx_l               Pad on X left (only supported for Maxpool)
  * @param padx_r               Pad on X right (only supported for Maxpool)
  * @param pady_t               Pad on Y top (only supported for Maxpool)
  * @param pady_b               Pad on Y bottom (only supported for Maxpool)
  * @param pool_type            0: max pool 1: avg pool
  * @param nb_line_per_blocks   Number of output lines per block
  * @retval                     Core status.
  * @details
    @verbatim
    Buffer footprint:
      All memory allocation in BRAM_A or BRAM_B are aligned on 32 bits
      In BRAM_A:
        in_c * (in_w + (padx_l + padx_r)) * (k_h + stridey)  : Input lines size (word aligned) in bytes
      In BRAM_B:
        nb_line_per_blocks * ou_c * ou_w                     : Output lines size (word aligned) in bytes
    Constraints:
      Kernel coefficient 1x1 not supported
    @endverbatim
  */
hsp_core_status_t HSP_ACC_CnnPool1_s8(hsp_engine_context_t *hmw,
                                      uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                      uint32_t k_w, uint32_t k_h,
                                      uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                      uint32_t stridex, uint32_t stridey,
                                      int8_t *p_input_data, int8_t *p_output_data, uint32_t sat_min, uint32_t sat_max,
                                      uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                      uint32_t pool_type, uint32_t nb_line_per_blocks)
{
  uint8_mema_t *pSrcA;
  uint8_memb_t *pDst;

  /* First of all, wakeup HSP with Direct command */
  HSP_HW_IF_CNN_Begin(hmw->hdriver, (uint32_t) HSP_DIRECT_CMD_CNN_POOL_I8);

  /* Then update all parameters */
  uint32_t pad = padx_l + padx_r + pady_t + pady_b; /* Pad option */
  uint32_t inp_w = in_w + (padx_l + padx_r);
  uint32_t inp_h = in_h + (pady_t + pady_b);

  uint32_t inLineSize8 = in_c * inp_w; /* 1 input line */
  uint32_t outLineSize8 = ou_c * ou_w; /* 1 output line */
  uint32_t circiLines = (k_h + stridey); /* For circular mode */
  uint32_t nbLinePerBlocks = nb_line_per_blocks;
  if (circiLines > inp_h)
  {
    circiLines = inp_h;
  }
  uint32_t circiSize = inLineSize8 * circiLines; /* For circular mode */
  /* Put input image in MEMA */
  if ((pSrcA = (uint8_mema_t *)alloc_in_memA(&(hmw->hbram), circiSize)) == NULL)
  {
    return HSP_CORE_ERROR;
  }
  /* Put output images in MEMB */
  if ((pDst  = (uint8_memb_t *)alloc_in_memB(&(hmw->hbram), nb_line_per_blocks * outLineSize8)) == NULL)
  {
    return HSP_CORE_ERROR;
  }

  HSP_HW_IF_WRITE_PARAMR0(inp_w);
  HSP_HW_IF_WRITE_PARAMR1(inp_h);
  HSP_HW_IF_WRITE_PARAMR2(in_c);
  HSP_HW_IF_WRITE_PARAMR3(k_w);
  HSP_HW_IF_WRITE_PARAMR4(k_h);
  HSP_HW_IF_WRITE_PARAMR5(ou_w);
  HSP_HW_IF_WRITE_PARAMR6(ou_h);
  HSP_HW_IF_WRITE_PARAMR7(ou_c);
  HSP_HW_IF_WRITE_PARAMR8(stridex);
  HSP_HW_IF_WRITE_PARAMR9(stridey);
  HSP_HW_IF_WRITE_PARAMR12(sat_min);
  HSP_HW_IF_WRITE_PARAMR13(sat_max);
  HSP_HW_IF_WRITE_PARAMR14(pool_type); /* pool type; */
  HSP_HW_IF_WRITE_PARAMR15((circiLines << HSP_CNN_CFG_NB_IN_LINE_SHIFT) |
                           (nbLinePerBlocks << HSP_CNN_CFG_NB_OU_LINE_SHIFT) | HSP_CNN_CFG_MODE_PCIRC);
  /* And finally sync with DCMD using DCMDPTR registers */
  HSP_HW_IF_WRITE_DCMDPTR0((uint32_t)pSrcA);
  HSP_HW_IF_WRITE_DCMDPTR2((uint32_t)pDst);

  /* Load first input tile */
  uint8_t val = 0x80;
  if (pad)
  {
    uint8_mema_t *inPad = pSrcA;
    uint32_t nbPadTop = 0U;
    /* First lines pad */
    HSP_MEMSET(inPad, val, (pady_t *((in_w + (padx_l + padx_r)) * in_c)));
    inPad += (pady_t *((in_w + (padx_l + padx_r)) * in_c));
    /* Middle lines to pad */
    uint32_t lineToPad = k_h - pady_t;
    if (lineToPad > in_h)
    {
      nbPadTop = lineToPad - in_h;
      lineToPad =  in_h;
    }
    for (uint32_t i = 0; i < lineToPad; i++)
    {
      /* First column pad */
      HSP_MEMSET(inPad, val, (padx_l * in_c));
      inPad += (padx_l * in_c);
      /* Copy image line */
      HSP_MEMCPY((int8_t *)inPad, (int8_t *)p_input_data, (in_w * in_c));
      inPad += (in_w * in_c);
      p_input_data += (in_w * in_c);
      /* Last column pad */
      HSP_MEMSET(inPad, val, (padx_r * in_c));
      inPad += (padx_r * in_c);
    }
    /* Add bottom pad if necessary */
    if (nbPadTop)
    {
      /* last lines pad */
      HSP_MEMSET(inPad, val, (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c)));
      inPad += (nbPadTop * ((in_w + (padx_l + padx_r)) * in_c));
    }
  }
  else
  {
    HSP_MEMCPY((int8_t *)pSrcA, (int8_t *)p_input_data, (inLineSize8 * k_h));
    p_input_data += (inLineSize8 * k_h);
  }
  /* Wait H2CSEMR for HSP done, check FWERR */
  HSP_HW_IF_WaitEndOfDirectCommand(hmw);
  if (HSP_HW_IF_GetFirmwareError(hmw->hdriver))
  {
    /* Free all memory */
    free_all_ai(&(hmw->hbram));
    return HSP_CORE_ERROR;
  }

  uint8_t *pSrcEnd = pSrcA + (circiSize);
  uint8_t *pSrcCur = pSrcA + (inLineSize8 * k_h);
  uint8_t *pDstEnd = (uint8_t *)(pDst + (nbLinePerBlocks * outLineSize8));
  uint8_t *pDstCur = (uint8_t *) pDst;
  uint32_t countlines = k_h;
  uint32_t countmax = inp_h - pady_b;
  uint32_t xloop = inLineSize8 - ((padx_l + padx_r) * in_c);
  /* Send event through CDEG */
  HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
  for (uint32_t lineIdx = 0; lineIdx < (ou_h - 1); lineIdx++)
  {
    /* Copy input line in circular buffer */
    for (uint32_t sidx = 0; sidx < stridey; sidx++)
    {
      if (countlines == countmax)
      {
        /* Last lines pad */
        HSP_MEMSET(pSrcCur, val, inLineSize8);
        pSrcCur += inLineSize8;
      }
      else
      {
        countlines++;
        if (pad)
        {
          /* First column pad */
          HSP_MEMSET(pSrcCur, val, (padx_l * in_c));
          pSrcCur += (padx_l * in_c);
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, xloop);
          pSrcCur += xloop;
          p_input_data += xloop;
          /* Last column pad */
          HSP_MEMSET(pSrcCur, val, (padx_r * in_c));
          pSrcCur += (padx_r * in_c);
        }
        else
        {
          HSP_MEMCPY((int8_t *)pSrcCur, (int8_t *)p_input_data, inLineSize8);
          p_input_data += inLineSize8;
          pSrcCur += inLineSize8;
        }
      }
      if (pSrcCur == pSrcEnd)
      {
        pSrcCur = pSrcA;
      }
    }
    /* Wait output line */
    HSP_HW_IF_CNN_WaitEndOfProcess(hmw->hdriver);
#ifdef STM32H7P5xx
    __DSB();
#endif /* STM32H7P5xx */

    /* Send event through CDEG */
    HSP_HW_IF_CNN_StartOfProcess(hmw->hdriver, HSP_CNN_CDEG_EVT);
    HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
    p_output_data += outLineSize8;
    pDstCur += outLineSize8;
    if (pDstCur == pDstEnd)
    {
      pDstCur = pDst;
    }
  }
  /* Wait last output line */
  HSP_HW_IF_WaitSemaphore(hmw->hdriver);
  HSP_MEMCPY((int8_t *)p_output_data, (int8_t *)pDstCur, outLineSize8);
  /* Clear sem */
  HSP_HW_IF_ReleaseSemaphore(hmw->hdriver);

#ifdef STM32H7P5xx
  __DSB();
#endif  /* STM32H7P5xx */

  HSP_HW_IF_CNN_End(hmw->hdriver);

  /* Free all memory */
  free_all_ai(&(hmw->hbram));
  return HSP_CORE_OK;
}

/**
  * @}
  */

/**
  * @addtogroup HSP_CNN_Private_Functions
  * @{
  */
/**
  * @brief Compute integer factor and shift to get results in Integer arithmetic
  *        with a per channel dimension using cmsis format (Shift)
  *        factor and shift are interleaved in output
  * @param   in_scale      input scale
  * @param   out_scale     output scale
  * @param   p_wt_scale    pointer in weight scales (one per output channel)
  * @param   ch_im_out     number of output channels
  * @param   p_out         pointer on output: scale factor and shift are interleaved
  */
static void align_factor_cmsisnn_fast_ch(float32_t in_scale, float32_t out_scale,
                                         const float32_t *p_wt_scale, uint16_t ch_im_out, int32_t *p_out)
{
  hsp_u32_f32_t out_align_factor;
  int32_t OutRShift;
  int32_t OutFactor;

  for (uint32_t i = 0, j = 0; i < ch_im_out; i++, j += 2U)
  {
    out_align_factor.f32 = (p_wt_scale[i] * in_scale / out_scale);
    OutRShift = 126 - ((out_align_factor.u32 & 0x7f800000) >> 23U);
    OutFactor = ((out_align_factor.u32 & 0x7FFFFF) + 0x800000);
    if (out_align_factor.u32 & 0x80000000)
    {
      OutFactor = -OutFactor;
    }
    if (OutRShift > 31)
    {
      OutFactor = 0;
      OutRShift = 0;
    }
    OutFactor <<= 7U;
    p_out[j] = OutFactor;
    p_out[j + 1U] = -OutRShift;
  }
}

/**
  * @brief compute integer factor and shift to get results in Integer arithmetic
  *        with a per channel dimension using cmsis format (Shift).
  *        factor, shift and bias are interleaved in output
  * @param   in_scale      input scale
  * @param   out_scale     output scale
  * @param   p_wt_scale    pointer in weight scales (one per output channel)
  * @param   p_bias_data   pointer in bias data (one per output channel)
  * @param   ch_im_out     number of output channels
  * @param   p_out         pointer on output: scale factor and shift are interleaved
  */
static void align_factor_cmsisnn_fast_ch_v2(float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                            int32_t *p_bias_data, uint16_t ch_im_out, int32_t *p_out)
{
  hsp_u32_f32_t out_align_factor;
  int32_t OutRShift;
  int32_t OutFactor;

  for (uint32_t i = 0, j = 0; i < ch_im_out; i++, j += 3U)
  {
    out_align_factor.f32 = (p_wt_scale[i] * in_scale / out_scale);
    OutRShift = 126 - ((out_align_factor.u32 & 0x7f800000) >> 23U);
    OutFactor = ((out_align_factor.u32 & 0x7FFFFF) + 0x800000);
    if (out_align_factor.u32 & 0x80000000)
    {
      OutFactor = -OutFactor;
    }
    if (OutRShift > 31)
    {
      OutFactor = 0;
      OutRShift = 0;
    }
    OutFactor <<= 7U;
    p_out[j] = p_bias_data[i];
    p_out[j + 1U] = OutFactor;
    p_out[j + 2U] = -OutRShift;
  }
}

/**
  * @brief Compute only one integer factor and shift to get results in Integer arithmetic
  *         using cmsis format (Shift).
  * @param   in_scale      input scale
  * @param   out_scale     output scale
  * @param   wt_scale      weight scales
  * @param   p_out_factor  pointer on output scale factors
  * @param   p_out_shift   pointer on output shift values
  */
static void align_factor_cmsisnn_fast_ch_v3(float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                            int32_t *p_out_factor, int32_t *p_out_shift)
{
  hsp_u32_f32_t out_align_factor;
  int32_t OutRShift;
  int32_t OutFactor;

  out_align_factor.f32 = (wt_scale * in_scale / out_scale);
  OutRShift = 126 - ((out_align_factor.u32 & 0x7f800000) >> 23U);
  OutFactor = ((out_align_factor.u32 & 0x7FFFFF) + 0x800000);
  if (out_align_factor.u32 & 0x80000000)
  {
    OutFactor = -OutFactor;
  }
  if (OutRShift > 31)
  {
    OutFactor = 0;
    OutRShift = 0;
  }
  OutFactor <<= 7U;
  *p_out_factor = OutFactor;
  *p_out_shift = -OutRShift;
}

/**
  * @brief Memory allocation in MEMA in AI area
  * @param   hhsp_bram     HSP BRAM handle
  * @param   size_in_byte  size in byte of allocation
  * @return  address in MEMA if allocation succeed, 0 if allocation failed
  */
static int8_mema_t *alloc_in_memA(hsp_bram_handle_t *hhsp_bram, uint32_t size_in_byte)
{
  /* First check if AI area is defined */
#if !defined(HSP_BRAM_AI_SIZE) || (HSP_BRAM_AI_SIZE == 0)
  return (NULL);
#else /* HSP_BRAM_AI_SIZE == 0 */
  uint32_t addr;
  uint32_t sizeInWord = (size_in_byte + (sizeof(uint32_t) - 1)) / sizeof(uint32_t); /* Align on Word */

  if (sizeInWord > hhsp_bram->maxSizeToAllocateA)
  {
    return (NULL);
  }

  /* Update the current MEMA shared offset */
  hhsp_bram->currentSharedOffsetA += sizeInWord; /* currentSharedOffset is calculate in word */
  /* Next free MEMA address */
  addr = (uint32_t)(hhsp_bram->currentSharedAddrA);
  hhsp_bram->currentSharedAddrA += (sizeInWord * sizeof(uint32_t));
  /* Calculate shared remaining size */
  hhsp_bram->maxSizeToAllocateA -= sizeInWord;

  return ((int8_mema_t *)addr);
#endif /* HSP_BRAM_AI_SIZE == 0 */
}

/**
  * @brief Memory allocation in MEMB in AI area
  * @param   hhsp_bram     HSP BRAM handle
  * @param   size_in_byte  size in byte of allocation
  * @return  address in MEMB if allocation succeed, 0 if allocation failed
  */
static int8_memb_t *alloc_in_memB(hsp_bram_handle_t *hhsp_bram, uint32_t size_in_byte)
{
  /* First check if AI area is defined */
#if !defined(HSP_BRAM_AI_SIZE) || (HSP_BRAM_AI_SIZE == 0)
  return (NULL);
#else /* HSP_BRAM_AI_SIZE == 0 */
  uint32_t addr;
  uint32_t sizeInWord = (size_in_byte + (sizeof(uint32_t) - 1)) / sizeof(uint32_t); /* Align on Word */

  if (sizeInWord > hhsp_bram->maxSizeToAllocateB)
  {
    return (NULL);
  }
  /* Update the current MEMB shared offset */
  hhsp_bram->currentSharedOffsetB += sizeInWord; /* currentSharedOffset is calculate in word */
  /* Next free MEMB address */
  addr = (uint32_t)(hhsp_bram->currentSharedAddrB);
  hhsp_bram->currentSharedAddrB += (sizeInWord * sizeof(uint32_t));
  /* Calculate shared remaining size */
  hhsp_bram->maxSizeToAllocateB -= sizeInWord;

  return ((int8_memb_t *)addr);
#endif /* HSP_BRAM_AI_SIZE == 0 */
}

/**
  * @brief Free all memory allocated during CNN processing.
  * @param  hhsp_bram  HSP BRAM handle
  */
static void free_all_ai(hsp_bram_handle_t *hhsp_bram)
{
  hhsp_bram->currentSharedOffsetA = 0U;
  hhsp_bram->currentSharedOffsetB = 0U;
  hhsp_bram->currentSharedAddrA = hhsp_bram->baseSharedAddrA;
  hhsp_bram->currentSharedAddrB = hhsp_bram->baseSharedAddrB;
  hhsp_bram->maxSizeToAllocateA = HSP_BRAM_AI_SIZE;
  hhsp_bram->maxSizeToAllocateB = HSP_BRAM_AI_SIZE;
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
