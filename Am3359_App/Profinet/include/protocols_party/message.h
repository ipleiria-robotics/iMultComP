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
/*  Component           : Message RELATION                                   */
/* $Workfile:: message.h                                                    $*/
/* $Revision:: 75                                                           $*/
/* $Date:: 17/10/16 9:39                                                    $*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  D e s c r i p t i o n :                                                  */
/*  Definition                                                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "stk_thd.h"
#include "system.h"

#define MSG_DATA_SIZE           (ETHERNET_MAX_FRAME_LENGTH + 4 + 87)  /* Max Size of the data inside a message  + 87 is used to align PN message with Applicom message*/
#define MIN_MESSAGE_FOR_STACK   (10)

#ifdef LLDP_SUPPORTED
  #define MAX_MSG               (15+20*MAX_DEVICE+WSE_LLDP_MAX_PORT)   /* Max message in the system*/  /* TODO calculate this and optimized it ! not for 15/10*/
#endif
#ifndef LLDP_SUPPORTED
  #define MAX_MSG               (15+20*MAX_DEVICE)   /* Max message in the system*/
#endif

#define PND_MAX_RETRY         1000000ul  /* Max retry to get message wait or Rpc buffer */

extern APP_DWORD gdwTaskID[MAXNUM_OF_QUEUE];

#define PN_MSG_VERSION_H 2
#define PN_MSG_VERSION_L 1
/*-----------------------------------------------------------------------*/
/* FSPMDEV Messages list                                                 */
/*-----------------------------------------------------------------------*/
#define ID_FSPMDEV_START_STACK                     0
#define ID_FSPMDEV_READ_RSP_POS                    1
#define ID_FSPMDEV_READ_RSP_NEG                    2
#define ID_FSPMDEV_WRITE_RSP_POS                   3
#define ID_FSPMDEV_WRITE_RSP_NEG                   4
#define ID_FSPMDEV_ALARM_REQ                       5
#define ID_FSPMDEV_ALARM_ACK_REQ                   6
#define ID_FSPMDEV_CONNECT_RSP_POS                 7
#define ID_FSPMDEV_CONNECT_RSP_NEG                 8
#define ID_FSPMDEV_DCONTROL_RSP_POS                9
#define ID_FSPMDEV_DCONTROL_RSP_NEG               10
#define ID_CPM_NO_DATA_IND                        11
#define ID_FSPMDEV_CCONTROL_REQ                   12
#define ID_CM_READ_IND                            13
#define ID_CM_WRITE_IND                           14
#define ID_CPM_SET_RED_ROLE_CNF                   15
#define ID_ALPMR_ACK_CNF_NEG                      16
#define ID_ALPMR_ACK_CNF_POS                      17
#define ID_CMDEV_NEW_OUTPUT_IND                   18
#define ID_ALPMI_IND_CNF_POS                      19
#define ID_ALPMI_IND_CNF_NEG                      20
#define ID_ALPMI_ACK                              21
#define ID_CMDEV_CONNECT_IND                      22
#define ID_CMDEV_RELEASE_IND                      23
#define ID_CMDEV_ABORT_IND                        24
#define ID_CMDEV_DCONTROL_IND                     25
#define ID_CMDEV_CCONTROL_CNF_POS                 26
#define ID_CMDEV_CCONTROL_CNF_NEG                 27
#define ID_RM_DCP_SET_NAME                        28
#define ID_RM_DCP_SET_IP                          29
#define ID_RM_DCP_SET_IP_FULL                     30
#define ID_RM_DCP_SET_BLINK                       31
#define ID_RM_DCP_SET_FACTORY_RESET               32
#define ID_RM_DCP_SET_RESET_FACTORY               33
#define ID_RM_DCP_SET_DHCP                        34
#define ID_RM_DCP_SET_DHCP_CONTROL                35
#define ID_FSPMDEV_ASESM_ABORT_REQ                36
#define ID_CM_INIT_CNF_POS                        37
#define ID_CM_INIT_CNF_NEG                        38
#define ID_FSPMDEV_DCP_SET_BLINK_CNF_POS          39
#define ID_FSPMDEV_DCP_SET_NAME_CNF_POS           40
#define ID_FSPMDEV_DCP_SET_NAME_CNF_NEG           41
#define ID_FSPMDEV_DCP_SET_IP_CNF_POS             42
#define ID_FSPMDEV_DCP_SET_IP_CNF_NEG             43
#define ID_FSPMDEV_DCP_SET_IP_FULL_CNF_POS        44
#define ID_FSPMDEV_DCP_SET_IP_FULL_CNF_NEG        45
#define ID_FSPMDEV_DCP_SET_RESET_FACTORY_CNF_POS  46
#define ID_FSPMDEV_DCP_SET_RESET_FACTORY_CNF_NEG  47
#define ID_FSPMDEV_DCP_SET_FACTORY_RESET_CNF_POS  48
#define ID_FSPMDEV_DCP_SET_FACTORY_RESET_CNF_NEG  49
#define ID_FSPMDEV_DCP_SET_DHCP_CNF_POS           50
#define ID_FSPMDEV_DCP_SET_DHCP_CNF_NEG           51
#define ID_FSPMDEV_DCP_SET_DHCP_CTRL_CNF_POS      52
#define ID_FSPMDEV_DCP_SET_DHCP_CTRL_CNF_NEG      53
#define ID_FSPMDEV_ALARM_CTRL_IND                 54
#define ID_POOL_UNLOCK_REQ                        55
#define ID_CMDEV_ABORT_CNF                        56
#define ID_FSPMDEV_START_RPC_COMPLETED            57
#define ID_FSPMDEV_MAX                            58

