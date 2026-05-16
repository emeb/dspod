/**
  ******************************************************************************************
  * @file hsp_fw_def.h
  * @brief HSP FW header file for hsp_fw_def.h
  * Generated file for HSP users FW version 1.1.7  -- 2025-12-04 13:10:47 --
  *
  ******************************************************************************************
  * \attention
  *
  *  ST makes no warranty express or implied including but not limited to, any warranty of
  *  (i) merchantability or fitness for a particular purpose and/or (ii) requirements, for
  *  a particular purpose in relation to the LICENSED MATERIALS, which is provided <93>AS IS<94>,
  *  WITH ALL FAULTS. ST does not represent or warrant that the LICENSED MATERIALS provided
  *  hereunder is free of infringement of any third party patents, copyrights, trade secrets
  *  or other intellectual property rights. ALL WARRANTIES, CONDITIONS OR OTHER TERMS
  *  IMPLIED BY LAW ARE EXCLUDED TO THE FULLEST EXTENT PERMITTED BY LAW.
  *
  *  Copyright (c) 2025 STMicroelectronics.
  *  All rights reserved.
  *
  ******************************************************************************************
  */

#ifndef __HSP_FW_DEF__H__
#define __HSP_FW_DEF__H__


/*
 * HSP FW version defines
 */
#define HSP_FW_VERSION      0x01107107U /*!< HSP FW version (HWl.RomMaj.RomMin.RamMaj.RamMin) */
#define HSP_FW_VERSION_CRAM 0x00000107U /*!< HSP FW CRAM version (RamMaj.RamMin) */
#define HSP_FW_VERSION_CROM 0x01107000U /*!< HSP FW CROM version (HWl.RomMaj.RomMin) */


/** @addtogroup HSP_ENGINE
  * @{
  */
/** @addtogroup HSP_Private_Constants
  * @{
  */
/** @defgroup HSP_MEMORY_Constants HSP Memory Constants
  * @{
 */
/** HSP side: HSP memory base address */
#define HSP_SHARED_MEMORY_BASE_ADDR (0x40000U * 4U)

/** HSP CC DRAM addr for the moment in DRAM */
#define HSP_DRAM_CC_ADDR 0x8U

/** Direct command start address */
#define HSP_DC_START 0x280U

/**
  * @}
  */

/** @defgroup HSP_Command_Fields_Constants HSP Command Constants
  * @{
  */
/** Kernels list enumeration */
enum HSP_KernelNameDef
{
  /* Do not modify order nor add or suppress definition */

  /* Filter related functions */
  HSP_KER_FIR_F32 = 0U, /*!< FIR filter nb taps >= 7 and not modulo4 */
  HSP_KER_FIR_LESS7TAPS_F32, /*!< FIR filter nosimd for nb taps < 7 */
  HSP_KER_FIR_MODULO4TAPS_F32, /*!< FIR filter for modulo 4 taps + size even */
  HSP_KER_FIR_1SAMPLE_F32, /*!< FIR filter 1 sample (all taps supported) */
  HSP_KER_FIRDEC_F32, /*!< FIR filter, with decimation */
  HSP_KER_FIRDEC_1TAP_F32, /*!< FIR filter, with decimation for 1 tap */
  HSP_KER_FIRDEC_SPEC_F32, /*!< FIR filter, with decimation and special size: nb taps >= size and other conditions */
  HSP_KER_LMS_F32, /*!< filter, with adaptive LMS algorithm. */
  HSP_KER_BQ_CAS_DF1_F32, /*!< IIR biquad cascade DF1 filter */
  HSP_KER_BQ_CAS_DF2T_F32, /*!< IIR biquad cascade DF2T filter */
  HSP_KER_IIR_DF1_F32, /*!< IIR DF1 filter */
  HSP_KER_IIR_3P3Z_1S_F32, /*!< IIR 3p3z for 1 sample filter */
  HSP_KER_IIR_2P2Z_1S_F32, /*!< IIR 2p2z for 1 sample filter */
  HSP_KER_IIR_LATTICE_F32, /*!< IIR lattice filter */
  HSP_KER_CONV_F32, /*!< Convolution */
  HSP_KER_CORR_F32, /*!< Correlation */
  HSP_KER_WINSYM_F32, /*!< Window symmetric kernel */
  /* Vector related functions */
  HSP_KER_VEC_ADD_F32, /*!< Element-wise summation of a vector */
  HSP_KER_VEC_SUB_F32, /*!< Element-wise subtraction of a vector */
  HSP_KER_VEC_MUL_F32, /*!< Element-wise multiplication of a vector */
  HSP_KER_VEC_DIV_F32, /*!< Element-wise division of a vector */
  HSP_KER_VEC_SQRT_F32, /*!< Calculates the sum of the */
  HSP_KER_VEC_ABS_F32, /*!< Element-wise absolute value of a vector */
  HSP_KER_VEC_AVG_F32, /*!< Compute the average value of a vector */
  HSP_KER_VEC_RMS_F32, /*!< Compute the RMS value of a vector */
  HSP_KER_VEC_SCALE_F32, /*!< Multiply each vector element by a scalar */
  HSP_KER_VEC_OFFSET_F32, /*!< Add to each vector element a scalar */
  HSP_KER_VEC_MAX_F32, /*!< Search for value and position of the biggest element of a vector */
  HSP_KER_VEC_ABSMAX_F32, /*!< Search for value and position of the absolute biggest element of a vector */
  HSP_KER_VEC_MIN_F32, /*!< Search for value and position of the smallest element of a vector */
  HSP_KER_VEC_COPY, /*!< Performs the copy of a vector (can be used for debug) */
  HSP_KER_VEC_SET, /*!< Add kernel function pointer and parameters according to user command */
  HSP_KER_VEC_DOTPROD_F32, /*!< Calculates the dot prod of the input vectors */
  HSP_KER_VEC_COS_F32, /*!< Element-wise cosine value of a vector */
  HSP_KER_VEC_SIN_F32, /*!< Element-wise sine value of a vector */
  HSP_KER_VEC_SINCOS_F32, /*!< Element-wise sine cosine value of a vector */
  HSP_KER_VEC_F2I, /*!< Element-wise float32 to integer 32bits conversion */
  HSP_KER_VEC_I2F, /*!< Element-wise integer 32bits to float32 conversion */
  HSP_KER_VEC_24S2F, /*!< Element-wise integer 24bits signed to float32 conversion */
  HSP_KER_VEC_F2U, /*!< Element-wise float32 to unsigned integer 32bits conversion */
  HSP_KER_VEC_U2F, /*!< Element-wise unsigned integer 32bits to float32 conversion */
  HSP_KER_VEC_DECIM, /*!< Decimation */
  HSP_KER_VEC_ZINS, /*!< Zero insertion */
  HSP_KER_VEC_ATAN2_F32, /*!< ATAN2 computation */
  HSP_KER_VEC_LN_F32, /*!< LN computation */
  HSP_KER_VEC_LOG10_F32, /*!< LOG10 computation */
  HSP_KER_VEC_EXP_F32, /*!< Exponential computation */
  HSP_KER_VEC_EXP10_F32, /*!< Exponential base 10 computation */
  HSP_KER_VEC_FLOGS_F32, /*!< Fast logs (base e, 2, 10, Db)  computation */
  HSP_KER_VEC_FEXPS_F32, /*!< Fast exps (base e, 2, 10, Db)  computation */
  HSP_KER_VEC_MUL_COS_F32, /*!< Compute the multiplication of one vector and cos ROM */
  HSP_KER_VEC_MUL_SIN_F32, /*!< Compute the multiplication of one vector and sin ROM */
  HSP_KER_VEC_SCALE_I_F32, /*!< Multiply each vector element by an immediate scalar */
  HSP_KER_VEC_OFFSET_I_F32, /*!< Add to each vector element an immediate scalar */
  HSP_KER_VEC_SET_I, /*!< Set each vector element with an immediate scalar */
  HSP_KER_VEC_F2Q31, /*!< Element-wise float32 to Q31 conversion */
  HSP_KER_VEC_Q312F, /*!< Element-wise Q31 to float32 conversion */
  HSP_KER_VEC_F2Q15, /*!< Element-wise float32 to Q15 conversion */
  HSP_KER_VEC_Q152F, /*!< Element-wise Q15 to float32 conversion */
  /* Scalar related functions */
  HSP_KER_SCA_ATAN2_F32, /*!< ATAN2 of a scalar */
  HSP_KER_SCA_COS_F32, /*!< Cos */
  HSP_KER_SCA_F2Q31, /*!< float32 to Q31 conversion */
  HSP_KER_SCA_Q312F, /*!< Q31 to float32 conversion */
  HSP_KER_SCA_EXP_F32, /*!< Exponential computation */
  HSP_KER_SCA_EXP10_F32,/*!< Exponential base 10 computation */
  HSP_KER_SCA_LN_F32, /*!< LN computation */
  HSP_KER_SCA_LOG10_F32,/*!< LOG10 computation */
  HSP_KER_SCA_FLOGS_F32, /*!< Fast logs (base e, 2, 10, Db)  computation */
  HSP_KER_SCA_FEXPS_F32, /*!< Fast exps (base e, 2, 10, Db)  computation */
  HSP_KER_SCA_SIN_F32, /*!< Sin */
  HSP_KER_SCA_SINCOS_F32, /*!< Cos */
  HSP_KER_SCA_CLARKE_F32, /*!< Clarke */
  HSP_KER_SCA_PARK_F32, /*!< Park */
  HSP_KER_SCA_ICLARKE_F32, /*!< Iclarke */
  HSP_KER_SCA_IPARK_F32, /*!< IPark */
  HSP_KER_SCA_FCOS_F32, /*!< FCos */
  HSP_KER_SCA_FSIN_F32, /*!< FSin */
  HSP_KER_SCA_FATAN2_F32, /*!< FAtan2 */
  /* Matrix related functions */
  HSP_KER_MAT_MUL_F32, /*!< Multiplication of two matrix */
  HSP_KER_MAT_TRANS_F32, /*!< Transposition of a matrix */
  HSP_KER_MAT_INV_F32, /*!< Matrix inversion */
  HSP_KER_M2V, /*!< mat 2 vect kernel copy */
  HSP_KER_V2M, /*!< vect 2 mat kernel copy */
  /* Transformation related funcons */
  HSP_KER_FFT_F32, /*!< Performs the fast Fourier transform of an input vector */
  HSP_KER_RFFT_F32, /*!< Performs the fast Fourier transform of an real input vector */
  HSP_KER_DCT_F32, /*!< Performs the discrete cosine transform of an input vector */
  HSP_KER_IDCT_F32, /*!< Performs the inverse discrete cosine transform of an input vector */
  /* Complex functions */
  HSP_KER_CMPLX_CONJ_F32, /*!< Compute the conjugate of each vector element */
  HSP_KER_CMPLX_DOTPROD_F32, /*!< Computes the dot product of two complex vectors */
  HSP_KER_CMPLX_MAG_F32, /*!< Vector Floating-point complex magnitude */
  HSP_KER_CMPLX_FMAG_F32, /*!< Vector Floating-point complex fast magnitude */
  HSP_KER_CMPLX_MAGSQUARED_F32, /*!< Vector Floating-point complex magnitude squared */
  HSP_KER_CMPLX_MUL_F32, /*!< Compute the complex multiplication of 2 complex vector */
  HSP_KER_CMPLX_RMUL_F32, /*!< Compute the multiplication of a complex vector by a real vector and generates a result */
  HSP_KER_CMPLX_MUL_EXP_F32, /*!< Compute the complex multiplication of one complex vector and exp ROM */
  HSP_KER_CMPLX_CMSIS_DOTPROD_F32, /*!< Computes the dot product of two complex vectors with 2 out real and imaginary */
  /* Specific functions */
  HSP_KER_FLTBANK_F32, /*!< Mel filter */
  /* Event kernel */
  HSP_KER_SEND_EVT, /*!< Send EVT */
  HSP_KER_SET_FLAG, /*!< Set KERF flag */
  HSP_KER_SET_TRGO, /*!< TRGO kernel: generate a trigger pulse on one of the hsp_trg_out */
  /* conditional branch kernel */
  HSP_KER_CMPB_F32, /*!< Compare and send cond EVT to schedule conditional list */
  HSP_KER_CMPCNT_F32, /*!< Compare and send cond EVT to schedule conditional list */
  HSP_KER_SCA2VECT, /*!< Write a scalar value into a vector in order to build a vector */
  HSP_KER_SAT_F32, /*!< Perform a saturation to each vector element, using a programmable value */
  HSP_KER_SET_BITS, /*!< Set the specified bitfield of a scalar to the specified value */
  HSP_KER_SET_GPO, /*!< Set the GPO field bit */
  HSP_KER_WAIT_COND, /*!< Checks the content of a bit field to a reference value and waits until the condition */
  /* Motor control */
  /* State kernels */
  HSP_RESET_STATE, /*!< hyp_reset_state */
  HSP_SET_STATE_CR2L, /*!< hyp_set_state_cr2l */
  HSP_SET_STATE_CL2, /*!< hyp_set_state_cl2r */
  HSP_SET_STATE_L2R, /*!< hyp_set_state_cl2r */
  HSP_GET_STATE_CR2L, /*!< hyp_get_state_cr2l */
  HSP_GET_STATE_CL2, /*!< hyp_get_state_cl2r */
  HSP_GET_STATE_L2R, /*!< hyp_get_state_cl2r */
  HSP_KER_CNT, /*!< Counter kernel */
  /* CRC kernel */
  HSP_CLR_FWERR, /*!< Clear FWERR register */
  /* CNN convolution kernels */
  HSP_CNN_CONV2D_I8, /*!< CNN 2D convolution */
  HSP_CNN_CONVDW_I8, /*!< CNN Depthwise convolution */
  HSP_CNN_CONVPW_I8, /*!< CNN pointwise */
  HSP_CNN_FC_I8, /*!< CNN fully connected */
  HSP_CNN_POOL_I8, /*!< CNN pooling functions */
  HSP_VECT_MB2A, /*!< copy of a vector from MEMB to MEMA */
  /* Integer functions */
  HSP_SCA_MADD_U32, /*!< Integer modulo add */
  /* End of kernels in hyp_ram_ker_list */
  HSP_NB_DRAM_KERNELS, /*!< Number of kernels in DRAM (in ::hyp_ram_ker_list) */

