/*---------------------------------------------------------------------------*/
/*  Copyright (C) 2006 Woodhead Software & Electonics. All rights reserved.  */
/*---------------------------------------------------------------------------*/
/*  This program is protected by international copyright.                    */
/*                                                                           */
/*  The use of this software including but not limited to its Source Code    */
/*  is subject to restrictions as agreed in the license agreement between    */
/*  you and Woodhead.                                                        */
/*  Copying or distribution is not allowed unless expressly permitted        */
/*  according to your license agreement with Woodhead.                       */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Project             : STACK PROFINET Controller                          */
/*  Component           : PDU of PNIO acyclic message                        */
/* $Workfile:: pnio_pdu.h                                                   $*/
/* $Revision:: 44                                                           $*/
/* $Date:: 17/10/16 10:06                                                   $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/


#ifndef _PNIO_ACYCLIC_PDU_H_
#define _PNIO_ACYCLIC_PDU_H_

#include "uuid_pn.h"

#define NAMELEN       240
#define ALIASLEN      (NAMELEN+ 15 + 1)
#define NAMEOFPORTLEN (OS_STRLEN("port-000"))

#define MIB_STRING_LEN     256

#define IM_Tag_Function_LEN 32
#define IM_Tag_Location_LEN 22
#define IM_Date_LEN         16
#define IM_Descriptor_LEN   54
#define IM_Signature_LEN    54

/* APPSTATUS define    5.2.12.76*/
/* ErrorCode*/
#define STS_APP             0x81
#define STS_RTAERROR        0xCF
#define STS_ALARMACK        0xDA
#define STS_IODCONNECTRES   0xDB
#define STS_IODRELEASERES   0xDC
#define STS_IOXCONNECTRES   0xDD
#define STS_IODREADRES      0xDE
#define STS_IODWRITERES     0xDF

/* ErrorDecode*/
#define STS_ED_APPRW     0x80
#define STS_ED_APP       0x81

/* Block Type ---->To be Continued 6.2.1.2 v2.1*/
#define BT_ALARMNOTIFICATIONHIGH      0x001
#define BT_ALARMNOTIFICATIONLOW       0x002
#define BT_WRITERECORDREQ             0x008
#define BT_READRECORDREQ              0x009
#define BT_DIAGNOSISDATA              0x010
#define BT_MULTICASTCONSUINFO         0x011
#define BT_EXPECTEDIDENTIFICATION     0x012
#define BT_READIDENTIFICATION         0x013
#define BT_SUBSTITUTEVALUE            0x014
#define BT_LOGDATA                    0x019
#define BT_ARBLOCKREQ                 0x101
#define BT_IOCRBLOCKREQ               0x102
#define BT_ALARMCRBLOCKREQ            0x103
#define BT_EXPECTEDSUBMODULEBLOCKREQ  0x104
#define BT_PRMSERVERBLOCKREQ          0x105
#define BT_MCRBLOCKREQ                0x106
#define BT_IRINFOBLOCKREQ             0x109
#define BT_SRINFOBLOCKREQ             0x10A
#define BT_ARFSUBLOCK                 0x10B
#define BT_IODCONTROLBLOCKREQ         0x110
#define BT_DCONTROLBLOCKPLUG          0x111
#define BT_IOXCONTROLBLOCKREQ         0x112
#define BT_CCONTROLBLOCKPLUG          0x113
#define BT_RELEASEBLOCKREQ            0x114
#define BT_ARRPCBLOCKREQ              0x115
#define BT_IOXCONTROLBLOCKRTC3REQ     0x117
#define BT_DCONTROL_PRM_BEGIN         0x118
#define BT_SUBMODULE_LIST_BLOCK       0x119
#define BT_EXPECTEDSUBMODULEBLOCKRES2 0x200
#define BT_IODREADREQ                 0x0500
#define BT_ALARMACKHIGH               0x8001
#define BT_ALARMACKLOW                0x8002
#define BT_ARBLOCKRES                 0x8101
#define BT_IOCRBLOCKRES               0x8102
#define BT_ALARMCRBLOCKRES            0x8103
#define BT_EXPECTEDSUBMODULEBLOCKRES  0x8104
#define BT_PRMSERVERBLOCKRES          0x8105
#define BT_ARSERVERBLOCKRES           0x8106
#define BT_IODCONTROLBLOCKRES         0x8110
#define BT_CONTROLBLOCKPLUGRES        0x8111
#define BT_IOXCONTROLBLOCKRES         0x8112
#define BT_CCONTROLBLOCKPLUGRES       0x8113
#define BT_RELEASEBLOCKRES            0x8114
#define BT_ARRPCBLOCKRES              0x8115
#define BT_IOXCONTROLBLOCKRTC3RES     0x8117
#define BT_DCONTROL_PRMBEGIN          0x8118
#define BT_WRITERECORDRES             0x8008
#define BT_READRECORDRES              0x8009
#define BT_RESPONSEMASK               0x8000

/* Values for ChannelErrorType: see 6.2.7.2 Coding of the field ChannelErrorType in specification 2.3              */
/* TODO: this log shall be study to understand when I can use it*/
#define  CET_RSERVED                               0x0000  /*  Reserved                                            Unknown error                                                                                                                    */
#define  CET_SHORT_CIRCUIT                         0x0001  /*  Short circuit                                       Short circuit                                                                                                                */
#define  CET_UNDERVOLTAGE                          0x0002  /*  Undervoltage                                        Undervoltage                                                                                                                */
#define  CET_OPVERVOLTAGE                          0x0003  /*  Overvoltage                                         Overvoltage                                                                                                                    */
#define  CET_OVERLOAD                              0x0004  /*  Overload                                            Overload                                                                                                                        */
#define  CET_OVERTEMPERATURE                       0x0005  /*  Overtemperature                                     Overtemperature                                                                                                            */
#define  CET_LINE_BREAK                            0x0006  /*  Line break                                          Line break                                                                                                                     */
#define  CET_UPPER_LIMIT_VALUE_EXCEEDED            0x0007  /*  Upper limit value exceeded                          Upper limit value exceeded                                                                                    */
#define  CET_LOWER_LIMIT_VALUE_EXCEEDED            0x0008  /*  Lower limit value exceeded                          Lower limit value exceeded                                                                                    */
#define  CET_ERROR                                 0x0009  /*  Error                                               Error                                                                                                                                */
#define  CET_SIMULATION_ACTIVE                     0x000A  /*  Simulation active                                   Simulation active                                                                                                        */
#define  CET_UNKNOW_ERROR_B                        0x000B  /*  Reserved                                            Unknown error                                                                                                                   */
#define  CET_UNKNOW_ERROR_C                        0x000C  /*  Reserved                                            Unknown error                                                                                                                   */
#define  CET_UNKNOW_ERROR_D                        0x000D  /*  Reserved                                            Unknown error                                                                                                                   */
#define  CET_UNKNOW_ERROR_E                        0x000E  /*  Reserved                                            Unknown error                                                                                                                   */
#define  CET_MS_PARAMETER_MISSING                  0x000F  /*  Manufacturer specific                               Recommended for “parameter missing”  The channel needs (additional) parameters. No or to less parameters are written  */
#define  CET_MS_PARAMETER_FAULT                    0x0010  /*  Manufacturer specific                               Recommended for “parameterization fault”   Parameterization fault. Wrong or to many parameters are written            */
#define  CET_MS_POWER_SUPPLY_FAULT                 0x0011  /*  Manufacturer specific                               Recommended for “power supply fault”   Power supply fault                                                            */
#define  CET_MS_FUSE_BLOWN_OPEN                    0x0012  /*  Manufacturer specific                               Recommended for “fuse blown / open”   Fuse blown / open                                                              */
#define  CET_MS_COMMUNICATION_FAULT                0x0013  /*  Manufacturer specific                               Recommended for “communication fault”  Communication fault. Sequence number wrong / sequence wrong                   */
#define  CET_MS_GROUND_FAULT                       0x0014  /*  Manufacturer specific                               Recommended for “ground fault”   Ground fault                                                                        */
#define  CET_MS_REFERENCE_POINT_LOST               0x0015  /*  Manufacturer specific                               Recommended for “reference point lost”   Reference point lost                                                        */
#define  CET_MS_ERROR                              0x0016  /*  Manufacturer specific                               Recommended for “process event lost / sampling error”  Process event lost / sampling error                            */
#define  CET_MS_THRESHOLD_WARNING                  0x0017  /*  Manufacturer specific                               Recommended for “threshold warning”  Threshold warning                                                                */
#define  CET_MS_OUTPUT_DISABLED                    0x0018  /*  Manufacturer specific                               Recommended for “output disabled”  Output disabled                                                                   */
#define  CET_MS_SAFETY_EVENT                       0x0019  /*  Manufacturer specific                               Recommended for “safety event”  Safety event                                                                        */
#define  CET_EXTERNEL_FAULT                        0x001A  /*  Manufacturer specific                               Recommended for “external fault”  External fault                                                                    */
#define  CET_MS_1B                                 0x001B  /*  Manufacturer specific                               Manufacturer specific                                                                                                */
#define  CET_MS_1C                                 0x001C  /*  Manufacturer specific                               Manufacturer specific                                                                                                */
#define  CET_MS_1D                                 0x001D  /*  Manufacturer specific                               Manufacturer specific                                                                                                */
#define  CET_MS_1E                                 0x001E  /*  Manufacturer specific                               Manufacturer specific                                                                                                */
#define  CET_MS_TEMPORARY_FAULT                    0x001F  /*  Manufacturer specific                               Recommended for “temporary fault”  Temporary fault                                                                 */
#define  CET_DATA_TRANSMISSION_IMPOSSIBLE          0x8000  /*  Data transmission impossible                        Data Transmission Impossible                                                                                */
#define  CET_REMOTE_MISMATCH                       0x8001  /*  Remote mismatch                                     Remote Mismatch                                                                                                            */
#define  CET_MEDIA_REDUNDANCY_MISMATCH             0x8002  /*  Media redundancy mismatch                           Media Redundancy Mismatch                                                                                        */
#define  CET_SYNC_MISMATCH                         0x8003  /*  Sync mismatch                                       Sync Mismatch                                                                                                                */
#define  CET_ISOCHONOUS_MISMACH                    0x8004  /*  IsochronousMode mismatch                            IsochronousMode Mismatch                                                                                        */
#define  CET_MULTICAST_CR_MISMATCH                 0x8005  /*  Multicast CR mismatch                               Multicast CR Mismatch                                                                                                */
#define  CET_RESERVED8006                          0x8006  /*  Reserved                                            Reserved                                                                                                                        */
#define  CET_FIBER_OPTIC_MISMATCH                  0x8007  /*  Fiber optic mismatch                                Information for fiber optic link.                                                                                    */
#define  CET_NETWORK_COMPONENET_FUNCTION_MISMATCH  0x8008  /*  Network component function mismatch                 Network functionality problems occur                                                                  */
#define  CET_TIME_MISMATCH                         0x8009  /*  Time mismatch                                       Time master not existent or precision problems                                                                              */
#define  CET_DYN_FRAME_PACK_MISMATCH               0x800A  /*  Dynamic frame packing function mismatch             DFP problems occur                                                                                 */
#define  CET_MRPD_PROBLEM                          0x800B  /*  Media redundancy with planned duplication mismatch  MRPD problems occur                                                                    */
#define  CET_SR_MISMATCH                           0x800C  /*  System redundancy mismatch                          System Redundancy Mismatch                                                                                    */

