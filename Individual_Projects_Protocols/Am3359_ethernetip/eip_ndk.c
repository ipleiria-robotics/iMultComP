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

#include <stdlib.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/ndk/inc/netmain.h>
#include <Include/protocol/eip_main.h>
#include <Include/protocol/user_acd.h>


static void * hCfg;
static void * cfgHostname;
static void * cfgAddress;
static void * cfgGateway;

extern void llTimerTick();

extern char *ti_ndk_config_Global_HostName;
/* Static IP Address settings */
extern char *LocalIPAddr;
extern char *LocalIPMask;
extern char *GatewayIP;
extern char *DomainName;
extern unsigned int acdARPFlag;
extern int acdIPAssign;
extern int acdCablePull;
extern Semaphore_Handle appInitSem;
extern ICSSEMAC_Handle emachandle;

int startupMode = EIP_STARTUP_STATICIP;

extern Void ti_ndk_config_tcp_init(void * hCfg);

/**DNS Server*/
char *DNSServer   = "0.0.0.0";
/**DHCP Options*/
uint8_t DHCP_OPTIONS[] = { DHCPOPT_SERVER_IDENTIFIER, DHCPOPT_ROUTER, DHCPOPT_SUBNET_MASK };
static void   EIPNDK_serviceReport(uint Item, uint Status, uint Report,
                                   void * hCfgEntry);
void * dhcphandle;
unsigned int startUPType = EIP_STARTUP_STATICIP;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @brief The function used to reset DHCP client
 *
 *
 *  @param  IfIdx    [in]
 *  @param  fOwnTask [in]
 *
 *  @retval none
 *
 */
void EIPNDK_dHCPReset(uint IfIdx, uint fOwnTask)
{
    CI_SERVICE_DHCPC dhcpc;
    void * h;
    int    rc, tmp;
    uint   idx;

    /* If we were called from a newly created task thread, allow
     the entity that created us to complete*/
    if(fOwnTask)
    {
        TaskSleep(500);
    }

    /* Find DHCP on the supplied interface */
    for(idx = 1; ; idx++)
    {
        /* Find a DHCP entry*/
        rc = CfgGetEntry(0, CFGTAG_SERVICE, CFGITEM_SERVICE_DHCPCLIENT,
                         idx, &h);

        if(rc != 1)
        {
            goto RESET_EXIT;
        }

        /* Get DHCP entry data*/
        tmp = sizeof(dhcpc);
        rc = CfgEntryGetData(h, &tmp, (uint8_t *)&dhcpc);

        /* If not the right entry, continue*/
        if((rc <= 0) || dhcpc.cisargs.IfIdx != IfIdx)
        {
            CfgEntryDeRef(h);
            h = 0;
            continue;
        }

        /* This is the entry we want!*/

        /* Remove the current DHCP service*/
        CfgRemoveEntry(0, h);

        /* Specify DHCP Service on specified IF*/
        bzero(&dhcpc, sizeof(dhcpc));
        dhcpc.cisargs.Mode   = CIS_FLG_IFIDXVALID;
        dhcpc.cisargs.IfIdx  = IfIdx;
        dhcpc.cisargs.pCbSrv = (void *)&EIPNDK_serviceReport;
        CfgAddEntry(0, CFGTAG_SERVICE, CFGITEM_SERVICE_DHCPCLIENT, 0,
                    sizeof(dhcpc), (uint8_t *)&dhcpc, 0);
        break;
    }

RESET_EXIT:

    /* If we are a function, return, otherwise, call TaskExit()*/
    if(fOwnTask)
    {
        TaskExit();
    }
}


/**
 *  @brief
 *
 *
 *  @param  none
 *
 *  @retval none
 *
 */
