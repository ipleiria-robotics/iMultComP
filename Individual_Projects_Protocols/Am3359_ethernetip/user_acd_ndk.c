/**
 * @file user_acd_ndk.c
 * @brief Contains Functions which implement ACD mechanism.
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
#include <string.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/stack/inc/resif.h>
#include <ti/ndk/inc/stack/inc/netdf.h>
#include <ti/ndk/inc/stack/inc/listlib.h>
#include <ti/ndk/inc/stack/inc/nimuif.h>
#include <ti/ndk/inc/stack/inc/lliif.h>
#include <Include/protocol/icss_dlr.h>
#include <Include/protocol/icss_ptpd_driver.h>
#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/icss_emac/icss_emacCommon.h>
#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>

#include <Include/protocol/eip_main.h>
#include <Include/board/board_spi.h>
#include <Include/protocol/user_acd_ndk.h>


#define BUFSIZE (ARPHDR_SIZE + ETHHDR_SIZE)
#define ARPBUFSIZE (BUFSIZE * 32)
#define ACDPERIOD 10   /* 5 ms */

/**Message definition */
#define PORT1_MSG_LINKUP    1
#define PORT1_MSG_LINKDOWN  2
#define MSG_ARP             3
#define MSG_NETSTOP         4
#define PORT2_MSG_LINKUP    5
#define PORT2_MSG_LINKDOWN  6
#define PORTS_MSG_LINKDOWN  7

#define USE_ACD_ARP 1
#define USE_NDK_ARP 0

#define NUMMSGS       16
#define NIMUINDEX 1

typedef struct ACD_Object
{
    Mailbox_Handle mbox;
    Task_Handle task;
    Semaphore_Handle validSem;
} ACD_Object;

/***/
unsigned int acdEventMissed = 0;
/***/
static ACD_Object acdObj;
/**Global holder to save the ARP frame received from NDK stack*/
static void * arpq;
/**Flag which informs whether an IP is obtained*/
static int validAddress;
/**Local variable to store MAC ID of the device*/
static unsigned char macAddr[6];
/**Flag to inform the LED task regarding the ACD status*/
unsigned int ledACD = 0;
/**Flag to indicate ACD Stack is started*/
unsigned int acdStart = 0;
/**Flag to indicate ACD Link Down event occured*/
unsigned int acdCablePull = 0;
/**Flag to indicate IP is assigned to the device*/
unsigned int acdIPAssign = 0;
/**Variable to indicate ACD or TCP/IP stack to process ARP*/
unsigned int acdARPFlag = USE_ACD_ARP;
/**Variable which informs whether both link were down or not. Use*/
unsigned int linkDownStat = 0;
/**Local array to store Link status*/
Uint8 eipLinkStat[2] = {0};

/**Variable used when ACD work around for gratitous arp is in use*/
extern unsigned int countACD;
extern S25FL_Handle flashHandle;
extern TimeSync_ParamsHandle_t timeSyncHandle;
extern EIP_Handle eipHandle;
extern int startupMode;
extern unsigned int eipAppStat;

/**
 *  @brief  Function which informs the ACD monitor task on the Link status change
 *
 *
 *  @param linkStatus [in] Link Status
 *  @param portNum    [in] Portnumber
 *
 *  @retval  none
 *
 */
void EIPACD_updateLinkStatus(Uint32 linkStatus , Uint32 portNum)
{

    Uint8 validMessage = 1;
    UInt msg;

    if(linkStatus)
    {
        if(eipLinkStat[portNum])    /* Make sure speed or duplexity changes doesnot send ACD ARP*/
        {
            validMessage = 0;
        }

        else
        {
            msg = (portNum * 4) + 1;
        }

        eipLinkStat[portNum] = 1;
        linkDownStat = 1;
    }

    else
    {
        eipLinkStat[portNum] = 0;

        if(eipLinkStat[0] | eipLinkStat[1])
        {
            msg = (portNum * 4) + 2;
            linkDownStat = 1;
        }

        else
        {
            if(!linkDownStat)
            {
                validMessage = 0;
            }

            msg = PORTS_MSG_LINKDOWN;
            linkDownStat = 0;
        }
    }

    if(validMessage)
    {
        Mailbox_post(acdObj.mbox, &msg, 0);
    }
}
/**
 *  @brief  Link change callback for Port0
 *
 *          This function will be called by the EMACLLD whenever linkstatus changes in Port0.
 *          Calls DLR API to inform DLR driver about the Link change and informs the ACD monitor task
 *          on the Link status change
 *
 *
 *  @param linkStatus [in] Link Status on Port0
 *  @param arg        [in] Emac handle
 *
 *  @retval  none
 *
 */
