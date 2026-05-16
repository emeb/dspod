/**
  ******************************************************************************
  * @file    hsp_api_def.h
  * @brief   Header file
  ******************************************************************************
  * @attention
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
#ifndef HSP_API_DEF_H
#define HSP_API_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hsp_fw_def.h"

#if defined(USE_HSP_ARM_MATH_TYPES) && (USE_HSP_ARM_MATH_TYPES == 1)
/* Include of "arm_math_types.h" for the declaration of float32_t type */
#include "arm_math_types.h"
#endif /* USE_HSP_ARM_MATH_TYPES */

#if defined(USE_HSP_MEMCPY) && (USE_HSP_MEMCPY == 1)
#define HSP_MEMCPY stm32_hsp_memcpy
#else /* USE_HSP_MEMCPY */
#define HSP_MEMCPY memcpy
#endif  /* USE_HSP_MEMCPY */

/** @defgroup HSP_ENGINE  HSP Engine
  * @{
  */

/** @addtogroup HSP_CORE
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup HSP_COMMON_Exported_Constants HSP Common Exported Constants
  * @{
  */
#define HSP_STREAM_BUFFER_NBR  4U

/** @defgroup HSP_SEQ_Exported_Constants HSP Sequencer Exported Constants
  * @{
  */
#define HSP_SEQ_IOTYPE_DEFAULT 0x00000000  /*!< Param in BRAM memory */
#define HSP_SEQ_IOTYPE_POP_0   0x00000001  /*!< First param is pop */
#define HSP_SEQ_IOTYPE_POP_1   0x00000002  /*!< Second param is pop */
#define HSP_SEQ_IOTYPE_POP_2   0x00000004  /*!< Third param is pop */
#define HSP_SEQ_IOTYPE_IMM_0   0x00000008  /*!< First scalar param is an immediate */
#define HSP_SEQ_IOTYPE_IMM_1   0x00000010  /*!< Second scalar param is an immediate */
#define HSP_SEQ_IOTYPE_RXTX_0  0x00000020  /*!< First param is RXTX buffer */
#define HSP_SEQ_IOTYPE_RXTX_1  0x00000040  /*!< Second param is RXTX buffer */
#define HSP_SEQ_IOTYPE_RXTX_2  0x00000080  /*!< Third param is RXTX buffer */
#if defined(__HSP_DMA__)
#define HSP_SEQ_IOTYPE_EXT_0   0x00000100  /*!< First param is external */
#define HSP_SEQ_IOTYPE_EXT_1   0x00000200  /*!< Second param is external */
#define HSP_SEQ_IOTYPE_EXT_2   0x00000400  /*!< Third param is external */
#define HSP_SEQ_IOTYPE_ADC_0   0x00001000  /*!< First param is ADC */
#define HSP_SEQ_IOTYPE_ADC_1   0x00002000  /*!< Second param is ADC */
#define HSP_SEQ_IOTYPE_ADC_2   0x00004000  /*!< Third param is ADC */

#define HSP_SEQ_IOTYPE_AP_0_0  0x00010000  /*!< First param is STI AP0 */
#define HSP_SEQ_IOTYPE_AP_0_1  0x00020000  /*!< First param is STI AP1 */
#define HSP_SEQ_IOTYPE_AP_0_2  0x00030000  /*!< First param is STI AP2 */
#define HSP_SEQ_IOTYPE_AP_0_3  0x00040000  /*!< First param is STI AP3 */
#define HSP_SEQ_IOTYPE_AP_0_4  0x00050000  /*!< First param is STI AP4 */

#define HSP_SEQ_IOTYPE_AP_1_0  0x00100000  /*!< Second param is STI AP0 */
#define HSP_SEQ_IOTYPE_AP_1_1  0x00200000  /*!< Second param is STI AP1 */
#define HSP_SEQ_IOTYPE_AP_1_2  0x00300000  /*!< Second param is STI AP2 */
#define HSP_SEQ_IOTYPE_AP_1_3  0x00400000  /*!< Second param is STI AP3 */
#define HSP_SEQ_IOTYPE_AP_1_4  0x00500000  /*!< Second param is STI AP4 */