void EIPNDK_checkDHCPOptions()
{
    CI_ROUTE RT;
    char IPString[16];
    uint32_t  IPAddr;
    int  i, rc;

    /* Now scan for DHCPOPT_ROUTER via the configuration*/
    for(i = 1;; i++)
    {
        /* Try and get a DNS server*/
        rc = CfgGetImmediate(0, CFGTAG_SYSINFO, DHCPOPT_ROUTER,
                             i, 4, (uint8_t *)&IPAddr);

        if(rc != 4)
        {
            break;
        }

        /* We got something*/

        /* Convert IP to a string:*/
        NtIPN2Str(IPAddr, IPString);

        bzero(&RT, sizeof(RT));
        RT.IPDestAddr = 0;
        RT.IPDestMask = 0;
        RT.IPGateAddr = inet_addr(IPString);


        CfgAddEntry(hCfg, CFGTAG_ROUTE, 1, 0,
                    sizeof(CI_ROUTE), (uint8_t *)&RT, &cfgGateway);
    }
}
/**
 *  @brief  The function is called whenever there is a status change in DHCP mode of operation
 *
 *
 *  @param  Item   [in]
 *  @param  Status [in]
 *  @param  Report [in]
 *  @param  h      [in]
 *
 *  @retval none
 *
 */
static void EIPNDK_serviceReport(uint Item, uint Status, uint Report, void * h)
{

    /*TaskName[Item-1];
    ReportStr[Report/256];
    Report&0xFF;
    StatusStr[Status];

     Example of adding to the DHCP configuration space

     When using the DHCP client, the client has full control over access
     to the first 256 entries in the CFGTAG_SYSINFO space.

     Note that the DHCP client will erase all CFGTAG_SYSINFO tags except
     CFGITEM_DHCP_HOSTNAME. If the application needs to keep manual
     entries in the DHCP tag range, then the code to maintain them should
     be placed here.

     Here, we want to manually add a DNS server to the configuration, but
     we can only do it once DHCP has finished its programming.

    */

    if(Item == CFGITEM_SERVICE_DHCPCLIENT &&
            Status == CIS_SRV_STATUS_ENABLED &&
            (Report == (NETTOOLS_STAT_RUNNING | DHCPCODE_IPADD) ||
             Report == (NETTOOLS_STAT_RUNNING | DHCPCODE_IPRENEW)))
    {
        uint32_t IPTmp;

        /* Manually add the DNS server when specified*/
        IPTmp = inet_addr(DNSServer);

        if(IPTmp)
            CfgAddEntry(0, CFGTAG_SYSINFO, CFGITEM_DHCP_DOMAINNAMESERVER,
                        0, sizeof(IPTmp), (uint8_t *)&IPTmp, 0);

        /* We can now check on what the DHCP server supplied in
           response to our DHCP option tags.*/
        EIPNDK_checkDHCPOptions();


    }

    /* Reset DHCP client service on failure*/
    if(Item == CFGITEM_SERVICE_DHCPCLIENT && (Report & ~0xFF) == NETTOOLS_STAT_FAULT)
    {
        CI_SERVICE_DHCPC dhcpc;
        int tmp;
        /* Get DHCP entry data (for index to pass to DHCP_reset).*/
        tmp = sizeof(dhcpc);
        CfgEntryGetData(h, &tmp, (uint8_t *)&dhcpc);

        /* Create the task to reset DHCP on its designated IF
         We must use TaskCreate instead of just calling the function as
         we are in a callback function.*/
        TaskCreate(EIPNDK_dHCPReset, "DHCPreset", OS_TASKPRINORM, 0x1000,
                   dhcpc.cisargs.IfIdx, 1, 0);
    }
}

/**
 *  @brief  This function adds the cfg entries.
 *
 *          Depending on the startup types in non volatile meory, DHCP mode/ Static  IP mode is
 *          selected. This function is called from the Network Start hook.
 *
 *  @param  hCfg [in] handle
 *
 *  @retval none
 *
 */
