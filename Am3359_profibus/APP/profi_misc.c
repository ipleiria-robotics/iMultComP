/*
 * @file    profi_misc.c
 * @brief   This file has functions for registering PRUSS interrupt in
 *          the ARM INTC, getting station address from UART, and initializing EDMA
 *          to be used by ARM and PRU
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
/* SysBios header files */

/* Stack header files */
#include "pruss_intc_mapping.h"

#include <ti/drv/pruss/pruicss.h>
#include <profibus_pru0_main_bin.h>
#include <profibus_pru1_main_bin.h>
#include <ti/drv/uart/UART_stdio.h>
#include "profi_edma.h"
#include "profi_soc.h"
#include "profi_hw_utils.h"


extern PRUICSS_Handle pruIcssHandle;

/****************************************************************************/
/*                      FUNCTION PROTOTYPES                                 */
/****************************************************************************/
static void profi_put_message(void);
static void pru_handler(void);
void testPutString(char *str);

/****************************************************************************/
/*                      GLOBAL VARIABLES                                    */
/****************************************************************************/
extern int slave_addr;
unsigned long baseAddr;
volatile char temp_gl = 1;
volatile char start = 1;
volatile char tChar = 5;

typedef void (*PRUSSDRV_IRQ_HANDLER)(void *);
/****************************************************************************
 *                      Defines
 ****************************************************************************/
#define APPL_BUILD_VER  "1.0.0"

/****************************************************************************
 *                      LOCAL FUNCTION DEFINITIONS
 ****************************************************************************/

/****************************************************************************
 *  name        : profi_uart_itoa
 *  description : Converts int to char
 *
 ***************************************************************************/
void profi_uart_itoa(int val)
{
    unsigned char tChar = (val % 10) + '0';

    if(val)
    {
        val /= 10;
        profi_uart_itoa(val);
        UART_putc(tChar);
    }
}

/*****************************************************************************
 *  name        : profi_put_message
 *  description : This function waits for 5 sec to take station address from
 *                the user, otherwise takes the default station address to be 5
 ******************************************************************************/
static void profi_put_message(void)
{


    if(slave_addr)
    {
        profi_uart_itoa(slave_addr);
    }

    UART_printf("\n\rProfibus Application Running...\n\r");
}
/******************************************************************************************
 *  name        : arm_regitser_interrupts
 *  description : This function registers PRUSS host interrupt 2 (SYS_INT_PRUSS1_EVTOUT0)
 *                in ARM INTC and
 ******************************************************************************************/
static void arm_regitser_interrupts(void)
{
    profi_soc_register_interrupts(&pru_handler);
}

/******************************************************************************************
 *  name        : pru_handler
 *  description : This is a user defined ISR for SYS_INT_PRUSS1_EVTOUT0 interrupt
 *                 that clears the software PRU1_ARM_INTERRUPT in ICSS INTC.
 ******************************************************************************************/
void pru_handler(void)
{
    PRUICSS_pruClearEvent(pruIcssHandle, PRU1_ARM_INTERRUPT);
}

/******************************************************************************************
 *  name        : profi_init_soc
 *  description : Calls functions for registering PRUSS interrupt in the ARM INTC, getting
 *                station address from UART, and initializing EDMA to be used by ARM and PRU.
 ******************************************************************************************/
void profi_init_soc(void)
{
    /* Uncomment this function if hardware timer is used
     * for tickcounter in profi_timer.c file. Default is
     * Timer_task in main.c which gives 5ms tick using Task_sleep()
     * profi_timer_init();
     */

    /*The Profibus application waits for 15sec to get station address from
     * the UART console otherwise sets default address to 5.
     * This is done because on ICE the UART console can be connected only
     * after the board is powered on using mini USB port.
     */
    profi_put_message();
    arm_regitser_interrupts();
    profi_edma_init();
}



void profi_loadPruFirmware(uint8_t pruNum)
{
    uint32_t *firmware;
    uint32_t size;

#ifndef PRU_TX_EN

    if(0u == pruNum)
    {
        firmware = (uint32_t *)DataArray0;
        size = sizeof(DataArray0);
    }

    else
    {
        firmware = (uint32_t *)DataArray1;
        size = sizeof(DataArray1);
    }

#else

    if(0u == pruNum)
    {
        firmware = (uint32_t *)DataArray1;
        size = sizeof(DataArray1);
    }

    else
    {
        firmware = (uint32_t *)DataArray0;
        size = sizeof(DataArray0);
    }

#endif
    /* load and start PRU 1 first because the initialization uses the PRU 1 services */
    PRUICSS_pruDisable(pruIcssHandle, pruNum);
    /* PRU firmwares are loaded as header files in appliation  */
    PRUICSS_pruWriteMemory(pruIcssHandle, PRU_ICSS_IRAM(pruNum) , 0,
                                  (uint32_t *) firmware, size);
    PRUICSS_pruEnable(pruIcssHandle, pruNum);
}

void profi_disablePru(uint8_t pruNum)
{
    PRUICSS_pruDisable(pruIcssHandle, pruNum);
}

void profi_enablePru(uint8_t pruNum)
{
    PRUICSS_pruEnable(pruIcssHandle, pruNum);
}
