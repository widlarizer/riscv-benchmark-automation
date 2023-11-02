/******************************************************************************
 * @file     arm_sorting.h
 * @brief    Private header file for CMSIS DSP Library
 * @version  V1.7.0
 * @date     2019
 ******************************************************************************/
/*
 * Copyright (c) 2010-2019 Arm Limited or its affiliates. All rights reserved.
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

#pragma once

#include "arm_math.h"


  /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data.
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_bubble_sort_f32(
    const arm_sort_instance_f32 * S, 
          float32_t * pSrc, 
          float32_t * pDst, 
    uint32_t blockSize);

   /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data.
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_heap_sort_f32(
    const arm_sort_instance_f32 * S, 
          float32_t * pSrc, 
          float32_t * pDst, 
    uint32_t blockSize);

  /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data.
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_insertion_sort_f32(
    const arm_sort_instance_f32 * S, 
          float32_t *pSrc, 
          float32_t* pDst, 
    uint32_t blockSize);

  /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_quick_sort_f32(
    const arm_sort_instance_f32 * S, 
          float32_t * pSrc, 
          float32_t * pDst, 
    uint32_t blockSize);

  /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_selection_sort_f32(
    const arm_sort_instance_f32 * S, 
          float32_t * pSrc, 
          float32_t * pDst, 
    uint32_t blockSize);
 
  /**
   * @param[in]  S          points to an instance of the sorting structure.
   * @param[in]  pSrc       points to the block of input data.
   * @param[out] pDst       points to the block of output data
   * @param[in]  blockSize  number of samples to process.
   */
  void arm_bitonic_sort_f32(
    const arm_sort_instance_f32 * S,
          float32_t * pSrc,
          float32_t * pDst,
          uint32_t blockSize);



