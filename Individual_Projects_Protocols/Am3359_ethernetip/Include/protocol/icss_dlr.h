/**
* @file  icss_dlr.h
*
* @brief Include file for icss_dlr.c
*
*\par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*\par
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

#ifndef ICSS_DLR_H_
#define ICSS_DLR_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drv/icss_emac/icss_emac_ver.h>

#if ICSS_EMAC_DRV_VERSION_ID ==(0x01000002)
#include <ti/drv/icss_emac/src/icss_emacSwitch.h>
#else
#include <ti/drv/icss_emac/src/icss_emacLoc.h>
#endif
#include <ti/drv/icss_emac/icss_emacCommon.h>
#include <ti/drv/icss_emac/icss_emacLearning.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>

#include <Include/protocol/icss_switch_dlr.h>
#include <Include/protocol/icss_dlrDef.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*#define DLR_DEBUG*/

/**For future use when Ring Supervisor is implemented*/
/*#define IS_A_DLR_SUPERVISOR*/

#ifdef TEST_DEBUG
#include "testing.h"
#endif

#include <Include/protocol/icss_eip_driverDef.h>


#define ETHERNET_FRAME_SIZE_60 60

/**Value written to IEP WD Predivider register to configure 1us increment*/
#define IEP_WD_PRE_DIV_10US     2000

/**ID for PD Watchdog in IEP*/
#define PORT0_WATCH_DOG_ID   0      /*PD_WD*/
/**ID for PDI Watchdog in IEP*/
#define PORT1_WATCH_DOG_ID   1      /*PDI_WD*/

#define IS_A_LINK_STATUS_FRAME      DLR_TRUE
#define IS_A_NEIGHBOR_STAT_FRAME    DLR_FALSE

/*Generic flags*/
#define BOTH_LINKS_UP           0x0
#define PORT0_IS_DOWN           0x2
#define PORT1_IS_DOWN           0x1
#define BOTH_LINKS_DOWN         0x3

/*PRUSS INTC Mask for PRU EVENT 5*/
#define PORT0_WD_ISR_MASK       0x2000000
/*PRUSS INTC Mask for PRU EVENT 5*/
#define PORT1_WD_ISR_MASK       0x4000000

/**
* @def PDI_WD_TRIGGER_RX_SOF
*  Watchdog RX Start of Frame Trigger
*/
#define PDI_WD_TRIGGER_RX_SOF       (0 << 4)

/**
* @def PDI_WD_TRIGGER_LATCH_IN
*  Watchdog LATCH IN Trigger
*/
#define PDI_WD_TRIGGER_LATCH_IN     (1 << 4)

/**
* @def PDI_WD_TRIGGER_SYNC0_OUT
*  Watchdog SYNC0 Trigger
*/
#define PDI_WD_TRIGGER_SYNC0_OUT    (2 << 4)

/**
* @def PDI_WD_TRIGGER_SYNC1_OUT
*  Watchdog SYNC1 Trigger
*/
#define PDI_WD_TRIGGER_SYNC1_OUT    (3 << 4)

#ifdef DLR_DEBUG
#define MAX_EVENTS_CAPTURED     1000

/*DLR events for debugging*/
#define LINK1_BREAK     1
#define LINK2_BREAK     2
#define BOTH_LINK_DISABLED  3
#define EMPTY_SIGNON_FRAME_RCVD 4
#define COMPLETE_SIGNON_FRAME_RCVD  5

#define RING_FAULT_RCVD_PORT0   6
#define RING_FAULT_RCVD_PORT1   7

#define RING_FAULT_TRANSITION_PORT0 8
#define RING_FAULT_TRANSITION_PORT1 9

#define RING_NORMAL_TRANSITION_PORT0    10
#define RING_NORMAL_TRANSITION_PORT1    11

#define START_TIMER0            12
#define START_TIMER1            13

#define NCRES_RCVD_PORT0        14
#define NCRES_RCVD_PORT1        15

#define NCREQ_RCVD_PORT0        16
#define NCREQ_RCVD_PORT1        17

#define LOCFAULT_RCVD_PORT0     18
#define LOCFAULT_RCVD_PORT1     19

