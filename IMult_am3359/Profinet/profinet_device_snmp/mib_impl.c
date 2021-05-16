/**
 * @file mib_impl.c
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
#include <xdc/std.h>
#include <string.h>
#include <stdint.h>
#include <Profinet/include/snmp_ndk_interface.h>

#include <Profinet/include/snmp_lldp_interface.h>
#ifndef C66X
#include <Profinet/source_snmp/source/include/snmpagt.h>
#endif
/*For getMacID*/
#include <Profinet/include/iPNLegacy.h>

#include <Include/Board_am335x/board_spi.h>

#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>

oid      sys_id[] = {1, 3, 6, 1, 4, 1, ENTERPRISE, 1, 5, 1, 1, 1};
unsigned sys_id_len = sizeof(sys_id) / sizeof(oid);
oid      oid00[] = {0, 0};    /* Object Id for ipRouteInfo, etc */
long     long_return =  0L;
uint16_t ifNumber;
struct ifEntry_mib  ifp[SNMP_NUMBER_OF_PORTS];

/** @brief Assuming lldpPortConfigTable has NUMBER_OF_LLDP_PORTS entries */
ST_LLDP_INTERFACE_PORT_CONFIG lpGetConfig[NUMBER_OF_LLDP_PORTS];
/** @brief Assuming lldpStatsTxPortTable has NUMBER_OF_LLDP_PORTS entries */
ST_LLDP_INTERFACE_TX_STATS lpGetTxStats[NUMBER_OF_LLDP_PORTS];
/** @brief Assuming lldpStatsRxPortTable has NUMBER_OF_LLDP_PORTS entries */
ST_LLDP_INTERFACE_RX_STATS lpGetRxStats[NUMBER_OF_LLDP_PORTS];
/** @brief Assuming lldpLocPortTable has NUMBER_OF_LLDP_PORTS entries */
ST_LLDP_INTERFACE_DESCRIPTION lpDescription[NUMBER_OF_LLDP_PORTS];
/** @brief Assuming lldpRemTable has NUMBER_OF_LLDP_PORTS * MAX entries per portentries */
ST_LLDP_INTERFACE_REMOTE_INFORMATION lpRemInfo[NUMBER_OF_LLDP_PORTS * 4];
/** @brief FW moved to global */
ST_LLDP_INTERFACE_SYSTEM_CONFIG lpSystem;
/** @brief MM use define for size of addr mgt*/
ST_LLDP_INTERFACE_ADR_MANAGEMENT gStInterfaceAdderMan[1];
/** @brief MM use define for size of addr mgt*/
ST_LLDP_INTERFACE_REMOTE_ADDR_MGT gStInterfaceRemAdderMan[1];
/** @briefMM one per port*/
ST_LLDP_PORT_INFORMATION gStRemotePeerListInfo[NUMBER_OF_LLDP_PORTS];
/** @brief 2 ports and 4 entries per port...*/
ST_LLDP_INTERFACE_REMOTE_ADDR_MGT_LIST
gstRemoteSystemManagmentAddressList[NUMBER_OF_LLDP_PORTS * 4];

ST_LLDP_INTERFACE_REMOTE_UNK_TLV_LIST gRemoteUnkTlvList;
ST_LLDP_INTERFACE_REMOTE_UNK_TLV gstUnknownTlv;
ST_LLDP_INTERFACE_ORG_UNK_TLV_LIST gstOrgUnkTlv;
ST_LLDP_REMOTE_STATS_TABLES gstRemStatsTables = {0};
ST_LLDP_INTERFACE_DEVICENAME_CONFIG gstDevName;
ST_LLDP_INTERFACE_DESCRIPTION gstlldpItfDesc = {0};

char szNameOfStation[255] = {0};

oid goidTab[7];

/**
 * @def SPI_EEPROM_SNMP_SYS_OFFSET
 *       SPI Flash offset of permanent System data
 */
#define SPI_EEPROM_SNMP_SYS_OFFSET 0x310000
/**
 *  @brief Structure to hold the permanent SYSTEM data
 *
 */
typedef struct permanentSysData
{
    uint8_t isValid;
    uint8_t x;
    uint16_t y; /**Padding for 4 byte alignment. Required for K2G Flash read/write Apis */
    char permanentSysContact[SYS_STRING_MAX];    /**< system contact person */
    char permanentSysLocation[SYS_STRING_MAX];   /**< system location */
    char permanentSysName[SYS_STRING_MAX];       /**< system name */
} permanentSysData_t;

permanentSysData_t *permanentSystemData;
/**
 * @def MAC_LENGTH
 *      MAC ID length
 */
#define MAC_LENGTH  6

#define SNMP_MAUTYPE_10BASETXFD         0x000A
#define SNMP_MAUTYPE_10BASETXHD         0x000B
#define SNMP_MAUTYPE_100BASETXFD        0x0010
#define SNMP_MAUTYPE_100BASETXHD        0x0011

/* According to RFC2576 (sec 3.1), we have the following OID
 * mappings of generic trap types
 *  generic-trap parameter   snmpTrapOID.0
 *  ======================   =============
 *  0                        1.3.6.1.6.3.1.1.5.1 (coldStart)
 *  1                        1.3.6.1.6.3.1.1.5.2 (warmStart)
 *  2                        1.3.6.1.6.3.1.1.5.3 (linkDown)
 *  3                        1.3.6.1.6.3.1.1.5.4 (linkUp)
 *  4                        1.3.6.1.6.3.1.1.5.5 (authenticationFailure)
 *  5                        1.3.6.1.6.3.1.1.5.6 (egpNeighborLoss)
 *
 * Hence snmpgenoid[] is used to do the mapping. Based on "int trapType",
 * snmpgenoid[] is updated and used to form the varbind in the PDU
 */

oid      sys_uptime[] = { 1, 3, 6, 1, 2, 1, 1, 3, 0 } ;
unsigned sys_uptime_len = sizeof(sys_uptime) / sizeof(oid);
oid      snmptrapoid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 } ;
unsigned snmptrapoid_len = sizeof(snmptrapoid) / sizeof(oid);
oid      snmpgenoid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 1, 0 } ;


/* Flash header file */


#include <Profinet/include/PN_Handle.h>


extern uint32_t hcc_init(void);
/* array of semaphores required for proper operation of TCP/IP stack
 * (only two slots, NET_RESID and SNMP_RESID, are populated (in
 * NetworkStart())).
 */

#if defined (SOC_AM335x) || defined (SOC_K2G)
extern Board_flashHandle flashHandle;
#else
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>
extern S25FL_Handle flashHandle;
#endif
extern PN_Handle appPnHandle;


/* FUNCTION: dtrap()
 *
 * This function is called when the system discovers an error during
 * its processing.
 *
 * PARAMS: none
 *
 * RETURN: none
 *
 */
void
dtrap(void)
{
   for (;;);
}


/**
 * @brief Interniche SNMP initialization
 * Creates semaphores which is used internally by SNMP,
 * LLDP module init,
 * Stack init.
 *
 * \callgraph
 */
void initSNMP(void)
{
    uint32_t ioMode;
    ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
    /* create the semaphores required by InterNiche TCP/IP stack for
     * mutual exclusion.
     */


    /* initialize the InterNiche TCP/IP stack */
    LLDP_Init();

    (void)hcc_init();

    permanentSystemData = (permanentSysData_t *)malloc(sizeof(permanentSysData_t));

    Board_flashRead(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                    (uint8_t *)permanentSystemData, sizeof(permanentSysData_t), (void *)(&ioMode));

    if(1 == permanentSystemData->isValid)
    {
        strcpy(sysName, permanentSystemData->permanentSysName);
        strcpy(sysContact, permanentSystemData->permanentSysContact);
        strcpy(sysLocation, permanentSystemData->permanentSysLocation);
    }

    /* SNMP Initialization Done */
}

/* FUNCTION: add_lldpConfig()
 *
 * PARAM1: u_char  *var_val
 * PARAM2: u_char  var_val_type
 * PARAM3: int      var_val_len
 * PARAM4: u_char  *statP
 * PARAM5: int      statLen
 *
 *
 *
 * RETURNS:
 * returns an snmp error from npsnmp.h (0 == no error)
 */