#define HSP_SEQ_IOTYPE_AP_2_0  0x01000000  /*!< Third param is STI AP0 */
#define HSP_SEQ_IOTYPE_AP_2_1  0x02000000  /*!< Third param is STI AP1 */
#define HSP_SEQ_IOTYPE_AP_2_2  0x03000000  /*!< Third param is STI AP2 */
#define HSP_SEQ_IOTYPE_AP_2_3  0x04000000  /*!< Third param is STI AP3 */
#define HSP_SEQ_IOTYPE_AP_2_4  0x05000000  /*!< Third param is STI AP4 */
#endif /* __HSP_DMA__ */

#define HSP_SEQ_SENDEVT_ITF_HSEG  0U
#define HSP_SEQ_SENDEVT_ITF_HDEG  1U

/**
  * @}
  */

/**
  * @}
  */

/* Exported Macros -----------------------------------------------------------*/
/** @defgroup HSP_API_DEF_Exported_Macros HSP Common Exported Macros
  * @{
  */
/**
  * @brief Transform a event id in range [0..31] to a bitfield mask
  * @param event_id ID of HSP event
  * @return mask
  */
#define HSP_CORE_EVENT_ID_TO_BITMASK(event_id)  (1UL << event_id)

#if defined(USE_HSP_ASSERT_DBG_PARAM) && (USE_HSP_ASSERT_DBG_PARAM == 1)
/**
  * @brief  The HSP_ASSERT_DBG_PARAM macro is used for function's parameters check.
  * @param  expr If expr is false, it calls hsp_assert_dbg_param_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
#define HSP_ASSERT_DBG_PARAM(expr) ((expr) ? (void)0U : hsp_assert_dbg_param_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void hsp_assert_dbg_param_failed(uint8_t *file, uint32_t line);
#else /* USE_HSP_ASSERT_DBG_PARAM */
#define HSP_ASSERT_DBG_PARAM(expr) ((void)0U)
#endif /* USE_HSP_ASSERT_DBG_PARAM */

#if defined(USE_HSP_ASSERT_DBG_STATE) && (USE_HSP_ASSERT_DBG_STATE == 1)
/**
  * @brief  The HSP_ASSERT_DBG_STATE macro is used for function's states check.
  * @param  __STATE__ the state filed within the PPP handle
  * @param  __VAL__ the authorized states value(s) to be checked
  *                 can be a combination of states
  * @note   if __STATE__ & __VAL__ is zero (unauthorized state) then
  * @note   assert_dbg_state_failed function is called which reports
  *         the name of the source file and the source line number of the call that failed.
  *         if __STATE__ & __VAL__ is zero (unauthorized state) then, the HSP_ASSERT_DBG_STATE macro returns no value.
  */
#define HSP_ASSERT_DBG_STATE(__STATE__,__VAL__) (((((uint32_t)(__STATE__)) &  ((uint32_t)(__VAL__))) != 0U) ?  \
                                                 (void)0U :                                                    \
                                                 hsp_assert_dbg_state_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void hsp_assert_dbg_state_failed(uint8_t *file, uint32_t line);
#else /* USE_HSP_ASSERT_DBG_STATE */
#define HSP_ASSERT_DBG_STATE(__STATE__,__VAL__) ((void)0U)
#endif /* USE_HSP_ASSERT_DBG_STATE */

/** @brief Check the current Middleware handle state and move it to new state in an atomic way.
  * @param handle specifies the Middleware Handle.
  * @param state_field specifies the state field within the Handle.
  * @param ppp_conditional_state state to be checked to authorize moving to the new state.
  * @param ppp_new_state new state to be set.
  * @note  This macro can be used for the following purpose:
  *        - When the define USE_HSP_CHECK_PROCESS_STATE is set to "1", this macro allows to check the current
  *          handle state versus a conditional state and if true set to the desired new state.
  *          the check and update of the state is done using exclusive Load/store instruction making
  *          the operation atomic
  *        - When the define USE_HSP_CHECK_PROCESS_STATE is not set to "1", this macro simply assign the new
  *          handle state field to the new desired state without any check
  * @retval HAL_BUSY if the define USE_HSP_CHECK_PROCESS_STATE is set to "1" and the current state doesn't match
  *         ppp_conditional_state.
  */
