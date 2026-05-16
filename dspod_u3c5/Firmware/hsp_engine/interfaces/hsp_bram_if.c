/**
  ******************************************************************************
  * @file    hsp_bram_if.c
  * @brief   This file implements the service provide by the HSP BRAM Interface
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
#include "hsp_bram_if.h"
#include "hsp_fw_def.h"
#include "hsp_hw_if.h"

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
extern unsigned int Image$$RW_HSP_DATA_BRAM$$Base;
extern unsigned int Image$$RW_HSP_DATA_BRAM$$Limit;
#elif defined (__GNUC__)
extern char __section_static_hsp_data_bram_start__;
extern char __section_static_hsp_data_bram_end__;
#elif defined (__ICCARM__)
#pragma section="HSP_DATA_BRAM"
#endif /* __ICCARM__ */

/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_INTERFACES
  * @{
  */

/** @addtogroup HSP_BRAM_IF
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup HSP_BRAM_IF_Private_Constants HSP_BRAM_IF Private Constants
  * @{
  */
#define IF_SIZEOF_HEADER_STATE_BUFF  20U  /*!< Sizeof struct hsp_hw_if_filter_state_t */
#define IF_SIZEOF_HEADER_FIR_DECIM_STATE_BUFF  28U  /*!< Sizeof struct hsp_hw_if_filter_state_t */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup HSP_BRAM_IF_Private_Functions HSP_BRAM_IF Private Functions
  * @{
  */
static hsp_hw_if_filter_state_t *HSP_BRAM_IF_MallocStateBuffer(hsp_bram_handle_t *hhsp_bram, uint32_t stateSize,
                                                               uint32_t info, hsp_bram_allocation_t area);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HSP_BRAM_IF_Exported_Functions
  * @{
  */
/**
  * @brief Returns the free space in word in MEMAB
  * @param  hhsp_bram  HSP handle
  * @param area        HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                    HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval  Number of free word, 0 if error
  */
uint32_t HSP_BRAM_IF_GetFreeSize(hsp_bram_handle_t *hhsp_bram, hsp_bram_allocation_t area)
{
  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  if (area == HSP_BRAM_ALLOCATION_DEFAULT)
  {
    return (hhsp_bram->maxSizeToAllocate / sizeof(uint32_t));
  }
  else
  {
    if (area == HSP_BRAM_ALLOCATION_PERSISTENT)
    {
      return (hhsp_bram->maxSizePersistent / sizeof(uint32_t));
    }
    else
    {
      return 0U;
    }
  }
}

/**
  * @brief  Initialize BRAM interface
  * @param  hhsp_bram  HSP BRAM handle
  * @param  hdriver    Pointer the HSP Driver
  * @retval            HSP_IF_OK  Init success
  *                    HSP_IF_ERROR Init failure
  */
hsp_if_status_t HSP_BRAM_IF_Init(hsp_bram_handle_t *hhsp_bram, void *hdriver)
{
  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));
  HSP_ASSERT_DBG_PARAM((hdriver != NULL));

  uint32_t bram_static_allocation_size = 0U;
  hal_hsp_capabilities_t hsp_capa;

  if (HAL_HSP_GetCapabilities((hal_hsp_handle_t *)hdriver, &hsp_capa) != HAL_OK)
  {
    return HSP_IF_ERROR;
  }

  /* Verify reserved size for AI is coherent with MemAB size */
#if (HSP_BRAM_AI_SIZE != 0U)
  if (HSP_BRAM_AI_SIZE > (hsp_capa.bramab_size_byte >> 1U))
  {
    return HSP_IF_ERROR;
  }
#endif /* HSP_BRAM_AI_SIZE */

  uint32_t persistentSize = hsp_capa.bramab_size_byte - (HSP_BRAM_AI_SIZE * sizeof(uint32_t));

  hhsp_bram->baseSharedAddr = hsp_capa.bramab_base_address;
  hhsp_bram->maxSizeToAllocate = hsp_capa.bramab_size_byte;
  hhsp_bram->currentPersistentOffset = 0U;
  hhsp_bram->topSharedAddr = hhsp_bram->baseSharedAddr + hhsp_bram->maxSizeToAllocate;
  hhsp_bram->currentSharedAddr = hhsp_bram->baseSharedAddr;
  hhsp_bram->currentSharedOffset = hhsp_bram->maxSizeToAllocate / sizeof(uint32_t);

  hhsp_bram->baseSharedAddrA = hsp_capa.brama_base_address + persistentSize;
  hhsp_bram->baseSharedAddrB = hsp_capa.bramb_base_address + persistentSize;
  hhsp_bram->currentSharedAddrA = hhsp_bram->baseSharedAddrA;
  hhsp_bram->currentSharedAddrB = hhsp_bram->baseSharedAddrB;
  hhsp_bram->currentSharedOffsetA = 0U;
  hhsp_bram->currentSharedOffsetB = 0U;
  hhsp_bram->maxSizeToAllocateA = HSP_BRAM_AI_SIZE; /* size in word */
  hhsp_bram->maxSizeToAllocateB = HSP_BRAM_AI_SIZE; /* size in word */

  /* Get Size of static allocation in BRAM region */
  /* Total space used in shared memory after symbol placement */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
  bram_static_allocation_size = (uint32_t)((&Image$$RW_HSP_DATA_BRAM$$Limit - &Image$$RW_HSP_DATA_BRAM$$Base) * 4U);
#elif defined (__GNUC__)
  bram_static_allocation_size = &__section_static_hsp_data_bram_end__ - &__section_static_hsp_data_bram_start__;
#elif defined (__ICCARM__)
  bram_static_allocation_size = __section_size("HSP_DATA_BRAM");
#else /* __ICCARM__ */
#error "Compiler not supported"
#endif /* __ICCARM__ */
  bram_static_allocation_size += (bram_static_allocation_size % sizeof(uint32_t));

  if ((bram_static_allocation_size + HSP_BRAM_AI_SIZE) > hhsp_bram->maxSizeToAllocate)
  {
    /*  No more space in BRAM */
    return HSP_IF_ERROR;
  }
  else
  {
    /* Calculate shared remaining size */
    hhsp_bram->maxSizeToAllocate -= bram_static_allocation_size;
    /* Used to calculate HSP bram address */
    hhsp_bram->bramOffset = HSP_SHARED_MEMORY_BASE_ADDR - hhsp_bram->baseSharedAddr;
  }

  hhsp_bram->maxSizePersistent = hhsp_bram->maxSizeToAllocate - (HSP_BRAM_AI_SIZE * sizeof(uint32_t));
  hhsp_bram->currentPersistentOffset = (bram_static_allocation_size / sizeof(uint32_t));

  return HSP_IF_OK;
}

/**
  * @brief  DeInitialize timer.
  */
void HSP_BRAM_IF_DeInit(hsp_bram_handle_t *hhsp_bram)
{
  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  hhsp_bram->baseSharedAddr = 0U;
  hhsp_bram->maxSizeToAllocate = 0U;
  hhsp_bram->maxSizePersistent = 0U;
  hhsp_bram->currentSharedOffset = 0U;
  hhsp_bram->currentPersistentOffset = 0U;
  hhsp_bram->topSharedAddr = 0U;
  hhsp_bram->currentSharedAddr = 0U;
  hhsp_bram->baseSharedAddrA = 0U;
  hhsp_bram->baseSharedAddrB = 0U;
}

/**
  * @brief  Allocation in BRAM_AB
  * @param  hhsp_bram            HSP BRAM handle
  * @param  alloc_size_in_word   Number of word to allocate
  * @param area                  HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                              HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval                      NULL  Allocation failure
  *                              Address value if success
  */
void *HSP_BRAM_IF_Malloc(hsp_bram_handle_t *hhsp_bram, uint32_t alloc_size_in_word, hsp_bram_allocation_t area)
{
  uint32_t addr;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  if (area == HSP_BRAM_ALLOCATION_PERSISTENT)
  {
    /* Allocation bottom to top */
    if ((alloc_size_in_word * sizeof(uint32_t)) > (int32_t)(hhsp_bram->maxSizePersistent))
    {
      return (NULL);
    }
    /* addr is a byte address */
    addr = hhsp_bram->baseSharedAddr + (hhsp_bram->currentPersistentOffset * sizeof(uint32_t));
    hhsp_bram->currentPersistentOffset += alloc_size_in_word; /* currentPersistentOffset is calculate in word */
    /* Calculate shared remaining size */
    hhsp_bram->maxSizePersistent  -= (alloc_size_in_word * sizeof(uint32_t));  /* maxSizePersistent is byte  */
    /* Update shared remaining size */
    hhsp_bram->maxSizeToAllocate  -= (alloc_size_in_word * sizeof(uint32_t));  /* maxSizeToAllocate is byte  */

    return ((void *)addr);
  }
  else
  {
    if (area == HSP_BRAM_ALLOCATION_DEFAULT)
    {
      /* Allocation top to bottom */
      if ((alloc_size_in_word * sizeof(uint32_t)) > hhsp_bram->maxSizeToAllocate)
      {
        return (NULL);
      }
      /* Update the current AB shared offset */
      hhsp_bram->currentSharedOffset -= alloc_size_in_word; /* currentSharedOffset is calculate in word */
      /* addr is a byte address */
      addr = (uint32_t) hhsp_bram->baseSharedAddr + (hhsp_bram->currentSharedOffset * sizeof(uint32_t));
      /* Calculate shared remaining size */
      hhsp_bram->maxSizeToAllocate -= (alloc_size_in_word * sizeof(uint32_t));  /* maxSizeToAllocate is byte  */
      /* Check if allocation overflows in persistent memory */
      if (HSP_BRAM_AI_SIZE != 0U)
      {
        uint32_t baseAi = hhsp_bram->topSharedAddr - (HSP_BRAM_AI_SIZE * sizeof(uint32_t));

        if (addr < baseAi) /* address overlay in persistent area */
        {
          /* Non persistent allocation overlay in persistent area: subtract overlap */
          hhsp_bram->maxSizePersistent = ((hhsp_bram->currentSharedOffset - hhsp_bram->currentPersistentOffset) * sizeof(uint32_t));

        }
      }
      return ((void *)addr);
    }
    else
    {
      return (NULL);
    }
  }
}

/**
  * @brief Set value address in pointer of pointer in HSP memory mapping.
  * @param hhsp_bram   HSP BRAM handle
  * @param pop         Pointer of pointer
  * @param addr        Address to set in pointer of pointer
  * @retval            HSP_IF_OK : success
  */
hsp_if_status_t HSP_BRAM_IF_SetPopAddress(hsp_bram_handle_t *hhsp_bram, uint32_t *pop, uint32_t addr)
{
  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));
  HSP_ASSERT_DBG_PARAM((pop != NULL));

  *pop = addr;

  return HSP_IF_OK;
}

