/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mse_q7.c
 * Description:  Mean square error between two Q7 vectors
 *
 * $Date:        04 April 2022
 * $Revision:    V1.10.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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
  @defgroup MSE Mean Square Error

  Calculates the mean square error between two vectors.

 */

/**
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two Q7 vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult     mean square error
  @return        none
 */
void arm_mse_q7(
  const q7_t * pSrcA,
  const q7_t * pSrcB,
        uint32_t blockSize,
        q7_t * pResult)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t sum = 0;                                 /* Temporary result storage */
        q7_t inA,inB;                                       /* Temporary variable to store input value */



  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;

    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store result in q7 format */
  *pResult = (q7_t) __SSAT((q15_t) (sum / blockSize)>>5, 8);;
}

/**
  @} end of MSE group
 */
