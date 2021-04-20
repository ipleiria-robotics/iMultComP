/**
 * @file iPnOs.c
 *
 * @brief OS dependent code to support PRU-ICSS Profinet driver. This implementation
 * is based on SYSBIOS 6.
 *
 *\par
 * * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *\par
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <xdc/std.h>
#include <ti/ndk/inc/stkmain.h>
#include <ti/ndk/inc/os/osif.h>
#include <ti/sysbios/hal/Hwi.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
/*
 * pnDrvConfig.h needs to be first first file included in driver!
 * It is application dependent and as such part of the application code.
 * It defines all basic feature options in driver (compile time!)
 */



#include <Profinet/include/protocol_slave/RT_MRP/pnDrvConfig.h>

#include <Profinet/include/protocol_slave/PN_Handle.h>
#include <Profinet/include/protocol_slave/PN_HandleDef.h>
#include <Profinet/include/protocol_slave/iRtcDrv.h>
#include <Profinet/include/protocol_slave/iPnOs.h>

#ifdef PTCP_SUPPORT
#include <Profinet/include/protocol_slave/iPtcpDrv.h>
#include <Profinet/include/protocol_slave/iPtcpUtils.h>
#endif

#include <ti/sysbios/gates/GateHwi.h>
#include <ti/csl/hw_types.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/hal/Hwi.h>


#include <Include/Board_am335x/soc_icss_header.h>
#include <osal/TaskP.h>

extern SemaphoreP_Handle switchReady;
/* ========================================================================== */
/*                          Local Variables                                   */
/* ========================================================================== */

#ifdef MRP_SUPPORT
/** @addtogroup PN_MRP
@{ */
/**
* \brief MRP Task to control flush mode of ICSS
*
* @detail In flush mode ICSS will send all PPM on both ports. This task will
* enable/disable this mode based on the MRP state machine @ref tMrpStates .
* It implements a state machine according to ProfinetFlushTask\n
* Implements the following state machine
* @image html mrp_state.png
*
* @param arg0  not used
* @param arg1  not used
*/
void PN_MRP_CPMTask(UArg arg0, UArg arg1);

/**
@}
*/
#endif /* MRP_SUPPORT*/

/**
 * @brief dedicated task to tap the ICSS WatchDog Timer
 *
 *
 * @param arg0 not used
 * @param arg1 not used
 */
#ifdef WATCHDOG_SUPPORT
void PN_tapWatchDog_task(UArg arg0, UArg arg1);
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
#ifdef IRT_LEGACY_STARTUP_SUPPORT

/** @addtogroup PN_IRT_LEGACY
@{ */
/**
* @brief Dedicated task for legacy startup mode scheduling. Manages the state machine
* Required in IRT mode to support legacy startup. States defined by @ref tLegStates.
* Implements the following state machine \n
* @image html irt_legstate.png
*
* @param arg0  not used
* @param arg1  not used
*/
void PN_IRT_legacyTask(UArg arg0, UArg arg1);
/**
@}
*/
#endif /*IRT_LEGACY_STARTUP_SUPPORT*/

/**
 * @brief Dedicated task for PTCP delay measurement scheduling
 * Required in IRT mode
 *
 * @param arg0 not used
 * @param arg1 not used
 */
void PN_PTCP_task(UArg arg0, UArg arg1);

/**
 * @brief Dedicated task for PTCP sync monitor scheduling
 * Required in IRT mode
 *
 * @param arg0 not used
 * @param arg1 not used
 */
void PN_PTCP_syncMonitorTask(UArg arg0, UArg arg1);

#ifdef IRT_LEGACY_STARTUP_SUPPORT

