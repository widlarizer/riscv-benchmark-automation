/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_scale_q7.c
 * Description:  Multiplies a Q7 vector by a scalar
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicScale
  @{
 */

/**
  @brief         Multiplies a Q7 vector by a scalar.
  @param[in]     pSrc       points to the input vector
  @param[in]     scaleFract fractional portion of the scale value
  @param[in]     shift      number of bits to shift the result by
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.7 format.
                   These are multiplied to yield a 2.14 intermediate result and this is shifted with saturation to 1.7 format.
 */

void arm_scale_q7(
  const q7_t * pSrc,
        q7_t scaleFract,
        int8_t shift,
        q7_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
        int8_t kShift = 7 - shift;                     /* Shift to apply after scaling */


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* C = A * scale */

    /* Scale input and store result in destination buffer. */
    *pDst++ = (q7_t) (__SSAT((((q15_t) *pSrc++ * scaleFract) >> kShift), 8));

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of BasicScale group
 */
