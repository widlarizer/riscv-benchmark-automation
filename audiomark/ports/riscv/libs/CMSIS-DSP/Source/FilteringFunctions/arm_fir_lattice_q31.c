/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_lattice_q31.c
 * Description:  Q31 FIR lattice filter processing function
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
  @addtogroup FIR_Lattice
  @{
 */

/**
  @brief         Processing function for the Q31 FIR lattice filter.
  @param[in]     S          points to an instance of the Q31 FIR lattice structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   In order to avoid overflows the input signal must be scaled down by 2*log2(numStages) bits.
 */

void arm_fir_lattice_q31(
  const arm_fir_lattice_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *px;                                     /* Temporary state pointer */
  const q31_t *pk;                                     /* Temporary coefficient pointer */
        uint32_t numStages = S->numStages;             /* Number of stages in the filter */
        uint32_t blkCnt, stageCnt;                     /* Loop counters */
        q31_t fcurr0, fnext0, gnext0, gcurr0;          /* Temporary variables */

//#if !defined(ARM_MATH_CM0_FAMILY)


  gcurr0 = 0;


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* f0(n) = x(n) */
    fcurr0 = *pSrc++;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pk = pCoeffs;

    /* read g2(n) from state buffer */
    gcurr0 = *px;

    /* for sample 1 processing */
    /* f1(n) = f0(n) +  K1 * g0(n-1) */
    fnext0 = (q31_t) (((q63_t) gcurr0 * (*pk)) >> 32U);
    fnext0 = (fnext0 << 1U) + fcurr0;

    /* g1(n) = f0(n) * K1  +  g0(n-1) */
    gnext0 = (q31_t) (((q63_t) fcurr0 * (*pk++)) >> 32U);
    gnext0 = (gnext0 << 1U) + gcurr0;

    /* save g1(n) in state buffer */
    *px++ = fcurr0;

    /* f1(n) is saved in fcurr0 for next stage processing */
    fcurr0 = fnext0;

    stageCnt = (numStages - 1U);

    /* stage loop */
    while (stageCnt > 0U)
    {
      /* read g2(n) from state buffer */
      gcurr0 = *px;

      /* save g1(n) in state buffer */
      *px++ = gnext0;

      /* Sample processing for K2, K3.... */
      /* f2(n) = f1(n) +  K2 * g1(n-1) */
      fnext0 = (q31_t) (((q63_t) gcurr0 * (*pk)) >> 32U);
      fnext0 = (fnext0 << 1U) + fcurr0;

      /* g2(n) = f1(n) * K2  +  g1(n-1) */
      gnext0 = (q31_t) (((q63_t) fcurr0 * (*pk++)) >> 32U);
      gnext0 = (gnext0 << 1U) + gcurr0;

      /* f1(n) is saved in fcurr0 for next stage processing */
      fcurr0 = fnext0;

      stageCnt--;
    }

    /* y(n) = fN(n) */
    *pDst++ = fcurr0;

    blkCnt--;
  }


}

/**
  @} end of FIR_Lattice group
 */
