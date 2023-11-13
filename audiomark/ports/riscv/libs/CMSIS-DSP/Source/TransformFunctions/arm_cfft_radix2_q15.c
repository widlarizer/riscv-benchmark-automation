/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_radix2_q15.c
 * Description:  Radix-2 Decimation in Frequency CFFT & CIFFT Fixed point processing function
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

#include "dsp/transform_functions.h"

void arm_radix2_butterfly_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint16_t twidCoefModifier);

void arm_radix2_butterfly_inverse_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint16_t twidCoefModifier);

void arm_bitreversal_q15(
        q15_t * pSrc,
        uint32_t fftLen,
        uint16_t bitRevFactor,
  const uint16_t * pBitRevTab);

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @brief         Processing function for the fixed-point CFFT/CIFFT.
  @deprecated    Do not use this function. It has been superseded by \ref arm_cfft_q15 and will be removed in the future.
  @param[in]     S    points to an instance of the fixed-point CFFT/CIFFT structure
  @param[in,out] pSrc points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @return        none
 */

void arm_cfft_radix2_q15(
  const arm_cfft_radix2_instance_q15 * S,
        q15_t * pSrc)
{

  if (S->ifftFlag == 1U)
  {
    arm_radix2_butterfly_inverse_q15 (pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
  }
  else
  {
    arm_radix2_butterfly_q15 (pSrc, S->fftLen, S->pTwiddle, S->twidCoefModifier);
  }

  arm_bitreversal_q15(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
}

/**
  @} end of ComplexFFT group
 */

void arm_radix2_butterfly_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint16_t twidCoefModifier)
{

  uint32_t i, j, k, l;
  uint32_t n1, n2, ia;
  q15_t xt, yt, cosVal, sinVal;


  // N = fftLen;
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  /* loop for groups */
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[(ia * 2)];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    /* loop for butterfly */
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
      pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

      yt = (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
      pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1U) +
                         (pSrc[2 * i + 1] >> 1U)  ) >> 1U;

      pSrc[2 * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) +
                     ((int16_t) (((q31_t) yt * sinVal) >> 16)));

      pSrc[2U * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) -
                          ((int16_t) (((q31_t) xt * sinVal) >> 16)));

    } /* butterfly loop end */

  } /* groups loop end */

  twidCoefModifier = twidCoefModifier << 1U;

  /* loop for stage */
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    /* loop for groups */
    for (j = 0; j < n2; j++)
    {
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      /* loop for butterfly */
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1U;

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1U;

        pSrc[2 * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) +
                       ((int16_t) (((q31_t) yt * sinVal) >> 16)));

        pSrc[2U * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) -
                            ((int16_t) (((q31_t) xt * sinVal) >> 16)));

      } /* butterfly loop end */

    } /* groups loop end */

    twidCoefModifier = twidCoefModifier << 1U;
  } /* stages loop end */

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  /* loop for groups */
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[ia * 2];
    sinVal = pCoef[(ia * 2) + 1];

    ia = ia + twidCoefModifier;

    /* loop for butterfly */
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2 * l] = xt;

      pSrc[2 * l + 1] = yt;

    } /* butterfly loop end */

  } /* groups loop end */

  twidCoefModifier = twidCoefModifier << 1U;


}


void arm_radix2_butterfly_inverse_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint16_t twidCoefModifier)
{

        uint32_t i, j, k, l;
        uint32_t n1, n2, ia;
        q15_t xt, yt, cosVal, sinVal;

  // N = fftLen;
  n2 = fftLen;

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  /* loop for groups */
  for (j = 0; j < n2; j++)
  {
    cosVal = pCoef[(ia * 2)];
    sinVal = pCoef[(ia * 2) + 1];
    ia = ia + twidCoefModifier;

    /* loop for butterfly */
    for (i = j; i < fftLen; i += n1)
    {
      l = i + n2;
      xt = (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
      pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

      yt = (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
      pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1U) +
                         (pSrc[2 * i + 1] >> 1U)  ) >> 1U;

      pSrc[2 * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) -
                     ((int16_t) (((q31_t) yt * sinVal) >> 16)));

      pSrc[2 * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) +
                         ((int16_t) (((q31_t) xt * sinVal) >> 16)));

    } /* butterfly loop end */

  } /* groups loop end */

  twidCoefModifier = twidCoefModifier << 1U;

  /* loop for stage */
  for (k = fftLen / 2; k > 2; k = k >> 1)
  {
    n1 = n2;
    n2 = n2 >> 1;
    ia = 0;

    /* loop for groups */
    for (j = 0; j < n2; j++)
    {
      cosVal = pCoef[(ia * 2)];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      /* loop for butterfly */
      for (i = j; i < fftLen; i += n1)
      {
        l = i + n2;
        xt = pSrc[2 * i] - pSrc[2 * l];
        pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1U;

        yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
        pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1U;

        pSrc[2 * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) -
                       ((int16_t) (((q31_t) yt * sinVal) >> 16))  );

        pSrc[2 * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) +
                           ((int16_t) (((q31_t) xt * sinVal) >> 16))  );

      } /* butterfly loop end */

    } /* groups loop end */

    twidCoefModifier = twidCoefModifier << 1U;
  } /* stages loop end */

  n1 = n2;
  n2 = n2 >> 1;
  ia = 0;

  cosVal = pCoef[(ia * 2)];
  sinVal = pCoef[(ia * 2) + 1];

  ia = ia + twidCoefModifier;

  /* loop for butterfly */
  for (i = 0; i < fftLen; i += n1)
  {
    l = i + n2;
    xt = pSrc[2 * i] - pSrc[2 * l];
    pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

    yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
    pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

    pSrc[2 * l] = xt;

    pSrc[2 * l + 1] = yt;

  } /* groups loop end */



}
