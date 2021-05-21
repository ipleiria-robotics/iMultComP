/**
 * @file  icss_timeSync.h
 *
 * @brief Include file for icss_timeSync.c
 *
 *\par
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *\par
 */
#ifndef ICSS_TIMESYNC_H_
#define ICSS_TIMESYNC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdlib.h>
#include <math.h>
#include <ti/drv/icss_emac/icss_emac_ver.h>

#if ICSS_EMAC_DRV_VERSION_ID ==(0x01000002)
#include <ti/drv/icss_emac/src/icss_emacSwitch.h>
#else
#include <ti/drv/icss_emac/src/icss_emacLoc.h>
#endif
#include "icss_timeSyncApi.h"


/* ========================================================================== */
/*                                Error Codes                                 */
/* ========================================================================== */
/**
 @brief Success
 */
#define TIME_SYNC_OK                                                         (1)
/**
 @brief Time Sync module is unable to create a Task
 */
#define TIME_SYNC_UNABLE_TO_CREATE_TASK                                     (-1)
/**
 @brief Time Sync module is unable to create a Semaphore
 */
#define TIME_SYNC_UNABLE_TO_CREATE_SEMAPHORE                                (-2)
/**
 @brief Time Sync module is unable to create an interrupt
 */
#define TIME_SYNC_UNABLE_TO_CREATE_INTERRUPT                                (-3)
/**
 @brief Time Sync module is unable to allocate memory
 */
#define TIME_SYNC_UNABLE_TO_ALLOC_MEM                                       (-4)
/**
 @brief Time Sync module is unable to create timer
 */
#define TIME_SYNC_UNABLE_TO_CREATE_CLOCK                                    (-5)
/**
 @brief Time Sync module is unable to create a mailbox
 */
#define TIME_SYNC_UNABLE_TO_CREATE_MAILBOX                                  (-6)
/**
 @brief Time Sync module is unable to create an event
 */
#define TIME_SYNC_UNABLE_TO_CREATE_EVENT                                    (-7)
/**
 @brief Time Sync unsupported format
 */
#define TIME_SYNC_UNSUPPORTED_FORMAT                                        (-8)
/**
 @brief Time Sync not able to initialize EDMA
 */
#define TIME_SYNC_EDMA_INIT_FAILED                                          (-9)
/**
 @brief Time Sync feature disabled
 */
#define TIME_SYNC_FEATURE_NOT_ENABLED                                       (-10)
/**
 @brief Time Sync handle uninitialized
 */
#define TIME_SYNC_HANDLE_NOT_INITIALIZED                                    (-11)


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**
 * @def DEFAULT_PDELAY_REQ_LOG_INTERVAL
 *      Default PDelay Request packet interval. Actual value is 2^value seconds
 */
#define DEFAULT_PDELAY_REQ_LOG_INTERVAL         3
/**
 * @def DEFAULT_SYNC_SEND_LOG_INTERVAL
 *      Default Sync transmit interval. Actual value is 2^value seconds
 */
#define DEFAULT_SYNC_SEND_LOG_INTERVAL          0
/**
 * @def DEFAULT_ANNOUNCE_SEND_LOG_INTERVAL
 *      Default Announce packet transmit interval. Actual value is 2^value seconds
 */
#define DEFAULT_ANNOUNCE_SEND_LOG_INTERVAL      1
/**
 * @def DEFAULT_ANNOUNCE_TIMEOUT_LOG_INTERVAL
 *      Default Announce packet timeout. Actual value is 2^value seconds
 */
#define DEFAULT_ANNOUNCE_TIMEOUT_LOG_INTERVAL   3

/**
 * @def TIMESYNC_SYNC_ISR_MASK_P1
 *      Interrupt mask for PRU event 22
 */
#define TIMESYNC_SYNC_ISR_MASK_P1               0x400000

/**
 * @def TIMESYNC_SYNC_ISR_MASK_P2
 *      Interrupt mask for PRU event 25
 */
#define TIMESYNC_SYNC_ISR_MASK_P2               0x2000000
/**
 * @def TIMESYNC_LATCH0_ISR_MASK
 *      Interrupt mask for LATCH0 input
 */