  /* Starting here some enum for kernels which are not in hyp_ram_ker_list table but used in processing list process */

  /* Scaler ASM kernels WARNING, order is used by FW, do not modified it */
  HSP_KER_SCA_ADD_F32, /*!< Summation of 2 scalars */
  HSP_KER_SCA_SUB_F32, /*!< Subtraction of 2 scalars */
  HSP_KER_SCA_MUL_F32, /*!< Product of 2 scalars */
  HSP_KER_SCA_DIV_F32, /*!< Division of 2 scalars */
  HSP_KER_SCA_NEG_F32, /*!< Negate 1 scalar */
  HSP_KER_SCA_MAC_F32, /*!< Mac of 2 scalars */
  HSP_SCA_SAT_F32,
  HSP_SCA_ADD_I32, /*!< Integer scalar addition with constant */
  HSP_SCA_SUB_I32, /*!< Integer scalar addition with constant */
  HSP_SCA_MUL_I32, /*!< Integer scalar multiplication with constant */
  HSP_SCA_SHIFT_I32,
  HSP_SCA_AND_U32,
  HSP_SCA_OR_U32,
  HSP_SCA_XOR_U32,
  HSP_KER_SCA_SET, /*!< Scalar set */
  HSP_KER_SCA_ABS_F32, /*!< Absolute value of an element */
  HSP_KER_SCA_SQRT_F32, /*!< Square-root of an element */
  HSP_KER_SCA_F2I,
  HSP_KER_SCA_I2F,
  HSP_KER_SCA_F2U,
  HSP_KER_SCA_U2F,
  HSP_KER_SCA_24S2F,
  HSP_SCA_INC_U32,
  HSP_SCA_NOT_U32,
  HSP_GET_SATF,
  HSP_CLR_SATF,
  /* Comparison kernels */
  HSP_KER_IFE_CNT, /*!< ife count kernel comparison */
  HSP_KER_IFE_LOOP, /*!< ife loop kernel comparison */
  HSP_KER_IFE, /*!< ife kernel comparison */
  /* loop kernel */
  HSP_KER_LOOP, /*!< loop kernel */
  /* End of all kernels */
  HSP_MAX_NB_KERNELS, /*!< Put in last enum position the define for the number of kernels ID */
};

/* Kernel ID */
#define HSP_KERNEL_ID_MSK 0x1ffU /*!< Kernel ID mask */
#define HSP_KERNEL_INIT_TYPE_MSK 0x00007e00U /*!< Mask for kernel init type (filet, io, fft...) */
#define HSP_KERNEL_INIT_TYPE_SHIFT 9U /*!< Mask for kernel init type (filet, io, fft...) */

/* Command group init specific */
#define HSP_KER_IO_INIT       (0x00U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 1 input and 1 output */
#define HSP_KER_IOS_INIT      (0x01U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 1 input and 1 output + size */
#define HSP_KER_IIO_INIT      (0x02U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 2 inputs and 1 output */
#define HSP_KER_IIOS_INIT     (0x03U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 2 inputs and 1 output + size */
#define HSP_KER_IIOSS_INIT    (0x04U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 2 inputs and 1 output + sizeA + sizeB */
#define HSP_KER_IIIOSS_INIT   (0x05U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 3 inputs and 1 output + 2 params */
#define HSP_KER_FLT_INIT      (0x06U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for filtering */
#define HSP_KER_FFT_INIT      (0x07U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for fft */
#define HSP_MAT_IIO_INIT      (0x08U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for matrix */
#define HSP_KER_VAL_INIT      (0x09U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernels for one value */
#define HSP_KER_IFE_INIT      (0x0aU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Comparison (if, else, endif) kernel init */
#define HSP_KER_LOOP_INIT     (0x0bU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for loop */
#define HSP_KER_REG_INIT      (0x0cU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernels for register bit manipulation */
#define HSP_KER_PLG_INIT      (0x0dU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Plugin kernel init */
#define HSP_KER_SASM2_INIT    (0x0eU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for asm inlining scalar */
#define HSP_KER_SASM3_INIT    (0x0fU << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for asm inlining scalar */
#define HSP_KER_SASM_INIT     (0x10U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel for asm inlining scalar */
#define HSP_KER_IOSSSSS_INIT  (0x11U << HSP_KERNEL_INIT_TYPE_SHIFT) /*!< Kernel with 6 inputs and 1 output  */
#define HSP_MAX_NB_INIT 0x11U

/* Specify some kernel size at 0x0000f000 of kernelID */
#define HSP_KER_STIA_MSK 0x00000000U /*!< Kernel STI encoded mask for all allowed external buffer */
#define HSP_KER_STI0_MSK 0x00010000U /*!< Kernel STI encoded mask for forbidden external buffer */
#define HSP_KER_STI1_MSK 0x00020000U /*!< Kernel STI encoded mask for forbidden external buffer */
#define HSP_KER_STI2_MSK 0x00040000U /*!< Kernel STI encoded mask for forbidden external buffer */
#define HSP_KER_STIN_MSK 0x00070000U /*!< Kernel STI encoded mask for forbidden external buffer */
#define HSP_KER_STI_SHIFT 16U /*!< Kernel STI encoded shift for allowed external buffer */

