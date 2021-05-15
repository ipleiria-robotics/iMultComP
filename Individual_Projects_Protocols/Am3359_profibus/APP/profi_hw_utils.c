/*
 * @file    profi_hw_utils.c
 * @brief   Implementation of initilization and usage of peripherals used
 *          for profibus like UART, ECAP, and Timer
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
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

/* Application header files */
#include <AM335x/profi_hw_utils.h>
#include <AM335x/profi_timer.h>

#include <ti/drv/pruss/pruicss.h>
#include <ti/csl/src/ip/icss/V0/cslr_icssm_ecap.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_uart.h>
#include <ti/drv/i2c/I2C.h>
#include <AM335x/basictyp.h>


/****************************************************************************/
/*                      GLOBAL VARIABLES                                    */
/****************************************************************************/

extern PRUICSS_Handle pruIcssHandle;

/****************************************************************************/
/*                     Function definition                                  */
/****************************************************************************/

/**
 *  @brief   ecap_init
 *  @param   pruIcssHandle    pruicsshandle
 *
 *
 ***************************************************************************/
void profi_hwutills_ecap_init(PRUICSS_Handle pruIcssHandle)
{


    PRUICSS_HwAttrs      const    *hwAttrs;


    hwAttrs = pruIcssHandle->hwAttrs;
    HWREGH(hwAttrs->prussEcapRegBase + CSL_ICSSM_ECAP_ECEINT) = 0;
    HWREGH(hwAttrs->prussEcapRegBase + CSL_ICSSM_ECAP_ECCLR)  = 0xFFFF;
    HWREGH(hwAttrs->prussEcapRegBase + CSL_ICSSM_ECAP_ECCTL1) = 0x8100;
    HWREGH(hwAttrs->prussEcapRegBase + CSL_ICSSM_ECAP_ECCTL2) = 0x0086;
    HWREGH(hwAttrs->prussEcapRegBase + CSL_ICSSM_ECAP_ECEINT) = 0x0020;
}

/**
 * @brief wrapper function to Task-sleep.
 * @param   milisec    time for sleep
 * @retval  1.
 *
 */
int bsp_msleep(unsigned long milisec)
{
    Task_sleep(milisec);
    return 1;
}

/**
 * @brief Sets the DLL and DLH values in UART according to baudrate
 * @param   DLL
 * @param   DLH
 *
 */
void profibus_hwutils_set_uart_param(unsigned int DLL, unsigned int DLH)
{



    PRUICSS_HwAttrs      const    *hwAttrs;


    hwAttrs = pruIcssHandle->hwAttrs;


    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTPOWERMANAGEMENTEMULATION) =
        0;


    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTMODEMCONTROL) = 0;

    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTDIVISORLSB) = DLL;


    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTDIVISORMSB) = DLH;


    HWREGH(hwAttrs->prussUartRegBase +
           CSL_ICSSUART_UARTINTERRUPTIDENTIFICATIONFIFOCONTROL) = 0;


    HWREGH(hwAttrs->prussUartRegBase +
           CSL_ICSSUART_UARTINTERRUPTIDENTIFICATIONFIFOCONTROL) = 0x06;

    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTLINECONTROL) = 0x1B;
    HWREGH(hwAttrs->prussUartRegBase + CSL_ICSSUART_UARTPOWERMANAGEMENTEMULATION) =
        0x6001;
}

/**
 * @brief calls functions that registers the timer callback function.
 *
 *  @param   period    Time period of timer
 *
 *  @param   timerfn    Call back function
 *
 */
int profibus_hwutils_systemtimer_init(unsigned int period,
                                      profibus_hwutils_function_handler timerfn)
{
    profi_timer_register_call_back(period, timerfn);
    return 0;
}

/**
 * @brief wrapper function that calls ecap init.
 *
 *  @param   pruIcssHandle    PRUICSS handle
 *
 */
int profibus_hwutils_timer_init(PRUICSS_Handle pruIcssHandle)
{
    profi_hwutills_ecap_init(pruIcssHandle);
    return 0;
}

/**
 * @brief wrapper function for Clock_getTicks which is a Sysbios function
 *                  that gives the tickcount in milisec.
 *
 * @retval  tick count.
 *
 */
unsigned int long  GetTickCount(void)
{
    return Clock_getTicks();
}

/**
 * @brief sets the baudrate on UART.
 *
 * @param   baudrate    baud rate
 * @retval  0 on success,-1 on failure.
 *
 */
int bsp_uart_init(unsigned int baudrate)
{
    switch(baudrate)
    {
        case BAUD_12M:
            profibus_hwutils_set_uart_param(1, 0);
            break;

        case BAUD_6M:
            profibus_hwutils_set_uart_param(2, 0);
            break;

        case BAUD_3M:
            profibus_hwutils_set_uart_param(4, 0);
            break;

        case BAUD_1_5M:
            profibus_hwutils_set_uart_param(8, 0);
            break;

        case BAUD_500K:
            profibus_hwutils_set_uart_param(0x18, 0);
            break;

        case BAUD_187K:
            profibus_hwutils_set_uart_param(0x40, 0);
            break;

        case BAUD_93K:
            profibus_hwutils_set_uart_param(0x80, 0);
            break;

        case BAUD_45K:
            profibus_hwutils_set_uart_param(0x8, 0x1);
            break;

        case BAUD_19K:
            profibus_hwutils_set_uart_param(0x71, 0x2);
            break;

        case BAUD_9K:
            profibus_hwutils_set_uart_param(0xE2, 0x4);
            break;

        default:
            return -1;
    }

    return 0;
}
