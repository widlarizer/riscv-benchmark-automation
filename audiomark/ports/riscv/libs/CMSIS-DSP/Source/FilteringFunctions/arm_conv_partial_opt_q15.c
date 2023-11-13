/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_partial_opt_q15.c
 * Description:  Partial convolution of Q15 sequences
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
  @brief         Partial convolution of Q15 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written
  @param[in]     firstIndex is the first output sample to start with
  @param[in]     numPoints  is the number of output points to be computed
  @param[in]     pScratch1  points to scratch buffer of size max(srcALen, srcBLen) + 2*min(srcALen, srcBLen) - 2.
  @param[in]     pScratch2  points to scratch buffer of size min(srcALen, srcBLen).
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : requested subset is not in the range [0 srcALen+srcBLen-2]

  @remark
                   Refer to \ref arm_conv_partial_fast_q15() for a faster but less precise version of this function.
 */

arm_status arm_conv_partial_opt_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints,
        q15_t * pScratch1,
        q15_t * pScratch2)
{

        q15_t *pOut = pDst;                            /* Output pointer */
        q15_t *pScr1 = pScratch1;                      /* Temporary pointer for scratch1 */
        q15_t *pScr2 = pScratch2;                      /* Temporary pointer for scratch1 */
        q63_t acc0;                                    /* Accumulator */
        q31_t x1;                                      /* Temporary variables to hold state and coefficient values */
        q31_t y1;                                      /* State variables */
  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
  const q15_t *px;                                     /* Intermediate inputA pointer */
        q15_t *py;                                     /* Intermediate inputB pointer */
        uint32_t j, k, blkCnt;                         /* Loop counter */
        uint32_t tapCnt;                               /* Loop count */
        arm_status status;                             /* Status variable */


  /* Check for range of output samples to be calculated */
  if ((firstIndex + numPoints) > ((srcALen + (srcBLen - 1U))))
  {
    /* Set status as ARM_MATH_ARGUMENT_ERROR */
    status = ARM_MATH_ARGUMENT_ERROR;
  }
  else
  {
    /* The algorithm implementation is based on the lengths of the inputs. */
    /* srcB is always made to slide across srcA. */
    /* So srcBLen is always considered as shorter or equal to srcALen */
    if (srcALen >= srcBLen)
    {
      /* Initialization of inputA pointer */
      pIn1 = pSrcA;

      /* Initialization of inputB pointer */
      pIn2 = pSrcB;
    }
    else
    {
      /* Initialization of inputA pointer */
      pIn1 = pSrcB;

      /* Initialization of inputB pointer */
      pIn2 = pSrcA;

      /* srcBLen is always considered as shorter or equal to srcALen */
      j = srcBLen;
      srcBLen = srcALen;
      srcALen = j;
    }

    /* Temporary pointer for scratch2 */
    py = pScratch2;

    /* pointer to take end of scratch2 buffer */
    pScr2 = pScratch2 + srcBLen - 1;

    /* points to smaller length sequence */
    px = pIn2;


    /* Initialize k with number of samples */
    k = srcBLen;


    while (k > 0U)
    {
      /* copy second buffer in reversal manner for remaining samples */
      *pScr2-- = *px++;

      /* Decrement loop counter */
      k--;
    }

    /* Initialze temporary scratch pointer */
    pScr1 = pScratch1;

    /* Assuming scratch1 buffer is aligned by 32-bit */
    /* Fill (srcBLen - 1U) zeros in scratch buffer */
    arm_fill_q15(0, pScr1, (srcBLen - 1U));

    /* Update temporary scratch pointer */
    pScr1 += (srcBLen - 1U);

    /* Copy bigger length sequence(srcALen) samples in scratch1 buffer */

    /* Copy (srcALen) samples in scratch buffer */
    arm_copy_q15(pIn1, pScr1, srcALen);

    /* Update pointers */
    pScr1 += srcALen;

    /* Fill (srcBLen - 1U) zeros at end of scratch buffer */
    arm_fill_q15(0, pScr1, (srcBLen - 1U));

    /* Update pointer */
    pScr1 += (srcBLen - 1U);

    /* Initialization of pIn2 pointer */
    pIn2 = py;

    pScratch1 += firstIndex;

    pOut = pDst + firstIndex;

    /* Actual convolution process starts here */


    /* Initialize blkCnt with number of samples */
    blkCnt = numPoints;


    /* Calculate convolution for remaining samples of Bigger length sequence */
    while (blkCnt > 0)
    {
      /* Initialze temporary scratch pointer as scratch1 */
      pScr1 = pScratch1;

      /* Clear Accumlators */
      acc0 = 0;

      tapCnt = (srcBLen) >> 1U;

      while (tapCnt > 0U)
      {
        /* Read next two samples from scratch1 buffer */
        x1 = read_q15x2_ia (&pScr1);

        /* Read two samples from smaller buffer */
        y1 = read_q15x2_ia ((q15_t **) &pIn2);

        acc0 = __SMLALD(x1, y1, acc0);

        /* Decrement the loop counter */
        tapCnt--;
      }

      tapCnt = (srcBLen) & 1U;

      /* apply same above for remaining samples of smaller length sequence */
      while (tapCnt > 0U)
      {
        /* accumulate the results */
        acc0 += (*pScr1++ * *pIn2++);

        /* Decrement loop counter */
        tapCnt--;
      }

      blkCnt--;

      /* The result is in 2.30 format.  Convert to 1.15 with saturation.
       ** Then store the output in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT((acc0 >> 15), 16));

      /* Initialization of inputB pointer */
      pIn2 = py;

      pScratch1 += 1U;

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