void EIPACD_linkStatusPort0(uint8_t linkStatus, UArg arg)
{
    EIP_DLR_TIMESYNC_port0ProcessLinkBrk(linkStatus,
                                         eipHandle);    /*Link status based operations on DLR & timeSync*/
    EIPACD_updateLinkStatus(linkStatus ,
                            0);   /*Link status based operations for ACD*/
}
/**
 *  @brief  Link change callback for Port1
 *
 *          This function will be called by the EMACLLD whenever linkstatus changes in Port0.
 *          Calls DLR API to inform DLR driver about the Link change and informs the ACD monitor task
 *          on the Link status change
 *
 *
 *  @param linkStatus [in] Link Status on Port1
 *  @param arg        [in] Emac handle
 *
 *  @retval  none
 *
 */
void EIPACD_linkStatusPort1(uint8_t linkStatus, UArg arg)
{
    EIP_DLR_TIMESYNC_port1ProcessLinkBrk(linkStatus,
                                         eipHandle);    /*Link status based operations on DLR & timeSync*/
    EIPACD_updateLinkStatus(linkStatus ,
                            1);   /*Link status based operations for ACD*/
}

/**
 *  @brief  ARP hook function
 *
 *          NDK stack will call this function whenever the ARP is received.
 *
 *          This is required as the ARP frames need to be passed to ACD stack
 *
 *  @param   buf [in] ARP frame pointer
 *
 *  @retval  none
 *
 */
static void EIPACD_queueARP(void *buf)
{
    if(acdStart)
    {
        UInt msg = MSG_ARP;
        SBWrite(arpq, BUFSIZE, buf, NULL);
        Mailbox_post(acdObj.mbox, &msg, 0);
    }
}


/**
 *  @brief  IP address assign hook function
 *
 *          NDK stack will call this function before IP address is assigned.Operations to be done before IP address assignment
 *          should be done here
 *
 *          Gets the Link status and starts the ACD
 *
 *  @param   ipAddr [in] IP Address
 *
 *  @retval  validAddress IPaddress which is obtained be ACD stack
 *
 */
static int EIPACD_validateIPAddress(uint32_t ipAddr)
{
    NIMU_IF_REQ ifReq;
    ifReq.name[0] = 0;
    ifReq.index = NIMUINDEX;

    if(NIMUIoctl(NIMU_GET_DEVICE_MAC, &ifReq, macAddr, sizeof(macAddr)))
    {
        return FALSE;
    }



    EIPACD_registerLinkCallBack(eipHandle->emacHandle);

    ACD_Start(htonl(ipAddr), macAddr, 0);
    EIPACD_start();

    /*Do Configs and Callback to be done before IP Assign*/
    EIPHAL_setEthernetLinkConfig();

    if((eipDevInitConfig.ethConfig[0].bAutoNegotiate)
            && (eipDevInitConfig.tcpipConfig.startupType == EIP_STARTUP_DHCP))
    {
        if(((ICSS_EmacObject *)(eipHandle->emacHandle)->object)->linkStatus[0])
        {
            EIPACD_updateLinkStatus(1, 0);
        }

        if(((ICSS_EmacObject *)(eipHandle->emacHandle)->object)->linkStatus[1])
        {
            EIPACD_updateLinkStatus(1, 1);
        }
    }

    ptpDrvStackInit(timeSyncHandle);

    /* wait for the ACD state machine to check address */
    Semaphore_pend(acdObj.validSem, BIOS_WAIT_FOREVER);

    return (validAddress);
}

/**
 *  @brief  Function receives the ARP packet from ACD Stack and sends it out
 *
 *
 *  @param  arp   [in]  ARP packet pointer
 *  @param  IPDst [in]  Destination IP address if any
 *
 *  @retval none
 *
 */