/*-----------------------------------------------------------------------*/
/* CMDEV Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMDEV_BAD_FUNCTION                      0
#define ID_CMDEV_INIT_REQ                          1
#define ID_RM_CONNECT_IND                          2
#define ID_CM_CONNECT_RSP_POS                      3
#define ID_CM_CONNECT_RSP_NEG                      4
#define ID_RM_DCONTROL_IND                         5
#define ID_CM_DCONTROL_RSP_POS                     6
#define ID_CM_DCONTROL_RSP_NEG                     7
#define ID_CM_CCONTROL_REQ                         8
#define ID_RM_CCONTROL_CNF_POS                     9
#define ID_RM_CCONTROL_CNF_NEG                    10
#define ID_RM_RELEASE_IND                         11
#define ID_CMSU_START_CNF_POS                     12
#define ID_CMSU_START_CNF_NEG                     13
#define ID_CMIO_INFO_IND                          14
#define ID_CMDEV_STATE_IND                        15
#define ID_CM_ABORT_REQ                           16
#define ID_CMDEV_MAX                              17

/*-----------------------------------------------------------------------*/
/* CMRPC Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMRPC_BAD_FUNCTION                      0
#define ID_CMRPC_RM_CCONTROL_REQ                   1
#define ID_CMRPC_RM_CCONTROL_CNF_NEG               2
#define ID_CMRPC_RM_CCONTROL_CNF_POS               3
#define ID_CMRPC_RM_DCONTROL_IND                   4
#define ID_CMRPC_RM_DCONTROL_RSP_POS               5
#define ID_CMRPC_RM_DCONTROL_RSP_NEG               6
#define ID_CMRPC_RM_WRITE_IND                      7
#define ID_CMRPC_RM_WRITE_RSP_POS                  8
#define ID_CMRPC_RM_WRITE_RSP_NEG                  9
#define ID_CMRPC_RM_READ_IND                      10
#define ID_CMRPC_RM_READ_IMPL_IND                 11
#define ID_CMRPC_RM_READ_RSP_POS                  12
#define ID_CMRPC_RM_READ_RSP_NEG                  13
#define ID_CMRPC_RM_CONNECT_IND                   14
#define ID_CMRPC_RM_CONNECT_RSP_POS               15
#define ID_CMRPC_RM_CONNECT_RSP_NEG               16
#define ID_CMRPC_RM_RELEASE_IND                   17
#define ID_CMRPC_RM_RELEASE_RSP_POS               18
#define ID_CMRPC_RM_RELEASE_RSP_NEG               19
#define ID_CMRPC_RM_CANCEL_CNF                    20
#define ID_CMRPC_CMDEV_STATE_IND                  21
#define ID_CMRPC_LOOKUP_REQ                       22
#define ID_CMRPC_LOOKUPFREE_REQ                   23
#define ID_CMRPC_MAX                              24

/*-----------------------------------------------------------------------*/
/* CMRDR Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMRDR_BAD_FUNCTION                      0
#define ID_CMRDR_RM_READ_IND                       1
#define ID_CMRDR_CM_READ_RSP_POS                   2
#define ID_CMRDR_CM_READ_RPS_NEG                   3
#define ID_CMRDR_MAX                               4

/*-----------------------------------------------------------------------*/
/* CMWRR Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMWRR_BAD_FUNCTION                      0
#define ID_CMWRR_RM_WRITE_IND                      1
#define ID_CMWRR_CM_WRITE_RSP_POS                  2
#define ID_CMWRR_CM_WRITE_RSP_NEG                  3
#define ID_CMWRR_CMDEV_STATE_IND                   4
#define ID_CMWRR_MAX                               5

/*-----------------------------------------------------------------------*/
/* CMPBE Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMPBE_BAD_FUNCTION                      0
#define ID_CMPBE_CMDEV_STATE_IND                   1
#define ID_CMPBE_RM_DCONTROL_IND                   2
#define ID_CMPBE_CM_DCONTROL_RSP                   3
#define ID_CMPBE_CM_CCONTROL_REQ                   4
#define ID_CMPBE_RM_DCONTROL_RSP                   5
#define ID_CMPBE_MAX                               6

/*-----------------------------------------------------------------------*/
/* CMSU Messages list                                                    */
/*-----------------------------------------------------------------------*/
#define ID_CMSU_BAD_FUNCTION                       0
#define ID_CMSU_START_REQ                          1
#define ID_CMSU_ALPMI_ACTIVATION_CNF_POS           2
#define ID_CMSU_ALPMI_ACTIVATION_CNF_NEG           3
#define ID_CMSU_ALPMI_CLOSE_CNF                    4
#define ID_CMSU_ALPMI_ERROR_IND                    5
#define ID_CMSU_ALPMR_ACTIVATION_CNF_POS           6
#define ID_CMSU_ALPMR_ACTIVATION_CNF_NEG           7
#define ID_CMSU_ALPMR_ACLOSE_CNF                   8
#define ID_CMSU_ALPMR_ERROR_IND                    9
#define ID_CMSU_APMR_ERROR_IND                    10
#define ID_CMSU_APMS_ERROR_IND                    11
#define ID_CMSU_CPM_ACTIVATION_CNF_POS            12
#define ID_CMSU_CPM_ACTIVATION_CNF_NEG            13
#define ID_CMSU_CPM_CLOSE_CNF                     14
#define ID_CMSU_CPM_ERROR_IND                     15
#define ID_CMSU_PPM_ACTIVATION_CNF_POS            16
#define ID_CMSU_PPM_ACTIVATION_CNF_NEG            17
#define ID_CMSU_PPM_CLOSE_CNF                     18
#define ID_CMSU_PPM_ERROR_IND                     19
#define ID_CMSU_CMDMC_ACTIVATE_CNF_POS            20
#define ID_CMSU_CMDMC_CLOSE_CNF                   21
#define ID_CMSU_CMDMC_ERROR_IND                   22
#define ID_CMSU_CMDEV_STATE_IND                   23
#define ID_CMSU_MAX                               24

