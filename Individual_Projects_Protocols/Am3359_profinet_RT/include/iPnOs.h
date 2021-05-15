/**
 * @file iPnOs.h
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

#ifndef IPNOS_H_
#define IPNOS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


#include "PN_Handle.h"
#include "iRtcDrv2.h"

/* ========================================================================== */
/*                          Doxygen                                           */
/* ========================================================================== */

/**
 * @page PN_IRT_DOC Profinet IRT Module documentation
 * -@subpage PN_EMAC_LLD_INTERFACE
 * -@subpage PN_OS_INTEGRATION
 * -@subpage PN_CPM_PPM_MANAGEMENT_PAGE
 * -@subpage PN_IRT_TRIPLE_BUFFER_PAGE
 * -@subpage PN_MRP_PAGE
 * -@subpage PN_IRT_LEGACY_PAGE
 * -@subpage PN_PHASE_MANAGEMENT_PAGE
 * -@subpage PN_PTCP_PAGE
 * -@subpage PN_ISOM_PAGE
 * -@subpage PN_OTHER_TOPICS
 */
/**
 * @page PN_EMAC_LLD_INTERFACE Integration with EMAC-LLD
 * @tableofcontents
 * @section INTRO_PN_EMAC_LLD_INTERFACE Introduction
 * TI Profinet solution uses the EMAC-LLD as it's base switch layer. The PRU Firmware is customized for
 * Profinet functionalities, and is not same as a standard switch firmware. The NRT(non-real time) traffic is handled
 * by EMAC-LLD, wherein the packets are forwarded either to the TCP stack or a custom callback(configurable in EMAC LLD)
 *
 * @section PN_EMAC_LLD_INTERFACE_DESC Interface with EMAC-LLD
 * On a standard EMAC-LLD Switch driver, of the 4 queues, all traffic is forwarded to Queue 4(which is then forwarded to
 * the TCP/IP stack). In case of Profinet firmware, the Queue are designed as shown in below diagram
 * @image html pn_switch_queues.png
 * The highest priority queues are used for LLDP, MRP, DCP, RTA and PTCP. These packet are directly forwarded to the registered
 * callback in EMAC-LLD(using rxRTCallBack). And the Queues 2 and 3 are forwarded to TCP/IP. \n
 * The Profinet Driver/stack also uses the TX/RX APIs available in the EMAC-LLD for transmission/reception of packets like
 * PTCP, LLDP, etc. More info on EMAC-LLD can be found here <a href="http://processors.wiki.ti.com/index.php/Processor_SDK_RTOS_ICSS-EMAC-Design">
 * EMAC-LLD Developer's Guide</a>
 *
 * @section PN_EMAC_LLD_INTERFACE_CONFIG Configuring the EMAC-LLD Handle
 * The EMAC-LLD Handle(ICSS_EmacHandle) configures the EMAC-LLD. This should be configured as a standard switch,
 * except the following exceptions \n
 * switchEmacCfg->ethPrioQueue = ICSS_EMAC_QUEUE3; \n
 * switchEmacCfg->halfDuplexEnable = 0; \n
 * switchEmacCfg->rxIntNum = 120 + 32; //In case of AM57xx; \n
 * switchEmacCfg->linkIntNum = 126 + 32; //In case of AM57xx; \n
 * ethPrioQueue configures the queues as shown in above figure. i.e., by setting it to ICSS_EMAC_QUEUE3 the EMAC-LLD
 * is configured to forward packets coming in QUEUE3 and QUEUE4 to TCP/IP stack \n
 * Details of ARM Interrupt numbers used can be found here - @ref PN_OS_INTEGRATION_INTERRUPTS
 */
