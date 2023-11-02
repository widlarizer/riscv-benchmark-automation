/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_decimate_fast_q15.c
 * Description:  Fast Q15 FIR Decimator
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
  @brief         Processing function for the Q15 FIR decimator (fast variant).
  @param[in]     S          points to an instance of the Q15 FIR decimator structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of input samples to process per call
  @return        none

  @par           Scaling and Overflow Behavior
                   This fast version uses a 32-bit accumulator with 2.30 format.
                   The accumulator maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around and distorts the result.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits (log2 is read as log to the base 2).
                   The 2.30 accumulator is then truncated to 2.15 format and saturated to yield the 1.15 result.
  @remark
                   Refer to \ref arm_fir_decimate_q15() for a slower implementation of this function which uses 64-bit accumulation to avoid wrap around distortion.
                   Both the slow and the fast versions use the same instance structure.
                   Use function \ref arm_fir_decimate_init_q15() to initialize the filter structure.
 */


void arm_fir_decimate_fast_q15(
  const arm_fir_decimate_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
        q15_t *pState = S->pState;                     /* State pointer */
  const q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q15_t *pStateCur;                              /* Points to the current sample of the state */
        q15_t *px;                                     /* Temporary pointer for state buffer */
  const q15_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q15_t x0, x1, c0;                              /* Temporary variables to hold state and coefficient values */
        q31_t sum0;                                    /* Accumulators */
        q31_t acc0, acc1;
        q15_t *px0, *px1;
        uint32_t blkCntN3;
        uint32_t numTaps = S->numTaps;                 /* Number of taps */
        uint32_t i, blkCnt, tapCnt, outBlockSize = blockSize / S->M;  /* Loop counters */


  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);

  /* Total number of output samples to be computed */
  blkCnt = outBlockSize / 2;
  blkCntN3 = outBlockSize - (2 * blkCnt);

  while (blkCnt > 0U)
  {
    /* Copy 2 * decimation factor number of new input samples into the state buffer */
    i = S->M * 2;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    acc0 = 0;
    acc1 = 0;

    /* Initialize state pointer */
    px0 = pState;
    px1 = pState + S->M;

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
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M * 2;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */

    *pDst++ = (q15_t) (__SSAT((acc0 >> 15), 16));
    *pDst++ = (q15_t) (__SSAT((acc1 >> 15), 16));

    /* Decrement loop counter */
    blkCnt--;
  }

  while (blkCntN3 > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    sum0 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;


    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;


    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */
    *pDst++ = (q15_t) (__SSAT((sum0 >> 15), 16));

    /* Decrement loop counter */
    blkCntN3--;
  }

  /* Processing is complete.
   ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.
   ** This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;

  i = (numTaps - 1U) >> 2U;

  /* copy data */
  while (i > 0U)
  {
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    i--;
  }

  i = (numTaps - 1U) % 0x04U;

  /* copy data */
  while (i > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    i--;
  }
}


/**
  @} end of FIR_decimate group
 */
