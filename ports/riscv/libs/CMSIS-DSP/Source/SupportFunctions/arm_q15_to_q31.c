/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_q15_to_q31.c
 * Description:  Converts the elements of the Q15 vector to Q31 vector
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

#include "dsp/support_functions.h"

/**
  @ingroup groupSupport
 */

/**
  @addtogroup q15_to_x
  @{
 */

/**
  @brief         Converts the elements of the Q15 vector to Q31 vector.
  @param[in]     pSrc       points to the Q15 input vector
  @param[out]    pDst       points to the Q31 output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Details
                   The equation used for the conversion process is:
  <pre>
      pDst[n] = (q31_t) pSrc[n] << 16;   0 <= n < blockSize.
  </pre>
 */
void arm_q15_to_q31(
  const q15_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
  const q15_t *pIn = pSrc;                             /* Source pointer */



  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* C = (q31_t) A << 16 */

    /* Convert from q15 to q31 and store result in destination buffer */
    *pDst++ = (q31_t) *pIn++ << 16;

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of q15_to_x group
 */