/**
 * @page PN_OS_INTEGRATION OS Components
 * @tableofcontents
 * @section INTRO_PN_OS_INTEGRATION Introduction
 * This section describes the OS entities used by Profinet
 *
 * @section PN_OS_INTEGRATION_INTERRUPTS Interrupts
 * Profinet uses these ARM Interrupts
 * ARM Interrupt Number  | PRU Interrupt Number | ISR in Driver                          | Function
 * --------------------- | -------------------- |--------------------------------------- | --------------
 * 152(AM57x)            | 20                   | ICSS_EmacRxInterruptHandler(EMAC-LLD)  | Handles RX
 * 153(AM57x)            | 21                   | @ref PN_ppmIsrHandler                  | PPM TX completed
 * 154(AM57x)            | 22                   | @ref PN_cpmIsrHandler                  | CPM Receive event
 * 155(AM57x)            | 23                   | @ref PN_dhtIsrHandler                  | DHT and other status events
 * 156(AM57x)            | 24                   | @ref PN_PTCP_isrHandler                | Initiates the sync handling process
 * 158(AM57x)            | 26                   | ICSS_EmacLinkISR (EMAC-LLD)            | Link ISR
 *
 * @section PN_OS_INTEGRATION_TASKS Tasks
 * The tasks used in the Profinet driver(not application) are
 * Task Name                     | Function
 * ----------------------------- | -------------
 * @ref PN_PTCP_task             | PTCP delay measurement scheduling
 * @ref PN_PTCP_syncMonitorTask  | PTCP sync monitor scheduling
 * @ref PN_IRT_legacyTask        | Legacy startup mode scheduling
 * @ref PN_MRP_CPMTask           | MRP Task to control flush mode of ICSS
 * @ref PN_tapWatchDog_task      | Tap the ICSS WatchDog Timer
 *
 * @section PN_OS_INTEGRATION_OTHERS Resource Locking
 * Profinet Driver uses two <a href="http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/sysbios/6_32_04_49/exports/bios_6_32_04_49/docs/cdoc/ti/sysbios/gates/GateHwi.html">GateHwi's</a>
 * GateHwi Handle Name           | Function
 * ----------------------------- | -------------
 * @ref swapGatePpm              | Protect swapping the PPM triple buffers
 * @ref swapGateCpm              | Protect swapping the CPM triple buffers
 * PTCP module uses two semaphores
 * Semaphore Handle Name          | Function
 * ------------------------------ | -------------
 * @ref ptcpStartSem              | Indicates start of PTCP
 * @ref ptcpTriggerMeasurementSem | Triggers delay measurement
 *
 */
/**
 * @page PN_OTHER_TOPICS Other Topics
 * @subpage PN_DCP_FILTER_PAGE
 * @subpage PN_FILTER_TABLE_PAGE
 * @subpage PN_WATCHDOG_PAGE
 */
/**
 * @defgroup PN_IRT_TRIPLE_BUFFER Triple buffer management APIs
 */

