/**
 * \file eip_main.c
 * \brief File which contains Ethernetip init and exit functionalities
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

#include <ti/drv/uart/UART_stdio.h>

#include <Include/protocol/eip_main.h>
#include <Include/protocol/user_api.h>
#include <Include/protocol/eip_device.h>
#include <Include/protocol/hal.h>
#include <Include/protocol/EipConfData.H>
#include <Include/protocol/eip_api.h>
#include <Include/protocol/icss_ptpd_driver.h>
#include <Include/board/board_spi.h>
#include <Include/board/board_tlkphy.h>
#include <Include/protocol/app_restart.h>

#include <Include/protocol/icss_eip_firmware.h>
#include <Include/protocol/icss_dlr.h>
#include <Include/protocol/icss_eip_mcFltr.h>
#include <Include/protocol/icss_eip_driver.h>
#include <Include/protocol/timeSync/icss_timeSync.h>
#include <Include/protocol/debugtrace.h>

#include <Include/protocol/eip_utils.h>
#include <Include/protocol/user_acd_ndk.h>_>




#include <ti/drv/icss_emac/icss_emacDrv.h>
#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>



#include <Include/protocol/eip_soc.h>

extern PRUICSS_Handle handle;
extern ICSSEMAC_Handle emachandle;
extern TimeSync_ParamsHandle_t timeSyncHandle;
extern EIP_Handle eipHandle;
extern int32_t EIPACD_checkGratArp(void *txArg);

extern S25FL_Handle flashHandle;
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/**local IP Address*/
EIP_DWORD gdwIPAddress           = 0;
/**Local Mask*/
EIP_DWORD gdwMask                = 0xFFFFFF00;
/**Configuration file*/
EIP_CHAR  *gszConfigurationFile  = 0;
/**EIP Configuration File poiter*/
EIP_BYTE  *gpbyFilePtr;
/**Device Capabilities*/
EIP_DWORD gdwCapabilities = 0;
/**Shared memory pointer for IO Data Exchange*/
ST_SHM_PARAMS *gpstSharedMemory;
/**EIP Running mode status*/
EIP_BOOL ModeRunning = EIP_FALSE;
/**Flag to check uart input data processing*/
uint8_t user_input_processing = 0;
/** Variablr to receive uart input*/
char rxByte;
/**Variable to store IP Address*/
char chIpAddr[16];
uint8_t ipAddrFlag = 0;

unsigned int acdEIPStatus = EIP_ACD_DEFAULT;
DEVICE_INIT_CONFIG eipDevInitConfig;
ST_TCPIP_CONFIGURATION  eipDevTcpIpConfiguration;
unsigned int eipAppStat = 0;
extern Semaphore_Handle appInitSem;

#ifdef PTP_TESTING

/*Used to track min/max offset from master
 * Useful for testing*/
int32_t min_offset = PTP_MIN_OFFSET_INIT_VAL;
int32_t max_offset = PTP_MAX_OFFSET_INIT_VAL;

#endif //PTP_TESTING

