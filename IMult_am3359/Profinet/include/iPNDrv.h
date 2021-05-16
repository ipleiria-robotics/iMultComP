/**
 * @file  iPNDrv.h
 *
 * @brief Profinet driver initialization function declarations
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

#ifndef IPNDRV_H_
#define IPNDRV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/icss_emac/icss_emacDrv.h>
#include "PN_Handle.h"
#include <stdint.h>

/* ========================================================================== */
/*                          Doxygen                                           */
/* ========================================================================== */
/**
 * @defgroup PN_PHASE_MANAGEMENT Phase Management APIs
 */

/**
 * @page PN_PHASE_MANAGEMENT_PAGE Phase Management
 * @tableofcontents
 * @section PHASE_MANAGEMENT_INTRO Introduction
 * Every cycle in Profinet IRT is split into a RED and GREEN period. The driver has APIs to configure these
 * periods, which is mostly done during establishment of connection. GREEN period can be further classified into
 * Green steady period and Yellow period. If a packet has to be transmitted during Yellow period, the driver has to
 * check if the packet can be send out in the remaining time left. Otherwise it's send out in the next green period \n
 * @image html phase_mgmnt.png \n
 * The API definitions can be found here @ref PN_PHASE_MANAGEMENT
 * @section PHASE_MANAGEMENT_IMPLEMENTATION Implementation
 * The phases are mapped to profiles(which holds the information regarding RX start of green period and TX start of green period).
 * There can be 5 profiles each for a port, which is configured by @ref PN_setProfile.
 * The 16 phases in Profinet can be mapped to any of these Profiles using @ref PN_mapPhaseToProfile. All this configuration
 * should be done during the connection establishment \n
 * The enabling/disabling of Phase management is based on the RTC3 port state. Phase management is disabled if RTC3 port
 * state is @ref OFF, and enabled if it's @ref RUN or @ref UP. \n
 * The stack also has to configure the Max Bridge delay(@ref PN_setMaxBridgeDelay), Max Line RX Delay(@ref PN_setMaxLineRxDelay), and
 * the Yellow period(@ref PN_setYellowPeriod). These information are available from the GSD file. The RED guard also should be set
 * by the stack, which defines a range of valid Red frame IDs(@ref PN_setRedGuard) \n
 * The API @ref PN_setRtc3PortStatus can be used by the stack to set the Port status to enable RTC3 on the port(valid states
 * @ref RUN, @ref UP and @ref OFF)
 */

/**
* @defgroup PN_DCP_FILTER DCP Filter APIs
*/

/**
 * @page PN_DCP_FILTER_PAGE DCP Filter
 * @tableofcontents
 * @section DCP_FILTER_INTRO Introduction
 * In a chain of devices, during startup the first device receives a storm of Ident Requests. As all these packets go to ARM,
 * there is a possibility that the device might lose the Ident Req which is meant for the device. To avoid this situation,
 * the DCP Filter can be used. This ensures that only the Ident Request packet with the configured station name is
 * forwarded to ARM. A maximum of 8 characters is compared by the firmware(last 8 characters). If the length of station name
 * is set to 0, this indicates the DCP Filter is not in use.
 * The API definitions can be found here @ref PN_DCP_FILTER
 */

/**
* @defgroup PN_FILTER_TABLE Multicast Filter Table APIs
*/

/**
 * @page PN_FILTER_TABLE_PAGE Multicast Filter Tables
 * @tableofcontents
 * @section PN_FILTER_TABLE_INTRO Introduction
 * The stack has to configure the Multicast filter table. This is done by the API @ref PN_setStaticFilterTable . There are
 * two tables - @ref FORWARDING table and @ref BLOCKING table. The Multi cast addresses can be configured to function in one of these states
 * - @ref NO_RCV_NO_FWD
 * - @ref RCV_NO_FWD
 * - @ref NO_RCV_FWD
 * - @ref RCV_FWD \n
 * The API definitions can be found here @ref PN_FILTER_TABLE
 */

/**
* @defgroup PN_WATCHDOG Watchdog timer APIs
*/