#define BEACON0_MISSED_FAULT    20
#define BEACON1_MISSED_FAULT    21
#define BEACON0_MISSED_NORMAL   22
#define BEACON1_MISSED_NORMAL   23

#define LINK1_FAULT_BREAK   24
#define LINK2_FAULT_BREAK   25

#define LINK1_NORMAL_BREAK      27
#define LINK2_NORMAL_BREAK      28

#define NEIGHBOR_TIMEOUT_PORT0_RETRY    29
#define NEIGHBOR_TIMEOUT_PORT1_RETRY    30
#define NEIGHBOR_TIMEOUT_PORT0_MAX      31
#define NEIGHBOR_TIMEOUT_PORT1_MAX      32

#define DLR_RESET_MACHINE   33

#define BEACON0_MISSED  34
#define BEACON1_MISSED  35

#define STOP_BOTH_TIMERS_PORT0 36
#define STOP_BOTH_TIMERS_PORT1 37

#define PORT0_BEACON_STALL   38
#define PORT1_BEACON_STALL   39
#endif

/**DLR Timer default values, this is redundant since values are eventually read from supervisor*/
#define DEFAULT_BEACON_INTERVAL_VARIABLE 400 /*in microseconds*/
#define DEFAULT_BEACON_TIMEOUT_VARIABLE (DEFAULT_BEACON_INTERVAL_VARIABLE * 4) /*in microseconds*/

/**The actual time is 100ms but since system tick period is half, this is twice of what it should be*/
#define DEFAULT_NEIGHBOR_TIMEOUT_INTERVAL   200 /*in milliseconds*/

/**The actual time is 100ms but since system tick period is half, this is twice of what it should be*/
#define DEFAULT_DLR_PERIODIC_INTERVAL   100 /*in milliseconds*/

/**Number of neighbor retries before a neighbor status message is sent*/
#define MAX_NUM_RETRIES     3

/**Number of times beacon timer is triggered before it is disabled*/
#define BEACON_CPU_STALL_THRESHOLD  4

/**the value 1 indicates that this is only a
 * ring node capable device, for other values see below*/

#define DLR_DEFAULT_CAPABILITIES    (1 << 7) | (1 << 1)

/*DLR Packet Generation Offsets*/
#define DLR_SIGNON_FRAME_SIZE           ICSS_EMAC_MAXMTU

#define DLR_COMMON_FRAME_HEADER_SIZE    18
#define DLR_COMMON_FRAME_OFFSET         12

/**DLR port 0 Interrupt Flag for ICSS INTC*/
#define ICSS_DLR_PORT0_INT_FLAG              0x200000
/**DLR port 1 Interrupt Flag for ICSS INTC*/
#define ICSS_DLR_PORT1_INT_FLAG              0x400000

/**Generic DM Timer 4 ID used for timer creation*/
#define DLR_DMTIMER4_ID     3
/**Generic DM Timer 4 ID used for timer creation*/
#define DLR_DMTIMER5_ID     4

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/** @addtogroup EIP_DLR
 @{ */

