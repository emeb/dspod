/**
  ******************************************************************************
  * @file    hsp_cnn.h
  * @brief   API for HSP CNN functions
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
#ifndef HSP_CNN_H
#define HSP_CNN_H

/* Includes ------------------------------------------------------------------*/
#include "hsp_api_def.h"


/** @addtogroup HSP_ENGINE
  * @{
  */

/** @addtogroup HSP_MODULES
  * @{
  */

/** @defgroup HSP_MODULES_CNN HSP Modules CNN
  * @details
  * This section includes a set of common CNN functions, allowing acceleration of
  * small neural networks. All functions are executed in the accelerator mode of the HSP.
  * @{
  */
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/** @defgroup HSP_MODULES_CNN_Exported_Functions HSP CNN Exported Functions
  * @{
  */
/* Exported functions --------------------------------------------------------*/
#if defined(__HSP_DMA__)
hsp_core_status_t HSP_ACC_CnnConvPointwise0_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max);
#endif /* __HSP_DMA__ */
hsp_core_status_t HSP_ACC_CnnConvPointwise1_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max);
hsp_core_status_t HSP_ACC_CnnConvPointwise2_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t nb_line_per_blocks);
hsp_core_status_t HSP_ACC_CnnConvPointwise3_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t nb_line_per_blocks);
#if defined(__HSP_DMA__)
hsp_core_status_t HSP_ACC_CnnConv2d0_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c, uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b);
#endif /* __HSP_DMA__ */
hsp_core_status_t HSP_ACC_CnnConv2d1_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks_m2);
hsp_core_status_t HSP_ACC_CnnConv2d2_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks_m2);
hsp_core_status_t HSP_ACC_CnnConv2d3_s8(hsp_engine_context_t *hmw,
                                        uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                        uint32_t k_w, uint32_t k_h,
                                        uint32_t ou_w, uint32_t ou_h, uint32_t ou_c, uint32_t stridex, uint32_t stridey,
                                        int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                        uint32_t *p_bias_data,
                                        float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                        uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max,
                                        uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                        uint32_t nb_line_per_blocks);
hsp_core_status_t HSP_ACC_CnnConvDepthwise1_s8(hsp_engine_context_t *hmw,
                                               uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                               uint32_t k_w, uint32_t k_h,
                                               uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                               uint32_t stridex, uint32_t stridey,
                                               int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                               int32_t *p_bias_data,
                                               float32_t in_scale, float32_t out_scale, const float32_t *p_wt_scale,
                                               int32_t off_in, int32_t off_ou, int32_t sat_min, int32_t sat_max,
                                               uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b);
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
                                               uint32_t nb_line_per_blocks);
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
                                               uint32_t nb_line_per_blocks);
hsp_core_status_t HSP_ACC_CnnFullyConnected0_s8(hsp_engine_context_t *hmw, uint32_t in_c, uint32_t ou_c,
                                                int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                                uint32_t *p_bias_data,
                                                float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                                uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max);
hsp_core_status_t HSP_ACC_CnnFullyConnected1_s8(hsp_engine_context_t *hmw, uint32_t in_c, uint32_t ou_c,
                                                int8_t *p_input_data, int8_t *p_filter_data, int8_t *p_output_data,
                                                uint32_t *p_bias_data,
                                                float32_t in_scale, float32_t out_scale, float32_t wt_scale,
                                                uint32_t off_in, uint32_t off_ou, uint32_t sat_min, uint32_t sat_max);
hsp_core_status_t HSP_ACC_CnnPool0_s8(hsp_engine_context_t *hmw, uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                      uint32_t k_w, uint32_t k_h, uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                      uint32_t stridex, uint32_t stridey, int8_t *p_input_data, int8_t *p_output_data,
                                      uint32_t sat_min, uint32_t sat_max, uint32_t pool_type);
hsp_core_status_t HSP_ACC_CnnPool1_s8(hsp_engine_context_t *hmw,
                                      uint32_t in_w, uint32_t in_h, uint32_t in_c,
                                      uint32_t k_w, uint32_t k_h,
                                      uint32_t ou_w, uint32_t ou_h, uint32_t ou_c,
                                      uint32_t stridex, uint32_t stridey,
                                      int8_t *p_input_data, int8_t *p_output_data, uint32_t sat_min, uint32_t sat_max,
                                      uint32_t padx_l, uint32_t padx_r, uint32_t pady_t, uint32_t pady_b,
                                      uint32_t pool_type, uint32_t nb_line_per_blocks);

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

#endif /* HSP_CNN_H */
