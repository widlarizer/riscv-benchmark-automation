/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_scale_q15.c
 * Description:  Multiplies a Q15 matrix by a scalar
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

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixScale
  @{
 */

/**
  @brief         Q15 matrix scaling.
  @param[in]     pSrc        points to input matrix
  @param[in]     scaleFract  fractional portion of the scale factor
  @param[in]     shift       number of bits to shift the result by
  @param[out]    pDst        points to output matrix structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.15 format.
                   These are multiplied to yield a 2.30 intermediate result and this is shifted with saturation to 1.15 format.
 */
arm_status arm_mat_scale_q15(
  const arm_matrix_instance_q15 * pSrc,
        q15_t                     scaleFract,
        int32_t                   shift,
        arm_matrix_instance_q15 * pDst)
{
        q15_t *pIn = pSrc->pData;                      /* Input data matrix pointer */
        q15_t *pOut = pDst->pData;                     /* Output data matrix pointer */
        uint32_t numSamples;                           /* Total number of elements in the matrix */
        uint32_t blkCnt;                               /* Loop counter */
        arm_status status;                             /* Status of matrix scaling */
        int32_t kShift = 15 - shift;                   /* Total shift to apply after scaling */



  {
    /* Total number of samples in input matrix */
    numSamples = (uint32_t) pSrc->numRows * pSrc->numCols;


    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;


    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * k */

      /* Scale, saturate and store result in destination buffer. */
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixScale group
 */
