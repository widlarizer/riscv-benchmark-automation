/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_trans_f64.c
 * Description:  Floating-point matrix transpose
 *
 * $Date:        10 August 2022
 * $Revision:    V1.9.1
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
  @defgroup MatrixTrans Matrix Transpose

  Tranposes a matrix.

  Transposing an <code>M x N</code> matrix flips it around the center diagonal and results in an <code>N x M</code> matrix.
  \image html MatrixTranspose.gif "Transpose of a 3 x 3 matrix"
 */

/**
  @addtogroup MatrixTrans
  @{
 */

/**
  @brief         Floating-point matrix transpose.
  @param[in]     pSrc      points to input matrix
  @param[out]    pDst      points to output matrix
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
 */
arm_status arm_mat_trans_f64(
    const arm_matrix_instance_f64 * pSrc,
    arm_matrix_instance_f64 * pDst)
{
    float64_t *pIn = pSrc->pData;                  /* input data matrix pointer */
    float64_t *pOut = pDst->pData;                 /* output data matrix pointer */
    float64_t *px;                                 /* Temporary output data matrix pointer */
    uint16_t nRows = pSrc->numRows;                /* number of rows */
    uint16_t nCols = pSrc->numCols;                /* number of columns */
    uint64_t col, row = nRows, i = 0U;             /* Loop counters */
    arm_status status;                             /* status of matrix transpose */
    
        
    {
        /* Matrix transpose by exchanging the rows with columns */
        /* row loop */
        do
        {
            /* Pointer px is set to starting address of column being processed */
            px = pOut + i;
            
            
            /* Initialize col with number of samples */
            col = nCols;
            
            
            while (col > 0U)
            {
                /* Read and store input element in destination */
                *px = *pIn++;
                
                /* Update pointer px to point to next row of transposed matrix */
                px += nRows;
                
                /* Decrement column loop counter */
                col--;
            }
            
            i++;
            
            /* Decrement row loop counter */
            row--;
            
        } while (row > 0U);          /* row loop end */
        
        /* Set status as ARM_MATH_SUCCESS */
        status = ARM_MATH_SUCCESS;
    }
    
    /* Return to application */
    return (status);
}
/**
 * @} end of MatrixTrans group
 */
