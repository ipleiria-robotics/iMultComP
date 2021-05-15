/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * @file    profi_misc.h
 * @brief   misc
 */
#ifndef PROFI_MISC_H
#define PROFI_MISC_H

#include "profi_hw_utils.h"

/*! \mainpage PROFIBUS SLAVE API
 *
 * \version 1.0.0.1
 *
 * \section intro_sec Introduction
 *
 * PRU-ICSS Profibus slave firmware implements Profibus slave functionality integrated into AM57xx SOC with PRU-ICSS IP.
 *
 * \section pn_purpose Purpose and Scope
 * Purpose of this document is to assist developers porting their Profibus slave  stack to \n
 *  TI AM57xx SOCs.
 *
 * \section pn_audience Intended Audience
 * -System integrators using TI Profibus slave  in their products \n
 * -Profibus slave  stack vendors to support TI Profibus slave  solution
 *
 * \section pn_arch Software Architecture
 * @image html profibus_slave_arch.png
 *
 * \section pn_kickoff Procedure to kickoff Profibus slave firmware
 * . Initialize the required tasks and interrupts
 * . Initialize the Profibus slave Driver, Memory(shared memory0)and PRU-ICSS INTC \n
 * . Load Profibus slave firmware into PRUs of PRU-ICSS \n
 * . Initialize the Profibus slave stack \n
 * . Start firmware \n
 * . Handle the events as needed - these have respective callbacks that can be registered to custom stack APIs  \n
 *
 * \section pn_fw_api Firmware APIs
 * These APIs implement the key interface between Profibus slave firmware and  stack. They are
 * implemented in following source code files \n
 * third_party\protocols\profibus_slave\include\L2.h \n
 *
 * <b>PRU_ICSS Profibus slave firmware headers</b> \n
 * protocols\profibus_slave\firmware\profibus_pru0_main_bin.h  \n
 * protocols\profibus_slave\firmware\profibus_pru1_main_bin.h  \n
 *
 * \section install_sec Installation
 *
 * This driver is installed as part of PRU-ICSS Profibus slave  Package
 *
 * \section Documentation
 * <a href="pages.html">Module Documentation</a>
 *
 * \section terms_abbrevs Term and Abbreviations
 * Abbreviation  | Expansion
 * ------------- | -------------
 * PRUSS         | Programmable RealTime Unit Sub System
 * PRU-ICSS      | Programmable RealTime Unit - Industrial Communication Sub System - PRUSS with industrial communication support
 * FDL           | Field Data bus Link
 * DP            | Decentralized Peripherals
 * SAP           | Service Access Point
 * IPC           | Inter Processor Communication
 */
/**
* @defgroup PB_ARM_STACK_TO_FIRMWARE TMG Stack to Firmware(FDL) Interface
* The IPC between these two happens through a shared memory structure @ref TL2GlobVar which is maintained
* in ICSS shared RAM (base address: 0x4b210000)
*/
/**
* @defgroup PB_ARM_APPLICATION_UTILITY Application Utility Interface
* Application utility module configures the I2c,UART,and Timer for profibus operations.
*/
/**
 * @defgroup PB_ARM_DMA_UTILITY EDMA Interface with Stack
 * Application uses the EDMA for memcopy operation
 */

