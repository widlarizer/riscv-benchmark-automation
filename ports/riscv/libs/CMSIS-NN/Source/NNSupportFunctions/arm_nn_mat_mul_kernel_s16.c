/*
 * SPDX-FileCopyrightText: Copyright 2010-2020, 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        arm_nn_mat_mult_kernel_s16.c
 * Description:  Matrix-multiplication function for convolution
 *
 * $Date:        26 October 2022
 * $Revision:    V.1.1.1
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * Matrix-multiplication function for convolution with per-channel requantization.
 *
 * Refer header file for details.
 *
 */

int16_t *arm_nn_mat_mult_kernel_s16(const int8_t *input_a,
                                    const int16_t *input_b,
                                    const int32_t output_ch,
                                    const int32_t *out_shift,
                                    const int32_t *out_mult,
                                    const int16_t activation_min,
                                    const int16_t activation_max,
                                    const int32_t num_col_a,
                                    const int64_t *const output_bias,
                                    int16_t *out_0)
{

    (void)input_a;
    (void)input_b;
    (void)output_ch;
    (void)out_shift;
    (void)out_mult;
    (void)activation_min;
    (void)activation_max;
    (void)num_col_a;
    (void)output_bias;
    (void)out_0;
    /* To be completed */
    return NULL;
}

/**
 * @} end of Doxygen group
 */