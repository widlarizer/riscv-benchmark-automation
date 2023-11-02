/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_float_to_q31.c
 * Description:  Converts the elements of the floating-point vector to Q31 vector
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
 * @defgroup float_to_x  Convert 32-bit floating point value
 */

/**
  @addtogroup float_to_x
  @{
 */

/**
  @brief         Converts the elements of the floating-point vector to Q31 vector.
  @param[in]     pSrc       points to the floating-point input vector
  @param[out]    pDst       points to the Q31 output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Details
                   The equation used for the conversion process is:
  <pre>
      pDst[n] = (q31_t)(pSrc[n] * 2147483648);   0 <= n < blockSize.
  </pre>

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q31 range[0x80000000 0x7FFFFFFF] are saturated.

  @note
                   In order to apply rounding, the library should be rebuilt with the ROUNDING macro
                   defined in the preprocessor section of project options.
 */

void arm_float_to_q31(
  const float32_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
  const float32_t *pIn = pSrc;                         /* Source pointer */



  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  while (blkCnt > 0U)
  {
    /* C = A * 2147483648 */

    /* convert from float to Q31 and store result in destination buffer */

    /* C = A * 2147483648 */
    /* Convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));


    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of float_to_x group
 */