/* Plugin init index 0xf80000 */
#define HSP_KER_PLG           0x8000U /*!< Plugin kernel */
#define HSP_KER_PLG_IDX_MSK   0x3f000000U /*!< Mask for plugin init index */
#define HSP_KER_PLG_IDX_SHIFT 24U /*!< Shift for plugin init index */

/* Specify preload info */
#define HSP_KER_PRE_MSK   0x00080000U /*!< KernelID mask for preload possibility of first input when external buffer */
#define HSP_KER_PRE_SHIFT 19U /*!< KernelID shift to get preload field */


/* Specify immediate value */
#define HSP_KER_IMM_MASK 0x00100000U /*!< KernelID mask for immediate value */
#define HSP_KER_IMM_SHIFT 20U /*!< KernelID shift for immediate value */

/**
  * @}
  */

/** @defgroup HSP_PLCommand_Constants HSP Processing list Command Constants
  * @{
  */

/* FW command */
#define HSP_CMD_FW_START            0x84000000U /*!< After boot, FW is waiting for ::HSP_CMD_FW_START */

/* Processing list configuration command for supervisor mode */
#define HSP_CMD_PRO_CFG_START 0x81000000U /*!< Start create PL: supervisor mode blocking no wfe until end PL creation */
#define HSP_CMD_PRO_CFG_END   0x82000000U /*!< Close the started processing list creation and go back to wfe */
#define HSP_CMD_PL_RESET      0x83000000U /*!< In supervisor mode: Reset all processing lists and go back to wfe */

/* Filter related functions */
/** FIR filter (all other cases) */
#define HSP_CMD_FIR_F32         (HSP_KER_FIR_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** FIR filter, with decimation (more efficient function for (nb taps < nb samples) and
    ((nb samples - nb taps) > (2*decimator))) */
#define HSP_CMD_FIRDEC_F32      (HSP_KER_FIRDEC_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** FIR filter, with adaptive LMS algorithm. */
#define HSP_CMD_LMS_F32         (HSP_KER_LMS_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** IIR biquad cascade DF1 filter */
#define HSP_CMD_BQ_CAS_DF1_F32  (HSP_KER_BQ_CAS_DF1_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** IIR biquad cascade DF2T filter */
#define HSP_CMD_BQ_CAS_DF2T_F32 (HSP_KER_BQ_CAS_DF2T_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** IIR DF1 filter */
#define HSP_CMD_IIR_DF1_F32     (HSP_KER_IIR_DF1_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** IIR 3p3z for 1 sample filter */
#define HSP_CMD_IIR_3P3Z_1S_F32 (HSP_KER_IIR_3P3Z_1S_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** IIR 2p2z for 1 sample filter */
#define HSP_CMD_IIR_2P2Z_1S_F32 (HSP_KER_IIR_2P2Z_1S_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** IIR Lattice filter */
#define HSP_CMD_IIR_LATTICE_F32 (HSP_KER_IIR_LATTICE_F32 | HSP_KER_FLT_INIT | HSP_KER_STIA_MSK)
/** Convolution */
#define HSP_CMD_CONV_F32        (HSP_KER_CONV_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)
/** Correlation */
#define HSP_CMD_CORR_F32        (HSP_KER_CORR_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)
/** Window symmetric kernel */
#define HSP_CMD_WINSYM_F32      (HSP_KER_WINSYM_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)
/** FLTBANK filter */
#define HSP_CMD_FLTBANK_F32     (HSP_KER_FLTBANK_F32 | HSP_KER_FLT_INIT | HSP_KER_STIN_MSK)


/* Vector related functions */
/** Element-wise summation of a vector */
#define HSP_CMD_VEC_ADD_F32        (HSP_KER_VEC_ADD_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise subtraction of a vector */
#define HSP_CMD_VEC_SUB_F32        (HSP_KER_VEC_SUB_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise multiplication of a vector */
#define HSP_CMD_VEC_MUL_F32        (HSP_KER_VEC_MUL_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise division of a vector */
#define HSP_CMD_VEC_DIV_F32        (HSP_KER_VEC_DIV_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise square-root of a vector */
#define HSP_CMD_VEC_SQRT_F32       (HSP_KER_VEC_SQRT_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise absolute value of a vector */
#define HSP_CMD_VEC_ABS_F32        (HSP_KER_VEC_ABS_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Compute the average value of a vector */
#define HSP_CMD_VEC_AVG_F32        (HSP_KER_VEC_AVG_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Compute the RMS value of a vector */
#define HSP_CMD_VEC_RMS_F32        (HSP_KER_VEC_RMS_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Multiply each vector element by a scalar */
#define HSP_CMD_VEC_SCALE_F32      (HSP_KER_VEC_SCALE_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Add to each vector element a scalar */
#define HSP_CMD_VEC_OFFSET_F32     (HSP_KER_VEC_OFFSET_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Search for value and position of the absolute biggest element of a vector */
#define HSP_CMD_VEC_ABSMAX_F32     (HSP_KER_VEC_ABSMAX_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Search for value and position of the biggest element of a vector */
#define HSP_CMD_VEC_MAX_F32        (HSP_KER_VEC_MAX_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Search for value and position of the smallest element of a vector */
#define HSP_CMD_VEC_MIN_F32        (HSP_KER_VEC_MIN_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Performs the copy of a vector (can be used for debug) */
#define HSP_CMD_VEC_COPY       (HSP_KER_VEC_COPY | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Add kernel function pointer and parameters according to user command */
#define HSP_CMD_VEC_SET        (HSP_KER_VEC_SET | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK)
/** Calculates the dot prod of the input vectors */
#define HSP_CMD_VEC_DOTPROD_F32    (HSP_KER_VEC_DOTPROD_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise cosine value of a vector */
#define HSP_CMD_VEC_COS_F32        (HSP_KER_VEC_COS_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise sine value of a vector */
#define HSP_CMD_VEC_SIN_F32        (HSP_KER_VEC_SIN_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise sine and cosine value of a vector */
#define HSP_CMD_VEC_SINCOS_F32     (HSP_KER_VEC_SINCOS_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise float32 to integer 32bits conversion */
#define HSP_CMD_VEC_F2I        (HSP_KER_VEC_F2I | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise integer 32bits to float32 conversion */
#define HSP_CMD_VEC_I2F        (HSP_KER_VEC_I2F | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise integer 24bits signed to float32 conversion */
#define HSP_CMD_VEC_24S2F      (HSP_KER_VEC_24S2F | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise float32 to unsigned integer 32bits conversion */
#define HSP_CMD_VEC_F2U        (HSP_KER_VEC_F2U | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise unsigned integer 32bits to float32 conversion */
#define HSP_CMD_VEC_U2F        (HSP_KER_VEC_U2F | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Vector decimation */
#define HSP_CMD_VEC_DECIM      (HSP_KER_VEC_DECIM | HSP_KER_IIOS_INIT | HSP_KER_STI1_MSK)
/** Vector zero insertion */
#define HSP_CMD_VEC_ZINS       (HSP_KER_VEC_ZINS | HSP_KER_IIOS_INIT | HSP_KER_STI1_MSK | HSP_KER_PRE_MSK)
/** Element-wise Atan2 computation of a vector */
#define HSP_CMD_VEC_ATAN2_F32      (HSP_KER_VEC_ATAN2_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise Logarithm neperien computation of a vector */
#define HSP_CMD_VEC_LN_F32         (HSP_KER_VEC_LN_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise Logarithm base 10 computation of a vector */
#define HSP_CMD_VEC_LOG10_F32      (HSP_KER_VEC_LOG10_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise Exponential computation of a vector */
#define HSP_CMD_VEC_EXP_F32        (HSP_KER_VEC_EXP_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise Exponential base 10 computation of a vector */
#define HSP_CMD_VEC_EXP10_F32      (HSP_KER_VEC_EXP10_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
#define HSP_CMD_VEC_FLOGS_F32       (HSP_KER_VEC_FLOGS_F32 | HSP_KER_IOSSSSS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
#define HSP_CMD_VEC_FEXPS_F32       (HSP_KER_VEC_FEXPS_F32 | HSP_KER_IOSSSSS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Compute the multiplication of one vector and cos ROM */
#define HSP_CMD_VEC_MUL_COS_F32    (HSP_KER_VEC_MUL_COS_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Compute the multiplication of one vector and sin ROM */
#define HSP_CMD_VEC_MUL_SIN_F32    (HSP_KER_VEC_MUL_SIN_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Element-wise float32 to Q31 conversion of a vector */
#define HSP_CMD_VEC_F2Q31      (HSP_KER_VEC_F2Q31 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise Q31 to float32 conversion of a vector */
#define HSP_CMD_VEC_Q312F      (HSP_KER_VEC_Q312F | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise float32 to Q15 conversion of a vector */
#define HSP_CMD_VEC_F2Q15      (HSP_KER_VEC_F2Q15 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Element-wise Q15 to float32 conversion of a vector */
#define HSP_CMD_VEC_Q152F      (HSP_KER_VEC_Q152F | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Perform a saturation to each vector element, using a programmable value */
#define HSP_CMD_VEC_SAT_F32    (HSP_KER_SAT_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)