/**
*  @brief  API to initialize the DLR driver
*
*          Initializes variables and timers & clocks, call once at the beginning
*
*  @param  dlrHandle [in] DLR driver handle
*
*  @retval  none
*
*/
void EIP_DLR_init(EIP_DLRHandle dlrHandle);
/**
*  @brief  API to start the DLR driver
*          Calling this enables DLR on the device
*
*  @param  dlrHandle [in] DLR driver handle
*
*  @retval  none
*
*/
void EIP_DLR_start(EIP_DLRHandle dlrHandle) ;
/**
*  @brief  API to stop the DLR driver
*          Halt DLR. Calling this disables DLR on the device
*
*  @param  dlrHandle [in] DLR driver handle
*
*  @retval  none
*
*/
void EIP_DLR_stop(EIP_DLRHandle dlrHandle);
/**
*  @brief  Fast ISR for Port 0, bypasses the buffer copy and NDK
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_port0ISR(UArg arg);
/**
*  @brief  Fast ISR for Port 1, bypasses the buffer copy and NDK
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_port1ISR(UArg arg);
/**
*  @brief  ISR for beacon timeout for Port 0
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_beaconTimeoutISR_P0(UArg arg);

/**
*  @brief  ISR for beacon timeout for Port 1
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_beaconTimeoutISR_P1(UArg arg);

/**
*  @brief Process DLR state machine in the event of a link break on Port0
*
*  @param  linkStatus [in] link status of the port. Up/Down. 1/0
*  @param  arg2       [in] argument DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_port0ProcessLinkBrk(uint8_t linkStatus, void *arg2);
/**
*  @brief Process DLR state machine in the event of a link break on Port1
*
*  @param  linkStatus [in] link status of the port. Up/Down. 1/0
*  @param  arg2       [in] argument DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_port1ProcessLinkBrk(uint8_t linkStatus, void *arg2);
/**
*  @brief ISR for Neighbor timeout timer for port 0
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_neighborTimeoutISR0(UArg arg);
/**
*  @brief ISR for Neighbor timeout timer for port 1
*
*  @param  arg [in] user argument. DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_neighborTimeoutISR1(UArg arg);
/**
*  @internal
*  @brief   Add the new VLAN ID to the message
*
*  @param  src    [in] pointer to the packet
*  @param  vlanID [in] VLAN ID of supervisor
*
*  @retval  none
*
*/
void EIP_DLR_addVlanID(uint8_t *src, uint16_t vlanID);
/**
*  @internal
*  @brief   Generates a Neighbor check request frame
*
*           When a node receives a locate fault request from the supervisor
*           it sends neighbor check request frames out of both ports to get the
*           status of it's neighbors, if the neighbor doesn't respond it sends
*           a link status frame to the supervisor
*
*  @param  dlrHandle  [in] DLR handle
*  @param  src        [in] pointer to the packet
*  @param  sourcePort [in] port on which the frame will be sent
*
*  @retval  none
*
*/
void EIP_DLR_genNCReqFrame(EIP_DLRHandle dlrHandle, uint8_t *src,
                           uint8_t sourcePort);
/**
*  @internal
*  @brief   Generates a Neighbor check response frame
*
*           A neighbor check response frame is sent in response to a
*           neighbor request frame
*
*  @param  src        [in] pointer to the packet
*  @param  sourcePort [in] port on which the frame will be sent
*  @param  reqSrcPort [in] request source port of the neighbor check request
*  @param  sequenceId [in] sequence Id of the neighbor check request
*
*  @retval  none
*
*/
void EIP_DLR_genNCResFrame(uint8_t *src, uint8_t sourcePort, uint8_t reqSrcPort,
                           uint32_t sequenceId);
/**
*  @internal
*  @brief   Generates a Link/Neighbor status frame
*
*           A neighbor check response frame is sent in response to a
*           neighbor request frame
*
*  @param  dlrHandle      [in] DLR handle
*  @param  src            [in] pointer to the packet
*  @param  sourcePort     [in] port on which the frame will be sent
*  @param  linkOrNeighbor [in] whether the packet is a link status or a neighbor status frame
*  @param  linkStatus     [in] indicates which link is down. Possible values are DLR_PORT0_FAIL or DLR_PORT1_FAIL
*
*  @retval  none
*
*/
void EIP_DLR_genNeighborLinkStatFrame(EIP_DLRHandle dlrHandle, uint8_t *src,
                                      uint8_t sourcePort,
                                      uint8_t linkOrNeighbor, uint8_t linkStatus);
/**
*  @internal
*  @brief Copies sequence id into byte stream in big-endian byte order
*
*  @param  src    [in] Byte stream pointer
*  @param  header [in] Byte stream header pointer
*
*  @retval  none
*
*/
void EIP_DLR_initDLRFrameHeader(uint8_t *src, uint8_t *header);

/**
*  @internal
*  @brief   Add number of nodes to the sign on frame
*
*  @param  src      [in] pointer to stream where IP Addr is to be added
*  @param  numNodes [in] number of nodes in the ring
*
*  @retval  none
*
*/
void EIP_DLR_addSignOnNumNodes(uint8_t *src, uint16_t numNodes);
/**
*  @brief Processes a sign on and Neighbor check request frame
*
*  @param  dlrHandle [in] DLR handle
*  @param  pktBuffer [in] pointer to bytestream
*  @param  portNum   [in] Port number wherer the packet arrived
*  @param  size      [in] size of packet
*
*  @retval  none
*
*/
void EIP_DLR_processDLRFrame(EIP_DLRHandle dlrHandle, uint8_t *pktBuffer,
                             uint8_t portNum, uint16_t size);