/**
  * @brief Create FIR filter state in HSP internal memory.
  * @param hhsp_bram   HSP BRAM handle
  * @param nbTaps      Number of filter taps
  * @param nbSamples   Number of float elements to proceed
  * @param area        HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                    HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval            If allocation succeed, returns identifier of the created filter state
  *                    otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_Fir(hsp_bram_handle_t *hhsp_bram, uint32_t nbTaps,
                                                                uint32_t nbSamples, hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbTaps + 1U;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, nbTaps, area)) == NULL)
  {
    return (0U);
  }
  if (nbSamples == 1U)
  {
    fs->dirCmd = HSP_DIRECT_CMD_FIR_1SAMPLE_F32;
  }
  else
  {
    if (nbTaps < 7U)
    {
      fs->dirCmd = HSP_DIRECT_CMD_FIR_LESS7TAPS_F32;
    }
    else
    {
      if (((nbTaps & 1U) != 1U) && ((nbTaps & 3U) == 0U) && ((nbSamples & 1U) == 0U) && (nbSamples > 4U))
      {
        fs->dirCmd = HSP_DIRECT_CMD_FIR_MODULO4TAPS_F32;
      }
      else
      {
        fs->dirCmd = HSP_DIRECT_CMD_FIR_F32;
      }
    }
  }

  return ((uint32_t)fs);
}

/**
  * @brief Create FIR DECIMATE filter state in HSP internal memory.
  * @param hhsp_bram    HSP BRAM handle
  * @param nbTaps       Number of filter taps
  * @param nbSamples    Number of float elements to proceed
  * @param decimFactor  Decimation factor
  * @param area         HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                     HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval             If allocation succeed, returns identifier of the created filter state
  *                     otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_FirDecimate(hsp_bram_handle_t *hhsp_bram, uint32_t nbTaps,
                                                                        uint32_t nbSamples, uint32_t decimFactor,
                                                                        hsp_bram_allocation_t area)
{
  uint32_t addr;
  uint32_t stateSize;
  uint32_t ouAddr;
  uint32_t sizeToMalloc;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbTaps + 1U;
  sizeToMalloc = stateSize + (IF_SIZEOF_HEADER_FIR_DECIM_STATE_BUFF / sizeof(uint32_t));

  if ((addr = (uint32_t)HSP_BRAM_IF_Malloc(hhsp_bram, sizeToMalloc, area)) != (uint32_t)NULL)
  {
    /* First reset filter state buffer */
    float32_t *ptmp = (float32_t *)addr;
    for (uint32_t i = 0; i < sizeToMalloc; i++)
    {
      ptmp[i] = 0.0f;
    }

    /* Initialize filter state structure */
    hsp_hw_if_fir_decimate_filter_state_t *fs = (hsp_hw_if_fir_decimate_filter_state_t *)addr;
    uint32_t offset = ((uint32_t)fs - hhsp_bram->baseSharedAddr);
    ouAddr = HSP_SHARED_MEMORY_BASE_ADDR + offset; /* buffer addr in HSP memory space */
    uint32_t startState = (uint32_t)ouAddr + IF_SIZEOF_HEADER_FIR_DECIM_STATE_BUFF;
    fs->addr = (float32_t *)startState; /* State buffer address in HSP memory space */
    fs->ptr = (float32_t *)startState; /* Current pointer position in HSP memory space */
    /* This field (size) is used by FW and contains taps number */
    fs->size = nbTaps;
    fs->addrHsp = ouAddr + 8U; /*  sizeof (dirCmd) and (addrHsp) fields */

    if (nbTaps == 1U)
    {
      fs->dirCmd = HSP_DIRECT_CMD_FIRDEC_1TAP_F32;
    }
    else
    {
      if (nbTaps >= nbSamples)
      {
        fs->dirCmd = HSP_DIRECT_CMD_FIRDEC_SPEC_F32;
      }
      else
      {
        uint32_t quotient = (nbTaps / decimFactor);
        uint32_t remainder = nbTaps - (quotient * decimFactor);
        if (remainder > 1U)
        {
          fs->firstLoop = quotient + 1U;
        }
        else
        {
          fs->firstLoop = quotient;
        }
        fs->secondLoop = (nbSamples / decimFactor) - fs->firstLoop;
        if (fs->secondLoop < 1U)
        {
          fs->dirCmd = HSP_DIRECT_CMD_FIRDEC_SPEC_F32;
        }
        else
        {
          fs->dirCmd = HSP_DIRECT_CMD_FIRDEC_F32;
        }
      }
    }

    return ((uint32_t)fs);
  }

  /*  Invalid state identifier */
  return (0U);
}

