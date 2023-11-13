/*
 * SPDX-FileCopyrightText: Copyright 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        arm_depthwise_conv_fast_s16.c
 * Description:  Optimized s16 depthwise separable convolution function for
 *               channel multiplier of 1.
 *
 * $Date:        26 October 2022
 * $Revision:    V.1.1.1
 *
 * Target Processor:  Cortex-M CPUs
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
 * Optimized s16 depthwise convolution function with constraint that in_channel equals out_channel
 *
 *  Refer prototype header file for details.
 *
 */

arm_cmsis_nn_status arm_depthwise_conv_fast_s16(const cmsis_nn_context *ctx,
                                                const cmsis_nn_dw_conv_params *dw_conv_params,
                                                const cmsis_nn_per_channel_quant_params *quant_params,
                                                const cmsis_nn_dims *input_dims,
                                                const int16_t *input,
                                                const cmsis_nn_dims *filter_dims,
                                                const int8_t *kernel,
                                                const cmsis_nn_dims *bias_dims,
                                                const int64_t *bias,
                                                const cmsis_nn_dims *output_dims,
                                                int16_t *output)
{
    const int32_t input_ch = input_dims->c;
    const int32_t output_ch = output_dims->c;

    /* Check input constraints input_ch == output_ch */
    if (input_ch != output_ch)
    {
        return ARM_CMSIS_NN_ARG_ERROR;
    }

    if (filter_dims->w * filter_dims->h >= 512)
    {
        return ARM_CMSIS_NN_ARG_ERROR;
    }

    if (ctx->buf == NULL && arm_depthwise_conv_fast_s16_get_buffer_size(input_dims, filter_dims) > 0)
    {
        return ARM_CMSIS_NN_ARG_ERROR;
    }

    /* Run the following code as reference implementation for Cortex-M0 and Cortex-M3 */
    return arm_depthwise_conv_s16(ctx,
                                  dw_conv_params,
                                  quant_params,
                                  input_dims,
                                  input,
                                  filter_dims,
                                  kernel,
                                  bias_dims,
                                  bias,
                                  output_dims,
                                  output);

    /* Return to application */
    return ARM_CMSIS_NN_SUCCESS;
}

int32_t arm_depthwise_conv_fast_s16_get_buffer_size(const cmsis_nn_dims *input_dims, const cmsis_nn_dims *filter_dims)
{
    (void)input_dims;
    (void)filter_dims;
    return 0;
}

/**
 * @} end of NNConv group
 */