/**
*  @internal
*  @brief  Initialize DLR control variables in the DRAM
*
*          This is defined separately because this needs to be called as part of
*           the switch initialization process after PRU code has been loaded
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_dRAMInit(EIP_DLRHandle dlrHandle);
/**
*  @brief  Initialize the state machine when it goes back to idle state
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_resetStateMachine(EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief  Set Default values for the DLR CIP Object
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_setDefaultValue(EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief   Set CIP Object values for fault status
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_switchToFault(EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief   Set CIP Object values for normal status
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_switchToNormal(EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief  Initialize DLR ISR's
*
*  @param  dlrHandle [in] DLR handle
*
*  @retval
*
*/
int32_t EIP_DLR_isrInit(EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief   This is called periodically every 100ms or whatever the NDK tick period is
*
*  @param userArg  [in] DLR handle is passed to this parameter
*
*  @retval
*
*/
void EIP_DLR_periodicProcessing(UArg userArg);

#ifdef DLR_DEBUG
void genSeqOfEvents(uint8_t event);
#endif
/**
*  @internal
*  @brief Add a macId to the exception list
*
*  @param dlrHandle [in] DLR handle
*  @param macId     [in] pointer to MAC ID
*
*  @retval none
*
*/
void EIP_DLR_addToExceptionList(EIP_DLRHandle dlrHandle, uint8_t *macId);
/**
*  @internal
*  @brief    Add a macId to the exception list
*
*  @param dlrHandle [in] DLR handle
*
*  @retval none
*
*/
void EIP_DLR_clearExceptionList(EIP_DLRHandle dlrHandle);

/**
*  @internal
*  @brief
*
*  @param macId     [in] pointer to MAC ID
*  @param dlrHandle [in] DLR handle
*
*  @retval
*
*/
uint8_t EIP_DLR_checkSupervisorException(uint8_t *macId,
        EIP_DLRHandle dlrHandle);
/**
*  @internal
*  @brief  Add a IP address to DLR
*
*  @param dlrHandle  [in] DLR handle
*  @param newIP      [in] IP Address in unsigned int 32 format
*
*  @retval  none
*
*/
void EIP_DLR_addModuleIPAddress(EIP_DLRHandle dlrHandle, uint32_t newIP);

/**
*  @brief  Sets the clock divider to 1us for IEP watch dog timers
*
*  @param dlrHandle  [in] DLR handle
*
*  @retval  none
*
*/
void EIP_DLR_setDivider_WD_IEP(EIP_DLRHandle dlrHandle);

/**
*  @brief  Enable the IEP Watch dog timers
*
*  @param dlrHandle  [in] DLR handle
*  @param id         [in] 0/1 for WD_PD and WD_PDI respectively
*
*  @retval  none
*
*/
void EIP_DLR_enable_WD_IEP(EIP_DLRHandle dlrHandle, uint8_t id);

/**
*  @brief  Disable the IEP Watch dog timers
*
*  @param dlrHandle  [in] DLR handle
*  @param id         [in] 0/1 for WD_PD/WD_PDI respectively
*
*  @retval  none
*
*/
void EIP_DLR_disable_WD_IEP(EIP_DLRHandle dlrHandle, uint8_t id);

/**
*  @brief  Set the timeout value in watchdog
*
*  @param dlrHandle  [in] DLR handle
*  @param periodInMicroSec [in] Timeout value in microseconds
*  @param id [in] watch dog ID
*
*  @retval  none
*
*/
void EIP_DLR_setTimeout_WD_IEP(EIP_DLRHandle dlrHandle,
                               uint16_t periodInMicroSec, uint8_t id);
/**
*  @brief  Set the PDI WD trigger mode
*
*  @param dlrHandle  [in] DLR handle
*  @param mode       [in] mode to be set in DGIO ctrl reg
*
*  @retval  none
*
*/
void EIP_DLR_set_pdi_wd_trigger_mode(EIP_DLRHandle dlrHandle, uint32_t mode);

/**
@}
*/
#endif /* ICSS_DLR_H_ */
