/**
* @file main.c
*
* @brief Basic example for a PROFINET slave device running on AM335x/AM437x/AM57xx devices.
* Requires a full ICSS system (both PRUs) and two external phys connected to
* ICSS MII/MDIO interfaces
*
* \par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
* \par
*
*/

/*! \mainpage PROFINET Slave Device Application
 * \version 1.0.1
 *
 * \section intro_sec Introduction
 *
 * This is an example PROFINET slave device application based on Profinet Drivers provided in PRU-ICSS
 * Profinet slave Package and Molex PROFINET stack. This example may be used for evaluation and test of
 * the TI PROFINET Slave device solution. It may also serve as a starting project for
 * customer applications.
 *
 * Build any configuration of the examples and use resulting binaries on
 * supported boards. See the GSD folder for required GSD file. Testing requires a PROFINET capable PLC.
 * How to use a PLC is not part of this documentation.
 *
 */


// include invierment
#include <app.h>
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

#include <stdio.h>


int main()
{

    Board_init(BOARD_INIT_MODULE_CLOCK | BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO | BOARD_INIT_ICSS_ETH_PHY | BOARD_INIT_PINMUX_CONFIG );
    UART_init();
   _main_profinet_();
    // init OS
    OSAL_OS_start();

    return 0;

}