static void EIPNDK_registerIPCfg(void * hCfg)
{
    CI_IPNET NA;
    CI_ROUTE RT;
    uint tmp;
    CI_SERVICE_DHCPC dhcpc;

    if((eipDevInitConfig.tcpipConfig.startupType == EIP_STARTUP_DHCP)
            && (!acdCablePull))
    {
        startupMode = EIP_STARTUP_DHCP;

        bzero(&dhcpc, sizeof(dhcpc));
        dhcpc.cisargs.Mode   = CIS_FLG_IFIDXVALID;
        dhcpc.cisargs.IfIdx  = 1;
        dhcpc.cisargs.pCbSrv = (void *)&EIPNDK_serviceReport;
        dhcpc.param.pOptions = DHCP_OPTIONS;
        dhcpc.param.len = 2;
        CfgAddEntry(hCfg, CFGTAG_SERVICE, CFGITEM_SERVICE_DHCPCLIENT, 0,
                    sizeof(dhcpc), (uint8_t *)&dhcpc, &dhcphandle);
    }

    else
    {

        startupMode = EIP_STARTUP_STATICIP;
        /* Add our global hostname to hCfg (to be claimed in all connected domains) */
        CfgAddEntry(hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_HOSTNAME, 0,
                    strlen(ti_ndk_config_Global_HostName),
                    (uint8_t *)ti_ndk_config_Global_HostName, &cfgHostname);

        /* Configure IP address manually on interface 1 */
        bzero(&NA, sizeof(NA));

        NA.IPAddr  = htonl(eipDevInitConfig.tcpipConfig.deviceIP);
        NA.IPMask  = htonl(eipDevInitConfig.tcpipConfig.networkMask);
        strcpy(NA.Domain, DomainName);
        NA.NetType = 0;
        CfgAddEntry(hCfg, CFGTAG_IPNET, 1, 0,
                    sizeof(CI_IPNET), (uint8_t *)&NA, &cfgAddress);

        /*
         *  Add the default gateway. Since it is the default, the
         *  destination address and mask are both zero (we go ahead
         *  and show the assignment for clarity).
         */
        bzero(&RT, sizeof(RT));
        RT.IPDestAddr = 0;
        RT.IPDestMask = 0;
        RT.IPGateAddr = htonl(eipDevInitConfig.tcpipConfig.gatewayAddress);

        CfgAddEntry(hCfg, CFGTAG_ROUTE, 1, 0,
                    sizeof(CI_ROUTE), (uint8_t *)&RT, &cfgGateway);
    }

    /*Remove Debug prints from NDK*/
    tmp = DBG_NONE;
    CfgAddEntry(hCfg, CFGTAG_OS, CFGITEM_OS_DBGPRINTLEVEL,
                CFG_ADDMODE_UNIQUE, sizeof(uint), (uint8_t *)&tmp, 0);
    /*Set NDK Kernel Task Priority to 11*/
    tmp = 11;
    CfgAddEntry(hCfg, CFGTAG_OS, CFGITEM_OS_TASKPRIKERN,
                CFG_ADDMODE_UNIQUE, sizeof(uint), (uint8_t *)&tmp, 0);

    acdCablePull = 0;
}

/**
 *  @brief  Function to get the TCP/IP details assigend to the interface
 *
 *  @param  tcpIPDetails [out] TCP IP configuration structure pointer
 *
 *  @retval none
 *
 */
void EIPNDK_getTCPIPConfigDetails(ST_TCPIP_CONFIGURATION *tcpIPDetails)
{
    CI_ROUTE CAT;
    CI_IPNET NAT;

    CfgGetImmediate(hCfg, CFGTAG_ROUTE, 1, 1, sizeof(CI_ROUTE),
                    (uint8_t *)&CAT);
    CfgGetImmediate(hCfg, CFGTAG_IPNET, 1, 1, sizeof(NAT), (uint8_t *)&NAT);
    tcpIPDetails->IP_Address = (EIP_DWORD)NAT.IPAddr;
    tcpIPDetails->Network_Mask = (EIP_DWORD)NAT.IPMask;
    tcpIPDetails->Gateway_Address = (EIP_DWORD)CAT.IPGateAddr;

    EIPUTILS_changeIPEndianness(&tcpIPDetails->IP_Address);
    EIPUTILS_changeIPEndianness(&tcpIPDetails->Network_Mask);
    EIPUTILS_changeIPEndianness(&tcpIPDetails->Gateway_Address);

}
/**
 *  @brief  This function removes the cfg entries added during startup.
 *
 *  @param  hCfg [in] handle
 *
 *  @retval none
 *
 */