#if defined(USE_HSP_CHECK_PROCESS_STATE) && (USE_HSP_CHECK_PROCESS_STATE == 1)
#define HSP_CHECK_UPDATE_STATE(handle, state_field, ppp_conditional_state, ppp_new_state)                           \
  do {                                                                                                              \
    do {                                                                                                            \
      /* Return HAL_BUSY if the status is not ready */                                                              \
      if (__LDREXW((volatile uint32_t *)((uint32_t)&(handle)->state_field)) != (uint32_t)(ppp_conditional_state))   \
      {                                                                                                             \
        return HSP_CORE_BUSY;                                                                                       \
      }                                                                                                             \
      /* if state is ready then attempt to change the state to the new one */                                       \
    } while (__STREXW((uint32_t)(ppp_new_state), (volatile uint32_t *)((uint32_t)&((handle)->state_field))) != 0U); \
    /* Do not start any other memory access until memory barrier is complete */                                     \
    __DMB();                                                                                                        \
  } while (0)
#else /* USE_HSP_CHECK_PROCESS_STATE */
#define HSP_CHECK_UPDATE_STATE(handle, state_field, ppp_conditional_state, ppp_new_state) \
  (handle)->state_field = (ppp_new_state)
#endif /* USE_HSP_CHECK_PROCESS_STATE == 1 */

/**
  * @}
  */

/* Exported Types ------------------------------------------------------------*/
/** @defgroup HSP_COMMON_Exported_Types HSP Common Exported Types
  * @{
  */
#if !defined(USE_HSP_ARM_MATH_TYPES) || (USE_HSP_ARM_MATH_TYPES == 0)
/** Define the float32_t based on the default float type */
typedef float float32_t;
#endif /* USE_HSP_ARM_MATH_TYPES */

/** @defgroup HSP_BRAM_Exported_Types HSP BRAM Exported Types
  * @{
  */
/** BRAM Allocation area: in full memory or in persistent memory */
typedef enum
{
  HSP_BRAM_ALLOCATION_DEFAULT = 0, /*!< Risk of data corruption when AI is used due to memory overlay */
  HSP_BRAM_ALLOCATION_PERSISTENT   /*!< Data cannot be corrupted by AI                                */
} hsp_bram_allocation_t;

/** Filter state identifier */
typedef uint32_t hsp_filter_state_identifier_t;

#if defined(__HSP_DMA__)

#define HSP_DC_PID_FILTER_BYP 0 /*!< PID Bypass mode */
#define HSP_DC_PID_FILTER_F1  1 /*!< PID filter type 1 */
#define HSP_DC_PID_FILTER_F2  2 /*!< PID Bypass type 2 */

/*!< Common structure for all PIs */
typedef struct
{
  float32_t kp;       /*!< Proportional gain */
  float32_t ki;       /*!< Integral gain */
  float32_t kfb;      /*!< Feedback gain (discharge) */
  float32_t itg;      /*!< integral term */
  float32_t itgL[2];  /*!< Integral threshold */
  float32_t pItgL[2]; /*!< Output threshold */
  float32_t ffd;      /*!< Feed-forward requested */
} hyp_dc_pipx_ctrl_t;

/*!< Structure for PID controllers  */
typedef struct
{
  hyp_dc_pipx_ctrl_t pip;
  float32_t outL[2]; /*!< Output threshold */
  float32_t kd;      /*!< Differential gain */
  float32_t err1;    /*!< previous process variable error */
  float32_t A;       /*!< A = W/(W+2); W=2*tan(pi* Fc/Fs) */
  float32_t B;       /*!< B = (W-2)/(W+2); */
  float32_t diff1;   /*!< previous process diff. value */
  uint32_t cfgLpf;
} hyp_dc_pidpx_ctrl_t;

/*!< Structure for generic PID controller  */
typedef struct
{
  hyp_dc_pipx_ctrl_t pip;
  float32_t outL[2]; /*!< Output threshold */
  float32_t kd;      /*!< Differential gain */
  float32_t err1;    /*!< previous process variable error */
  float32_t A;       /*!< A = W/(W+2); W=2*tan(pi* Fc/Fs) */
  float32_t B;       /*!< B = (W-2)/(W+2); */
  float32_t diff1;   /*!< previous process diff. value */
  uint32_t cfgLpf;
  uint32_t pis;
  uint32_t dis;
  uint32_t diff;
} hyp_dc_gpid_ctrl_t;

/*!< Proportional resonant data structure */
typedef struct
{
  float32_t kp; /*!< Proportional gain */
  float32_t a1; /*!< filter coef a1 */
  float32_t b1; /*!< filter coef b1 */
  float32_t b2; /*!< filter coef b2 */
  float32_t b0; /*!< filter coef 1/b0 */
  float32_t resL[2]; /*!< Resonant threshold */
  float32_t outL[2]; /*!< Output threshold */
} hyp_dc_ppr1_ctrl_t;

