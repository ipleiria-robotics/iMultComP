/**
 * \file eip_main.h
 * \brief Ethernetip Application related header file
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
#ifndef ETHERNETIP_ADAPTER_EIP_MAIN_H_
#define ETHERNETIP_ADAPTER_EIP_MAIN_H_

#include <assert.h>
#include <Include/protocol/app_api.h>
#include <Include/protocol/hal.h>
#include <Include/protocol/eip_device.h>

#include "eip_utils.h"
#include "app_restart.h"
#include "user_acd_ndk.h"



/**Escape Key ASCI*/
#define ESC_KEY     0x1b
#define ENTER_KEY       0x0D
#define BSPACE_KEY      0x08

/**Major Revision NUmber*/
#define APP_REVISION_MAJOR  3
/**Minor Revision NUmber*/
#define APP_REVISION_MINOR  1

/*Start the offset from 2 MB onwards. And 64 k for each offset*/
#define SPI_EEPROM_DEVICEIP_OFFSET 0x200000
#define SPI_EEPROM_SPEED_OFFSET    0x210000
#define SPI_EEPROM_ACD_CONFLICT_OFFSET  0x220000
#define SPI_EEPROM_ACD_OFFSET  0x230000
#define SPI_EEPROM_QOS_OFFSET    0x240000
#define SPI_EEPROM_INCARN_OFFSET    0x250000

#define EIP_STARTUP_DHCP      2
#define EIP_STARTUP_STATICIP  0

#define EIP_ACD_DEFAULT  0
#define EIP_ACD_START  1
#define EIP_ACD_RESTART  2
#define EIP_ACD_STOP  3

/*PTP min/max offset values*/
#define PTP_MIN_OFFSET_INIT_VAL     999999
#define PTP_MAX_OFFSET_INIT_VAL     -100000

/**Ip Address used by the device*/
extern EIP_DWORD gdwIPAddress;
/**Ip mask used by the device*/
extern EIP_DWORD gdwMask;
extern DWORD ifConnfigACDStat;
extern Uint8 user_input_processing;
extern Char rxByte;
extern unsigned char board_Type;
/**
 * \brief IO Data Connection Parameters
 */
typedef struct
{
    /**Data size of IN data*/
    EIP_WORD wSizeIn;
    /**Offset to get IN data*/
    EIP_WORD wOffsetIn;
    /**Data size of Out data*/
    EIP_WORD wSizeOut;
    /**Offset to get Out data*/
    EIP_WORD wOffsetOut;
} ST_CNXN_PARAMS;

typedef struct
{
    EIP_DWORD deviceIP;
    EIP_BOOL startupType;
    EIP_DWORD networkMask;
    EIP_DWORD gatewayAddress;
    EIP_DWORD serverName;
} EPROM_TCPIP;


typedef struct
{
    EPROM_TCPIP tcpipConfig;
    ST_ETH_CONTROL ethConfig[2];
    EIP_BOOL acdEnable;
    EIP_BOOL qosEnable;
    EIP_WORD incarnNum;
    CLASS_TCP_LAST_CONFLICT_DETECTED acdConflictDetails;
} DEVICE_INIT_CONFIG;

extern ST_TCPIP_CONFIGURATION eipDevInterfaceConfig;
extern DEVICE_INIT_CONFIG eipDevInitConfig;
extern EPROM_TCPIP tcpControl;
extern unsigned int acdEIPStatus;


/**
 *  @brief  Main function called to start EIP application and Stack
 *
 *  @param  eipicssEmacHandle [in] - EMAC LLD handle
 *
 *  @retval  0  - When the EIP app is stopped
 *
 */
int EIPAPP_main(ICSSEMAC_Handle pruIcssHandle);
/**
 *  @brief  Uart Task definition
 *
 *          Task continuously waits for uart input.This is processed and appropriate actions are
 *          taken
 *
 *  @param  a0 [in]
 *  @param  a1 [in]
 *
 *  @retval  none
 *
 */
void EIPAPP_uartTask(UArg a0, UArg a1);
/**
 *  @brief  This function calls the DLR and PTP APIs that are to be called on IP changes
 *
 *          This inturn needs to be caled whenever there is an IP change
 *
 *  @param  ipAddress   [in] IP Address
 *  @param  ipAddAssign [in] 1 - IP is assigend
 *                           0 - IP is removed
 *
 *  @retval  none
 *
 */
EIP_VOID EIPAPP_ipAddressChange(Uint32 ipAddress, Uint32 ipAddAssign);
/**
 *  @brief  Wrapper API to get the Link status
 *
 *          Gets the Linbk status from EMAC LLD handle
 *
 *  @param  portnum [in] Port number
 *
 *  @retval  1 - On Link up
 *           0 - If Link is down
 *
 */
uint8_t GetLinkStatus(uint8_t portnum);

void monitorPTPStatus_Task(UArg arg0, UArg arg1);

void EIP_DLR_TIMESYNC_port0ProcessLinkBrk(uint8_t linkStatus, void *arg2);

void EIP_DLR_TIMESYNC_port1ProcessLinkBrk(uint8_t linkStatus, void *arg2);

void EIP_DLR_TIMESYNC_port0ProcessLinkBrk(uint8_t linkStatus, void *arg2);

void EIP_DLR_TIMESYNC_port1ProcessLinkBrk(uint8_t linkStatus, void *arg2);
#endif /* ETHERNETIP_ADAPTER_EIP_MAIN_H_ */
