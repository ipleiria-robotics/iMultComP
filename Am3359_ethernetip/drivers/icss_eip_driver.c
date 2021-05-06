/**
* @file  icss_eip_driver.c
*
* @brief PTP and DLR Init and De-Init calls are abstracted here. It also contains the callback for real time processing
*
*\par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*\par
*/

#include <protocols/ethernetip_adapter/include/icss_dlr.h>
#include <protocols/ethernetip_adapter/include/icss_eip_driver.h>
#include <protocols/timeSync/include/icss_timeSync_init.h>
#include <ti/drv/icss_emac/icss_emacStormControl.h>
#include "icss_eip_mcFltr.h"

/**PTP MAC ID for comparison*/
uint8_t ptpMAC[6] = {0x1, 0x0, 0x5e, 0x0, 0x1, 0x81};
/**DLR MAC ID for comparison*/
uint8_t dlrMAC[6] = {0x1, 0x21, 0x6c, 0x0, 0x0, 0x2};

/** Product Description for CIP Sync. Reads Texas Instruments:AM3359 in unicode */
uint8_t productDesc[] = {84, 101, 120, 97, 115, 32, 73, 110, 115, 116, 114, 117, 109, 101,   \
                         110, 116, 115, 59, 65, 77, 51, 51, 53, 57
                        };
/**Product Revision Description for CIP Sync */
uint8_t revDesc[] = {49, 59, 49, 59, 49};

/**Used to initialize Port Profile identity*/
uint8_t portProfIdentity[] = {0x1, 0x21, 0x6c, 0x0, 0x1, 0x0};

/**Manufacturers Identity*/
uint8_t manufacturerIdentity[] = {0xC4, 0xED, 0xBA};

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  @brief  Initialization routine for Ethernet/IP driver functions
 *
 *          This API does the following functionalities
 *              Register the Real Time Rx Call back
 *              Enable Storm prevention
 *              Call DLR init API
 *              Call PTP init API
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvInit(EIP_Handle icssEipHandle)
{
    ICSS_EmacHandle icssEmacHandle = icssEipHandle->emacHandle;

    /*Packet processing callback*/
    ((((ICSS_EmacObject *)
       icssEmacHandle->object)->callBackHandle)->rxRTCallBack)->callBack =
           (ICSS_EmacCallBack)EIP_processProtocolFrames;
    ((((ICSS_EmacObject *)
       icssEmacHandle->object)->callBackHandle)->rxRTCallBack)->userArg =
           icssEipHandle;

    /*Initialize DLR Engine*/
    EIP_DLR_init(icssEipHandle->dlrHandle);

    /*Initialise Multicast filter*/
    eip_multicast_filter_init(icssEmacHandle);

    /*Initialize PTP Handle*/

    TimeSync_drvInit(icssEipHandle->timeSyncHandle);

    /*In case of EIP the TimeSync_addIP() function
     * is called before TimeSync_drvInit() resulting in IP address
     * getting overwritten. This is the fix for it
     */
    TimeSync_addIP(icssEipHandle->timeSyncHandle,
                   icssEipHandle->timeSyncHandle->udpParams.srcIP);
}

/**
 *  @brief  EIP driver start API
 *
 *          This API starts DLR and enables PTP
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvStart(EIP_Handle icssEipHandle)
{
    /*Start DLR Engine*/
    EIP_DLR_start(icssEipHandle->dlrHandle);

    /*Start PTP Engine*/
    TimeSync_drvEnable(icssEipHandle->timeSyncHandle);
}
/**
 *  @brief  EIP driver stop API
 *
 *          This API stops DLR and disables PTP
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvStop(EIP_Handle icssEipHandle)
{

    /*Halt DLR Engine*/
    EIP_DLR_stop(icssEipHandle->dlrHandle);

    /*Halt PTP Engine*/
    TimeSync_drvDisable(icssEipHandle->timeSyncHandle);
}

/**
 *  @brief  API to process the real time Packets
 *
 *          This API will be registered as Real Tme Rx Call back. Incase of EIP, the driver
 *          receives DLR and PTP packets. This API receives the packet, checks the packet
 *          type and passes the packet to DLR driver or PTP driver
 *
 *  @param  queue_number [in] Queue where the packet is present
 *  @param  userArg      [in] userArgumment. EIP handle
 *
 *  @retval  none
 *
 */