#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t PN_initOs(PN_Handle pnHandle)
{
    Task_Params taskParams;
    GateHwi_Params gateParams;

    GateHwi_Params_init(&gateParams);
    pnHandle->swapGatePpm = GateHwi_create(&gateParams, NULL);

    if(pnHandle->swapGatePpm == NULL)
    {
        return -4;
    }

    GateHwi_Params_init(&gateParams);
    pnHandle->swapGateCpm = GateHwi_create(&gateParams, NULL);

    if(pnHandle->swapGateCpm == NULL)
    {
        return -4;
    }

    GateHwi_Params_init(&gateParams);
    pnHandle->toggleListMutex = GateHwi_create(&gateParams, NULL);

    if(pnHandle->toggleListMutex == NULL)
    {
        return -4;
    }

#ifdef PTCP_SUPPORT
    Task_Params_init(&taskParams);
    taskParams.priority = 11;
    taskParams.arg0 = (UArg)pnHandle;
    taskParams.instance->name = "PTCPTask";
    pnHandle->hPTCP = Task_create(PN_PTCP_task, &taskParams, NULL);

    if(pnHandle->hPTCP == NULL)
    {
        return -1;
    }

    Task_Params_init(&taskParams);
    taskParams.priority = 11;
    taskParams.instance->name = "SyncMonitorTask";
    taskParams.arg0 = (UArg)pnHandle;
    pnHandle->hSyncMonitor = Task_create(PN_PTCP_syncMonitorTask, &taskParams,
                                         NULL);

    if(pnHandle->hSyncMonitor == NULL)
    {
        return -2;
    }

#endif /*PTCP_SUPPORT*/

#ifdef IRT_LEGACY_STARTUP_SUPPORT
    Task_Params_init(&taskParams);
    taskParams.priority = 3;
    taskParams.instance->name = "LegacyModeTask";
    taskParams.arg0 = (UArg)pnHandle;
    pnHandle->hLegMode = Task_create(PN_IRT_legacyTask, &taskParams, NULL);

    if(pnHandle->hLegMode == NULL)
    {
        return -3;
    }

    PN_registerSetState(pnHandle, PN_setLegState);
    PN_registerSetPkt(pnHandle, PN_setLegPkt);
#endif /*IRT_LEGACY_STARTUP_SUPPORT*/

#ifdef MRP_SUPPORT
    Task_Params_init(&taskParams);
    taskParams.priority = 3;        /* FW low prio ok?*/
    taskParams.instance->name = "MrpMachine";
    taskParams.arg0 = (UArg)pnHandle;
    pnHandle->hMrpMachine = Task_create(PN_MRP_CPMTask, &taskParams, NULL);
#endif /*MRP_SUPPORT*/
#ifdef WATCHDOG_SUPPORT
    Task_Params_init(&taskParams);
    taskParams.priority = 11;
    taskParams.arg0 = (UArg)pnHandle;
    taskParams.instance->name = "WatchDogTimer";
    pnHandle->hWatchDogTimer = Task_create(PN_tapWatchDog_task, &taskParams, NULL);

    if(pnHandle->hWatchDogTimer == NULL)
    {
        return -4;
    }

#endif

    return 0;
}

int32_t PN_RTC_setupIsr(PN_Handle pnHandle)
{
    PN_IntConfig *intConfig = pnHandle->pnIntConfig;
    Hwi_Params hwiParams;
    Error_Block eb;

    Hwi_Params_init(&hwiParams);
    Error_init(&eb);

    /* setup CPM ISR*/
    hwiParams.arg = (UArg)(
                        intConfig->cpmIntConfig)->args;        /*Emac SUub system handle*/
    hwiParams.eventId = (intConfig->cpmIntConfig)->socEvId;
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    hwiParams.enableInt = FALSE;        /* enable later*/
    hwiParams.priority = (intConfig->cpmIntConfig)->intPrio;

    Hwi_create((intConfig->cpmIntConfig)->coreIntNum,
               (IHwi_FuncPtr)((intConfig->cpmIntConfig)->isrFnPtr), &hwiParams, &eb);

    if(Error_check(&eb))
    {
        /* handle the error*/
        return -1;
    }

    /* setup PPM ISR*/
    hwiParams.arg = (UArg)((intConfig->ppmIntConfig)->args);      /* just an ID*/
    hwiParams.eventId = (intConfig->ppmIntConfig)->socEvId;
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    hwiParams.enableInt = FALSE;        /* enable later*/
    hwiParams.priority = (intConfig->ppmIntConfig)->intPrio;

    Hwi_create((intConfig->ppmIntConfig)->coreIntNum,
               (IHwi_FuncPtr)((intConfig->ppmIntConfig)->isrFnPtr), &hwiParams, &eb);

    if(Error_check(&eb))
    {
        /* handle the error*/
        return -1;
    }

    /* setup DHT expired ISR*/
    hwiParams.arg = (UArg)((intConfig->dhtIntConfig)->args);        /* just an ID*/
    hwiParams.eventId = (intConfig->dhtIntConfig)->socEvId;
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    hwiParams.enableInt = FALSE;        /* enable later*/
    hwiParams.priority =
        (intConfig->dhtIntConfig)->intPrio;    /* FW bump the ISR prio*/

    Hwi_create((intConfig->dhtIntConfig)->coreIntNum,
               (IHwi_FuncPtr)((intConfig->dhtIntConfig)->isrFnPtr), &hwiParams, &eb);

    if(Error_check(&eb))
    {
        /* handle the error*/
        return -1;
    }

    return 0;
}