/**
 * @page PN_IRT_TRIPLE_BUFFER_PAGE Triple Buffer Management
 * @tableofcontents
 * @section INTRO_TRIPLE_BUFFER Introduction
 *
 * @subsection TRIPLE_BUFFER_INTRO Overview
 * The Triple Buffer scheme ensures that the stack or PRU always finds a buffer
 * to update. In case of PPM, the stack is the Producer and PRU consumes this, and in
 * case of CPM, the PRU is the producer and Stack consumes this. \n
 * Each CPM/PPM descriptor has three buffers associated with it, these buffers are indexed
 * by three pointers - next, last and proc (in @ref t_rtcPacket). There is also a validLast flag
 * in the same structure that indicates there is an update in data(CPM or PPM, set correspondingly
 * by PRU or stack). \n
 * NEXT - Index to the next available buffer. The stack/PRU writes to this buffer whenever there is
 * new data \n
 * LAST - Index to the last updated buffer. At any point of time, this buffer contains the latest data
 * to be consumed(CPM) or produced(PPM). \n
 * PROC - Index to the buffer being currently processed. In case of CPM, this indicates the buffer which
 * is currently being consumed by the stack. This buffer will be locked by the stack(using @ref PN_cpmBuffLock)
 * This indicates to PRU that the PROC buffer cannot be written. The PRU ping pongs between NEXT and
 * LAST buffer to update data\n
 * In case of PPM, this indactes the buffer being send by the PRU. The stack updates the NEXT and LAST
 * buffer in ping pong fashion until the PRU is ready to send updated data(by @ref PN_checkLastPPM )\n
 * validLast - In case of CPM, the PRU sets this flag whenever there is updated data in the LAST buffer.
 * The stack calls @ref PN_nextCpmRdy when it's ready to consume a new packet, and checks this flag for any
 * new data. \n
 * In case of PPM, when the stack has updated data in the LAST buffer, this flag is set(@ref PN_relPpmBuff).
 * When the PRU is ready to take a new buffer, @ref PN_checkLastPPM is called and the new PPM is produced by PRU. \n
 * The API definitions can be found here @ref PN_IRT_TRIPLE_BUFFER
 *
 * @section PPM_TRIPLE_BUFFER PPM Triple buffer management
 * In case of PPM, the stack is the Producer, and PRU consumes it to send it out on the wire \n
 * When the stack has a new PPM to be updated to send, it calls the API @ref PN_getPpmBuff, which returns
 * the a free buffer in the triple buffer(NEXT). The stack then starts updating data in NEXT(marked by grey box in
 * the following diagram).
 * @image html ppm_triplebuffer_1.png "Fig 1.1"
 * When the stack has completed updating data(marked by Green box in the following diagram to NEXT,
 * it calls the API @ref PN_relPpmBuff. At this point, the LAST index and NEXT index is swapped.
 * This means that the LAST data(previous) is discarded and we
 * have a new LAST data(the validLast flag is set). If the stack wants to further update new data,
 * the NEXT pointer is again used.
 * @image html ppm_triplebuffer_2.png "Fig 1.2"
 * Next, when the PRU is ready to update data(which is send on the wire), @ref PN_checkLastPpm is triggered.
 * At this point, the PROC and LAST indexes are swapped. This enables PRU to send the LAST data, which is the
 * latest available PPM data at any point of time. The validLast flag is cleared, which indicates the PRU that
 * there is no update in the data. Afer this, the stack has NEXT and LAST buffers to update future data(in a
 * ping pong fashion) as shown in Fig 1.1
 * @image html ppm_triplebuffer_3.png "Fig 1.3"
 * @section CPM_TRIPLE_BUFFER CPM Triple buffer management
 * In case of CPM, the PRU is the Producer, and the stack consumes it to use the data. \n
 * The stack always reads from the PROC buffer and also has it locked using @ref PN_cpmBuffLock. The PRU
 * never writes into this buffer. Fig 1.4 shows that the PRU is writing in NEXT buffer, while PROC(Buffer 2)
 * is locked by the stack.
 * @image html cpm_triplebuffer_1.png "Fig 1.4"
 * When the PRU has new data to be written, the NEXT and LAST indexes are swapped. This indicates
 * that LAST is pointed to the latest available data(to be consumed by the stack). If the PRU has updated
 * data to be written, it uses the NEXT buffer. The PRU keeps continuing in the loop i.e., switching the NEXT
 * and LAST buffers in a ping pong fashion till the stack consumes the LAST data.
 * @image html cpm_triplebuffer_2.png "Fig 1.5"
 * When the stack is ready to consume new data, it calls the API @PN_getLastCpm. A swap of LAST and PROC
 * indexes in done here. This indicates the stack consumes the latest data available at any point of time.
 * The stack also should lock the buffer using @ref PN_cpmBuffLock. At this point PRU starts updating any new
 * data in the NEXT buffer
 * @image html cpm_triplebuffer_3.png "Fig 1.6"
 */

/**
* @defgroup PN_MRP MRP APIs
*/

/**
* @page PN_MRP_PAGE Media Redundancy Protocol(MRP)
* @tableofcontents
* @section INTRO_MRP Introduction
* A state machine is implemented by @ref PN_MRP_CPMTask to implement MRP
* If a break is detected in the topology, this state machine switches to
* a state where the PRU sends PPM on both ports. It is also made sure that
* the FDB for both the ports are cleared. Once all the CPM's learn the correct
* port number (@ref t_rtcPacket -> port), the flush mode is deactivated. \n
* Note: to enable MRP, it's required to set the macro @ref MRP_SUPPORT \n
* The API definitions can be found here @ref PN_MRP
*
* @section MRP_STATE_MACHINE MRP state machine
* The MRP state machine has 5 states defined by @ref tMrpStates \n
* The state machine flow charts is shown in Fig 2.1
* @image html mrp_state.png "Fig 2.1"
* @ref MRPREADY - At initialization the state machine is set to this. This is an
* idle loop that does nothing, and indicates all links are OK. \n
* @ref MRPENTER - Whenever a link break is detected. The stack calls @ref PN_enterFlushMode
* to switch the state machine to MRPENTER. In this state, the port(@ref t_rtcPacket -> port)
* of all the CPM packets are set to 0(invalid state), and the PRU is indicated to switch to
* Flush mode.  \n
* @ref MRPWAIT - This state is used to switch between @ref MRPCHECK and @ref MRPWAIT with a sleep \n
* @ref MRPCHECK - This state checks if the port of all CPMs are learned, if yes it goes to @ref MRPEXIT,
* else sleep(going to @ref MRPWAIT)\n
* @ref MRPEXIT - resets the state machine back to @MRPREADY
*
*/

