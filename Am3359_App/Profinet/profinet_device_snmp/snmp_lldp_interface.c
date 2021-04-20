/**
 * @file snmp_lldp_interface.c
 *
 * \par
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 * \par
*/
#include <string.h>
#include <third_party/protocols/profinet_slave/include/types_pn.h>
#include <third_party/protocols/profinet_slave/include/snmp_lldp_interface.h>
#include <ti/sysbios/BIOS.h>



SemaphoreP_Handle gMUX_LLDP = NULL;
SemaphoreP_Handle gmSynchro = NULL;

Uint16 numberOfLldpInterfaces()
{
    return NUMBER_OF_LLDP_PORTS;
}

APP_WORD LLDP_Init(APP_VOID)
{
    SemaphoreP_Params semParams;
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    gMUX_LLDP = SemaphoreP_create(0, &semParams);

    if(gMUX_LLDP == NULL)
    {
        return 101;
    }

    gmSynchro = SemaphoreP_create(1, &semParams);

    if(gmSynchro == NULL)
    {
        return 102;
    }

    return 0;
}

#define LLDP_PROTECT_ON SemaphoreP_pend(gmSynchro, BIOS_WAIT_FOREVER)/*;printf("ON\n")*/
#define LLDP_PROTECT_OFF SemaphoreP_post(gmSynchro)/*;printf("OFF\n")*/