/*ExtChannelErrorType: Definition is depending on the ChannelErrorType (see tables below).*/
/*Only a little part of the ExtChannelErrorType is here. Add the other one         */
#define  CET_EXT_MCRM_MULTICAST_CONSUMER_CR_TIMEOUT 0x8000  /* Multicast Consumer CR timed out                                                                                                            */
#define  CET_EXT_MCRM_ADDRESS_RESOLUTION_FAILED     0x8001  /* Address resolution failed  Alarm/diagnosis                                                                                                 */

/*********************************************** INTERFACE Data 4 */
#define IF_UUIDDATA4    {0x82, 0x71, 0x00,0xA0, 0x24,0x42,0xDF,0x7D}

/*********************************************** ACTIVITY UUI Definition*/
/* Data1 : DWORD in little endian*/
#define ACT_UUIDDATA1   0x00000003
/* Data 2*/
#define ACT_UUIDDATA2   0x0000
/* Data 3*/
#define ACT_UUIDDATA3   0x1010
/* Data 4*/
#define ACT_UUIDDATA4  {0x81,0x9A, 0x08,0x00,0x06,0x6B,0x92,0xEC}

/* Block version*/
#define BLOCKVERSIONHIGH  1
#define BLOCKVERSIONLOW   0

/* IOUser Priority */
/* 6 --> 110 */
/* mask is 1110 0000 0000 0000*/
/* then valu must be 1100 0000 0000 0000 */
#define IO_CR_PRIORITY 49152L

/* AR TYPE     5.2.12.3*/
#define IOCRARSINGLE     0x01
#define IOCRARSAR        0x06
#define IOCARSR          0x20
#define IOCRARSINGLERTC3 0x10

/*IOCR TYPE  */
#define IOCR_INPUT_CR       1
#define IOCR_OUTPUT_CR      2
#define IOCR_MULTICAST_PROV 3
#define IOCR_MULTICAST_CONS 4

/* Alarm CR Type*/
#define IOCR_ALARM_CR   1

/* Alarm Frame ID */
#define FRAMEID_ALM_LOW         0xFE01
#define FRAMEID_ALM_HIGH        0xFC01

#define MIN_ALARM_LEN 200
#define MAX_ALARM_LEN 1432

/* FRAME ID Don't care*/
#define FRAMEID_OUTPUT_CR 0xFFFF

/* PNIO Status Constants*/
/* ErrorCode spec v2.1 table 276*/
#define ERR_PNIO            0x81
#define ERR_RTA             0xCF
#define ERR_ALARM_ACK       0XDA
#define ERR_IODCONNECT_RES  0XDB
#define ERR_IODRELEASE_RES  0XDC
#define ERR_IOXCONTROLRES   0XDD
#define ERR_IODREADRES      0XDE
#define ERR_IODWRITERES     0XDF

/* DCP State machine error class */
#define ERR_CTXT                          1
#define ERR_CPROTOCOL                     2
#define ERR_CINTERNAL                   255

/* DCP State machine error codes */
#define ERR_INVALID_STATE                 0
#define ERR_TIME_OUT                      1

#define UDPRTPORT_MAX         0x03FF    /* UDP RT port shall be greater than 0x03FF*/

#define CONTROL_COMMAND_PRM_END              0x0001
#define CONTROL_COMMAND_APPLICATION_READY    0x0002
#define CONTROL_COMMAND_RELEASE              0x0004
#define CONTROL_COMMAND_DONE                 0x0008
#define CONTROL_COMMAND_READY_FOR_COMPANION  0x0010
#define CONTROL_COMMAND_READY_FOR_RTCCLASS3  0x0020
#define CONTROL_COMMAND_PRMBEGIN             0x0040

#define CONTROL_PROPERTIES_READY_FOR_RTCCLASS3 0x0002

/* Module State definition */
#define MODULE_STATE_NO          0
#define MODULE_STATE_WRONG       1
#define MODULE_STATE_PROPER      2
#define MODULE_STATE_SUBSTITUTE  3

/*AlarmCrBlockReq 0x0001*/
#define ALARM_CR_TYPE 0x0001

/* SubModule State definition */
#define SUBMODULE_STATE_FORMAT_INDICATOR  0x8000
#define SUBMODULE_STATE_FORMAT_IDENTINFO  0x7000
#define SUBMODULE_STATE_FORMAT_ARINFO     0x00780
#define SUBMODULE_STATE_DETAIL            !SUBMODULE_STATE_FORMAT_INDICATOR

#define SUBMODULE_STATE_FORMAT_IDENTINFO_OK          0
#define SUBMODULE_STATE_FORMAT_IDENTINFO_SUBSTITUTE  1
#define SUBMODULE_STATE_FORMAT_IDENTINFO_WRONG       2
#define SUBMODULE_STATE_FORMAT_IDENTINFO_NOSUBMODULE 3

#define SUBMODULE_STATE_FORMAT_ARINFO_OWN 0
#define SUBMODULE_STATE_FORMAT_ARINFO_ARP 1
#define SUBMODULE_STATE_FORMAT_ARINFO_SO  2
#define SUBMODULE_STATE_FORMAT_ARINFO_IOC 3
#define SUBMODULE_STATE_FORMAT_ARINFO_IOS 4
#define SUBMODULE_STATE_FORMAT_NOT_CONNECTED 0xffff /* Internal status*/

#define SUBMODULE_STATE_DETAIL_NOSUBMODULE      0
#define SUBMODULE_STATE_DETAIL_WRONGSUBMODULE   1
#define SUBMODULE_STATE_DETAIL_IOCLOCK          2
#define SUBMODULE_STATE_DETAIL_ARR              4
#define SUBMODULE_STATE_DETAIL_SUBSTITUTE       7

#define GET_SUBMODULE_STATE_IDENTINFO(a)  (a&SUBMODULE_STATE_FORMAT_IDENTINFO)>>11
#define GET_SUBMODULE_STATE_ARINFO(a)     (a&SUBMODULE_STATE_FORMAT_ARINFO)>>7
#define GET_SUBMODULE_STATE_DETAIL(a)     (a&SUBMODULE_STATE_DETAIL)

/* INDEX */

/* User specific */
#define INDEX_USER_0                                      0x0000
#define INDEX_USER_07FF                                   0x07FF