/**
  * @brief Create BIQUAD Cascade Df1 filter state in HSP internal memory.
  * @param hhsp_bram      HSP BRAM handle
  * @param nbStages       Number of stage
  * @param area           HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                       HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval               If allocation succeed, returns identifier of the created filter state
  *                       otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_BiquadCascadeDf1(hsp_bram_handle_t *hhsp_bram,
                                                                             uint32_t nbStages,
                                                                             hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbStages * 4U;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, nbStages, area)) == NULL)
  {
    return (0U);
  }

  fs->dirCmd = HSP_DIRECT_CMD_BIQUAD_CASCADE_DF1_F32;

  return ((uint32_t)fs);
}

/**
  * @brief Create BIQUAD Cascade Df2T filter state in HSP internal memory.
  * @param hhsp_bram    HSP BRAM handle
  * @param nbStages     Number of stage
  * @param area         HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                     HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval             If allocation succeed, returns identifier of the created filter state
  *                     otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_BiquadCascadeDf2t(hsp_bram_handle_t *hhsp_bram,
                                                                              uint32_t nbStages,
                                                                              hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbStages * 2U;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, nbStages, area)) == NULL)
  {
    return (0U);
  }

  fs->dirCmd = HSP_DIRECT_CMD_BIQUAD_CASCADE_DF2T_F32;

  return ((uint32_t)fs);
}

/**
  * @brief Create LMS filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param nbTaps        Number of filter taps
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_Lms(hsp_bram_handle_t *hhsp_bram, uint32_t nbTaps,
                                                                hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbTaps + 1U;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, nbTaps, area)) == NULL)
  {
    return (0U);
  }

  fs->dirCmd = HSP_DIRECT_CMD_LMS_F32;

  return ((uint32_t)fs);
}

/**
  * @brief Create IIR Lattice filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param nbStages      Number of stage
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_IirLattice(hsp_bram_handle_t *hhsp_bram, uint32_t nbStages,
                                                                       hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbStages;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, nbStages, area)) == NULL)
  {
    return (0U);
  }

  fs->dirCmd = HSP_DIRECT_CMD_IIR_LATTICE_F32;

  return ((uint32_t)fs);
}

/**
  * @brief Create IIR Df1 filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param nbStages      Number of stage
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_IirDf1(hsp_bram_handle_t *hhsp_bram, uint32_t nbStages,
                                                                   hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = nbStages * 2U;
  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, stateSize, area)) == NULL)
  {
    return (0U);
  }

  fs->dirCmd = HSP_DIRECT_CMD_IIR_DF1_F32;

  return ((uint32_t)fs);
}

#if defined(__HSP_DMA__)
/**
  * @brief Create IIR 4p4z filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_Iir4p4z(hsp_bram_handle_t *hhsp_bram,
                                                                    hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = 10U;

  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, stateSize, area)) == NULL)
  {
    return (0U);
  }

  fs->ptr = fs->ptr + 1U; /* Current pointer position in HSP memory space */
  fs->dirCmd = HSP_DIRECT_CMD_IIR_4P4Z_1S_F32;

  return ((uint32_t)fs);
}
#endif /* __HSP_DMA__ */