/*!< Structure for generic PID controller */
typedef struct
{
  hyp_dc_pipx_ctrl_t pip;
  float32_t busComp;
  float32_t xclpO;
  float32_t xclpI;
} hyp_dc_pis3_ctrl_t;

/*!< Proportional resonant state structure */
typedef struct
{
  float32_t err2;
  float32_t out1;
  float32_t out2;
  float32_t err1;
} hyp_dc_ppr1_sta_t;

#endif /* __HSP_DMA__ */

/**
  * @brief HSP BRAM Handle Type Structure Definition
  */
typedef struct hsp_bram_handle_s hsp_bram_handle_t;

/**
  * @brief HSP BRAM Handle Structure Definition
  */
struct hsp_bram_handle_s
{
  uint32_t baseSharedAddr;          /*!< Base address of the shared memory */
  uint32_t topSharedAddr;           /*!< Top address of the shared memory */
  uint32_t currentSharedAddr;       /*!< Current top address of the shared memory */
  uint32_t maxSizeToAllocate;       /*!< Maximum size that can be allocated */
  uint32_t maxSizePersistent;       /*!< Maximum size that can be allocated in persistent memory */
  uint32_t currentSharedOffset;     /*!< Start offset to allocate shared memory buffer */
  uint32_t currentPersistentOffset; /*!< Start offset to allocate shared memory buffer */
  int32_t  bramOffset;              /*!< Offset used to calculate HSP shared address */
  uint32_t baseSharedAddrA;         /*!< Base address of the MEMA memory */
  uint32_t baseSharedAddrB;         /*!< Base address of the MEMB memory */
  uint32_t maxSizeToAllocateA;      /*!< Maximum size that can be allocated in MEMA */
  uint32_t maxSizeToAllocateB;      /*!< Maximum size that can be allocated in MEMB */
  uint32_t currentSharedAddrA;      /*!< Current top address of the shared memory MEMA */
  uint32_t currentSharedAddrB;      /*!< Current top address of the shared memory MEMB */
  uint32_t currentSharedOffsetA;    /*!< Start offset to allocate shared memory buffer in MEMA */
  uint32_t currentSharedOffsetB;    /*!< Start offset to allocate shared memory buffer in MEMB */
};

/**
  * @}
  */

/** @defgroup HSP_SEQ_Exported_Types HSP Sequencer Exported Types
  * @{
  */

/**
  * @brief Define the conditional command for Processing List
  */
typedef enum
{
  HSP_SEQ_CMP_IFEQ,     /*!< if comparison equal */
  HSP_SEQ_CMP_IFNE,     /*!< if comparison not equal */
  HSP_SEQ_CMP_IFGT,     /*!< if comparison greater than */
  HSP_SEQ_CMP_IFLT,     /*!< if comparison less than */
  HSP_SEQ_CMP_IFGE,     /*!< if comparison greater or equal */
  HSP_SEQ_CMP_IFLE,     /*!< if comparison less or equal */
} hsp_proclist_cond_cmd_t;

typedef enum
{
  HSP_SEQ_CMP_VECT_TYPE_GT = HSP_CMP_TYPE_GT, /*!< comparison greater than */
  HSP_SEQ_CMP_VECT_TYPE_GE = HSP_CMP_TYPE_GE, /*!< comparison greater or equal */
  HSP_SEQ_CMP_VECT_TYPE_LT = HSP_CMP_TYPE_LT, /*!< comparison less than */
  HSP_SEQ_CMP_VECT_TYPE_LE = HSP_CMP_TYPE_LE, /*!< comparison less or equal */
  HSP_SEQ_CMP_VECT_TYPE_EQ = HSP_CMP_TYPE_EQ, /*!< comparison equal */
} hsp_proclist_cmp_vect_cond_cmd_t;

/**
  * @brief Define the type of RFFT
  */
typedef enum
{
  HSP_RFFT_TYPE_1 = HSP_TYPE_RFFT_TYPE_1, /*!< RFFT: identify a real FFT with out[0]=in[0]+in[1] and out[1]=0 */
  HSP_RFFT_TYPE_2 = HSP_TYPE_RFFT_TYPE_2, /*!< RFFT: identify a real FFT CMSIS-like */
  HSP_RFFT_TYPE_3 = HSP_TYPE_RFFT_TYPE_3  /*!< RFFT: identify a real FFT with n+1 elements with out[n]=in[0]+in[1] */
} hsp_type_rfft_cmd_t;