/*-----------------------------------------------------------------------*/
/* CMINA Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMINA_BAD_FUNCTION                      0
#define ID_CMINA_STARTUP                           1
#define ID_CMINA_FU_TIMEOUT                        2
#define ID_CMINA_DCPUCR_SET_IND                    3
#define ID_CMINA_DCP_SET_NAME_CNF_POS              4
#define ID_CMINA_DCP_SET_NAME_CNF_NEG              5
#define ID_CMINA_DCP_SET_IP_CNF_POS                6
#define ID_CMINA_DCP_SET_IP_CNF_NEG                7
#define ID_CMINA_DCP_SET_IP_FULL_CNF_POS           8
#define ID_CMINA_DCP_SET_IP_FULL_CNF_NEG           9
#define ID_CMINA_DCP_SET_BLINK_CNF_POS            10
#define ID_CMINA_DCP_SET_RESET_FACTORY_CNF_POS    11
#define ID_CMINA_DCP_SET_RESET_FACTORY_CNF_NEG    12
#define ID_CMINA_DCP_SET_FACTORY_RESET_CNF_POS    13
#define ID_CMINA_DCP_SET_FACTORY_RESET_CNF_NEG    14
#define ID_CMINA_DCP_SET_DHCP_CNF_POS             15
#define ID_CMINA_DCP_SET_DHCP_CNF_NEG             16
#define ID_CMINA_DCP_SET_DHCP_CTRL_CNF_POS        17
#define ID_CMINA_DCP_SET_DHCP_CTRL_CNF_NEG        18
#define ID_CMINA_ASE_DCP_HELLO_REQUESTED          19
#define ID_CMINA_ASE_DHCP_OFFER_REQ               20
#define ID_CMINA_MAX                              21

/*-----------------------------------------------------------------------*/
/* CMIO Messages list                                                    */
/*-----------------------------------------------------------------------*/
#define ID_CMIO_BAD_FUNCTION                       0
#define ID_CMIO_CPM_NEW_DATA_IND                   1
#define ID_CMIO_CPM_STATE_DATA_IND                 2
#define ID_CMIO_CMDEV_STATE_IND                    3
#define ID_CMIO_TIMER_EXPIRED                      4
#define ID_CMIO_MAX                                5

/*-----------------------------------------------------------------------*/
/* CMDMC Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_CMDMC_BAD_FUNCTION                      0
#define ID_CMDMC_ACTIVATE_REQ                      1
#define ID_CMDMC_CLOSE_REQ                         2
#define ID_CMDMC_IDENTIFY_CNF                      3
#define ID_CMDMC_CPM_ACTIVATE_CNF_POS              4
#define ID_CMDMC_CPM_ACTIVATE_CNF_NEG              5
#define ID_CMDMC_CPM_NEW_DATA_IND                  6
#define ID_CMDMC_CPM_STATE_IND                     7
#define ID_CMDMC_CPM_CLOSE_CNF                     8
#define ID_CMDMC_MAX                               9

/*-----------------------------------------------------------------------*/
/* CMDEV_DA Messages list                                                */
/*-----------------------------------------------------------------------*/
#define ID_CMDEV_DA_BAD_FUNCTION                   0
#define ID_CMDEV_DA_INIT_REQ                       1
#define ID_CMDEV_DA_CONNECT_IND                    2
#define ID_CMDEV_DA_CM_ABORT_REQ                   3
#define ID_CMDEV_DA_CONNECT_RSP_POS                4
#define ID_CMDEV_DA_CONNECT_RSP_NEG                5
#define ID_CMDEV_DA_RELEASE_IND                    6
#define ID_CMDEV_DA_MAX                            7

/*-----------------------------------------------------------------------*/
/* CMSM Messages list                                                    */
/*-----------------------------------------------------------------------*/
#define ID_CMSM_BAD_FUNCTION                       0
#define ID_CMSM_CMDEV_STATE_IND                    1
#define ID_CMSM_TIMEOUT                            2
#define ID_CMSM_RM_READ_IND                        3
#define ID_CMSM_CM_READ_IND                        4
#define ID_CMSM_CM_READ_RSP                        5
#define ID_CMSM_CM_WRITE_IND                       6
#define ID_CMSM_CM_WRITE_RSP                       7
#define ID_CMSM_MAX                                8