#define TIMESYNC_LATCH0_ISR_MASK                0x1000
/*
 * @def TIMESYNC_TX_TS_ISR_MASK_P1
 *      Interrupt mask for PRU event 23
 */
#define TIMESYNC_TX_TS_ISR_MASK_P1              0x800000
/**
 * @def TIMESYNC_TX_TS_ISR_MASK_P2
 *      Interrupt mask for PRU event 24
 */
#define TIMESYNC_TX_TS_ISR_MASK_P2              0x1000000

/**
 * @def PTP_SYNC_MSG_ID
 *      Sync message ID value
 */
#define PTP_SYNC_MSG_ID                     0x00
/**
 * @def PTP_DLY_REQ_MSG_ID
 *      Delay request message ID value
 */
#define PTP_DLY_REQ_MSG_ID                  0x01
/**
 * @def PTP_PDLY_REQ_MSG_ID
 *      PDelay request message ID value
 */
#define PTP_PDLY_REQ_MSG_ID                 0x02
/**
 * @def PTP_PDLY_RSP_MSG_ID
 *      PDelay response message ID value
 */
#define PTP_PDLY_RSP_MSG_ID                 0x03
/**
 * @def PTP_FOLLOW_UP_MSG_ID
 *      Follow up message ID value
 */
#define PTP_FOLLOW_UP_MSG_ID                0x08
/**
 * @def PTP_DLY_RESP_MSG_ID
 *      Delay response message ID value
 */
#define PTP_DLY_RESP_MSG_ID                 0x09
/**
 * @def PTP_PDLY_RESP_FLW_UP_MSG_ID
 *      PDelay response follow up message ID value
 */
#define PTP_PDLY_RESP_FLW_UP_MSG_ID         0x0A
/**
 * @def PTP_ANNOUNCE_MSG_ID
 *      Announce message ID value
 */
#define PTP_ANNOUNCE_MSG_ID                 0x0B
/**
 * @def PTP_MGMT_MSG_ID
 *      Management message ID value
 */
#define PTP_MGMT_MSG_ID                     0x0D

/**
 * @def PTP_FLW_UP_CTRL_MSG_ID
 *      Control message ID value for Follow Up
 */
#define PTP_FLW_UP_CTRL_MSG_ID              0x02

/**
 * @def SEC_TO_NS
 * Value of seconds in nanoseconds. Useful for calculations
 */
#define SEC_TO_NS                           1000000000

/**
 * @def FILTER_ALPHA_COEFF
 *      Alpha coefficient for sync interval exponential filter and other filters
 */
#define FILTER_ALPHA_COEFF                          0.85
/**
 * @def FILTER_ALPHA_COEFF_TOD
 *      Alpha coefficient for the filter used in Time of Day synchronization
 */

#define FILTER_ALPHA_COEFF_TOD                      0.75

/**
 * @def PTP_FOLLOW_UP_TASK_PRIORITY
 *      Priority of task which sends Follow Up frames
 */
#define SYNC_FOLLOW_UP_BUFFER_SIZE                  64

/**
 * @def PTP_SYNC0_PERIOD_DIVIDER
 *      CMP1 period divided by this number makes the width of the Sync pulse
 */
#define PTP_SYNC0_PERIOD_DIVIDER        4

/**
 * @def RX_PHY_LATENCY
 *      Rx PHY Latency in nano seconds. This is the known delay for TLK110
 */
#define RX_PHY_LATENCY                  186
/**
 * @def TX_PHY_LATENCY
 *      Tx PHY latency in nano seconds
 */
/* Since there is no corresponding SFD timestamp for Tx
 * we add a fixed delay of 640ns to the known SOF Tx timestamp.
 * 640 (1 byte delay to get SFD) + 86 (PHY delay) = 726ns */
#define TX_PHY_LATENCY                  726

/**
 * @def OFFSET_THRESHOLD_FOR_RESET
 *      If offset from Master goes above this threshold it will
 *      trigger a reset. Value is in nanoseconds
 */
#define OFFSET_THRESHOLD_FOR_RESET     1000