static void EIPNDK_deregisterIPCfg(void * hCfg)
{

    if(startupMode == EIP_STARTUP_DHCP)
    {
        CfgRemoveEntry(hCfg, dhcphandle);
    }

    else
    {

        CfgRemoveEntry(hCfg, cfgGateway);
        CfgRemoveEntry(hCfg, cfgAddress);
        CfgRemoveEntry(hCfg, cfgHostname);
    }


}
/**
 *  @brief  This function is called after the configuration has booted
 *
 *  @param  none
 *
 *  @retval none
 *
 */
static void EIPNDK_ndkconfigGlobalNetworkOpen()
{
    EIPNDK_registerIPCfg(hCfg);
}
/**
 *  @brief  This function is called when the network is shutting down,
 *          or when it no longer has any IP addresses assigned to it.
 *
 *
 *  @param  none
 *
 *  @retval none
 *
 */
static void EIPNDK_ndkconfigGlobalNetworkClose()
{
    EIPNDK_deregisterIPCfg(hCfg);
}

/**
 *  @brief  This function is called whenever an IP address binding is
 *           added or removed from the system.
 *
 *
 *  @param  IPAddr [in] IP Address
 *  @param  IfIdx  [in] Iinterface ID whose IP is changed
 *  @param  fAdd   [in] 1 - IP is added, 0 IP is removed
 *
 *  @retval none
 *
 */
static void EIPNDK_ndkconfigGlobalNetworkIPAddr(uint32_t IPAddr, uint IfIdx,
        uint fAdd)
{
    uint32_t IPTmp;
    IPTmp = ntohl(IPAddr);

    EIPAPP_ipAddressChange(IPTmp, fAdd);
}
/**
 *  @brief  NDK Task Definition
 *
 *          This is used to implement ACD mechanism. Here we have better control over IP assign mechanism.
 *          We have set the IP assign hook, which will be called by NDk before assigning IP address. IN this
 *          hook we have implemented the ACD probing.
 *
 *          By using Custom NDK task we have better control over its operation
 *
 *  @param  arg0
 *  @param  arg1
 *
 *  @retval none
 *
 */
void EIPNDK_acdStackThread(UArg arg0, UArg arg1)
{
    Clock_Params clockParams;
    int rc;

    /* Create the NDK heart beat */
    Clock_Params_init(&clockParams);
    clockParams.startFlag = TRUE;
    clockParams.period =
        200; /*Period set to 200 as we have 500microsec tick period*/
    Clock_create(&llTimerTick, clockParams.period, &clockParams, NULL);

    /* THIS MUST BE THE ABSOLUTE FIRST THING DONE IN AN APPLICATION!! */
    rc = NC_SystemOpen(NC_PRIORITY_HIGH, NC_OPMODE_INTERRUPT);

    if(rc)
    {

    }

    /* Create and build the system configuration from scratch. */
    hCfg = CfgNew();

    if(!hCfg)
    {
        goto main_exit;
    }

    /* add the Tcp module configuration settings. */
    ti_ndk_config_tcp_init(hCfg);

    Semaphore_pend(appInitSem, BIOS_WAIT_FOREVER);

    /*
     *  Boot the system using this configuration
     *
     *  We keep booting until the function returns 0. This allows
     *  us to have a "reboot" command.
    */
    do
    {
        rc = NC_NetStart(hCfg, EIPNDK_ndkconfigGlobalNetworkOpen,
                         EIPNDK_ndkconfigGlobalNetworkClose,
                         EIPNDK_ndkconfigGlobalNetworkIPAddr);
    }
    while(rc > 0 || rc == ACD_LINKDOWN);

    /*EIP Application Exit*/
    EIPACD_exit();

    /* Delete Configuration */
    CfgFree(hCfg);
    /* Close the OS */
main_exit:
    NC_SystemClose();
    System_flush();
}
