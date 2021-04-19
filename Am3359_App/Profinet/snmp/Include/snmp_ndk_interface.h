/**
 * snmp_ndk_interface.h
 *
 *
 * Copyright (c) 2012 Texas Instruments Incorporated ALL RIGHTS RESERVED
 *
*/

#ifndef SNMP_NDK_INTERFACE_H_
#define SNMP_NDK_INTERFACE_H_
#include "stdio.h"
#include "stdint.h"
#include <ti/ndk/inc/stkmain.h>
#ifdef C66X
#include <third_party/protocols/snmp/include/snmpagt.h>
#endif
#include <ti/transport/ndk/nimu_icss/nimu_icssIoctl.h>


/*
 * Number of Interfaces
 */
#define SNMP_NUMBER_OF_PORTS        3
#define NIMU_MAC_INDEX      1
#define SUPPORTED_SERVICES  78



/**
 * \brief   Gets number of Interfaces
 *
 * \param   None
 *
 * \return  Number of Interfaces
 **/
uint16_t numberOfInterfaces();

/**
 * \brief   Gets Statistics from CPSW_Stats, uses NIMUIoctl
 *
 * \param   statsCommand  Statistics Updated to this structure
 *          ifEntryObj    interface MIB structure, contains index
 *          port_no       Interface number
 *
 * \return  None
 **/
void getNimuStats(NIMU_IcssStatsCmd *statsCommand, int port_no);

/**
 * \brief   Gets ifDescr value
 *
 * \param   interface Interface number
 *
 * \return  ifDescr
 **/
char *getIfDescr(int interface);

/**
 * \brief   Gets IfType value
 *
 * \param   None
 *
 * \return  IfType value
 **/
int getIfType();

/**
 * \brief   Gets ifMtu value
 *
 * \param   None
 *
 * \return  MTU value
 **/
uint16_t getIfMtu();

/**
 * \brief   Gets ifSpeed value
 *
 * \param   interface interface number
 *
 * \return  Phy speed
 **/
uint32_t getIfSpeed(int interface);

/**
 * \brief   Gets ifPhyAddress value
 *
 * \param   interface interface number
 * \param   macAddr Return pointer
 *
 * \return  IfSpeed
 **/
void getIfPhyAddress(int interface, uint8_t *macAddr);

/**
 * \brief   Updates ifAdminStatus, ifOperStatus and ifLastChange values
 *          ifAdminStatus currently supports only 1(up)
 *
 * \param   ifAdminStatus     OUT/IN pointer to Admin Status
 *          ifOperStatus      OUT pointer to Oper Status
 *          ifLastChange      OUT pointer to last change time
 *          port_no           Interface number
 *
 * \return  None
 **/
void getIfStatus(long *ifAdminStatus, long *ifOperStatus, u_long *ifLastChange,
                 int port_no);

/**
 * \brief   Gets ifInOctets value
 *
 * \param   port_no       Interface number
 *
 * \return  None
 **/
u_long getIfInOctets(int port_no);

/**
 * \brief   Gets ifInUcastPkts value
 *
 * \param   port_no       Interface number
 *
 * \return  IfInUcastPkts
 **/
u_long getIfInUcastPkts(int port_no);

/**
 * \brief   Gets ifInNUcastPkts value
 *
 * \param   port_no       Interface number
 *
 * \return  IfInNUcastPkts
 **/
u_long getIfInNUcastPkts(int port_no);

/**
 * \brief   Gets ifInDiscards value
 *
 * \param   port_no       Interface number
 *
 * \return  IfInDiscards
 **/
u_long getIfInDiscards(int port_no);

/**
 * \brief   Gets ifInErrors value
 *
 * \param   port_no       Interface number
 *
 * \return  IfInErrors
 **/
u_long getIfInErrors(int port_no);

/**
 * \brief   Gets ifInUnkownProtos value
 *
 * \param   port_no       Interface number
 *
 * \return  IfInUnkownProtos
 **/
u_long getIfInUnkownProtos(int port_no);

/**
 * \brief   Gets ifOutOctets value
 *
 * \param   port_no       Interface number
 *
 * \return  IfOutOctets
 **/
u_long getIfOutOctets(int port_no);

/**
 * \brief   Gets ifOutUcastPkts value
 *
 * \param   port_no       Interface number
 *
 * \return  IfOutUcastPkts
 **/
u_long getIfOutUcastPkts(int port_no);

/**
 * \brief   Gets ifOutNUcastPkts value
 *
 * \param   port_no       Interface number
 *
 * \return  IfOutNUcastPkts
 **/
u_long getIfOutNUcastPkts(int port_no);

/**
 * \brief   Gets ifOutDiscards value
 *
 * \param   port_no       Interface number
 *
 * \return  IfOutDiscards
 **/
u_long getIfOutDiscards(int port_no);

/**
 * \brief   Gets ifOutErrors value
 *
 * \param   port_no       Interface number
 *
 * \return  IfOutErrors
 **/
u_long getIfOutErrors(int port_no);

#endif /* SNMP_NDK_INTERFACE_H_ */