/**
 * @def STABLE_FILTER_THRESHOLD
 *      When clock drift goes below this value it indicates drift has
 *      stabilized and SMA filter can kick in
 */
#define STABLE_FILTER_THRESHOLD         100

/**
 * @def TIMESYNC_PEER_DELAY_ERROR_THRESHOLD
 *      When peer delay exceeds this value it gets reset to 0
 *      Sometimes wrong values are calculated when doing peer delay
 *      calculation when clock on peer changes in between peer delay
 *      messages and because of the filter this value stays for a long time
 *      This helps in fixing it. This is set to a value much higher than
 *      max peer delay (of a 100 mtr cable)
 */
#define TIMESYNC_PEER_DELAY_ERROR_THRESHOLD     10000

/**
 * @def TIMESYNC_PEER_DELAY_ERROR_THRESHOLD
 *      When line delay exceeds this value it gets reset to 0
 *      Sometimes wrong values are calculated when doing line delay
 *      calculation when master and slave clocks are synchronizing,
 *      this check prevents going into a state which results in runaway drift
 *      value is set much higher than max cable delay (of a 100 mtr cable)
 */
#define TIMESYNC_LINE_DELAY_ERROR_THRESHOLD     10000

/**
 * @def NUM_SYNC_MISSED_THRESHOLD
 *      If this many consecutive sync frames are missed
 *      the DUT gets reset
 */
#define NUM_SYNC_MISSED_THRESHOLD       3

/**
 * @def AVG_NUM_DELAY_MEASUREMENTS
 *      Number of values in the running average
 */
#define AVG_NUM_DELAY_MEASUREMENTS      3

/*Hex Offsets for different fields in PTP Delay Req/Res Packet. Annex E*/
/*To get Annex F offsets subtract ANNEX_D_ANNEX_F_DIFF from it*/

/**
 * @def SRC_MAC_OFFSET
 *      Source MAC address offset
 */
#define SRC_MAC_OFFSET              6
/**
 * @def SRC_IP_OFFSET
 *      Source IP address offset in IP4/UDP
 */
#define SRC_IP_OFFSET               26
/**
 * @def DST_IP_OFFSET
 *      Destination IP address offset in IP4/UDP
 */
#define DST_IP_OFFSET               30
/**
 * @def IP_CHKSUM_OFFSET
 *      IP4 checksum offset in IP4/UDP
 */
#define IP_CHKSUM_OFFSET            40
/**
 * @def PTP_MSG_ID_OFFSET
 *      PTP message ID offset
 */
#define PTP_MSG_ID_OFFSET           42
/**
 * @def PTP_DOMAIN_NUM_OFFSET
 *      PTP domain number offset
 */
#define PTP_DOMAIN_NUM_OFFSET       46
/**
 *
 * Offset for PTP flags (including 2 step)
 */
#define PTP_FLAG_OFFSET             48
/**
 *  @def PTP_CORRECTION_OFFSET
 *      Offset for PTP correction field
 */
#define PTP_CORRECTION_OFFSET       50
/**
 * @def PTP_SRC_CLK_IDENTITY
 *      Offset for PTP source clock identity (MAC + protocol id)
 */
#define PTP_SRC_CLK_IDENTITY        62
/**
 * @def PTP_SRC_PORT_ID_OFFSET
 *      Offset source port id
 */
#define PTP_SRC_PORT_ID_OFFSET      70
/**
 * @def PTP_SEQ_ID_OFFSET
 *      Offset for sequence identifier (incremented every frame)
 */
#define PTP_SEQ_ID_OFFSET           72
/**
 * @def PTP_CONTROL_MSG_ID_OFFSET
 *      Offset for message id. This categorizes frames into two groups
 *      time critical and non time critical
 */
#define PTP_CONTROL_MSG_ID_OFFSET   74
/**
 * @def PTP_LOG_MSG_PERIOD
 *      Log message interval offset
 */
#define PTP_LOG_MSG_PERIOD          75
/**
 * @def PTP_REQ_RCPT_TS_SEC_OFFSET
 *      Offset for seconds timestamp
 */
#define PTP_REQ_RCPT_TS_SEC_OFFSET  76
/**
 * @def PTP_REQ_RCPT_TS_NSEC_OFFSET
 *      Offset for nanoseconds timestamp
 */