static void EIPACD_sendNDKArpPacket(unsigned char *arp, uint32_t IPDst)
{
    PBM_Pkt *pkt;
    ARPHDR *arpHdr;
    NETIF_DEVICE *nimu;
    static uint8_t BROADCAST[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    if((nimu = NIMUFindByIndex(NIMUINDEX)) == NULL)
    {
        return;
    }

    if(!(pkt = NIMUCreatePacket(ARPHDR_SIZE)))
    {
        return;
    }

    /* ACD module includes an Ethernet header, so skip it */
    arpHdr = (ARPHDR *)(arp + ETHHDR_SIZE);
    memcpy(pkt->pDataBuffer + pkt->DataOffset, arpHdr, ARPHDR_SIZE);
    pkt->ValidLen = ARPHDR_SIZE;

    llEnter();

    if(NIMUAddHeader(nimu, (void *)pkt, BROADCAST, NULL, 0x806) == 0)
    {
        /* Layer 2 header has been added successfully; now only can we send the
         * packet through the NIMU Interface. */
        NIMUSendPacket(nimu, pkt);
    }

    else
    {
        PBM_free(pkt);
    }

    llExit();
}

/**
 *  @brief  Function used by ACD monitor task to reveive the ARP frame from frame holde
 *
 *          The ARP hook function receives the ARP fram from NDK to the global holder arpq.This function copies the frame to
 *          the ACD stack frame holder
 *
 *  @param  arp   [in]  ARP packet pointer
 *  @param  IPDst [in]  Destination IP address if any
 *
 *  @retval none
 *
 */
static int32_t EIPACD_readARP(unsigned char *buf)
{
    UInt key;
    int size;

    key = Hwi_disable();
    size = SBRead(arpq, BUFSIZE, 0, buf, NULL, NULL, 0);
    Hwi_restore(key);

    return (size);
}
/**
 *  @brief  ACD monitor Task definition
 *
 *
 *  @param   arg0 [in] Argument if any
 *  @param   arg1 [in] Argument if any
 *
 *  @retval  none
 *
 */
static void EIPACD_acdMonitor(UArg arg0, UArg arg1)
{
    ACD_Object *acdObj = (ACD_Object *)arg0;
    unsigned char arp[ARPHDR_SIZE + ETHHDR_SIZE];
    uint32_t msg;
    Semaphore_Handle clockSem;
    Clock_Params clockParams;
    Semaphore_Params semParams1;
    Semaphore_Params_init(&semParams1);
    semParams1.mode = Semaphore_Mode_BINARY;

    clockSem = Semaphore_create(0, &semParams1, NULL);

    Clock_Params_init(&clockParams);
    clockParams.period = ACDPERIOD;
    clockParams.arg = (UArg)clockSem;
    clockParams.startFlag = TRUE;
    Clock_create((Clock_FuncPtr)Semaphore_post, ACDPERIOD, &clockParams, NULL);

    while(1)
    {
        Semaphore_pend(clockSem, BIOS_WAIT_FOREVER);

        ACD_IncTick();

        if(acdEventMissed)
        {
            if(eipAppStat && (!linkDownStat))
            {
                acdCablePull = 1;
                ACD_Link(0, 0);
                ACD_Link(1, 0);
                ACD_Stop();
                acdEIPStatus = EIP_ACD_RESTART;
                acdEventMissed = 0;
            }
        }

        if(Mailbox_pend(acdObj->mbox, &msg, BIOS_NO_WAIT))
        {
            switch(msg)
            {
                case PORT1_MSG_LINKUP:
                    if(startupMode == EIP_STARTUP_DHCP)
                    {
                        Task_sleep(6000);
                    }

                    ACD_Link(0, 1);
                    acdEventMissed = 0;
                    break;

                case PORT1_MSG_LINKDOWN:
                    ACD_Link(0, 0);
                    break;

                case PORT2_MSG_LINKUP:
                    if(startupMode == EIP_STARTUP_DHCP)
                    {
                        Task_sleep(6000);
                    }

                    ACD_Link(1, 1);
                    acdEventMissed = 0;
                    break;

                case PORT2_MSG_LINKDOWN:
                    ACD_Link(1, 0);
                    break;

                case PORTS_MSG_LINKDOWN:
                    if(eipAppStat)    /* Donot restart in the when EIP is not UP*/
                    {
                        /*set flag to indicate cable pull*/
                        /* and check instead od dhcp chek in register ppfg*/
                        acdCablePull = 1;
                        ACD_Link(0, 0);
                        ACD_Link(1, 0);
                        ACD_Stop();
                        acdEIPStatus = EIP_ACD_RESTART;
                    }

                    else
                    {
                        acdEventMissed = 1;
                    }

                    break;

                case MSG_ARP:
                    EIPACD_readARP(arp);
                    ACD_RcvArpFrame(ARPHDR_SIZE + ETHHDR_SIZE, arp);
                    break;

                case MSG_NETSTOP:
                    Task_exit();
                    break;

                default:
                    break;
            }
        }
    }
}

/**
 *  @brief  Function to init the ACD monitor mechanism
 *
 *          Creates the holder to receive the ARP frame.Sets the NDK hooks required for ACD
 *
 *  @param   none
 *  @retval  none
 *
 */
void EIPACD_init(void)
{
    arpq = SBNew(ARPBUFSIZE, 0, SB_MODE_LINEAR);

    if(arpq == NULL)
    {
    }

    /** Set ARP hook.*/
    LLI_setARPHook(EIPACD_queueARP);
    /** Set IP address assign hook.*/
    NS_setAddrHook(EIPACD_validateIPAddress);

    if(!ACD_Init())
    {
        System_abort("ACD_Init() failed\n");
    }
}

/**
 *  @brief  Function to exit ACD
 *
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
EIP_VOID EIPACD_exit()
{
    ACD_Exit();
}

/**
 *  @brief  Function to register the Link change callback
 *
 *          This function internally calls the EMAC LLD API to register the Link change callback
 *
 *
 *  @param   icssEmacHandle [in] ICSS EMAC handle
 *  @retval  none
 *
 */
void EIPACD_registerLinkCallBack(ICSSEMAC_Handle icssEmacHandle)
{
    ICSS_EmacRegisterPort1ISRCallback(icssEmacHandle,
                                      (ICSS_EmacCallBack)EIPACD_linkStatusPort1,
                                      (void *)icssEmacHandle);
    ICSS_EmacRegisterPort0ISRCallback(icssEmacHandle,
                                      (ICSS_EmacCallBack)EIPACD_linkStatusPort0,
                                      (void *)icssEmacHandle);
}

/**
 *  @brief  Starts the ACD Monitor mechanism
 *
 *          Creates ACD monitor task, semaphore and acdobj mailbox
 *
 *  @param   none
 *  @retval  Success - TRUE
 *           Fail   -  FALSE
 *
 */
Bool EIPACD_start(void)
{
    Task_Params taskParams;

    acdObj.mbox = Mailbox_create(sizeof(UInt), NUMMSGS, NULL, NULL);

    Task_Params_init(&taskParams);
    taskParams.instance->name = "UserACD_Monitor";
    taskParams.arg0 = (UArg)&acdObj;
    taskParams.priority = 8;
    acdObj.task = Task_create(EIPACD_acdMonitor, &taskParams, NULL);

    acdObj.validSem = Semaphore_create(0, NULL, NULL);

    acdStart = 1;
    return (TRUE);
}


/**
 *  @brief  Stops the ACD Monitor Task
 *
 *          Deletes the task,mailbox and semaphore used by the ACD monitor mechanism
 *
 *  @param   none
 *  @retval  none
 *
 */
void EIPACD_stop()
{
    uint32_t msg = MSG_NETSTOP;
    Mailbox_post(acdObj.mbox, &msg, 0);

    do
    {
        Task_sleep(1);
    }
    while(Task_getMode(acdObj.task) != Task_Mode_TERMINATED);

    Mailbox_delete(&acdObj.mbox);
    Semaphore_delete(&acdObj.validSem);

    memset(&acdObj, 0, sizeof(acdObj));
    acdStart = 0;

}

/**
 *  @brief  Function to inform the NDK Task about IP address announce/IP address lost
 *
 *          This function needs to be called whenever an IP is lost due to Conflict detection or the IP is announced
 *          after ACD probing
 *
 *  @param   flag [in] Flag to inform IP to be removed or added
 *  @retval  none
 *
 */
static void EIPACD_signalAddress(int flag)
{
    validAddress = flag;
    Semaphore_post(acdObj.validSem);
}

/**
 *  @brief  Function to get the current ACD status.
 *
 *          Used by the LED Task to know whether Conflict has occured or not
 *
 *  @param   none
 *  @retval  1 (ACD_LED_CONFLICT) - In case conflict occured
 *           0  No conflict occured
 */
EIP_WORD EIPACD_getACDLEDStat()
{
    return ledACD;
}

/**
 *  @brief  Function to be called by the ACD stack whenever an event occurs
 *
 *          The ACD stack informs the application about the ACD events using this Function.
 *
 *
 *  @param   stAcdConflictDetected  [in]  Structure containing ACD Conflict details
 *  @retval  Success  -   0
 *           Error   -   <0
 */
EIP_BOOL EIPACD_event(EIP_DWORD dwEvent)
{
    uint32_t event = dwEvent & 0xffff;

    switch(event)
    {
        case ACD_EVENT_IP_LOST:
            acdARPFlag = USE_ACD_ARP;
            ledACD = ACD_LED_CONFLICT; /*Set led flag to indicate IP Address conflict*/
            EIPACD_signalAddress(FALSE);
            ACD_Link(0, 0);
            ACD_Link(1, 0);
            ACD_Stop();
            ACD_Exit();
            EIPACD_stop();
            acdEIPStatus = EIP_ACD_STOP;
            break;

        case ACD_EVENT_IP_ANNOUNCED:
            acdARPFlag = USE_NDK_ARP;
            EIPACD_signalAddress(TRUE);   /*Signal to NDK to use the IP Address*/
            acdEIPStatus = EIP_ACD_START;
            break;

        case ACD_EVENT_LINK_INTEGRITY:
            break;

        case ACD_EVENT_DEFENSE:
            acdARPFlag = USE_ACD_ARP;
            break;

        case ACD_EVENT_NO_LINK:
            break;

        default:
            break;
    }

    return EIP_TRUE;
}

/**
 *  @brief  Function to store the Conflict details to non volatile memory
 *
 *          This Function needs to be called by the ACD Stack whenever an ACD conflict occurs
 *          The Conflict details obtained from stack is stored to Non volatile memory (SPI Flash)
 *
 *
 *  @param   stAcdConflictDetected  [in]  Structure containing ACD Conflict details
 *  @retval  Success  -   0
 *           Error   -   <0
 */
EIP_BOOL EIPACD_conflictDetected(ST_ACD_CONFLICT_DETECTED
                                 stAcdConflictDetected)
{
    uint32_t blockNum, pageNum;
    /*Writes the ACD conflict parameters into non volatile memory*/
    Board_flashOffsetToBlkPage((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_CONFLICT_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk((Board_flashHandle)flashHandle, blockNum);
    Board_flashWrite((Board_flashHandle)flashHandle, SPI_EEPROM_ACD_CONFLICT_OFFSET,
                     (uint8_t *) & (stAcdConflictDetected.byAcdActivity),
                     sizeof(ST_ACD_CONFLICT_DETECTED), NULL);

    return EIP_TRUE;
}

/**
 *  @brief  Function to send out the ARP packet framed by ACD Stack
 *
 *          This Function needs to be called by the ACD Stack to send out ARP frames.This function also count the ARP frames
 *          which is used for Gratitous ARP blocking workaorund
 *
 *
 *  @param   wFrameSize  [in]  size of the frame to be sent
 *  @param   byFrame     [in]  ARP packet pointer
 *  @retval  Success  -   0
 *           Error   -   <0
 */
EIP_BOOL EIPACD_sendArpFrame(EIP_WORD wFrameSize, EIP_BYTE *byFrame)
{
    /*Counted as a part of blocking Gratitous ARP blocking workaorund.Details in acd_arp_filter.c */
    countACD++;

    EIPACD_sendNDKArpPacket(byFrame, 0);
    return EIP_TRUE;
}
