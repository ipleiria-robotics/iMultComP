/**
* @file  icss_eip_driver.h
*
* @brief Include file for icss_eip_driver.c
*
*\par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*\par
*/

#ifndef ICSS_EIP_DRIVER_H_
#define ICSS_EIP_DRIVER_H_

/* ========================================================================== */
/*                          Doxygen                                           */
/* ========================================================================== */

/**
 * @page IEP_ADAPTER_DOC Ethernet/IP Adapter documentation
 * -@subpage EIP_EMAC_LLD_INTERFACE
 * -@subpage EIP_OS_INTEGRATION
 * -@subpage EIP_DLR_PAGE
 * -@subpage EIP_PTP_1588_PAGE
 * -@subpage EIP_ACD_PAGE
 * -@subpage EIP_OTHER_TOPICS_PAGE
 */

/**
 * @page EIP_EMAC_LLD_INTERFACE Integration with EMAC-LLD
 * @tableofcontents
 * @section INTRO_EIP_EMAC_LLD_INTERFACE Introduction
 * TI's Ethernet/IP solution uses the EMAC-LLD as it's base switch layer. The PRU Firmware is identical to the one used
 * in EMAC-LLD save for the additional PTP and DLR functionalities. The NRT(non-real time) traffic is handled
 * by EMAC-LLD, wherein the packets are forwarded either to the TCP stack or a custom callback(configurable in EMAC LLD)
 *
 * @section EIP_EMAC_LLD_INTERFACE_DESC Interface with EMAC-LLD
 * Ethernet/IP packets are standard ethernet frames and all protocol specific data is embedded in TCP/IP payload. Hence the standard
 * switch model applies. Packets are segregated based on VLAN PCP field. The 8 priorities map to 4 queues. With highest and next highest
 * priorities going to Queue 0 and Queue 1 and so on. If PCP field does not exist, then frames go to Queue 3.
 * All queues except the highest priority queue forward the frames to TCP/IP stack. Further if packet type does not match PTP or DLR then that
 * frame goes to TCP/IP stack.
 * @image html eip_switch_queues.png
 * The highest priority queues are used for PTP and DLR. These packet are directly forwarded to the registered
 * callback in EMAC-LLD(using rxRTCallBack).
 * More info on EMAC-LLD can be found here <a href="http://processors.wiki.ti.com/index.php/Processor_SDK_RTOS_ICSS-EMAC-Design">
 * EMAC-LLD Developer's Guide</a>
 *
 * @section EIP_EMAC_LLD_INTERFACE_CONFIG Configuring the EMAC-LLD Handle
 * The EMAC-LLD Handle(ICSS_EmacHandle) configures the EMAC-LLD. This should be configured as a standard switch,
 * except the following exceptions \n
 * switchEmacCfg->ethPrioQueue = ICSS_EMAC_QUEUE4; \n
 * switchEmacCfg->halfDuplexEnable = 1; \n
 * switchEmacCfg->rxIntNum = 120 + 32; //In case of AM57xx; \n
 * switchEmacCfg->linkIntNum = 126 + 32; //In case of AM57xx; \n
 * ethPrioQueue configures the queues as shown in above figure. i.e., by setting it to ICSS_EMAC_QUEUE4 the EMAC-LLD
 * is configured to forward packets coming in QUEUE4 to TCP/IP stack \n
 * Details of ARM Interrupt numbers used can be found here - @ref EIP_OS_INTEGRATION_INTERRUPTS
 */
/**
 * @page EIP_OS_INTEGRATION OS Components
 * @tableofcontents
 * @section INTRO_EIP_OS_INTEGRATION Introduction
 * This section describes the OS entities used by EtherNet/IP
 *
 * @section EIP_OS_INTEGRATION_INTERRUPTS Interrupts
 * EtherNet/IP uses these ARM Interrupts
 * ARM Interrupt Number  | PRU Interrupt Number | ISR in Driver                          | Function
 * --------------------- | -------------------- |--------------------------------------- | --------------
 * 152(AM57x)            | 20                   | ICSS_EmacRxInterruptHandler(EMAC-LLD)  | Handles RX
 * 153(AM57x)            | 21                   | @ref EIP_DLR_port0ISR                  | DLR Interrupt for Port 0
 * 154(AM57x)            | 22                   | @ref EIP_DLR_port1ISR                  | DLR Interrupt for Port 1
 * 155(AM57x)            | 23                   | @ref TimeSync_syncTxIsr                | PTP ISR for Sync frame
 * 156(AM57x)            | 24                   | @ref TimeSync_processPdelayRespFrame   | PTP ISR for Delay Response frame
 * 158(AM57x)            | 26                   | ICSS_EmacLinkISR (EMAC-LLD)            | Link ISR
 *
 */

/**
* @defgroup EIP_API EtherNet/IP APIs
*/

/**
* @defgroup EIP_DLR DLR APIs
*/

/**
 * @page EIP_DLR_PAGE Device Level Ring
 * @tableofcontents
 * @section INTRO_EIP_DLR Introduction
 * Device Level Ring on EtherNet/IP firmware provides redundancy to the switch implementation. It's a beacon based
 * implementation with support for a minimum beacon of 200us and timeout of 400us. More information is available here <a href="http://processors.wiki.ti.com/index.php?title=ICSS_EIP_Adapter_Developer_Guide#DLR">
 * EtherNet/IP developer guide</a>
 * @section EIP_OS_INTEGRATION_TIMERS HW and SW Timers
 * EtherNet/IP Driver (DLR) uses two DMTimers (4 & 5) for it's operation. In addition to this it uses two software timers
 * provided by the BIOS for implementing neighbor timeout clock
 * Timer Handle Name             | Function
 * ----------------------------- | -------------
 * dlrTimer_PORT0                | Beacon Timeout timer for Port 0. DMTimer 4
 * dlrTimer_PORT1                | Beacon Timeout timer for Port 1. DMTimer 5
 * dlrNeighborTimeoutClock[0]    | Neighbor timeout clock for Port 0
 * dlrNeighborTimeoutClock[1]    | Neighbor timeout clock for Port 1
 */