t_snmp_ret
add_systemConfig(u_char
                 *var_val,     /* pointer to asn1 encoded set value ??? */
                 u_char   var_val_type,           /* asn1 type of set value */
                 unsigned int      var_val_len,               /* length of set value */
                 u_char     *statP,               /* pointer returned by var_atEntry */
                 int      statLen)                 /* *var_len" from var_atEntry */
{
    char datatmp[SYS_STRING_MAX];
    uint16_t str_len;
    t_snmp_ret ret_val;             /* SNMP return value */
    struct variable *vp =  set_parms.vp;
    uint32_t blockNum, pageNum;
    uint32_t ioMode;
    ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
    str_len = SYS_STRING_MAX;
    ret_val = SNMP_MIB_NOSUCHNAME;

    switch(vp->magic)
    {
        case SYSNAME:
            /* parse the received value */
            ret_val = snmp_comp_parse_string( var_val, var_val_len, var_val_type, (unsigned char *)datatmp, &str_len );
            if ( ret_val == SNMP_SUCCESS )
            {
                /* index to set was left in long_return */
                memset(permanentSystemData->permanentSysName, 0,
                       sizeof(permanentSystemData->permanentSysName));
                memset(sysName, 0, sizeof(sysName));
                memcpy(permanentSystemData->permanentSysName, datatmp, str_len);
                memcpy(sysName, datatmp, str_len);
                permanentSystemData->isValid = 1;

                Board_flashOffsetToBlkPage(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                           &blockNum, &pageNum);

                Board_flashEraseBlk(flashHandle, blockNum);
                Board_flashWrite(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                 (uint8_t *)permanentSystemData, sizeof(permanentSysData_t), (void *)(&ioMode));
            }
            break;

        case SYSCONTACT:
            /* parse the received value */
            ret_val = snmp_comp_parse_string( var_val, var_val_len, var_val_type, (unsigned char *)datatmp, &str_len );
            if ( ret_val == SNMP_SUCCESS )
            {
                memset(permanentSystemData->permanentSysContact, 0,
                       sizeof(permanentSystemData->permanentSysContact));
                memset(sysContact, 0, sizeof(sysContact));
                memcpy(permanentSystemData->permanentSysContact, datatmp, str_len);
                memcpy(sysContact, datatmp, str_len);
                permanentSystemData->isValid = 1;
                Board_flashOffsetToBlkPage(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                           &blockNum, &pageNum);

                Board_flashEraseBlk(flashHandle, blockNum);
                Board_flashWrite(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                 (uint8_t *)permanentSystemData, sizeof(permanentSysData_t), (void *)(&ioMode));
            }
            break;

        case SYSLOCATION:
            /* parse the received value */
            ret_val = snmp_comp_parse_string( var_val, var_val_len, var_val_type, (unsigned char *)datatmp, &str_len );
            if ( ret_val == SNMP_SUCCESS )
            {
                memset(permanentSystemData->permanentSysLocation, 0,
                       sizeof(permanentSystemData->permanentSysLocation));
                memset(sysLocation, 0, sizeof(sysLocation));
                memcpy(permanentSystemData->permanentSysLocation, datatmp, str_len);
                memcpy(sysLocation, datatmp, str_len);
                permanentSystemData->isValid = 1;
                Board_flashOffsetToBlkPage(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                           &blockNum, &pageNum);

                Board_flashEraseBlk(flashHandle, blockNum);
                Board_flashWrite(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                                 (uint8_t *)permanentSystemData, sizeof(permanentSysData_t), (void *)(&ioMode));
            }
            break;

        default:                       /* shouldn't happen */
            dtrap();
    }

    return ret_val;
}

/* 'var_' routine frames for variables table */

/* FUNCTION: var_system()
 *
 * PARAM1: struct variable * vp
 * PARAM2: oid *   name
 * PARAM3: int *   length
 * PARAM4: int      oper
 * PARAM5: int     varlen
 *
 * RETURNS:
 */

/**
 * @brief Callback of SYSTEM variables
 *
 * @param[in] vp  pointer to variables[] entry
 * @param[in,out] name input name requested; output name found
 * @param[in,out] length length of input & output oids
 * @param[in] oper NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1)
 * @param[out] var_len length of variable, or 0 if function
 *
 * @retval Pointer to value of requested OID
 */
u_char *
var_system(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid     *name,       /* IN/OUT - input name requested; output name found */
    int     *length,  /* IN/OUT - length of input & output oids */
    int      oper,    /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int     *var_len) /* OUT - length of variable, or 0 if function */
{


    /**SYS UPTIME calculation variables*/
    uint64_t tickcount;
    Types_FreqHz freq;
    Types_Timestamp64 tmstamp;

    if(oper == SET_OP)
    {
        if(*length != 9)  /* MUST have complete index! */
        {
            return NULL;    /* NOSUCH error reply */
        }

        /* leave set info in static area so add_atEntry() can find it */
        set_parms.access_method = (void *)add_systemConfig;
        set_parms.vp = vp;
        set_parms.name = name;
        set_parms.vbflags |= SF_NOASCIICHK ; /* don't do is_us_ascii() check */


        *var_len = sizeof(long);   /* default length return */
        /* return a pointer to where add_atEntry() available at statP, if required */

        switch(vp->magic)
        {
            case SYSNAME:
                long_return = (long)sysName;
                return (u_char *)&long_return;

            case SYSCONTACT:
                long_return = (long)sysContact;
                return (u_char *)&long_return;

            case SYSLOCATION:
                long_return = (long)sysLocation;
                return (u_char *)&long_return;
        }
    }

    if(oper && (compare(name, *length, vp->name, (int)vp->namelen) != 0))
    {
        return NULL;
    }

    memcpy(name, vp->name, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);   /* default length */

    if(vp->acl == RWRITE)   /* force string length checking on sets */
    {
        set_parms.do_range = TRUE;
        set_parms.hi_range = SYS_STRING_MAX - 1;
        set_parms.lo_range = 0;
    }

    LLDP_GetDescription(&gstlldpItfDesc, 0);

    switch(vp->magic)
    {
        case SYSDESCR:
            strcpy(sys_descr, (char *)gstlldpItfDesc.sSysDesc);
            *var_len = strlen(sys_descr);
            return (u_char *)sys_descr;

        case SYSOBJECTID:
            *var_len = sizeof(sys_id);
            return (u_char *)sys_id;

        case SYSUPTIME:

            Timestamp_get64(&tmstamp);
            Timestamp_getFreq(&freq);
            tickcount =((uint64_t)tmstamp.lo) | ((uint64_t)tmstamp.hi << 32);
            long_return = (long)((tickcount*100)/freq.lo);

            return (u_char *)&long_return;

        case SYSCONTACT:
            *var_len = strlen(sysContact);
            return (u_char *)sysContact;

        case SYSNAME:
            *var_len = strlen(sysName);
            return (u_char *)sysName;

        case SYSLOCATION:
            *var_len = strlen(sysLocation);
            return (u_char *)sysLocation;

        case SYSSERVICES:
            long_return = SUPPORTED_SERVICES ;
            return (u_char *)&long_return;

        default:
            SNMPERROR("var_system: Unknown magic number");
    }

    return NULL;   /* default FAIL return. */
}

/************************************************************************************
 *                          Interface MIB
 ************************************************************************************/

/**
 * @brief Callback of INTERFACE IfNumber
 *
 * @param[in] vp  pointer to variables[] entry
 * @param[in,out] name input name requested; output name found
 * @param[in,out] length length of input & output oids
 * @param[in] oper NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1)
 * @param[out] var_len length of variable, or 0 if function
 *
 * @retval Pointer to value of requested OID
 */
u_char *
var_interfaces(struct variable *vp,
               oid *name, int *length, int oper,
               int *var_len)
{
    if(oper && (compare(name, *length, vp->name, (int)vp->namelen) != 0))
    {
        return NULL;
    }

    memcpy(name, vp->name, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);   /* default length */

    if(vp->acl == RWRITE)   /* force string length checking on sets */
    {
        set_parms.do_range = TRUE;
        set_parms.hi_range = SYS_STRING_MAX - 1;
        set_parms.lo_range = 0;
    }

    switch(vp->magic)
    {
        case IFNUMBER:
            long_return = numberOfInterfaces();
            return (u_char *)&long_return;

        default:
            SNMPERROR("var_system: Unknown magic number");
    }

    return NULL;   /* default FAIL return. */
}


u_char *
var_snmpEngine(struct variable * vp,
    oid* name, int * len, int exact,
    int * var_len)
{
    memcpy((char *)name, (char *)vp->name, (int)vp->namelen * sizeof(oid));
    *var_len = sizeof(long);
    long_return = 1;
    return (u_char *)&long_return;
}


/**
 * @brief Callback of IFENTRY variables
 * Note: newname[10] is used to append the index of this table(in this case Interface no)
 *
 * @param[in] vp  pointer to variables[] entry
 * @param[in,out] name input name requested; output name found
 * @param[in,out] length length of input & output oids
 * @param[in] oper NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1)
 * @param[out] var_len length of variable, or 0 if function
 *
 * @retval Pointer to value of requested OID
 */
