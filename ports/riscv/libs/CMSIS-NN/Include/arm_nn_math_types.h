/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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

/******************************************************************************
 * @file     arm_nn_math_types.h
 * @brief    Compiler include and basic types
 * @version  V1.2.1
 * @date     26 October 2022
 * Target Processor: Cortex-M
 ******************************************************************************/

/**
   Copied from CMSIS/DSP/arm_math_types.h and modified
*/


#pragma once


#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

/* Compiler specific diagnostic adjustment */


/* Included for instrinsics definitions */
#define __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline))
#define __STATIC_INLINE static inline


/* evaluate ARM DSP feature */


/* Compiler specific diagnostic adjustment */
// #pragma GCC diagnostic pop





/**
 * @brief Add necessary typedefs
 */

#define NN_Q31_MAX ((int32_t)(0x7FFFFFFFL))
#define NN_Q15_MAX ((int16_t)(0x7FFF))
#define NN_Q7_MAX ((int8_t)(0x7F))
#define NN_Q31_MIN ((int32_t)(0x80000000L))
#define NN_Q15_MIN ((int16_t)(0x8000))
#define NN_Q7_MIN ((int8_t)(0x80))

/**
 * @brief Error status returned by some functions in the library.
 */

typedef enum
{
    ARM_CMSIS_NN_SUCCESS = 0,        /**< No error */
    ARM_CMSIS_NN_ARG_ERROR = -1,     /**< One or more arguments are incorrect */
    ARM_CMSIS_NN_NO_IMPL_ERROR = -2, /**<  No implementation available */
} arm_cmsis_nn_status;