/**
  * @brief Create IIR 3p3z filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_Iir3p3z(hsp_bram_handle_t *hhsp_bram,
                                                                    hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = 6U;

  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, stateSize, area)) == NULL)
  {
    return (0U);
  }

  fs->ptr = fs->ptr + 1U; /* Current pointer position in HSP memory space */
  fs->dirCmd = HSP_DIRECT_CMD_IIR_3P3Z_1S_F32;

  return ((uint32_t)fs);
}

/**
  * @brief Create IIR 2p2z filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns identifier of the created filter state
  *                      otherwise returns 0
  */
hsp_filter_state_identifier_t HSP_BRAM_IF_MallocStateBuffer_Iir2p2z(hsp_bram_handle_t *hhsp_bram,
                                                                    hsp_bram_allocation_t area)
{
  uint32_t stateSize;

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  stateSize = 4U;

  hsp_hw_if_filter_state_t *fs;

  if ((fs = HSP_BRAM_IF_MallocStateBuffer(hhsp_bram, stateSize, stateSize, area)) == NULL)
  {
    return (0U);
  }

  fs->ptr = fs->ptr + 1U; /* Current pointer position in HSP memory space */
  fs->dirCmd = HSP_DIRECT_CMD_IIR_2P2Z_1S_F32;

  return ((uint32_t)fs);
}