/* Scalar related functions */
/** Absolute value of an element */
#define HSP_CMD_SCA_ABS_F32      (HSP_KER_SCA_ABS_F32 | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar addition */
#define HSP_CMD_SCA_ADD_F32      (HSP_KER_SCA_ADD_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Scalar atan2 */
#define HSP_CMD_SCA_ATAN2_F32    (HSP_KER_SCA_ATAN2_F32 | HSP_KER_IIO_INIT | HSP_KER_STIA_MSK)
/** Scalar cosinus */
#define HSP_CMD_SCA_COS_F32      (HSP_KER_SCA_COS_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar division */
#define HSP_CMD_SCA_DIV_F32      (HSP_KER_SCA_DIV_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Scalar negate */
#define HSP_CMD_SCA_NEG_F32      (HSP_KER_SCA_NEG_F32 | HSP_KER_SASM3_INIT | HSP_KER_STI0_MSK)
/** Scalar Mac */
#define HSP_CMD_SCA_MAC_F32      (HSP_KER_SCA_MAC_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Scalar saturation */
#define HSP_CMD_SCA_SAT_F32      (HSP_SCA_SAT_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Scalar float32 to integer 32bits conversion */
#define HSP_CMD_SCA_F2I      (HSP_KER_SCA_F2I | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar integer 32bits to float32 conversion */
#define HSP_CMD_SCA_I2F      (HSP_KER_SCA_I2F | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar integer 24bits signed to float32 conversion */
#define HSP_CMD_SCA_24S2F    (HSP_KER_SCA_24S2F | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar float32 to unsigned integer 32bits conversion */
#define HSP_CMD_SCA_F2U      (HSP_KER_SCA_F2U | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar unsigned integer 32bits to float32 conversion */
#define HSP_CMD_SCA_U2F      (HSP_KER_SCA_U2F | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar float32 to Q31 conversion */
#define HSP_CMD_SCA_F2Q31    (HSP_KER_SCA_F2Q31 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar Q31 to float32 conversion */
#define HSP_CMD_SCA_Q312F    (HSP_KER_SCA_Q312F | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar exponential neperien */
#define HSP_CMD_SCA_EXP_F32      (HSP_KER_SCA_EXP_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar exponential base 10 */
#define HSP_CMD_SCA_EXP10_F32    (HSP_KER_SCA_EXP10_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar logarithm neperien */
#define HSP_CMD_SCA_LN_F32       (HSP_KER_SCA_LN_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar logarithm base 10 */
#define HSP_CMD_SCA_LOG10_F32    (HSP_KER_SCA_LOG10_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar exponential */
#define HSP_CMD_SCA_FEXPS_F32     (HSP_KER_SCA_FEXPS_F32 | HSP_KER_IOSSSSS_INIT | HSP_KER_STI2_MSK)
/** Scalar logarithm */
#define HSP_CMD_SCA_FLOGS_F32     (HSP_KER_SCA_FLOGS_F32 | HSP_KER_IOSSSSS_INIT | HSP_KER_STI2_MSK)
/** Scalar multiplication */
#define HSP_CMD_SCA_MUL_F32      (HSP_KER_SCA_MUL_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Scalar Sinus */
#define HSP_CMD_SCA_SIN_F32      (HSP_KER_SCA_SIN_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar Sinus and Cosinus */
#define HSP_CMD_SCA_SINCOS_F32   (HSP_KER_SCA_SINCOS_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar set */
#define HSP_CMD_SCA_SET      (HSP_KER_SCA_SET | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Square-root of an element */
#define HSP_CMD_SCA_SQRT_F32     (HSP_KER_SCA_SQRT_F32 | HSP_KER_SASM2_INIT | HSP_KER_STI2_MSK)
/** Scalar Clarke transform (#hyp_sca_clarke) */
#define HSP_CMD_SCA_CLARKE_F32   (HSP_KER_SCA_CLARKE_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar Park transform (#hyp_sca_park) */
#define HSP_CMD_SCA_PARK_F32     (HSP_KER_SCA_PARK_F32 | HSP_KER_IIO_INIT | HSP_KER_STIA_MSK)
/** Scalar Iclarke transform (#hyp_sca_iclarke) */
#define HSP_CMD_SCA_ICLARKE_F32  (HSP_KER_SCA_ICLARKE_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar IPark transform (#hyp_sca_ipark) */
#define HSP_CMD_SCA_IPARK_F32    (HSP_KER_SCA_IPARK_F32 | HSP_KER_IIO_INIT | HSP_KER_STIA_MSK)
/** Scalar fast Cosinus */
#define HSP_CMD_SCA_FCOS_F32     (HSP_KER_SCA_FCOS_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar fast Sinus */
#define HSP_CMD_SCA_FSIN_F32     (HSP_KER_SCA_FSIN_F32 | HSP_KER_IO_INIT | HSP_KER_STI2_MSK)
/** Scalar fast FAtan2 */
#define HSP_CMD_SCA_FATAN2_F32   (HSP_KER_SCA_FATAN2_F32 | HSP_KER_IIO_INIT | HSP_KER_STIA_MSK)
/** Scalar Subtraction */
#define HSP_CMD_SCA_SUB_F32      (HSP_KER_SCA_SUB_F32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)

/* Matrix related functions */
/** Multiplication of two matrix */
#define HSP_CMD_MAT_MUL_F32     (HSP_KER_MAT_MUL_F32 | HSP_MAT_IIO_INIT | HSP_KER_STIA_MSK)
/** Transposition of a matrix */
#define HSP_CMD_MAT_TRANS_F32   (HSP_KER_MAT_TRANS_F32 | HSP_MAT_IIO_INIT | HSP_KER_STIA_MSK)
/** Matrix inversion */
#define HSP_CMD_MAT_INV_F32     (HSP_KER_MAT_INV_F32 | HSP_MAT_IIO_INIT | HSP_KER_STIA_MSK)
/** Matrix to column vector kernel copy */
#define HSP_CMD_M2V         (HSP_KER_M2V | HSP_KER_IIIOSS_INIT | HSP_KER_STIA_MSK)
/** Vector to column matrix kernel copy */
#define HSP_CMD_V2M         (HSP_KER_V2M | HSP_KER_IIIOSS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)

/* Transformation related funcons */
/** Performs the complex fast Fourier transform of an input vector */
#define HSP_CMD_FFT_F32   (HSP_KER_FFT_F32 | HSP_KER_FFT_INIT | HSP_KER_STIN_MSK)
/** Performs the real fast Fourier transform of an input vector */
#define HSP_CMD_RFFT_F32  (HSP_KER_RFFT_F32 | HSP_KER_FFT_INIT | HSP_KER_STIN_MSK)
/** Performs the discrete cosine transform of an input vector */
#define HSP_CMD_DCT_F32   (HSP_KER_DCT_F32 | HSP_KER_FFT_INIT | HSP_KER_STIN_MSK)
/** Performs the inverse discrete cosine transform of an input vector */
#define HSP_CMD_IDCT_F32  (HSP_KER_IDCT_F32 | HSP_KER_FFT_INIT | HSP_KER_STIN_MSK)

/* Complex functions */
/** Compute the conjugate of each vector element */
#define HSP_CMD_CMPLX_CONJ_F32       (HSP_KER_CMPLX_CONJ_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Computes the dot product of two complex vectors */
#define HSP_CMD_CMPLX_DOTPROD_F32    (HSP_KER_CMPLX_DOTPROD_F32 |\
                                      HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Vector Floating-point complex magnitude */
#define HSP_CMD_CMPLX_MAG_F32        (HSP_KER_CMPLX_MAG_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Vector Floating-point complex fast magnitude */
#define HSP_CMD_CMPLX_FMAG_F32       (HSP_KER_CMPLX_FMAG_F32 | HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Vector Floating-point complex magnitude squared */
#define HSP_CMD_CMPLX_MAGSQUARED_F32 (HSP_KER_CMPLX_MAGSQUARED_F32 |\
                                      HSP_KER_IOS_INIT | HSP_KER_STI2_MSK | HSP_KER_PRE_MSK)
/** Compute the complex multiplication of 2 complex vector */
#define HSP_CMD_CMPLX_MUL_F32        (HSP_KER_CMPLX_MUL_F32 | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)
/** Compute the multiplication of a complex vector by a real vector and generates a complex result */
#define HSP_CMD_CMPLX_RMUL_F32       (HSP_KER_CMPLX_RMUL_F32 | HSP_KER_IIOS_INIT |  HSP_KER_STIA_MSK |  HSP_KER_PRE_MSK)
/** Compute the complex multiplication of one complex vector and exp ROM */
#define HSP_CMD_CMPLX_MUL_EXP_F32    (HSP_KER_CMPLX_MUL_EXP_F32 |\
                                      HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK | HSP_KER_PRE_MSK)

/* Counter function */
/** Global counter kernel (use DRAM counter in order to use HSP_CMD_IFE_CNT) */
#define HSP_CMD_COUNT (HSP_KER_CNT | HSP_KER_IO_INIT)