u_char *
var_ifEntry(struct variable *vp,
            oid *name, int *length, int oper,
            int *var_len)
{
    unsigned   interface;
    oid        newname[64];

    int        result;
    ifNumber = numberOfInterfaces();
    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < ifNumber; interface++)
    {
        newname[10] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= ifNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    /*1 Interface only */
    ifp[interface].ifIndex = interface + 1;

    switch(vp->magic)
    {
        case IFINDEX:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = ifp[interface].ifIndex;
            return (u_char *)&long_return;

        case IFDESCR:
            ifp[interface].ifDescr = getIfDescr(interface);
            *var_len = strlen(ifp[interface].ifDescr);
            return (u_char *)ifp[interface].ifDescr;

        case IFTYPE:
            ifp[interface].ifType = getIfType();
            long_return = ifp[interface].ifType;
            return (u_char *)&long_return;

        case IFMTU:
            ifp[interface].ifMtu = getIfMtu();
            long_return = ifp[interface].ifMtu;
            return (u_char *)&long_return;

        case IFSPEED:
            ifp[interface].ifSpeed = getIfSpeed(interface);
            long_return = ifp[interface].ifSpeed;
            return (u_char *)&long_return;

        case IFPHYSADDRESS:
            if(NULL == (ifp[interface].ifPhysAddress))
            {
                ifp[interface].ifPhysAddress = (uint8_t *)malloc(MAC_LENGTH);
            }

            PN_EmacSocMACAddrGet(appPnHandle, interface,
                                 (uint8_t *)(ifp[interface].ifPhysAddress));
            *var_len = MAC_LENGTH;
            return (u_char *)ifp[interface].ifPhysAddress;

        case IFADMINSTATUS:
            getIfStatus(&(ifp[interface].ifAdminStatus), &(ifp[interface].ifOperStatus),
                        &(ifp[interface].ifLastChange), interface);
            long_return = ifp[interface].ifAdminStatus;
            return (u_char *)&long_return;

        case IFOPERSTATUS:
            getIfStatus(&(ifp[interface].ifAdminStatus), &(ifp[interface].ifOperStatus),
                        &(ifp[interface].ifLastChange), interface);
            long_return = ifp[interface].ifOperStatus;
            return (u_char *)&long_return;

        case IFLASTCHANGE:
            getIfStatus(&(ifp[interface].ifAdminStatus), &(ifp[interface].ifOperStatus),
                        &(ifp[interface].ifLastChange), interface);
            long_return = ifp[interface].ifLastChange;
            return (u_char *)&long_return;

        case IFINOCTETS:
            ifp[interface].ifInOctets = getIfInOctets(interface);
            long_return = ifp[interface].ifInOctets;
            return (u_char *)&long_return;

        case IFINUCASTPKTS:
            ifp[interface].ifInUcastPkts = getIfInUcastPkts(interface);
            long_return = ifp[interface].ifInUcastPkts;
            return (u_char *)&long_return;

        case IFINNUCASTPKTS:
            ifp[interface].ifInNUcastPkts = getIfInNUcastPkts(interface);
            long_return = ifp[interface].ifInNUcastPkts;
            return (u_char *)&long_return;

        case IFINDISCARDS:
            ifp[interface].ifInDiscards = getIfInDiscards(interface);
            long_return = ifp[interface].ifInDiscards;
            return (u_char *)&long_return;

        case IFINERRORS:
            ifp[interface].ifInErrors = getIfInErrors(interface);
            long_return = ifp[interface].ifInErrors;
            return (u_char *)&long_return;

        case IFINUNKNOWNPROTOS:
            ifp[interface].ifInUnknownProtos = getIfInUnkownProtos(interface);
            long_return = ifp[interface].ifInUnknownProtos;
            return (u_char *)&long_return;

        case IFOUTOCTETS:
            ifp[interface].ifOutOctets = getIfOutOctets(interface);
            long_return = ifp[interface].ifOutOctets;
            return (u_char *)&long_return;

        case IFOUTUCASTPKTS:
            ifp[interface].ifOutUcastPkts = getIfOutUcastPkts(interface);
            long_return = ifp[interface].ifOutUcastPkts;
            return (u_char *)&long_return;

        case IFOUTNUCASTPKTS:
            ifp[interface].ifOutNUcastPkts = getIfOutNUcastPkts(interface);
            long_return = ifp[interface].ifOutNUcastPkts;
            return (u_char *)&long_return;

        case IFOUTDISCARDS:
            ifp[interface].ifOutDiscards = getIfOutDiscards(interface);
            long_return = ifp[interface].ifOutDiscards;
            return (u_char *)&long_return;

        case IFOUTERRORS:
            ifp[interface].ifOutErrors = getIfOutErrors(interface);
            long_return = ifp[interface].ifOutErrors;
            return (u_char *)&long_return;

        case IFSPECIFIC:
            long_return = 0;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (ifp[interface])) + vp->magic);
    }
}


/************************************************************************************
 *                          LLDP MIB
 ************************************************************************************/
/* FUNCTION: add_lldpConfig()
 *
 * PARAM1: u_char  *var_val
 * PARAM2: u_char  var_val_type
 * PARAM3: int      var_val_len
 * PARAM4: u_char  *statP
 * PARAM5: int      statLen
 *
 *
 *
 * RETURNS:
 * returns an snmp error from npsnmp.h (0 == no error)
 */

t_snmp_ret
add_lldpConfig(u_char
               *var_val,       /* pointer to asn1 encoded set value ??? */
               u_char   var_val_type,           /* asn1 type of set value */
               int      var_val_len,            /* length of set value */
               u_char     *statP,               /* pointer returned by var_atEntry */
               int      statLen)                 /* *var_len" from var_atEntry */
{
    ST_LLDP_INTERFACE_GET_TIME_CONFIG lpSetTime;
    t_snmp_ret ret_val;
    struct variable *vp =  set_parms.vp;

    ret_val = SNMP_MIB_NOSUCHNAME;

    switch(vp->magic)
    {
        case LLDPMESSAGETXINTERVAL:
            /* parse the received value */
            ret_val = (int)snmp_comp_parse_int( var_val, var_val_len, var_val_type, &long_return );
            if ( ret_val == SNMP_SUCCESS )
            {
                /* index to set was left in long_return */
                lpSetTime.wMsgTxInterval =
                    long_return;     /*long_return contains the required value*/
                LLDP_SetTimeConfig(&lpSetTime);
            }
            break;

        case LLDPMESSAGETXHOLDMULTIPLIER:
            /* parse the received value */
            ret_val = (int)snmp_comp_parse_int( var_val, var_val_len, var_val_type, &long_return );
            if ( ret_val == SNMP_SUCCESS )
            {
                /* index to set was left in long_return */
                lpSetTime.wMsgHold =
                    long_return;       /*long_return contains the required value*/
                LLDP_SetTimeConfig(&lpSetTime);
            }
            break;

        case LLDPREINITDELAY:
            /* parse the received value */
            ret_val = (int)snmp_comp_parse_int( var_val, var_val_len, var_val_type, &long_return );
            if ( ret_val == SNMP_SUCCESS )
            {
                /* index to set was left in long_return */
                lpSetTime.wMsgTxReInitDelay =
                    long_return;      /*long_return contains the required value*/
                LLDP_SetTimeConfig(&lpSetTime);
            }
            break;

        case LLDPTXDELAY:
            /* parse the received value */
            ret_val = (int)snmp_comp_parse_int( var_val, var_val_len, var_val_type, &long_return );
            if ( ret_val == SNMP_SUCCESS )
            {
                /* index to set was left in long_return */
                lpSetTime.wMsgTxDelay =
                    long_return;        /*long_return contains the required value*/
                LLDP_SetTimeConfig(&lpSetTime);
            }
            break;

        case LLDPNOTIFICATIONINTERVAL:                 /* TODO : NOT IMPLEMENTED */

        /*return 0;*/
        default:                       /* shouldn't happen */
            dtrap();
    }

    return ret_val;
}

