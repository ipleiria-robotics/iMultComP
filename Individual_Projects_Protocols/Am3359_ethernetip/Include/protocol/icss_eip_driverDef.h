/**
* @file  icss_eip_driverDef.h
*
* @brief Contains the main EtherNet/IP handle structure and all other members
*
*\par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*\par
*/

/*! \mainpage EtherNet/IP Adapter API
*
* \version 01.00.03.04
*
* \section intro_sec Introduction
*
* PRU-ICSS EtherNet/IP firmware implements EtherNet/IP adapter functionality and provides EtherNet/IP \n
* functionality integrated into AM3357/AM3359/AM4377/AM4379/AM571x/AM572x/K2G SOC with PRU-ICSS IP.
*
* \section eip_purpose Purpose and Scope
* Purpose of this document is to assist developers porting their EtherNet/IP adapter stack to \n
*  TI AM3357/9 or AM4377/9 or AM57xx or K2G SOCs.
*
* \section eip_audience Intended Audience
* -System integrators using TI EtherNet/IP adapter in their products \n
* -EtherNet/IP adapter stack vendors to support TI EtherNet/IP Adapter solution
*
* \section eip_arch Software Architecture
* @image html eip_sw_architecture.png
*
* \section pn_kickoff Procedure to kickoff EtherNet/IP firmware
* 1. Initialize EMAC-LLD to work as a switch
* 2. Initialize the required tasks and interrupts
* 3. Initialize the EtherNet/IP Driver, PTP, DLR and PRU-ICSS INTC \n
* 4. Register the callbacks \n
* 5. Load EtherNet/IP firmware into PRUs of PRU-ICSS \n
* 6. Initialize the EtherNet/IP slave stack \n
* 7. Start firmware \n
* 8. Handle the events as needed - The event/ISR definitions are availale in developer guide, these have
* respective callbacks that can be registered to custom stack APIs  \n
*
* \section eip_fw_api Firmware APIs
* These APIs implement the key interface between EtherNet/IP firmware and Slave stack. They are
* implemented in following source code files \n
* <b>TI h/w access layer to support EtherNet/IP slave using PRU-ICSS</b> \n
* protocols\ethernetip_adapter\drivers\icss_dlr.c \n
* protocols\ethernetip_adapter\include\icss_dlr.h \n
* protocols\ethernetip_adapter\drivers\icss_eip_mcFltr.c \n
* protocols\ethernetip_adapter\include\icss_eip_mcFltr.h \n
* protocols\ethernetip_adapter\include\icss_dlrDef.h \n
* protocols\ethernetip_adapter\include\icss_switch_dlr.h \n
* protocols\ethernetip_adapter\drivers\icss_eip_driver.c \n
* protocols\ethernetip_adapter\include\icss_eip_driver.h \n
* protocols\ethernetip_adapter\include\icss_eip_driverDef.h \n
* protocols\timeSync\drivers\icss_timeSync.c \n
* protocols\timeSync\drivers\icss_timeSync_ecap_edma.c \n
* protocols\timeSync\drivers\icss_timeSync_init.c \n
* protocols\timeSync\drivers\icss_timeSync_osal.c \n
* protocols\timeSync\drivers\icss_timeSyncApi.c \n
* protocols\timeSync\include\icss_timeSync.h \n
* protocols\timeSync\include\icss_timeSync_ecap_edma.h \n
* protocols\timeSync\include\icss_timeSync_init.h \n
* protocols\timeSync\include\icss_timeSync_memory_map.h \n
* protocols\timeSync\include\icss_timeSync_osal.h \n
* protocols\timeSync\include\icss_timeSyncApi.h \n
* <b>PRU_ICSS EtherNet/IP firmware headers</b> \n
* protocols\ethernetip_adapter\firmware\PRU_ICSS_VERSION\ethernetip_adapter_pru0_bin.h  \n
* protocols\ethernetip_adapter\firmware\PRU_ICSS_VERSION\ethernetip_adapter_pru1_bin.h  \n
*
* \section install_sec Installation
*
* This driver is installed as part of PRU-ICSS EtherNet/IP Adapter Package
*
* \section Documentation
* <a href="pages.html">Module Documentation</a>
*
* \section terms_abbrevs Term and Abbreviations
* Abbreviation  | Expansion
* ------------- | -------------
* PRUSS         | Programmable RealTime Unit Sub System
* PRU-ICSS      | Programmable RealTime Unit - Industrial Communication Sub System - PRUSS with industrial communication support
* DLR           | Device Level Ring (A redundancy protocol)
* PTP           | Precision Time Protocol
* NDK           | Network Development Kit (TCP/IP Stack)
* OC            | Ordinary Clock
* TC            | Transparent Clock
* E2E           | End to End
*/

#ifndef ICSS_EIP_DRIVERDEF_H_
#define ICSS_EIP_DRIVERDEF_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <Include/protocol/timeSync/icss_timeSyncApi.h>
/**
 * @brief Type of clock supported by PTP Implementation. This is not specified by standard
 */
typedef enum
{
    /**Ordinary clock/End Node*/
    ORDINARY_CLOCK = 0,
    /**Transparent clock*/
    TRANSPARENT_CLOCK = 1,
    /**Transparent clock*/
    BOUNDARY_CLOCK = 2

} clockType;

/**
 * @brief Specifies the properties of a clock
 */