/**
* @page PB_IRT_DOC PROFIBUS Slave  documentation
* -@subpage PB_STACK_FDL_INTERFACE
* -@subpage PB_FIRMWARE_ARCH
* -@subpage PB_SHARED_MEMORY_INTERFACE
* -@subpage PB_COMMAND_INTERFACE
* -@subpage PB_DP_STATUS
* -@subpage PB_BAUD_RATE_DETECTION
*
*/
/**
* @page PB_STACK_FDL_INTERFACE Integration with FDL Firmware
* @section PB_STACK_FDL_INTERFACE Introduction
* TI Profibus solution uses the PRU-ICSS to run the L2 FDL .The PRU Firmware is customized for
* Profibus functionalities.
*
* @section PB_FIRMWARE_ARCH Firmware Architecture
* the below Figure shows the high level view of PRU-ICSS FDL PROFIBUS slave firmware in operation
* and how it interacts with Cortex A host stack using PRU-ICSS shared memory interface and
* PRU-ICSS INTC interrupts in Sitara AM335x/AM437x/AM57x SOCs

* @image html FDL_arch.png
* PROFIBUS L7 stack runs on ARM and L2 FDL runs on PRU-ICSS. The IPC between these two happens through a shared memory and PRU1 data memory of PRU-ICSS and
* using the Interrupts between ARM and PRU-ICSS

* @section PB_SHARED_MEMORY_INTERFACE Shared Memory Interface
*  This interface uses the shared memory of PRU-ICSS.The data structure @ref TL2GlobVar is maintained in this memory which can be accessed by both ARM and PRU\n\n
*  The following table lists the fields in the shared memory structure with the offsets from the base address. The stack APIs can appropriately access this structure
*  to communicate with FDL
* @image html sharedmemory_1.png
* @image html sharedmemory_2.png
* @image html sharedmemory_3.png
*
*
* @section  PB_COMMAND_INTERFACE Command Interface
*  Command interface provides consistent locked access to Layer2 interface. ARM issues a command and must wait for\n
*  PRU response before proceeding to issue the next command.The below figure gives the details of the command.
*  @image html cmd.png
*
* @section  PB_DP_STATUS DP status
* The below table gives the description of the DP status
*  @image html dp_status.png
*
*
* @section  PB_BAUD_RATE_DETECTION Automatic Baud Rate detection
*     Automatic baud rate is supported using StartDelimterCounter variable in L3 Shared memory structure.
*     PRU FDL increments this counter whenever it sees a valid Start Delimiter (SD) on the serial bus.
*     Below is the procedure to implement automatic baud rate detection in the L7 stack
*
*           Check StartDelimterCounter periodically (Say 100 ms)
*
*           If StartDelimterCounter is non zero then reset StartDelimterCounter to zero and restart the timer
*
*           If StartDelimterCounter is zero then this means PRU FDL did not lock to the baudrate set on the bus by the master\n
*           Disable PRU0 and PRU1\n
*           Set the next baud rate using PROFIBUS driver API\n
*            Re-enable PRU1\n
*            Set Slave Address, Min TSDR and Current Baudrate at PRU FDL using CMD_SET_VALUE_REQ\n
*            Re-enable PRU0\n
*            Restart the timer for a period of 10ms\n
 */

/* @def ENABLE_WATCHDOG_SUPPORT
 *      Enable watchdog support
 */
#define DRAM_L3BASEADDRESS_OFFSET    0xF0

/* @def ENABLE_WATCHDOG_SUPPORT
 *      Enable watchdog support
 */
#define ENABLE_WATCHDOG_SUPPORT


/* @def RTC_DEBUG
 *      Time for watchdog expiry in milliseconds
 */
#define WATCHDOG_EXPIRE_TIME      100
/**
 * @brief Calls functions for registering PRUSS interrupt in the ARM INTC, getting
 *         Station address from UART, and initializing EDMA to be used by ARM and PRU.
 *
 *
 */
void profi_init_soc(void);

/**
 * @brief Converts int to char.
 *
 *  @param   val    Integer value to be converted
 *
 */
void profi_uart_itoa(int val);

/**
 * @brief Function loads the PRU firmware to the specified PRU.
 *
 *  @param   pruNum    PRU number to which firmware to be loaded
 *
 */
void profi_loadPruFirmware(uint8_t pruNum);

/**
 * @brief Function disables the PRU.
 *
 *  @param   pruNum    PRU number to be disabled.
 *
 */
void profi_disablePru(uint8_t pruNum);

/**
 * @brief Function enables the PRU.
 *
 *  @param   pruNum    PRU number to be enabled.
 *
 */
void profi_enablePru(uint8_t pruNum);

#endif
