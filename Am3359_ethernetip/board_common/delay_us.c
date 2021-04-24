/**
 * delay_us.c
 *
*/
/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stdlib.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Types.h>
#ifdef C66X
#include <ti/csl/csl_tsc.h>

static uint8_t timer_init_flag = 0;
#endif
static Types_FreqHz CPU_FreqHz = {0, 0};

static uint32_t read_cycle_counter(void)
{
    uint32_t timeVal;
#ifndef C66X
#ifdef __aarch64__
   //For AM65x - A53 64-bit
    __asm__ __volatile__("mrs %0, pmcr_el0" : "=r" (timeVal));
#endif
#ifdef __TI_ARM__
    //For R5F & M4 using TI CGT
    timeVal = __MRC( 15, 0, 9, 13, 0);
#else
#ifdef __ARM_ARCH_7A__
    //For Sitara and K2G
    __asm__ __volatile__("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(timeVal));
#endif
#endif
#else
    timeVal = (uint32_t) CSL_tscRead();
#endif
    return timeVal;
}


void delay_us(uint32_t delay_in_microseconds)
{
    uint32_t delay_count;
    uint32_t start_count, curr_count, count_diff = 0;

#ifdef C66X

    if(timer_init_flag == 0)
    {
        CSL_tscEnable();
        timer_init_flag = 1;
    }

#endif

    start_count = read_cycle_counter();

    if(CPU_FreqHz.lo == 0)
    {
        BIOS_getCpuFreq(&CPU_FreqHz);
    }

    delay_count = delay_in_microseconds * (CPU_FreqHz.lo / 1000000);
    curr_count = read_cycle_counter();

    while(count_diff < delay_count)
    {
        curr_count = read_cycle_counter();

        if(curr_count > start_count)
        {
            count_diff = curr_count - start_count;
        }

        else
        {
            count_diff = 0xFFFFFFFFU - start_count;
            count_diff += curr_count;
        }
    }
}