int32_t PN_RTC_enableISR(PN_Handle pnHandle)
{
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    PN_IntAttrs *ppmIntConfig = (pnHandle->pnIntConfig)->ppmIntConfig;
    PN_IntAttrs *cpmIntConfig = (pnHandle->pnIntConfig)->cpmIntConfig;
    PN_IntAttrs *dhtIntConfig = (pnHandle->pnIntConfig)->dhtIntConfig;
    /* reset some flags used for ISR*/
    HWREGB(emacBaseAddr->dataRam0BaseAddr + RTC_NOTIFY_DHT_EVENT_OFFSET) = 0;
    HWREGB(emacBaseAddr->dataRam0BaseAddr + RTC_NOTIFY_LIST_TOGGLE_EVENT_OFFSET) =
        0;
    /* clear pending PRU ISR flags*/
    PN_clearPruIRQ(emacBaseAddr, cpmIntConfig->pruIntNum);
    PN_clearPruIRQ(emacBaseAddr, ppmIntConfig->pruIntNum);
    PN_clearPruIRQ(emacBaseAddr, dhtIntConfig->pruIntNum);
    /*enable all IRQ*/
    Hwi_enableInterrupt(cpmIntConfig->coreIntNum);
    Hwi_enableInterrupt(ppmIntConfig->coreIntNum);
    Hwi_enableInterrupt(dhtIntConfig->coreIntNum);

    return 0;
}

int32_t PN_RTC_disableISR(PN_Handle pnHandle)
{
    PN_IntAttrs *ppmIntConfig = (pnHandle->pnIntConfig)->ppmIntConfig;
    PN_IntAttrs *cpmIntConfig = (pnHandle->pnIntConfig)->cpmIntConfig;
    PN_IntAttrs *dhtIntConfig = (pnHandle->pnIntConfig)->dhtIntConfig;
    /*disable all IRQ*/
    Hwi_disableInterrupt(cpmIntConfig->coreIntNum);
    Hwi_disableInterrupt(ppmIntConfig->coreIntNum);
    Hwi_disableInterrupt(dhtIntConfig->coreIntNum);

    return 0;
}

/*-----------------------------------------------------
 * new data API
 *-----------------------------------------------------*/
/*
 * called by HWI!
 */
int32_t PN_checkLastPPM(PN_Handle pnHandle, t_rtcPacket *pkt)
{
    uint8_t temp;
    IArg key;

    if(pkt->type != PPM)
    {
        return -1;   /* only usable with PPM packet*/
    }

    if(pkt->validLast)
    {
        key = GateHwi_enter(pnHandle->swapGatePpm); /* protect the swap operation*/
        temp = pkt->proc;               /*swap the tripple buffer indexes*/
        pkt->proc = pkt->last;
        pkt->last = temp;
        pkt->validLast = 0;
        PN_chgPpmBuffer(pnHandle, pkt);             /* update descriptor -> new data*/
        GateHwi_leave(pnHandle->swapGatePpm, key);
        return 1;
    }

    return 0;
}

uint8_t *PN_getPpmBuff(t_rtcPacket *pkt)
{
    if(pkt->type != PPM)
    {
        return NULL;    /* only usable with PPM packet*/
    }

    return pkt->pBuffer->addr[pkt->next];   /* current NEXT buffer address*/

}