/* SubModule */
#define INDEX_SUBSLOT_EXPECTEDIDENTIFICATIONDATA          0x8000
#define INDEX_SUBSLOT_REALIDENTIFICATIONDATA              0x8001
#define INDEX_SUBSLOT_DIAGINCHANNELCODING                 0x800A
#define INDEX_SUBSLOT_DIAGINALLCODING                     0x800B
#define INDEX_SUBSLOT_DIAGMAINTENANCE                     0x800C
#define INDEX_SUBSLOT_MAINTENANCEREQUIREDCHANNELCODING    0x8010
#define INDEX_SUBSLOT_MAINTENANCEDEMANDEDCHANNELCODING    0x8011
#define INDEX_SUBSLOT_MAINTENANCEREQUIREDALLCODING        0x8012
#define INDEX_SUBSLOT_MAINTENANCEDEMANDEDALLCODING        0x8013
#define INDEX_SUBSLOT_SUBSTITUTE                          0x801E
#define INDEX_SUBSLOT_PORTDATAREALEXT                     0x8027
#define INDEX_SUBSLOT_RECORDINPUTDATA                     0x8028
#define INDEX_SUBSLOT_RECORDOUTPUTDATA                    0x8029
#define INDEX_SUBSLOT_PORTDATAREAL                        0x802A
#define INDEX_SUBSLOT_PORTDATACHECK                       0x802B
#define INDEX_SUBSLOT_PDIRDATA                            0x802C
#define INDEX_SUBSLOT_PDSYNCDATAOVERRTA                   0x802D
#define INDEX_SUBSLOT_PDSYNCDATAOVERRTC                   0x802E
#define INDEX_SUBSLOT_PDPORTDATAADJUST                    0x802F
#define INDEX_SUBSLOT_ISOCHRONOUSMODE                     0x8030
#define INDEX_SUBSLOT_PDSYNCDATA1                         0x8031
#define INDEX_SUBSLOT_PDSYNCDATA2                         0x8032
#define INDEX_SUBSLOT_PDSYNCDATA3                         0x8033
#define INDEX_SUBSLOT_PDSYNCDATA4                         0x8034
#define INDEX_SUBSLOT_PDSYNCDATA5                         0x8035
#define INDEX_SUBSLOT_PDSYNCDATA6                         0x8036
#define INDEX_SUBSLOT_PDSYNCDATA7                         0x8037
#define INDEX_SUBSLOT_PDSYNCDATA8                         0x8038
#define INDEX_SUBSLOT_PDSYNCDATA9                         0x8039
#define INDEX_SUBSLOT_PDSYNCDATA10                        0x803A
#define INDEX_SUBSLOT_PDSYNCDATA11                        0x803B
#define INDEX_SUBSLOT_PDSYNCDATA12                        0x803C
#define INDEX_SUBSLOT_PDSYNCDATA13                        0x803D
#define INDEX_SUBSLOT_PDSYNCDATA14                        0x803E
#define INDEX_SUBSLOT_PDSYNCDATA15                        0x803F
#define INDEX_SUBSLOT_PDSYNCDATA16                        0x8040
#define INDEX_SUBSLOT_PDSYNCDATA17                        0x8041
#define INDEX_SUBSLOT_PDSYNCDATA18                        0x8042
#define INDEX_SUBSLOT_PDSYNCDATA19                        0x8043
#define INDEX_SUBSLOT_PDSYNCDATA20                        0x8044
#define INDEX_SUBSLOT_PDSYNCDATA21                        0x8045
#define INDEX_SUBSLOT_PDSYNCDATA22                        0x8046
#define INDEX_SUBSLOT_PDSYNCDATA23                        0x8047
#define INDEX_SUBSLOT_PDSYNCDATA24                        0x8048
#define INDEX_SUBSLOT_PDSYNCDATA25                        0x8049
#define INDEX_SUBSLOT_PDSYNCDATA26                        0x804A
#define INDEX_SUBSLOT_PDSYNCDATA27                        0x804B
#define INDEX_SUBSLOT_PDSYNCDATA28                        0x804C
#define INDEX_SUBSLOT_PDSYNCDATA29                        0x804D
#define INDEX_SUBSLOT_PDSYNCDATA30                        0x804E
#define INDEX_SUBSLOT_PDSYNCDATA31                        0x804F
#define INDEX_SUBSLOT_PDIFMRPDATAREAL                     0x8050
#define INDEX_SUBSLOT_PDIFMRPDATACHECK                    0x8051
#define INDEX_SUBSLOT_PDIFMRPDATAADJUST                   0x8052
#define INDEX_SUBSLOT_PDPORTMRPDATAADJUST                 0x8053
#define INDEX_SUBSLOT_PDPORTMRPDATAREAL                   0x8054
#define INDEX_SUBSLOT_PORTFOMRPDATAREAL                   0x8060
#define INDEX_SUBSLOT_PORTFODATACHECK                     0x8061
#define INDEX_SUBSLOT_PORTFODATAADJUST                    0x8062
#define INDEX_SUBSLOT_PDNCDATACHECK                       0x8070
#define INDEX_SUBSLOT_PDINTERFACEADJUST                   0x8071
#define INDEX_SUBSLOT_PDPORTSTATISTIC                     0x8072
#define INDEX_SUBSLOT_PDITFDATA_REAL                      0x8080
#define INDEX_SUBSLOT_PDIFFSUADJUST                       0x8090
#define INDEX_PESAP                                       0x80A0
#define INDEX_SUBSLOT_IM0                                 0xAFF0
#define INDEX_SUBSLOT_IM1                                 0xAFF1
#define INDEX_SUBSLOT_IM2                                 0xAFF2
#define INDEX_SUBSLOT_IM3                                 0xAFF3
#define INDEX_SUBSLOT_IM4                                 0xAFF4
#define INDEX_SUBSLOT_IM5                                 0xAFF5
#define INDEX_SUBSLOT_IM6                                 0xAFF6
#define INDEX_SUBSLOT_IM7                                 0xAFF7
#define INDEX_SUBSLOT_IM8                                 0xAFF8
#define INDEX_SUBSLOT_IM9                                 0xAFF9
#define INDEX_SUBSLOT_IM10                                0xAFFA
#define INDEX_SUBSLOT_IM11                                0xAFFB
#define INDEX_SUBSLOT_IM12                                0xAFFC
#define INDEX_SUBSLOT_IM13                                0xAFFD
#define INDEX_SUBSLOT_IM14                                0xAFFE
#define INDEX_SUBSLOT_IM15                                0xAFFF

/* Slot specific */
#define INDEX_SLOTEXPECTEDIDENTIFICATION                  0xC000
#define INDEX_SLOTREALIDENTIFICATION                      0xC001
#define INDEX_SLOTCHANNELDIAGNOSIS                        0xC00A
#define INDEX_SLOTALLDIAGNOSIS                            0xC00B
#define INDEX_SLOTDIAGNOSISMAINTENANCE                    0xC00C
#define INDEX_SLOTMAINTENANCEREQCHANNEL                   0xC010
#define INDEX_SLOTMAINTENANCEDEMCHANNEL                   0xC011
#define INDEX_SLOTMAINTENANCEREQALL                       0xC012
#define INDEX_SLOTMAINTENANCEDEMALL                       0xC013
#define INDEX_SLOT_0XC02A                                 0xC02A
#define INDEX_SLOT_0XC02B                                 0xC02B
#define INDEX_SLOT_0XC02C                                 0xC02C
#define INDEX_SLOT_0XC02D                                 0xC02D
#define INDEX_SLOT_0XC02E                                 0xC02E
#define INDEX_SLOT_0XC02F                                 0xC02F

/* AR Specific */
#define INDEX_AREXPECTEDIDENTIFICATION                    0xE000
#define INDEX_ARREALIDENTIFICATION                        0xE001
#define INDEX_ARMODDIF                                    0xE002
#define INDEX_ARCHANNELDIAGNOSIS                          0xE00A
#define INDEX_ARALLDIAGNOSIS                              0xE00B
#define INDEX_ARDIAGNOSISMAINTENANCE                      0xE00C
#define INDEX_ARMAINTENANCEREQCHANNEL                     0xE010
#define INDEX_ARMAINTENANCEDEMCHANNEL                     0xE011
#define INDEX_ARMAINTENANCEREQALL                         0xE012
#define INDEX_ARMAINTENANCEDEMALL                         0xE013
#define INDEX_AR_0XE02A                                   0xE02A
#define INDEX_AR_0XE02B                                   0xE02B
#define INDEX_AR_0XE02C                                   0xE02C
#define INDEX_AR_0XE02D                                   0xE02D
#define INDEX_AR_0XE02E                                   0xE02E
#define INDEX_AR_0XE02F                                   0xE02F
#define INDEX_ARISOCHRONOUS                               0xE030
#define INDEX_ARMULTIPLE                                  0xE040
#define INDEX_ARFSU                                       0xE050