/*-----------------------------------------------------------------------*/
/* STTUP Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_FSPMDEV_DCP_SET_NAME_IND               0
#define ID_FSPMDEV_DCP_SET_IP_IND                 1
#define ID_FSPMDEV_DCP_SET_IP_FULL_IND            2
#define ID_FSPMDEV_DCP_SET_BLINK_IND              3
#define ID_FSPMDEV_CONNECT_IND                    4
#define ID_FSPMDEV_READ_IND                       5
#define ID_FSPMDEV_CCONTROL_RSP_POS               6
#define ID_FSPMDEV_CCONTROL_RSP_NEG               7
#define ID_FSPMDEV_RELEASE_IND                    8
#define ID_STTUP_CONTROL_REQUEST                  9
#define ID_FSPMDEV_ABORT_IND                     10
#define ID_FSPMDEV_DCP_SET_FACTORY_RESET_IND     11
#define ID_FSPMDEV_DCP_SET_RESET_FACTORY_IND     12
#define ID_FSPMDEV_DCP_DHCP_IND                  13
#define ID_FSPMDEV_DCP_DHCP_CTRL_IND             14
#define ID_FSPMDEV_ALARM_NOTIFICATION_CNF_NEG    15
#define ID_FSPMDEV_ALARM_NOTIFICATION_CNF_POS    16
#define ID_FSPMDEV_ALARM_ACK_IND                 17
#define ID_STTUP_GET_INPUT_REQ                   18
#define ID_STTUP_SET_OUTPUT_REQ                  19
#define ID_STTUP_SET_INPUT_IOCS_REQ              20
#define ID_STTUP_GET_OUTPUT_IOCS_REQ             21
#define ID_STTUP_SET_OUTPUT_APDU_REQ             22
#define ID_PPM_SET_INPUT_CNF_POS                 23
#define ID_PPM_SET_INPUT_CNF_NEG                 24
#define ID_PPM_SET_OUTPUT_APDU_CNF_POS           25
#define ID_PPM_SET_OUTPUT_APDU_CNF_NEG           26
#define ID_CPM_GET_OUTPUT_CNF_POS                27
#define ID_CPM_GET_OUTPUT_CNF_NEG                28
#define ID_CPM_GET_OUTPUT_IOCS_CNF_POS           29
#define ID_CPM_GET_OUTPUT_IOCS_CNF_NEG           30
#define ID_STTUP_ALARM_REQ                       31
#define ID_STTUP_CTRL_ALARM_ACK                  32
#define ID_STTUP_ALARM_CTRL_IND                  33
#define ID_STTUP_INIT                            34
#define ID_FSPMDEV_INIT_CNF_POS                  35
#define ID_FSPMDEV_INIT_CNF_NEG                  36
#define ID_STTUP_GET_INPUT_APDU_REQ              37
#define ID_STTUP_STOP                            38
#define ID_STTUP_HELLO_CONFIGURATION             39
#define ID_STTUP_SSLOT_CONFIGURATION             40
#define ID_STTUP_REGISTER_ALIAS_NAME             41
#define ID_STTUP_UPDATE_ALIAS_NAME               42
#define ID_STTUP_REMOVE_ALIAS_NAME               43
#define ID_STTUP_PLUG_REQ                        44
#define ID_STTUP_PULL_REQ                        45
#define ID_FSPMDEV_WRITE_IND                     46
#define ID_FSPMDEV_DCONTROL_IND                  47
#define ID_STTUP_PLUG_CONTROL_REQUEST            48
#define ID_STTUP_PLUG_CCONTROL_RSP               49
#define ID_PLUGSM_ALM_NOT_CNF_POS                50
#define ID_PLUGSM_ALM_NOT_CNF_NEG                51
#define ID_PLUGSM_ALARM_CNF_TIMEOUT              52
#define ID_PLUGSM_ALARM_REQ                      53
#define ID_PULLSM_ALARM_REQ                      54
#define ID_PULLSM_ALM_NOT_CNF_POS                55
#define ID_PULLSM_ALM_NOT_CNF_NEG                56
#define ID_STTUP_SET_SUBMODULE_STATE             57
#define ID_STTUP_GET_SUBMODULE_STATE             58
#define ID_CMDEV_IN_DATA_IND                     59
#define ID_STTUP_PEER_MISMATCH_IND               60
#define ID_CMDEV_NEW_DATA_IND                    61
#define ID_XPM_SM_REMOVE_ITEM_CNF                62
#define ID_XPM_SM_ADD_ITEM_CNF                   63
#define ID_SR_RDHT                               64
#define ID_SR_SWITCHOVER                         65
#define ID_SR_REISSUE_ALM                        66
#define ID_FSPMDEV_R_TIMEOUT                     67
#define ID_FSPMDEV_READ_CNF_POS                  68
#define ID_FSPMDEV_READ_CNF_NEG                  69
#define ID_FSPMDEV_W_TIMEOUT                     70
#define ID_FSPMDEV_WRITE_CNF                     71
#define ID_PLUSM_WRITE_CNF                       72
#define ID_STTUP_GET_DIAGNOSIS_COUNTER           73
#define ID_STTUP_RESET_DIAGNOSIS_COUNTER         74
#define ID_STTUP_SET_PARAM                       75
#define ID_STTUP_INFORM_LINKORMAU_CHANGE         76
#define ID_STTUP_MAUTYPE_MISMATCH_IND            77
#define ID_STTUP_LINK_MISMATCH_IND               78
#define ID_STTUP_INFORM_LINEDELAY_CHANGE         79
#define ID_STTUP_INFORM_SYNCSTATUS_CHANGE        80
#define ID_STTUP_INFORM_PEERRMAU_CHANGE          81
#define ID_STTUP_INFORM_PEERRDELAY_CHANGE        82
#define ID_STTUP_INFORM_PEERRPTCP_CHANGE         83
#define ID_STTUP_INFORM_PEERPORTSTATUS_CHANGE    84
#define ID_STTUP_INFORM_REMOVEPEER_CHANGE        85
#define ID_STTUP_LINEDELAY_MISMATCH_IND          86
#define ID_STTUP_PEERMAUTYPE_MISMATCH_IND        87
#define ID_STTUP_PEERDELAY_MISMATCH_IND          88
#define ID_STTUP_PEERPTCP_MISMATCH_IND           89
#define ID_STTUP_ADD_DIAGNOSTIC                  90
#define ID_STTUP_REMOVE_DIAGNOSTIC               91
#define ID_FSPMDEV_PRM_BEGIN_IND                 92
#define ID_STTUP_INFORM_PEERMRPPORTSTATUS_CHANGE 93
#define ID_LLDP_PARAMETER_APPLIED                94
#define ID_STTUP_MAX                             95

#ifdef PN_RTC3_SUPPORTED
#define ID_PSM_INIT                             0
#define ID_PSM_EXIT                             1
#define ID_PSM_PEERPORT_STATE                   2
#define ID_PSM_PEERPTCP_CHANGE                  3
#define ID_PSM_NEIGBOR                          4
#define ID_PSM_SYNC                             5
#define ID_PSM_LINE_DELAY                       6
#define ID_PSM_LINK_STATE                       7
#define ID_PSM_PTCP_CHANGE                      8
#endif

#define ID_PSM_MAX                              9

/*-----------------------------------------------------------------------*/
/* FAL Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_MSG_ACYC_STOP                           0
#define ID_MSG_ACYC_START                          1

/*-----------------------------------------------------------------------*/
/* RPC Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_RPCRUNTIME_STOP                    0
#define ID_RPCRUNTIME_INVOKE                  1
#define ID_RPCRUNTIME_CANCEL                  2
#define ID_RPC_ALLOC                          3
#define ID_RPC_ALLOC_ITFS                     4
#define ID_RPC_CLEAR_ITF                      5
#define ID_RPCRUNTIME_RESPONSE                6
#define ID_RPCRUNTIME_TIMER                   7
#define ID_RPCRUNTIME_TMFRAG                  8
#define ID_RPCRUNTIME_TMWAIT                  9
#define ID_RPCRUNTIME_TMPING                 10
#define ID_RPCRUNTIME_TMACK                  11
#define ID_RPCRUNTIME_TMCANCEL               12
#define ID_RPCRUNTIME_TMBROADCAST            13
#define ID_RPCRUNTIME_TMIDLE                 14
#define ID_RPCRUNTIME_TMRESEND               15
#define ID_RPCRUNTIME_START                  16
#define ID_RPCRUNTIME_RESTART_SERVER         17
#define ID_RPC_MAX                           18


/*-----------------------------------------------------------------------*/
/* CPM Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_CPM_ACTIVE_REQ                          0
#define ID_CPM_CLOSE_REQ                           1
#define ID_CPM_GET_CONS_STATUS_REQ                 2
#define ID_CPM_SET_RED_ROLE_REQ                    3
#define ID_CPM_GET_CONS_DATA_REQ                   4
#define ID_CPM_C_DATA_IND                          5
#define ID_CPM_STOP                                6
#define ID_CPM_START                               7
#define ID_CPM_READY                               8
#define ID_CPM_END                                 9
#ifdef SHARED_MEMORY_MODE
#define ID_CPM_SM_REMOVE_ITEM                     10
#define ID_CPM_SM_ADD_ITEM                        11
#endif
#define ID_CPM_MAX                                12

/*-----------------------------------------------------------------------*/
/* PPM Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_PPM_START                               0
#define ID_PPM_ACTIVE_REQ                          1
#define ID_PPM_CLOSE_REQ                           2
#define ID_PPM_SET_PROV_DATA_REQ                   3
#define ID_PPM_SET_PROV_STATUS_REQ                 4
#define ID_PPM_C_DATA_CNF                          5
#define ID_PPM_STOP                                6
#define ID_PPM_READY                               7
#define ID_PPM_END                                 8
#define ID_PPM_SET_PB_INDICATOR_REQ                9
#define ID_PPM_MAX                                10

/*-----------------------------------------------------------------------*/
/* DCP Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_DCPMCR_IDENTIFY_REQ                     0
#define ID_DCPMCR_TIMEOUT                          1
#define ID_DCPUCR_GET_REQ                          2
#define ID_DCPUCR_SET_REQ                          3
#define ID_DCPUCR_SET_RSP                          4
#define ID_DCPUCR_TIMEOUT                          5
#define ID_DCPHMCS_HELLO_REQ                       6
#define ID_DV_DCP_MAX                              7

/*-----------------------------------------------------------------------*/
/* ALM Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_ALM_BAD_FUNCTION                        0
#define ID_ALPMR_ACTIVE_REQ                        1
#define ID_ALPMR_CLOSE_REQ                         2
#define ID_ALPMR_ALARM_ACK_REQ                     3
#define ID_ALPMR_APMR_A_DATA_IND                   4
#define ID_ALPMR_APMR_A_DATA_CNF_POS               5
#define ID_ALPMR_APMR_A_DATA_CNF_NEG               6
#define ID_ALPMI_ACTIVE_REQ                        7
#define ID_ALPMI_CLOSE_REQ                         8
#define ID_ALPMI_ALARM_NOT_REQ                     9
#define ID_ALPMI_APMR_A_DATA_IND                  10
#define ID_ALPMI_APMR_A_DATA_CNF_POS              11
#define ID_ALPMI_APMR_A_DATA_CNF_NEG              12
#define ID_APMR_A_DATA_IND                        13
#define ID_APMS_A_DATA_REQ                        14
#define ID_APMS_A_DATA_IND                        15
#define ID_APMS_A_TIMER_EVENT                     16
#define ID_ALM_STOP                               17
#define ID_ALARM_START                            18
#define ID_ALM_MAX                                19

/*-----------------------------------------------------------------------*/
/* ADDIN Messages list                                                   */
/*-----------------------------------------------------------------------*/
#define ID_MSG_ADDIN_STOP                          0
#define ID_MSG_ADDIN_START                         1
#define ID_MSG_ADDIN_MAX                           2