#define PTP_REQ_RCPT_TS_NSEC_OFFSET 82
/**
 * @def PTP_REQ_SRC_PORT_IDENTITY
 *      Offset for requestor clock identity
 */
#define PTP_REQ_SRC_PORT_IDENTITY   86
/**
 * @def PTP_REQ_SRC_PORT_ID
 *      Offset for requestor port identity
 */
#define PTP_REQ_SRC_PORT_ID         94

/**
 * @def PTP_UTC_OFFSET
 *      Offset for UTC offset in Announce frame
 */
#define PTP_UTC_OFFSET              86

/**
 * @def PTP_PRIORITY1_OFFSET
 *      Offset for Priority 1 field in Announce frame
 */
#define PTP_PRIORITY1_OFFSET        89

/**
 * @def PTP_GM_CLK_CLASS_OFFSET
 *      Offset for GM clock class field in Announce frame
 */
#define PTP_GM_CLK_CLASS_OFFSET     90

/**
 * @def PTP_GM_CLK_ACCU_OFFSET
 *      Offset for GM clock accuracy field in Announce frame
 */
#define PTP_GM_CLK_ACCU_OFFSET          91

/**
 * @def PTP_GM_CLK_VARIANCE_OFFSET
 *      Offset for GM clock variance field in Announce frame
 */
#define PTP_GM_CLK_VARIANCE_OFFSET      92

/**
 * @def PTP_PRIORITY2_OFFSET
 *      Offset for priority 2 field in Announce frame
 */
#define PTP_PRIORITY2_OFFSET            94

/**
 * @def PTP_GM_CLK_IDENTITY_OFFSET
 *      Offset for GM clock identity field in Announce frame
 */
#define PTP_GM_CLK_IDENTITY_OFFSET      95

/**
 * @def PTP_STEPS_REMOVED_OFFSET
 *      Offset for steps removed field in Announce frame
 */
#define PTP_STEPS_REMOVED_OFFSET        103

/**
 * @def PTP_TIME_SRC_OFFSET
 *      Offset for GM time source field in Announce frame
 */
#define PTP_TIME_SRC_OFFSET             104

/**
 * @def PTP_E2E_BUFFER_OFFSET
 *      Offset for start of PTP data in UDP/IP4 frame
 */
#define PTP_E2E_BUFFER_OFFSET           42
/**
 * @def PTP_P2P_BUFFER_OFFSET
 *      Offset for start of PTP data in 802.3 frame
 */
#define PTP_P2P_BUFFER_OFFSET           14

/**
 * @def ANNEX_D_ANNEX_F_DIFF
 *      Number of Extra bytes from Annex F(802.1) to Annex E(UDP)
 */
#define ANNEX_D_ANNEX_F_DIFF            28
/**
 * @def HSR_CORRECTION
 *      Number of bytes in HSR Header
 */
#define HSR_CORRECTION                  6

/**
 * @def SRC_DST_MAC_SIZE
 *      Num bytes in source plus destination MAC
 */
#define SRC_DST_MAC_SIZE                12

/**
 * @def PTP_TWO_STEP_MASK
 *      Mask used to extract two step bit value
 */
#define PTP_TWO_STEP_MASK               0x200

/**
 * @def SYNC_INTERVAL_PERIOD_CHANGE_THRESHOLD
 *      If current sync interval varies from previously measured sync
 *      interval by this much percentage then it triggers a PTP reset
 */
#define SYNC_INTERVAL_PERIOD_CHANGE_THRESHOLD   60

/**
 * @def GPTP_NUM_DOMAINS
 *      Number of domains supported by GPTP implementation
 */
#define GPTP_NUM_DOMAINS                        2

/**
* @defgroup PTP_API PTP/1588 APIs
*/

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 * @brief Resets CMP1 once first adjustment is done, else 1PPS signal doesn't trigger
 * @param timeSyncHandle pointer to PTP Handle structure
 * @return None
 */
void TimeSync_resetIEP(TimeSync_ParamsHandle_t timeSyncHandle);
/**
 * @brief Do initial adjustment for the IEP based on sync timestamp
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param portNum ICSS_EMAC_PORT_1 or ICSS_EMAC_PORT_2
 * @return None
 */