void EIP_processProtocolFrames(uint32_t *queue_number, void *userArg)
{

    int16_t size = 0;
    ICSS_EmacRxArgument rxArg;

    EIP_Handle icssEipHandle = (EIP_Handle)userArg;
    ICSSEMAC_Handle eipIcssEmacHandle = icssEipHandle->emacHandle;

    uint8_t *dstMacId = icssEipHandle->tempFrame;

    rxArg.icssEmacHandle = eipIcssEmacHandle;
    rxArg.destAddress = (uint32_t)(icssEipHandle->tempFrame);
    rxArg.more = 0;
    rxArg.queueNumber = *queue_number;
    rxArg.port = 0;

    if(((((ICSS_EmacObject *)
            eipIcssEmacHandle->object)->callBackHandle)->rxCallBack)->callBack != NULL)
    {
        size = ((((ICSS_EmacObject *)
                  eipIcssEmacHandle->object)->callBackHandle)->rxCallBack)->callBack(&rxArg,
                          NULL);
    }

    /*Compare Destination MAC ID and determine if this is a DLR packet*/
    if(COMPARE_MAC(dstMacId, dlrMAC))
    {
        EIP_DLR_processDLRFrame(icssEipHandle->dlrHandle, icssEipHandle->tempFrame,
                                rxArg.port - 1, size);
    } /*Compare Destination MAC ID and determine if this is a PTP packet*/

    else if(COMPARE_MAC(dstMacId, ptpMAC))
    {
        /*Link local field doesn't matter in case of EIP*/
        TimeSync_processPTPFrame(icssEipHandle->timeSyncHandle,
                                 icssEipHandle->tempFrame, rxArg.port, size, FALSE);
    }

    else
    {
        ((ICSS_EmacObject *)eipIcssEmacHandle->object)->icssEmacHwIntRx(queue_number,
                eipIcssEmacHandle);
    }

}
/**
 *  @brief  API to initialize the CIP Sync objects in the EIP handle
 *          Initialize CIP Sync member variables based on PTP implementation
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  0  - On success
 *           <0 - On failure
 *
 */
int8_t EIP_initializeCIPSync(EIP_Handle icssEipHandle)
{
    int8_t retVal = 0;
    int8_t i;
    TimeSync_Config_t *timeSyncConfig = &
                                        (icssEipHandle->timeSyncHandle->timeSyncConfig);

    /*The section below is applicable to both OC and TC*/
    icssEipHandle->cipSyncObj->localClkInfoPtr = (clockClass_t *)malloc(sizeof(
                clockClass_t));

    if(NULL == icssEipHandle->cipSyncObj->localClkInfoPtr)
    {
        retVal = -1;
    }

    for(i = 0; i < 8; i++)
    {
        icssEipHandle->cipSyncObj->localClkInfoPtr->clockIdentity[i] =
            timeSyncConfig->clockIdentity[i];
    }

    icssEipHandle->cipSyncObj->portEnable[0] = TRUE;
    icssEipHandle->cipSyncObj->portEnable[1] = TRUE;

    /*Add OUI of Texas Instruments*/
    memcpy(icssEipHandle->cipSyncObj->manufacturerIdentity, manufacturerIdentity,
           3);

    /*Unicode for Product Description*/
    icssEipHandle->cipSyncObj->productType.size = sizeof(productDesc);
    memcpy(icssEipHandle->cipSyncObj->productType.descr, productDesc,
           icssEipHandle->cipSyncObj->productType.size);

    /*Unicode for Firmware and Software Revision*/
    icssEipHandle->cipSyncObj->revData.size = sizeof(revDesc);
    memcpy(icssEipHandle->cipSyncObj->revData.descr, revDesc,
           icssEipHandle->cipSyncObj->productType.size);

    /*The unicode for User Description should be filled by application*/

    /*Based on CIP Sync Recommendations*/
    memcpy(icssEipHandle->cipSyncObj->profileInfo[0].portProfileIdentity,
           portProfIdentity, 6);

    /*The section below is only applicable to Ordinary Clocks*/
    if(timeSyncConfig->config == OC)
    {

        icssEipHandle->cipSyncObj->clockType = ORDINARY_CLOCK;

        icssEipHandle->cipSyncObj->ifPTPEnable = 1;
        icssEipHandle->cipSyncObj->IsSynchronized = 0;
        icssEipHandle->cipSyncObj->systemTimeMicrosec = 0;
        icssEipHandle->cipSyncObj->systemTimeNanosec = 0;
        icssEipHandle->cipSyncObj->offsetFromMaster = 0;
        icssEipHandle->cipSyncObj->maxOffsetFromMaster = 0;
        icssEipHandle->cipSyncObj->meanPathDelayToMaster = 0;
        icssEipHandle->cipSyncObj->grandMasterClkInfoPtr = (clockClass_t *)malloc(
                    sizeof(clockClass_t));

        if(NULL == icssEipHandle->cipSyncObj->grandMasterClkInfoPtr)
        {
            retVal = -1;
        }

        icssEipHandle->cipSyncObj->parentClkInfoPtr = (clockClass_t *)malloc(sizeof(
                    clockClass_t));

        if(NULL == icssEipHandle->cipSyncObj->parentClkInfoPtr)
        {
            retVal = -1;
        }

        icssEipHandle->cipSyncObj->numberOfPorts = 1;

        icssEipHandle->cipSyncObj->portState[0] = PASSIVE;
        icssEipHandle->cipSyncObj->portState[1] = PASSIVE;

        icssEipHandle->cipSyncObj->portLogAnnounceInterval[0] = 0;
        icssEipHandle->cipSyncObj->portLogAnnounceInterval[1] = 0;

        icssEipHandle->cipSyncObj->portLogSyncInterval[0] = 0;
        icssEipHandle->cipSyncObj->portLogSyncInterval[1] = 0;

        icssEipHandle->cipSyncObj->priority1 = 0;
        icssEipHandle->cipSyncObj->priority2 = 1;

        icssEipHandle->cipSyncObj->domainNumber = 0;
        icssEipHandle->cipSyncObj->stepsRemoved = 0;

        icssEipHandle->cipSyncObj->timeOffset.systemOffset = 0;
        icssEipHandle->cipSyncObj->timeOffset.systemTime = 0;

    }

    return retVal;

}
