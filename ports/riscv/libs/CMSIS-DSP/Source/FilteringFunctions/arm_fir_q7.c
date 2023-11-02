/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_q7.c
 * Description:  Q7 FIR filter processing function
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
  @brief         Processing function for Q7 FIR filter.
  @param[in]     S          points to an instance of the Q7 FIR filter structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 32-bit internal accumulator.
                   Both coefficients and state variables are represented in 1.7 format and multiplications yield a 2.14 result.
                   The 2.14 intermediate results are accumulated in a 32-bit accumulator in 18.14 format.
                   There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.
                   The accumulator is converted to 18.7 format by discarding the low 7 bits.
                   Finally, the result is truncated to 1.7 format.
 */

void arm_fir_q7(
  const arm_fir_instance_q7 * S,
  const q7_t * pSrc,
        q7_t * pDst,
        uint32_t blockSize)
{
        q7_t *pState = S->pState;                      /* State pointer */
  const q7_t *pCoeffs = S->pCoeffs;                    /* Coefficient pointer */
        q7_t *pStateCurnt;                             /* Points to the current sample of the state */
        q7_t *px;                                      /* Temporary pointer for state buffer */
  const q7_t *pb;                                      /* Temporary pointer for coefficient buffer */
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
    while (i > 0U)
    {
      acc0 += (q15_t) * (px++) * (*(pb++));
      i--;
    }

    /* The result is in 2.14 format. Convert to 1.7
       Then store the output in the destination buffer. */
    *pDst++ = __SSAT((acc0 >> 7U), 8);

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