/*-----------------------------------------------------------------------*/
/* MRP Messages list                                                     */
/*-----------------------------------------------------------------------*/
#define ID_MRP_INIT                                0
#define ID_MRP_EXIT                                1
#define ID_MRM_START                               2
#define ID_MRM_STOP                                3
#define ID_MRM_READ                                4
#define ID_MRC_START                               5
#define ID_MRC_STOP                                6
#define ID_MRC_READ                                7
#define ID_MRP_RCV_PDU                             8
#define ID_MRP_LOCAL_LINK_CHANGE                   9
#define ID_MRP_TIMER                              10
#define ID_MRP_MAX                                11

/*-----------------------------------------------------------------------*/
/* LMPM Messages list                                                    */
/*-----------------------------------------------------------------------*/
#define ID_LMPM_START                              0
#define ID_LMPM_MAX                                1

/*-----------------------------------------------------------------------*/
/* Schedule Messages list                                                */
/*-----------------------------------------------------------------------*/
#define ID_SCHEDULE_START                          0
#define ID_SCHEDULE_STOP                           1
#define ID_SCHEDULE_CREATE                         2
#define ID_SCHEDULE_CLOSE                          3
#define ID_PPM_PROD_INSERT                         4
#define ID_PPM_PROD_REMOVE                         5
#define ID_CPM_CONS_INSERT                         6
#define ID_CPM_CONS_REMOVE                         7
#define ID_WRITE_FRAME                             8
#define ID_WRITE_FRAME_SPECIFIC_PORT               9

