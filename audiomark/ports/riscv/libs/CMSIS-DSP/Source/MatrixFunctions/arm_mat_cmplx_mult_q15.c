/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_mat_mult_q15.c
 * Description:  Q15 complex matrix multiplication
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
  @addtogroup CmplxMatrixMult
  @{
 */

/**
  @brief         Q15 Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @param[in]     pScratch   points to an array for storing intermediate results
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Conditions for optimum performance
                   Input, output and state buffers should be aligned by 32-bit

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator. The inputs to the
                   multiplications are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow. The 34.30 result is then
                   truncated to 34.15 format by discarding the low 15 bits and then saturated to 1.15 format.
 */
arm_status arm_mat_cmplx_mult_q15(
  const arm_matrix_instance_q15 * pSrcA,
  const arm_matrix_instance_q15 * pSrcB,
        arm_matrix_instance_q15 * pDst,
        q15_t                   * pScratch)
{
        q15_t *pSrcBT = pScratch;                      /* input data matrix pointer for transpose */
        q15_t *pInA = pSrcA->pData;                    /* input data matrix pointer A of Q15 type */
        q15_t *pInB = pSrcB->pData;                    /* input data matrix pointer B of Q15 type */
        q15_t *px;                                     /* Temporary output data matrix pointer */
        uint16_t numRowsA = pSrcA->numRows;            /* number of rows of input matrix A */
        uint16_t numColsB = pSrcB->numCols;            /* number of columns of input matrix B */
        uint16_t numColsA = pSrcA->numCols;            /* number of columns of input matrix A */
        uint16_t numRowsB = pSrcB->numRows;            /* number of rows of input matrix A */
        q63_t sumReal, sumImag;                        /* accumulator */
        uint32_t col, i = 0U, row = numRowsB, colCnt;  /* Loop counters */
        arm_status status;                             /* Status of matrix multiplication */

        q15_t a, b, c, d;


  {
    /* Matrix transpose */
    do
    {
      /* The pointer px is set to starting address of column being processed */
      px = pSrcBT + i;


        /* Initialize blkCnt with number of samples */
        col = numColsB;


      while (col > 0U)
      {
        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Decrement column loop counter */
        col--;
      }

      i = i + 2U;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Reset variables for usage in following multiplication process */
    row = numRowsA;
    i = 0U;
    px = pDst->pData;

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of transposed pSrcB data */
      pInB = pSrcBT;

      /* column loop */
      do
      {
        /* Set variable sum, that acts as accumulator, to zero */
        sumReal = 0;
        sumImag = 0;

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA->pData + i * 2;

        /* Apply loop unrolling and compute 2 MACs simultaneously. */
        colCnt = numColsA >> 1U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */


          /* read real and imag values from pSrcA buffer */
          a = *pInA;
          b = *(pInA + 1U);
          /* read real and imag values from pSrcB buffer */
          c = *pInB;
          d = *(pInB + 1U);

          /* Multiply and Accumlates */
          sumReal += (q31_t) a *c;
          sumImag += (q31_t) a *d;
          sumReal -= (q31_t) b *d;
          sumImag += (q31_t) b *c;

          /* read next real and imag values from pSrcA buffer */
          a = *(pInA + 2U);
          b = *(pInA + 3U);
          /* read next real and imag values from pSrcB buffer */
          c = *(pInB + 2U);
          d = *(pInB + 3U);

          /* update pointer */
          pInA += 4U;

          /* Multiply and Accumlates */
          sumReal += (q31_t) a * c;
          sumImag += (q31_t) a * d;
          sumReal -= (q31_t) b * d;
          sumImag += (q31_t) b * c;
          /* update pointer */
          pInB += 4U;


          /* Decrement loop counter */
          colCnt--;
        }

        /* process odd column samples */
        if ((numColsA & 0x1U) > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */


          /* read real and imag values from pSrcA and pSrcB buffer */
          a = *pInA++;
          b = *pInA++;
          c = *pInB++;
          d = *pInB++;

          /* Multiply and Accumlates */
          sumReal += (q31_t) a * c;
          sumImag += (q31_t) a * d;
          sumReal -= (q31_t) b * d;
          sumImag += (q31_t) b * c;


        }

        /* Saturate and store result in destination buffer */
        *px++ = (q15_t) (__SSAT(sumReal >> 15, 16));
        *px++ = (q15_t) (__SSAT(sumImag >> 15, 16));

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      i = i + numColsA;

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