/**
 * @page PN_WATCHDOG_PAGE Watchdog timer
 * @tableofcontents
 * @section WATCHDOG_INTRO Introduction
 * Watchdog timer is used as a safety feature to monitor the application state and to turn off the PPM transmission after pre-defined interval if application is not responding. The watchdog will thereby protect the system from errors or faults by timeout or expiration. The expiration is used to initiate corrective action in order to keep the system in a safe state and restore normal operation based on configuration. Therefore, if the system is stable, the watchdog timer should be regularly reset or cleared to avoid timeout or expiration.
 * 
 * Application can use the API PN_setWatchDogTimer (defined in iPNDrv.c) to set the timeout value. Application needs to enable the WATCHDOG_SUPPORT macro to use this feature.
 * By default the PROFINET Slave driver sets the timeout to 100ms (watchDogExpireDuration defined in iPndrv.h) as follows:
 * PN_setWatchDogTimer(pnHandle, watchDogExpireDuration)
 * The PN_tapWatchDog_task task defined in iPnOs.c resets the watchdog timer periodically.
 * 
 * This timeout is system dependent and is the responsibility of user to set the trigger frequency and the timeout.
 * 
 * If watchdog timer expires, the firmware will stop all active PPM connections. Application can check the expiry state of watchdog timer using WD_STATUS register in ICSS IEP. Bit 0 of WD_STATUS is PD_WD_STAT. 
 * Example: 
 * Condition for Active or disabled: 0x0001 == HWREGH(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_WD_STATUS_REG)
 * Condition for Expired: 0x0000 == HWREGH(emacBaseAddr->prussIepRegs + CSL_ICSSIEP_WD_STATUS_REG)
 * 
 * The API definitions can be found here @ref PN_WATCHDOG
 */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * @def ERR_STATION_NAME_LENGTH
 *      Error code for Wrong station name length
 */
#define ERR_STATION_NAME_LENGTH     -1

/**
 * @def ERR_FIRMWARE_VERSION_BAD
 *      Error code for Wrong firmware version
 */
#define ERR_FIRMWARE_VERSION_BAD    -2

/**
 * @def ERR_FIRMWARE_LOAD_FAIL
 *      Error code for failure in firmware loading
 */
#define ERR_FIRMWARE_LOAD_FAIL      -3

/**
 * @def ERR_DRIVER_INIT_FAIL
 *      Error code for failure in initializing driver
 */
#define ERR_DRIVER_INIT_FAIL        -4

#define PRU_IEP_CMP_CFG_EN_SOG_EVENTS   0x000001cf  /*sets cmp0, cmp2/5/6/7*/

/**
 * @def NO_RCV_NO_FWD
 *      No receive and no forward
 */
#define NO_RCV_NO_FWD 0

/**
 * @def RCV_NO_FWD
 *      Receive but no forward
 */
#define RCV_NO_FWD    1

/**
 * @def NO_RCV_FWD
 *      No receive but forward
 */
#define NO_RCV_FWD    2

/**
 * @def RCV_FWD
 *      Receive and forward
 */
#define RCV_FWD       3

/**
 * @def OFF
 *      RTClass3 status values of a port
 */
#define OFF                         0

/**
 * @def UP
 *      RTClass3 status values of a port
 */
#define UP                          2

/**
 * @def RUN
 *      RTClass3 status values of a port
 */
#define RUN                         4

/** @addtogroup PN_MRP
 @{ */

/**
 * @def DISABLED
 *      MRP Port disabled state
 */
#define DISABLED                    0

/**
 * @def BLOCKING
 *      MRP Port blocking state
 */
#define BLOCKING                    1
/**
 * @def FORWARDING
 *      MRP Port forwarding state
 */
#define FORWARDING                  2

#ifdef WATCHDOG_SUPPORT
/**
 * @def watchDogExpireDuration
 *      ICSS WatchDog Expiry duration in milli seconds
 */
#define watchDogExpireDuration      100
#endif


/**
@}*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * @brief Initializes the Profinet Driver
 *
 * Sets the Port MAC addresses \n
 * Sets the Compensation value \n
 * Initialize the RTC driver   \n
 * Initialize the PTCP driver  \n
 * Set MRP port state \n
 * Loads the forward and receive multicast tables \n
 * Loads the Profinet firmware
 *
 * @param pnHandle Profinet Handle
 * @retval 0 on success     \n
 *  Error codes :   \n
 *      @ref ERR_DRIVER_INIT_FAIL \n
 *      @ref ERR_FIRMWARE_VERSION_BAD
 */
int32_t PN_initDrv(PN_Handle pnHandle);

/** @addtogroup PN_DCP_FILTER
 @{ */
/**
 * @brief API to configure the station name and length of station name of device for filtering DCP Identify requests.
 *
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] dcpNameOfStation      Pointer to the array which contains the station name
 * @param[in] lengthOfStationName   Length of the station name. Valid values are from 0 to 240.
 *                                  If the length is configured as zero then all DCP Ident request are passed to host.
 *
 * @retval 0 on success     \n
 *  Error codes :   \n
 *      @ref ERR_STATION_NAME_LENGTH
 */
int32_t PN_setDcpFilterStationName(ICSS_EmacBaseAddressHandle_T
                                   ICSS_EmacBaseAddressHandle, const uint8_t *dcpNameOfStation,
                                   uint8_t lengthOfStationName);

/**
@}
*/

