
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_boolean_distance.c
 * Description:  Templates for boolean distances
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




/**
 * @defgroup DISTANCEF Distance Functions
 *
 * Computes Distances between vectors. 
 *
 * Distance functions are useful in a lot of algorithms.
 *
 */


/**
 * @addtogroup DISTANCEF
 * @{
 */




#define _FUNC(A,B) A##B 

#define FUNC(EXT) _FUNC(arm_boolean_distance, EXT)

/**
 * @brief        Elements of boolean distances
 *
 * Different values which are used to compute boolean distances
 *
 * @param[in]    pA              First vector of packed booleans
 * @param[in]    pB              Second vector of packed booleans
 * @param[in]    numberOfBools   Number of booleans
 * @return None
 *
 */


void FUNC(EXT)(const uint32_t *pA
       , const uint32_t *pB
       , uint32_t numberOfBools
       )
{
  
    uint32_t a,b,ba,bb;
    int shift;

    while(numberOfBools >= 32)
    {
       a = *pA++;
       b = *pB++;
       shift = 0;
       while(shift < 32)
       {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;
          shift ++;
       }

       numberOfBools -= 32;
    }

    a = *pA++;
    b = *pB++;

    a = a >> (32 - numberOfBools);
    b = b >> (32 - numberOfBools);

    while(numberOfBools > 0)
    {
          ba = a & 1;
          bb = b & 1;
          a = a >> 1;
          b = b >> 1;

          numberOfBools --;
    }

}


/**
 * @} end of DISTANCEF group
 */