u_char *
var_lldpConfiguration(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    ST_LLDP_INTERFACE_GET_TIME_CONFIG lpGetTime;
    LLDP_GetTimeConfig(&lpGetTime);

    if(oper == SET_OP)
    {
        if(*length != 10)  /* MUST have complete index! */
        {
            return NULL;    /* NOSUCH error reply */
        }

        /* leave set info in static area so add_atEntry() can find it */
        set_parms.access_method = add_lldpConfig;
        set_parms.vp = vp;
        set_parms.name = name;
        set_parms.vbflags |= SF_NOASCIICHK ; /* don't do is_us_ascii() check */


        *var_len = sizeof(long);   /* default length return */
        /* return a pointer to where add_atEntry() available at statP, if required */

        switch(vp->magic)
        {
            case LLDPMESSAGETXINTERVAL:
                long_return = (long)lpGetTime.wMsgTxInterval;
                return (u_char *)&long_return;

            case LLDPMESSAGETXHOLDMULTIPLIER:
                long_return = lpGetTime.wMsgHold;
                return (u_char *)&long_return;

            case LLDPREINITDELAY:
                long_return = lpGetTime.wMsgTxReInitDelay;
                return (u_char *)&long_return;

            case LLDPTXDELAY:
                long_return = lpGetTime.wMsgTxDelay;
                return (u_char *)&long_return;

            case LLDPNOTIFICATIONINTERVAL:
                return (u_char *)
                       0;                                    /* TODO Not supported as in IM151 */
        }
    }

    if(oper && (compare(name, *length, vp->name, (int)vp->namelen) != 0))
    {
        return NULL;
    }

    memcpy(name, vp->name, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);   /* default length */

    if(vp->acl == RWRITE)   /* force string length checking on sets */
    {
        set_parms.do_range = TRUE;
        set_parms.hi_range = SYS_STRING_MAX - 1;
        set_parms.lo_range = 0;
    }

    switch(vp->magic)
    {
        case LLDPMESSAGETXINTERVAL:
            long_return = lpGetTime.wMsgTxInterval;
            return (u_char *)&long_return;

        case LLDPMESSAGETXHOLDMULTIPLIER:
            long_return = lpGetTime.wMsgHold;
            return (u_char *)&long_return;                           /* TODO Multiplier? */

        case LLDPREINITDELAY:
            long_return = lpGetTime.wMsgTxReInitDelay;
            return (u_char *)&long_return;

        case LLDPTXDELAY:
            long_return = lpGetTime.wMsgTxDelay;
            return (u_char *)&long_return;

        case LLDPNOTIFICATIONINTERVAL:
            return (u_char *)
                   0;                                      /* TODO Not Implemented? */

        default:
            SNMPERROR("var_system: Unknown magic number");
    }

    return NULL;    /* default FAIL return.*/
}

u_char *
var_lldpPortConfigEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[11] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */


    LLDP_GetPortConfig(&lpGetConfig[interface], interface);         /* TODO interface value? */

    switch(vp->magic)
    {
        case LLDPPORTCONFIGADMINSTATUS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = lpGetConfig[interface].byLLDPAdminStatus;
            return (u_char *)&long_return;

        case LLDPPORTCONFIGNOTIFICATIONENABLE:
            long_return = lpGetConfig[interface].bLLDPortEnabled;
            return (u_char *)&long_return;

        case LLDPPORTCONFIGTLVSTXENABLE:
            long_return = lpGetConfig[interface].byLLDPTlvEnabled;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpGetConfig[interface])) + vp->magic);
    }
}

/*TODO Not mapped to Stack variables*/
u_char *
var_lldpConfigManAddrEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    int i, lldpPortNumber = 1, interface, result;
    oid   current[DEF_VARLEN];

    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));
    /* scan at table for closest match */

    LLDP_GetAddressMgt(&gStInterfaceAdderMan[0]);

    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        current[vp->namelen - 1] = gStInterfaceAdderMan[0].byMgtAddrSubType;

        /* Size of Octec String, IPv4 addr = 4 */
        current[vp->namelen] = 4;
        u_char *cp = NULL;

        /* copy IP address into current. */
        cp = (u_char *) & (gStInterfaceAdderMan[0].dwMgtAddr);

        for(i = vp->namelen + 1; i < vp->namelen + 5;
                i++) /* current[12] - current[15] */
        {
            current[i] = *cp++;
        }

        result = compare(name, *length, current, (int)(vp->namelen + 5));

        /* If the operation is a SET or GET and the object Ids match,
         break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 5) * sizeof(oid));
    *length = (vp->namelen + 5);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPCONFIGMANADDRPORTSTXENABLE:
            long_return = 0xC0;
            *var_len = sizeof(char);
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            /*return (u_char *)(((char *)&(ifp[interface])) + vp->magic);  TODO*/
            return NULL;    /* default FAIL return.*/
    }
}


u_char *
var_lldpStatistics(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    if(oper && (compare(name, *length, vp->name, (int)vp->namelen) != 0))
    {
        return NULL;
    }

    memcpy(name, vp->name, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);   /* default length */

    if(vp->acl == RWRITE)   /* force string length checking on sets */
    {
        set_parms.do_range = TRUE;
        set_parms.hi_range = SYS_STRING_MAX - 1;
        set_parms.lo_range = 0;
    }

    LLDP_GetStatsRemoteTables(&gstRemStatsTables);

    switch(vp->magic)
    {
        case LLDPSTATSREMTABLESLASTCHANGETIME:
            return (u_char *) & (gstRemStatsTables.dwLastChangeTime);

        case LLDPSTATSREMTABLESINSERTS:
            return (u_char *) & (gstRemStatsTables.dwInserts);

        case LLDPSTATSREMTABLESDELETES:
            return (u_char *) & (gstRemStatsTables.dwDeletes);

        case LLDPSTATSREMTABLESDROPS:
            return (u_char *) & (gstRemStatsTables.dwDrops);

        case LLDPSTATSREMTABLESAGEOUTS:
            return (u_char *) & (gstRemStatsTables.dwAgeouts);

        default:
            SNMPERROR("var_system: Unknown magic number");
    }

    return NULL;    /* default FAIL return.*/
}

u_char *
var_lldpStatsTxPortEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;

    lldpPortNumber = numberOfLldpInterfaces();

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[11] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */


    LLDP_Tx_Stats(&lpGetTxStats[interface], interface);

    switch(vp->magic)
    {
        case LLDPSTATSTXPORTFRAMESTOTAL:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = lpGetTxStats[interface].dwFrameOutTotal;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpGetTxStats[interface])) + vp->magic);
    }

}

u_char *
var_lldpStatsRxPortEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;

    lldpPortNumber = numberOfLldpInterfaces();

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[11] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */


    LLDP_Rx_Stats(&lpGetRxStats[interface], interface);

    switch(vp->magic)
    {
        case LLDPSTATSRXPORTFRAMESDISCARDEDTOTAL:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = lpGetRxStats[interface].dwStatsFramesDiscardedTotal;
            return (u_char *)&long_return;

        case LLDPSTATSRXPORTFRAMESERRORS:
            long_return = lpGetRxStats[interface].dwStatsFramesInErrorsTotal;
            return (u_char *)&long_return;

        case LLDPSTATSRXPORTFRAMESTOTAL:
            long_return = lpGetRxStats[interface].dwStatsFramesInTotal;
            return (u_char *)&long_return;

        case LLDPSTATSRXPORTTLVSDISCARDEDTOTAL:
            long_return = lpGetRxStats[interface].dwStatsTLVsDiscardedTotal;
            return (u_char *)&long_return;

        case LLDPSTATSRXPORTTLVSUNRECOGNIZEDTOTAL:
            long_return = lpGetRxStats[interface].dwStatsTLVsUnrecognizedTotal;
            return (u_char *)&long_return;

        case LLDPSTATSRXPORTAGEOUTSTOTAL:
            long_return = lpGetRxStats[interface].dwStatsAgeOutsTotal;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpGetRxStats[interface])) + vp->magic);
    }

}

u_char *
var_lldpLocalSystemData(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    if(oper && (compare(name, *length, vp->name, (int)vp->namelen) != 0))
    {
        return NULL;
    }

    memcpy(name, vp->name, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);   /* default length */

    if(vp->acl == RWRITE)   /* force string length checking on sets */
    {
        set_parms.do_range = TRUE;
        set_parms.hi_range = SYS_STRING_MAX - 1;
        set_parms.lo_range = 0;
    }

    LLDP_GetSystemConfig(&lpSystem);
    LLDP_GetDescription(&lpDescription[0],
                        0);                         /* TODO port number?*/

    switch(vp->magic)
    {
        case LLDPLOCCHASSISIDSUBTYPE:
            long_return = lpSystem.byChassisIDSubType;
            return (u_char *)&long_return;

        case LLDPLOCCHASSISID:
            *var_len = strlen((const char *)lpSystem.sChassisID);
            return (u_char *)lpSystem.sChassisID;

        case LLDPLOCSYSNAME:
            *var_len = strlen((const char *)lpDescription[0].sSysName);
            return (u_char *)lpDescription[0].sSysName;

        case LLDPLOCSYSDESC:
            *var_len = strlen((const char *)lpDescription[0].sSysDesc);
            return (u_char *)lpDescription[0].sSysDesc;

        case LLDPLOCSYSCAPSUPPORTED:
            long_return = lpDescription[0].wSysCap;
            return (u_char *)&long_return;

        case LLDPLOCSYSCAPENABLED:
            long_return = lpDescription[0].wSysCapEnabled;
            return (u_char *)&long_return;

        default:
            SNMPERROR("var_system: Unknown magic number");
    }

    return NULL;    /* default FAIL return.*/
}