#ifdef SHARED_MEMORY_MODE
#define ID_SCHEDULE_SM_REMOVE_ITEM                10
#define ID_SCHEDULE_SM_ADD_ITEM                   11
#endif
#define ID_SCHEDULE_MAX                           12



typedef struct _st_restartserver
{
  APP_DWORD dwOldIp;
  APP_DWORD dwNewIP;
}ST_RESTART_SERVER, * PST_RESTART_SERVER , APP_FAR * LPST_RESTART_SERVER;


/*-----------------------------------------------------------------------*/
/* macro message                                                         */
/*-----------------------------------------------------------------------*/
#define OS_MESSAGE  __LPMSG
#define OS_DECLARE_MESSAGE LPST_MSG __LPMSG = APP_NULL
#define OS_FREE_MESSAGE    OsFreeMessage(&__LPMSG)

#define USER_OS_SEND_MESSAGE_MRP(c) OS_SEND_MESSAGE_MRP(ID_QUEUE_USER, c)
#define OS_SEND_MESSAGE_MRP(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_MRP;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_MRP],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_ADDIN(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_ADDIN;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_ADDIN],&__LPMSG,MSG_PRIO_NORMAL)

#define USER_OS_SEND_MESSAGE_STTUP(c) OS_SEND_MESSAGE_STTUP(ID_QUEUE_USER, c)
#define USER_OS_SEND_MESSAGE(q, c) OS_SEND_MESSAGE(ID_QUEUE_USER, q, c)

#define OS_SEND_MESSAGE_STTUP(queueSender, c) SEND_MESSAGE_STTUP(queueSender, c,__LPMSG)

#define SEND_MESSAGE_STTUP(queueSender, c, msg)\
  msg->dwQueue    = ID_QUEUE_ASESM;\
  msg->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_ASESM],&msg,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_PSM(queueSender, c) SEND_MESSAGE_PSM(queueSender, c,__LPMSG)

#define SEND_MESSAGE_PSM(queueSender, c, msg)\
  msg->dwQueue    = ID_QUEUE_PSM;\
  msg->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_PSM],&msg,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_FSPMDEV(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_FSPMDEV;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_FSPMDEV],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMDEV(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMDEV;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMDEV],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMRPC(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMRPC;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMRPC],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMPBE(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMPBE;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMPBE],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMSU(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMSU;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMSU],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMIO(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMIO;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMIO],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CMSM(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMSM;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMSM],&__LPMSG,MSG_PRIO_NORMAL)
#define OS_SEND_MESSAGE_CMINA(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CMINA;\
  __LPMSG->wCodeFnct  = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CMINA],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_DCP(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_DCP;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_DCP],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_CPM(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_CPM;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_CPM],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_PPM(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_PPM;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_PPM],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_ALM(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_ALM;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_ALM],&__LPMSG,MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_SCHEDULE(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_SCHEDULE;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_SCHEDULE],&__LPMSG,MSG_PRIO_NORMAL)

#define USER_SEND_MESSAGE_RPC(m,c) SEND_MESSAGE_RPC(ID_QUEUE_USER, m,c)

#define SEND_MESSAGE_RPC(queueSender, m,c)\
  (*m)->dwQueue    = ID_QUEUE_RPC;\
  (*m)->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_RPC],m,MSG_PRIO_NORMAL)

#define USER_OS_SEND_MESSAGE_RPC(c) SEND_MESSAGE_RPC(ID_QUEUE_USER, &__LPMSG, c)
#define OS_SEND_MESSAGE_RPC(queueSender, c) SEND_MESSAGE_RPC(queueSender, &__LPMSG,c)

#define SEND_MESSAGE_FROM_RPC(m, q,c)\
  (m)->dwQueue    = (q);\
  (m)->wCodeFnct = (c);\
  OsSendMessagePtr(ID_QUEUE_RPC, gdwTaskID[(q)], &(m),MSG_PRIO_NORMAL)

#define OS_SEND_MESSAGE_FROM_RPC(q,c)\
  SEND_MESSAGE_FROM_RPC(__LPMSG,q,c )

#define OS_SEND_MESSAGE(_queueSender, _dwQueue, _wCodeFunction)\
  __LPMSG->dwQueue    = _dwQueue;\
  __LPMSG->wCodeFnct  = _wCodeFunction;\
  OsSendMessagePtr(_queueSender, gdwTaskID[_dwQueue],&__LPMSG,MSG_PRIO_NORMAL);

#define USER_OS_SEND_MESSAGE_LLDP(c) OS_SEND_MESSAGE_LLDP(ID_QUEUE_USER, c)

#define OS_SEND_MESSAGE_LLDP(queueSender, c)\
  __LPMSG->dwQueue    = ID_QUEUE_LLDP;\
  __LPMSG->wCodeFnct = c;\
  OsSendMessagePtr(queueSender, gdwTaskID[ID_QUEUE_LLDP],&__LPMSG,MSG_PRIO_NORMAL)

#define LLDP_DATA_IND OS_SEND_MESSAGE_LLDP(ID_QUEUE_LMPM, ID_LLDP_LLDPDU)
#define OS_GET_MESSAGE_WAIT OsGetMessageWait(&__LPMSG)

