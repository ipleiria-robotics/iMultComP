/**
 * @file iPtcpDrv.h
 *
 * \par
 * * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 * \par
 */
#ifndef IPTCPDRV_H_
#define IPTCPDRV_H_

#include <ti/drv/icss_emac/icss_emacDrv.h>
/* ========================================================================== */
/*                          Doxygen                                           */
/* ========================================================================== */
/**
 * @defgroup PN_PTCP PTCP APIs
 */

/**
 * @page PN_PTCP_PAGE PTCP
 * @tableofcontents
 * @section PTCP_INTRO Introduction
 * The API definitions can be found here @ref PN_PTCP
 * @section PTCP_IMPLEMENTATION Implementation
 * PTCP is implemented in the PRU and PTCP driver. It mainly has two parts
 * @subsection PTCP_SYNC SYNC handling
 * PTCP has an ISR @ref PN_PTCP_isrHandler which is triggered when a SYNC packet is received from the PRU.
 * The ISR internally calls an important API @ref PN_PTCP_syncHandling which calculates the deltaT between
 * the master and slave, and according initiates the clock adjustment process. There is also a filter implemented
 * which makes sure the compensation is done only once the deltaT is stable. The clock adjustment
 * process is handled as a chain of transfers from ECAP->EDMA->IEP. An ecap period is set based on the
 * the calculated adjustment deltaT. At this ECAP period internal, ECAP generates an event which triggers
 * an EDMA transfer from a predifined location(@ref IEP_CONFIG_ADJ_OFFSET) to the IEP registers. This decides
 * if the IEP has to be compensated more(+ve) or less(-ve). This chain mechanism ensures that the value to be
 * compensated is spread out smoothly through the sync cycle time \n
 * The driver also maintains a task to monitor if there is a sync timeout @ref PN_PTCP_syncMonitorTask
 *
 * @subsection PTCP_DELAY Delay Handling
 * Another task @ref PN_PTCP_task does the handling of delays(cable delay, line delay etc.). This task
 * sends out multiple delay requests in a particular interval, and processes the delay responses(@ref PN_PTCP_delayMeasurement).
 * The calculated line delays(@ref deviceDelays_t) and port delays(@ref ptcpPortDelayVal_t) are stored in the internal
 * structures
 *
 * @section PTCP_INTEGRATION Integrating with the stack
 * The driver provides two main callbacks that can be used by the stack for certain even triggers. They are
 * @ref ptcpSyncStatusCall and @ref ptcpDelayUpdateCall(which can be registered by calling the APIs
 * @ref PN_PTCP_registerSyncStatusCall and @ref PN_PTCP_registerDelayUpdateCall). The @ref ptcpDelayUpdateCall is
 * triggered whenever there is any update in the delays calculated. The @ref ptcpSyncStatusCall is used by Sync to notify
 * any update in the SYNC state. It also indates error events like @ref TAKEOVER_TIMEOUT and @ref SYNC_TIMEOUT
 *
 */


/** @addtogroup PN_PTCP
 @{ */
/**
* @brief Enum for enable/disable states
*/

typedef enum
{
    disable = 0,            /**< Disabled state */
    enable  = 1            /**< Enabled state */
} ptcpPortStatus_t;


/**
* @brief Enum for the various sync states
*/

typedef enum
{
    OUT_OF_SYNC,            /**< Out of sync (Jitter Out of Boundary)*/
    IN_SYNC,                /**< In sync */
    TAKEOVER_TIMEOUT,       /**< Takeover timeout expired and Master Lost */
    SYNC_TIMEOUT           /**< Sync timeout expired (No sync msg received)*/
} syncState_t;

/**
* @brief Structure containing the port delay values of the local and remote
*/

typedef struct ptcpTimes
{
    uint32_t    rxDelayLocal;       /**< Local RX port delay */
    uint32_t    txDelayLocal;       /**< Local TX port delay */
    uint32_t    rxDelayRemote;      /**< Remote RX port delay */
    uint32_t    txDelayRemote;      /**< Remote TX port delay */
    uint32_t    cableDelay;         /**< Cable delay calculated by device */
} ptcpPortDelayVal_t ;

/**
* @brief Structure containing current sync info
*/