typedef struct
{
    /**Unique identifier for Clock, See standard for exact definition*/
    Uint8 clockIdentity[8];
    /**Specifies class of clock Quality*/
    Uint16 clockClass;
    /**Accuracy in seconds. See spec for more detail*/
    Uint16 timeAccuracy;
    /**OffsetScaledLogVariance is the variance measure of clock quality used by the Best Master
    algorithm to determine the grandmaster.*/
    Uint16 offsetScaledLogVariance;
    /**CurrentUtcOffset specifies the current UTC offset in
    seconds from International Atomic Time
    (TAI) of the clock. As of 0 hours 1 January 2006 UTC,
    the offset was 33 seconds*/
    Uint16 currentUTCOffset;
    /**TimePropertyFlags specifies the time property flags of the clock.*/
    Uint16 timePropertyFlags;
    /**TimeSource specifies the primary time source of the clock*/
    Uint16 timeSource;

    /**Same as offsetScaledLogVariance. Observed value.
     * Applicable for Grandmaster and Parent*/
    Uint16 observedOffsetScaledLogVariance;

    /**specifies an estimated measure of the parent clock�s drift as
        observed by the slave clock */
    Uint16 observedPhaseChangeRate;

} clockClass_t;

/**
 * @brief Description in unicode
 */
typedef struct
{
    /**number of unicode characters, capped to 32 characters*/
    Uint32 size;
    /**Stores unicode characters*/
    Uint8 descr[32];
} descr_t;

/**
 * @brief Specifies the PTP profile of each port of the device
 */
typedef struct
{
    Uint8 portProfileIdentity[8];
} portProfileIdentity_t;

/**
 * @brief Specifies the physical protocol and physical address (e.g. IEEE 802.3)
    of each port of the device (e.g. MAC address). The maximum number of characters is 16.
    Unused array elements are zero-filled
 */
typedef struct
{
    Uint8 physicalProtocol[16];
    Uint16 sizeOfAddress;
    Uint8 portPhysicalAddress[16];
} portPhysAddr_t;

/**
 * @brief Specifies the specifies the network and protocol address of each port of the device
    (e.g. IP address). The Network Protocol specifies the protocol for the network.
 */
typedef struct
{
    Uint16 portNumber;
    Uint16 networkProtocol;
    Uint16 addressSize;
    Uint8 portProtocolAddress[16];
} portProtAddr_t;

/**
 * @brief Specifies the system time in microseconds and
 *  the Offset to the local clock value
 */
typedef struct
{
    uint64_t systemTime;
    uint64_t systemOffset;
} sysTimeOffset_t;

/**
 * @brief CIP Sync configuration. Instance Attribute for
 * PTP Class (Class Code 0x43. CIP Spec Vol 1)
 */
typedef struct
{
    /**If PTP is enabled on system. Only applicable for OC*/
    uint8_t ifPTPEnable;
    /**If PTP is synchronized with Master*/
    uint8_t IsSynchronized;
    /**Current system time in Microseconds*/
    uint64_t systemTimeMicrosec;
    /**Current system time in Nanoseconds*/
    uint64_t systemTimeNanosec;
    /**Offset between local clock and master clock*/
    int64_t offsetFromMaster;
    /**Maximum offset between local clock and master clock*/
    uint64_t maxOffsetFromMaster;
    /**Mean path delay to master*/
    int64_t meanPathDelayToMaster;
    /**Grand master clock info*/
    clockClass_t *grandMasterClkInfoPtr;
    /**Parent clock info*/
    clockClass_t *parentClkInfoPtr;
    /**Local clock info*/
    clockClass_t *localClkInfoPtr;
    /**Number of PTP Ports on the device. See Spec*/
    uint16_t numberOfPorts;
    /** Status of PORT. PTP State machine values. See Spec*/
    port_State portState[NUM_PORTS];
    /**Port Enable/Disable*/
    uint16_t portEnable[NUM_PORTS];
    /**PTP announce interval between successive �Announce�
     messages issued by a master clock on each PTP port of the device*/
    uint16_t portLogAnnounceInterval[NUM_PORTS];
    /**specifies the PTP sync interval between successive �Sync�
     messages issued by a master clock on each PTP port of the device */
    uint16_t portLogSyncInterval[NUM_PORTS];

    /**attribute allows the user to override the automatic selection of the
    best master clock before any quality measures are evaluated*/
    uint8_t priority1;
    /**Same as above with a difference that it is used after clock is chosen*/
    uint8_t priority2;

    /**Domain Number as per PTP algorithm*/
    uint8_t domainNumber;
    /**Clock Type. OC, TC, BC etc*/
    clockType clockType;
    /**OUI given by IEEE*/
    uint8_t manufacturerIdentity[4];
    /**Product Type, Product description in Unicode format*/
    descr_t productType;
    /**Revision data of Clock, Firmware and Software. In Unicode*/
    descr_t revData;
    /**Description of device that contains the clock*/
    descr_t userDesc;

    /**the PTP profile of each port of the device*/
    portProfileIdentity_t profileInfo[NUM_PORTS];
    /**Physical Address Info of each port*/
    portPhysAddr_t physInfo[NUM_PORTS];
    /**Protocol Address Info of each port*/
    portProtAddr_t protInfo[NUM_PORTS];

    /**number of communication paths traversed between
      the local clock and the grandmaster clock*/
    uint16_t stepsRemoved;
    /**Specifies the system time in microseconds and the Offset to the local clock value*/
    sysTimeOffset_t timeOffset;

} cipSyncConfig_t;

/*
*  @brief     EIP Config
*             Structure storing the EIP info
*/
typedef struct eip_Config_s
{
    ICSS_EmacHandle emacHandle;
    TimeSync_ParamsHandle_t timeSyncHandle;
    cipSyncConfig_t *cipSyncObj;
    /**Temporary placeholder to copy packets*/
    uint8_t  tempFrame[ICSS_EMAC_MAXMTU];
    EIP_DLRHandle dlrHandle;
} eip_Config;

typedef struct eip_Config_s *EIP_Handle;
#endif /* ICSS_EIP_DRIVERDEF_H_ */
