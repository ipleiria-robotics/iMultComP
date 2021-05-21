/**
 * app_restart.c
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

#include <ti/sysbios/BIOS.h>

#if defined(SOC_AM335x) || defined(SOC_AM437x)
#include <ti/sysbios/family/arm/a8/Mmu.h>
#elif defined(SOC_AM572x) || defined(SOC_AM571x)  || defined(SOC_K2G)
#include <ti/sysbios/family/arm/a15/Mmu.h>
#include <ti/csl/soc/am572x/src/cslr_soc_mpu_baseaddress.h>
#include <ti/csl/soc/am571x/src/cslr_soc_mpu_baseaddress.h>
#endif


#include <ti/ndk/inc/stkmain.h>
#include <ti/ndk/inc/netctrl/netctrl.h>
#include <ti/sysbios/knl/Task.h>


#include <Include/protocol/app_restart.h>


#if defined(SOC_AM572x) || defined(SOC_AM571x)
extern uint8_t  __iodelaydata_start__;
extern uint8_t __iodelaydata_end__;
extern uint8_t  __iodelaycode_start__;
extern uint8_t __iodelaycode_end__;

/*L3 base address*/
#define DELAYDATA_SECT_START  CSL_MPU_OCMC_RAM1_REGS
#define DELAYDATA_SECT_SAVEADDR  0x80800000 /*Make sure this is placed well after the memory map ends*/

uint32_t secLength = 0;
#endif /*defined(SOC_AM572x) || defined(SOC_AM571x)*/
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @brief  The function restarts the application
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_restartApplication()
{
#if defined(SOC_AM572x) || defined(SOC_AM571x)
    EIPAPP_restoreIODelaySection();
#endif /*defined(SOC_AM572x) || defined(SOC_AM571x)*/
    BIOS_exit(100);
}

/**
 *  @brief  Exit handler for the application.
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_exitHandler(int var)
{

    void (*appEntry)();

    if(var == 100)
    {
        appEntry = (void (*)(void))0x80000000;
        Mmu_disable();
        (*appEntry)();
    }
}

#if defined(SOC_AM572x) || defined(SOC_AM571x)

/**
 *  @brief  Save IO delay to DDR. This is done to restore it before restart
 *
 *          The application during startup expects the IO delay data and code section to be placed
 *          in the begining of OCMC ram. During ICSS Switch operation this will be overwritten
 *          So this is stored atr a safe place
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_saveIODelaySection()
{
    uint8_t *secEnd;
    secEnd = &__iodelaycode_end__;
    secLength = ((uint32_t)secEnd - (uint32_t)DELAYDATA_SECT_START);

    memcpy((void *)DELAYDATA_SECT_SAVEADDR, (void *)DELAYDATA_SECT_START,
           secLength);


}
/**
 *  @brief  Restores the IO delay section saved.This is done before applciation restart
 *
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPAPP_restoreIODelaySection()
{
    memcpy((void *)DELAYDATA_SECT_START, (void *)DELAYDATA_SECT_SAVEADDR,
           secLength);
}
#endif /*defined(SOC_AM572x) || defined(SOC_AM571x)*/