uint8_t *PN_relPpmBuff(PN_Handle pnHandle, t_rtcPacket *pkt)
{
    uint8_t temp;
    IArg key;
    t_descList *pList;
    volatile uint8_t *pBcEvent;

    if(pkt->type != PPM)
    {
        return NULL;    /* only usable with PPM packet*/
    }

    if(0 == pkt->isActive)
    {
        return NULL;
    }

    key = GateHwi_enter(pnHandle->swapGatePpm); /* protect the swap operation*/
    temp = pkt->last;                   /* swap the tripple buffer indexes*/
    pkt->last = pkt->next;
    pkt->next = temp;
    pkt->validLast = 1;           /* queue up for next PPM BC HWI*/

    /* now check if we can directly write new last packet to PPM descriptor!*/


    if(NULL !=
            pnHandle->ppmList)                       /* just make sure we are error proove*/
    {
        pList = pnHandle->ppmList;
        pBcEvent = pList->pEvent + pkt->lIndex;

        if(*pBcEvent == 0)                  /* PPM not in send...*/
        {
            temp = pkt->proc;               /* start buffer swap*/
            pkt->proc = pkt->last;
            PN_chgPpmBuffer(pnHandle, pkt);             /* update descriptor -> new data*/
            pkt->validLast = 0;

            if(*pBcEvent == 0)              /* reread send status*/
            {
                pkt->last = temp;           /* complete the swap*/
                pkt->validLast = 0;
            }

            else
            {
                pkt->proc = temp;           /* reset old proc*/
                PN_chgPpmBuffer(pnHandle, pkt);         /* update descriptor -> old data!*/
                pkt->validLast = 1;             /* queue up for next PPM BC HWI*/
            }
        }
    }

    GateHwi_leave(pnHandle->swapGatePpm, key);
    return pkt->pBuffer->addr[pkt->next];   /* new NEXT buffer address*/
}

int32_t PN_nextCpmRdy(PN_Handle pnHandle, t_rtcPacket *pkt)
{
    uint8_t temp;
    int32_t ret = 0;
    int8_t buffIndex;

    IArg key;

    if(pkt->type != CPM)
    {
        return -1;    /* only usable with CPM packet*/
    }

    key = GateHwi_enter(pnHandle->swapGateCpm); /* protect the swap operation*/
    buffIndex = PN_getLastCpmBuffIndex(pnHandle,
                                       pkt->lIndex);

    if(buffIndex >= 0)
    {
        temp = pkt->last;             /*swap the tripple buffer indexes*/
        pkt->last = buffIndex;
        pkt->next = temp;
    }

    GateHwi_leave(pnHandle->swapGateCpm, key);

    if(pkt->last < 0)
    {
        ret = -1;
    }

    if(1 == pkt->validLast)        /*detect overrun*/
    {
        ret = 1;
    }

    pkt->validLast = 1;
    return ret;
}


uint8_t *PN_getLastCpm(PN_Handle pnHandle, t_rtcPacket *pkt)
{
    uint8_t temp;
    IArg key;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;

    if(pkt->type != CPM)
    {
        return NULL;    /* only usable with CPM packet*/
    }

    if(1 == pkt->validLast)
    {
        key = GateHwi_enter(pnHandle->swapGateCpm); /* protect the swap operation*/
        temp = pkt->proc;                   /* swap the tripple buffer indexes*/
        pkt->proc = pkt->last;
        pkt->last = temp;
        PN_cpmBuffLock(emacBaseAddr, pkt->lIndex, pkt->proc);   /* lock new PROC buffer*/
        GateHwi_leave(pnHandle->swapGateCpm, key);
        pkt->validLast = 0;
    }

    return pkt->pBuffer->addr[pkt->proc];   /* current RX data address*/
}

/*-----------------------------------------------------
 * legacy mode
 *-----------------------------------------------------*/
#ifdef IRT_LEGACY_STARTUP_SUPPORT

