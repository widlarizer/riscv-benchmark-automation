/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_nn_mat_mul_core_1x_s8.c
 * Description:  General Matrix-multiplication function
 *
 * $Date:        26 October 2022
 * $Revision:    V.3.1.1
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#include "arm_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * s8 matrix multiplication to process 1 row
 *
 * Refer header file for details.
 *
 */
arm_cmsis_nn_status arm_nn_mat_mul_core_1x_s8(int32_t row_elements,
                                              const int32_t skipped_row_elements,
                                              const int8_t *row_base_ref,
                                              const int8_t *col_base_ref,
                                              const int32_t out_ch,
                                              const cmsis_nn_conv_params *conv_params,
                                              const cmsis_nn_per_channel_quant_params *quant_params,
                                              const int32_t *bias,
                                              int8_t *output)
{
    (void)row_elements;
    (void)skipped_row_elements;
    (void)row_base_ref;
    (void)col_base_ref;
    (void)out_ch;
    (void)conv_params;
    (void)quant_params;
    (void)bias;
    (void)output;
    return ARM_CMSIS_NN_SUCCESS;
}

/**
 * @} end of supportConvolution group
 */
