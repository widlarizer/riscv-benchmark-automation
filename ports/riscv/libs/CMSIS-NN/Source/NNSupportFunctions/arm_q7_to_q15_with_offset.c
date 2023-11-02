/*
 * SPDX-FileCopyrightText: Copyright 2010-2020, 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in_q7x4 compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in_q7x4 writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_q7_to_q15_with_offset.c
 * Description:  Converts the elements of the Q7 vector to Q15 vector with an added offset
 *
 * $Date:        26 October 2022
 * $Revision:    V.2.0.3
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#include "arm_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConversion
 * @{
 */

void arm_q7_to_q15_with_offset(const int8_t *src, int16_t *dst, uint32_t block_size, int16_t offset)
{
    int block_cnt;

    /* Run the below code for Cortex-M0 */
    /* Loop over block_size number of values */
    block_cnt = block_size;

    while (block_cnt > 0)
    {
        *dst++ = (int16_t)*src++ + offset;

        /* Decrement the loop counter */
        block_cnt--;
    }
}

/**
 * @} end of Doxygen group
 */