/**
* @defgroup PN_IRT_LEGACY Legacy Startup APIs
*/

/**
* @page PN_IRT_LEGACY_PAGE IRT Legacy Startup support
* @tableofcontents
* @section INTRO_IRT_LEGACY Introduction
* The Profinet IRT Legacy startup feature is implemented using a state machine @ref PN_IRT_legacyTask
* Before a connection is established(i.e., @ref RTC_CPM_STATUS_OFFSET is not set to @ref RTC_CPM_RUN),
* if legacy mode is enabled, the driver configures a PPM packet to be send in Green period \n
* Note: to enable Legacy startup, it's required to set the macro @ref IRT_LEGACY_STARTUP_SUPPORT \n
* The API definitions can be found here @ref PN_IRT_LEGACY
*
* @section IRT_LEGACY_STATE_MACHINE IRT Legacy startup state machine
* The IRT Legacy Startup state machine has 4 states defined by @ref tLegStates \n
* The state machine flow charts is shown in Fig 3.1
* @image html irt_legstate.png "Fig 3.1"
* @ref NOINIT - At initialization the state machine is set to @ref NOINIT. This state waits for
* the base driver to be ready, and then switches the state machine to @ref READY. \n
* @ref READY - This state is an idle loop, which waits for PPM packet to be initialized \n
* @ref SENDPPM - When the PPM packet is ready, the application sets the state to @ref SENDPPM(by
* callback function @ref irtLegStateCall. When this is done, the driver will send out the latest PPM data from
* @ref pLegPkt(which is set by the callback @ref irtLegPktCall. The driver calls the callback in @ref insPpmList if the input
* param to the API has legMode configured). The packet is send out in intervals, till the connection is established \n
* @ref ABORT - Error state
*
*/
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/** @addtogroup PN_IRT_TRIPLE_BUFFER
 @{ */

/**
 * @brief Return a pointer to buffer indicated by NEXT \n
 * This will be the next available free buffer(in the triple buffer scheme).
 * Stack can now fill this buffer and once done call @ref PN_relPpmBuff to
 * indicate the PRU on new data.
 *
 * @param pkt pointer to packet object
 * @return pointer to NEXT packet buffer address
 */
uint8_t *PN_getPpmBuff(t_rtcPacket *pkt);

/**
 * @brief Swap NEXT with LAST and return new NEXT address \n
 * @details This API is called when stack has completed updating data to the buffer
 * received from @ref PN_getPpmBuff. This indicates the PRU about data update and updates
 * the @ref ACTIVE_LIST directly. Stack can now start filling data in the NEXT buffer returned
 *
 * @param pnHandle Profinet Handle
 * @param pkt pointer to packet object
 * @return pointer to NEXT packet buffer address
 */
uint8_t *PN_relPpmBuff(PN_Handle pnHandle, t_rtcPacket *pkt);

/**
 * @brief Update proc pointer in paket object and udpate descriptor if there is a new last buffer
 * @details Called from the @ref PN_ppmIsrHandler. The PRU firmware checks if there is a new
 * buffer available to send by checking the validLast flag in @ref t_rtcpacket . If available
 * swaps the proc buffer(which is currently being send by the PRU), with the last buffer(the
 * buffer which holds the latest data produced by the stack). This ensures that the PRU sends
 * out latest available valid data at any point of time.
 *
 * @param pnHandle Profinet Handle
 * @param pkt pointer to packet object
 * @return 1: new buffer activated \n
 *         0: no buffer update
 *         <0: Not a PPM packet
 */
int32_t PN_checkLastPPM(PN_Handle pnHandle, t_rtcPacket *pkt);

/**
 * @brief Swap CPM NEXT with LAST and set validLast flag
 * @detail Called from @ref PN_cpmIsrHandler , when a new CPM frame has arrived
 * and sets the validLast flag in @ref t_rtcPacket . When the stack is ready to
 * receive an updated CPM packet, it calls @ref PN_getLastCpm and this LAST packet
 * is consumed by the stack
 *
 * @param pnHandle Profinet Handle
 * @param pkt pointer to packet object
 * @retval 0 buffers swapped \n
 * @retval 1 buffers swapped and overrun detected\n
 * This indicates the validLast flag was already set, and the stack has missed to
 * consume a CPM packet
 * @retval -1 error - wrong packet type\n
 */