/**
  * @brief Define the fft log(point number)
  */
typedef enum
{
  HSP_LOG2NBP_32    = 5,  /*!< Value of Log2(32) for FFT 32 points */
  HSP_LOG2NBP_64    = 6,  /*!< Value of Log2(64) for FFT 64 points */
  HSP_LOG2NBP_128   = 7,  /*!< Value of Log2(128) forFFT 128 points */
  HSP_LOG2NBP_256   = 8,  /*!< Value of Log2(256) forFFT 256 points */
  HSP_LOG2NBP_512   = 9,  /*!< Value of Log2(512) forFFT 512 points */
  HSP_LOG2NBP_1024  = 10, /*!< Value of Log2(1024) forFFT 1024 points */
  HSP_LOG2NBP_2048  = 11, /*!< Value of Log2(2048) forFFT 2048 points */
  HSP_LOG2NBP_4096  = 12  /*!< Value of Log2(4096) forFFT 4096 points */
} hsp_ftt_lognbp_cmd_t;

/**
  * @brief  HSP type of memory for CRC calculation
  */
typedef enum
{
  HSP_CRC_CROM    = 0x00U, /*!< CROM memory  */
  HSP_CRC_DROM    = 0x01U, /*!< DROM memory  */
} hsp_crc_mem_type_cmd_t;

/**
  * @brief  HSP_CMD_CMPCNT threshold fields: [loTh, hiTh]
  */
typedef struct
{
  float32_t lo;     /*!< Low threshold value */
  float32_t hi;     /*!< High threshold value */
} hsp_cmp_cnt_lim_t;

/**
  * @brief  HSP_CMD_SCA_CLARKE ICLARKE configuration fields: [alpha, beta]
  */
typedef struct
{
  float32_t alpha; /*!< alpha */
  float32_t beta;  /*!< beta */
} hsp_i_alpha_beta_t;

/**
  * @brief  HSP_CMD_SCA_CLARKE ICLARKE configuration fields: [a, b]
  */
typedef struct
{
  float32_t a; /*!< a */
  float32_t b; /*!< b */
} hsp_i_a_b_t;

/**
  * @brief  HSP_CMD_SCA_PARK IPARK configuration fields: [alpha, beta]
  */
typedef struct
{
  float32_t alpha; /*!< alpha */
  float32_t beta;  /*!< beta */
} hsp_v_alpha_beta_t;

/**
  * @brief  HSP_CMD_SCA_PARK IPARK configuration fields: [q, d]
  */
typedef struct
{
  float32_t q; /*!< q */
  float32_t d; /*!< d */
} hsp_v_q_d_t;

/**
  * @brief  HSP_CMD_PID configuration fields:
  *   [IntegralTerm, UpperOutputLimit, LowerOutputLimit, KpGain, KiGain, KdGain, DerivativeTerm, PrevProcessVarError,
  *    UpperDerivativeLimit, LowerDerivativeLimit, Alpha]
  */
typedef struct
{
  float32_t IntegralTerm;     /*!< integral term */
  float32_t UpperOutputLimit; /*!< Upper limit used to saturate the PI output */
  float32_t LowerOutputLimit; /*!< Lower limit used to saturate the PI output */
  float32_t KpGain;           /*!< gain used by PID component */
  float32_t KiGain;           /*!< gain used by PID component */
  float32_t KdGain;           /*!< Kd gain used by PID component */
  float32_t DerivativeTerm;   /*!< Added for derivative term */
  float32_t PrevProcessVarError;  /*!< previous process variable used by the derivative part of the PID component */
  float32_t UpperDerivativeLimit; /*!< Upper limit used to saturate the derivative term */
  float32_t LowerDerivativeLimit; /*!< Lower limit used to saturate the derivative term */
  float32_t Alpha;                /*!< Added for LPF filter */
} hsp_pid_buff_cfg_t;

/**
  * @brief  HSP_CMD_CMPCNT max counter fields: [maxCntLo, maxCntHi]
  */
typedef struct
{
  uint32_t lo;  /*!< LO counter value: for number of consecutive value bigger than loTh */
  uint32_t hi;  /*!< HI counter value: for number of consecutive value lower than hiTh */
} hsp_cmp_cnt_cnt_t;

