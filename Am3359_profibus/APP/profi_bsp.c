/*
 * @file    profi_bsp.c
 * @brief  This file contains the functions for the
 *         PRU-ICSS Profibus implementation
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

#include "bsp.h"                /* this modules interface            */
#include "basictyp.h"
#include <L2.h>
#include "profi_pruss_driver.h"
#include <profi_soc.h>

#include <string.h> /* memset prototype */
extern void user_error(byte error_code);
TL2GlobVar XDATA *g_pL2GlobVar = NULL;
unsigned int offset;
extern int slave_addr;
extern int uartnum;
extern PRUICSS_Handle pruIcssHandle;
#define prussdrv_get_phys_addr(x) (unsigned int)(x)

/***************************************************************************/
/**
*       \author
*       \date
*       \return    void
*       \brief     this function initializes the hardware
*
*
****************************************************************************/
void SendCommandToFDL(void)
{

    PRUICSS_pruSendWaitCearEvent(pruIcssHandle, ARM_PRU1_INTERRUPT,
                                 PRU_EVTOUT0, PRU1_ARM_INTERRUPT);
}

void BSP_Init(void)
{

    PRUICSS_IntcInitData pruss_intc_initdata = PRUSS_INTC_INITDATA;
    byte data[512];
    memset(data, 0, 512);

    PRUICSS_pruIntcInit(pruIcssHandle, &pruss_intc_initdata);

    PRUICSS_pruDisable(pruIcssHandle, 0);
    PRUICSS_pruDisable(pruIcssHandle, 1);

    profibus_hwutils_timer_init(pruIcssHandle);            /*Initialize ECAP module*/

    PRUICSS_pruReset(pruIcssHandle, 0);
    PRUICSS_pruReset(pruIcssHandle, 1);

    PRUICSS_pruWriteMemory(pruIcssHandle, PRU_ICSS_DATARAM(0), 0,
                           (uint32_t *)data, 512);
    PRUICSS_pruWriteMemory(pruIcssHandle, PRU_ICSS_DATARAM(1), 0,
                           (uint32_t *)data, 512);
    profi_soc_bsp_init();

    g_pL2GlobVar->ArmDmaChannel = ARM_EDMA_CHANNEL;
    g_pL2GlobVar->PruDmaChannel = PRU_EDMA_CHANNEL;
}


void BSP_exit(void)
{

    PRUICSS_pruDisable(pruIcssHandle, 0);
    PRUICSS_pruDisable(pruIcssHandle, 1);


}


void DmaCpy(void XDATA *pDest, void XDATA *pSource, u32 n)
{

    edma_memcpy(g_pL2GlobVar->ArmDmaChannel, prussdrv_get_phys_addr(pDest),
                prussdrv_get_phys_addr(pSource), n);
}

void DmaSet(void XDATA *pDest, byte by, u32 n)
{
    memset(g_pL2GlobVar->tempDmaSetVal, by, n);
    edma_memcpy(g_pL2GlobVar->ArmDmaChannel, prussdrv_get_phys_addr(pDest),
                prussdrv_get_phys_addr(&g_pL2GlobVar->tempDmaSetVal), n);

}


/****************************************************************************
*
*      Name          : enable_timer
*      Beschreibung  : enables the Timer-Interrupt
*
****************************************************************************/
void enable_timer(void)
{
}

/****************************************************************************
*
*      Name          : disable_timer
*      Beschreibung  : disables the Timer-Interrupt
*
****************************************************************************/
void disable_timer(void)
{
}


/****************************************************************************
*
*      Name          : hw_clock_interrupt
*      Parameters    : --
*      Return Values : --
*      Description   : This function should be called when the timer generates
*                      an interrupt.
*
****************************************************************************/
void *TimerInterrupt_TP2(void *arg)
{
    do_int_timer();
    return NULL;
}

/****************************************************************************
*
*      Name          : init_hw_timer
*      Parameters    : --
*      Return Values : --
*      Description   : This function initializes the Hardware Timer
*
****************************************************************************/

void init_hw_timer(void)
{
    timer_inc = 1;

    profibus_hwutils_systemtimer_init(5, TimerInterrupt_TP2);
}


TBool g_bDoAbort = 0;

void cleanup_on_abort(int signum)
{
    g_bDoAbort = 1;
}

int BSP_main(int argc, char **argv)
{
    uartnum = 1;
    BSP_Init();                 /* initialize hardware */

    user(NULL);

    BSP_exit();

    return (0);
}
