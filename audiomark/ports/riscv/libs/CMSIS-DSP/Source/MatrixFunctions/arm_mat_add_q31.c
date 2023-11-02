/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_add_q31.c
 * Description:  Q31 matrix addition
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
  @addtogroup MatrixAdd
  @{
 */

/**
  @brief         Q31 matrix addition.
  @param[in]     pSrcA      points to first input matrix structure
  @param[in]     pSrcB      points to second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q31 range [0x80000000 0x7FFFFFFF] are saturated.
 */
arm_status arm_mat_add_q31(
  const arm_matrix_instance_q31 * pSrcA,
  const arm_matrix_instance_q31 * pSrcB,
        arm_matrix_instance_q31 * pDst)
{
  q31_t *pInA = pSrcA->pData;                    /* input data matrix pointer A */
  q31_t *pInB = pSrcB->pData;                    /* input data matrix pointer B */
  q31_t *pOut = pDst->pData;                     /* output data matrix pointer */

  uint32_t numSamples;                           /* total number of elements in the matrix */
  uint32_t blkCnt;                               /* loop counters */
  arm_status status;                             /* status of matrix addition */


  {
    /* Total number of samples in input matrix */
    numSamples = (uint32_t) pSrcA->numRows * pSrcA->numCols;


    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;


    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) + B(m,n) */

      /* Add, saturate and store result in destination buffer. */
      *pOut++ = __QADD(*pInA++, *pInB++);

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
  @} end of MatrixAdd group
 */
