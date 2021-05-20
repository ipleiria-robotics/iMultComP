/**
 * \file eip_utils.c
 * \brief
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
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include"eip_main.h"
#include <examples/board/include/board_gpioLed.h>
#include <examples/board/include/board_spi.h>
#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>
#include <ti/drv/uart/UART_stdio.h>


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
extern int acdIPAssign;
extern unsigned char board_Type;
extern char chIpAddr[16];
extern S25FL_Handle flashHandle;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  @brief  Function set the Led pattern on startup
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_initLedSeq()
{

    /*Turn on LED1 Green*/
    Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);
    Task_sleep(500);
    /*Turn off LED1*/
    Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 0);
    Board_setTriColorLED(BOARD_TRICOLOR0_RED, 1);
    Task_sleep(500);
    Board_setTriColorLED(BOARD_TRICOLOR0_RED, 0);
    Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);

    Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 1);
    Task_sleep(500);
    Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 0);
    Board_setTriColorLED(BOARD_TRICOLOR1_RED, 1);
    Task_sleep(500);
    Board_setTriColorLED(BOARD_TRICOLOR1_RED, 0);
}

/**
 *  @brief  Led Task Definition
 *
 *  @param  arg0
 *  @param  arg1
 *
 *  @retval none
 *
 */
void EIPUTILS_ledFxn(UArg arg0, UArg arg1)
{
    int led_val = 0;
    EIPUTILS_initLedSeq();

    while(1)
    {

        Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);

        if(led_val == 0)
        {
            if(EIPACD_getACDLEDStat())
            {
                /* MS blink red  NS Solid red*/
                Board_setTriColorLED(BOARD_TRICOLOR0_RED, 1);
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 0);

                Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 0);
                Board_setTriColorLED(BOARD_TRICOLOR1_RED, 1);
            }

            else
            {
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);
                Board_setTriColorLED(BOARD_TRICOLOR0_RED, 0);

                switch(USER_Get_ConnectionStat())
                {
                    case EIP_CONN_TIMEOUT:
                        Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 0);
                        Board_setTriColorLED(BOARD_TRICOLOR1_RED, 1);
                        break;

                    default:
                        if(acdIPAssign)
                        {
                            Board_setTriColorLED(BOARD_TRICOLOR1_RED, 0);
                            Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 1);
                        }

                        break;
                }

            }
        }

        else if(led_val == 1)
        {
            if(EIPACD_getACDLEDStat())    /*MS blink red  NS red*/
            {
                Board_setTriColorLED(BOARD_TRICOLOR0_RED, 0);
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 0);

                Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 0);
                Board_setTriColorLED(BOARD_TRICOLOR1_RED, 1);
            }

            else
            {
                Board_setTriColorLED(BOARD_TRICOLOR0_GREEN, 1);
                Board_setTriColorLED(BOARD_TRICOLOR0_RED, 0);

                switch(USER_Get_ConnectionStat())
                {
                    case EIP_CONN_AVAIL:
                        Board_setTriColorLED(BOARD_TRICOLOR1_RED, 0);
                        Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 1);
                        break;

                    default:
                        Board_setTriColorLED(BOARD_TRICOLOR1_RED, 0);
                        Board_setTriColorLED(BOARD_TRICOLOR1_GREEN, 0);
                        break;
                }
            }

        }

        led_val = (led_val + 1) % 2;
        Task_sleep(500);
    }
}