/* Integer functions */
/** Integer scalar addition with constant */
#define HSP_CMD_SCA_ADD_I32    (HSP_SCA_ADD_I32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar Subtraction with constant */
#define HSP_CMD_SCA_SUB_I32    (HSP_SCA_SUB_I32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar multiplication with constant */
#define HSP_CMD_SCA_MUL_I32    (HSP_SCA_MUL_I32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar INC */
#define HSP_CMD_SCA_INC_U32  (HSP_SCA_INC_U32 | HSP_KER_SASM2_INIT | HSP_KER_STIA_MSK)
/** Integer scalar NOT */
#define HSP_CMD_SCA_NOT_U32  (HSP_SCA_NOT_U32 | HSP_KER_SASM2_INIT | HSP_KER_STIA_MSK)
/** Integer scalar shift with constant */
#define HSP_CMD_SCA_SHIFT_I32  (HSP_SCA_SHIFT_I32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar AND with constant */
#define HSP_CMD_SCA_AND_U32  (HSP_SCA_AND_U32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar OR with constant */
#define HSP_CMD_SCA_OR_U32  (HSP_SCA_OR_U32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar XOR with constant */
#define HSP_CMD_SCA_XOR_U32  (HSP_SCA_XOR_U32 | HSP_KER_SASM3_INIT | HSP_KER_STIA_MSK)
/** Integer scalar modulo addition with constant */
#define HSP_CMD_SCA_MADD_U32   (HSP_SCA_MADD_U32 | HSP_KER_IIOS_INIT | HSP_KER_STI1_MSK)
/** Get saturation flag value */
#define HSP_CMD_GET_SATF  (HSP_GET_SATF | HSP_KER_SASM2_INIT | HSP_KER_STIA_MSK)
/** Clear saturation flag */
#define HSP_CMD_CLR_SATF  (HSP_CLR_SATF | HSP_KER_SASM_INIT | HSP_KER_STIA_MSK)


/* Comparison functions */
/** if kernel comparison */
#define HSP_CMD_IFE      (HSP_KER_IFE | HSP_KER_IFE_INIT | HSP_KER_STIN_MSK)
/** if kernel comparison */
#define HSP_CMD_IFE_LOOP (HSP_KER_IFE_LOOP | HSP_KER_IFE_INIT | HSP_KER_STIN_MSK)
/** if kernel comparison for global counters */
#define HSP_CMD_IFE_CNT  (HSP_KER_IFE_CNT | HSP_KER_IFE_INIT | HSP_KER_STIN_MSK)

/* Loop function */
/** loop kernel comparison */
#define HSP_CMD_LOOP (HSP_KER_LOOP | HSP_KER_LOOP_INIT | HSP_KER_STIN_MSK)

/* Kernels with conditional event and flag generation */
/** Comparison of an input vector to two thresholds (LOTH, HITH) with branch option */
#define HSP_CMD_CMPB_F32      (HSP_KER_CMPB_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)
/** Comparison of an input vector to two thresholds (LOTH, HITH) with branch option */
#define HSP_CMD_CMPCNT_F32    (HSP_KER_CMPCNT_F32 | HSP_KER_IIOSS_INIT | HSP_KER_STIA_MSK)
/** Write a scalar value into a vector in order to build a vector */
#define HSP_CMD_SCA2VECT  (HSP_KER_SCA2VECT | HSP_KER_IIOS_INIT | HSP_KER_STIA_MSK)
/** Set the specified bitfield of a scalar to the specified value */
#define HSP_CMD_SET_BITS  (HSP_KER_SET_BITS | HSP_KER_IOS_INIT | HSP_KER_STI1_MSK)
/** Set the GPO bitfield */
#define HSP_CMD_SET_GPO   (HSP_KER_SET_GPO | HSP_KER_IOS_INIT | HSP_KER_STIN_MSK)
/** Checks the content of a bit field to a reference value and waits until the condition */
#define HSP_CMD_WAIT_COND (HSP_KER_WAIT_COND | HSP_KER_REG_INIT | HSP_KER_STI1_MSK)


/* Memory transfer related functions */
/** Send event */
#define HSP_CMD_SEND_EVT (HSP_KER_SEND_EVT | HSP_KER_VAL_INIT)
/** Set KERF flag */
#define HSP_CMD_SET_FLAG (HSP_KER_SET_FLAG | HSP_KER_VAL_INIT)
/** TRGO kernel: generate a trigger pulse on one of the hsp_trg_out */
#define HSP_CMD_SET_TRGO (HSP_KER_SET_TRGO | HSP_KER_VAL_INIT)

/**
  * @}
  */

/** @defgroup HSP_DCommand_Constants HSP Direct Command Constants
  * @{
  */

/* Filter related functions */
/** FIR filter */
#define HSP_DIRECT_CMD_FIR_F32                 ((HSP_DC_START + ((HSP_KER_FIR_F32) * 2U)) * 4U)
/** FIR filter */
#define HSP_DIRECT_CMD_FIR_LESS7TAPS_F32       ((HSP_DC_START + ((HSP_KER_FIR_LESS7TAPS_F32) * 2U)) * 4U)
/** FIR filter */
#define HSP_DIRECT_CMD_FIR_MODULO4TAPS_F32     ((HSP_DC_START + ((HSP_KER_FIR_MODULO4TAPS_F32) * 2U)) * 4U)
/** FIR filter */
#define HSP_DIRECT_CMD_FIR_1SAMPLE_F32         ((HSP_DC_START + ((HSP_KER_FIR_1SAMPLE_F32) * 2U)) * 4U)
/** FIR filter, with decimation */
#define HSP_DIRECT_CMD_FIRDEC_F32              ((HSP_DC_START + ((HSP_KER_FIRDEC_F32) * 2U)) * 4U)
/** FIR filter, with decimation */
#define HSP_DIRECT_CMD_FIRDEC_1TAP_F32         ((HSP_DC_START + ((HSP_KER_FIRDEC_1TAP_F32) * 2U)) * 4U)
/** FIR filter, with decimation */
#define HSP_DIRECT_CMD_FIRDEC_SPEC_F32         ((HSP_DC_START + ((HSP_KER_FIRDEC_SPEC_F32) * 2U)) * 4U)
/** FIR filter, with adaptive LMS algorithm. */
#define HSP_DIRECT_CMD_LMS_F32                 ((HSP_DC_START + ((HSP_KER_LMS_F32) * 2U)) * 4U)
/** IIR biquad cascade DF1 filter */
#define HSP_DIRECT_CMD_BIQUAD_CASCADE_DF1_F32  ((HSP_DC_START + ((HSP_KER_BQ_CAS_DF1_F32) * 2U)) * 4U)
/** IIR biquad cascade DF2T filter */
#define HSP_DIRECT_CMD_BIQUAD_CASCADE_DF2T_F32 ((HSP_DC_START + ((HSP_KER_BQ_CAS_DF2T_F32) * 2U)) * 4U)
/** IIR DF1 filter */
#define HSP_DIRECT_CMD_IIR_DF1_F32             ((HSP_DC_START + ((HSP_KER_IIR_DF1_F32) * 2U)) * 4U)
/** Only define to generate FW error. IIR 3p3z for 1 sample filter */
#define HSP_DIRECT_CMD_IIR_3P3Z_1S_F32         ((HSP_DC_START + ((HSP_KER_IIR_3P3Z_1S_F32) * 2U)) * 4U)
/** Only define to generate FW error. IIR 2p2z for 1 sample filter */
#define HSP_DIRECT_CMD_IIR_2P2Z_1S_F32         ((HSP_DC_START + ((HSP_KER_IIR_2P2Z_1S_F32) * 2U)) * 4U)
/** IIR Lattice */
#define HSP_DIRECT_CMD_IIR_LATTICE_F32         ((HSP_DC_START + ((HSP_KER_IIR_LATTICE_F32) * 2U)) * 4U)
/** Convolution */
#define HSP_DIRECT_CMD_CONV_F32                ((HSP_DC_START + ((HSP_KER_CONV_F32) * 2U)) * 4U)
/** Correlation */
#define HSP_DIRECT_CMD_CORR_F32                ((HSP_DC_START + ((HSP_KER_CORR_F32) * 2U)) * 4U)
/** Window symmetric kernel */
#define HSP_DIRECT_CMD_WINSYM_F32              ((HSP_DC_START + ((HSP_KER_WINSYM_F32) * 2U)) * 4U)
/** FLTBANK filter */
#define HSP_DIRECT_CMD_FLTBANK_F32             ((HSP_DC_START + ((HSP_KER_FLTBANK_F32) * 2U)) * 4U)
/** FLTBANK filter with external coeffs */
#define HSP_DIRECT_CMD_FLTBANK_EXTC_F32        ((HSP_DC_START + ((HSP_KER_FLTBANK_EXTC_F32) * 2U)) * 4U)

/* State buffer functions */
/** Reset filter state */
#define HSP_DIRECT_CMD_RESET_STATE       ((HSP_DC_START + ((HSP_RESET_STATE) * 2U)) * 4U)
/** Set circular filter state from right to left */
#define HSP_DIRECT_CMD_SET_STATE_CR2L    ((HSP_DC_START + ((HSP_SET_STATE_CR2L) * 2U)) * 4U)
/** Set circular filter state from left to right */
#define HSP_DIRECT_CMD_SET_STATE_CL2R    ((HSP_DC_START + ((HSP_SET_STATE_CL2) * 2U)) * 4U)
/** Set linear filter state from left to right */
#define HSP_DIRECT_CMD_SET_STATE_L2R     ((HSP_DC_START + ((HSP_SET_STATE_L2R) * 2U)) * 4U)
/** Get circular filter state from right to left */
#define HSP_DIRECT_CMD_GET_STATE_CR2L    ((HSP_DC_START + ((HSP_GET_STATE_CR2L) * 2U)) * 4U)
/** Get circular filter state from left to right */
#define HSP_DIRECT_CMD_GET_STATE_CL2R    ((HSP_DC_START + ((HSP_GET_STATE_CL2)* 2U)) * 4U)
/** Get linear filter state from left to right */
#define HSP_DIRECT_CMD_GET_STATE_L2R     ((HSP_DC_START + ((HSP_GET_STATE_L2R) * 2U)) * 4U)

