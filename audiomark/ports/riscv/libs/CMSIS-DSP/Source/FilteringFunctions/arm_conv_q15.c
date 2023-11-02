/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_q15.c
 * Description:  Convolution of Q15 sequences
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup Conv
  @{
 */

/**
  @brief         Convolution of Q15 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length srcALen+srcBLen-1.
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both inputs are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow.
                   The 34.30 result is then truncated to 34.15 format by discarding the low 15 bits and then saturated to 1.15 format.

  @remark
                   Refer to \ref arm_conv_fast_q15() for a faster but less precise version of this function.
  @remark
                   Refer to \ref arm_conv_opt_q15() for a faster implementation of this function using scratch buffers.
 */
void arm_conv_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst)
{


  const q15_t *pIn1 = pSrcA;                           /* InputA pointer */
  const q15_t *pIn2 = pSrcB;                           /* InputB pointer */
        q63_t sum;                                     /* Accumulator */
        uint32_t i, j;                                 /* Loop counters */

  /* Loop to calculate convolution for output length number of values */
  for (i = 0; i < (srcALen + srcBLen - 1); i++)
  {
    /* Initialize sum with zero to carry on MAC operations */
    sum = 0;

    /* Loop to perform MAC operations according to convolution equation */
    for (j = 0U; j <= i; j++)
    {
      /* Check the array limitations */
      if (((i - j) < srcBLen) && (j < srcALen))
      {
        /* z[i] += x[i-j] * y[j] */
        sum += ((q31_t) pIn1[j] * pIn2[i - j]);
      }
    }

    /* Store the output in the destination buffer */
    pDst[i] = (q15_t) __SSAT((sum >> 15U), 16U);
  }


}

/**
  @} end of Conv group
 */