typedef struct
{
    syncState_t syncState;              /**< Current sync state of the device */
    uint8_t         masterSA[6];            /**< Sync Master's MAC address */
    uint8_t         subdomainUUID[16];      /**< Sub-domain UUID */
} ptcpSyncInfo_t;

/**
 *  @internal
 *  @brief Callback function for PTCP messaging to stack
 *
 *  @param  arg
 *  @param  arg2
 *  @retval none
 */
typedef void (*ptcpCallBack_t)(uint32_t arg, uint32_t arg2);

/**
 * @brief               Registers the callback function for getting notifications about change in sync state
 *
 * @param pnHandle Profinet Handle
 * @param[in] callBack  Function pointer for callback function\n
 *                      typedef void (*ptcpCallBack_t) (uint32_t arg1, uint32_t arg2);\n
 *                      arg1: syncState_t, arg2: NULL
 */
void PN_PTCP_registerSyncStatusCall(PN_Handle pnHandle,
                                    ptcpCallBack_t callBack);

/**
 * @brief                   Registers the callback function for getting notifications about new cable delay measurement
 *
 * @param pnHandle Profinet Handle
 * @param[in] callBack      Function pointer for callback function\n
 *                          typedef void (*ptcpCallBack_t) (uint32_t arg1, uint32_t arg2);\n
 *                          arg1: portNum (1 or 2), arg2: new cable delay
 */
void PN_PTCP_registerDelayUpdateCall(PN_Handle pnHandle,
                                     ptcpCallBack_t callBack);

/**
 * @brief                   Returns the current cable delay related values
 *
 * @param pnHandle Profinet Handle
 * @param[in] portDelays    Reference to structure for ptcp delay values\n
 * @param[in] portNum       Port no. (1 or 2) for which delay values are requested
 */
void PN_PTCP_getDelayValues(PN_Handle pnHandle, ptcpPortDelayVal_t *portDelays,
                            uint8_t portNum);

/**
 * @brief                   Get the local port delay values
 *
 * @param pnHandle Profinet Handle
 * @param[in] portNum       Port no. (1 or 2) for which delay values are requested\n
 * @param[in] outRxDelay    Pointer to Rx port delay variable\n
 * @param[in] outTxDelay    Pointer to Tx port delay variable
 */
void PN_PTCP_getLocalDelayValues(PN_Handle pnHandle, uint8_t portNum,
                                 uint32_t *outRxDelay, uint32_t *outTxDelay);

/**
 * @brief                   Get the remote port delay values
 *
 * @param pnHandle Profinet HAndle
 * @param[in] portNum       Port no.(1 or 2) for which delay values are requested\n
 * @param[in] outRxDelay    Pointer to Rx port delay variable\n
 * @param[in] outTxDelay    Pointer to Tx port delay variable
 */
void PN_PTCP_getRemoteDelayValues(PN_Handle pnHandle, uint8_t portNum,
                                  uint32_t *outRxDelay, uint32_t *outTxDelay);

/**
 * @brief               Returns current sync status and other sync related info
 *
 * @param pnHandle Profinet Handle
 * @param[in] syncInfo  Reference to structure for ptcp sync info
 */
void PN_PTCP_getSyncInfo(PN_Handle pnHandle, ptcpSyncInfo_t *syncInfo);

/**
 * @brief           Get the sync Master address (MAC)
 *
 * @param pnHandle Profinet Handle
 * @param[in] addr  Reference to MAC memory field (6 bytes)
 */
void PN_PTCP_getSyncMasterAddress(PN_Handle pnHandle, uint8_t *addr);

/**
 * @brief                       Sets the PLL window for sync handling
 *
 * @param pnHandle Profinet Handle
 * @param[in] pllWindowSize     Defines the PLL window size in ns. \n
 *                              If PLL window is not set by application, 1000 ns will be taken as default.\n
 *                              If deviation from master is within PLL window, device will be considered to be in sync state.\n
 *                              Otherwise, out of sync.
 */
void PN_PTCP_setPllWindow(PN_Handle pnHandle, uint32_t pllWindowSize);

