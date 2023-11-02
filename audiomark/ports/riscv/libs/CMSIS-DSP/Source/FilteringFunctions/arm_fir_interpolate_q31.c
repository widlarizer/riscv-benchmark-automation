/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_interpolate_q31.c
 * Description:  Q31 FIR interpolation
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
  @addtogroup FIR_Interpolate
  @{
 */

/**
  @brief         Processing function for the Q31 FIR interpolator.
  @param[in]     S          points to an instance of the Q31 FIR interpolator structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of input samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by <code>1/(numTaps/L)</code>.
                   since <code>numTaps/L</code> additions occur per output sample.
                   After all multiply-accumulates are performed, the 2.62 accumulator is truncated to 1.32 format and then saturated to 1.31 format.
 */

void arm_fir_interpolate_q31(
  const arm_fir_interpolate_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
//#if !defined(ARM_MATH_CM0_FAMILY)

        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCur;                              /* Points to the current sample of the state */
        q31_t *ptr1;                                   /* Temporary pointer for state buffer */
  const q31_t *ptr2;                                   /* Temporary pointer for coefficient buffer */
        q63_t sum0;                                    /* Accumulators */
        uint32_t i, blkCnt, tapCnt;                    /* Loop counters */
        uint32_t phaseLen = S->phaseLength;            /* Length of each polyphase filter component */
        uint32_t j;


  /* S->pState buffer contains previous frame (phaseLen - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (phaseLen - 1U);


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* Copy new input sample into the state buffer */
    *pStateCur++ = *pSrc++;

    /* Address modifier index of coefficient buffer */
    j = 1U;

    /* Loop over the Interpolation factor. */
    i = S->L;
    while (i > 0U)
    {
      /* Set accumulator to zero */
      sum0 = 0;

      /* Initialize state pointer */
      ptr1 = pState;

      /* Initialize coefficient pointer */
      ptr2 = pCoeffs + (S->L - j);

      /* Loop over the polyPhase length.
         Repeat until we've computed numTaps-(4*S->L) coefficients. */


      /* Initialize tapCnt with number of samples */
      tapCnt = phaseLen;


      while (tapCnt > 0U)
      {
        /* Perform the multiply-accumulate */
        sum0 += (q63_t) *ptr1++ * *ptr2;

        /* Upsampling is done by stuffing L-1 zeros between each sample.
         * So instead of multiplying zeros with coefficients,
         * Increment the coefficient pointer by interpolation factor times. */
        ptr2 += S->L;

        /* Decrement loop counter */
        tapCnt--;
      }

      /* The result is in the accumulator, store in the destination buffer. */
      *pDst++ = (q31_t) (sum0 >> 31);

      /* Increment the address modifier index of coefficient buffer */
      j++;

      /* Decrement the loop counter */
      i--;
    }

    /* Advance the state pointer by 1
     * to process the next group of interpolation factor number samples */
    pState = pState + 1;

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last phaseLen - 1 samples to the satrt of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;


    /* Initialize tapCnt with number of samples */
    tapCnt = (phaseLen - 1U);


  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }


}
/**
  @} end of FIR_Interpolate group
 */