void PN_IRT_legacyTask(UArg arg0, UArg arg1)
{
    PN_Handle pnHandle = (PN_Handle)arg0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    uint8_t *const pRtcConfig = (uint8_t *)(emacBaseAddr->dataRam0BaseAddr);
    uint16_t fCycleCount = 0;
    uint16_t    sClk;                   /* current send clock factor*/

    while(pnHandle->legState != ABORT)
    {
        switch(pnHandle->legState)
        {
            case NOINIT:        /* wait for switch up*/
                SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER); /* wait for init*/
                SemaphoreP_post(switchReady);
                pnHandle->legState = READY;
                break;

            case READY:         /* wait until PPM packet initialized*/
                TaskP_sleep(10); /* need something better here...*/
                break;

            case SENDPPM:       /* send as long as CPM is not in RUN*/

                /* check for CPM status first (assume RTC3 is in first descriptor ALWAYS)*/
                if(HWREGB(pRtcConfig + RTC_CPM_STATUS_OFFSET) == RTC_CPM_RUN)
                {
                    pnHandle->legState = READY;         /* connect done...*/
                    fCycleCount = 0;
                }

                else
                {
                    /* get current base clock*/
                    sClk =  HWREGH(pRtcConfig + RTC_SCF_OFFSET);

                    /* send a RTC3 PPM through low level PRU API (green phase!)*/
                    if((pnHandle->pLegPkt)->pBuffer->addr[(pnHandle->pLegPkt)->proc] != NULL)
                    {
                        /* write a faked cycle counter with right endianess...*/
                        HWREGH((pnHandle->pLegPkt)->pBuffer->addr[(pnHandle->pLegPkt)->proc] +
                               (pnHandle->pLegPkt)->length - 4) =
                                   (fCycleCount >> 8 | (fCycleCount & 0xFF) << 8);
                        PN_OS_txPacket(pnHandle->emacHandle,
                                       (pnHandle->pLegPkt)->pBuffer->addr[(pnHandle->pLegPkt)->proc],
                                       (pnHandle->pLegPkt)->port, ICSS_EMAC_QUEUE2,
                                       (pnHandle->pLegPkt)->length);      /* send high prio*/
                        fCycleCount += (sClk * 128);   /* RR=128*/
                        TaskP_sleep(sClk * 4 * 1000 /
                                   Clock_tickPeriod);         /* base clock * 128  period*/
                    }

                    else
                    {
                        pnHandle->legState = READY;    /* bad state!*/
                    }
                }

                break;

            default:
                System_printf("undefined LegState\n");
                pnHandle->legState = NOINIT;
        }
    }

    System_printf("Legacy process task aborted\n");

    return;
}


void PN_setLegState(void *arg, void *arg2)
{
    PN_Handle pnHandle = (PN_Handle)arg;
    pnHandle->legState = (tLegStates)arg2;
}


void PN_setLegPkt(void *arg, void *arg2)
{
    PN_Handle pnHandle = (PN_Handle)arg;
    pnHandle->pLegPkt = arg2;
}

#endif /*IRT_LEGACY_STARTUP_SUPPORT*/
/*-----------------------------------------------------
 * MRP support
 *-----------------------------------------------------*/
#ifdef MRP_SUPPORT

void PN_MRP_CPMTask(UArg arg0, UArg arg1)
{
    PN_Handle pnHandle = (PN_Handle)arg0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    uint8_t *const pRtcConfig = (uint8_t *)(emacBaseAddr->dataRam0BaseAddr);

    while(1)
    {
        switch(pnHandle->mrpState)
        {
            case MRPREADY:
                /* our IDLE state, all links ok...*/
                break;

            case MRPENTER:
               /* set PRU flush mode*/
                HWREGB(pRtcConfig + RTC_MRP_FDB_FLUSH_OFFSET) = 1;
                /* flush the FDB in the stack as it needs port number*/
                PN_resetCpmPorts(pnHandle);
                pnHandle->mrpState = MRPWAIT;
                break;

            case MRPWAIT:
                TaskP_sleep(10 * 1000 / Clock_tickPeriod);   /* 10 ms*/
                pnHandle->mrpState = MRPCHECK;
                break;

            case MRPCHECK:

                /* check CPMs and advance state accordingly*/
                if(PN_allCpmKnown(pnHandle))
                {
                    pnHandle->mrpState = MRPEXIT;
                }

                else
                {
                    pnHandle->mrpState = MRPWAIT;
                }

                break;

            case MRPEXIT:
                /*clear PRU flush mode*/
                HWREGB(pRtcConfig + RTC_MRP_FDB_FLUSH_OFFSET) = 0;
                pnHandle->mrpState = MRPREADY;
                PN_clearMrpFlag(pnHandle);
                break;

            default:
                /* invalid state...*/
                break;
        }

        TaskP_sleep(1 * 1000 / Clock_tickPeriod);
    }
}