/* Vector related functions */
/** Element-wise summation of a vector */
#define HSP_DIRECT_CMD_VEC_ADD_F32        ((HSP_DC_START + ((HSP_KER_VEC_ADD_F32) * 2U)) * 4U)
/** Element-wise subtraction of a vector */
#define HSP_DIRECT_CMD_VEC_SUB_F32        ((HSP_DC_START + ((HSP_KER_VEC_SUB_F32) * 2U)) * 4U)
/** Element-wise multiplication of a vector */
#define HSP_DIRECT_CMD_VEC_MUL_F32        ((HSP_DC_START + ((HSP_KER_VEC_MUL_F32) * 2U)) * 4U)
/** Element-wise division of a vector */
#define HSP_DIRECT_CMD_VEC_DIV_F32        ((HSP_DC_START + ((HSP_KER_VEC_DIV_F32) * 2U)) * 4U)
/** Calculates the sum of the */
#define HSP_DIRECT_CMD_VEC_SQRT_F32       ((HSP_DC_START + ((HSP_KER_VEC_SQRT_F32) * 2U)) * 4U)
/** Element-wise absolute value of a vector */
#define HSP_DIRECT_CMD_VEC_ABS_F32        ((HSP_DC_START + ((HSP_KER_VEC_ABS_F32) * 2U)) * 4U)
/** Compute the average value of a vector */
#define HSP_DIRECT_CMD_VEC_AVG_F32        ((HSP_DC_START + ((HSP_KER_VEC_AVG_F32) * 2U)) * 4U)
/** Compute the RMS value of a vector */
#define HSP_DIRECT_CMD_VEC_RMS_F32        ((HSP_DC_START + ((HSP_KER_VEC_RMS_F32) * 2U)) * 4U)
/** Multiply each vector element by a scalar */
#define HSP_DIRECT_CMD_VEC_SCALE_F32      ((HSP_DC_START + ((HSP_KER_VEC_SCALE_F32) * 2U)) * 4U)
/** Add to each vector element a scalar */
#define HSP_DIRECT_CMD_VEC_OFFSET_F32     ((HSP_DC_START + ((HSP_KER_VEC_OFFSET_F32) * 2U)) * 4U)
/** Search for value and position of the biggest element of a vector */
#define HSP_DIRECT_CMD_VEC_ABSMAX_F32     ((HSP_DC_START + ((HSP_KER_VEC_ABSMAX_F32) * 2U)) * 4U)
/** Search for value and position of the biggest element of a vector */
#define HSP_DIRECT_CMD_VEC_MAX_F32        ((HSP_DC_START + ((HSP_KER_VEC_MAX_F32) * 2U)) * 4U)
/** Search for value and position of the smallest element of a vector */
#define HSP_DIRECT_CMD_VEC_MIN_F32        ((HSP_DC_START + ((HSP_KER_VEC_MIN_F32) * 2U)) * 4U)
/** Performs the copy of a vector (can be used for debug) */
#define HSP_DIRECT_CMD_VEC_COPY       ((HSP_DC_START + ((HSP_KER_VEC_COPY) * 2U)) * 4U)
/** Add kernel function pointer and parameters according to user command */
#define HSP_DIRECT_CMD_VEC_SET        ((HSP_DC_START + ((HSP_KER_VEC_SET) * 2U)) * 4U)
/** Calculates the dot prod of the input vectors */
#define HSP_DIRECT_CMD_VEC_DOTPROD_F32    ((HSP_DC_START + ((HSP_KER_VEC_DOTPROD_F32) * 2U)) * 4U)
/** Element-wise cosine value of a vector */
#define HSP_DIRECT_CMD_VEC_COS_F32        ((HSP_DC_START + ((HSP_KER_VEC_COS_F32) * 2U)) * 4U)
/** Element-wise sine value of a vector */
#define HSP_DIRECT_CMD_VEC_SIN_F32        ((HSP_DC_START + ((HSP_KER_VEC_SIN_F32) * 2U)) * 4U)
/** Element-wise sine cosine value of a vector */
#define HSP_DIRECT_CMD_VEC_SINCOS_F32     ((HSP_DC_START + ((HSP_KER_VEC_SINCOS_F32) * 2U)) * 4U)
/** Element-wise float32 to integer 32bits conversion */
#define HSP_DIRECT_CMD_VEC_F2I        ((HSP_DC_START + ((HSP_KER_VEC_F2I) * 2U)) * 4U)
/** Element-wise integer 32bits to float32 conversion */
#define HSP_DIRECT_CMD_VEC_I2F        ((HSP_DC_START + ((HSP_KER_VEC_I2F) * 2U)) * 4U)
/** Element-wise integer 24bits signed to float32 conversion */
#define HSP_DIRECT_CMD_VEC_24S2F      ((HSP_DC_START + ((HSP_KER_VEC_24S2F) * 2U)) * 4U)
/** Element-wise float32 to unsigned integer 32bits conversion */
#define HSP_DIRECT_CMD_VEC_F2U        ((HSP_DC_START + ((HSP_KER_VEC_F2U) * 2U)) * 4U)
/** Element-wise unsigned integer 32bits to float32 conversion */
#define HSP_DIRECT_CMD_VEC_U2F        ((HSP_DC_START + ((HSP_KER_VEC_U2F) * 2U)) * 4U)
/** Decimation */
#define HSP_DIRECT_CMD_VEC_DECIM      ((HSP_DC_START + ((HSP_KER_VEC_DECIM) * 2U)) * 4U)
/** Zero insertion */
#define HSP_DIRECT_CMD_VEC_ZINS       ((HSP_DC_START + ((HSP_KER_VEC_ZINS) * 2U)) * 4U)
/** Atan2 computation */
#define HSP_DIRECT_CMD_VEC_ATAN2_F32      ((HSP_DC_START + ((HSP_KER_VEC_ATAN2_F32) * 2U)) * 4U)
/** Logarithm neperien computation */
#define HSP_DIRECT_CMD_VEC_LN_F32         ((HSP_DC_START + ((HSP_KER_VEC_LN_F32) * 2U)) * 4U)
/** Logarithm base 10 computation */
#define HSP_DIRECT_CMD_VEC_LOG10_F32      ((HSP_DC_START + ((HSP_KER_VEC_LOG10_F32) * 2U)) * 4U)
/** Exponential computation */
#define HSP_DIRECT_CMD_VEC_EXP_F32        ((HSP_DC_START + ((HSP_KER_VEC_EXP_F32) * 2U)) * 4U)
/** Exponential base 10 computation */
#define HSP_DIRECT_CMD_VEC_EXP10_F32      ((HSP_DC_START + ((HSP_KER_VEC_EXP10_F32) * 2U)) * 4U)
/** Compute the multiplication of one vector and cos ROM */
#define HSP_DIRECT_CMD_VEC_MUL_COS_F32    ((HSP_DC_START + ((HSP_KER_VEC_MUL_COS_F32) * 2U)) * 4U)
/** Compute the multiplication of one vector and sin ROM */
#define HSP_DIRECT_CMD_VEC_MUL_SIN_F32    ((HSP_DC_START + ((HSP_KER_VEC_MUL_SIN_F32) * 2U)) * 4U)
/** Multiply each vector element by a scalar */
#define HSP_DIRECT_CMD_VEC_SCALE_I_F32    ((HSP_DC_START + ((HSP_KER_VEC_SCALE_I_F32) * 2U)) * 4U)
/** Add to each vector element a scalar */
#define HSP_DIRECT_CMD_VEC_OFFSET_I_F32   ((HSP_DC_START + ((HSP_KER_VEC_OFFSET_I_F32) * 2U)) * 4U)
/** Set each vector element with an immediate scalar */
#define HSP_DIRECT_CMD_VEC_SET_I      ((HSP_DC_START + ((HSP_KER_VEC_SET_I) * 2U)) * 4U)
/** Element-wise float32 to Q31 conversion */
#define HSP_DIRECT_CMD_VEC_F2Q31      ((HSP_DC_START + ((HSP_KER_VEC_F2Q31) * 2U)) * 4U)
/** Element-wise Q31 to float32 conversion */
#define HSP_DIRECT_CMD_VEC_Q312F      ((HSP_DC_START + ((HSP_KER_VEC_Q312F) * 2U)) * 4U)
/** Element-wise float32 to Q15 conversion */
#define HSP_DIRECT_CMD_VEC_F2Q15      ((HSP_DC_START + ((HSP_KER_VEC_F2Q15) * 2U)) * 4U)
/** Element-wise Q15 to float32 conversion */
#define HSP_DIRECT_CMD_VEC_Q152F      ((HSP_DC_START + ((HSP_KER_VEC_Q152F) * 2U)) * 4U)
/** Perform a saturation to each vector element, using a programmable value */
#define HSP_DIRECT_CMD_VEC_SAT_F32    ((HSP_DC_START + ((HSP_KER_SAT_F32) * 2U)) * 4U)

/* Matrix related functions */
/** Multiplication of two matrix */
#define HSP_DIRECT_CMD_MAT_MUL_F32         ((HSP_DC_START  + ((HSP_KER_MAT_MUL_F32) * 2U)) * 4U)
/** Transposition of a matrix */
#define HSP_DIRECT_CMD_MAT_TRANS_F32       ((HSP_DC_START  + ((HSP_KER_MAT_TRANS_F32) * 2U)) * 4U)
/** Matrix inversion */
#define HSP_DIRECT_CMD_MAT_INV_F32         ((HSP_DC_START + ((HSP_KER_MAT_INV_F32) * 2U)) * 4U)

