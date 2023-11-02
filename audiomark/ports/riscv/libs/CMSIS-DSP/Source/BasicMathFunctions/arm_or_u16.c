/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_or_u16.c
 * Description:  uint16_t bitwise inclusive OR
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
  @defgroup Or Vector bitwise inclusive OR

  Compute the logical bitwise OR.

  There are separate functions for uint32_t, uint16_t, and uint8_t data types.
 */

/**
  @addtogroup Or
  @{
 */

/**
  @brief         Compute the logical bitwise OR of two fixed-point vectors.
  @param[in]     pSrcA      points to input vector A
  @param[in]     pSrcB      points to input vector B
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void arm_or_u16(
    const uint16_t * pSrcA,
    const uint16_t * pSrcB,
          uint16_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */

    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {
        *pDst++ = (*pSrcA++)|(*pSrcB++);

        /* Decrement the loop counter */
        blkCnt--;
    }
}

/**
  @} end of Or group
 */