u_char *
var_lldpLocPortEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{


    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;

    lldpPortNumber = numberOfLldpInterfaces();

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[11] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    LLDP_GetDescription(&lpDescription[interface], interface);
    LLDP_GetPortConfig(&lpGetConfig[interface], interface);

    switch(vp->magic)
    {
        case LLDPLOCPORTIDSUBTYPE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = lpGetConfig[interface].byPortSubType;
            return (u_char *)&long_return;

        case LLDPLOCPORTID:
            *var_len = strlen((const char *)lpGetConfig[interface].sPortID);
            return (u_char *)lpGetConfig[interface].sPortID;

        case LLDPLOCPORTDESC:
            ifp[interface].ifDescr = getIfDescr(interface + 1);
            *var_len = strlen(ifp[interface].ifDescr);
            return (u_char *)ifp[interface].ifDescr;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpDescription[interface])) + vp->magic);
    }
}

u_char *
var_lldpLocManAddrEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    int        lldpPortNumber, i;
    int        result, interface;
    lldpPortNumber = numberOfLldpInterfaces();
    uint16_t oid_length = 7;

    LLDP_GetAddressMgt(&gStInterfaceAdderMan[0]);

    if(gStInterfaceAdderMan[0].wResult == OS_NOT_OK)
    {
        return NULL;
    }

    /*
    * Below excluded code snippet shows implementation of atEntry table in rfc1213
    *
    * It searches the ARP table for a valid entry. In case of LLDP, it should search the
    * lldpLocManAddr to get a positive hit(for loop).
    *
    * current[vp->namelen-1] = 1; which defines the type. 1 in case of LLDP will be
    * ipv4(check definition of lldpLocManAddrSubtype in lldp.mib)
    *
    * The management address should then be appended to "current[vp->namelen+1]" along with the size(current[vp->namelen])
    *
    * In the end, a valid OID(in current) would look like - 1.0.8802.1.1.2.1.3.8.1.3.1.4.172.16.101.218
    *
    * 1.0.8802.1.1.2.1.3.8.1.3 - Original OID(already in vp->name)
    * 1 - ipv4
    * 4 - Size of Octect string
    * 172.16.101.218 - lldpLocManAddr
    *
    * Then processed according to GET/SET/GETNEXT requests.
    *
    * All the following tables require this change to be made
    *
    *
    */

    oid   current[DEF_VARLEN];

    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));
    /* scan at table for closest match */

    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        current[vp->namelen - 1] = 1;

        /* Size of Octec String, IPv4 addr = 4 */
        current[vp->namelen] = 4;
        u_char *cp = NULL;

        /* copy IP address into current. */
        cp = (u_char *) & (gStInterfaceAdderMan[0].dwMgtAddr);

        for(i = vp->namelen + 1; i < vp->namelen + 5;
                i++) /* current[12] - current[15] */
        {
            current[i] = *cp++;
        }

        result = compare(name, *length, current, (int)(vp->namelen + 5));

        /* If the operation is a SET or GET and the object Ids match,
        break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 5) * sizeof(oid));
    *length = (vp->namelen + 5);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPLOCMANADDRLEN:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = 5; /* Todo check this*/
            return (u_char *)&long_return;

        case LLDPLOCMANADDRIFSUBTYPE:
            long_return = gStInterfaceAdderMan[0].byMgtAddrItfSubType;
            return (u_char *)&long_return;

        case LLDPLOCMANADDRIFID:
            long_return = gStInterfaceAdderMan[0].dwMgtAddrItfNumber;
            return (u_char *)&long_return;

        case LLDPLOCMANADDROID:
            /*Use value from stack gStInterfaceAdderMan[0].byMgtAddrOidLength;*/
            /*Use value from stack gStInterfaceAdderMan[0].sAddrMgtOid;*/
            oid_length = sizeof(goidTab);
            (void)snmp_comp_parse_objid( gStInterfaceAdderMan[0].sOID
                                        , gStInterfaceAdderMan[0].byMgtAddrOidLength
                                        , ASN_OBJECT_ID
                                        , goidTab
                                        , &oid_length );
            //*var_len = sizeof(goidTab);
            *var_len = oid_length * 4u;
            return (u_char *)goidTab;

        default:   /* return 32 bit counter from table */
            return NULL;      /* FW fixed warning but what to return here really?*/
    }
}

u_char *
var_lldpRemEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{


    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();
    APP_BYTE byIndex = 0;
    int flag;
    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* iterator value : for each port */
    {
        /* Fetch out list of remote system (index)*/
        LLDP_GetPortRemotePeer(&gStRemotePeerListInfo[interface],
                               interface);    /* Get a list of Remotepeer index */
        flag = 0;

        if(gStRemotePeerListInfo[interface].wResult == OS_NOT_OK)
        {
            continue;
        }

        for(byIndex = 0; byIndex < gStRemotePeerListInfo[interface].byRemSysOnPortIndex;
                byIndex++)
        {

            LLDP_GetPortRemoteInformation(&lpRemInfo[byIndex],
                                          gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);            /* interface correct value? TODO */

            if(lpRemInfo[byIndex].wResult == OS_NOT_OK)
            {
                continue;
            }

            current[vp->namelen - 1] = lpRemInfo[byIndex].dwTimeMark;
            current[vp->namelen] = interface +
                                   1;                                        /* lldpRemLocalPortNum == local reception port of remote information*/
            current[vp->namelen + 1] = lpRemInfo[byIndex].byRemIndex + 1;

            result = compare(name, *length, current, (int)vp->namelen + 2);

            /* If the operation is a SET or GET and the object Ids match,
            break out of the loop */
            if(oper && (result == 0))
            {
                flag = 1;
                break;
            }

            if(!oper && (result < 0))
            {
                flag = 2;
                break;
            }
        }

        if(flag != 0)
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPREMCHASSISIDSUBTYPE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = lpRemInfo[byIndex].byRemChassisIdSubType;
            return (u_char *)&long_return;

        case LLDPREMCHASSISID:
            *var_len = strlen((const char *)lpRemInfo[byIndex].byRemChassisId);
            return (u_char *)lpRemInfo[byIndex].byRemChassisId;

        case LLDPREMPORTIDSUBTYPE:
            long_return = lpRemInfo[byIndex].byRemPortIdSubType;
            return (u_char *)&long_return;

        case LLDPREMPORTID:
            *var_len = strlen((const char *)lpRemInfo[byIndex].byRemPortId);
            return (u_char *)lpRemInfo[byIndex].byRemPortId;

        case LLDPREMPORTDESC:
            *var_len = strlen((const char *)lpRemInfo[byIndex].sPortDescription);
            return (u_char *)lpRemInfo[byIndex].sPortDescription;

        case LLDPREMSYSNAME:
            *var_len = strlen((const char *)lpRemInfo[byIndex].sSysName);
            return (u_char *)lpRemInfo[byIndex].sSysName;

        case LLDPREMSYSDESC:
            *var_len = strlen((const char *)lpRemInfo[byIndex].sSysDescription);
            return (u_char *)lpRemInfo[byIndex].sSysDescription;

        case LLDPREMSYSCAPSUPPORTED:
            long_return = lpRemInfo[byIndex].wRemCapability;
            return (u_char *)&long_return;

        case LLDPREMSYSCAPENABLED:
            long_return = lpRemInfo[byIndex].wRemCapabilityEnabled;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpRemInfo[byIndex])) + vp->magic);
    }
}

