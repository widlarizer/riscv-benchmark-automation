/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_scale_q31.c
 * Description:  Multiplies a Q31 vector by a scalar
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
  @brief         Multiplies a Q31 vector by a scalar.
  @param[in]     pSrc       points to the input vector
  @param[in]     scaleFract fractional portion of the scale value
  @param[in]     shift      number of bits to shift the result by
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.31 format.
                   These are multiplied to yield a 2.62 intermediate result and this is shifted 
                   with saturation to 1.31 format.
                   There is an intermediate shift by 32 to go from the
                   2.62 to 1.31 format. 
                   The shift argument is applied on the 1.31 result and not to the intermediate
                   2.62 format. 
 */

void arm_scale_q31(
  const q31_t *pSrc,
        q31_t scaleFract,
        int8_t shift,
        q31_t *pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t in, out;                                 /* Temporary variables */
        int8_t kShift = shift + 1;                     /* Shift to apply after scaling */
        int8_t sign = (kShift & 0x80);


  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;


  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A * scale */

      /* Scale input and store result in destination buffer. */
      in = *pSrc++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in << kShift;
      if (in != (out >> kShift))
          out = 0x7FFFFFFF ^ (in >> 31);
      *pDst++ = out;

      /* Decrement loop counter */
      blkCnt--;
    }
  }
  else
  {
    while (blkCnt > 0U)
    {
      /* C = A * scale */

      /* Scale input and store result in destination buffer. */
      in = *pSrc++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in >> -kShift;
      *pDst++ = out;

      /* Decrement loop counter */
      blkCnt--;
    }
  }

}

/**
  @} end of BasicScale group
 */