/**
 * @brief API to set the MRP state for a port
 *
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port for which MRP state has to be configured \n
 *              ICSS_EMAC_PORT_1 (1) \n
 *              ICSS_EMAC_PORT_2 (2)
 * @param[in] pState There are three valid values of the MRP port state \n
 *              @ref DISABLED \n
 *              @ref BLOCKING \n
 *              @ref FORWARDING
 * @retval 0 on success
 */
int32_t PN_MRP_setPortState(ICSS_EmacBaseAddressHandle_T
                            ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t pState);

/**
 * @brief Returns the MRP state of a port
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port number whose MRP port state has to be returned \n
 *              ICSS_EMAC_PORT_1 (1) \n
 *              ICSS_EMAC_PORT_2 (2)
 * @param[in] pState Reference to the MRP port state of the port \n
 *
 * @retval 0 on success
 */
int32_t PN_MRP_getPortState(ICSS_EmacBaseAddressHandle_T
                            ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t *pState);

/** @addtogroup PN_PHASE_MANAGEMENT
 @{ */
/**
 * @brief API to map a Phase to start of green Profile.
 *
 * A phase can be mapped to different profiles for Receive (Rx) and Transmit (Tx) on a port.
 *

 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port on which phase to profile mapping has to be done \n
 *              ICSS_EMAC_PORT_1 (1) \n
 *              ICSS_EMAC_PORT_2 (2)
 * @param[in] phaseNumber IRT Phase Number \n
 *              Valid values are from 1 to 16
 * @param[in] profileNumberRx Profile number mapped for Receive \n
 *              Valid values are from 1 to 5
 * @param[in] profileNumberTx Profile number mapped for Transmit \n
 *              Valid values are from 1 to 5
 *
 * @retval 0 on success
 */
int32_t PN_mapPhaseToProfile(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, int32_t portNumber, int32_t phaseNumber,
                             int32_t profileNumberRx, int32_t profileNumberTx);



/**
 * @brief API to configure the maximum line receive delay.
 *
 * This value is provided by the PLC when it establishes connection with the device.
 *

 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port for which receive line delay is configured \n
 *              ICSS_EMAC_PORT_1 (1) \n
 *              ICSS_EMAC_PORT_2 (2)
 * @param[in] maxLineRxDelayValue Line delay as seen by the PLC at a port. This value is computed by the PLC/Engineering tool.
 *
 * @retval 0 on success
 */
int32_t PN_setMaxLineRxDelay(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint8_t portNumber, int32_t maxLineRxDelayValue);

/**
 * @brief API to configure the maximum bridge delay. This value comes from the GSD file.
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] maxBridgeDelayValue \n
 *              Valid value for our device is 2900ns \n
 *              This value is defined in the GSD
 *
 * @retval 0 on success
 */
int32_t PN_setMaxBridgeDelay(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, int32_t maxBridgeDelayValue);


/**
 * @brief API to configure the yellow period.
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] yellowPeriodTime Length of the yellow period \n
 *              Valid value is 125 us as our device does not support fragmentation
 *
 * @retval 0 on success
 */
int32_t PN_setYellowPeriod(ICSS_EmacBaseAddressHandle_T
                           ICSS_EmacBaseAddressHandle, int32_t yellowPeriodTime);


/**
 * @brief API to configure a Profile.
 * In a profile the start of green time in a cycle for Receive (Rx) and Transmit (Tx) is set for a port.
 *
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port for which a profile is configured \n
 *              PORT1 (1)    \n
 *              PORT2 (2)
 * @param[in] profileNumber Five profiles can be defined for a port \n
 *              Valid values are from 1 to 5
 * @param[in] rxSoGValue Start of Green value for receive at the port \n
 *              Valid value can range from 0 to the max of cycle time
 * @param[in] txSoGValue Start of Green value for transmit at the port \n
 *              Valid value can range from 0 to the max of cycle time
 *
 * @retval 0 on success
 */
int32_t PN_setProfile(ICSS_EmacBaseAddressHandle_T ICSS_EmacBaseAddressHandle,
                      int32_t portNumber, int32_t profileNumber, int32_t rxSoGValue,
                      int32_t txSoGValue);

/**
 * @brief API to configure the compensation value which is used while computing the forward FSO for a RTC3 frame which has to be forwarded.
 * @param[in] compensationValue  compensation for the jitter
 *              Valid range is from 0 to 100
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @retval 0 on success
 */
void PN_setCompensationValue(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint16_t compensationValue);


/**
 * @brief API to set the Red Guard for the device.
 *
 * Received RTC3 frames are dropped if their FID doesn't fall in the range set by the Red Guard.
 *
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] validLowerFrameId Lower value of RTC3 FID for the Red Guard \n
 *              Valid range is 0x0100 to 0x0FFF \n
 * @param[in] validUpperFrameId \n
 *              Valid range is 0x0100 to 0x0FFF and equal or greater then validLowerFrameId \n
 * @retval 0 on success
 */
