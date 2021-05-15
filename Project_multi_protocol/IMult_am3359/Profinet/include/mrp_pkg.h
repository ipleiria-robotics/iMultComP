/***************************************************************************************
 *********** Copyright 2005 Zurich University of Applied Sciences / InES  **************
 ***************************************************************************************
 **
 **  File             : mrp_pkg.h
 **
 **  Description      : This file defines constants and needed enums
 **
 ***************************************************************************************
 **  Department       : Institute of Embedded Systems
 **  Project          : MRP
 **  Modul            : MRP Pkg
 **  Author           : Christian Schaer
 **  Contact          : scc@zhwin.ch
 **  Date             : 10.04.2006
 ****************************** Modifications ******************************************
 **
 **  Date  :    Who  :  Comment  :
 **  10.05.06   scc    initial release
 **************************************************************************************/
/* $Workfile:: mrp_pkg.h                                                    $*/
/* $Revision:: 9                                                            $*/
/* $Date:: 27/01/16 18:12                                                   $*/

#ifndef MRP_PKG_H 
#define MRP_PKG_H 


#include "mrp_datatypes.h"

#include <stdio.h>
/*#include <netinet/in.h>*/




#define TIMER_TICK_INTERVAL      1           /* Value in ms*/


/*----------------------------------------------------------------------------*/
/* mrp_pkg.h                                                                  */
/*----------------------------------------------------------------------------*/
struct MRP_EnvEvent_T;
typedef struct MRP_EnvEvent_T MRP_EnvEvent_T;

struct MRP_Environment_T;
typedef struct MRP_Environment_T MRP_Environment_T;

struct MRP_PDUFrames_T;
typedef struct MRP_PDUFrames_T MRP_PDUFrames_T;

struct MRP_ProtocolEvent_T;
typedef struct MRP_ProtocolEvent_T MRP_ProtocolEvent_T;

struct MRP_ProtocolMachine_T;
typedef struct MRP_ProtocolMachine_T MRP_ProtocolMachine_T;

struct MRP_Timer_T;
typedef struct MRP_Timer_T MRP_Timer_T;

struct MRP_FrameAnalyser_T;
typedef struct MRP_FrameAnalyser_T MRP_FrameAnalyser_T;

struct MRP_PDU_T;
typedef struct MRP_PDU_T MRP_PDU_T;

struct MRP_QueueSynchronized_T;
typedef struct MRP_QueueSynchronized_T MRP_QueueSynchronized_T;

struct MRP_Domain_T;
typedef struct MRP_Domain_T MRP_Domain_T;

struct MRP_ForwardingDataBaseItf_T;
typedef struct MRP_ForwardingDataBaseItf_T MRP_ForwardingDataBaseItf_T;

struct MRP_PortItf_T;


typedef struct DomainID DomainID;
struct DomainID
{
    Unsigned32 Data1;
    Unsigned16 Data2;
    Unsigned16 Data3;
    Unsigned8 Data4[8];
};

typedef struct DomainID ST_MRP_UUID, * LPST_MRP_UUID;

typedef enum MRPPortRole {
    MRP_PRIMARY,
    MRP_SECONDARY
} MRPPortRole;

typedef enum MRPRoleState 
{
    MRP_CLIENT,
    MRP_MANAGER
} MRPRoleState;

typedef enum MRPRingState 
{
    MRP_RING_OPEN_STATE,
    MRP_RING_CLOSED_STATE,
  MRP_RING_UNDEFINED_STATE
} MRPRingState;

typedef enum MRPPortState 
{
    MRP_DISABLED = 1,
    MRP_BLOCKED,
    MRP_FORWARDING
} MRPPortState;

typedef enum MRPLinkState 
{
    MRP_LNK_DOWN,
    MRP_LNK_UP
} MRPLinkState;

typedef enum MRPError 
{
    MRP_NO_ERROR,
  MRP_NOT_INITIALISED,
  MRP_NOT_ENABLED,
  MRP_DOMAIN_ID_MISMATCH,
  MRP_ROLE_NOT_SUPPORTED,
  MRP_INVALID_RINGPORT,
  MRP_MANAGER_ROLE_FAIL,
  MRP_CLIENT_ROLE_FAIL,
  MRP_RING_OPEN,
  MRP_MULTIPLE_MANAGERS,
  MRP_MANAGER_READ_FAIL,
  MRP_CLIENT_READ_FAIL,
  MRP_INTERNAL_ERROR
} MRPError;


#define MRP_VER            0x0001  
#define MRP_LT            0x88E3  
#define MRP_VLAN_LT          0x8100  
#define MRP_VLAN_PRIORITY      0x07  
#define MRP_FDB_PRIORITY      0xFFFF

#define MRP_END_TLV_TYPE      0x00
#define MRP_COMMON_TLV_TYPE      0x01
#define MRP_TEST_TLV_TYPE      0x02
#define MRP_TOPOLOGYCHANGE_TLV_TYPE 0x03
#define MRP_LINKDOWN_TLV_TYPE    0x04
#define MRP_LINKUP_TLV_TYPE      0x05
#define MRP_OPTION_TLV_TYPE      0x7F

#define MRP_ENV_EVENT_QUEUE_SIZE  64
#define MRP_FRAME_BUFFER_SIZE    100
#define MRP_REACT_ON_LINK_CHANGE  1
#define MRP_BLOCKED_SUPPORTED       1

//#define MRP_DOMAIN_COUNT 2
#define MRP_DOMAIN_COUNT 1 /* CM : stack does not support 2 MRP rings */

/* Helper Functions */
MRPBoolean equals(UChar var1[], UChar var2[], int n );
MRPBoolean compareDomainID(DomainID* id1, DomainID* id2);


#endif





