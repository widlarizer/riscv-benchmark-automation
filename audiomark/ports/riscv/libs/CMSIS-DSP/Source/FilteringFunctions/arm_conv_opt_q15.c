/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_opt_q15.c
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
  @param[in]     pScratch1  points to scratch buffer of size max(srcALen, srcBLen) + 2*min(srcALen, srcBLen) - 2.
  @param[in]     pScratch2  points to scratch buffer of size min(srcALen, srcBLen).
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both inputs are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow.
                   The 34.30 result is then truncated to 34.15 format by discarding the low 15 bits and then saturated to 1.15 format.
  @remark
                   Refer to \ref arm_conv_fast_q15() for a faster but less precise version of this function.
 */

void arm_conv_opt_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst,
        q15_t * pScratch1,
        q15_t * pScratch2)
{
        q63_t acc0;                                    /* Accumulators */
  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
        q15_t *pOut = pDst;                            /* Output pointer */
        q15_t *pScr1 = pScratch1;                      /* Temporary pointer for scratch1 */
        q15_t *pScr2 = pScratch2;                      /* Temporary pointer for scratch1 */
  const q15_t *px;                                     /* Intermediate inputA pointer */
        q15_t *py;                                     /* Intermediate inputB pointer */
        uint32_t j, k, blkCnt;                         /* Loop counter */
        uint32_t tapCnt;                               /* Loop count */



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

  /* Pointer to take end of scratch2 buffer */
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
  /* Fill (srcBLen - 1U) zeros in scratch1 buffer */
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

  /* Temporary pointer for scratch2 */
  py = pScratch2;


  /* Initialization of pIn2 pointer */
  pIn2 = py;


  /* Initialize blkCnt with number of samples */
  blkCnt = (srcALen + srcBLen - 1U);


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
      acc0 += (*pScr1++ * *pIn2++);
      acc0 += (*pScr1++ * *pIn2++);

      /* Decrement loop counter */
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
       Then store the output in the destination buffer. */
    *pOut++ = (q15_t) (__SSAT((acc0 >> 15), 16));

    /* Initialization of inputB pointer */
    pIn2 = py;

    pScratch1 += 1U;
  }

}

/**
  @} end of Conv group
 */