int32_t PN_setRedGuard(ICSS_EmacBaseAddressHandle_T ICSS_EmacBaseAddressHandle,
                       uint16_t validLowerFrameId, uint16_t validUpperFrameId);

/**
 * @brief API to set the RTClass3 port status of a port.
 *
 * If the status of a port is set to UP or RUN then firmware internally enables Phase
 *        Management on that port.
 *
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] portNumber Port whose status has to be configured \n
 *              ICSS_EMAC_PORT_1 (1)     \n
 *              ICSS_EMAC_PORT_2 (2)
 * @param[in] status There are three valid values of the RTClass3 port status \n
 *              @ref OFF \n
 *              @ref UP\n
 *              @ref RUN
 * @retval 0 on success
 */
int32_t PN_setRtc3PortStatus(ICSS_EmacBaseAddressHandle_T
                             ICSS_EmacBaseAddressHandle, uint8_t portNumber, uint8_t status);
/**
@}
*/

/** @addtogroup PN_FILTER_TABLE
 @{ */
/**
 * @brief API to enable/disable the reception & forward for a Multicast address in port multicast filter tables.
 * @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
 * @param[in] macAddr MAC Address for which the reception or forward has to be enabled or disabled \n
 *              Following are the valid ranges of multicast addresses: \n
 *              (01-0E-CF-00-00-00 TO 01-0E-CF-00-05-FF) \n
 *              (01-15-4E-00-00-00 TO 01-15-4E-00-00-1F) \n
 *              (01-80-C2-00-00-00 TO 01-80-C2-00-00-1F)
 * @param[in] ctrl  A MAC address can be configured using one of four valid configuration for reception and foward \n
                For example, if MAC address is called with NO_RCV_NO_FWD then it's reception and forwarding will be disabled \n
 *              @ref NO_RCV_NO_FWD \n
 *              @ref RCV_NO_FWD \n
 *              @ref NO_RCV_FWD \n
 *              @ref RCV_FWD
 * @param[in] portNumber Port on which reception and forwarding is configured for a MAC address
 *              PORT1 (1) \n
 *              PORT2 (2) \n
 * @param[in] table There are two sets of table one for BLOCKING state and second one for FORWARDING state \n
 *              @ref BLOCKING\n
 *              @ref FORWARDING
 * @retval 0 on success
 */
int32_t PN_setStaticFilterTable(ICSS_EmacBaseAddressHandle_T
                                ICSS_EmacBaseAddressHandle, const uint8_t *macAddr, uint8_t ctrl,
                                uint8_t portNumber, uint8_t table);
/**
@}
*/
#ifdef WATCHDOG_SUPPORT
/** @addtogroup PN_WATCHDOG
 @{ */
 
/**
* @brief API to configure the ICSS watchdog expiry duration.
* @param[in] ICSS_EmacBaseAddressHandle ICSS Emac LLD HW attrs
* @param[in] timerPeriod Time duration (in milliseconds) after which watchdog expires if not tapped in-between.
*              Valid range is from 2 ms to 6553 ms
* @retval 0 on success
*/

int32_t PN_setWatchDogTimer(PN_Handle pnHandle, int32_t timerPeriod);

/**
@}
*/
#endif
/**
 *  @ingroup
 *  @brief   PN_loadStaticTable API to load multicast static table to ICSS memory
 *  @details API to load multicast static table to ICSS memory
 *
 *  @param[in]  pCfg
 *  @param[in]  staticTable start  address of the static table
 *  @param[in]  staticTableLength  length of the static table in bytes
 *  @param[in]  staticTableType  type of static table 0 == Forward Table, 1 == Receive Table
 *  @retval     0 on sucess or <0 on failure
 */

int32_t PN_loadStaticTable(ICSS_EmacBaseAddressHandle_T
                           ICSS_EmacBaseAddressHandle,
                           const uint32_t *staticTable,
                           uint8_t staticTableLength,
                           uint8_t staticTableType,
                           uint8_t portNumber);

/**
 *  @ingroup
 *  @brief   Profinet CPM ISR handler
 *  @details
 *
 *  @param[in]  arg
 *  @retval     null
 */
void PN_cpmIsrHandler(void* arg);
/**
 *  @ingroup
 *  @brief   Profinet PPM ISR handler
 *  @details
 *
 *  @param[in]  arg
 *  @retval     null
 */
void PN_ppmIsrHandler(void* arg);
/**
 *  @ingroup
 *  @brief   Profinet DHT ISR handler
 *  @details
 *
 *  @param[in]  arg
 *  @retval     null
 */
void PN_dhtIsrHandler(void* arg);
/**
 *  @ingroup
 *  @brief   Profinet PTCP ISR handler
 *  @details
 *
 *  @param[in]  arg
 *  @retval     null
 */
void PN_PTCP_isrHandler(void* arg);
#endif /* IPNDRV_H_ */