int32_t PN_nextCpmRdy(PN_Handle pnHandle,
                      t_rtcPacket *pkt);


/**
 * @brief Swap CPM LAST with PROC if validLast is true, clear validLast flag
 * @detail When the stack is ready to consume a new CPM packet, this API is
 * called. This swaps the LAST buffer(which has the latest updated/consumed data
 * by the PRU firmware) and the PROC buffer(which is being currently consumed by the
 * stack). This also locks the new PROC buffer indicating that the Stack is consuming
 * this buffer(PRU won't write to this buffer)
 *
 * @param[in] pnHandle Profinet Handle
 * @param pkt pointer to packet object
 * @return buffer PROC buffer address
 */
uint8_t *PN_getLastCpm(PN_Handle pnHandle, t_rtcPacket *pkt);

/**
@}
*/
/** @addtogroup PN_MRP
 @{ */
/**
 * @brief Used to signal topology change to MRP state machine
 *
 * @param pnHandle Profinet Handle
 * @return 0 if we went to MRPENTER state
 * @return 1 if already in flush operation
 *
 * @pre MRP_SUPPORT defined
 */
uint32_t PN_enterFlushMode(PN_Handle pnHandle);
/**
 * @internal
 * @def state machine states for MRP task
 */
typedef enum
{
    MRPREADY,       /**< MRP module initiliazed. Idle state, do nothing */
    MRPENTER,       /**< MRP flush mode entered. Break in network detected, flush FDB */
    MRPWAIT,        /**< Waiting for CPM received. Sleep and goto @ref MRPCHECK */
    MRPCHECK,       /**< Check if all CPM received. Check by @ref PN_allCpmKnown*/
    MRPEXIT         /**< Exit MRP flush mode, back to normal op i.e., to @ref MRPREADY */
} tMrpStates;
/**
@}
*/

/** @addtogroup PN_IRT_LEGACY
 @{ */

/**
 * @brief Returns the IRT Legacy mode state
 *
 * @param arg0  not used
 * @param arg1  not used
 *
 * @return Current state of the Legacy mode state machine
 */
tLegStates PN_getLegState(void);

/**
 * @brief Sets the state of legacy state machine
 * This API is registered as a callback in @ref PN_registerSetState
 *
 * @param arg   new state @ref t_legStates
 * @param arg2  not used
 */
void PN_setLegState(void *arg, void *arg2);

/**
 * @brief Sets the Ethernet packet to output during legacy startup
 * Usually a copy of RTC3 PPM. This API is registered as a callback in @ref PN_registerSetPkt
 *
 * @param arg   Pointer to valid packet buffer, no checks. @ref t_rtcPacket
 * @param arg2  not used
 */
void PN_setLegPkt(void *arg, void *arg2);
/**
@}
*/

/**
 * @brief Initialization function for all PN driver tasks, mutexes
 *
 * @param pnHandle Profinet Handle
 * @return None
 */
int32_t PN_initOs(PN_Handle pnHandle);

/**
 * @brief Interrupt management function
 *        Initializes PPM/CPM/DHT interrupts. (OS dependent!)
 *
 * @retval 0 on success
 */
int32_t PN_RTC_setupIsr(PN_Handle pnHandle);

/**
 * @brief Enables the PN interrupts
 * @param[in] pnHandle Profinet HAndle
 * @retval 0 on Success
 *
 */
int32_t PN_RTC_enableISR(PN_Handle pnHandle);

/**
 * @brief Disables the PN interrupts
 * @param pnHandle Profinet Handle
 * @retval 0 on Success
 *
 */
int32_t PN_RTC_disableISR(PN_Handle pnHandle);

/**
 * @brief Switch driver extension of TxPacket
 *
 * @detail This is using a critical section to protect re-entry of TX function.
 * The protection scheme is borrowed from NDK and we use their code too.
 * This requires to adhere to NDK priority scheme
 *
 * @param[in] icssEmacHandle ICSS Emac LLD handle
 * @param srcAddress        pointer to TX packet
 * @param portNumber        output port number
 * @param queuePriority     output queue priority
 * @param lengthOfPacket    TX packet length (without CRC)
 * @callgraph
 */
int32_t PN_OS_txPacket(ICSSEMAC_Handle icssEmacHandle,
                       const uint8_t *srcAddress, int32_t portNumber, int32_t queuePriority,
                       int32_t lengthOfPacket);

#endif /* IPNOS_H_ */
