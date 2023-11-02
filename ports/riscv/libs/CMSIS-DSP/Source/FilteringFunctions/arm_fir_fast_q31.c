/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_fast_q31.c
 * Description:  Processing function for the Q31 Fast FIR filter
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
  @addtogroup FIR
  @{
 */

/**
  @brief         Processing function for the Q31 FIR filter (fast version).
  @param[in]     S          points to an instance of the Q31 structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   This function is optimized for speed at the expense of fixed-point precision and overflow protection.
                   The result of each 1.31 x 1.31 multiplication is truncated to 2.30 format.
                   These intermediate results are added to a 2.30 accumulator.
                   Finally, the accumulator is saturated and converted to a 1.31 result.
                   The fast version has the same overflow behavior as the standard version and provides less precision since it discards the low 32 bits of each multiplication result.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits.

 @remark
                   Refer to \ref arm_fir_q31() for a slower implementation of this function which uses a 64-bit accumulator to provide higher precision.  Both the slow and the fast versions use the same instance structure.
                   Use function \ref arm_fir_init_q31() to initialize the filter structure.
 */

void arm_fir_fast_q31(
  const arm_fir_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCurnt;                            /* Points to the current sample of the state */
        q31_t *px;                                     /* Temporary pointer for state buffer */
  const q31_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q31_t acc0;                                    /* Accumulators */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt;                    /* Loop counters */


  /* S->pState points to state array which contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = &(S->pState[(numTaps - 1U)]);


  /* Initialize blkCnt with number of taps */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* Copy one sample at a time into state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Set the accumulator to zero */
    acc0 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize Coefficient pointer */
    pb = pCoeffs;

    i = numTaps;

    /* Perform the multiply-accumulates */
    do
    {
      multAcc_32x32_keep32_R(acc0, (*px++), (*pb++));
      i--;
    } while (i > 0U);

    /* The result is in 2.30 format. Convert to 1.31
       Then store the output in the destination buffer. */
    *pDst++ = (q31_t) (acc0 << 1);

    /* Advance state pointer by 1 for the next sample */
    pState = pState + 1U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the start of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;


  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);


  /* Copy remaining data */
  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement the loop counter */
    tapCnt--;
  }

}
/**
  @} end of FIR group
 */
