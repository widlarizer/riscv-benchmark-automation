/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_mag_fast_q15.c
 * Description:  Q15 complex magnitude
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

#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup cmplx_mag
  @{
 */

/**
  @brief         Q15 complex magnitude.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.15 by 1.15 multiplications and finally output is converted into 2.14 format.
                   Fast functions are less accurate. This function will tend to clamp to 0
                   the too small values. So sqrt(x*x) = x will not always be true.
 */
void arm_cmplx_mag_fast_q15(
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */

       q15_t real, imag;                              /* Temporary input variables */
       q31_t acc0, acc1;                              /* Accumulators */


  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;


  while (blkCnt > 0U)
  {
    /* C[0] = sqrt(A[0] * A[0] + A[1] * A[1]) */

    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);

    /* store result in 2.14 format in destination buffer. */
    arm_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of cmplx_mag group
 */
