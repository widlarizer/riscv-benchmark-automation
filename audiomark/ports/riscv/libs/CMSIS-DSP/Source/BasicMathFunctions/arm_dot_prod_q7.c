/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_dot_prod_q7.c
 * Description:  Q7 dot product
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
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief         Dot product of Q7 vectors.
  @param[in]     pSrcA      points to the first input vector
  @param[in]     pSrcB      points to the second input vector
  @param[in]     blockSize  number of samples in each vector
  @param[out]    result     output result returned here
  @return        none

  @par           Scaling and Overflow Behavior
                   The intermediate multiplications are in 1.7 x 1.7 = 2.14 format and these
                   results are added to an accumulator in 18.14 format.
                   Nonsaturating additions are used and there is no danger of wrap around as long as
                   the vectors are less than 2^18 elements long.
                   The return result is in 18.14 format.
 */

void arm_dot_prod_q7(
  const q7_t * pSrcA,
  const q7_t * pSrcB,
        uint32_t blockSize,
        q31_t * result)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t sum = 0;                                 /* Temporary return variable */


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

    /* Calculate dot product and store result in a temporary buffer. */
//#if defined (ARM_MATH_DSP)
//    sum  = __SMLAD(*pSrcA++, *pSrcB++, sum);
//#else
    sum += (q31_t) ((q15_t) *pSrcA++ * *pSrcB++);
//#endif

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store result in destination buffer in 18.14 format */
  *result = sum;
}

/**
  @} end of BasicDotProd group
 */
