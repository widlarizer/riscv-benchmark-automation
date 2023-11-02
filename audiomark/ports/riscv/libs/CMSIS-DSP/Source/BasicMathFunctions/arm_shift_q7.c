/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_shift_q7.c
 * Description:  Processing function for the Q7 Shifting
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
  @addtogroup BasicShift
  @{
 */

/**
  @brief         Shifts the elements of a Q7 vector a specified number of bits
  @param[in]     pSrc       points to the input vector
  @param[in]     shiftBits  number of bits to shift.  A positive value shifts left; a negative value shifts right.
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           onditions for optimum performance
                   Input and output buffers should be aligned by 32-bit
  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q7 range [0x80 0x7F] are saturated.
 */

void arm_shift_q7(
  const q7_t * pSrc,
        int8_t shiftBits,
        q7_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
        uint8_t sign = (shiftBits & 0x80);             /* Sign of shiftBits */


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  /* If the shift value is positive then do right shift else left shift */
  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A << shiftBits */

      /* Shift input and store result in destination buffer. */
      *pDst++ = (q7_t) __SSAT(((q15_t) *pSrc++ << shiftBits), 8);

      /* Decrement loop counter */
      blkCnt--;
    }
  }
  else
  {
    while (blkCnt > 0U)
    {
      /* C = A >> shiftBits */

      /* Shift input and store result in destination buffer. */
      *pDst++ = (*pSrc++ >> -shiftBits);

      /* Decrement loop counter */
      blkCnt--;
    }
  }

}

/**
  @} end of BasicShift group
 */
