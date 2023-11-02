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
 * Title:        arm_convolve_fast_s16.c
 * Description:  Optimized s16 version of convolution.
 *
 * $Date:        26 October 2022
 * $Revision:    V.2.0.1
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup NNConv
 * @{
 */

/*
 * Basic s16 convolution function.
 *
 * Refer header file for details. Optimal use case for the DSP/MVE implementation is when input and output channels
 * are multiples of 4 or atleast greater than 4.
 *
 */

arm_cmsis_nn_status arm_convolve_fast_s16(const cmsis_nn_context *ctx,
                                          const cmsis_nn_conv_params *conv_params,
                                          const cmsis_nn_per_channel_quant_params *quant_params,
                                          const cmsis_nn_dims *input_dims,
                                          const int16_t *input_data,
                                          const cmsis_nn_dims *filter_dims,
                                          const int8_t *filter_data,
                                          const cmsis_nn_dims *bias_dims,
                                          const int64_t *bias_data,
                                          const cmsis_nn_dims *output_dims,
                                          int16_t *output_data)
{
    (void)bias_dims;
    if (filter_dims->w * filter_dims->h * input_dims->c >= 512)
    {
        return ARM_CMSIS_NN_ARG_ERROR;
    }

    if (ctx->buf == NULL && arm_convolve_s8_get_buffer_size(input_dims, filter_dims) > 0)
    {
        return ARM_CMSIS_NN_ARG_ERROR;
    }
    int16_t *buffer_a = (int16_t *)ctx->buf;

    const int32_t input_batches = input_dims->n;
    const int32_t input_x = input_dims->w;
    const int32_t input_y = input_dims->h;
    const int32_t input_ch = input_dims->c;
    const int32_t kernel_x = filter_dims->w;
    const int32_t kernel_y = filter_dims->h;
    const int32_t output_x = output_dims->w;
    const int32_t output_y = output_dims->h;
    const int32_t output_ch = output_dims->c;

    const int32_t pad_x = conv_params->padding.w;
    const int32_t pad_y = conv_params->padding.h;
    const int32_t stride_x = conv_params->stride.w;
    const int32_t stride_y = conv_params->stride.h;

    const int16_t out_activation_min = conv_params->activation.min;
    const int16_t out_activation_max = conv_params->activation.max;
    int32_t *output_mult = quant_params->multiplier;
    int32_t *output_shift = quant_params->shift;

    for (int i_batch = 0; i_batch < input_batches; i_batch++)
    {
        (void)input_data;
        (void)output_data;
        (void)bias_data;
        (void)filter_data;
        (void)buffer_a;
        (void)kernel_x;
        (void)kernel_y;
        (void)pad_x;
        (void)pad_y;
        (void)stride_x;
        (void)stride_y;
        (void)out_activation_min;
        (void)out_activation_max;
        (void)output_mult;
        (void)output_shift;
        return ARM_CMSIS_NN_ARG_ERROR;
        /* Advance to the next batch */
        input_data += (input_x * input_y * input_ch);
        output_data += (output_x * output_y * output_ch);
    }

    /* Return to application */
    return ARM_CMSIS_NN_SUCCESS;
}

int32_t arm_convolve_fast_s16_get_buffer_size(const cmsis_nn_dims *input_dims, const cmsis_nn_dims *filter_dims)
{
    (void)input_dims;
    (void)filter_dims;
    return 0;
}

/**
 * @} end of NNConv group
 */
