/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_householder_f16.c
 * Description:  Half floating-point Householder transform
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
#include "dsp/basic_math_functions_f16.h"
#include "dsp/fast_math_functions_f16.h"

#include "dsp/matrix_utils.h"


#include <math.h>



/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixHouseholder
  @{
 */

/**
  @brief         Householder transform of a half floating point vector.
  @param[in]     pSrc        points to the input vector.
  @param[in]     threshold   norm2 threshold.  
  @param[in]     blockSize   dimension of the vector space.
  @param[out]    pOut        points to the output vector.
  @return        beta        return the scaling factor beta
 */


/**
  @} end of MatrixHouseholder group
 */
