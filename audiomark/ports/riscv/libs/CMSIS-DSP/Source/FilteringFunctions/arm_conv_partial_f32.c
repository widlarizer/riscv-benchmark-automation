/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_partial_f32.c
 * Description:  Partial convolution of floating-point sequences
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
  @defgroup PartialConv Partial Convolution

  Partial Convolution is equivalent to Convolution except that a subset of the output samples is generated.
  Each function has two additional arguments.
  <code>firstIndex</code> specifies the starting index of the subset of output samples.
  <code>numPoints</code> is the number of output samples to compute.
  The function computes the output in the range
  <code>[firstIndex, ..., firstIndex+numPoints-1]</code>.
  The output array <code>pDst</code> contains <code>numPoints</code> values.

  The allowable range of output indices is [0 srcALen+srcBLen-2].
  If the requested subset does not fall in this range then the functions return ARM_MATH_ARGUMENT_ERROR.
  Otherwise the functions return ARM_MATH_SUCCESS.
  \note Refer to \ref arm_conv_f32() for details on fixed point behavior.

  @par           Fast Versions
                   Fast versions are supported for Q31 and Q15 of partial convolution.
                   Cycles for Fast versions are less compared to Q31 and Q15 of partial conv and the design requires
                   the input signals should be scaled down to avoid intermediate overflows.

  @par           Opt Versions
                   Opt versions are supported for Q15 and Q7. Design uses internal scratch buffer for getting good optimisation.
                   These versions are optimised in cycles and consumes more memory (Scratch memory) compared to Q15 and Q7 versions of partial convolution
 
  @par           Long versions:
                   For convolution of long vectors, those functions are
                   no more adapted and will be very slow.
                   An implementation based upon FFTs should be used.

 */

/**
  @addtogroup PartialConv
  @{
 */

/**
  @brief         Partial convolution of floating-point sequences.
  @param[in]     pSrcA       points to the first input sequence
  @param[in]     srcALen     length of the first input sequence
  @param[in]     pSrcB       points to the second input sequence
  @param[in]     srcBLen     length of the second input sequence
  @param[out]    pDst        points to the location where the output result is written
  @param[in]     firstIndex  is the first output sample to start with
  @param[in]     numPoints   is the number of output points to be computed
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : requested subset is not in the range [0 srcALen+srcBLen-2]
 */

arm_status arm_conv_partial_f32(
  const float32_t * pSrcA,
        uint32_t srcALen,
  const float32_t * pSrcB,
        uint32_t srcBLen,
        float32_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints)
{
/* alternate version for CM0_FAMILY */

  const float32_t *pIn1 = pSrcA;                       /* InputA pointer */
  const float32_t *pIn2 = pSrcB;                       /* InputB pointer */
        float32_t sum;                                 /* Accumulator */
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
      sum = 0.0f;

      /* Loop to perform MAC operations according to convolution equation */
      for (j = 0U; j <= i; j++)
      {
        /* Check the array limitations */
        if (((i - j) < srcBLen) && (j < srcALen))
        {
          /* z[i] += x[i-j] * y[j] */
          sum += ( pIn1[j] * pIn2[i - j]);
        }
      }

      /* Store the output in the destination buffer */
      pDst[i] = sum;
    }

    /* Set status as ARM_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);

}

/**
  @} end of PartialConv group
 */
