/**
* @file  icss_dlrDef.h
*
* @brief Contains definition for DLR handle, enum types and structure members. DLR CIP Object is also defined here.
*
*\par
* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*\par
*/

#ifndef ICSS_DLRDEF_H_
#define ICSS_DLRDEF_H_

#define DEFAULT_DLR_PACKET_SIZE         60
/* ========================================================================== */
/*                              Enums                                         */
/* ========================================================================== */
/**
 * @brief node state machine states
 */
typedef enum
{
    /**idle state*/
    NODE_IDLE = 0,
    /**fault state*/
    NODE_FAULT = 1,
    /**normal state*/
    NODE_NORMAL = 2
} nodeState;
/**
 * @brief ring state values
 */
typedef enum
{
    /**normal ring*/
    RING_NORMAL = 1,
    /**faulty ring*/
    RING_FAULT = 2
} ringState;

/**
 * @brief network topology : possible values for Attribute ID 1
 */
typedef enum
{
    /**linear topology*/
    LINEAR_TOP = 0,
    /**ring topology*/
    RING_TOP = 1
} nwTopology;

/**
 * @brief network status : possible values for Attribute ID 2
 */
typedef enum
{
    /**normal status*/
    NORMAL_STAT = 0,
    /**ring fault status*/
    RING_FAULT_STAT = 1,
    /**unexpected loop detected*/
    UNEXPECTED_LOOP = 2,
    /**ring is partially faulty*/
    PARTIAL_FAULT = 3,
    /**there is rapid connection/reset*/
    RAPID_FAULT = 4
} nwStatus;

/**
 * @brief device role: possible values for Attribute ID 3
 */
typedef enum
{
    /*indicates the node is functioning as a backup*/
    BACKUP_NODE = 0,
    /*indicates the device is functioning as the active ring supervisor*/
    ACTIVE_RING_SUPERVISOR = 1,
    /*indicates the device is functioning as a normal ring node*/
    RING_NODE = 2,
    /*indicates the device is operating in a non-DLR topology*/
    NON_DLR = 3,
    /**indicates the device cannot support the currently operating
      ring parameters (Beacon Interval and/or Beacon Timeout)*/
    FEATURE_UNSUPPORTED = 4

} supervisorStatus;

/* ========================================================================== */
/*                              Data Structures                               */
/* ========================================================================== */

/**
 * @brief List of MAC ID's which are exempted from Learning, this is for DLR implementation
 */
typedef struct
{
    /**MAC ID of supervisor which is exempt from learning*/
    uint8_t MAC[6];
    /**number of exceptions, this is a flag where value greater than 1 indicates that exception exists*/
    uint8_t numExceptions;

} exceptionList;

/**
 * @brief State machine variables, part of DLR Object and L2 implementation
 */
typedef struct
{
    /**device state. Internal*/
    uint8_t node_state;
    /**ring state. Internal*/
    uint8_t ring_state;
    /**network topology. Attribute ID 1*/
    uint8_t topology;
    /**network status. Attribute ID 2*/
    uint8_t status;
    /**supervisor status. Attribute ID 3*/
    uint8_t superStatus;

} dlrStateMachineVar;

/**
 * @brief Supervisor configuration. Attribute ID 4
 */
typedef struct
{

    /** device configured to run as supervisor
     * default : false
     */
    uint8_t  superEnable;
    /**beacon interval value (microseconds)*/
    uint32_t beaconInterval;
    /**beacon timeout value (microseconds)*/
    uint32_t beaconTimeout;

    /**VLAN Id*/
    uint16_t vLanId;

    /**Supervisor precedence*/
    uint8_t supPrecedence;

} superConfig;

/**
 * @brief Supervisor address, part of DLR Object. Attribute ID 10
 */
typedef struct
{

    /**supervisors mac address*/
    uint8_t supMACAddress[6];

    /**supervisor ip address*/
    uint32_t supIPAddress;

} activeSuperAddr;

/**
 * @brief Last active node at the end of the chain
 * Class for Attributes 6 and 7
 */
typedef struct
{
    /**IP Address of the last active Node*/
    uint32_t ipAddr;
    /**MAC of the last node*/
    uint8_t macAddr[6];

    /**from 0-N where N is the number of nodes (excluding supervisor) in the ring*/
    uint16_t deviceNum;
} lastActiveNode;

/**
 * @brief IP and MAC of the Ring devices. Attribute ID 9
 */
typedef struct
{
    uint32_t ipAddr;
    uint8_t macAddr[6];
} protocolParticipants;

/**
 * @brief DLR parent structure through which all other structures can be accessed
 */
typedef struct
{
    /**Supervisor config. Attribute ID 4*/
    superConfig supConfig;
    /**Supervisor address. Attribute ID 10*/
    activeSuperAddr addr;
    /**State Machine variables. Attributes 1 through 3*/
    dlrStateMachineVar SMVariables;

#ifdef IS_A_DLR_SUPERVISOR
    /**Attribute ID's 6 and 7*/
    lastActiveNode activeNode[2];
#endif
    /**DLR Capabilities, flag
    The Map is as follows
     * 0 : Announce Based Ring Node
     * 1 : Beacon based Ring Node
     * 2-4 : Reserved
     * 5 : Supervisor capable
     * 6 : Redundant capable
     * 7 : Flush Table frame capable
     * 8-31 : Reserved
     */
    /**DLR Capabilities of the device. Attribute ID 12*/
    uint32_t dlrCapabilities;

    /**Active Supervisor precedence. Attribute ID 11*/
    uint8_t activeSuperPred;

    /**DLR Port 0 Interrupt number for ARM*/
    uint8_t port0IntNum;

    /**DLR Port 1 Interrupt number for ARM*/
    uint8_t port1IntNum;

    /**DLR interrupt for beacon timeout for Port 0*/
    uint8_t beaconTimeoutIntNum_P0;

    /**DLR interrupt for beacon timeout for Port 1*/
    uint8_t beaconTimeoutIntNum_P1;

#ifdef IS_A_DLR_SUPERVISOR
    /**Number of Ring Faults since power up. Attribute ID 5*/
    uint32_t numRingFaultsPowerUp;

    /**Number of ring participants. Attribute ID 8*/
    uint16_t ringParticipantsCount;

    /**pointer to array of structures. Attribute ID 9*/
    protocolParticipants **ringNodes;
#endif

} dlrStruct;


typedef struct dlr_Config_s
{
    dlrStruct *dlrObj;
    ICSS_EmacHandle emacHandle;
    /**Device IP*/
    uint32_t deviceIP;
    /**Sequence ID as per DLR spec*/
    uint32_t sequenceID;
    /**Learning table exception for MAC ID*/
    exceptionList *exclusionList;
    /**Clock handle for neighbor timeout timers**/
    void *dlrNeighborTimeoutClock[NUM_PORTS];
    /*Clock handle for DLR periodic processing*/
    void *dlrPeriodicTimer;
    /**Buffer used for General DLR Messages **/
    uint8_t dlrEmptyFrame[DEFAULT_DLR_PACKET_SIZE];
    /**Variable which keeps count of whether loop exists*/
    uint8_t checkForLoop;
    uint32_t   tracePktIntervalCount;
    uint32_t   stateMachineCount;
    uint8_t    pktSendCounter;
    uint8_t ISRcountPort[NUM_PORTS];

} dlr_Config;

typedef struct dlr_Config_s *EIP_DLRHandle;
#endif /* ICSS_DLRDEF_H_ */