void TimeSync_doFirstAdjustment(TimeSync_ParamsHandle_t timeSyncHandle,
                                uint8_t portNum);

/**
 * @brief Enable PTP memory for all structures
 * @param timeSyncHandle pointer to PTP Handle structure
 * @return 0/1 sucess/fail
 */
uint8_t TimeSync_allocMem(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @brief Callback for link status change on Port 1
 * @param linkStatus 1/0 whether up or down
 * @param arg2
 *
 * @return arg2 timeSyncHandle pointer to PTP Handle structure
 */
void TimeSync_Port1linkResetCallBack(uint8_t linkStatus, void *arg2);

/**
 * @brief Callback for link status change on Port 2
 * @param linkStatus 1/0 whether up or down
 * @param arg2
 *
 * @return arg2 timeSyncHandle pointer to PTP Handle structure
 */
void TimeSync_Port2linkResetCallBack(uint8_t linkStatus, void *arg2);

/**
 *  @brief  Reset the state machine in firmware to restart synchronization (First adjustment happens again)
 *
 *          If Sync interval changes or link break happens or there is a large adjustment in time
 *          this function is called.
 *
 *  @param  timeSyncHandle pointer to PTP Handle structure
 *
 *  @retval None
 *
 */
void TimeSync_reset(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @brief Processes a PTP Announce message and extract fields
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param pktBuffer pointer to packet data
 * @param portNum port number on which the packet was received
 * @param size size of the packet
 * @param isLinkLocal
 *
 * @return none
 */
void TimeSync_processPTPFrame(TimeSync_ParamsHandle_t timeSyncHandle,
                              uint8_t *pktBuffer, \
                              uint8_t portNum, uint16_t size, uint8_t isLinkLocal);

/**
 * @brief Return True/False 1/0 if timeSync module is enabled
 * @param timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
uint8_t TimeSync_isEnabled(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @brief Write the MAC ID of PTP master to Firmware. BMC Algorithm calls this API
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param parentMac Pointer to MAC ID of master.
 * @return none
 */
void TimeSync_updateParentAddress(TimeSync_ParamsHandle_t timeSyncHandle,
                                  uint8_t *parentMac);
/**
 * @brief Retuns the previous master's MAC ID. BMC Algorithm calls this.
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param prevMac Pointer to MAC ID of previous master.
 * @return none
 */
void TimeSync_getPrevAddress(TimeSync_ParamsHandle_t timeSyncHandle,
                             uint8_t *prevMac);
/**
 * @brief Copies the PTP announce message data to PTP stack buffer.
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param buff Pointer to buffer of PTP Stack.
 * @return none
 */
void TimeSync_getGeneralMessage(TimeSync_ParamsHandle_t timeSyncHandle,
                                int8_t *buff);
/**
 * @brief Performs PHY delay correction on Rx timestamp
 * Assumption : The function expects the timestamp in rxTimestamp_gPTP
 * field on which it does correction
 * @param timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_rxPhyDelayCorrection(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @brief When application receives the Time of Day information, it calls this API
 * with the seconds and nanoseconds fields. This starts the time of day synchronization
 * mechanism.
 * Assumption : Latch0 input is also enabled and is being fed by 1PPS output
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param nanoseconds Nanoseconds component of time of day
 * @param seconds Seconds component of time of day
 * @return Error value
 */
int8_t TimeSync_processTimeOfDay(TimeSync_ParamsHandle_t timeSyncHandle,
                                 uint32_t nanoseconds, uint64_t seconds);

/**
 * @internal
 * @brief Get the timestamp of transmitted frame and process if required
 * This is called by both PTP_TxTSTask_P1 and PTP_TxTSTask_P2 tasks with port number as input
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @param  portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @param frameType Whether Sync, Delay req or Delay response
 * @return none
 */
void TimeSync_getTxTS(TimeSync_ParamsHandle_t timeSyncHandle, uint8_t portNum,
                      ptpFrameTypes_t frameType);

/**
 * @internal
 * @brief Calculate Line Delay given the parameters from Delay Response
 * This is called from inside the interrupt context
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_lineDelayCalc(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @internal
 * @brief Calculate Peer Delay given the parameters from Peer Delay Req/Res
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param twoStep Indicates if calculation is through 2 step mechanism
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_peerDelayCalc(TimeSync_ParamsHandle_t timeSyncHandle,
                            uint8_t twoStep, uint8_t portNum);
/**
 * @internal
 * @brief Process Sync/Follow Up frames and do synchronization
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param buff pointer to the sync and follow up frame
 * @param followUp Set to 1 if it's a follow up frame else 0
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @param size size of the frame
 * @return none
 */
void TimeSync_processSyncFrame(TimeSync_ParamsHandle_t timeSyncHandle,
                               uint8_t *buff, \
                               uint8_t followUp, uint8_t portNum, uint16_t size);
/**
 * @internal
 * @brief Process Pdelay response frame
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param buff pointer to the Pdelay Response Frame
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_processDelayResFrame(TimeSync_ParamsHandle_t timeSyncHandle,
                                   uint8_t *buff, \
                                   uint8_t portNum);
/**
 * @internal
 * @brief Process Pdelay request frame
 * @param timeSyncHandle pointer to PTP Handle structure
 * @param buff pointer to the Pdelay Request Frame
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_processPdelayReqFrame(TimeSync_ParamsHandle_t timeSyncHandle,
                                    uint8_t *buff, \
                                    uint8_t portNum);
/**
 * @internal
 * @brief Process Pdelay response and Pdelay response followUp frame
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @param buff pointer to the Pdelay Response/Response Follow Up Frame
 * @param followUp 0 if it's a response frame, 1 if response follow up
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_processPdelayRespFrame(TimeSync_ParamsHandle_t timeSyncHandle,
                                     uint8_t *buff, \
                                     uint8_t followUp, uint8_t portNum);
/**
 * @internal
 * @brief Called for forced 2-step mode. Sends out follow up frame with bridge delay
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_forced2StepBDCalc(TimeSync_ParamsHandle_t timeSyncHandle,
                                uint8_t portNum);
/**
 * @internal
 * @brief Called internally to process latch0 timestamp and synchronize internal clock
 * to the rising edge of 1PPS signal
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_synchronizeToD(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @internal
 * @brief Synchronize clock upon receipt of Sync Frame. Applicable for OC
 * This is called from inside the interrupt context
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_synchronizeClock(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @internal
 * @brief Calculate Neighbor rate ratio for adjacent nodes
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_calcNRR(TimeSync_ParamsHandle_t timeSyncHandle, uint8_t portNum);
/**
 * @internal
 * @brief Updates NRR info which is subsequently used to calculate
 * the final NRR value
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @param portNum port number. ICSS_EMAC_PORT_1/ICSS_EMAC_PORT_2
 * @return none
 */
void TimeSync_updateNRRParams(TimeSync_ParamsHandle_t timeSyncHandle,
                              uint8_t portNum);
/**
 * @internal
 * @brief This is similar to TimeSync_calcRcfAndSyncInterval, instead
 * of sync packet interval it calculates interval between latch0 triggers.
 * And there is no RCF calculation.
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_calcSyncInterval_ToD(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @internal
 * @brief Calculate rcf for syntonization from line delay parameters
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_calcRcfAndSyncInterval(TimeSync_ParamsHandle_t timeSyncHandle);

/**
 * @internal
 * @brief For single step master, computation of Tx seconds and nanoseconds timestamp requires
 * some techniques to avoid a long division in firmware. Logic is inside the definition
 * @param  timeSyncHandle pointer to PTP Handle structure
 * @return none
 */
void TimeSync_writeTS_SingleStep_Sync(TimeSync_ParamsHandle_t timeSyncHandle,
                                      uint8_t portNum);

void TimeSync_dummyBMCA(TimeSync_ParamsHandle_t timeSyncHandle,
                        uint8_t *pktBuffer);


uint64_t getIEPTimestamp(TimeSync_ParamsHandle_t timeSyncHandle);

/** @} */

#endif /* ICSS_TIMESYNC_H_ */