u_char *
var_lldpRemManAddrEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    int byIndex;
    int byIndex2;
    int i;
    int flag;
    uint16_t oid_length = 7;


    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];

    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        flag = 0;
        LLDP_GetPortRemotePeer(&gStRemotePeerListInfo[interface],
                               interface);    /* Get a list of Remotepeer index */

        if(gStRemotePeerListInfo[interface].wResult == OS_NOT_OK)
        {
            continue;
        }

        for(byIndex = 0; byIndex < gStRemotePeerListInfo[interface].byRemSysOnPortIndex;
                byIndex++)
        {
            /*For each remote system*/
            /* Fetch out list of managment addresse index*/
            LLDP_GetRemAddressMgtList(
                &gstRemoteSystemManagmentAddressList[NUMBER_OF_LLDP_PORTS * interface +
                        byIndex], gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);

            if(gstRemoteSystemManagmentAddressList[/*interface*/NUMBER_OF_LLDP_PORTS *
                    interface + byIndex].wResult == OS_NOT_OK)
            {
                continue;
            }


            /* Get all remote Address managment record */
            for(byIndex2 = 0;
                    byIndex2 < gstRemoteSystemManagmentAddressList[/*interface*/NUMBER_OF_LLDP_PORTS
                            * interface + byIndex].byNumberOfAddrMgtIndex; byIndex2++)
            {
                LLDP_GetRemAddressMgt(&gStInterfaceRemAdderMan[byIndex2],
                                      gstRemoteSystemManagmentAddressList[NUMBER_OF_LLDP_PORTS * interface +
                                              byIndex].byRemIndex ,
                                      gstRemoteSystemManagmentAddressList[interface].byMgtAddrIndex[byIndex2]);

                if(gStInterfaceRemAdderMan[byIndex2].wResult == OS_NOT_OK)
                {
                    continue;
                }

                LLDP_GetPortRemoteInformation(&lpRemInfo[byIndex],
                                              gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);         /* interface correct value? TODO */

                if(lpRemInfo[byIndex].wResult == OS_NOT_OK)
                {
                    continue;
                }

                current[vp->namelen - 1] = lpRemInfo[byIndex].dwTimeMark;
                current[vp->namelen] = interface +
                                       1;                                 /* lldpRemLocalPortNum == interface? TODO */
                current[vp->namelen + 1] = lpRemInfo[byIndex].byRemIndex + 1;
                current[vp->namelen + 2] = gStInterfaceRemAdderMan[byIndex2].byAddrMgtSubType;

                /* Size of Octec String, IPv4 addr = 4 */
                current[vp->namelen + 3] = 4;


                u_char *cp = NULL;
                /* copy IP address into current. */
                cp = (u_char *)(gStInterfaceRemAdderMan[byIndex2].byAddrMgtAddr);

                for(i = vp->namelen + 4; i < vp->namelen + 8;
                        i++) /* current[12] - current[15] */
                {
                    current[i] = *cp++;
                }

                result = compare(name, *length, current, (int)vp->namelen + 8);

                /* If the operation is a SET or GET and the object Ids match,break out of the loop */
                if(oper && (result == 0))
                {
                    flag = 1;
                    break;
                }

                if(!oper && (result < 0))
                {
                    flag = 2;
                    break;
                }
            }

            if(flag != 0)
            {
                break;
            }
        }

        if(flag != 0)
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 8) * sizeof(oid));
    *length = (vp->namelen + 8);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                             /*TODO Mapping incorrect ? */
    {
        case LLDPREMMANADDRIFSUBTYPE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = gStInterfaceRemAdderMan[byIndex2].byAddrMgtIfSubType;
            return (u_char *)&long_return;

        case LLDPREMMANADDRIFID:
            long_return = gStInterfaceRemAdderMan[byIndex2].dwAddrMgtIfNumber;
            return (u_char *)&long_return;

        case LLDPREMMANADDROID:
            /*Use value from stack gStInterfaceAdderMan[0].byMgtAddrOidLength;*/
            /*Use value from stack gStInterfaceAdderMan[0].sAddrMgtOid;*/

            oid_length = sizeof(goidTab);
            (void)snmp_comp_parse_objid( gStInterfaceAdderMan[0].sOID
                                        , gStInterfaceAdderMan[0].byMgtAddrOidLength
                                        , ASN_OBJECT_ID
                                        , goidTab
                                        , &oid_length );
            //*var_len = sizeof(goidTab);
            *var_len = oid_length;
            return (u_char *)goidTab;

        default:   /* return 32 bit counter from table */
            return (u_char *)(((char *) & (lpRemInfo[interface])) + vp->magic);
    }
}

u_char *
var_lldpRemUnknownTLVEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int byIndex;
    int byIndex2;
    int result;
    int flag;
    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));
    /* scan at table for closest match */
    /* find "next" interface */
    lldpPortNumber = numberOfLldpInterfaces();

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        flag = 0;
        /* Fetch out list of remote system (index)*/
        LLDP_GetPortRemotePeer(&gStRemotePeerListInfo[interface],
                               interface);    /* Get a list of Remotepeer index */

        if(gStRemotePeerListInfo[interface].wResult == OS_NOT_OK)
        {
            continue;
        }

        /* Fetch out list of UnknownTlvList*/
        for(byIndex = 0; byIndex < gStRemotePeerListInfo[interface].byRemSysOnPortIndex;
                byIndex++)
        {
            LLDP_GetRemoteUnknownTlvList(&gRemoteUnkTlvList,
                                         gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);

            if(gRemoteUnkTlvList.wResult == OS_NOT_OK)
            {
                continue;
            }

            for(byIndex2 = 0; byIndex2 < gRemoteUnkTlvList.byNumberUnknownTlvIndex;
                    byIndex2++)
            {
                LLDP_GetRemoteUnknownTlv(&gstUnknownTlv ,
                                         gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex],
                                         gRemoteUnkTlvList.byUnknownTlvIndex[byIndex2]);

                if(gstUnknownTlv.wResult == OS_NOT_OK)
                {
                    continue;
                }

                LLDP_GetPortRemoteInformation(&lpRemInfo[byIndex],
                                              byIndex);         /* interface correct value? TODO */

                if(lpRemInfo[interface].wResult == OS_NOT_OK)
                {
                    continue;
                }

                current[vp->namelen - 1] = lpRemInfo[byIndex].dwTimeMark;
                current[vp->namelen] =
                    byIndex+1;                                      /* lldpRemLocalPortNum == interface? TODO */
                current[vp->namelen + 1] = lpRemInfo[byIndex].byRemIndex+1;
                current[vp->namelen + 2] = gstUnknownTlv.byUnknownTlvType;

                result = compare(name, *length, current, (int)vp->namelen + 3);

                /* If the operation is a SET or GET and the object Ids match,
                   break out of the loop */
                if(oper && (result == 0))
                {
                    flag = 1;
                    break;
                }

                if(!oper && (result < 0))
                {
                    flag = 2;
                    break;
                }
            }

            if(flag != 0)
            {
                break;
            }
        }

        if(flag != 0)
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 3) * sizeof(oid));
    *length = (vp->namelen + 3);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                           /*TODO Mapping incorrect ? */
    {
        case LLDPREMUNKNOWNTLVINFO:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return = (long)gstUnknownTlv.sUnknownTlvInfo;
            *var_len = gstUnknownTlv.wUnknownTlvLenght;
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpRemOrgDefInfoEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int byIndex;
    int byIndex2;
    int result;
    int flag;
    oid   current[DEF_VARLEN];
    ST_LLDP_INTERFACE_ORG_UNK_TLV gstOrgTlv;
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));
    /* scan at table for closest match */
    /* find "next" interface */
    lldpPortNumber = numberOfLldpInterfaces();

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        flag = 0;
        /* Fetch out list of remote system (index)*/
        LLDP_GetPortRemotePeer(&gStRemotePeerListInfo[interface],
                               interface);    /* Get a list of Remotepeer index */

        if(gStRemotePeerListInfo[interface].wResult == OS_NOT_OK)
        {
            continue;
        }

        /* Fetch out list of UnknownTlvList*/
        for(byIndex = 0; byIndex < gStRemotePeerListInfo[interface].byRemSysOnPortIndex;
                byIndex++)
        {

            LLDP_GetRemoteOrgTlvList(&gstOrgUnkTlv,
                                     gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);

            if(gstOrgUnkTlv.wResult == OS_NOT_OK)
            {
                continue;
            }

            for(byIndex2 = 0; byIndex2 < gstOrgUnkTlv.byNumberOrgTlvIndex; byIndex2++)
            {
                LLDP_GetOrgUnknownTlv(&gstOrgTlv ,
                                      gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex],
                                      gstOrgUnkTlv.byRemoteOrgUnkTlvIndex[byIndex2]);

                if(gstUnknownTlv.wResult == OS_NOT_OK)
                {
                    continue;
                }

                LLDP_GetPortRemoteInformation(&lpRemInfo[byIndex],
                                              byIndex);         /* interface correct value? TODO */

                if(lpRemInfo[interface].wResult == OS_NOT_OK)
                {
                    continue;
                }

                current[vp->namelen - 1] = lpRemInfo[byIndex].dwTimeMark;
                current[vp->namelen] =
                    byIndex+1;                                      /* lldpRemLocalPortNum == interface? TODO */
                current[vp->namelen + 1] = lpRemInfo[byIndex].byRemIndex+1;
                current[vp->namelen + 2] =
                    0;                                        /*TODO Fill in lldpRemOrgDefInfoOUI HARDCODED */
                current[vp->namelen + 3] =
                    0;                                        /*TODO Fill in lldpRemOrgDefInfoSubtype HARDCODED */
                current[vp->namelen + 4] =
                    0;                                        /*TODO Fill in lldpRemOrgDefInfoIndex HARDCODED */

                result = compare(name, *length, current, (int)vp->namelen + 5);

                /* If the operation is a SET or GET and the object Ids match,
                   break out of the loop */
                if(oper && (result == 0))
                {
                    flag = 1;
                    break;
                }

                if(!oper && (result < 0))
                {
                    flag = 2;
                    break;
                }


            }

            if(flag != 0)
            {
                break;
            }
        }

        if(flag != 0)
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 5) * sizeof(oid));
    *length = (vp->namelen + 5);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                           /*TODO Mapping incorrect ? */
    {
        case LLDPREMORGDEFINFO:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                      /* TODO Return lldpRemOrgDefInfo HARDCODED NOW*/
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */       
            return NULL;
    }
}

