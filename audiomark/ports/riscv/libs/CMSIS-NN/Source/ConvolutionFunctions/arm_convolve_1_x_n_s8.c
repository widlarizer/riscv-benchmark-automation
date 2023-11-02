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
 * Title:        arm_convolve_1_x_n_s8.c
 * Description:  s8 version of 1xN convolution using symmetric quantization.
 *
 * $Date:        26 October 2022
 * $Revision:    V.3.1.1
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
 * 1xN s8 convolution function.
 *
 * Refer header file for details.
 *
 */

arm_cmsis_nn_status arm_convolve_1_x_n_s8(const cmsis_nn_context *ctx,
                                          const cmsis_nn_conv_params *conv_params,
                                          const cmsis_nn_per_channel_quant_params *quant_params,
                                          const cmsis_nn_dims *input_dims,
                                          const int8_t *input_data,
                                          const cmsis_nn_dims *filter_dims,
                                          const int8_t *filter_data,
                                          const cmsis_nn_dims *bias_dims,
                                          const int32_t *bias_data,
                                          const cmsis_nn_dims *output_dims,
                                          int8_t *output_data)
{
    (void)bias_dims;
    arm_cmsis_nn_status status = ARM_CMSIS_NN_SUCCESS;
    /* The wrapper API is the ultimate reference for argument check */
    if ((input_dims->h != 1) || (output_dims->w % 4 != 0) || conv_params->dilation.w != 1)
    {
        status = ARM_CMSIS_NN_ARG_ERROR;
        goto out;
    }

    status = arm_convolve_s8(ctx,
                             conv_params,
                             quant_params,
                             input_dims,
                             input_data,
                             filter_dims,
                             filter_data,
                             bias_dims,
                             bias_data,
                             output_dims,
                             output_data);

out:
    /* Return to application */
    return status;
}

int32_t arm_convolve_1_x_n_s8_get_buffer_size(const cmsis_nn_dims *input_dims, const cmsis_nn_dims *filter_dims)
{
    return arm_convolve_s8_get_buffer_size(input_dims, filter_dims);
}

/**
 * @} end of NNConv group
 */