/**
 * @brief                           Sets the Sync timeout factor
 *
 * @param pnHandle Profinet Handle
 * @param[in] syncTimeoutFactor     Sync timeout factor in terms of sync intervals.\n
 *                                  If it is not set by application, 6 will be taken as default.\n
 *                                  If no. of consecutive sync  packet missed is equal to syncTimeoutFactor, notify application.
 */
void PN_PTCP_setSyncTimeoutFactor(PN_Handle pnHandle,
                                  uint32_t syncTimeoutFactor);

/**
 * @brief                           Sets the takeover timeout factor
 *
 * @param pnHandle Profinet Handle
 * @param[in] takeoverTimeoutFactor Defines the takeover timeout factor in terms of sync intervals.\n
 *                                  If it is not set by application, 3 will be taken as default\n
 *                                  If no. of consecutive sync  packet missed is equal to takeoverTimeoutFactor, notify application
 */
void PN_PTCP_setTakeoverTimeoutFactor(PN_Handle pnHandle,
                                      uint32_t takeoverTimeoutFactor);

/**
 * @brief                       Sets the sync UUID for the device.\n
 *                              Application is also notified about the current sync status.\n
 * @param pnHandle Profinet Handle
 * @param[in] subdomainUUID     Reference to the array containing subdomainUUID
 */
void PN_PTCP_setSyncUUID(PN_Handle pnHandle, uint8_t *subdomainUUID);

/**
 * @brief               Controls(enable/disable) the sync forwarding; by default it is enabled
 *
 * @param ICSS_EmacBaseAddressHandle ICSS EMAC LLD Hw Attrs for base addresses
 * @param[in] state     disable /enable
 */
void PN_PTCP_configureSyncFwd(ICSS_EmacBaseAddressHandle_T
                              ICSS_EmacBaseAddressHandle, ptcpPortStatus_t state);

/**
 * @brief               Controls(enable/disable) the delay measurement process on a particular port.\n
 *                      By default, delay measurement is enabled on both the ports.
 *
 * @param pnHandle Profinet Handle
 * @param[in] portNum   Port no. (1 or 2) which needs to be configured.\n
 * @param[in] state     disable / enable
 */
void PN_PTCP_configureDelayMeasurement(PN_Handle pnHandle, uint8_t portNum,
                                       ptcpPortStatus_t state);

/**
  * Configures whether delay response should be sent on a particular port or not, in response for delay request
  *
  * @param portNum: port no. (1 or 2)which needs to be configured
  * @param state  : disable / enable
  */

void PN_PTCP_configureDelayResp(ICSS_EmacBaseAddressHandle_T
                                ICSS_EmacBaseAddressHandle, uint8_t portNum, ptcpPortStatus_t state);
/**
@}
*/

/**
 * @internal
 * @brief Timestamp structure for absolute time API
 */
typedef struct
{
    uint16_t Status;
    uint16_t SecondsHigh;
    uint32_t SecondsLow;
    uint32_t Nanoseconds;
} PNIO_TimeStamp;

/**
 * @internal
 * @brief Structure to store variables for absolute time API
 */
typedef struct
{
    uint32_t TorgSec;
    uint32_t TorgNsec;
    uint32_t TDelay;

    uint32_t IEP_count_tS;
    uint32_t IEP_count_tL_R;
    uint32_t IEP_count_tL_F;
    uint32_t IEP_count_fn;

    uint64_t tick_S;
    uint64_t tick_L;
    uint64_t tick_fn;
} LatchVars;


/**
 * @brief API to return absolute PTCP time.
 *
 * Absolute time calculation triggered by latch0 event or function call.
 * When triggered by latch0 event, ISR calculates absolute time.
 *
 * @param[in] LatchEn Trigger \n
 *              1 - Triggered by latch input\n
 *              0 - Triggered by function call itself\n
 * @param[in] p_PNIO_TimeStamp ointer to structure PNIO_TimeStamp \n
 *
 * @retval 0 if device is in sync with PLC, -1 if out of sync
 */

int32_t PN_PTCP_getAbsoluteTime(PN_Handle pnHandle,
                                PNIO_TimeStamp *p_PNIO_TimeStamp, int32_t LatchEn);


void PN_PTCP_latchInit(PN_Handle pnHandle);

#endif /* IPTCPDRV_H_ */
