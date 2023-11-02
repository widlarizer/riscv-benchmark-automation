/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir_lattice_q31.c
 * Description:  Q31 IIR Lattice filter processing function
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
  @addtogroup IIR_Lattice
  @{
 */

/**
  @brief         Processing function for the Q31 IIR lattice filter.
  @param[in]     S          points to an instance of the Q31 IIR lattice structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by 2*log2(numStages) bits.
                   After all multiply-accumulates are performed, the 2.62 accumulator is saturated to 1.32 format and then truncated to 1.31 format.
 */

void arm_iir_lattice_q31(
  const arm_iir_lattice_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{       
        q31_t *pState = S->pState;                       /* State pointer */
        q31_t *pStateCur;                                /* State current pointer */
        q31_t fcurr, fnext = 0, gcurr = 0, gnext;        /* Temporary variables for lattice stages */
        q63_t acc;                                       /* Accumlator */
        q31_t *px1, *px2, *pk, *pv;                      /* Temporary pointers for state and coef */
        uint32_t numStages = S->numStages;               /* Number of stages */
        uint32_t blkCnt, tapCnt;                         /* Temporary variables for counts */


  /* initialise loop count */
  blkCnt = blockSize;


  /* Sample processing */
  while (blkCnt > 0U)
  {
    /* Read Sample from input buffer */
    /* fN(n) = x(n) */
    fcurr = *pSrc++;

    /* Initialize Ladder coeff pointer */
    pv = &S->pvCoeffs[0];

    /* Initialize Reflection coeff pointer */
    pk = &S->pkCoeffs[0];

    /* Initialize state read pointer */
    px1 = pState;

    /* Initialize state write pointer */
    px2 = pState;

    /* Set accumulator to zero */
    acc = 0;

    tapCnt = numStages;

    while (tapCnt > 0U)
    {
      gcurr = *px1++;
      /* Process sample */
      /* fN-1(n) = fN(n) - kN * gN-1(n-1) */
      fnext = clip_q63_to_q31(((q63_t) fcurr - ((q31_t) (((q63_t) gcurr * (*pk  )) >> 31))));

      /* gN(n) = kN * fN-1(n) + gN-1(n-1) */
      gnext = clip_q63_to_q31(((q63_t) gcurr + ((q31_t) (((q63_t) fnext * (*pk++)) >> 31))));

      /* Output samples */
      /* y(n) += gN(n) * vN */
      acc += ((q63_t) gnext * *pv++);

      /* write gN-1(n-1) into state for next sample processing */
      *px2++ = gnext;

      /* Update f values for next coefficient processing */
      fcurr = fnext;

      tapCnt--;
    }

    /* y(n) += g0(n) * v0 */
    acc += ((q63_t) fnext * *pv++);

    *px2++ = fnext;

    /* write out into pDst */
    *pDst++ = (q31_t) (acc >> 31U);

    /* Advance the state pointer by 1 to process the next group of samples */
    pState = pState + 1U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete. Now copy last S->numStages samples to start of the buffer
     for the preperation of next frame process */

  /* Points to the start of the state buffer */
  pStateCur = &S->pState[0];
  pState = &S->pState[blockSize];

  tapCnt = numStages;

  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }


}

/**
  @} end of IIR_Lattice group
 */