APP_WORD LLDP_GetTimeConfig(ST_LLDP_INTERFACE_GET_TIME_CONFIG *lpGetTime)
{
    APP_WORD wStatus;
    memset(lpGetTime, 0, sizeof(ST_LLDP_INTERFACE_GET_TIME_CONFIG));
    LLDP_PROTECT_ON;

    lpGetTime->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetTimeConfig_Req((void *)lpGetTime, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpGetTime->wResult;
}

APP_WORD LLDP_GetPortConfig(ST_LLDP_INTERFACE_PORT_CONFIG *lpPortConfig,
                            APP_BYTE byport)
{
    APP_WORD wStatus;


    memset(lpPortConfig, 0, sizeof(ST_LLDP_INTERFACE_PORT_CONFIG));
    LLDP_PROTECT_ON;
    lpPortConfig->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetPortConfig_Req(lpPortConfig, byport, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);     /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpPortConfig->wResult;
}

APP_WORD LLDP_Tx_Stats(ST_LLDP_INTERFACE_TX_STATS *lpTxStats, APP_BYTE byport)
{
    APP_WORD wStatus;
    memset(lpTxStats, 0, sizeof(ST_LLDP_INTERFACE_TX_STATS));
    LLDP_PROTECT_ON;
    lpTxStats->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_Tx_Stats_Req(lpTxStats, byport, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/


    if(lpTxStats->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;
}

APP_WORD LLDP_Rx_Stats(ST_LLDP_INTERFACE_RX_STATS *lpRxStats, APP_BYTE port)
{
    APP_WORD wStatus;

    memset(lpRxStats, 0, sizeof(ST_LLDP_INTERFACE_RX_STATS));

    LLDP_PROTECT_ON;
    lpRxStats->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_Rx_Stats_Req(lpRxStats, port, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpRxStats->wResult;
}

APP_WORD LLDP_GetStatsRemoteTables(ST_LLDP_REMOTE_STATS_TABLES
                                   *lpRemStatsTables)
{
    APP_WORD wStatus;

    memset(lpRemStatsTables, 0, sizeof(ST_LLDP_REMOTE_STATS_TABLES));

    LLDP_PROTECT_ON;

    lpRemStatsTables->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteStatsTables_Req(lpRemStatsTables, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);   /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpRemStatsTables->wResult;
}
APP_WORD LLDP_GetSystemConfig(ST_LLDP_INTERFACE_SYSTEM_CONFIG *lpSystem)
{
    APP_WORD wStatus;

    memset(lpSystem, 0, sizeof(ST_LLDP_INTERFACE_SYSTEM_CONFIG));
    LLDP_PROTECT_ON;
    lpSystem->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetSystemConfig_Req(lpSystem, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpSystem->wResult;
}


APP_WORD LLDP_GetDescription(ST_LLDP_INTERFACE_DESCRIPTION *lpDescription,
                             APP_BYTE byPort)
{
    APP_WORD wStatus;
    memset(lpDescription, 0, sizeof(ST_LLDP_INTERFACE_DESCRIPTION));
    LLDP_PROTECT_ON;
    lpDescription->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetDescription_Req(lpDescription, byPort, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpDescription->wResult;
}

APP_WORD LLDP_GetAddressMgt(ST_LLDP_INTERFACE_ADR_MANAGEMENT *lpInterface)
{
    APP_WORD wStatus;
    memset(lpInterface, 0, sizeof(ST_LLDP_INTERFACE_ADR_MANAGEMENT));
    LLDP_PROTECT_ON;
    lpInterface->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetAddressMgt_Req(lpInterface, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpInterface->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;
}

APP_WORD LLDP_GetPortRemotePeer(ST_LLDP_PORT_INFORMATION *lpPeerInfo,
                                APP_BYTE byPortIndex)
{
    APP_WORD wStatus;

    memset(lpPeerInfo, 0, sizeof(ST_LLDP_PORT_INFORMATION));

    LLDP_PROTECT_ON;
    lpPeerInfo->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetPortSysList_Req(lpPeerInfo, byPortIndex, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/
    LLDP_PROTECT_OFF;

    return lpPeerInfo->wResult;
}


APP_WORD LLDP_GetPortRemoteInformation(ST_LLDP_INTERFACE_REMOTE_INFORMATION
                                       *lpRemInfo, APP_BYTE byRemIndex)
{
    APP_WORD wStatus;

    memset(lpRemInfo, 0, sizeof(ST_LLDP_INTERFACE_REMOTE_INFORMATION));

    LLDP_PROTECT_ON;
    lpRemInfo->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetPortRemoteInformation_Req(lpRemInfo, byRemIndex,
            &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/
    LLDP_PROTECT_OFF;

    return lpRemInfo->wResult;
}

APP_WORD LLDP_SetTimeConfig(ST_LLDP_INTERFACE_GET_TIME_CONFIG *lpGetTime)
{
    APP_WORD wStatus;

    memset(lpGetTime, 0, sizeof(ST_LLDP_INTERFACE_GET_TIME_CONFIG));
    LLDP_PROTECT_ON;

    lpGetTime->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_SetTimeConfig_Req(lpGetTime, lpGetTime->wMsgHold,
            lpGetTime->wMsgTxInterval, lpGetTime->wMsgTxReInitDelay, lpGetTime->wMsgTxDelay,
            &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpGetTime->wResult;
}



APP_WORD LLDP_GetRemAddressMgtList(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST
                                   *lpRemAddrList, APP_BYTE byRemoteSysIndex)
{
    APP_WORD wStatus;
    memset(lpRemAddrList, 0, sizeof(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST));

    LLDP_PROTECT_ON;
    lpRemAddrList->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetPortAddrMgtList_Req(lpRemAddrList, byRemoteSysIndex,
            &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpRemAddrList->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;
}



APP_WORD LLDP_GetRemAddressMgt(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT *lpRemAddr,
                               APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteMgtAddrIndex)
{
    APP_WORD wStatus;
    memset(lpRemAddr, 0, sizeof(ST_LLDP_INTERFACE_REMOTE_ADDR_MGT));

    LLDP_PROTECT_ON;
    lpRemAddr->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteAddressMgt_Req(lpRemAddr, byRemoteSysIndex,
            byRemoteMgtAddrIndex, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpRemAddr->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;
}

APP_WORD LLDP_GetRemoteUnknownTlvList(ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST
                                      *lpUnkTlv, APP_BYTE byRemoteSysIndex)
{
    APP_WORD wStatus;
    memset(lpUnkTlv, 0, sizeof(ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST));

    LLDP_PROTECT_ON;
    lpUnkTlv->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteUnknownTlvList_Req(lpUnkTlv, byRemoteSysIndex,
            &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpUnkTlv->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;

}


APP_WORD LLDP_GetRemoteUnknownTlv(ST_LLDP_INTERFACE_REMOTE_UNK_TLV *lpUnkTlv,
                                  APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteUnkTlvIndex)
{
    APP_WORD wStatus;
    memset(lpUnkTlv, 0, sizeof(ST_LLDP_INTERFACE_REMOTE_UNK_TLV));

    LLDP_PROTECT_ON;
    lpUnkTlv->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteUnknownTlv_Req(lpUnkTlv, byRemoteSysIndex,
            byRemoteUnkTlvIndex,  &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpUnkTlv->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;

}


APP_WORD LLDP_GetRemoteOrgTlvList(ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST
                                  *lpOrgUnkTlv, APP_BYTE byRemoteSysIndex)
{
    APP_WORD wStatus;
    memset(lpOrgUnkTlv, 0, sizeof(ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST));

    LLDP_PROTECT_ON;
    lpOrgUnkTlv->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteOrgUnkTlvList_Req(lpOrgUnkTlv,
            byRemoteSysIndex,  &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpOrgUnkTlv->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;

}


APP_WORD LLDP_GetOrgUnknownTlv(ST_LLDP_INTERFACE_ORG_UNK_TLV *lpOrgTlv,
                               APP_BYTE byRemoteSysIndex, APP_BYTE byRemoteOrgTlvIndex)
{
    APP_WORD wStatus;
    memset(lpOrgTlv, 0, sizeof(ST_LLDP_INTERFACE_ORG_UNK_TLV));

    LLDP_PROTECT_ON;
    lpOrgTlv->MUX_LLDP    = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetRemoteOrganizationUnkTlv_Req(lpOrgTlv,
            byRemoteSysIndex, byRemoteOrgTlvIndex,  &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);       /* Required? YES*/

    if(lpOrgTlv->wResult == OS_NOT_OK)
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    LLDP_PROTECT_OFF;
    return OS_OK;

}

APP_WORD LLDP_GetDeviceNameConfig(ST_LLDP_INTERFACE_DEVICENAME_CONFIG
                                  *lpDevName)
{
    APP_WORD wStatus;

    memset(lpDevName, 0, sizeof(ST_LLDP_INTERFACE_DEVICENAME_CONFIG));
    LLDP_PROTECT_ON;
    lpDevName->MUX_LLDP = gMUX_LLDP;

    if(OS_NOT_OK == WSE_LLDP_GetDeviceNameConfig_Req(lpDevName, &wStatus))
    {
        LLDP_PROTECT_OFF;
        return OS_NOT_OK;
    }

    SemaphoreP_pend(gMUX_LLDP, BIOS_WAIT_FOREVER);   /* Required? YES*/

    LLDP_PROTECT_OFF;
    return lpDevName->wResult;
}