uint32_t PN_enterFlushMode(PN_Handle pnHandle)
{
    if(MRPREADY == pnHandle->mrpState)
    {
        pnHandle->mrpState = MRPENTER;
        return 0;
    }

    return 1;
}
#endif /*MRP_SUPPORT*/
/*-----------------------------------------------------
 * PTCP support
 *-----------------------------------------------------*/
#ifdef PTCP_SUPPORT

void PN_PTCP_task(UArg arg0, UArg arg1)
{
    PN_Handle pnHandle = (PN_Handle)arg0;
    SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER); /* wait for init*/
    SemaphoreP_post(switchReady);

    while(!(((ICSS_EmacObject *)(pnHandle->emacHandle)->object)->linkStatus[0] ||
            ((ICSS_EmacObject *)(
                 pnHandle->emacHandle)->object)->linkStatus[1]))             /* wait for link up*/
    {
        TaskP_sleep(1);                              /* needs link down management later*/
    }

    PN_PTCP_smaDelayMeasurement(pnHandle);      /*endless loop*/
}


void PN_PTCP_syncMonitorTask(UArg arg0, UArg arg1)
{
    PN_Handle pnHandle = (PN_Handle)arg0;
    SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER); /* wait for init*/
    SemaphoreP_post(switchReady);

    while(!(((ICSS_EmacObject *)(pnHandle->emacHandle)->object)->linkStatus[0] ||
            ((ICSS_EmacObject *)(
                 pnHandle->emacHandle)->object)->linkStatus[1]))             /* wait for link up*/
    {
        TaskP_sleep(1);                              /* needs link down management later*/
    }

    PN_PTCP_syncTimeoutMonitor(pnHandle);       /* endless loop*/
}
#endif /*PTCP_SUPPORT*/

/*-----------------------------------------------------
 *
 *-----------------------------------------------------*/
int32_t PN_OS_txPacket(ICSSEMAC_Handle icssEmacHandle,
                       const uint8_t *srcAddress, int32_t portNumber, int32_t queuePriority,
                       int32_t lengthOfPacket)
{
    int32_t ret=0;
    ICSS_EmacTxArgument txArgs;
    llEnter();
    memset(&txArgs, 0, sizeof(ICSS_EmacTxArgument));
    txArgs.icssEmacHandle = icssEmacHandle;
    txArgs.lengthOfPacket = lengthOfPacket;
    txArgs.portNumber = portNumber;
    txArgs.queuePriority = queuePriority;
    txArgs.srcAddress = srcAddress;

    ret= ICSS_EmacTxPacket(&txArgs, NULL);

    llExit();

    return ret;
}

/*-----------------------------------------------------
 *
 *-----------------------------------------------------*/
#ifdef WATCHDOG_SUPPORT
void PN_tapWatchDog_task(UArg arg0, UArg arg1)
{
    PN_Handle pnHandle = (PN_Handle)arg0;
    ICSS_EmacBaseAddressHandle_T emacBaseAddr = ((ICSS_EmacHwAttrs *)(
                pnHandle->emacHandle)->hwAttrs)->emacBaseAddrCfg;
    SemaphoreP_pend(switchReady, BIOS_WAIT_FOREVER);   /* wait for init*/
    SemaphoreP_post(switchReady);

    while(1)                /* wait for watchdog to be enabled*/
    {
        if(pnHandle->icssWatchDogEnabled)
        {
            HWREGH(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_PD_WD_TIM_REG) =
                pnHandle->icssWachDogTimerPeriod;
        }

        TaskP_sleep((pnHandle->icssWachDogTimerPeriod / 10) - (((
                       pnHandle->icssWachDogTimerPeriod / 10) / 2)));
    }

}
#endif