/************************************************************************************
 *                          LLDP_MIB_EXT_PNIO
 ************************************************************************************/
u_char *
var_lldpXPnoConfigEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    /* TODO Add code for SET_OP */
    if(oper == SET_OP)
    {
#if 0

        if(*length != 10)  /* MUST have complete index! */
        {
            return NULL;    /* NOSUCH error reply */
        }

        /* leave set info in static area so add_atEntry() can find it */
        set_parms.access_method = add_lldpConfig;
        set_parms.vp = vp;
        set_parms.name = name;
        set_parms.vbflags |= SF_NOASCIICHK ; /* don't do is_us_ascii() check */


        *var_len = sizeof(long);   /* default length return */
        /* return a pointer to where add_atEntry() available at statP, if required */

        switch(vp->magic)
        {
            case LLDPMESSAGETXINTERVAL:
                long_return = (long)lpGetTime.wMsgTxInterval;
                return (u_char *)&long_return;

            case LLDPMESSAGETXHOLDMULTIPLIER:
                long_return = lpGetTime.wMsgHold;
                return (u_char *)&long_return;

            case LLDPREINITDELAY:
                long_return = lpGetTime.wMsgTxReInitDelay;
                return (u_char *)&long_return;

            case LLDPTXDELAY:
                long_return = lpGetTime.wMsgTxDelay;
                return (u_char *)&long_return;

            case LLDPNOTIFICATIONINTERVAL:
                return (u_char *)
                       0;                                    /* TODO Not supported as in IM151 */
        }

#endif
    }

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXPNOCONFIGSPDTXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOCONFIGPORTSTATUSTXENABLE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOCONFIGALIASTXENABLE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOCONFIGMRPTXENABLE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOCONFIGPTCPTXENABLE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXPnoLocEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface + 1);
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }


    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    LLDP_GetSystemConfig(&lpSystem);
    LLDP_GetDeviceNameConfig(&gstDevName);
    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXPNOLOCLPDVALUE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTTXDVALUE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTRXDVALUE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT2:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTNOS:
            *var_len = strlen((const char *)
                              gstDevName.sDeviceName);                              /*TODO NameOfStation == chasis ID???*/
            return (u_char *)gstDevName.sDeviceName;

        case LLDPXPNOLOCPORTMRPUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTMRRTSTATUS:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTPTCPMASTER:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTPTCPSUBDOMAINUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTPTCPIRDATAUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTMODERT3:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTPERIODLENGTH:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTPERIODVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTREDOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTREDVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTORANGEOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTORANGEVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTGREENOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTGREENVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3OPTIMIZATIONSUPPORTED:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENINGSUPPORTED:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3PREAMBLESHORTENING:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3FRAGMENTATIONSUPPORTED:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOLOCPORTSTATUSRT3FRAGMENTATION:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }

}

u_char *
var_lldpXPnoRemEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();
    int byIndex;
    int flag;
    unsigned short usIndex = 0;
    u_char  byDot = 0;

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    /* TODO Correct APIs? Copied from var_lldpRemEntry */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* iterator value : for each port */
    {
        /* Fetch out list of remote system (index)*/
        LLDP_GetPortRemotePeer(&gStRemotePeerListInfo[interface],
                               interface);    /* Get a list of Remotepeer index */
        flag = 0;

        if(gStRemotePeerListInfo[interface].wResult == OS_NOT_OK)
        {
            continue;
        }

        for(byIndex = 0; byIndex < gStRemotePeerListInfo[interface].byRemSysOnPortIndex;
                byIndex++)
        {

            LLDP_GetPortRemoteInformation(
                &lpRemInfo[byIndex], /*byIndex*/gStRemotePeerListInfo[interface].byRemSysOnPort[byIndex]);         /* interface correct value? TODO */

            if(lpRemInfo[byIndex].wResult == OS_NOT_OK)
            {
                continue;
            }

            current[vp->namelen - 1] = lpRemInfo[byIndex].dwTimeMark;
            current[vp->namelen] = interface +
                                   1;                                     /* lldpRemLocalPortNum == interface? TODO */
            current[vp->namelen + 1] = lpRemInfo[byIndex].byRemIndex + 1;

            result = compare(name, *length, current, (int)vp->namelen + 2);

            /* If the operation is a SET or GET and the object Ids match,
            break out of the loop */
            if(oper && (result == 0))
            {
                flag = 1;
                break;
            }

            if(!oper && (result < 0))
            {
                flag = 2;
                break;
            }
        }

        if(flag != 0)
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXPNOREMLPDVALUE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTTXDVALUE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTRXDVALUE:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTSTATUSRT2:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTSTATUSRT3:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTNOS:
            {
                /* Get NameOfstation */
                for(usIndex = 0; usIndex < lpRemInfo[byIndex].byPortIdLen; usIndex++)
                {
                    if(lpRemInfo[byIndex].byRemPortId[usIndex] == '.')
                    {
                        byDot = 1;
                        break;
                    }
                }

                if(byDot == 1)
                {
                    strncpy(szNameOfStation,
                            (char *)(&(lpRemInfo[byIndex].byRemPortId[usIndex]) + 1),
                            lpRemInfo[byIndex].byPortIdLen - strlen("port-xxx."));
                }

                else
                {
                    strcpy(szNameOfStation, (char *)lpRemInfo[byIndex].byRemChassisId);
                }

                *var_len = strlen((const char *)
                                  szNameOfStation);                                    /*TODO NameOfStation == chasisID??? */
                return (u_char *)szNameOfStation;
            }

        case LLDPXPNOREMPORTMRPUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTMRRTSTATUS:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTPTCPMASTER:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTPTCPSUBDOMAINUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTPTCPIRDATAUUID:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTMODERT3:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTPERIODLENGTH:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTPERIODVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTREDOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTREDVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTORANGEOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTORANGEVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTGREENOFFSET:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTGREENVALIDITY:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTSTATUSRT3PREAMBLESHORTENING:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXPNOREMPORTSTATUSRT3FRAGMENTATION:
            long_return =
                0;                                                                  /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

/************************************************************************************
 *                          LLDP_MIB_EXT3
 ************************************************************************************/
u_char *
var_lldpXdot3PortConfigEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT3PORTCONFIGTLVSTXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            /*return (u_char *)(((char *)&(ifp[interface])) + vp->magic);  TODO*/
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot3LocPortEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT3LOCPORTAUTONEGSUPPORTED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                1;                                                              /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPORTAUTONEGENABLED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                1;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPORTAUTONEGADVERTISEDCAP:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                1;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPORTOPERMAUTYPE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                    SNMP_MAUTYPE_100BASETXFD;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            /*return (u_char *)(((char *)&(ifp[interface])) + vp->magic);  TODO*/
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot3LocPowerEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT3LOCPOWERPORTCLASS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                              /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPOWERMDISUPPORTED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPOWERMDIENABLED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPOWERPAIRCONTROLABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPOWERPAIRS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCPOWERCLASS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */      
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot3LocLinkAggEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT3LOCLINKAGGSTATUS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                              /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3LOCLINKAGGPORTID:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */          
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot3LocMaxFrameSizeEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT3LOCMAXFRAMESIZE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                              /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            /*return (u_char *)(((char *)&(ifp[interface])) + vp->magic);  TODO*/
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot3RemPortEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;

        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


            switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
            {
            case LLDPXDOT3REMPORTAUTONEGSUPPORTED:
                long_return =1;
                return (u_char *)&long_return;

            case  LLDPXDOT3REMPORTAUTONEGENABLED:
                long_return =1;
                return (u_char *)&long_return;

            case LLDPXDOT3REMPORTAUTONEGADVERTISEDCAP:
                long_return =1;
                return (u_char *)&long_return;

            case LLDPXDOT3REMPORTOPERMAUTYPE:
                long_return =0;
                return (u_char *)&long_return;
                default:   /* return 32 bit counter from table */
                    return NULL;
    }
}

u_char *
var_lldpXdot3RemPowerEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;

        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT3REMPOWERCLASS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot3RemLinkAggEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;

        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT3REMLINKAGGSTATUS:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT3REMLINKAGGPORTID:
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot3RemMaxFrameSizeEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;

        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT3REMMAXFRAMESIZE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

/************************************************************************************
 *                          LLDP_MIB_EXT1
 ************************************************************************************/

u_char *
var_lldpXdot1ConfigPortVlanEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    /* Index used of Augmented table lldpPortConfigEntry */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpPortConfigPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)
    {
        case LLDPXDOT1CONFIGPORTVLANTXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                              /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;    /* default FAIL return.*/
    }
}