/* API specific */
#define INDEX_APIREALIDENTIFICATION                       0xF000
#define INDEX_APICHANNELDIAGNOSIS                         0xF00A
#define INDEX_APIALLDIAGNOSIS                             0xF00B
#define INDEX_APIDIAGNOSISMAINTENANCE                     0xF00C
#define INDEX_APIMAINTENANCEREQCHANNEL                    0xF010
#define INDEX_APIMAINTENANCEDEMCHANNEL                    0xF011
#define INDEX_APIMAINTENANCEREQALL                        0xF012
#define INDEX_APIMAINTENANCEDEMALL                        0xF013
#define INDEX_APIARDATA                                   0xF020

/* device specific */
#define INDEX_DEVICE_MAINTENANCEDIAG                      0xF80C
#define INDEX_DEVICE_ARDATA                               0xF820
#define INDEX_DEVICE_APIDATA                              0xF821
#define INDEX_DEVICE_LOGDATA                              0xF830
#define INDEX_DEVICE_PDEVDATA                             0xF831
#define INDEX_DEVICE_IMFILTERDATA                         0xF840
#define INDEX_DEVICE_PDREALDATA                           0xF841
#define INDEX_DEVICE_PDEXPECTEDDATA                       0xF842
#define INDEX_AUTO_CONFIGURATION                          0xF850
#define INDEX_TRIGGER                                     0xFBFF

/* internal management record */
#define MDIFF_BLOC_TYPE      0x8104

#define RECORD_WSE_DEBUG_INFO 0x7fff    /* WSE specific debug record ((see Table 226 — Index (user specific)) */

/* record data response blockType */
#define DIAGNOSIS_DATA                                    0x0010
#define EI_BLOC_TYPE                                      0x0012
#define RI_BLOC_TYPE                                      0x0013
#define RECORD_SUBSTITUE_VALUE_ELEMENT_TYPE               0x0014
#define RECORD_INPUT_DATA_OBJECT_ELEMENT_TYPE             0x0015
#define RECORD_OUTPUT_DATA_OBJECT_ELEMENT_TYPE            0x0016
#define ARDATA_BLOC_TYPE                                  0x0018
#define LOGATA_BLOC_TYPE                                  0x0019
#define IM0_BLOC_TYPE                                     0x0020
#define IM1_BLOC_TYPE                                     0x0021
#define IM2_BLOC_TYPE                                     0x0022
#define IM3_BLOC_TYPE                                     0x0023
#define IM4_BLOC_TYPE                                     0x0024
#define APIDATA_BLOC_TYPE                                 0x001A
#define IM0_FILTERDATA_SUBMODULE_BLOC_TYPE                0x0030
#define IM0_FILTERDATA_MODULE_BLOC_TYPE                   0x0031
#define IM0_FILTERDATA_DEVICE_BLOC_TYPE                   0x0032
#define AR_FSU_BLOCK_TYPE                                 0x010b
#define PDPORT_DATA_CHCK_BLOC_TYPE                        0x0200
#define PDPORT_DATA_ADJT_BLOC_TYPE                        0x0202
#define PDSYNC_DATA_BLOC_TYPE                             0X0203
#define PDIR_DATA_BLOC_TYPE                               0X0205
#define PDIR_GLOBAL_DATA_BLOC_TYPE                        0X0206
#define PDIR_FRAME_DATA_BLOC_TYPE                         0X0207
#define PDIR_BEGINEND_DATA_BLOC_TYPE                      0X0208
#define PDPORT_ADJUST_DOMAIN_BOUNDARY_BLOC_TYPE           0x0209
#define PDPORT_ADJUST_MULTICAST_BOUNDARY_BLOC_TYPE        0x0210
#define PDPORT_CHECK_PEER_BLOC_TYPE                       0x020a
#define PDPORT_CHECK_LINE_DELAY_BLOC_TYPE                 0x020b
#define PDPORT_CHECK_MAU_BLOC_TYPE                        0x020c
#define PDPORT_ADJUST_MAU_BLOC_TYPE                       0x020e
#define PDPORT_DATA_REAL_BLOC_TYPE                        0x020F
#define MRP_IDA_FOR_SUBS_BLOC_TYPE                        0x0211
#define MRP_IDC_FOR_SUBS_BLOC_TYPE                        0x0213
#define MRP_IDR_FOR_SUBS_BLOC_TYPE                        0x0212
#define MRP_PDA_FOR_SUBS_BLOC_TYPE                        0x0214
#define MRP_PDR_FOR_SUBS_BLOC_TYPE                        0x0215
#define MRP_CPARAM_BLOC_TYPE                              0x0217
#define MRP_MPARAM_BLOC_TYPE                              0x0216
#define PDPORT_ADJUST_LINK_STATE_BLOC_TYPE                0x021B
#define PDPORT_CHECK_LINK_STATE_BLOC_TYPE                 0x021C
#define PDPORT_CHECK_SYNC_DIFF_BLOC_TYPE                  0x021E
#define PDPORT_CHECK_MAUTYPE_DIFF_BLOC_TYPE               0x021F
#define PDPORT_ADJUST_PEER_TO_PEER_BOUNDARY               0x0224
#define PDPORT_ADJUST_DCP_BOUNDARY                        0x0225
#define PDPORT_ADJUST_PREAMBULE_LENGTH                    0x0226
#define PDPORT_DATA_REAL_EXTENDED_BLOC_TYPE               0x022C
#define PDTITFDATA_REAL_BLOC_TYPE                         0x0240
#define PDTITF_ADJUST_BLOC_TYPE                           0x0250
#define PDTITFPORT_STATISTIC_BLOC_TYPE                    0x0251
#define PDPORT_DATA_REALEXT_OWNPORT_BLOC_TYPE             0x0260
#define PDPORT_DATA_REALEXT_NEIGHBORS_BLOC_TYPE           0x0261
#define MULTIPLE_BLOCK_HEADER_BLOC_TYPE                   0x0400
#define FS_HELLO_BLOCK_TYPE                               0x0600
#define FS_PARAMETERS_BLOCK_TYPE                          0x0601
#define AR_PD_ITF_FSU_DATAAJUST_BLOCK_TYPE                0x0608
#define AR_FSU_DATAAJUST_BLOCK_TYPE                       0x0609
#define AUTO_CONF_DATA_HEADER_BLOC_TYPE                   0x0700
#define AUTO_CONF_COMM_DATAHEADER_BLOC_TYPE               0x0701
#define AUTO_CONF_CONF_DATAHEADER_BLOC_TYPE               0x0702
#define MAINTENANCE_ITEM_BLOCK_TYPE                       0x0f00

/* AR res Block */
#define AR_BLOC_RES_LEN             30
#define AR_BLOC_MAX_RESPONDER_PORT  0x03FF
/* AR block req len */
#define AR_REQ_BLOC_LEN           54

/* CR res Block*/
#define CR_BLOC_RES_LEN            8

/* Alarm bloc res */
#define ALARM_BLOC_RES_LEN         8

/* Prm Server res Block */
#define PRMSERVER_BLOC_RES_LEN    26

/* Read/Write bloc res */
#define READ_WRITE_BLOC_RES_LEN   60
#define READ_BLOC_RES_LEN         60

/* Control res bloc len*/
#define CTRL_BLOC_RES_LEN         28
#define CTRL_BLOC_REQ_LEN         28

/* Control res bloc len*/
#define RELEASE_BLOC_RES_LEN      28

/*SR info req bloc len*/
#define ST_SRINFO_BLOCK_REQ_LEN (ST_SRINFO_BLOCK_REQ_SIZEOF+2)

#define DATA_DESCRITION_INPUT   1
#define DATA_DESCRITION_OUTPUT  2

#define SUBMODULE_PROPERTIES_TYPE_NO_IO_DATA  0
#define SUBMODULE_PROPERTIES_TYPE_INPUT_DATA  1
#define SUBMODULE_PROPERTIES_TYPE_OUTPUT_DATA 2
#define SUBMODULE_PROPERTIES_TYPE_IO_DATA     3

/*device checking contsante */
#define AR_TIME_LOW_VALUE          0xDEA00000L
#define AR_TIME_MID_VALUE          0x6c97L
#define AR_TIME_HIGH_VERSION_VALUE 0x11D1L
#define AR_TIME_CLOCK2_VALUE2      0x82L
#define AR_TIME_CLOCK2_VALUE1      0x71L

#define AR_PROPERTIES_STATE_BACKUP  0
#define AR_PROPERTIES_STATE_PRIMARY 1

/* AR properties param server is store in Ar properties
   param server mask is 0001 0000 then the value 0 keep   0
                     the value 1 became 16
*/
#define AR_PROPERTIES_PARAM_SERVER_PRM 0
#define AR_PROPERTIES_PARAM_SERVER_CM  16 /*values is 1 but we use 16 to not have to decale bit*/

