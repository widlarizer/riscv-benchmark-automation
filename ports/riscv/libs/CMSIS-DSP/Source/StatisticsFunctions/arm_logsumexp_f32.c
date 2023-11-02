/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_logsumexp_f32.c
 * Description:  LogSumExp
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/statistics_functions.h"
#include <limits.h>
#include <math.h>


/**
 * @addtogroup LogSumExp
 * @{
 */


/**
 * @brief Computation of the LogSumExp
 *
 * In probabilistic computations, the dynamic of the probability values can be very
 * wide because they come from gaussian functions.
 * To avoid underflow and overflow issues, the values are represented by their log.
 * In this representation, multiplying the original exp values is easy : their logs are added.
 * But adding the original exp values is requiring some special handling and it is the
 * goal of the LogSumExp function.
 *
 * If the values are x1...xn, the function is computing:
 *
 * ln(exp(x1) + ... + exp(xn)) and the computation is done in such a way that
 * rounding issues are minimised.
 *
 * The max xm of the values is extracted and the function is computing:
 * xm + ln(exp(x1 - xm) + ... + exp(xn - xm))
 *
 * @param[in]  *in         Pointer to an array of input values.
 * @param[in]  blockSize   Number of samples in the input array.
 * @return LogSumExp
 *
 */

float32_t arm_logsumexp_f32(const float32_t *in, uint32_t blockSize)
{
    float32_t maxVal;
    float32_t tmp;
    const float32_t *pIn;
    uint32_t blkCnt;
    float32_t accum;
 
    pIn = in;
    blkCnt = blockSize;

    maxVal = *pIn++;
    blkCnt--;

    while(blkCnt > 0)
    {
       tmp = *pIn++;

       if (tmp > maxVal)
       {
          maxVal = tmp;
       }
       blkCnt--;
    
    }

    blkCnt = blockSize;
    pIn = in;
    accum = 0;
    while(blkCnt > 0)
    {
       tmp = *pIn++;
       accum += expf(tmp - maxVal);
       blkCnt--;
    
    }
    accum = maxVal + logf(accum);

    return(accum);
}

/**
 * @} end of LogSumExp group
 */