/**
  * @}
  */

/** @defgroup HSP_CORE_Exported_Types HSP Core Exported Types
  * @{
  */
/** HSP HAL Handle State enumeration */
typedef enum
{
  HSP_CORE_STATE_RESET              = 0UL,           /*!< HSP not yet initialized                                */
  HSP_CORE_STATE_FW_LOADED          = (1UL << 31U),  /*!< HSP FW is loaded                                       */
  HSP_CORE_STATE_READY              = (1UL << 30U),  /*!< HSP FW & Plugins are loaded and HSP is ready to boot   */
  HSP_CORE_STATE_IDLE               = (1UL << 29U),  /*!< HSP initialized, FW & Plugin Loaded and Boot completed */
  HSP_CORE_STATE_PROCLIST_RECORDING = (1UL << 28U),  /*!< HSP is recording a Processing List                     */
  HSP_CORE_STATE_CNN_ACTIVE         = (1UL << 27U),  /*!< HSP is running a CNN function                          */

  HSP_CORE_STATE_FAULT = (1UL << 0U),  /*!< HSP hardware error and a HW reset is required                        */
} hsp_core_state_t;

typedef enum
{
  HSP_CORE_OK            = 0x00U, /*!< MW HSP operation completed successfully */
  HSP_CORE_ERROR         = 0x01U, /*!< MW HSP operation completed with error   */
  HSP_CORE_BUSY          = 0x02U, /*!< MW HSP concurrent process ongoing       */
  HSP_CORE_INVALID_PARAM = 0x03U, /*!< MW HSP invalid parameter                */
  HSP_CORE_TIMEOUT       = 0x04U  /*!< MW HSP operation exceeds user timeout   */
} hsp_core_status_t;

/**
  * @brief HSP Middleware Context Type Structure Definition
  */
typedef struct hsp_engine_context_s hsp_engine_context_t;

/**
  * @brief HSP Middleware Context Structure Definition
  */
struct hsp_engine_context_s
{
  void *hdriver;
  volatile hsp_core_state_t global_state;

  hsp_bram_handle_t hbram; /*!< Handle for BRAM */

  void (* ErrorCallback)(uint32_t fw_error_code, uint32_t spe_error_inf);

#if defined(USE_HSP_MODULES_PROCLIST) && (USE_HSP_MODULES_PROCLIST == 1U)
  void (* ProcListEventCallback)(uint32_t pfct_events);
#endif /* USE_HSP_MODULES_PROCLIST */
#if defined(USE_HSP_ENGINE_STREAM_BUFFER) && (USE_HSP_ENGINE_STREAM_BUFFER == 1U)
  int32_t *p_stream_tx_data[HSP_STREAM_BUFFER_NBR];
  uint32_t p_stream_tx_data_size[HSP_STREAM_BUFFER_NBR];

  float *p_stream_rx_data[HSP_STREAM_BUFFER_NBR];
  uint32_t p_stream_rx_data_size[HSP_STREAM_BUFFER_NBR];
#endif /* USE_HSP_ENGINE_STREAM_BUFFER */

};

typedef void (*p_hsp_engine_cb_t)(uint32_t mw_id);
typedef void (*p_hsp_engine_one_param_cb_t)(uint32_t mw_id, uint32_t value);
typedef void (*p_hsp_engine_two_params_cb_t)(uint32_t mw_id, uint32_t value1, uint32_t value2);

#if defined(USE_HSP_PLUGIN)
/**
  * @brief HSP Plugin Structure Definition
  */
typedef struct
{
  const uint8_t *p_bin;
  uint32_t size_byte;
} hsp_core_plugin_t;
#endif /* USE_HSP_PLUGIN */

#if defined(__HSP_DMA__)
/**
  * @brief  HSP_CMD_RAMP configuration values
  */
typedef struct
{
  uint32_t init;    /*!< Force ramp init */
  float32_t start;  /*!< Start point */
  float32_t final;  /*!< Final point */
  uint32_t nbSteps; /*!< Number of steps to reach the final point */
  uint32_t stepCnt; /*!< Counter step */
  float32_t acc;    /*!< Accelerator generated shape */
} hsp_dc_ramp_cfg_t;
#endif /* __HSP_DMA__ */

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HSP_API_DEF_H */