/* AR properties device accees is store in Ar properties
   param server mask is 0000 0001 0000 0000 the value 0 keep   0
                      the value 1 became 256
*/
/*Only the submodules from the ExpectedSubmoduleBlock are accessible*/
#define AR_DEVICE_ACCESS_ONLY_FROM_EXPECTED_SUBMODULE 0
/*Submodule access is controlled by IO device application*/
#define AR_DEVICE_ACCESS_CONTROLLE_BY_DEVICE 256

/* AR properties companion AR is store in Ar properties
   param server mask is 0000 0110 0000 0000 the value 3 became 1536
*/
#define AR_COMPANION_AR_RESERVED 1536

/* IR properties fast forwarding value*/
#define IR_FASTFORWARDINGMMACADD 2

/* IR properties forwarding mode */
#define IR_FORWARDINGMODE 1

/* IOCR RTClass is store in IOCR properties
   RTClass mask is 0000 1111 the value don't change
*/
#define IOCR_RT_CLASS_1_VALUE    1
#define IOCR_RT_CLASS_2_VALUE    2
#define IOCR_RT_CLASS_3_VALUE    3
#define IOCR_RT_CLASS_UDP_VALUE  4

/* LT (Length/Type)*/
#define LT_IP_UDP_RPC_SNMP_ICMP      0x0800
#define LT_ARP                      0x0806
#define LT_TAG_CONTROL_INFORMATION  0x8100
#define LT_RTC_RTA_DCP_PTCP_MRRT    0x8892
#define LT_MRP                      0x88E3
#define LT_LLDP                     0x88CC

/* No media redundant frame transfer */
#define NO_MEDIA_REDUNDANT_FRAME    0
/* media redundant frame transfer    */
#define MEDIA_REDUNDANT_FRAME       2048

/*reduction ratio */
#define REDUCTION_RATIO_32     32
#define REDUCTION_RATIO_64     64
#define REDUCTION_RATIO_256    256
#define REDUCTION_RATIO_512    512
#define REDUCTION_RATIO_8192   8192
#define REDUCTION_RATIO_16384  16384

/* WatchDog Factor */
#define WATCHDOG_FACTOR_0_0001  0x0001
#define WATCHDOG_FACTOR_0_1e00  0x1e00
#define WATCHDOG_FACTOR_0_03125 (FSO_SEND_CLOCK_FACTOR_MULTIPLCATEUR) / (1000000L)
#define WATCHDOG_FACTOR_61440   61440L
#define WATCHDOG_FACTOR_1920    1920L

/* DataHoldFactor */
#define DATA_OLD_FACTOR_0_0001  0x0001
#define DATA_OLD_FACTOR_0_1e00  0x1e00
#define DATA_OLD_FACTOR_0_03125 (FSO_SEND_CLOCK_FACTOR_MULTIPLCATEUR) / (1000000L)
#define DATA_OLD_FACTOR_61440   61440L
#define DATA_OLD_FACTOR_1920    1920

/* frame send offset constante */
#define FSO_SEND_CLOCK_FACTOR_MULTIPLCATEUR 31250L /*ns*/
#define FSO_FORMAT_MULTIPLCATEUR 1000L
#define FSO_MIN_VALUE 0x003d0900L
#define FSO_MAX_VALUE 0xfffffffeL

/*frame ID */
#define FRAME_ID_OUT_OF_RANGE 0xffff

/*Alarm constante */
#define ALARM_TRANSPORT_RTA_CLASS_1   0
#define ALARM_TRANSPORT_RTA_CLASS_UDP 2

/*Max Alarm  data length */
#define MAX_ALARM_LENGTH_MIN_VALUE    200
#define MAX_ALARM_LENGTH_MAX_VALUE   1432

/* Ar timeout factor*/
#define MAX_ARBLOCK_CM_INITIATOR_TIMEOUT_FACTOR 1000

/*Alarm User Priority */
#define ALARM_USER_PRIORITY_LOW     0xa000
#define ALARM_USER_PRIORITY_HIGTH   0xc000
#define ALARM_CR_VLAN_IX_MAX_VALUE  0xfff

/* AddressResolutionProperties.Protocol*/
/* 0x01 Multicast consumer uses DNS to resolve the multicast provider source MAC address.*/
#define ADD_RESOLUTION_PROP_PROTOCOLE_DNS 0x01

/* 0x02 Multicast consumer uses DCP to resolve the multicast provider source MAC address*/
#define ADD_RESOLUTION_PROP_PROTOCOLE_DCP 0x02

/*MCR Block Request  */
#define MCR_ADD_RESOLUTION_PROP_FACTOR_MIN_VALUE 0x0001
#define MCR_ADD_RESOLUTION_PROP_FACTOR_MAX_VALUE 0x0064
#define MCR_MCI_TIMEOUT_FACTOR_MAX_VALUR            100
#define MCR_STATION_NAME_MIN_LENGTH                   1
#define MCR_STATION_NAME_MAX_LENGTH                 240
#define MCR_BLOCK_REQ_FIELD_LENTGH                   12

/* PullModuleAlarmAllow   1000 0000     */
#define AR_BLOCK_ALARM_ALLOW_MASK                  0x80
#define STARTUP_MODE_LEGACY                        0x00
#define STARTUP_MODE_ADVANCED                      0x01

/*AP PRC Block request */
#define BT_ARRPCBLOCK_REQ_LEN                         4
#define BT_ALARMCRBLOCK_REQ_LEN                      22

/* Checking define */
#define SUB_SLOT_NUMBER_MAX_ALLOWED              0x7fff

/* parametrization server   mask:  0000 0011    */
#define SUB_MODULE_PROPERTIES_TYPE_CHECKING         0x2
#define SUB_MODULE_PROPERTIES_TYPE_MASK               3
#define SUB_MODULE_PROPERTIES_TYPE_LAG                3
#define SUB_MODULE_PROPERTIES_RESERVED_LAG            3

/* parametrization server   mask:  0000 0011    */
#define SUB_MODULE_PROPERTIES_SHARED_INPUT_CHECKING 0x4     /* 0x04 --> bit 2 set to 1*/
#define SUB_MODULE_PROPERTIES_SHARED_INPUT_MASK       4
#define AR_BLOC_MULTICAST_MAC_MASK 0x01
#define SRINFO_BLOC_PROP_RESERVED_1_MASK 0x7FFC

/*AR PrRoperties chcking */
/* we want the three first bit mask: 0000 0111 */
/* it correspond of a 7 */
#define AR_PROPERTIES_STATE_CHECKING 7

/* Check of Takeover */
#define AR_PROPERTIES_TAKEOVERALLOWED 8

/* parametrization server   mask:  0001 0000    */
#define AR_PROPERTIES_PARAM_SERVER_CHECKING 16

/* IR Properties forwarding mode mask:  0000 0001    */
#define IR_PROPERTIES_FORWARDING_MODE_CHECKING 1

/* IR Properties fast forwarding multicast mac add mask:  0000 0002    */
#define IR_PROPERTIES_FAST_FWD_MULTICASTMAC_CHECKING 2

/* device access mask 0000 0001 0000 0000  */
#define AR_DEVICE_ACCESS_CHECKING 256

/*companion AR  mask 0000 0110 0000 0000*/
#define AR_COMPANION_AR_CHECKING 1536

/* RTClass AR  mask 0000 1111     */
#define IOCRBLOCKREQ_RTCLASS_CHECKING 15

/*IOUser Priority   1110 0000 0000 0000 */
#define IOUSER_PRIORITY_CHECKING 57344L

/*AlarmCRProperties priority   10 */
#define ALARMCR_TRANSPORT_CHECKING 2

/*AlarmCRProperties priority   1111 1111 1111 1111 1111 1111 1111 1100 */
#define ALARMCR_RESERVED_CHECKING 0xfffffffcL

/* IOCrBlockReq IOCRProperties.reserved1 mask 0111 1111 0000*/
#define IOCRBLOCKREQ_IOCRPROPERTIES_RESERVED1_CHECKING 2032

/* IOCrBlockReq IOCRProperties.reserved2 mask 1111 1111 1111 1111 1111 0000 0000 0000      */
#define IOCRBLOCKREQ_IOCRPROPERTIES_RESERVED2_CHECKING 0xfffff000L

/*IOCrBlockReq IOCRProperties.media redundancy mask 0000 1000 0000 0000*/
#define IOCRBLOCKREQ_MEDIA_REDUNDANCY_CHECKING 2048

#define FRAME_SEND_OFFSET_BEST_AS_POSSIBLE          0xffffffffL
#define RTA_TIMEOUT_FACTOR_MIN                      0x0001
#define RTA_TIMEOUT_FACTOR_MAX                      0x0064
#define RTA_RETRY_MIN                               3
#define RTA_RETRY_MAX                               15
#define AR_RPC_INITIATOR_RPC_SERVER_PORT_MIN        0x0400
#define AR_RPC_INITIATOR_RPC_SERVER_PORT_MAX        0xffff
#define REDUNDANCY_DHF_MIN                          0x0003
#define SUBMODULE_PROPERTIES_TYPE_MASK              3