/** @addtogroup HSP_BRAM_IF_Private_Functions
  * @{
  */
/**
  * @brief Create filter state in HSP internal memory.
  * @param hhsp_bram     HSP BRAM handle.
  * @param stateSize     State size needed by filter
  * @param info          Information needed by FW depending of filter type
  * @param area          HSP_BRAM_ALLOCATION_DEFAULT: allocation in all BRAM
  *                      HSP_BRAM_ALLOCATION_PERSISTENT: allocation in persistent memory
  * @retval              If allocation succeed, returns a filter state structure
  *                      otherwise returns 0
  */
static hsp_hw_if_filter_state_t *HSP_BRAM_IF_MallocStateBuffer(hsp_bram_handle_t *hhsp_bram, uint32_t stateSize,
                                                               uint32_t info, hsp_bram_allocation_t area)
{
  uint32_t addr;
  uint32_t ouAddr;
  uint32_t sizeToMalloc = stateSize + (IF_SIZEOF_HEADER_STATE_BUFF  / sizeof(uint32_t));

  HSP_ASSERT_DBG_PARAM((hhsp_bram != NULL));

  if ((addr = (uint32_t)HSP_BRAM_IF_Malloc(hhsp_bram, sizeToMalloc, area)) != (uint32_t)NULL)
  {
    /* First reset filter state buffer */
    float32_t *ptmp = (float32_t *)addr;
    for (uint32_t i = 0; i < sizeToMalloc; i++)
    {
      ptmp[i] = 0.0f;
    }
    /* Initialize filter state structure */
    hsp_hw_if_filter_state_t *fs = (hsp_hw_if_filter_state_t *)addr;
    uint32_t offset = ((uint32_t)fs - hhsp_bram->baseSharedAddr);
    ouAddr = HSP_SHARED_MEMORY_BASE_ADDR + offset; /* buffer addr in HSP memory space */
    uint32_t startState = (uint32_t)ouAddr + IF_SIZEOF_HEADER_STATE_BUFF;
    fs->addr = (float32_t *)startState; /* State buffer address in HSP memory space */
    fs->ptr = (float32_t *)startState; /* Current pointer position in HSP memory space */
    /* This field (size) is used by FW and its meaning is filter type dependent */
    fs->size = info; /* Buffer size */
    fs->addrHsp = ouAddr + 8U; /*  sizeof (dirCmd) and (addrHsp) fields */
    return (fs);
  }

  return (NULL);
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