#ifdef SOC_K2G
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK                 (0x3C000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_SHIFT                (26U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_RESETVAL             (0x00000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MAX                  (0x00000004U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MII_MODE_VAL         (2u)
#endif


/* ========================================================================== */
/*                            Extern Variables                                */
/* ========================================================================== */
/**Variable to check for Device to be restarted or not*/
extern EIP_BOOL gbResetIdentity;
extern int acdIPAssign;

/**
 *  @brief  Gets the Initial configuration from non volatile memory for Application startup
 *
 *
 *  @param  eipicssEmacHandle [in[ EMAC LLD handle
 *
 *  @retval  EIP_TRUE - On success
 *
 */
EIP_BOOL EIPAPP_getInitParams(ICSSEMAC_Handle eipicssEmacHandle);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @brief  The function initializes the Configuration file and Shared memory for IO data exchange
 *
 *  @param  none
 *
 *  @retval 0 - On Success
 *          1 - On failure
 *
 */
int EIPAPP_initApp()
{
    EIP_BYTE byMajor, byMinor, byBuild, bySpecial;
    EIP_GetVersion(&byMajor, &byMinor, &byBuild, &bySpecial);
    byMajor = EIPDEV_getMajorRevision();
    byMinor = EIPDEV_getMinorRevision();
    /*Configuration file ptr*/
    gpbyFilePtr = (EIP_BYTE *)EipConfData;
    /*SharedMemory Initializaation*/
    gpstSharedMemory = EIPDEV_getSharedMemoryPtr();

    if(gpbyFilePtr == EIP_NULL)
    {
        return 1;
    }

    return 0;
}

/**
 *  @brief  The function frees the shared memory
 *
 *  @param  none
 *
 *  @retval none
 *
 */
EIP_VOID EIPAPP_exitApp()
{
    if(gszConfigurationFile != EIP_NULL)
    {
        free(gszConfigurationFile);
        gszConfigurationFile = EIP_NULL;
    }

    if(gpstSharedMemory != EIP_NULL)
    {
        free(gpstSharedMemory);
        gpstSharedMemory = EIP_NULL;
    }
}
/**
 *  @brief  The function prints the PTP/1588 status
 *
 *  @param  eipicssEmacHandle PTP structure Handle
 *
 *  @retval none
 *
 */
void printPTPStatus(TimeSync_ParamsHandle_t timeSyncHandle)
{
    char pBuf[1024];
    uint64_t seconds = 0;
    uint32_t nanoseconds = 0;

    if(!TimeSync_isEnabled(timeSyncHandle))
    {
        UART_printf("\n\r PTP is disabled. Forgot to call ptpEnable() ?");
    }

    else
    {
        sprintf(pBuf, "\n\rLine Delay :\t\t\t%d ns",
                (int)timeSyncHandle->tsRunTimeVar->pathDelay[timeSyncHandle->tsRunTimeVar->syncPortNum
                        - 1]);
        UART_printf(pBuf);

        UART_printf("\n\r\n\r--------DUT configured as Slave---------");

        TimeSync_getCurrentTime(timeSyncHandle, &nanoseconds, &seconds);

        UART_printf("\n\r*********PTP/1588 Params********");
        sprintf(pBuf, "\n\rClock Drift :\t\t\t\t%d ns",
                (int)timeSyncHandle->tsRunTimeVar->clockDrift);
        UART_printf(pBuf);
        sprintf(pBuf, "\n\rCurr offset :\t\t\t\t%d ns",
                (int)timeSyncHandle->tsRunTimeVar->currOffset);
        UART_printf(pBuf);
#ifdef PTP_TESTING
        sprintf(pBuf, "\n\rMin offset :\t\t\t\t%d ns",
                (int)min_offset);
        UART_printf(pBuf);
        sprintf(pBuf, "\n\rMax offset :\t\t\t\t%d ns",
                (int)max_offset);
        UART_printf(pBuf);
        sprintf(pBuf, "\n\rNum Sync Missed :\t\t\t%d",
                (int)(int)timeSyncHandle->numSyncMissed);
        UART_printf(pBuf);
#endif //PTP_TESTING
        sprintf(pBuf, "\n\rUTC Offset (Seconds field) :\t\t%ld seconds",
                (long int)seconds);
        UART_printf(pBuf);
        sprintf(pBuf, "\n\rMaster connected on Port :\t\t%d",
                (int)timeSyncHandle->tsRunTimeVar->syncPortNum);
        UART_printf(pBuf);
        UART_printf("\n\r*******************************");

    }

}

#ifdef PTP_TESTING
void monitorPTPStatus_Task(UArg arg0, UArg arg1)
{
    TimeSync_ParamsHandle_t timeSyncHandle = NULL;
    timeSyncHandle = (TimeSync_ParamsHandle_t)arg0;

    while(1)
    {
        if(timeSyncHandle->tsRunTimeVar->stateMachine &
                TS_STATE_MACHINE_DEVICE_IN_SYNC)
        {
            if(timeSyncHandle->tsRunTimeVar->currOffset < min_offset)
            {
                min_offset = timeSyncHandle->tsRunTimeVar->currOffset;
            }

            if(timeSyncHandle->tsRunTimeVar->currOffset > max_offset)
            {
                max_offset = timeSyncHandle->tsRunTimeVar->currOffset;
            }
        }

        Task_sleep(500);
    }
}
#endif //PTP_TESTING

/**
 *  @brief  The function prints the operation modes in uart console
 *
 *  @param  none
 *
 *  @retval none
 *
 */
EIP_VOID EIPAPP_printMenu()
{
    UART_printf("\n\r !!! Stack has demo limitation of 1 hour !!!...\n\n\r Menu:");
    UART_printf("\n\r Stack is online, press T to start...\n\n\r Menu:");
    UART_printf("\n\r\tESC-Quit"
                "\n\r\tT-Start/Stop (toggle)"
                "\n\r\tR-Run IO");
    UART_printf("\n\r\tI-Idle IO "
                "\n\r\tC-Copy IN -> OUT (toggle) "
                "\n\r\tU-Increment Data (toggle) "
                "\n\r\tN-Change IP Address "
                "\n\r\tE-Erase Non Volatile Memory"
                "\n\r\tP-Show PTP status"
                "\n\r\tA-configure ACD status | 0 : disable | 1 : enable"
                "\n\r\tK-Read Media Counters"
                "\n\r\tL-Clean Media Counters"
                "\n\r\tM : Multicast filter handling\n\r"
                "\n\r\t");
}

void printMulticastFilterSubMenu(void)
{

    UART_printf("********************************************\n\r");
    UART_printf("\n\rMulticast filter Sub Menu Options. Press key (Upper/Lower)\n");
    UART_printf("D : Disable multicast filtering\n\r");
    UART_printf("E : Enable multicast filtering\n\r");
    UART_printf("M : Configure mask\n\r");
    UART_printf("A : Add multicast MAC ID\n\r");
    UART_printf("R : Remove multicast MAC ID\n\r");
    UART_printf("S : Enable multicast storm prevention\n\r");
    UART_printf("P : Disable multicast storm prevention\n\r");
    UART_printf("********************************************\n\r");
}

int isValidMacid(char *macid_str)
{
    unsigned int n1, n2, n3, n4, n5, n6;

    if(sscanf(macid_str, "%u %u %u %u %u %u", &n1, &n2, &n3, &n4, &n5, &n6) != 6)
    {
        return FALSE;
    }

    if((n1 != 0) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255)
            && (n4 <= 255) && (n5 <= 255) && (n6 <= 255))
    {
        char buf[64];
        sprintf(buf, "%u %u %u %u %u %u", n1, n2, n3, n4, n5, n6);

#if DEBUG_MC_FLT
        UART_printf("buf:%s macid_str:%s",
                    buf, macid_str);
#endif

        if(strcmp(buf, macid_str))
        {
            return 0;
        }

        return TRUE;
    }

    return FALSE;
}

EIP_VOID EIPAPP_printMediaCounter(EIP_WORD wInstance,
                                  CLASS_ETH_MEDIA_COUNTERS *pMediaCnt)
{
    if(wInstance == 1)
    {
        UART_printf("\n\r\tPRU0 Media Counters");
    }

    else
    {
        UART_printf("\n\r\tPRU1 Media Counters");
    }

    UART_printf("\n\r Alignment_Errors:%d",
                pMediaCnt[wInstance - 1].Alignment_Errors);
    UART_printf("\n\r FCS_Errors:%d", pMediaCnt[wInstance - 1].FCS_Errors);
    UART_printf("\n\r Single_Collisions:%d",
                pMediaCnt[wInstance - 1].Single_Collisions);
    UART_printf("\n\r Multiple_Collisions:%d",
                pMediaCnt[wInstance - 1].Multiple_Collisions);
    UART_printf("\n\r Deferred_Transmissions:%d",
                pMediaCnt[wInstance - 1].Deferred_Transmissions);
    UART_printf("\n\r Late_Collisions:%d",
                pMediaCnt[wInstance - 1].Late_Collisions);
    UART_printf("\n\r Excessive_Collisions:%d",
                pMediaCnt[wInstance - 1].Excessive_Collisions);
    UART_printf("\n\r MAC_Transmit_Errors:%d",
                pMediaCnt[wInstance - 1].MAC_Transmit_Errors);
    UART_printf("\n\r Alignment_Errors:%d",
                pMediaCnt[wInstance - 1].Frame_Too_Long);
    UART_printf("\n\r MAC_Receive_Errors:%d",
                pMediaCnt[wInstance - 1].MAC_Receive_Errors);
}


/**
 *  @brief  The function initialize the adapter operations
 *
 *  @param  pstEipInit
 *  @param pstEipOnline
 *
 *  @retval EIP_TRUE  - Success
 *          EIP_FALSE - Failure
 *
 */
static int EIPAPP_initSample(ST_EIP_INIT *pstEipInit,
                             ST_EIP_ONLINE *pstEipOnline)
{
    if(EIP_Init(pstEipInit) == EIP_FALSE)
    {
        return EIP_FALSE;
    }

    /* EIP Stack Configuration*/
    if(EIP_Configure(gpbyFilePtr, gpstSharedMemory) == EIP_FALSE)
    {
        printf("Fatal Error: EIP_Configure failed! \n");
        return EIP_FALSE;
    }

    if(EIPDEV_userOBJInit() == EIP_FALSE)
    {
        return EIP_FALSE;
    }

    /* go online*/
    if(EIP_Online(pstEipOnline) == EIP_FALSE)
    {
        return EIP_FALSE;
    }

    /*menu management*/
    EIPAPP_printMenu();

    return EIP_TRUE;
}

/**
 *  @brief  The function exits the adapter operations
 *
 *  @param  none
 *
 *  @retval none
 *
 */
static EIP_VOID EIPAPP_exitSample()
{
    eipAppStat = 0;
    EIP_Stop();
    EIP_Offline();
    EIP_Exit();
}


/**
 *  @brief  The function starts the EIP application
 *
 *  @param  none
 *
 *  @retval  0  - On success
 *          <0  - On failure
 *
 */
int EIPAPP_init()
{
    ST_EIP_INIT stEipInit = {0};
    ST_EIP_ONLINE stEipOnline = {0};

    /* Application Initialization*/
    if(EIPAPP_initApp() != 0)
    {
        return -1;
    }

    EIPHAL_setTCPIPInitConfig();

    gdwCapabilities |= EIP_CAPABILITY_ACD;
    gdwMask = eipDevTcpIpConfiguration.Network_Mask;;
    /* EIP Stack Initialization */
    stEipOnline.dwLocalIP            = eipDevTcpIpConfiguration.IP_Address;
    stEipOnline.dwSubNetMask           = eipDevTcpIpConfiguration.Network_Mask;
    stEipInit.wExpMsgTimeOut       = (EIP_WORD)10000;
    stEipInit.dwCapabilities       = gdwCapabilities;


    if(!EIPAPP_initSample(&stEipInit, &stEipOnline))
    {
        return -1;
    }

    if(!EIP_Start())
    {
        UART_printf("\n\r Error: EIP_Start failed!");
        return -1;
    }

    else
    {
        UART_printf("\n\r Start IO");
    }

    return 0;
}

/**
 *  @brief  The function starts the EIP application
 *
 *  @param  resetType [in] 1 - Type 0 reset
 *                         2 - Type 1 reset
 *
 *  @retval  EIP_TRUE  - On success
 *
 */
EIP_BOOL EIPAPP_resetImplement(EIP_DWORD resetType)
{
    ST_ETH_CONTROL   stEthControl = {0};
    ST_TCPIP_CONTROL tcpConfigControl = {0};
    CLASS_TCP_LAST_CONFLICT_DETECTED lastConflictParams = {0};
    CLASS_QOS userQOSObj;
    EIP_WORD acdEnablereset = 1;
    uint32_t blockNum, pageNum;

    if(resetType == 1) /*Type 0 reset*/
    {
        tcpConfigControl.wStartupConfiguration = EIP_STARTUP_STATICIP;
        EIPHAL_setConfigurationControl(&tcpConfigControl);
        PRUICSS_pruDisable(handle, 1);
        PRUICSS_pruDisable(handle, 0);
        EIPAPP_exitSample();

        ptpDrvStackDeInit(timeSyncHandle);

        NC_NetStop(0);
        EIPAPP_restartApplication();
    }

    else if(resetType == 2) /*Type 1 reset*/
    {

        userQOSObj.byDscpUrgent      = 55; /* Urgent */
        userQOSObj.byDscpScheduled   = 47; /* Scheduled */
        userQOSObj.byDscpHigh        = 43; /* High */
        userQOSObj.byDscpLow         = 31; /* Low */
        userQOSObj.byDscpExplicit    = 27; /* Explicit */
        tcpConfigControl.wStartupConfiguration = EIP_STARTUP_DHCP;
        EIPHAL_setConfigurationControl(&tcpConfigControl);
        EIPHAL_setLastConflictDetails(&lastConflictParams);
        EIPHAL_setACDEnable(&acdEnablereset);
        EIPHAL_setQoSValues(&userQOSObj);
        stEthControl.bAutoNegotiate = 1;
        stEthControl.bForceFullDuplex = 0;
        stEthControl.wForceInterfaceSpeed = 0;

        Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                                   &blockNum, &pageNum);

        Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
        Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                         (uint8_t *)&stEthControl, sizeof(ST_ETH_CONTROL), NULL);

        PRUICSS_pruDisable(handle, 1);
        PRUICSS_pruDisable(handle, 0);
        EIPAPP_exitSample();

        ptpDrvStackDeInit(timeSyncHandle);

        NC_NetStop(0);
        EIPAPP_restartApplication();
    }

    return EIP_TRUE;
}

/*Registering Port 1 Link break callback */
void EIP_DLR_TIMESYNC_port0ProcessLinkBrk(uint8_t linkStatus, void *arg2)
{
    EIP_DLR_port0ProcessLinkBrk(linkStatus, (void *)(eipHandle->dlrHandle));
    TimeSync_Port1linkResetCallBack(linkStatus,
                                    (void *)(eipHandle->timeSyncHandle));
}

/*Registering Port 2 Link break callback */
void EIP_DLR_TIMESYNC_port1ProcessLinkBrk(uint8_t linkStatus, void *arg2)
{
    EIP_DLR_port1ProcessLinkBrk(linkStatus, (void *)(eipHandle->dlrHandle));
    TimeSync_Port2linkResetCallBack(linkStatus,
                                    (void *)(eipHandle->timeSyncHandle));
}

/**
 *  @brief  Main function called to start EIP application and Stack
 *
 *  @param  eipicssEmacHandle [in] - EMAC LLD handle
 *
 *  @retval  0  - When the EIP app is stopped
 *
 */
int EIPAPP_main(ICSSEMAC_Handle eipicssEmacHandle)
{

    char key = 0;
    uint8_t ipCounter = 0;
    EIP_BOOL  bStopped = EIP_TRUE;
    char bkupMacidString[30];
    unsigned int acdStatus;
    uint32_t *controlPointer;
    uint32_t temp_addr = 0U;
    ICSS_EmacFwStaticMmap *pStaticMMap = (&((ICSS_EmacObject *)
                                            emachandle->object)->fwStaticMMap);

    SO_Open_Session();

    /* Do EIP Application specific Init*/
    EIPAPP_getInitParams(emachandle);



    if(eipDevInitConfig.acdEnable)
    {
        EIPACD_init();
        ((((ICSS_EmacObject *)
           eipicssEmacHandle->object)->callBackHandle)->txCallBack)->callBack =
               (ICSS_EmacCallBack)EIPACD_checkGratArp;
    }

    Semaphore_post(appInitSem);

    EIPUTILS_displayMacid();
    EIPUTILS_displayApplicationDetails();

    while(!acdIPAssign)
    {
        if(eipDevInitConfig.tcpipConfig.startupType == EIP_STARTUP_DHCP)
        {
            if(user_input_processing == 1)
            {
                if(!ipAddrFlag)
                {
                    if((rxByte == 'N') || (rxByte == 'n'))
                    {
                        ipAddrFlag = 1;
                        UART_printf("\n\rEnter IP Address and Press Enter");
                    }
                }

                else
                {
                    if(EIPUTILS_assignUserIP(chIpAddr))
                    {
                        EIPAPP_restartApplication();
                    }

                    else
                    {
                        UART_printf("\n\rInvalid IP Address Entered");
                    }
                }

                user_input_processing = 0;
            }
        }

        Task_sleep(200);
    }

    EIPUTILS_displayIPDetails();

    EIP_configurePHYLeds();
    EIP_drvInit(eipHandle);
    EIP_drvStart(eipHandle);

    if(!eipDevInitConfig.acdEnable)
    {
        ICSS_EmacRegisterPort0ISRCallback(eipicssEmacHandle,
                                          (ICSS_EmacCallBack)EIP_DLR_TIMESYNC_port0ProcessLinkBrk, (void *)(eipHandle));
        /*Port 1 Link break callback */
        ICSS_EmacRegisterPort1ISRCallback(eipicssEmacHandle,
                                          (ICSS_EmacCallBack)EIP_DLR_TIMESYNC_port1ProcessLinkBrk, (void *)(eipHandle));

        EIPHAL_setEthernetLinkConfig();

        ptpDrvStackInit(timeSyncHandle);
    }


    EIPAPP_init();

    bStopped = !bStopped;
    IO_SetModeRun();
    ModeRunning = EIP_TRUE;
    UART_printf("\n\r EIP Set Mode => RUN");
    eipAppStat = 1;
    CLASS_ETH_MEDIA_COUNTERS MediaCnt;

    do
    {
        if((user_input_processing == 1) || gbResetIdentity)
        {
            if(gbResetIdentity)
            {
                if(EIPAPP_resetImplement(gbResetIdentity))
                {
                    gbResetIdentity = EIP_FALSE;
                }
            }

            key = rxByte;

            switch(key)
            {
                case 't':
                case 'T':
                    if(bStopped)
                    {
                        if(!EIP_Start())
                        {
                            UART_printf("\n\r Error: EIP_Start failed!");
                        }

                        else
                        {
                            UART_printf("\n\r Start IO");
                        }
                    }

                    else
                    {
                        if(!EIP_Stop())
                        {
                            UART_printf("\n\r Error: EIP_Stop failed!");
                        }

                        else
                        {
                            UART_printf("\n\r Stop IO");
                        }
                    }

                    bStopped = !bStopped;
                    break;

                /* COPY IN -> OUT*/
                case 'c':
                case 'C':
                    EIPDEV_sHMCopyInOut();
                    break;

                /* INC OUT*/
                case 'u':
                case 'U':
                    EIPDEV_sHMIncOut();
                    break;

                case 'i':
                case 'I':
                    IO_SetModeIdle();
                    ModeRunning = EIP_FALSE;
                    UART_printf("\n\r EIP Set Mode => IDLE");
                    break;

                case 'r':
                case 'R':
                    IO_SetModeRun();
                    ModeRunning = EIP_TRUE;
                    UART_printf("\n\r EIP Set Mode => RUN");
                    break;

                case 'a':
                case 'A':
                    UART_gets(bkupMacidString, -2);
                    sscanf(bkupMacidString, "%u", &acdStatus);
                    EIPHAL_setACDEnable((EIP_WORD *)&acdStatus);

                    EIPAPP_exitSample();
                    ptpDrvStackDeInit(timeSyncHandle);
                    NC_NetStop(0);
                    EIPAPP_restartApplication();
                    break;

                case 'n':
                case 'N':
                    ipAddrFlag = 1;
                    UART_printf("\n\rEnter IP Address and Press Enter");
                    user_input_processing = 0;
                    ipCounter = 0;

                    while((!user_input_processing) && (ipCounter < 40))  // Increase ipCounter to insert IpAdress
                    {
                        ipCounter++;
                        Task_sleep(1000);
                    }

                    if(!EIPUTILS_assignUserIP(chIpAddr))
                    {
                        UART_printf("\n\rInvalid IP Address Entered");
                        EIPAPP_printMenu();
                    }

                    else
                    {
                        EIPAPP_exitSample();
                        ptpDrvStackDeInit(timeSyncHandle);
                        NC_NetStop(0);
                        EIPAPP_restartApplication();

                    }

                    user_input_processing = 1;
                    break;

                case 'e':
                case 'E':
                    EIPAPP_resetImplement(2);
                    UART_printf("\n\r Non volatile memory Erased.");
                    EIPAPP_printMenu();
                    break;

                case 'p':
                case 'P':
                    printPTPStatus(timeSyncHandle);
                    EIPAPP_printMenu();
                    break;

                case 'k':
                case 'K':
                    EIPHAL_getMediaCounters((EIP_WORD)1, &MediaCnt, EIP_FALSE);
                    EIPAPP_printMediaCounter((EIP_WORD)1, &MediaCnt);
                    EIPHAL_getMediaCounters((EIP_WORD)2, &MediaCnt, EIP_FALSE);
                    EIPAPP_printMediaCounter((EIP_WORD)2, &MediaCnt);
                    EIPAPP_printMenu();
                    break;

                case 'l':
                case 'L':
                    EIPHAL_getMediaCounters((EIP_WORD)1, &MediaCnt, EIP_TRUE);
                    EIPHAL_getMediaCounters((EIP_WORD)2, &MediaCnt, EIP_TRUE);
                    break;

                case 'm':
                case 'M':

                    temp_addr = ((((ICSS_EmacHwAttrs *)
                                   emachandle->hwAttrs)->emacBaseAddrCfg)->dataRam0BaseAddr +
                                 pStaticMMap->featureOffset);
                    controlPointer = (uint32_t *)(temp_addr);

                    if(*controlPointer & (1 << FEATURE_MULTICAST_FILTER_SHIFT))
                    {
                        printMulticastFilterSubMenu();
                        rxByte = (char) UART_getc();

                        if((rxByte == 'D') || (rxByte == 'd'))
                        {
                            eip_multicast_filter_config(emachandle, MC_FLTR_DIS);
                        }

                        else if((rxByte == 'E') || (rxByte == 'e'))
                        {
                            eip_multicast_filter_config(emachandle, MC_FLTR_ENA);
                        }

                        else if((rxByte == 'S') || (rxByte == 's'))
                        {
                            eip_multicast_stromPrev_config(emachandle, MC_SP_ENA);
                        }

                        else if((rxByte == 'P') || (rxByte == 'p'))
                        {
                            eip_multicast_stromPrev_config(emachandle, MC_SP_DIS);
                        }

                        else
                        {
                            char bkupMacidString[30];

                            UART_gets(bkupMacidString, -2);
#if DEBUG_MC_FLT
                            UART_puts(bkupMacidString, -2);
#endif

                            if(isValidMacid(bkupMacidString))
                            {
                                if((rxByte == 'M') || (rxByte == 'm'))
                                {
                                    eip_multicast_filter_override_hashmask(emachandle, bkupMacidString);
                                    UART_printf("\n\rSuccessfully configured new mask");
                                }

                                else if((rxByte == 'A') || (rxByte == 'a'))
                                {
                                    eip_multicast_filter_update_macid(emachandle, bkupMacidString,
                                                                      ADD_MC_MAC_ID);
                                    UART_printf("\n\rSuccessfully added");
                                }

                                else if((rxByte == 'R') || (rxByte == 'r'))
                                {
                                    eip_multicast_filter_update_macid(emachandle, bkupMacidString,
                                                                      REM_MC_MAC_ID);
                                    UART_printf("\n\rSuccessfully removed");
                                }
                            }
                        }
                    }

                    else
                    {
                        Debug_Printf(DBG_WARN, "Multicast filtering not supported in firmware!\n");
                    }

                    break;

                default:
                    EIPAPP_printMenu();
                    break;
            }

            user_input_processing = 0;
        }

        if(acdEIPStatus == EIP_ACD_START)
        {
            if(eipAppStat != 1)
            {
                EIPAPP_init();
                bStopped = !bStopped;
                IO_SetModeRun();
                ModeRunning = EIP_TRUE;
                UART_printf("\n\r EIP Set Mode => RUN");
                acdEIPStatus = EIP_ACD_DEFAULT;
                eipAppStat = 1;
            }
        }

        if(acdEIPStatus == EIP_ACD_RESTART)
        {

            Board_phyMDIXFixDeInit();

            EIPAPP_exitSample();

            ptpDrvStackDeInit(timeSyncHandle);
            EIPACD_stop();
            NC_NetStop(ACD_LINKDOWN);
            acdEIPStatus = EIP_ACD_DEFAULT;
        }

        if(acdEIPStatus == EIP_ACD_STOP)
        {

            Board_phyMDIXFixDeInit();

            EIPAPP_exitSample();

            ptpDrvStackDeInit(timeSyncHandle);

            NC_NetStop(0);
            acdEIPStatus = EIP_ACD_DEFAULT;
        }

        Task_sleep(500);

    }
    while(key != ESC_KEY);

    EIPAPP_exitSample();
    EIPAPP_exitApp();

    SO_Close_Session();
    return 0;
}
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
void EIPAPP_uartTask(UArg a0, UArg a1)
{

    while(1)
    {
        if(ipAddrFlag)
        {
            UART_gets(chIpAddr, -2);
        }

        else
        {
            rxByte = (char) UART_getc();
        }

        user_input_processing = 1;

        while(user_input_processing)
        {
            Task_sleep(2500);
        }
    }
}

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
EIP_VOID EIPAPP_ipAddressChange(Uint32 ipAddress, Uint32 ipAddAssign)
{
    if(ipAddAssign)
    {
        acdIPAssign = 1;
        EIP_DLR_addModuleIPAddress(eipHandle->dlrHandle, ipAddress);
        TimeSync_addIP(timeSyncHandle, ipAddress);

    }

    else
    {
        acdIPAssign = 0;
        EIP_DLR_addModuleIPAddress(eipHandle->dlrHandle, 0);
        TimeSync_addIP(timeSyncHandle, 0);

    }
}


/**
 *  @brief  Gets the Initial configuration from non volatile memory for Application startup
 *
 *
 *  @param  eipicssEmacHandle [in[ EMAC LLD handle
 *
 *  @retval  EIP_TRUE - On success
 *
 */
EIP_BOOL EIPAPP_getInitParams(ICSSEMAC_Handle eipicssEmacHandle)
{
    uint16_t incarnId = 0;
    /*Allocate memory for PTP Object*/
    /* Read TCP/IP config stored in SpiFlash */
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_DEVICEIP_OFFSET,
                    (uint8_t *)& eipDevInitConfig.tcpipConfig, sizeof(EPROM_TCPIP), NULL);

    /*IF IP from flash is 0x0 start in DHCP mode*/
    if(!eipDevInitConfig.tcpipConfig.deviceIP)
    {
        eipDevInitConfig.tcpipConfig.startupType = EIP_STARTUP_DHCP;
    }

    /* Read ACD Enable bit stored in SpiFlash */
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_OFFSET,
                    (uint8_t *)& eipDevInitConfig.acdEnable, sizeof(EIP_BYTE), NULL);
    /* Read Ethernet Link Config stored in SpiFlash */
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                    (uint8_t *)& eipDevInitConfig.ethConfig[0], sizeof(ST_ETH_CONTROL), NULL);
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_SPEED_OFFSET,
                    (uint8_t *)& eipDevInitConfig.ethConfig[0], sizeof(ST_ETH_CONTROL), NULL);
    /* Read ACD Conflict detectd bit from SPI Flash*/
    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_CONFLICT_OFFSET,
                    (uint8_t *)& eipDevInitConfig.acdConflictDetails,
                    sizeof(CLASS_TCP_LAST_CONFLICT_DETECTED), NULL);

    Board_flashRead((Board_flashHandle)flashHandle, SPI_EEPROM_INCARN_OFFSET, (uint8_t *)&incarnId,
                    sizeof(EIP_WORD), NULL);

    eipDevInitConfig.incarnNum = incarnId;

    if(eipDevInitConfig.acdEnable)
    {
        eipDevInitConfig.acdEnable = 1;
    }

    return EIP_TRUE;
}

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
uint8_t GetLinkStatus(uint8_t portnum)
{
    return(((ICSS_EmacObject *)emachandle->object)->linkStatus[portnum - 1]);
}
