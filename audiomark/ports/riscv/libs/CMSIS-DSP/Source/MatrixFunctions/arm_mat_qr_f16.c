/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_qr_f16.c
 * Description:  Half floating-point matrix QR decomposition.
 *
 * $Date:        15 June 2022
 * $Revision:    V1.11.0
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

#include "dsp/matrix_functions_f16.h"
#include "dsp/matrix_utils.h"



/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixQR
  @{
 */

/**
  @brief         QR decomposition of a m x n half floating point matrix with m >= n.
  @param[in]     pSrc      points to input matrix structure. The source matrix is modified by the function.
  @param[in]     threshold norm2 threshold.    
  @param[out]    pOutR     points to output R matrix structure of dimension m x n
  @param[out]    pOutQ     points to output Q matrix structure of dimension m x m (can be NULL)
  @param[out]    pOutTau   points to Householder scaling factors of dimension n
  @param[inout]  pTmpA     points to a temporary vector of dimension m.
  @param[inout]  pTmpB     points to a temporary vector of dimension m.
  @return        execution status
                   - \ref ARM_MATH_SUCCESS       : Operation successful
                   - \ref ARM_MATH_SIZE_MISMATCH : Matrix size check failed
  
  @par           pOutQ is optional:
                 pOutQ can be a NULL pointer.
                 In this case, the argument will be ignored
                 and the output Q matrix won't be computed.

  @par           f16 implementation
                 The f16 implementation is not very accurate.

  @par           Norm2 threshold 
                 For the meaning of this argument please 
                 refer to the \ref MatrixHouseholder documentation

 */





/**
  @} end of MatrixQR group
 */
