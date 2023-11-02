/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_partial_q7.c
 * Description:  Partial convolution of Q7 sequences
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
  @addtogroup PartialConv
  @{
 */

/**
  @brief         Partial convolution of Q7 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written
  @param[in]     firstIndex is the first output sample to start with
  @param[in]     numPoints  is the number of output points to be computed
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : requested subset is not in the range [0 srcALen+srcBLen-2]

  @remark
                   Refer to \ref arm_conv_partial_opt_q7() for a faster implementation of this function.
 */

arm_status arm_conv_partial_q7(
  const q7_t * pSrcA,
        uint32_t srcALen,
  const q7_t * pSrcB,
        uint32_t srcBLen,
        q7_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints)
{

/* alternate version for CM0_FAMILY */

  const q7_t *pIn1 = pSrcA;                            /* InputA pointer */
  const q7_t *pIn2 = pSrcB;                            /* InputB pointer */
        q31_t sum;                                     /* Accumulator */
        uint32_t i, j;                                 /* Loop counters */
        arm_status status;                             /* Status of Partial convolution */

  /* Check for range of output samples to be calculated */
  if ((firstIndex + numPoints) > ((srcALen + (srcBLen - 1U))))
  {
    /* Set status as ARM_MATH_ARGUMENT_ERROR */
    status = ARM_MATH_ARGUMENT_ERROR;
  }
  else
  {
    /* Loop to calculate convolution for output length number of values */
    for (i = firstIndex; i <= (firstIndex + numPoints - 1); i++)
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
          sum += ((q15_t) pIn1[j] * (pIn2[i - j]));
        }
      }

      /* Store the output in the destination buffer */
      pDst[i] = (q7_t) __SSAT((sum >> 7U), 8U);
    }

    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);


}

/**
  @} end of PartialConv group
 */