u_char *
var_lldpXdot1ConfigVlanNameEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    /* Index used lldpXdot1LocVlanId of Augmented table */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocVlanId Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1CONFIGVLANNAMETXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1ConfigProtoVlanEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    /* Index used - lldpXdot1LocProtoVlanId of Augmented table */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocProtoVlanId Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1CONFIGPROTOVLANTXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1ConfigProtocolEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{
    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    /* Index used - lldpXdot1LocProtocolIndex of Augmented Table */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocProtocolIndex Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1CONFIGPROTOCOLTXENABLE:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1LocEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    oid        newname[64];
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();          /* TODO port number ? */

    memcpy((char *)newname, (char *)vp->name, (int)vp->namelen * sizeof(oid));

    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber; interface++)
    {
        newname[14] = (oid)(interface +
                            1);                                          /*TODO interface == lldpLocPortNum? */
        result = compare(name, *length, newname, (int)vp->namelen);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >=
            lldpPortNumber)                                                 /*TODO Correct count? */
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)newname, (int)vp->namelen * sizeof(oid));
    *length = vp->namelen;
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1LOCPORTVLANID:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1LocProtoVlanEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocProtoVlanId Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1LOCPROTOVLANSUPPORTED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT1LOCPROTOVLANENABLED:
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1LocVlanNameEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocVlanId Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */

    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1LOCVLANNAME:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1LocProtocolEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        current[vp->namelen - 1] = interface + 1;
        current[vp->namelen] =
            0;                                                /* lldpXdot1LocProtocolIndex Hardcoded to 0 TODO */

        result = compare(name, *length, current, (int)vp->namelen + 1);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 1) * sizeof(oid));
    *length = (vp->namelen + 1);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1LOCPROTOCOLID:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1RemEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;

        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1REMPORTVLANID:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1RemProtoVlanEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;
        current[vp->namelen + 2] =
            0;                                            /* lldpXdot1RemProtoVlanId Hardcoded to 0!! TODO */


        result = compare(name, *length, current, (int)vp->namelen + 2);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 2) * sizeof(oid));
    *length = (vp->namelen + 2);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1REMPROTOVLANSUPPORTED:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        case LLDPXDOT1REMPROTOVLANENABLED:
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1RemVlanNameEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;
        current[vp->namelen + 2] =
            0;                                            /* lldpXdot1RemVlanId Hardcoded to 0!! TODO */


        result = compare(name, *length, current, (int)vp->namelen + 3);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 3) * sizeof(oid));
    *length = (vp->namelen + 3);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1REMVLANNAME:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}

u_char *
var_lldpXdot1RemProtocolEntry(
    struct variable *vp,    /* IN - pointer to variables[] entry */
    oid    *name,       /* IN/OUT - input name requested; output name found */
    int    *length,     /* IN/OUT - length of input & output oids */
    int oper,       /* IN - NEXT_OP (=0), GET_OP (=1), or SET_OP (=-1) */
    int    *var_len)    /* OUT - length of variable, or 0 if function */
{

    unsigned   interface;
    int        lldpPortNumber;
    int        result;
    lldpPortNumber = numberOfLldpInterfaces();

    oid   current[DEF_VARLEN];
    /* fill in object part of name for current (less sizeof instance part) */
    memcpy((char *)current, (char *)vp->name, DEF_VARLEN * sizeof(oid));

    /* scan at table for closest match */
    /* find "next" interface */
    for(interface = 0; interface < lldpPortNumber;
            interface++)                 /* How to find iterator value? TODO Hardcoded as lldpPortNumber */
    {
        LLDP_GetPortRemoteInformation(&lpRemInfo[interface],
                                      interface);         /* Correct API? TODO */

        current[vp->namelen - 1] = lpRemInfo[interface].dwTimeMark;
        current[vp->namelen] =
            interface+1;                                        /* lldpRemLocalPortNum == interface? TODO */
        current[vp->namelen + 1] = lpRemInfo[interface].byRemIndex+1;
        current[vp->namelen + 2] =
            0;                                            /* lldpXdot1RemProtocolIndex Hardcoded to 0!! TODO */


        result = compare(name, *length, current, (int)vp->namelen + 3);

        /* If the operation is a SET or GET and the object Ids match,
           break out of the loop */
        if(oper && (result == 0))
        {
            break;
        }

        if(!oper && (result < 0))
        {
            break;
        }
    }

    if(interface >= lldpPortNumber)
    {
        return(NULL);
    }

    memcpy((char *)name, (char *)current, (int)(vp->namelen + 3) * sizeof(oid));
    *length = (vp->namelen + 3);
    *var_len = sizeof(long);      /* default to 32 bit return */


    switch(vp->magic)                                                               /*TODO Mapping incorrect ? */
    {
        case LLDPXDOT1REMPROTOCOLID:
            /* we store 'em 0 thru n-1, snmp wants 1 thru n */
            long_return =
                0;                                                          /*TODO Return value from Stack */
            return (u_char *)&long_return;

        default:   /* return 32 bit counter from table */
            return NULL;
    }
}


/**
 * @brief Resets sysContact, sysLocation, sysName
 *
 * @retval negative value on error, zero on success
 */
int8_t SNMP_resetField()
{
    int8_t retVal = -1;
    uint32_t blockNum, pageNum;
    uint32_t ioMode;
    ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
    /*strcpy(sysName, "");*/
    memset(sysName, 0, sizeof(sysName));
    /*strcpy(sysLocation, "");*/
    memset(sysLocation, 0, sizeof(sysLocation));
    /*strcpy(sysContact, "");*/
    memset(sysContact, 0, sizeof(sysContact));
    /*strcpy(permanentSystemData->permanentSysName, "");*/
    memset(permanentSystemData->permanentSysName, 0,
           sizeof(permanentSystemData->permanentSysName));
    /*strcpy(permanentSystemData->permanentSysLocation, "");*/
    memset(permanentSystemData->permanentSysLocation, 0,
           sizeof(permanentSystemData->permanentSysLocation));
    /*strcpy(permanentSystemData->permanentSysContact, "");*/
    memset(permanentSystemData->permanentSysContact, 0,
           sizeof(permanentSystemData->permanentSysContact));
    permanentSystemData->isValid = 1;

    Board_flashOffsetToBlkPage(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk(flashHandle, blockNum);
    retVal = Board_flashWrite(flashHandle, SPI_EEPROM_SNMP_SYS_OFFSET,
                              (uint8_t *)permanentSystemData, sizeof(permanentSysData_t), (void *)(&ioMode));
    return retVal;
}

/* Following is a list of traps that can be sent.
   NOTIFICATION-TYPE macro in RFCs enlist the OBJECTs
   to be sent in a specific trap. So when a specific
   trap is to be sent, SNMP will pick varbinds from
   these structures.
*/
struct trapVar nt_lldpRemTablesChange[4] =
{
    {{1, 0, 8802, 1, 1, 2, 1, 2, 2, 0xFF},   10, 0, 0, NULL, 0 },
    {{1, 0, 8802, 1, 1, 2, 1, 2, 3, 0xFF},   10, 0, 0, NULL, 0 },
    {{1, 0, 8802, 1, 1, 2, 1, 2, 4, 0xFF},   10, 0, 0, NULL, 0 },
    {{1, 0, 8802, 1, 1, 2, 1, 2, 5, 0xFF},   10, 0, 0, NULL, 0 },
};

/* Following is a List of new TEXTUAL CONVENTIONS defined
   in the MIBs
*/
struct enumList en_LldpChassisIdSubtype[] =
{
    { "chassisComponent", 1 },
    { "interfaceAlias", 2 },
    { "portComponent", 3 },
    { "macAddress", 4 },
    { "networkAddress", 5 },
    { "interfaceName", 6 },
    { "local", 7 },
    { NULL, 0 }
};

struct enumList en_LldpPortIdSubtype[] =
{
    { "interfaceAlias", 1 },
    { "portComponent", 2 },
    { "macAddress", 3 },
    { "networkAddress", 4 },
    { "interfaceName", 5 },
    { "agentCircuitId", 6 },
    { "local", 7 },
    { NULL, 0 }
};

struct enumList en_LldpManAddrIfSubtype[] =
{
    { "unknown", 1 },
    { "ifIndex", 2 },
    { "systemPortNumber", 3 },
    { NULL, 0 }
};

struct enumList en_LldpSystemCapabilitiesMap[] =
{
    { "other", 0 },
    { "repeater", 1 },
    { "bridge", 2 },
    { "wlanAccessPoint", 3 },
    { "router", 4 },
    { "telephone", 5 },
    { "docsisCableDevice", 6 },
    { "stationOnly", 7 },
    { NULL, 0 }
};



