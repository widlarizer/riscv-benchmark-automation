/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_conv_partial_fast_q31.c
 * Description:  Fast Q31 Partial convolution
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
  @brief         Partial convolution of Q31 sequences (fast version).
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
                   Refer to \ref arm_conv_partial_q31() for a slower implementation of this function which uses a 64-bit accumulator to provide higher precision.
 */

arm_status arm_conv_partial_fast_q31(
  const q31_t * pSrcA,
        uint32_t srcALen,
  const q31_t * pSrcB,
        uint32_t srcBLen,
        q31_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints)
{
  const q31_t *pIn1;                                   /* InputA pointer */
  const q31_t *pIn2;                                   /* InputB pointer */
        q31_t *pOut = pDst;                            /* Output pointer */
  const q31_t *px;                                     /* Intermediate inputA pointer */
  const q31_t *py;                                     /* Intermediate inputB pointer */
  const q31_t *pSrc1, *pSrc2;                          /* Intermediate pointers */
        q31_t sum;                                     /* Accumulators */
        uint32_t j, k, count, check, blkCnt;
        int32_t blockSize1, blockSize2, blockSize3;    /* Loop counters */
        arm_status status;                             /* Status of Partial convolution */


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

    /* Conditions to check which loopCounter holds
     * the first and last indices of the output samples to be calculated. */
    check = firstIndex + numPoints;
    blockSize3 = ((int32_t)check > (int32_t)srcALen) ? (int32_t)check - (int32_t)srcALen : 0;
    blockSize3 = ((int32_t)firstIndex > (int32_t)srcALen - 1) ? blockSize3 - (int32_t)firstIndex + (int32_t)srcALen : blockSize3;
    blockSize1 = ((int32_t) srcBLen - 1) - (int32_t) firstIndex;
    blockSize1 = (blockSize1 > 0) ? ((check > (srcBLen - 1U)) ? blockSize1 :  (int32_t)numPoints) : 0;
    blockSize2 = (int32_t) check - ((blockSize3 + blockSize1) + (int32_t) firstIndex);
    blockSize2 = (blockSize2 > 0) ? blockSize2 : 0;

    /* conv(x,y) at n = x[n] * y[0] + x[n-1] * y[1] + x[n-2] * y[2] + ...+ x[n-N+1] * y[N -1] */
    /* The function is internally
     * divided into three stages according to the number of multiplications that has to be
     * taken place between inputA samples and inputB samples. In the first stage of the
     * algorithm, the multiplications increase by one for every iteration.
     * In the second stage of the algorithm, srcBLen number of multiplications are done.
     * In the third stage of the algorithm, the multiplications decrease by one
     * for every iteration. */

    /* Set the output pointer to point to the firstIndex
     * of the output sample to be calculated. */
    pOut = pDst + firstIndex;

    /* --------------------------
     * Initializations of stage1
     * -------------------------*/

    /* sum = x[0] * y[0]
     * sum = x[0] * y[1] + x[1] * y[0]
     * ....
     * sum = x[0] * y[srcBlen - 1] + x[1] * y[srcBlen - 2] +...+ x[srcBLen - 1] * y[0]
     */

    /* In this stage the MAC operations are increased by 1 for every iteration.
       The count variable holds the number of MAC operations performed.
       Since the partial convolution starts from firstIndex
       Number of Macs to be performed is firstIndex + 1 */
    count = 1U + firstIndex;

    /* Working pointer of inputA */
    px = pIn1;

    /* Working pointer of inputB */
    pSrc2 = pIn2 + firstIndex;
    py = pSrc2;

    /* ------------------------
     * Stage1 process
     * ----------------------*/

    /* The first stage starts here */
    while (blockSize1 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;


      /* Initialize k with number of samples */
      k = count;


      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = sum << 1;

      /* Update the inputA and inputB pointers for next MAC calculation */
      py = ++pSrc2;
      px = pIn1;

      /* Increment MAC count */
      count++;

      /* Decrement loop counter */
      blockSize1--;
    }

    /* --------------------------
     * Initializations of stage2
     * ------------------------*/

    /* sum = x[0] * y[srcBLen-1] + x[1] * y[srcBLen-2] +...+ x[srcBLen-1] * y[0]
     * sum = x[1] * y[srcBLen-1] + x[2] * y[srcBLen-2] +...+ x[srcBLen] * y[0]
     * ....
     * sum = x[srcALen-srcBLen-2] * y[srcBLen-1] + x[srcALen] * y[srcBLen-2] +...+ x[srcALen-1] * y[0]
     */

    /* Working pointer of inputA */
    if ((int32_t)firstIndex - (int32_t)srcBLen + 1 > 0)
    {
      pSrc1 = pIn1 + firstIndex - srcBLen + 1;
    }
    else
    {
      pSrc1 = pIn1;
    }
    px = pSrc1;

    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    py = pSrc2;

    /* count is index by which the pointer pIn1 to be incremented */
    count = 0U;

    /* -------------------
     * Stage2 process
     * ------------------*/

    /* Stage2 depends on srcBLen as in this stage srcBLen number of MACS are performed.
     * So, to loop unroll over blockSize2,
     * srcBLen should be greater than or equal to 4 */
    if (srcBLen >= 4U)
    {

      /* Initialize blkCnt with number of samples */
      blkCnt = blockSize2;


      while (blkCnt > 0U)
      {
        /* Accumulator is made zero for every iteration */
        sum = 0;


        /* Initialize blkCnt with number of samples */
        k = srcBLen;


        while (k > 0U)
        {
          /* Perform the multiply-accumulate */
          sum = (q31_t) ((((q63_t) sum << 32) +
                          ((q63_t) *px++ * (*py--))) >> 32);

          /* Decrement loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = sum << 1;

        /* Increment MAC count */
        count++;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement loop counter */
        blkCnt--;
      }
    }
    else
    {
      /* If the srcBLen is not a multiple of 4,
       * the blockSize2 loop cannot be unrolled by 4 */
      blkCnt = (uint32_t) blockSize2;

      while (blkCnt > 0U)
      {
        /* Accumulator is made zero for every iteration */
        sum = 0;

        /* srcBLen number of MACS should be performed */
        k = srcBLen;

        while (k > 0U)
        {
          /* Perform the multiply-accumulate */
          sum = (q31_t) ((((q63_t) sum << 32) +
                          ((q63_t) *px++ * (*py--))) >> 32);

          /* Decrement loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = sum << 1;

        /* Increment the MAC count */
        count++;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement the loop counter */
        blkCnt--;
      }
    }


    /* --------------------------
     * Initializations of stage3
     * -------------------------*/

    /* sum += x[srcALen-srcBLen+1] * y[srcBLen-1] + x[srcALen-srcBLen+2] * y[srcBLen-2] +...+ x[srcALen-1] * y[1]
     * sum += x[srcALen-srcBLen+2] * y[srcBLen-1] + x[srcALen-srcBLen+3] * y[srcBLen-2] +...+ x[srcALen-1] * y[2]
     * ....
     * sum +=  x[srcALen-2] * y[srcBLen-1] + x[srcALen-1] * y[srcBLen-2]
     * sum +=  x[srcALen-1] * y[srcBLen-1]
     */

    /* In this stage the MAC operations are decreased by 1 for every iteration.
       The count variable holds the number of MAC operations performed */
    count = srcBLen - 1U;

    /* Working pointer of inputA */
    if (firstIndex > srcALen)
    {
       pSrc1 = (pIn1 + firstIndex) - (srcBLen - 1U);
    }
    else
    {
       pSrc1 = (pIn1 + srcALen) - (srcBLen - 1U);
    }
    px = pSrc1;

    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    py = pSrc2;

    /* -------------------
     * Stage3 process
     * ------------------*/

    while (blockSize3 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;


      /* Initialize blkCnt with number of samples */
      k = count;


      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        /* sum +=  x[srcALen-1] * y[srcBLen-1] */
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = sum << 1;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = ++pSrc1;
      py = pSrc2;

      /* Decrement MAC count */
      count--;

      /* Decrement the loop counter */
      blockSize3--;
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
