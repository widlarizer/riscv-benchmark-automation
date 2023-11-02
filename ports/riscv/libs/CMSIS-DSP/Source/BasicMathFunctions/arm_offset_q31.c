/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_offset_q31.c
 * Description:  Q31 vector offset
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
  @addtogroup BasicOffset
  @{
 */

/**
  @brief         Adds a constant offset to a Q31 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     offset     is the offset to be added
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q31 range [0x80000000 0x7FFFFFFF] are saturated.
 */

void arm_offset_q31(
  const q31_t * pSrc,
        q31_t offset,
        q31_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* C = A + offset */

    /* Add offset and store result in destination buffer. */
    *pDst++ = (q31_t) clip_q63_to_q31((q63_t) * pSrc++ + offset);

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of BasicOffset group
 */
