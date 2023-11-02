
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_braycurtis_distance_f32.c
 * Description:  Bray-Curtis distance between two vectors
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>



/**
  @addtogroup braycurtis
  @{
 */


/**
 * @brief        Bray-Curtis distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
float32_t arm_braycurtis_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize)
{
   float32_t accumDiff=0.0f, accumSum=0.0f, tmpA, tmpB;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      accumDiff += fabsf(tmpA - tmpB);
      accumSum += fabsf(tmpA + tmpB);
      blockSize --;
   }
   /*

   It is assumed that accumSum is not zero. Since it is the sum of several absolute
   values it would imply that all of them are zero. It is very unlikely for long vectors.

   */
   return(accumDiff / accumSum);
}


/**
 * @} end of braycurtis group
 */