/* Transformation related funcons */
/** Performs the fast Fourier transform of an input vector */
#define HSP_DIRECT_CMD_FFT_F32             ((HSP_DC_START  + ((HSP_KER_FFT_F32) * 2U)) * 4U)
/** Performs the fast Fourier transform of an input vector */
#define HSP_DIRECT_CMD_RFFT_F32            ((HSP_DC_START  + ((HSP_KER_RFFT_F32) * 2U)) * 4U)
/** Performs the discrete cosine transform of an input vector */
#define HSP_DIRECT_CMD_DCT_F32             ((HSP_DC_START  + ((HSP_KER_DCT_F32) * 2U)) * 4U)
/** Performs the inverse discrete cosine transform of an input vector */
#define HSP_DIRECT_CMD_IDCT_F32            ((HSP_DC_START  + ((HSP_KER_IDCT_F32) * 2U)) * 4U)

/* Complex functions */
/** Compute the conjugate of each vector element */
#define HSP_DIRECT_CMD_CMPLX_CONJ_F32          ((HSP_DC_START + ((HSP_KER_CMPLX_CONJ_F32) * 2U)) * 4U)
/** Computes the dot product of two complex vectors */
#define HSP_DIRECT_CMD_CMPLX_DOTPROD_F32       ((HSP_DC_START + ((HSP_KER_CMPLX_DOTPROD_F32) * 2U)) * 4U)
/** Vector Floating-point complex magnitude */
#define HSP_DIRECT_CMD_CMPLX_MAG_F32           ((HSP_DC_START + ((HSP_KER_CMPLX_MAG_F32) * 2U)) * 4U)
/** Vector Floating-point complex fast magnitude */
#define HSP_DIRECT_CMD_CMPLX_FMAG_F32          ((HSP_DC_START + ((HSP_KER_CMPLX_FMAG_F32) * 2U)) * 4U)
/** Vector Floating-point complex magnitude squared */
#define HSP_DIRECT_CMD_CMPLX_MAGSQUARED_F32    ((HSP_DC_START + ((HSP_KER_CMPLX_MAGSQUARED_F32) * 2U)) * 4U)
/** Compute the complex multiplication of 2 complex vector */
#define HSP_DIRECT_CMD_CMPLX_MUL_F32           ((HSP_DC_START + ((HSP_KER_CMPLX_MUL_F32) * 2U)) * 4U)
/** Compute the multiplication of a complex vector by a real vector and generates a complex result */
#define HSP_DIRECT_CMD_CMPLX_RMUL_F32          ((HSP_DC_START + ((HSP_KER_CMPLX_RMUL_F32) * 2U)) * 4U)
/** Compute the complex multiplication of one complex vector and exp ROM */
#define HSP_DIRECT_CMD_CMPLX_MUL_EXP_F32       ((HSP_DC_START + ((HSP_KER_CMPLX_MUL_EXP_F32) * 2U)) * 4U)
/** Compute the dot product of two complex vectors */
#define HSP_DIRECT_CMD_CMPLX_CMSIS_DOTPROD_F32 ((HSP_DC_START + ((HSP_KER_CMPLX_CMSIS_DOTPROD_F32) * 2U)) * 4U)


/* Counter function */
/** Direct Global counter kernel to set counter value */
#define HSP_DIRECT_CMD_COUNT ((HSP_DC_START + ((HSP_KER_CNT) * 2U)) * 4U)

/* Clear FWERR register function */
/** Clear FWERR register */
#define HSP_DIRECT_CMD_CLR_FWERR  ((HSP_DC_START + ((HSP_CLR_FWERR) * 2U)) * 4U)

/* Convolution function */
/** CNN conv 2D kernel */
#define HSP_DIRECT_CMD_CNN_CONV2D_I8  ((HSP_DC_START + ((HSP_CNN_CONV2D_I8) * 2U)) * 4U)
/** CNN conv deptwise kernel */
#define HSP_DIRECT_CMD_CNN_CONVDW_I8  ((HSP_DC_START + ((HSP_CNN_CONVDW_I8) * 2U)) * 4U)
/** CNN conv pointwise kernel */
#define HSP_DIRECT_CMD_CNN_CONVPW_I8  ((HSP_DC_START + ((HSP_CNN_CONVPW_I8) * 2U)) * 4U)
/** CNN conv fully connected kernel */
#define HSP_DIRECT_CMD_CNN_FC_I8      ((HSP_DC_START + ((HSP_CNN_FC_I8) * 2U)) * 4U)
/** CNN pooling kernel */
#define HSP_DIRECT_CMD_CNN_POOL_I8    ((HSP_DC_START + ((HSP_CNN_POOL_I8) * 2U)) * 4U)
/** Copy of a vector from MEMB to MEMA kernel */
#define HSP_DIRECT_CMD_MB2A        ((HSP_DC_START + ((HSP_VECT_MB2A) * 2U)) * 4U)
/* End direct commands */
/**
  * @}
  */

/** @defgroup HSP_Command_Fields_Constants HSP Command Fields Constants
  * @brief    HSP command fields description\n
  * @{
  */
/* plugin functions */
/** DRAM Kernel table size */
#define HSP_NB_DRAM_KERNELS_SIZE (HSP_NB_DRAM_KERNELS * 2U)
/** Define for Plug ID start offset */
#define HSP_DIRECT_CMD_PLUG_ID_OFFSET ((HSP_DC_START + (HSP_NB_DRAM_KERNELS_SIZE + 1U)) * 4U)
#define HSP_DIRECT_CMD_PLUG_ID_SCALE 12U /*!< Direct command for plugin scaling factor for DC ID */

/*
 _______________________________________________________________________________________________________________________________________________________________
| 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
|_______________________________________________________________________________________________________________________________________________________________|
|                     NB_OU_LINE                                      |                     NB_IN_LINE                                      |        mode       |
|_______________________________________________________________________________________________________________________________________________________________|
 */
#define HSP_CNN_CFG_MODE_MASK    0x0000000fU  /*!< mask for CNN mode */
#define HSP_CNN_CFG_STEP_MASK    0x00000007U  /*!< mask for step mode */
#define HSP_CNN_CFG_NB_LINE_MASK 0x00003fffU  /*!< mask for in/out nb line */
#define HSP_CNN_CFG_NB_IN_LINE_SHIFT   4U /*!< Shift for nb input lines in buffer */
#define HSP_CNN_CFG_NB_OU_LINE_SHIFT  18U /*!< Shift for nb output lines in buffer */
#define HSP_CNN_CFG_MODE_0STEP  0U  /*!< Mode 0 step for conv */
#define HSP_CNN_CFG_MODE_1STEP  1U  /*!< Mode 1 step for conv */
#define HSP_CNN_CFG_MODE_2STEP  2U  /*!< Mode 2 steps for conv */
#define HSP_CNN_CFG_MODE_3STEP  4U  /*!< Mode 3 steps for conv */
#define HSP_CNN_CFG_MODE_PCIRC  1U  /*!< Circular mode for pooling */
#define HSP_CNN_CFG_MODE_FCPP   1U  /*!< Ping pong mode for fully connected */

/* IOTYPE for pointer on pointer */
#define HSP_IOTYPE_POP  0xf0U /*!< One of parameter pointer is a pointer on pointer */
#define HSP_IOTYPE_POP0 0x10U /*!< First pointer address is a pointer on pointer */
#define HSP_IOTYPE_POP1 0x20U /*!< Second pointer address is a pointer on pointer */
#define HSP_IOTYPE_POP2 0x40U /*!< Third pointer address is a pointer on pointer */
#define HSP_IOTYPE_POP3 0x80U /*!< Third pointer address is a pointer on pointer */

/* IOTYPE for scalar immediate value */
#define HSP_IOTYPE_IMM  0x1000U /*!< Scalar parameter is an immediate */


/* BUFFx registers offset addresses */
#define HSP_REG_SPE_BUFF0DR           0x002010F8U
#define HSP_REG_SPE_BUFF1DR           0x002010FCU
#define HSP_REG_SPE_BUFF2DR           0x00201100U
#define HSP_REG_SPE_BUFF3DR           0x00201104U

/**
  * @}
  */

/** @defgroup HSP_BSTAT_Constants HSP State Constants
  * @brief    HSP State constant description\n
  * @{
  */
/** Define BSTAT position if not yet defined */
#define HSP_SPE_CR_BSTAT_Pos HSP_CR_BSTAT_Pos

#define HSP_BSTAT_RESETV   (0x0UL << HSP_SPE_CR_BSTAT_Pos) /*!< RESETV state: reset value */
#define HSP_BSTAT_BOOTOK   (0x1UL << HSP_SPE_CR_BSTAT_Pos) /*!< BOOTOK state: wait for init command */
#define HSP_BSTAT_WAICMD   (0x2UL << HSP_SPE_CR_BSTAT_Pos) /*!< WAICMD state: wait for first command */
#define HSP_BSTAT_PROCFG   (0x3UL << HSP_SPE_CR_BSTAT_Pos) /*!< PROCFG state: processing list configuration */
#define HSP_BSTAT_RST_PL   (0x4UL << HSP_SPE_CR_BSTAT_Pos) /*!< Reset PL: reset all Processing list */
#define HSP_BSTAT_ERROR    (0x6UL << HSP_SPE_CR_BSTAT_Pos) /*!< Bad init command */
#define HSP_BSTAT_ADD_PG   (0x7UL << HSP_SPE_CR_BSTAT_Pos) /*!< Add plugin */
#define HSP_BSTAT_ERR_CROM (0x8UL << HSP_SPE_CR_BSTAT_Pos) /*!< CROM ID error, not in lign with CRAM */

/**
  * @}
  */

/** @defgroup HSP_EVENT_Constants HSP Event Constants
  * @{
  */
