/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_mult_q15.c
 * Description:  Q15 matrix multiplication
 *
 * $Date:        3 Nov 2021
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

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixMult
  @{
 */

/**
  @brief         Q15 matrix multiplication.
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @param[in]     pState     points to the array for storing intermediate results
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator. The inputs to the
                   multiplications are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow.
                   The 34.30 result is then truncated to 34.15 format by discarding the low 15 bits
                   and then saturated to 1.15 format.
  @par
                   Refer to \ref arm_mat_mult_fast_q15() for a faster but less precise version of this function.
 */
arm_status arm_mat_mult_q15(
  const arm_matrix_instance_q15 * pSrcA,
  const arm_matrix_instance_q15 * pSrcB,
        arm_matrix_instance_q15 * pDst,
        q15_t                   * pState)
{
        q63_t sum;                                     /* Accumulator */


        q15_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
        q15_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
        q15_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A of Q15 type */
        q15_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B of Q15 type */
        q15_t *pOut = pDst->pData;                     /* Output data matrix pointer */
        q15_t *px;                                     /* Temporary output data matrix pointer */
        uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
        uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
        uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A    */
        uint32_t col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */
        arm_status status;                             /* Status of matrix multiplication */
        (void)pState;


  {
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
      px = pOut + i;

      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcB->pData;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initiate pointer pIn1 to point to starting address of pSrcA */
        pIn1 = pInA;

        /* Matrix A columns number of MAC operations are to be performed */
        colCnt = numColsA;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform multiply-accumulates */
          sum += (q31_t) * pIn1++ * *pIn2;
          pIn2 += numColsB;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert result from 34.30 to 1.15 format and store saturated value in destination buffer */

        /* Saturate and store result in destination buffer */
        *px++ = (q15_t) __SSAT((sum >> 15), 16);

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col);

      } while (col > 0U);

      /* Update pointer pSrcA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);


    /* Set status as ARM_MATH_SUCCESS */
    status = ARM_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
