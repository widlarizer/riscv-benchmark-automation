/******************************************************************************
 * @file     matrix_utils.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.11.0
 * @date     30 May 2022
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2022 Arm Limited or its affiliates. All rights reserved.
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

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"


#define ELEM(A,ROW,COL) &((A)->pData[(A)->numCols* (ROW) + (COL)])

#define SCALE_COL_T(T,CAST,A,ROW,v,i)        \
{                                       \
  int32_t w;                            \
  T *data = (A)->pData;                 \
  const int32_t numCols = (A)->numCols; \
  const int32_t nb = (A)->numRows - ROW;\
                                        \
  data += i + numCols * (ROW);          \
                                        \
  for(w=0;w < nb; w++)                  \
  {                                     \
     *data *= CAST v;                   \
     data += numCols;                   \
  }                                     \
}

#define COPY_COL_T(T,A,ROW,COL,DST)               \
{                                                 \
    uint32_t row;                                 \
    T *pb=DST;                                    \
    T *pa = (A)->pData + ROW * (A)->numCols + COL;\
    for(row = ROW; row < (A)->numRows; row ++)    \
    {                                             \
         *pb++ = *pa;                             \
         pa += (A)->numCols;                      \
    }                                             \
}



#define SWAP_ROWS_F32(A,COL,i,j)       \
{                                      \
  int32_t w;                           \
  float32_t tmp;                       \
  float32_t *dataI = (A)->pData;       \
  float32_t *dataJ = (A)->pData;       \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols - COL;    \
                                       \
  dataI += i*numCols + (COL);          \
  dataJ += j*numCols + (COL);          \
                                       \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define SCALE_ROW_F32(A,COL,v,i)       \
{                                      \
  int32_t w;                           \
  float32_t *data = (A)->pData;        \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols - COL;    \
                                       \
  data += i*numCols + (COL);           \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     *data++ *= v;                     \
  }                                    \
}


#define MAC_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  int32_t w;                           \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);    \
                                       \
  dataA = dataA + i*numCols + (COL);   \
  dataB = dataB + j*numCols + (COL);   \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     *dataA++ += v* *dataB++;          \
  }                                    \
}

#define MAS_ROW_F32(COL,A,i,v,B,j)     \
{                                      \
  int32_t w;                           \
  float32_t *dataA = (A)->pData;       \
  float32_t *dataB = (B)->pData;       \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);    \
                                       \
  dataA = dataA + i*numCols + (COL);   \
  dataB = dataB + j*numCols + (COL);   \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     *dataA++ -= v* *dataB++;          \
  }                                    \
}



/* Functions with only a scalar version */

#define COPY_COL_F32(A,ROW,COL,DST) \
  COPY_COL_T(float32_t,A,ROW,COL,DST)

#define COPY_COL_F64(A,ROW,COL,DST) \
  COPY_COL_T(float64_t,A,ROW,COL,DST)

#define SWAP_COLS_F32(A,COL,i,j)               \
{                                              \
  int32_t w;                                  \
  float32_t *data = (A)->pData;                \
  const int32_t numCols = (A)->numCols;       \
  for(w=(COL);w < numCols; w++)                \
  {                                            \
     float32_t tmp;                            \
     tmp = data[w*numCols + i];                \
     data[w*numCols + i] = data[w*numCols + j];\
     data[w*numCols + j] = tmp;                \
  }                                            \
}

#define SCALE_COL_F32(A,ROW,v,i)        \
  SCALE_COL_T(float32_t,,A,ROW,v,i)

#define SWAP_ROWS_F64(A,COL,i,j)       \
{                                      \
  int32_t w;                           \
  float64_t *dataI = (A)->pData;       \
  float64_t *dataJ = (A)->pData;       \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);    \
                                       \
  dataI += i*numCols + (COL);          \
  dataJ += j*numCols + (COL);          \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     float64_t tmp;                    \
     tmp = *dataI;                     \
     *dataI++ = *dataJ;                \
     *dataJ++ = tmp;                   \
  }                                    \
}

#define SWAP_COLS_F64(A,COL,i,j)               \
{                                              \
  int32_t w;                                  \
  float64_t *data = (A)->pData;                \
  const int32_t numCols = (A)->numCols;       \
  for(w=(COL);w < numCols; w++)                \
  {                                            \
     float64_t tmp;                            \
     tmp = data[w*numCols + i];                \
     data[w*numCols + i] = data[w*numCols + j];\
     data[w*numCols + j] = tmp;                \
  }                                            \
}

#define SCALE_ROW_F64(A,COL,v,i)       \
{                                      \
  int32_t w;                           \
  float64_t *data = (A)->pData;        \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);    \
                                       \
  data += i*numCols + (COL);           \
                                       \
  for(w=0;w < nb; w++)                 \
  {                                    \
     *data++ *= v;                     \
  }                                    \
}

#define SCALE_COL_F64(A,ROW,v,i)        \
  SCALE_COL_T(float64_t,,A,ROW,v,i)

#define MAC_ROW_F64(COL,A,i,v,B,j)      \
{                                       \
  int32_t w;                           \
  float64_t *dataA = (A)->pData;        \
  float64_t *dataB = (B)->pData;        \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);     \
                                        \
  dataA += i*numCols + (COL);           \
  dataB += j*numCols + (COL);           \
                                        \
  for(w=0;w < nb; w++)                  \
  {                                     \
     *dataA++ += v* *dataB++;           \
  }                                     \
}

#define MAS_ROW_F64(COL,A,i,v,B,j)      \
{                                       \
  int32_t w;                           \
  float64_t *dataA = (A)->pData;        \
  float64_t *dataB = (B)->pData;        \
  const int32_t numCols = (A)->numCols;\
  const int32_t nb = numCols-(COL);     \
                                        \
  dataA += i*numCols + (COL);           \
  dataB += j*numCols + (COL);           \
                                        \
  for(w=0;w < nb; w++)                  \
  {                                     \
     *dataA++ -= v* *dataB++;           \
  }                                     \
}


