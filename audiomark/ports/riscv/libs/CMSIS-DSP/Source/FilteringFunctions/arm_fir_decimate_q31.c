/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_decimate_q31.c
 * Description:  Q31 FIR Decimator
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
  @addtogroup FIR_decimate
  @{
 */

/**
  @brief         Processing function for the Q31 FIR decimator.
  @param[in]     S          points to an instance of the Q31 FIR decimator structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of input samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits (where log2 is read as log to the base 2).
                   After all multiply-accumulates are performed, the 2.62 accumulator is truncated to 1.32 format and then saturated to 1.31 format.

 @remark
                   Refer to \ref arm_fir_decimate_fast_q31() for a faster but less precise implementation of this function.
 */

void arm_fir_decimate_q31(
  const arm_fir_decimate_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCur;                              /* Points to the current sample of the state */
        q31_t *px0;                                    /* Temporary pointer for state buffer */
  const q31_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q31_t x0, c0;                                  /* Temporary variables to hold state and coefficient values */
        q63_t acc0;                                    /* Accumulator */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;  /* Loop counters */


  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);


  /* Initialize blkCnt with number of samples */
  blkCnt = outBlockSize;


  while (blkCnt > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    acc0 = 0;

    /* Initialize state pointer */
    px0 = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;


    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;


    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 += (q63_t) x0 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = (q31_t) (acc0 >> 31);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;


  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);


  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

}
/**
  @} end of FIR_decimate group
 */
