/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_init_q15.c
 * Description:  RFFT & RIFFT Q15 initialisation function
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
#include "arm_common_tables.h"
#include "arm_const_structs.h"

/**
  @addtogroup RealFFT
  @{
 */

/**
  @brief         Initialization function for the Q15 RFFT/RIFFT.
  @param[in,out] S              points to an instance of the Q15 RFFT/RIFFT structure
  @param[in]     fftLenReal     length of the FFT
  @param[in]     ifftFlagR      flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLenReal</code> is not a supported length

  @par           Details
                   The parameter <code>fftLenReal</code> specifies length of RFFT/RIFFT Process.
                   Supported FFT Lengths are 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192.
  @par
                   The parameter <code>ifftFlagR</code> controls whether a forward or inverse transform is computed.
                   Set(=1) ifftFlagR to calculate RIFFT, otherwise RFFT is calculated.
  @par
                   The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
                   Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
  @par
                   This function also initializes Twiddle factor table.
 */

arm_status arm_rfft_init_q15(
    arm_rfft_instance_q15 * S,
    uint32_t fftLenReal,
    uint32_t ifftFlagR,
    uint32_t bitReverseFlag)
{
     /*  Initialise the default arm status */
    arm_status status = ARM_MATH_ARGUMENT_ERROR;



    /*  Initialise the default arm status */
    status = ARM_MATH_SUCCESS;

    /*  Initialize the Real FFT length */
    S->fftLenReal = (uint16_t) fftLenReal;

    /*  Initialize the Twiddle coefficientA pointer */
    S->pTwiddleAReal = (q15_t *) realCoefAQ15;

    /*  Initialize the Twiddle coefficientB pointer */
    S->pTwiddleBReal = (q15_t *) realCoefBQ15;

    /*  Initialize the Flag for selection of RFFT or RIFFT */
    S->ifftFlagR = (uint8_t) ifftFlagR;

    /*  Initialize the Flag for calculation Bit reversal or not */
    S->bitReverseFlagR = (uint8_t) bitReverseFlag;

    /*  Initialization of coef modifier depending on the FFT length */
    switch (S->fftLenReal)
    {
    case 8192U:
        S->twidCoefRModifier = 1U;

          S->pCfft = &arm_cfft_sR_q15_len4096;
        break;
    case 4096U:
        S->twidCoefRModifier = 2U;

           S->pCfft = &arm_cfft_sR_q15_len2048;
        break;
    case 2048U:
        S->twidCoefRModifier = 4U;

           S->pCfft = &arm_cfft_sR_q15_len1024;
        break;
    case 1024U:
        S->twidCoefRModifier = 8U;

          S->pCfft = &arm_cfft_sR_q15_len512;
        break;
    case 512U:
        S->twidCoefRModifier = 16U;

           S->pCfft = &arm_cfft_sR_q15_len256;
        break;
    case 256U:
        S->twidCoefRModifier = 32U;

           S->pCfft = &arm_cfft_sR_q15_len128;
        break;
    case 128U:
        S->twidCoefRModifier = 64U;

           S->pCfft = &arm_cfft_sR_q15_len64;
        break;
    case 64U:
        S->twidCoefRModifier = 128U;

          S->pCfft = &arm_cfft_sR_q15_len32;
        break;
    case 32U:
        S->twidCoefRModifier = 256U;

           S->pCfft = &arm_cfft_sR_q15_len16;
        break;
    default:
        /*  Reporting argument error if rfftSize is not valid value */
        status = ARM_MATH_ARGUMENT_ERROR;
        break;
    }

    /* return the status of RFFT Init function */
    return (status);
}

/**
  @} end of RealFFT group
 */