/**
 *  @brief  Function displays Application status and details on startup
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayApplicationDetails()
{

    if(eipDevInitConfig.acdEnable == 1)
    {
        UART_printf("\n\rACD Bit Enabled in SPIFlash. Starting the Application in ACD Mode");
        UART_printf("\n\rIP Address will be taken once Link is up");
    }
    else
    {
        UART_printf("\n\rACD Bit Disabled in SPIFlash.Starting the Application in Normal Mode");
    }

    if(eipDevInitConfig.tcpipConfig.startupType == EIP_STARTUP_DHCP)
    {
        UART_printf("\n\rWaiting for IP Address to be assigned by DHCP Server");
        UART_printf("\n\rPress N to assign IP Manually");
    }
    else
    {
        UART_printf("\n\rIP Address obtained from SPI Flash");
    }
}
/**
 *  @brief  Function displays Assigned IP address in serial console
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayIPDetails()
{
    EIP_CHAR chDefaultIP[16];
    EIPUTILS_getCurrentIP(chDefaultIP);
    UART_printf("\n\rIP Address Assigned: ");
    UART_printf((char *)(chDefaultIP));
    UART_printf("\n\r");
}
/**
 *  @brief  Function displays Assigned macid in serial console
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPUTILS_displayMacid()
{
    int i;
    char buf_str[18];
    Uint8 bMacAddr[6];
    char *buf_ptr;

    NIMU_IF_REQ if_req;
    if_req.name[0] = 0;
    if_req.index   = 1;

    NIMUIoctl(NIMU_GET_DEVICE_MAC, &if_req, &bMacAddr, sizeof(bMacAddr));


    buf_ptr = buf_str;

    for(i = 0; i < 6; i++)
    {
        if(i != 5)
        {
            buf_ptr += sprintf(buf_ptr, "%02x:", (char)bMacAddr[i]);
        }

        else if(i == 5)
        {
            buf_ptr += sprintf(buf_ptr, "%02x", (char)bMacAddr[i]);
        }
    }

    sprintf(buf_ptr, "\n");
    *(buf_ptr + 1) = '\0';

    UART_printf("\n\r");
    UART_printf("MAC : ");

    UART_printf(buf_str);

}
/**
 *  @brief  The function returns the IP addreses assigned
 *
 *  @param   ipAddr [out] IP address pointer
 *
 *  @retval EIP_TRUE  - If success
 *          EIP_FALSE - If failed
 *
 */
EIP_BOOL EIPUTILS_getCurrentIP(EIP_CHAR *ipAddr)
{
    CI_IPNET NA;

    if(CfgGetImmediate(0, CFGTAG_IPNET, 1, 1, sizeof(NA),
                       (uint8_t *)&NA) == sizeof(NA))
    {
        NtIPN2Str(NA.IPAddr, ipAddr);
        return EIP_TRUE;
    }

    else
    {
        return EIP_FALSE;
    }

}
/**
 *  @brief  The function is used to validate an IP adddress.
 *
 *  @param   ip_str [in] IP address pointer
 *
 *  @retval EIP_TRUE  - If valid IP address
 *          EIP_FALSE - If invalid IP address
 *
 */
int EIPUTILS_isValidIP(const char *ip_str)
{
    unsigned int n1, n2, n3, n4;

    if(sscanf(ip_str, "%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4)
    {
        return EIP_FALSE;
    }

    if((n1 != 0) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255)
            && (n4 <= 255))
    {
        char buf[64];
        sprintf(buf, "%u.%u.%u.%u", n1, n2, n3, n4);

        if(strcmp(buf, ip_str))
        {
            return 0;
        }

        return EIP_TRUE;
    }

    return EIP_FALSE;
}

/**
 *  @brief  The function changes the endianness of the IP Address
 *
 *  @param   dwIPAddress [in] IP address pointer
 *
 *  @retval  none
 *
 */
EIP_VOID EIPUTILS_changeIPEndianness(EIP_DWORD *dwIPAddress)
{
    EIP_DWORD dwTempIP;

    dwTempIP = * dwIPAddress << 24;
    dwTempIP |= (* dwIPAddress & 0xFF00) << 8;
    dwTempIP |= (* dwIPAddress & 0xFF0000) >> 8;
    dwTempIP |= (* dwIPAddress & 0xFF000000) >> 24;
    * dwIPAddress = dwTempIP;
}

/**
 *  @brief  The function receives IP Address from User and assign it to the device.
 *
 *  @param   userIPAddress [in] IP address pointer
 *
 *  @retval  1 - If success
 *           0 - If failed
 *
 */
EIP_BOOL EIPUTILS_assignUserIP(char *userIPAddress)
{
    EPROM_TCPIP tcpControl = {0};
    EIP_DWORD ipAddr;
    uint32_t blockNum, pageNum;

    if(EIPUTILS_isValidIP(userIPAddress))
    {
        ipAddr = inet_addr(userIPAddress);
        EIPUTILS_changeIPEndianness(&ipAddr);
        tcpControl.deviceIP = ipAddr;
        tcpControl.networkMask = 0xffffff00;
        tcpControl.startupType = EIP_STARTUP_STATICIP;

        Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                                   &blockNum, &pageNum);

        Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
        Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                         (uint8_t *)&tcpControl, sizeof(EPROM_TCPIP), NULL);

        return 1;
    }

    else
    {
        return 0;
    }
}