/**
 * @page EIP_PTP_1588_PAGE IEEE PTP-1588 Implementation
 * @tableofcontents
 * @section INTRO_PTP_1588 Introduction
 * PTP/1588 on EtherNet/IP provides time synchronization support. The implementation is driven by CIP Sync
 * requirements which require End to End clock support over UDP (Annex D). EtherNet/IP adapter application supports
 * both OC and TC implementations along with syntonization.
 * Since it's used by other Industrial protocols it exists as a separate protocol in the implementation. All PTP/1588 related information can be found in <a href="http://processors.wiki.ti.com/index.php/ICSS_PTP_1588_Developer_Guide">
 * PTP Developer Guide</a>
 * @section PTP_OS_INTEGRATION_TASKS Tasks
 * The tasks used in the PTP-1588 driver(not application) are
 * Task Name                                 | Function
 * ----------------------------------------- | -------------
 * @ref TimeSync_calcRcfAndSyncInterval      | PTP Delay Req Transmit & RCF calculation function
 *
 * @section PTP_OS_INTEGRATION_SEM Semaphore
 * PTP-1588 module uses one semaphore
 * Semaphore Handle Name          | Function
 * ------------------------------ | -------------
 * @ref timeSync_SyntInfo_t       | Triggers calculation of syntonization factor
 *

 */

/**
 * @page EIP_ACD_PAGE ACD Implementation
 * @tableofcontents
 * @section INTRO_EIP_ACD Introduction
 */


/**
 * @page EIP_OTHER_TOPICS_PAGE Other modules
 * @tableofcontents
 * @section INTRO_OTHER_TOPICS Introduction
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/icss_emac/icss_emacCommon.h>
#include <ti/drv/icss_emac/icss_emacLearning.h>
#include <ti/drv/icss_emac/icss_emacStatistics.h>
#include <Include/protocol/timeSync/icss_timeSync.h>

#include "icss_eip_driverDef.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*Use compare reg 4*/
#define IEP_CMP_DEFAULT_VAL      0x11
#define IEP_CMP4_DEFAULT_VAL     0x5000
#define IEP_CMP0_DEFAULT_VAL     0xffffffff

/**IEP Compare Register*/
#define PRU_IEP_CMP_CFG_REG             0x40
/*Compare 0 register*/
#define PRU_IEP_CMP0_REG                0x48

/**EIP Tick duration, used for DLR link detection as well as Half Duplex*/
#define EIP_TICK_PERIOD 1 /*in milliseconds*/
#define LOOPBK_PKT_SEND_PERIOD 1000

#define DEFAULT_BC_PKT_SIZE     60

#define ONE_SECOND_INTERVAL 1000    /*in milliseconds*/
#define TWO_MINUTE_INTERVAL  120000 /*in milliseconds*/

/**Mask for carrier sense status on MII RT for Half Duplex*/
#define CRS_STATUS_MASK                     0x2
/**Shift Val for carrier sense status on MII RT for Half Duplex*/
#define CRS_STATUS_SHIFT                    0x1

/**PTP Protocol type*/
#define PTP_PROT_TYPE       0x88F7
#define DLR_MDIO_PHY0 1
#define DLR_MDIO_PHY1 2

/*DLR port 0 Interrupt*/
#define PHYBMSR_OFFSET                      0x1
#define PHYSTS_OFFSET                       0x10

#define PHY_LINK_STATUS_MASK                0x4
#define PHY_LINK_STATUS_SHIFT               0x2

/** @addtogroup EIP_API
 @{ */

/**
 *  @brief  Initialization routine for Ethernet/IP driver functions
 *
 *          This API does the following functionalities
 *              Register the Real Time Rx Call back
 *              Enable Storm prevention
 *              Call DLR init API
 *              Call PTP init API
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvInit(EIP_Handle icssEipHandle);
/**
 *  @brief  EIP driver stop API
 *
 *          This API stops DLR and disables PTP
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvStop(EIP_Handle icssEipHandle);
/**
 *  @brief  EIP driver start API
 *
 *          This API starts DLR and enables PTP
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  none
 *
 */
void EIP_drvStart(EIP_Handle icssEipHandle);
/**
 *  @brief  API to process the real time Packets
 *
 *          This API will be registered as Real Tme Rx Call back. Incase of EIP, the driver
 *          receives DLR and PTP packets. This API receives the packet, checks the packet
 *          type and passes the packet to DLR driver or PTP driver
 *
 *  @param  queue_number [in] Queue where the packet is present
 *  @param  userArg      [in] userArgumment. EIP handle
 *
 *  @retval  none
 *
 */
void EIP_processProtocolFrames(uint32_t *queue_number, void *userArg);

/**
 *  @brief  API to initialize the CIP Sync objects in the EIP handle
 *          Initialize CIP Sync member variables based on PTP implementation
 *
 *  @param  icssEipHandle [in] EIP handle
 *
 *  @retval  0  - On success
 *           <0 - On failure
 *
 */
int8_t EIP_initializeCIPSync(EIP_Handle icssEipHandle);

/**
@}
*/
#endif /* TOOLS_H_ */