#define OS_GET_MESSAGE_POOL(lpstPoolIocr, lpstPoolUsi, lpstPoolEc) \
{\
  APP_DWORD dwCmp = 0;\
  while( OsGetMessage(&__LPMSG) == OS_NOT_OK ) \
  {\
    OsWait_ms(10);\
    dwCmp++;\
    if(dwCmp > PND_MAX_RETRY){\
      OS_ASSERT(APP_FALSE);\
    }\
  }\
  __LPMSG->wDataSize = 0;\
  lpstPoolIocr = (LPST_POOL)&__LPMSG->byData[(__LPMSG->wDataSize)];\
  __LPMSG->wDataSize += sizeof(ST_POOL);\
  lpstPoolUsi  = (LPST_POOL)&__LPMSG->byData[(__LPMSG->wDataSize)];\
  __LPMSG->wDataSize += sizeof(ST_POOL);\
  lpstPoolEc   = (LPST_POOL)&__LPMSG->byData[(__LPMSG->wDataSize)];\
  pool_init_empty_pool(lpstPoolIocr, (MAX_CR_PER_AR*MAX_ARD_PER_DEVICE), sizeof(ST_CR));\
  pool_init_empty_pool(lpstPoolUsi , (PN_CONFIGURATION_SIZE)           , sizeof(ST_CR));\
  pool_init_empty_pool(lpstPoolEc  , (PN_CONFIGURATION_SIZE)           , sizeof(ST_CR));\
}

#define OS_GET_MESSAGE OsGetMessage(&__LPMSG)

#define OS_GET_HELLO_PARAM_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_HELLO_PARAMETERS)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_HELLO_PARAMETERS)

#define OS_GET_SET_PARAM_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_SET_PARAM)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_SET_PARAM)

#define OS_GET_INPUT_PARAM_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_INPUT_PARAM)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_INPUT_PARAM)

#define OS_GET_PLUG_PULL_PARAM_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_PLUG_PULL_PARAM)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_PLUG_PULL_PARAM)

#define OS_GET_INITIALIZATION_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_INITIALIZATION)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_INITIALIZATION)

#define OS_GET_ALM_NOTIFICATION_WAIT(x, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_DATA_ALARM_NOTIFICATION)&(__LPMSG->byData[OFFSET_PARAM1_MESSAGE]);\
    __LPMSG->wDataSize = (APP_WORD)(sizeof(ST_DATA_ALARM_NOTIFICATION) + wSizeOfData)

#define OS_GET_INTERFACE_REG_WAIT(x) \
  OS_GET_MESSAGE_WAIT;\
  x=(LPST_INTERFACE_REGISTER)__LPMSG->byData;\
    __LPMSG->wDataSize = sizeof(ST_INTERFACE_REGISTER)

#define OS_GET_ST_PPM_SET_PROV_DATA_REQ_WAIT(pData,wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_PPM_SET_PROV_DATA_REQ)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define OS_GET_ST_CPM_GET_CONS_DATA_REQ_WAIT(pData,wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_CPM_GET_CONS_DATA_REQ)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define OS_GET_ST_SCHEDULE_INSERT_WAIT(pData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_SCHEDULE_INSERT)__LPMSG->byData;\
  __LPMSG->wDataSize = sizeof(ST_SCHEDULE_INSERT)

#define OS_GET_ST_SCHEDULE_REMOVE_WAIT(pData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_SCHEDULE_REMOVE)__LPMSG->byData;\
  __LPMSG->wDataSize = sizeof(ST_SCHEDULE_REMOVE)

#define OS_GET_ST_MRM_CONF_WAIT(pData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MRM_CONF)(&__LPMSG->byData[OFFSET_PARAM2_MESSAGE]);\
  __LPMSG->wDataSize = sizeof(ST_MRM_CONF)

#define OS_GET_ST_MRC_CONF_WAIT(pData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MRC_CONF)(&__LPMSG->byData[OFFSET_PARAM2_MESSAGE]);\
  __LPMSG->wDataSize = sizeof(ST_MRC_CONF)

#define STACK_SEND_MSG_STOP_THREAD(a,b)\
  while( OsGetMessage(&__LPMSG) == OS_NOT_OK){\
  OsWait_ms(10);\
  }\
  __LPMSG->dwQueue = a;\
  __LPMSG->wCodeFnct = b;\
  __LPMSG->wDataSize = 0;\
  OsSendMessagePtr(ID_QUEUE_USER, gdwTaskID[a],&__LPMSG,MSG_PRIO_HIGH)

/**************************** LLDP messaging *********************************/
#define OS_GET_ST_LLDP_CONFIGTIME_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLDP_CONFIGTIME)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLDP_CONFIGTIME_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_CONFIGTIME)(&((a)->byData))

#define OS_GET_ST_LLDP_PORTCONFIG_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLDP_PORTCONFIG)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLDP_PORTCONFIG_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_PORTCONFIG)(&((a)->byData))

#define GET_ST_LLDP_PORTADMINCONFIG_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_PORTADMINCONFIG)(&((a)->byData))


#define OS_GET_ST_LLDP_SYSTEMCONFIG_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLDP_SYSTEM_CONFIG)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLDP_SYSTEMCONFIG_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_SYSTEM_CONFIG)(&((a)->byData))

#define OS_GET_ST_LLDP_DESCRIPTION_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLDP_DESCRIPTION)__LPMSG->byData;\
  OS_MEMSET(pData, 0, sizeof(ST_MSG_LLDP_DESCRIPTION));\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLDP_DESCRIPTION_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_DESCRIPTION)(&((a)->byData))

#define OS_GET_ST_LLDP_PNPORTSTATUS_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSP_LLDP_PN_PORTSTATUS)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLDP_PNPORTSTATUS_IN_MESSAGE(a,b)  (b)=(LPST_MSP_LLDP_PN_PORTSTATUS)(&((a)->byData))

#define OS_GET_ST_LLLDP_PN_CHASSISMAC_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLLDP_PN_CHASSISMAC)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLLDP_PN_CHASSISMAC_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLLDP_PN_CHASSISMAC)(&((a)->byData))

