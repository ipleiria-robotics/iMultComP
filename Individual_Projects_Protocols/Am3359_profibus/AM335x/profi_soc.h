/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

/** ============================================================================
 * @file    profi_soc.h
 * @brief   misc
 */
#ifndef PROFI_SOC_H
#define PROFI_SOC_H

#define PRUICSS_INSTANCE_ONE   1
#define PRUICSS_INSTANCE_TWO   2


#if defined(SOC_AM571x)
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#elif defined(SOC_AM572x)
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#elif defined(SOC_K2G)
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#elif defined(SOC_AM335x)
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_ONE
#elif defined(SOC_AM437x)
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#else
#define PRUICSS_INSTANCE    PRUICSS_INSTANCE_TWO
#endif

/******************************************************************************************
 *  name        : profi_soc_register_interrupts
 *  description : This function registers PRUSS host interrupt 2 (SYS_INT_PRUSS1_EVTOUT0)
 *                in ARM INTC
 ******************************************************************************************/
void profi_soc_register_interrupts(void * irqHandler);
/****************************************************************************
 *  name        : Any SOC/Board specific initializations
 *  description :
 *                TX and RX pin muxing
 *                TX enable pin muxing
 *
 *
 *
 ***************************************************************************/
void profi_soc_hw_init(void);
/******************************************************************************************
 *  name        : profi_soc_bsp_init
 *  description : SOC/Board specific configuration of Shared structure
 *
 ******************************************************************************************/
void profi_soc_bsp_init();

/******************************************************************************************
 *  name        : profi_soc_set_digoutput
 *  description : SOC/Board specific API to set the Digital output corresponding to the Output data
 *
 ******************************************************************************************/
void profi_soc_set_digoutput(uint8_t outData);

/******************************************************************************************
 *  name        : profi_soc_edma_init
 *  description : This function does the EDMS configurations for PRU Channel only. (EDMA LLD used for ARM
 *                channel initializations
 ******************************************************************************************/
void profi_soc_edma_init(void);
/******************************************************************************************
 *  name        : profi_soc_watchdog_init
 *  description : This function does the configuration of watch dog feature. Enable and configure  the
 *                IEP PDI watch dog timer in PRUSS
 ******************************************************************************************/
void profi_soc_watchdog_init(uint32_t expiryDuration);
/******************************************************************************************
 *  name        : profi_soc_watchdog_reset
 *  description : This function resets the  IEP PDI watch dog timer. Needs to be called periodically from
 *                Watch dog task
 ******************************************************************************************/
void profi_soc_watchdog_reset(uint32_t expiryDuration);
#endif
