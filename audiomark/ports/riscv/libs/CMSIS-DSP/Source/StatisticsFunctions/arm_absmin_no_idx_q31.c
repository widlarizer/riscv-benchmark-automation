/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_absmin_no_idx_q31.c
 * Description:  Minimum value of absolute values of a Q31 vector
 *
 * $Date:        16 November 2021
 * $Revision:    V1.10.0
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

#include "dsp/statistics_functions.h"

/**
  @ingroup groupStats
 */


/**
  @addtogroup AbsMin
  @{
 */

/**
  @brief         Minimum value of absolute values of a Q31 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @return        none
 */

void arm_absmin_no_idx_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult)
{
        q31_t minVal, out;                             /* Temporary variables to store the output value. */
        uint32_t blkCnt;                     /* Loop counter */

  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
}
/**
  @} end of AbsMin group
 */