#define OS_GET_ST_LLLDP_PN_MRP_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_MSG_LLDP_PN_MRP)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLLDP_PN_MRP_IN_MESSAGE(a,b)  (b)=(LPST_MSG_LLDP_PN_MRP)(&((a)->byData))

#define OS_GET_ST_LLLDP_8023_AUTONEG_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_LLDP_MSG_8023_AUTONEG)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLLDP_8023_AUTONEG_IN_MESSAGE(a,b)  (b)=(LPST_LLDP_MSG_8023_AUTONEG)(&((a)->byData))

#define GET_ST_LLLDP_8023_PHYCONFIG_IN_MESSAGE(a,b)  (b)=(LPST_LLDP_MSG_8023_PHYCONFIG)(&((a)->byData))

#define OS_GET_ST_LLLDP_PN_ADDR_MGT_WAIT(pData, wSizeOfData) \
  OS_GET_MESSAGE_WAIT;\
  pData=(LPST_LLDP_MSG_PN_ADDR_MGT)__LPMSG->byData;\
  __LPMSG->wDataSize = wSizeOfData

#define GET_ST_LLLDP_PN_ADDR_MGT_IN_MESSAGE(a,b)  (b)=(LPST_LLDP_MSG_PN_ADDR_MGT)(&((a)->byData))

/*-----------------------------------------------------------------------*/
/* definition                                                            */
/*-----------------------------------------------------------------------*/
#define SEND_MESSAGE    0
#define NO_SEND_MESSAGE ((APP_DWORD)(~SEND_MESSAGE))
#define MSG_PRIO_NORMAL 0
#define MSG_PRIO_HIGH   1

typedef union _UN_PN_VERSION{
  struct
  {
    APP_BYTE byVerH;       /* Version Higth of the current message */
    APP_BYTE byVerL;       /* Version Low   of the current message */
  }ST_PN_VERSION_MSG;
  APP_DWORD dwVersion;     /* Align this structure on a DWORD !    */
}UN_PN_VERSION;

typedef union _UN_PN_SIZE{

  APP_WORD  wSizeMailMax;  /* Max Data that this mail can store    */
  APP_DWORD dwSize;        /* Align this structure on a DWORD !    */
}UN_PN_SIZE;

#define MSG_VERSION_L unVersion.ST_PN_VERSION_MSG.byVerL
#define MSG_VERSION_H unVersion.ST_PN_VERSION_MSG.byVerH


#define PN_MAX_DEBUG_MSG_ELMTS 15

#ifdef OS_DEBUG_MSG

#define MSG_STATE_FREE 1
#define MSG_STATE_TAKE 2
#define MSG_STATE_SEND 3
#define MSG_STATE_READ 4

typedef struct _stack_msg_debug
{
  APP_BYTE  byMsgState;
  APP_DWORD dwLastTakeMsg;

  APP_WORD  wCurrentIndex;
  APP_WORD  wLogCount;
  APP_DWORD dwQueue[PN_MAX_DEBUG_MSG_ELMTS];
  APP_WORD  wMsgId[PN_MAX_DEBUG_MSG_ELMTS];
} ST_MSG_DEBUG, APP_FAR *LPST_MSG_DEBUG;
#endif

/*-----------------------------------------------------------------------*/
/* message structure                                                     */
/*-----------------------------------------------------------------------*/
typedef struct _stack_msg
{
  UN_PN_VERSION   unVersion;             /* Version of the current message                              */
  UN_PN_SIZE      unSize;                /* Max Data that this mail can store                           */
  APP_DWORD       dwReserved;            /* Now this parameter is not used. Later use it to link mail ? */
  APP_DWORD       dwQueue;               /* ID of sub queue inside the Thread/Task                      */
  APP_DWORD       dwQueueSender;         /*                                                             */
  APP_LPVOID      lpvRep;                /* Arep/Drep/Irep                                              */
  APP_LPVOID      lpvCrep;               /* Alm/PPM/CPM                                                 */
  APP_LPVOID      lpvContexte;           /* Contexte in message                                         */
  APP_WORD        wCodeFnct;             /* ID of the function to call                                  */
  APP_WORD        wDataSize;             /* Size of data used in byData                                 */
  APP_BYTE        byData[MSG_DATA_SIZE]; /* Data Associated to the message                              */
  #ifdef OS_DEBUG_MSG
  ST_MSG_DEBUG    stDebugMsg;
  #endif
} ST_MSG,APP_FAR *LPST_MSG;
#define SIZE_ST_MSG (sizeof(ST_MSG))

/*inter thread message call type*/
typedef APP_DWORD  (APP_FAR *FCT_MSG_ARG)(LPST_MSG* ppMsg);

/*-----------------------------------------------------------------------*/
/* Prototype                                                             */
/*-----------------------------------------------------------------------*/
APP_WORD  OsInitMessage         (APP_VOID);
APP_WORD  OsExitMessage         (APP_VOID);
APP_WORD  OsGetMessage          (LPST_MSG* ppMsg);
APP_WORD  OsFreeMessage         (LPST_MSG* ppMsg);
APP_WORD  OsGetMessageWait      (LPST_MSG* ppMsg);
APP_WORD  OsCreateMsgQueue      (APP_LONG lTaskId);
APP_WORD  OsDestroyMsgQueue     (APP_LONG lTaskId);
APP_WORD  OsGetNbFreeMessage    (APP_VOID);
APP_WORD  OsSendMessagePtr      (APP_DWORD dwQueueSender, APP_LONG lTaskId, LPST_MSG* pMsg, APP_WORD wPrio);
APP_WORD  OsReadMessageNoWaitPtr(APP_LONG lTaskId, LPST_MSG* pMsg );
APP_WORD  OsReadMessageWaitPtr  (APP_LONG lTaskId, LPST_MSG* pMsg );
APP_DWORD APP_FAR FxEmpty       (LPST_MSG* ppMsg);
#endif
