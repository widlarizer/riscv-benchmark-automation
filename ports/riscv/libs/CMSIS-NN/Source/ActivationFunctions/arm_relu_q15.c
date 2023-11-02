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
 * Title:        arm_relu_q15.c
 * Description:  Q15 version of ReLU
 *
 * $Date:        26 October 2022
 * $Revision:    V.1.0.4
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
 * @addtogroup Acti
 * @{
 */

/*
 * Q15 ReLu function
 *
 * Refer header file for details.
 *
 */

void arm_relu_q15(int16_t *data, uint16_t size)
{

    /* Run the following code as reference implementation for M cores without DSP extension */
    uint16_t i;

    for (i = 0; i < size; i++)
    {
        if (data[i] < 0)
            data[i] = 0;
    }

}

/**
 * @} end of Acti group
 */