/** Event to switch in supervisor mode */
#define HSP_EVENT_SUPERVISOR 37U

/** Event to switch in direct command */
#define HSP_EVENT_DIRECT_COMMAND 38U
/**
  * @}
  */


/** @defgroup HSP_CMPB_Constants HSP Comparison vector Constants
  * @{
  */
/** Type for comparison type used by comparison command */
#define HSP_CMP_TYPE_GT 0x01U /*!< comparison greater than */
#define HSP_CMP_TYPE_GE 0x02U /*!< comparison greater or equal */
#define HSP_CMP_TYPE_LT 0x03U /*!< comparison less than */
#define HSP_CMP_TYPE_LE 0x04U /*!< comparison less or equal */
#define HSP_CMP_TYPE_EQ 0x05U /*!< comparison equal */
/**
  * @}
  */

/** @defgroup HSP_COND_Constants HSP Conditional Constants
  * @{
  */
/** Type for data type of variable condition for conditional command */
#define HSP_COND_TYPE_UINT32 1U /*!< 32 bit unsigned integer */
#define HSP_COND_TYPE_INT32  2U /*!< 32 bit signed integer */
#define HSP_COND_TYPE_FLOAT  3U /*!< 32 bit float */
#define HSP_COND_TYPE_CONSTANT_UINT32 ((0x1U << 2U)| HSP_COND_TYPE_UINT32) /*!< 32 bit unsigned int immediate value */
#define HSP_COND_TYPE_CONSTANT_INT32  ((0x1U << 2U) | HSP_COND_TYPE_INT32) /*!< 32 bit signed int immediate value */
#define HSP_COND_TYPE_CONSTANT_FLOAT  ((0x1U << 2U) | HSP_COND_TYPE_FLOAT) /*!< 32 bit float immediate value */

/**
  * @}
  */
/**
  * @}
  */
/* End of private constants --------------------------------------------------*/

/** @addtogroup HSP_Exported_Constants
  * @{
  */
/** @addtogroup HSP_Transform_Constants HSP Transform Constants
  * @{
  */
#define HSP_TYPE_FFT_COMPLEX   0U  /*!< CFFT: identify a complex FFT */
#define HSP_TYPE_RFFT_TYPE_1   1U  /*!< RFFT: identify a real FFT with out[0]=in[0]+in[1] and out[1]=0 */
#define HSP_TYPE_RFFT_TYPE_2   2U  /*!< RFFT: identify a real FFT CMSIS-like */
#define HSP_TYPE_RFFT_TYPE_3   3U  /*!< RFFT: identify a real FFT with n+1 elements with out[n]=in[0]+in[1] */

/**
  * @}
  */

#define HSP_CMP_IFEQ      0U /*!< if kernel comparison equal */
#define HSP_CMP_IFNE      1U /*!< if kernel comparison not equal */
#define HSP_CMP_IFGT      2U /*!< if kernel comparison greater than */
#define HSP_CMP_IFLT      3U /*!< if kernel comparison less than */
#define HSP_CMP_IFGE      4U /*!< if kernel comparison greater or equal */
#define HSP_CMP_IFLE      5U /*!< if kernel comparison less or equal */

/** @defgroup HSP_Conditional_Expr_Constants HSP Conditional expression Constants
  * @{
  */
#define HSP_CMP_IFEQ_F32  0x0000U /*!< if kernel comparison equal */
#define HSP_CMP_IFEQ_INT  0x0001U /*!< if kernel comparison equal */
#define HSP_CMP_IFNE_F32  0x0002U /*!< if kernel comparison not equal */
#define HSP_CMP_IFNE_INT  0x0003U /*!< if kernel comparison not equal */
#define HSP_CMP_IFGT_F32  0x0004U /*!< if kernel comparison greater than */
#define HSP_CMP_IFGT_I32  0x0005U /*!< if kernel comparison greater than */
#define HSP_CMP_IFGT_U32  0x0006U /*!< if kernel comparison greater than */
#define HSP_CMP_IFLT_F32  0x0007U /*!< if kernel comparison less than */
#define HSP_CMP_IFLT_I32  0x0008U /*!< if kernel comparison less than */
#define HSP_CMP_IFLT_U32  0x0009U /*!< if kernel comparison less than */
#define HSP_CMP_IFGE_F32  0x000aU /*!< if kernel comparison greater or equal */
#define HSP_CMP_IFGE_I32  0x000bU /*!< if kernel comparison greater or equal */
#define HSP_CMP_IFGE_U32  0x000cU /*!< if kernel comparison greater or equal */
#define HSP_CMP_IFLE_F32  0x000dU /*!< if kernel comparison less or equal */
#define HSP_CMP_IFLE_I32  0x000eU /*!< if kernel comparison less or equal */
#define HSP_CMP_IFLE_U32  0x000fU /*!< if kernel comparison less or equal */
#define HSP_CMP_IFODD     0x0010U /*!< if kernel comparison odd value */
#define HSP_CMP_IFEVEN    0x0011U /*!< if kernel comparison even value */
#define HSP_CMP_IFELSE    0x0100U /*!< if kernel comparison else */
#define HSP_CMP_IFENDIF   0x0200U /*!< if kernel comparison endif */
#define HSP_CMP_LOOPSTART 0U /*!< loop kernel start loop */
#define HSP_CMP_LOOPEND   1U /*!< loop kernel end loop */

/** Maximum number of nested if */
#define HSP_MAX_NB_IF 15U

/** Maximum number of nested loop */
#define HSP_MAX_NB_LOOP 5U
/**
  * @}
  */

/** @defgroup HSP_Command_Status_Constants HSP Command status Constants
  * @{
  */
/* Command status return in FWERR or mailbox status register */
#define HSP_CMD_STATUS_MSK                    0x000000ffU /*!< Mask for command status */
#define HSP_CMD_STATUS_OK                     0x0000U /*!< User command completed with success */
#define HSP_CMD_STATUS_ERROR_CMDL_MEM_ALLOC   0x0001U /*!< Kernel cfg failed to alloc memory (CRAM) for PL */
#define HSP_CMD_STATUS_ERROR_KERNEL_MEM_ALLOC 0x0002U /*!< Kernel cfg failed to alloc memory (DRAM) */
#define HSP_CMD_STATUS_ERROR_SUPER_CMD        0x0003U /*!< Error: bad supervisor command */
#define HSP_CMD_STATUS_ERROR_INI_CMD          0x0004U /*!< Error: bad command, waiting for init command */
#define HSP_CMD_STATUS_ERROR_KERNEL_ID        0x0005U /*!< Kernel configuration failed: Bad kernel ID */
#define HSP_CMD_STATUS_ERROR_EMPTY_PL         0x0006U /*!< Empty processing list scheduled */
#define HSP_CMD_STATUS_ERROR_FLTBANK_DMA_SIZE 0x0007U /*!< Error, bad FLTBANK command size */
#define HSP_CMD_STATUS_ERROR_FATAL            0x0008U /*!< Fatal error: PL supervisor/direct/error alloc failed */
#define HSP_CMD_STATUS_ERROR_DIR_EXTB         0x0009U /*!< Error: bad external io detected in direct command */
#define HSP_CMD_STATUS_ERROR_DIR_CMD          0x000aU /*!< Error: bad command in direct command */
#define HSP_CMD_STATUS_ERROR_TSKOFF           0x000bU /*!< Error: tskoff address too high for TSKOFF0R size */
#define HSP_CMD_STATUS_ERROR_MAT_INV_SINGULAR 0x000cU /*!< Error: Matinv input matrix is singular: non-invertible */
#define HSP_CMD_STATUS_ERROR_PLUGIN_D_ALLOC   0x000dU /*!< Error: DRAM memory allocation failed to add plugin */
#define HSP_CMD_STATUS_ERROR_BGND_CHAN        0x000eU /*!< Error: bad background channel number */
#define HSP_CMD_STATUS_ERROR_BGND_CMD         0x000fU /*!< Error: bad background command suspend, resume, stop */
#define HSP_CMD_STATUS_ERROR_IFE_CMD          0x0010U /*!< Error: bad command ife */
#define HSP_CMD_STATUS_ERROR_IFE_MAX_IF       0x0011U /*!< Error: maximum number of nested <if-else> is reached */
#define HSP_CMD_STATUS_ERROR_IFE_UNCLOSED_IF  0x0012U /*!< Error: unclosed <if-else> */
#define HSP_CMD_STATUS_ERROR_MAX_LOOP         0x0013U /*!< Error: max loop reached */
#define HSP_CMD_STATUS_ERROR_LOOP_UNCLOSED    0x0014U /*!< Error: unclosed /loop/ */
#define HSP_CMD_STATUS_ERROR_NO_LOOP          0x0015U /*!< Error: not in /loop/ statement */
#define HSP_CMD_STATUS_ERROR_PLUGIN_INST      0x0016U /*!< Error: bad instruction in plugin */

/**
  * @}
  */

/** @defgroup HSP_Command_CountMode HSP Counter Command mode Constants
  * @{
  */
/* Command status return in FWERR or mailbox status register */
#define HSP_CMD_COUNT_MODE_SET 0U /*!< Counter command mode set */
#define HSP_CMD_COUNT_MODE_ADD 1U /*!< Counter command mode add */
/**
  * @}
  */
/** @defgroup HSP_Command_CountNb HSP Counter Command max number of counters Constants
  * @{
  */
/** Maximum number of countif */
#define HSP_CMD_COUNT_MAX_NB_COUNTIF 15U
/**
  * @}
  */
/* End of exported constants -------------------------------------------------*/
/**
  * @}
  */
/**
  * @}
  */
#endif /* __HSP_FW_DEF__H__ */