/* Sysytem redundancy: SRProperties     */
#define SR_INPUT_VALID_ON_BACKUP_MASK     0x01

/* CRVLANID higth*/
/* mask : 0000 1111 1111 1111*/
#define ALARM_CRVLANID_CHECKIN 4095L
/* mask : 1110 0000 0000 0000*/
#define ALARM_USER_PRIORITY_CHECKIN 57344L

/*MC Block requestu checking */
/*mask :0000 0000 0000 0111*/
#define MCR_ADD_RESOLUTION_PROP_PROTOCOLE_CHECKIN 7L

/*mask :0000 0000 0000 0000 1111 1111 1111 1000*/
#define MCR_ADD_RESOLUTION_PROP_RESERVED_CHECKIN 65528L

/* mask :1111 1111 1111 1111 0000 0000 0000 0000*/
#define MCR_ADD_RESOLUTION_PROP_FACTOR_CHECKIN 0xffff0000L

#define DIFF_BLOCK_MAINTENANCE_REQUIRED       (16)
#define DIFF_BLOCK_MAINTENANCE_DEMANDED       (32)
#define DIFF_BLOCK_DIAG_AVAILABLE_DECALLAGE   (64)
#define DIFF_BLOCK_ARINFO_DECALLAGE           (128)
#define DIFF_BLOCK_IDENT_INFO_DECALLAGE       (2048L)
#define DIFF_BLOCK_FORMAT_IND_DECALLAGE       (32768L)

#define INITIATOR_TIMEOUT_FACTOR_BASE 100

/* DCP checking rules */
#define DCP_NAME_PORT_FORMAT "port-"
#define DCP_NAME_PORT_FORMAT_LEN  5
#define DCP_NAME_LABEL_MAX_LEN   63
#define DCP_NAME_LABEL_MIN_LEN    1
#define DCP_NAME_IP_MAX_NUMBER    3
#define DCP_NAME_IP_MIN_NUMBER    0
#define DCP_NAME_IP_LABEL_NUMBER  4


#define PNIO_RTC3_MULTICAST_MAC {0x01, 0x0e, 0xcf, 0x00, 0x01, 0x01}

#define LLDP_PEERTOPEER_BOUNDARY_EGRESS_CHECKING 0x0001

/*--------------------------------------------------------------------------*/
/*                       STRUCTURE DEFINITION                                */
/*---------------------------------------------------------------------------*/

#include "os_pck.h"

struct _ST_PNIO_STATUS
{
  APP_BYTE ErrorCode;
  APP_BYTE ErrorDecode;
  APP_BYTE ErrorCode1;
  APP_BYTE ErrorCode2;
}STRUCT_ALIGNEMENT;
typedef struct _ST_PNIO_STATUS PACK_ALIGNEMENT(ST_PNIO_STATUS), APP_FAR * LPST_PNIO_STATUS;
#define ST_PNIO_STATUS_SIZEOF 4
STRUCT_SIZE_CHECK(ST_PNIO_STATUS, ST_PNIO_STATUS_SIZEOF)

struct _ST_BLOCK_HEADER
{ /* 5.2.7*/
  APP_WORD BlocType;                                                      /* 0 - 1 */
  APP_WORD BLockLength;                                                   /* 2 - 3 */
  APP_BYTE BlockVersionHigh;                                              /* 4 - 4 */
  APP_BYTE BlockVersionLow;                                               /* 5 - 5 */
}STRUCT_ALIGNEMENT;
typedef struct _ST_BLOCK_HEADER PACK_ALIGNEMENT(ST_BLOCK_HEADER), APP_FAR * LPST_BLOCK_HEADER;
#define ST_BLOCK_HEADER_SIZEOF 6
STRUCT_SIZE_CHECK(ST_BLOCK_HEADER, ST_BLOCK_HEADER_SIZEOF)

