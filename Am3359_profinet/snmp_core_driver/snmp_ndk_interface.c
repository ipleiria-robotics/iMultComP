
/**
 * snmp_ndk_inteface.c - Provides the values read from NDK to fill in the MIB structure
 *
 *
 * Copyright (c) 2012 Texas Instruments Incorporated ALL RIGHTS RESERVED
 *
*/

#include <protocols/snmp/include/snmp_ndk_interface.h>
#ifndef C66X
#include <third_party/protocols/snmp/source/include/snmpagt.h>
#endif



uint16_t numberOfInterfaces()
{
    return SNMP_NUMBER_OF_PORTS;
}

void getNimuStats(NIMU_IcssStatsCmd *statsCommand, int port_no)
{
    int ret_code;
    NIMU_IF_REQ if_req;
    if_req.index = NIMU_MAC_INDEX;

    statsCommand->opcode = ICSS_IOCTL_STAT_GET;
    statsCommand->portNum = port_no;
    ret_code = NIMUIoctl(ICSS_IOCTL_STAT_CMD, &if_req, statsCommand,
                         sizeof(NIMU_IcssStatsCmd *));

    if(ret_code < 0)
    {
        printf("NIMUIOCTL Failed with error code: %d\n", ret_code);
        return;
    }
}

char *getIfDescr(int interface)
{
    if(interface == 0)
    {
        return "TI AM335x Host Port";
    }

    else if(interface == 1)
    {
        return "TI AM335x Phy Port 1";
    }

    if(interface == 2)
    {
        return "TI AM335x Phy Port 2";
    }

    /* interface number out of bounds*/
    return NULL;
}

int getIfType()
{
    /*Other, 117(gigabitEthernet) for CPSW*/
    return 1;
}
uint16_t getIfMtu()
{
    int ret_code;
    uint16_t mtu;
    NIMU_IF_REQ if_req;
    if_req.index = NIMU_MAC_INDEX;
    ret_code = NIMUIoctl(NIMU_GET_DEVICE_MTU, &if_req, &mtu, sizeof(uint16_t));

    if(ret_code < 0)
    {
        printf("NIMUIOCTL Failed with error code: %d\n", ret_code);
        return 0;
    }

    return mtu;
}

uint32_t getIfSpeed(int interface)
{
    NIMU_IcssConsCmd switch_obj;
    NIMU_IcssPortCmd port_obj;
    int ret_code;
    NIMU_IF_REQ if_req;

    if_req.index = NIMU_MAC_INDEX;
    port_obj.port = interface;

    switch_obj.opcode = ICSS_IOCTL_GET_PORT_CONFIG;
    switch_obj.PortCmd  = port_obj;
    ret_code = NIMUIoctl(ICSS_IOCTL_SWITCH_CMD, &if_req, &switch_obj,
                         sizeof(NIMU_IcssConsCmd *));

    if(ret_code < 0)
    {
        printf("NIMUIOCTL Failed with error code: %d\n", ret_code);
        return 0;
    }

    return switch_obj.PortCmd.value;
}

void getIfPhyAddress(int interface, uint8_t *macAddr)
{
    NIMU_IcssConsCmd switch_obj;
    NIMU_IcssPortCmd port_obj;
    int ret_code;
    NIMU_IF_REQ if_req;

    if_req.index = NIMU_MAC_INDEX;
    port_obj.port = interface;

    switch_obj.opcode = ICSS_IOCTL_GET_PORT_CONFIG;
    switch_obj.PortCmd = port_obj;
    ret_code = NIMUIoctl(ICSS_IOCTL_SWITCH_CMD, &if_req, &switch_obj,
                         sizeof(NIMU_IcssConsCmd *));

    if(ret_code < 0)
    {
        printf("NIMUIOCTL Failed with error code: %d\n", ret_code);
        return;
    }

    memcpy(macAddr, &(switch_obj.PortCmd.add), 6);
}

void getIfStatus(long *ifAdminStatus, long *ifOperStatus, u_long *ifLastChange,
                 int port_no)
{
    int tempOperStatus;
    int ret_code;
    NIMU_IcssConsCmd switch_obj;
    NIMU_IcssPortCmd port_obj;

    NIMU_IF_REQ if_req;

    /*ifAdminStatus only supports 1 - UP as for now */
    *ifAdminStatus = 1;

    if_req.index = NIMU_MAC_INDEX;
    port_obj.port = port_no;

    switch_obj.opcode = ICSS_IOCTL_GET_PORT_CONFIG;
    switch_obj.PortCmd = port_obj;
    ret_code = NIMUIoctl(ICSS_IOCTL_SWITCH_CMD, &if_req, &switch_obj,
                         sizeof(NIMU_IcssConsCmd *));

    if(ret_code < 0)
    {
        printf("NIMUIOCTL Failed with error code: %d\n", ret_code);
        return;
    }

    tempOperStatus = *ifOperStatus;

    /*ifAdminStatus is 1 and Port Link is up, then ifOperStatus is UP(1) */
    if(*ifAdminStatus == 1 && switch_obj.PortCmd.port == 1)
    {
        *ifOperStatus = 1;
    }

    /*ifAdminStatus is 1 and Port Link is Down, then ifOperStatus is DOWN(2) */
    else if(*ifAdminStatus == 1 && switch_obj.PortCmd.port == 0)
    {
        *ifOperStatus = 2;
    }

    /*ifAdminStatus is 1 and Port Link is not 1/0, then ifOperStatus is UNKNOWN(4) */
    else if(*ifAdminStatus == 1)
    {
        *ifOperStatus = 4;
    }

    /*Status changed */
    if(tempOperStatus != *ifOperStatus)
    {
        *ifLastChange = ( (psp_get_tick_count() *  100) / PSP_TICK_RATE );
    }
}

u_long getIfInOctets(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.rxNetOctets;
}

u_long getIfInUcastPkts(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.rxUcast;
}

u_long getIfInNUcastPkts(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.rxMcast + statsCommand.statsObj.rxBcast;
}

u_long getIfInDiscards(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.droppedPackets;
}

u_long getIfInErrors(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);

    return statsCommand.statsObj.rxErrorFrames;
}

u_long getIfInUnkownProtos(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.rxUnknownProtocol;
}

u_long getIfOutOctets(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.txNetOctets;
}

u_long getIfOutUcastPkts(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.txUcast;
}

u_long getIfOutNUcastPkts(int port_no)
{
    NIMU_IcssStatsCmd statsCommand;
    getNimuStats(&statsCommand, port_no);
    return statsCommand.statsObj.txMcast + statsCommand.statsObj.txBcast;
}

u_long getIfOutDiscards(int port_no)
{
    /*NOT IMPLEMENTED*/
    return 0;
}

u_long getIfOutErrors(int port_no)
{
    /*NOT IMPLEMENTED*/
    return 0;
}

