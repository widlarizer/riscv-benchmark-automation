/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mse_f32.c
 * Description:  Floating point mean square error
 *
 * $Date:        05 April 2022
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
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two floating point vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult      mean square error
  @return        none
 */









void arm_mse_f32(
    const float32_t * pSrcA,
    const float32_t * pSrcB,
    uint32_t    blockSize,
    float32_t * pResult)

{
  uint32_t blkCnt;                               /* Loop counter */
  float32_t inA, inB;
  float32_t sum = 0.0f;                          /* Temporary return variable */
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
  while (blkCnt > 0U)
  {
    inA = *pSrcA++; 
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store result in destination buffer */
  *pResult = sum / blockSize;
}


/**
  @} end of MSE group
 */