/* PROFINETIOServiceReqPDU */
struct _ST_AR_BLOCK_REQ
{
  APP_WORD         ARType;
  ST_CLR_RPC_UUID  ARUUID;
  APP_WORD         SessionKey;
  APP_BYTE         CMInitiatorMacAdd[6];
  ST_CLR_RPC_UUID  CMInitiatorObjectUUID;
  ALIGNED_DWORD_DECL(ARProperties);
  APP_WORD         CMInitiatorActivityTimeoutFactor;
  APP_WORD         InitiatorUDPRTPort;
  APP_WORD         StationNameLength;
  APP_BYTE         CMInitiatorStationName[NAMELEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_AR_BLOCK_REQ PACK_ALIGNEMENT(ST_AR_BLOCK_REQ), APP_FAR *LPST_AR_BLOCK_REQ;
#define ST_AR_BLOCK_REQ_SIZEOF 292
STRUCT_SIZE_CHECK(ST_AR_BLOCK_REQ, ST_AR_BLOCK_REQ_SIZEOF)

struct _ST_IOCR_BLOCK_REQ
{
  APP_WORD   IOCRType;
  APP_WORD   IOCRReference;
  APP_WORD   LT;
  ALIGNED_DWORD_DECL(IOCRProperties);
  APP_WORD   DataLength;
  APP_WORD   FrameId;
  APP_WORD   SendClockFactor;
  APP_WORD   ReductionRatio;
  APP_WORD   Phase;
  APP_WORD   Sequence;
  ALIGNED_DWORD_DECL(FrameSendOffset);
  APP_WORD   WatchdogFactor;
  APP_WORD   DataHoldFactor;
  APP_WORD   IOCRTagHeader;
  APP_BYTE   IOCRMulticastMACAdd[6];
  APP_WORD   NumberOfAPIs;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_REQ PACK_ALIGNEMENT(ST_IOCR_BLOCK_REQ), APP_FAR *LPST_IOCR_BLOCK_REQ;
#define ST_IOCR_BLOCK_REQ_SIZEOF 40
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_REQ, ST_IOCR_BLOCK_REQ_SIZEOF)

struct _ST_IOCR_BLOCK_REQ_API
{
  ALIGNED_DWORD_DECL(API);
  APP_WORD   NumberOfIODataObjects;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_REQ_API PACK_ALIGNEMENT(ST_IOCR_BLOCK_REQ_API),ST_IOCR_BLOCK_RES_API, APP_FAR * LPST_IOCR_BLOCK_REQ_API;
#define ST_IOCR_BLOCK_REQ_API_SIZEOF 6
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_REQ_API, ST_IOCR_BLOCK_REQ_API_SIZEOF)

struct _ST_IOCR_BLOCK_REQ_IODATA_OBJECTS
{
  APP_WORD SlotNumber;
  APP_WORD SubSlotNumber;
  APP_WORD IODataObjectFrameOffset;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_REQ_IODATA_OBJECTS PACK_ALIGNEMENT(ST_IOCR_BLOCK_REQ_IODATA_OBJECTS), APP_FAR * LPST_IOCR_BLOCK_REQ_IODATA_OBJECTS;
#define ST_IOCR_BLOCK_REQ_IODATA_OBJECTS_SIZEOF 6
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_REQ_IODATA_OBJECTS, ST_IOCR_BLOCK_REQ_IODATA_OBJECTS_SIZEOF)

struct _ST_IOCR_BLOCK_REQ_IOCS
{
  APP_WORD NumberofIOCS;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_REQ_IOCS PACK_ALIGNEMENT(ST_IOCR_BLOCK_REQ_IOCS);
#define ST_IOCR_BLOCK_REQ_IOCS_SIZEOF 2
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_REQ_IOCS, ST_IOCR_BLOCK_REQ_IOCS_SIZEOF)


struct _ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS
{
  APP_WORD SlotNumber;
  APP_WORD SubSlotNumber;
  APP_WORD IOCSDataObjectFrameOffset;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS PACK_ALIGNEMENT(ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS), APP_FAR * LPST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS;
#define ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS_SIZEOF 6
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS, ST_IOCR_BLOCK_REQ_IOCS_DATA_OBJECTS_SIZEOF)

struct _ST_EXPETED_SUBMODULE_BLOCK_REQ
{
  APP_WORD NumberOfAPIs;
}STRUCT_ALIGNEMENT;
typedef struct _ST_EXPETED_SUBMODULE_BLOCK_REQ PACK_ALIGNEMENT(ST_EXPETED_SUBMODULE_BLOCK_REQ), APP_FAR * LPST_EXPETED_SUBMODULE_BLOCK_REQ;
#define ST_EXPETED_SUBMODULE_BLOCK_REQ_SIZEOF 2
STRUCT_SIZE_CHECK(ST_EXPETED_SUBMODULE_BLOCK_REQ, ST_EXPETED_SUBMODULE_BLOCK_REQ_SIZEOF)

struct _ST_MODULE_BLOCK_REQ
{
  ALIGNED_DWORD_DECL(API);
  APP_WORD   SlotNumber;
  ALIGNED_DWORD_DECL(ModuleIdentNumber);
  APP_WORD   ModuleProperties;
  APP_WORD   NumberOfSubModules;
}STRUCT_ALIGNEMENT;
typedef struct _ST_MODULE_BLOCK_REQ PACK_ALIGNEMENT(ST_MODULE_BLOCK_REQ), APP_FAR * LPST_MODULE_BLOCK_REQ;
#define ST_MODULE_BLOCK_REQ_SIZEOF 14
STRUCT_SIZE_CHECK(ST_MODULE_BLOCK_REQ, ST_MODULE_BLOCK_REQ_SIZEOF)

struct _ST_SUBMODULE_BLOCK_REQ
{
  APP_WORD   SubSlotNumber;
  ALIGNED_DWORD_DECL(SubModuleIdentNumber);
  APP_WORD   SubModuleProperties;
}STRUCT_ALIGNEMENT;
typedef struct _ST_SUBMODULE_BLOCK_REQ PACK_ALIGNEMENT(ST_SUBMODULE_BLOCK_REQ), APP_FAR * LPST_SUBMODULE_BLOCK_REQ;
#define ST_SUBMODULE_BLOCK_REQ_SIZEOF 8
STRUCT_SIZE_CHECK(ST_SUBMODULE_BLOCK_REQ, ST_SUBMODULE_BLOCK_REQ_SIZEOF)

struct _ST_DATADESCRIPTION_BLOCK_REQ
{
  APP_WORD   DataDescription;
  APP_WORD   SubModuleDataLength;
  APP_BYTE   LengthIOPS;
  APP_BYTE   LengthIOCS;
}STRUCT_ALIGNEMENT;
typedef struct _ST_DATADESCRIPTION_BLOCK_REQ PACK_ALIGNEMENT(ST_DATADESCRIPTION_BLOCK_REQ), APP_FAR * LPST_DATADESCRIPTION_BLOCK_REQ;
#define ST_DATADESCRIPTION_BLOCK_REQ_SIZEOF 6
STRUCT_SIZE_CHECK(ST_DATADESCRIPTION_BLOCK_REQ, ST_DATADESCRIPTION_BLOCK_REQ_SIZEOF)

struct _ST_ALARM_CR_BLOCK_REQ
{
  APP_WORD   AlarmCRType;
  APP_WORD   LT;
  ALIGNED_DWORD_DECL(AlarmCRProperties);
  APP_WORD   RTATimeoutFactor;
  APP_WORD   RTARetries;
  APP_WORD   LocalAlarmReference;
  APP_WORD   MaxAlarmDataLength;
  APP_WORD   AlarmCRTagHeaderHigh;
  APP_WORD   AlarmCRTagHeaderLow;
}STRUCT_ALIGNEMENT;
typedef struct _ST_ALARM_CR_BLOCK_REQ PACK_ALIGNEMENT(ST_ALARM_CR_BLOCK_REQ), APP_FAR *LPST_ALARM_CR_BLOCK_REQ, APP_FAR *LPST_ALARM_CR_BLOCK_RES;
#define ST_ALARM_CR_BLOCK_REQ_SIZEOF 20
STRUCT_SIZE_CHECK(ST_ALARM_CR_BLOCK_REQ, ST_ALARM_CR_BLOCK_REQ_SIZEOF)

struct _ST_PRMSERVER_CR_BLOCK_REQ
{
  ST_CLR_RPC_UUID  ParameterServerObjectUUID;
  ALIGNED_DWORD_DECL(ParameterServerProperties);
  APP_WORD         CMInitiatorActivityTimeoutFactor;
  APP_WORD         StationNameLength;
  APP_BYTE         ParameterServerStationName[NAMELEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_PRMSERVER_CR_BLOCK_REQ PACK_ALIGNEMENT(ST_PRMSERVER_CR_BLOCK_REQ), APP_FAR *LPST_PRMSERVER_CR_BLOCK_REQ;
#define ST_PRMSERVER_CR_BLOCK_REQ_SIZEOF 264
STRUCT_SIZE_CHECK(ST_PRMSERVER_CR_BLOCK_REQ, ST_PRMSERVER_CR_BLOCK_REQ_SIZEOF)

struct _ST_MCR_CR_BLOCK_REQ
{
  APP_WORD IOCRReference;
  ALIGNED_DWORD_DECL(AddressResolutionProperties);
  APP_WORD MCTimeOutFactor;
  APP_WORD StationNameLength;
  APP_BYTE ProviderStationName[NAMELEN];
}STRUCT_ALIGNEMENT;
typedef struct _ST_MCR_CR_BLOCK_REQ PACK_ALIGNEMENT(ST_MCR_CR_BLOCK_REQ), APP_FAR *LPST_MCR_CR_BLOCK_REQ;
#define ST_MCR_CR_BLOCK_REQ_SIZEOF 250
STRUCT_SIZE_CHECK(ST_MCR_CR_BLOCK_REQ, ST_MCR_CR_BLOCK_REQ_SIZEOF)

struct _ST_ARRPC_CR_BLOCK_REQ
{
  APP_WORD InitiatorRPCServerPort;
}STRUCT_ALIGNEMENT;
typedef struct _ST_ARRPC_CR_BLOCK_REQ PACK_ALIGNEMENT(ST_ARRPC_CR_BLOCK_REQ), APP_FAR *LPST_ARRPC_CR_BLOCK_REQ;
#define ST_ARRPC_CR_BLOCK_REQ_SIZEOF 2
STRUCT_SIZE_CHECK(ST_ARRPC_CR_BLOCK_REQ, ST_ARRPC_CR_BLOCK_REQ_SIZEOF)

struct _ST_IODCONNECT_REQ
{
  ST_AR_BLOCK_REQ  ArBlockReq;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IODCONNECT_REQ PACK_ALIGNEMENT(ST_IODCONNECT_REQ);
#define ST_IODCONNECT_REQ_SIZEOF 0
STRUCT_SIZE_CHECK(ST_IODCONNECT_REQ, ST_IODCONNECT_REQ_SIZEOF)

struct  _ST_ARBLOCK_RES
{
  APP_WORD         ARType;
  ST_CLR_RPC_UUID  ARUIUID;
  APP_WORD         wSession;
  APP_BYTE         CMResponderMac[6];
  APP_WORD         wCMResponderUdpPort;
}STRUCT_ALIGNEMENT;
typedef struct  _ST_ARBLOCK_RES PACK_ALIGNEMENT(ST_ARBLOCK_RES), APP_FAR * LPST_ARBLOCK_RES;
#define ST_ARBLOCK_RES_SIZEOF sizeof(ST_ARBLOCK_RES)
STRUCT_SIZE_CHECK(ST_ARBLOCK_RES, ST_ARBLOCK_RES_SIZEOF)

struct _ST_IOCR_BLOCK_RES
{
  APP_WORD wIOCRType;
  APP_WORD wIOCRReference;
  APP_WORD wFrameId;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOCR_BLOCK_RES PACK_ALIGNEMENT(ST_IOCR_BLOCK_RES), APP_FAR * LPST_IOCR_BLOCK_RES;
#define ST_IOCR_BLOCK_RES_SIZEOF 6
STRUCT_SIZE_CHECK(ST_IOCR_BLOCK_RES, ST_IOCR_BLOCK_RES_SIZEOF)

struct _ST_ALARM_BLOCK_RES
{
  APP_WORD wALARMCRType;
  APP_WORD wLocalAlarmReference;
  APP_WORD wMaxAlarmDataLen;
}STRUCT_ALIGNEMENT;
typedef struct _ST_ALARM_BLOCK_RES PACK_ALIGNEMENT(ST_ALARM_BLOCK_RES), APP_FAR * LPST_ALARM_BLOCK_RES;
#define ST_ALARM_BLOCK_RES_SIZEOF 6
STRUCT_SIZE_CHECK(ST_ALARM_BLOCK_RES, ST_ALARM_BLOCK_RES_SIZEOF)

struct _ST_EXPECTED_MODULE_BLOCK_RES
{
  APP_WORD wNumberOfAPI;
}STRUCT_ALIGNEMENT;
typedef struct _ST_EXPECTED_MODULE_BLOCK_RES PACK_ALIGNEMENT(ST_EXPECTED_MODULE_BLOCK_RES), APP_FAR * LPST_EXPECTED_MODULE_BLOCK_RES;
#define ST_EXPECTED_MODULE_BLOCK_RES_SIZEOF 2
STRUCT_SIZE_CHECK(ST_EXPECTED_MODULE_BLOCK_RES, ST_EXPECTED_MODULE_BLOCK_RES_SIZEOF)

struct _ST_EXPECTED_API_MODULE_BLOCK_RES
{
  ALIGNED_DWORD_DECL(dwAPI);
  APP_WORD  wNumberOfModules;
}STRUCT_ALIGNEMENT;
typedef struct _ST_EXPECTED_API_MODULE_BLOCK_RES PACK_ALIGNEMENT(ST_EXPECTED_API_MODULE_BLOCK_RES), APP_FAR * LPST_EXPECTED_API_MODULE_BLOCK_RES;
#define ST_EXPECTED_API_MODULE_BLOCK_RES_SIZEOF 6
STRUCT_SIZE_CHECK(ST_EXPECTED_API_MODULE_BLOCK_RES, ST_EXPECTED_API_MODULE_BLOCK_RES_SIZEOF)

struct _ST_EXPECTED_MODULE_STATE_BLOCK_REQ
{
  APP_WORD   SlotNumber;
  ALIGNED_DWORD_DECL(ModuleIdentNumber);
  APP_WORD   ModuleState;
  APP_WORD   NumberOfSubModules;
}STRUCT_ALIGNEMENT;
typedef struct _ST_EXPECTED_MODULE_STATE_BLOCK_REQ PACK_ALIGNEMENT(ST_EXPECTED_MODULE_STATE_BLOCK_REQ), APP_FAR * LPST_EXPECTED_MODULE_STATE_BLOCK_REQ;
#define ST_EXPECTED_MODULE_STATE_BLOCK_REQ_SIZEOF 10
STRUCT_SIZE_CHECK(ST_EXPECTED_MODULE_STATE_BLOCK_REQ, ST_EXPECTED_MODULE_STATE_BLOCK_REQ_SIZEOF)

struct _ST_EXPECTED_SUBMODULE_STATE_RES
{
  APP_WORD  SubSlotNumber;
  ALIGNED_DWORD_DECL(SubModuleIdentNumber);
  APP_WORD  SubModuleState;
}STRUCT_ALIGNEMENT;
typedef struct _ST_EXPECTED_SUBMODULE_STATE_RES PACK_ALIGNEMENT(ST_EXPECTED_SUBMODULE_STATE_RES),APP_FAR * LPST_EXPECTED_SUBMODULE_STATE_RES;
#define ST_EXPECTED_SUBMODULE_STATE_RES_SIZEOF 8
STRUCT_SIZE_CHECK(ST_EXPECTED_SUBMODULE_STATE_RES, ST_EXPECTED_SUBMODULE_STATE_RES_SIZEOF)

#define RW_PADDING_LEN          24
#define RW_PADDING_LEN_IMPLICIT  8
#define R_PADDING_RES_LEN       20

struct _ST_IOD_READ_WRITE
{
  APP_WORD        Seq_Number;
  ST_CLR_RPC_UUID ARUUID;
  ALIGNED_DWORD_DECL(API);
  APP_WORD        Slot_Number;
  APP_WORD        Subslot_Number;
  APP_WORD        Padding;
  APP_WORD        Index;
  ALIGNED_DWORD_DECL(Record_Data_Length);
  union {
    struct {
      APP_BYTE RWPAdding[RW_PADDING_LEN];
    }Wr_Req;

    struct {
      APP_WORD AddData1;
      APP_WORD AddData2;
      APP_BYTE ErrorCode;
      APP_BYTE ErrorDecode;
      APP_BYTE ErrorCode1;
      APP_BYTE ErrorCode2;
    }Wr_Res;

  }RdWr_u;
}STRUCT_ALIGNEMENT;
typedef struct _ST_IOD_READ_WRITE PACK_ALIGNEMENT(ST_IOD_READ_WRITE), APP_FAR * LPST_IOD_READ_WRITE;
#define ST_IOD_READ_WRITE_SIZEOF 58
STRUCT_SIZE_CHECK(ST_IOD_READ_WRITE, ST_IOD_READ_WRITE_SIZEOF)

struct _ST_CONTROL_BLOCK
{
  ST_BLOCK_HEADER  stBlockHeader;
  APP_WORD         Reserved1;
  ST_CLR_RPC_UUID  ARUUID;
  APP_WORD         wSession;
  APP_WORD         Reserved2;
  APP_WORD         Control_Command;
  APP_WORD         Control_Properties;
}STRUCT_ALIGNEMENT;
typedef struct _ST_CONTROL_BLOCK PACK_ALIGNEMENT(ST_CONTROL_BLOCK), APP_FAR * LPST_CONTROL_BLOCK;
#define ST_CONTROL_BLOCK_SIZEOF (26 + ST_BLOCK_HEADER_SIZEOF)

struct _ST_SUBMODULE_LIST_BLOCK
{
  APP_WORD         wNumberOfEntries;
}STRUCT_ALIGNEMENT;
typedef struct _ST_SUBMODULE_LIST_BLOCK PACK_ALIGNEMENT(ST_SUBMODULE_LIST_BLOCK), APP_FAR * LPST_SUBMODULE_LIST_BLOCK;
#define ST_SUBMODULE_LIST_BLOCK_SIZEOF 2

struct _ST_SUBMODULE
{
  APP_DWORD       dwAPI;
  APP_WORD        wSlotNumber;
  APP_WORD        wSubslotNumber;
}STRUCT_ALIGNEMENT;
typedef struct _ST_SUBMODULE PACK_ALIGNEMENT(ST_SUBMODULE), APP_FAR * LPST_SUBMODULE;
#define ST_SUBMODULE_SIZEOF 8

struct _ST_CONTROL_REQ_BLOCK
{
  APP_WORD         Reserved1;
  ST_CLR_RPC_UUID  ARUUID;
  APP_WORD         wSession;
  APP_WORD         Reserved2;
  APP_WORD         Control_Command;
  APP_WORD         Control_Properties;
}STRUCT_ALIGNEMENT;
typedef struct _ST_CONTROL_REQ_BLOCK PACK_ALIGNEMENT(ST_CONTROL_REQ_BLOCK), APP_FAR * LPST_CONTROL_REQ_BLOCK;
#define ST_CONTROL_REQ_BLOCK_SIZEOF (sizeof(ST_CONTROL_REQ_BLOCK))

struct _ST_LISTOF_IOCR
{
   APP_WORD wNbIOCR;
   LPST_IOCR_BLOCK_REQ pListIOCR;
}STRUCT_ALIGNEMENT;
typedef struct _ST_LISTOF_IOCR PACK_ALIGNEMENT(ST_LISTOF_IOCR), APP_FAR * LPST_LISTOF_IOCR;

struct _ST_FRAME_PROFINET_ALARM
{
  APP_WORD wAlarmDstEndpoint;
  APP_WORD wAlarmSrcEndpoint;
  APP_BYTE PDUType;
  APP_BYTE AddFlags;
  APP_WORD wSendSeqNum;
  APP_WORD wAckSeqNum;
  APP_WORD wVarParLen;
}STRUCT_ALIGNEMENT;
typedef struct _ST_FRAME_PROFINET_ALARM PACK_ALIGNEMENT(ST_FRAME_PROFINET_ALARM), APP_FAR * LPST_FRAME_PROFINET_ALARM;
#define ST_FRAME_PROFINET_ALARM_SIZEOF 12
STRUCT_SIZE_CHECK(ST_FRAME_PROFINET_ALARM, ST_FRAME_PROFINET_ALARM_SIZEOF)

struct _ST_FS_PARAMETER_BLOCK
{
  ALIGNED_DWORD_DECL(dwFSParameterBlock);
  ST_CLR_RPC_UUID  stFSParameterUUID;

}STRUCT_ALIGNEMENT;
typedef struct _ST_FS_PARAMETER_BLOCK PACK_ALIGNEMENT(ST_FS_PARAMETER_BLOCK), APP_FAR *LPST_FS_PARAMETER_BLOCK;
#define ST_FS_PARAMETER_BLOCK_SIZEOF 20
STRUCT_SIZE_CHECK(ST_AR_BLOCK_REQ, ST_FS_PARAMETER_BLOCK_SIZEOF)

/* System redundancy */
struct _ST_SRINFO_BLOCK_REQ
{
  APP_WORD         wRedundancyDataHoldFactor;
  ALIGNED_DWORD_DECL(dwSRProperties);
}STRUCT_ALIGNEMENT;
typedef struct _ST_SRINFO_BLOCK_REQ PACK_ALIGNEMENT(ST_SRINFO_BLOCK_REQ), APP_FAR *LPST_SRINFO_BLOCK_REQ;
#define ST_SRINFO_BLOCK_REQ_SIZEOF 6


/* Maintenance Item */
struct _ST_MAINTENANCE_ITEM_BLOCK
{
  APP_WORD         wPadding;
  ALIGNED_DWORD_DECL(dwMaintenanceStatus);
}STRUCT_ALIGNEMENT;
typedef struct _ST_MAINTENANCE_ITEM_BLOCK PACK_ALIGNEMENT(ST_MAINTENANCE_ITEM_BLOCK), APP_FAR *LPST_MAINTENANCE_ITEM_BLOCK;
#define ST_MAINTENANCE_ITEM_BLOCK_SIZEOF 6
#include "os_unpck.h"

#endif
