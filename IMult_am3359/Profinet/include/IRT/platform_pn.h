/*
*---------------------------------------------------------------------------*
*  Copyright (C) 2012 Woodhead Software & Electonics. All rights reserved.  *
*---------------------------------------------------------------------------*
*  This program is protected by international copyright.                    *
*                                                                           *
*  The use of this software including but not limited to its Source Code    *
*  is subject to restrictions as agreed in the license agreement between    *
*  you and Woodhead.                                                        *
*  Copying or distribution is not allowed unless expressly permitted        *
*  according to your license agreement with Woodhead.                       *
*---------------------------------------------------------------------------*
*                                                                           *
*  Project             : STACK PROFINET Device                              *
*  Component           : OS RELATION                                        *
* $Workfile:: platform_pn.h                                                $*
* $Revision:: 1                                                            $*
* $Date:: 5/01/16 9:56                                                     $*
*                                                                           *
*                                                                           */
/****************************************************************************/
/**
* @file platform_pn.h
*
*
*****************************************************************************/


#ifndef _PLATFORM_PN_H
#define _PLATFORM_PN_H

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Indicates that the Stack is going to be compiled as a Profinet IO-Device.
*
* This compilation constant indicates that the Stack is going to be compiled as a Profinet IO-Device.
*
* This constant competes with the IOCONTROLLER constant.
*
* This stack version does not support the use of the IOCONTROLLER constant.
*
* If IODEVICE is not defined, it will not be possible to compile the Stack.
*
*
*****************************************************************************/
#define IODEVICE

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @def PLATFORM_BIG_ENDIAN
* @brief Indicates to the Stack that the target is in big endian.
*
* One of the two compilation constants (@ref PLATFORM_BIG_ENDIAN and
* @ref PLATFORM_LITTLE_ENDIAN) must be defined in order to indicate to
* the Stack that the target is in big endian or little endian format.
*
* If both constants or neither constant is defined, it will not be possible to compile
* the Stack. The platform.h file integrates a define validation area.
*
* @hideinitializer
*****************************************************************************/
/*#define PLATFORM_BIG_ENDIAN          *//* motorola : */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @def PLATFORM_LITTLE_ENDIAN
* @brief Indicates to the Stack that the target is in little endian.
*
* One of the two compilation constants (@ref PLATFORM_BIG_ENDIAN and
* @ref PLATFORM_LITTLE_ENDIAN) must be defined in order to indicate to
* the Stack that the target is in big endian or little endian format.
*
* If both constants or neither constant is defined, it will not be possible to compile
* the Stack. The platform.h file integrates a define validation area.
*
* @hideinitializer
*****************************************************************************/
#define PLATFORM_LITTLE_ENDIAN      /* intel*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Reserved
*
* This constant is reserved. Must be equal to 1.
*
* @hideinitializer
*****************************************************************************/
#define MAX_DEVICE                  1


/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Reserved.
*
* This version of the stack does not support multi-instance.
* This constant is reserved. Must be equal to 1.
*
* @hideinitializer
*****************************************************************************/
#define MAX_INSTANCE_PER_DEVICE     1


/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of normal connections per instance.
*
* This constant defines the maximum number of normal connections that the IO-Device can manage per instance.
*
* @hideinitializer
*****************************************************************************/
#define MAX_IOC_CONNEXION_PER_INSTANCE   1   /* Number of normal     connection accepted */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of supervisor connections per instance.
*
* This constant defines the maximum number of supervisor connections that the IO-Device can manage per instance.
*
* @hideinitializer
*****************************************************************************/
#define MAX_IOS_CONNEXION_PER_INSTANCE  1   /* Number of supervisor connection accepted    */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of implicit connections per instance.
*
* This constant defines the maximum number of implicit connections that the IO-Device can manage per instance.
*
* @hideinitializer
*****************************************************************************/
#define MAX_IMP_CONNEXION_PER_INSTANCE   1   /* Number of implicit   connection accepted    */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the total number of AR accepted by the stack.
*
* This constant defines the total number of AR accepted by the stack.
* The value shall always be equal to the sum of all other AR.
* @warning Do not change this value
*
* @hideinitializer
*****************************************************************************/
#define MAX_ARD_PER_DEVICE            (MAX_IOC_CONNEXION_PER_INSTANCE + MAX_IOS_CONNEXION_PER_INSTANCE) /* Do not change this value*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines number of input CR per device. Shall be set to 1.
*
* This constant defines number of input CR per device. Shall be set to 1.
* @warning Do not change this value
*
* @hideinitializer
*****************************************************************************/
#define MAX_CR_IN_PER_AR  1 /* One input  CR: input  for controller then, PPM CR  */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines number of output CR per device. Shall be set to 1
*
* This constant defines number of output CR per device. Shall be set to 1
* @warning Do not change this value
*
* @hideinitializer
*****************************************************************************/
#define MAX_CR_OUT_PER_AR 1 /* One output CR: output for controller then, CPM CR  */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines number of input + output CR per device. Shall be set to 1.
*
* This constant defines number of input + output CR per device. Shall be set to @ref MAX_CR_IN_PER_AR + @ref MAX_CR_OUT_PER_AR.
* @warning Do not change this value
*
* @hideinitializer
*****************************************************************************/
#define MAX_CR_PER_AR (MAX_CR_IN_PER_AR+MAX_CR_OUT_PER_AR) /* One input CR and one output CR */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Reserved
*
* This constant is reserved. Must be equal to 0.
*
* @hideinitializer
*****************************************************************************/
#define API_RANGE_MIN 0 /* An Api can't be smallest than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of APIs supported by the Stack.
*
* This constant defines the maximum number of APIs supported by the Stack.
* If the DUA tries to add a module in an API greater than this constant, an error is generated.
*
* @hideinitializer
*****************************************************************************/
#define API_RANGE_MAX 1 /* An Api can't be Largest  than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the minimum number of the Slot supported by the Stack.
*
* This constant defines the minimum number of the Slot supported by the Stack.
* If the DUA tries to add a module in a slot smaller than this constant, an error is generated.
*
* @hideinitializer
*****************************************************************************/
#define SLOT_RANGE_MIN 0  /* A Slot can't be smallest than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of the Slot supported by the Stack.
*
* This constant defines the maximum number of the Slot supported by the Stack.
* If the DUA tries to add a module in a slot greater than this constant, an error is generated.
*
* @hideinitializer
*****************************************************************************/
#define SLOT_RANGE_MAX 12  /* A Slot can't be Largest  than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the minimum number of the Subslot supported by the Stack.
*
* This constant defines the minimum number of the subslot supported by the Stack.
* If the DUA tries to add a submodule in a subslot smaller than this constant, an error is generated.
*
* @hideinitializer
*****************************************************************************/
#define SUBSLOT_RANGE_MIN 0 /* A Subslot can't be smallest than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of the Subslot supported by the Stack.
*
* This constant defines the maximum number of the subslot supported by the Stack.
* If the DUA tries to add a submodule in a subslot greater than this constant, an error is generated.
*
* @hideinitializer
*****************************************************************************/
#define SUBSLOT_RANGE_MAX 0X8010 /* A Subslot can't be Largest  than this value */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the allocated ressources by IO-Device stack to manage real and expected configuration.
*
* This constant defines the allocated ressources by IO-Device stack to manage real and expected configuration.
* To be completed successfully, each PND_PlugSubModule function calls need an allocated configuration resource. In addition,
* the expected configuration that doesn't match a real configuration also needs an allocated resource.
*
* @warning When IO-Controller request connection on slot not configured in real configuration, stack
* need memory to manage it. This allocated ressource is for real configuration (Plug request) and expected configuration (only expected
* that has no real configuration associated).@n
* Sample 1:
*    Real configuration                               | Expected configuration                           | Result
*    ------------------------------------------------ | ------------------------------------------------ | -------------
*    API 1 Slot 0 Subslot 0 @n API 1 Slot 1 Subslot 0 | API 1 Slot 0 Subslot 1 @n API 1 Slot 1 Subslot 1 | Stack needs four allocated configuration ressources (One for @n each tuple) Api/Slot/Subslot.@n If no any ressources are available an out of ressources error is @n answering into the network.
* Sample 2:
*    Real configuration                               | Expected configuration                           | Result
*    ------------------------------------------------ | ------------------------------------------------ | -------------
*    API 1 Slot 0 Subslot 0 @n API 1 Slot 1 Subslot 0 | API 1 Slot 0 Subslot 0 @n API 1 Slot 1 Subslot 0 | Stack needs two allocated configuration ressources (One for @n each tuple) Api/Slot/Subslot.
*
* @hideinitializer
*****************************************************************************/
#define PN_CONFIGURATION_SIZE 50

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of interface submodule supported by IO-Device configuration structure.
*
* This constant defines the number of interface submodule supported by IO-Device configuration structure.
*
* An interface submodule is define in specification to have the subslot number has the form of 0xXY00 where:
*  -  X shall be set to 0x8 (or 0x9 in the case of System Redudnacy, for interface and related ports on the right or above delivering endnode may exist).
*  -  Y is in the range 0x0 up to 0xF. This parameters describe the interface: 0x8000 (Interface 1), 0x8100 (Interface 2), to 0x8F00 (Interface 16).
*
*   It's recommance to use 0x8000.
*   For certification at least one interface shall be define.
*
* @hideinitializer
*****************************************************************************/
#define PN_CONFIGURATION_ITF_SIZE           1

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of port submodule supported by IO-Device configuration structure.
*
* This constant defines the number of  port submodule supported by IO-Device configuration structure.
*
* A  port submodule is define in specification to have the subslot number has the form of 0xXYZZ where:
*  -  X shall be set to 0x8 (or 0x9 in the case of System Redudnacy, for interface and related ports on the right or above delivering endnode may exist).
*  -  Y is in the range 0x0 up to 0xF. This parameters describe the interface: 0x8000 (Interface 1), 0x8100 (Interface 2), to 0x8F00 (Interface 16).
*  - ZZ is in the range 0x1 up to 0xFF. This parameters describe the port. 0x1 correcpond to port 1.
*
*   It's recommance to use 0x8001 for port 1 and 0x8002 for port 2.....
*
* @hideinitializer
*****************************************************************************/
#define PN_CONFIGURATION_PORT_SIZE           WSE_LLDP_MAX_PORT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the size of the HashTable used to store the tuple Api/Slot/Subslot.
*
* This constant defines the size of the HashTable used to store the tuple Api/Slot/Subslot.
*
* If the HashTable is too large, memory will be lost (some part of the HashTable will never be used).
*
* If the HashTable is too small, some collisions can appear. A High number of collisions can degrade performance
* when using the HashTable.
* The localisation of an element in the Hahstable is generated by function getHashKey.
*
* @hideinitializer
*****************************************************************************/
#define HASH_SIZE (22)

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of multicast request managed simultaneously by the stack.
*
* This constant defines the number of multicast request managed simultaneously by the stack.
* The stack IO-Device doesn't answer directly to an identify request. When the request is received,
* a timer is started and all other requests are ignored. This behaviour can generate issue on
* network where identify all request is made (The IO-Device seems to be not present in network for other
* IO-Controller).@n
* This define allows stack to manage multiple request simultaneously thus even if multiple
* IO-Controller and engineering station made itendity request on network at the same time, the
* IO-Device is still present.
*
* @hideinitializer
*****************************************************************************/
#define MAX_DCP_MULTICAST_REQUEST 1

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the scheduler frequency.
*
* The constant defines the scheduler frequency. This constant's unit is in ms. Amongst other things this constant
* allows the minimum data exchange cycle time per IO-Device to be defined.
* This value shall be coherent with the scheduling time of the stack (see @ref OsWaitForTimer).
*
* @hideinitializer
*****************************************************************************/
#define SYSTEM_POLLING_DELAY 1

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of event functions managed by the scheduler.
*
* This constant defines the maximum number of event functions managed by the scheduler.
*
* @hideinitializer
*****************************************************************************/
#define MAX_CBF (5+MAX_DEVICE*10)

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Default IP address of the IO-Device.
*
* Default IP address of the IO-Device. This value must be used by the DUA when @ref PND_CB_DcpFactoryResetInd is
* indicated.
*
* For conformity, note that the Profinet V2.1 standard states that this value should be
* 0x0000000.
*
* Related callback: @ref PND_CB_DcpDeviceNameInd
*
* @hideinitializer
*****************************************************************************/
#define FACTORY_RESET_IP          0  /* This is the IP use when device receive a device factory reset : this ip must be form as a DWORD           */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Default sub-network mask for the IO-Device.
*
* Default sub-network mask for the IO-Device. This value must be used by the DUA when
* @ref PND_CB_DcpFactoryResetInd is indicated.
*
* For conformity, note that the Profinet V2.1 standard states that this value should be either "0.0.0.0", or
* 0x0000000.
*
* Related callback: @ref PND_CB_DcpDeviceNameInd
*
* @hideinitializer
*****************************************************************************/
#define FACTORY_RESET_SUBNETMASK  0  /* This is the sub net mask use when device receive a device factory reset : this sub net mask must be form as a DWORD */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Default gateway address for the IO-Device.
*
* Default gateway address for the IO-Device. This value must be used by the DUA when
* @ref PND_CB_DcpFactoryResetInd is indicated.
*
* For conformity, note that the Profinet V2.1 standard states that this value should be either "0.0.0.0", or
* 0x0000000.
*
* Related callback: @ref PND_CB_DcpDeviceNameInd
*
* @hideinitializer
*****************************************************************************/
#define FACTORY_RESET_GATEWAY     0  /* This is the Mask used when device receive a dcp factory reset : this gateway must be form as a DWORD*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Default name for the IO-Device.
*
* Default name for the IO-Device. This value must be used by the DUA when @ref PND_CB_DcpFactoryResetInd is indicated.
*
* For conformity, note that the Profinet V2.1 standard states that this value should be ' '.
*
* @hideinitializer
*****************************************************************************/
#define FACTORY_RESET_NAME        "" /* This is the name used when device receive a device factory reset */

/* FUNCTIONNAL COMPILATION CONSTANTS */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to "expected" and "real" configuration.
*
* This constant enables internal support of records related to "expected" and "real" configurations. When this constant is
* defined, read requests for these records are processed internally by the Stack and the DUA is not alerted to these
* requests. See @ref PND_CB_ReadInd.
*
* Following indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0x8000 | SubSlot Expected Identification Data
* 0x8001 | SubSlot Real identification Data
* 0xC000 | Slot Expected Identification Data
* 0xC001 | Slot Real identification Data
* 0xE000 | AR Expected Identification Data
* 0xE001 | AR Real identification Data
* 0xE002 | API Expected identification Data
* 0xF000 | API Real identification
* 0xF020 | ARData for one API
* 0xF820 | ARData
* 0xF821 | APIData
* 0xF840 | I&M0FilterData
*
*****************************************************************************/
#define RECORD_INTERNAL

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to I&M information.
*
* This constant enables internal support of records related to I&M information. When this constant is defined, read
* requests for these records are processed internally by the Stack and a new callback has to be managed by DUA. See
* @ref PND_CB_Get_Im0_Data.
*
* Following indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0xAFF0 | I&M0
* 0xF840 | I&MFilterData
*
* @warning This define only works when @ref RECORD_INTERNAL is defined.
*
* @hideinitializer
*****************************************************************************/
#define RECORD_INTERNAL_IM             /* sub definition of RECORD_INTERNAL: this one allow stack to answer I&M0 record with default values         (from define)          */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to interface information.
*
* This constant enables internal support of records related to interface information. When this constant is defined, read
* requests for these records are processed internally by the Stack and new callback has to be managed by DUA.
*
* Following indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0x8080 | Subslot PDInterface data real
* 0x802A | Subslot Port data Real
* 0x802B | Subslot Port data Check
* 0x802F | Subslot PDPort data Adjust
* 0xF842 | Device PDExpected Data
* 0xF841 | Device PDreal Data
*
* @warning This define only works when @ref RECORD_INTERNAL is defined.
*
* @hideinitializer
*****************************************************************************/
#define RECORD_INTERNAL_INTERFACE      /* sub definition of RECORD_INTERNAL: this one allow stack to answer interface record with default values    (from LLDP and define) */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to IO information.
*
* This constant enables internal support of records related to IO information. When this constant is defined, read
* requests for these records are processed internally by the Stack.
*
* Following indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0x8028 | Subslot Record Input Data
* 0x8029 | Subslot Record Output Data
*
* To get missing information to fulfill network buffer, stack will call following callback:
*                          - @ref PND_CB_Get_Input_data
*                          - @ref PND_CB_Get_Output_data
*                          - @ref PND_CB_Get_OutputSubsTitute_data
*
* @warning This define only works when @ref RECORD_INTERNAL is defined.
*
* @hideinitializer
*****************************************************************************/
#define RECORD_INTERNAL_IO             /* sub definition of RECORD_INTERNAL: this one allow stack to answer Input/Output record with default values (from PPM and CPM)     */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to Port statistic information.
*
* This constant enables internal support of records related to Port statistic information. When this constant is defined, read
* requests for these records are processed internally by the Stack.
*
* Following indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0x8072 | PDPortStatistic for one subslot
*
* To get missing information to fulfill network buffer, stack will call following callback:
*   - @ref PND_CB_Get_PortStatistic
*
*
* @warning This define only works when @ref RECORD_INTERNAL is defined.
*
* @hideinitializer
*****************************************************************************/
#define RECORD_INTERNAL_PORT_STATISTIC /* sub definition of RECORD_INTERNAL: this one allow stack to answer PDPortStatistic (statistic about port data)                    */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal support of records related to FSU information.
*
* This constant enables internal support of records related to Fast Start Up information. When this constant is defined, read
* requests for these records are processed internally by the Stack.
*
* Following read indexes are automatically managed by the stack once this define is used:
*
* Index  | Description
* ------ | ----------------
* 0xE050 | ARFSUDataAdjust data for one AR
* 0x8090 | Expected PDInterfaceFSUDataAdjust
*
* @hideinitializer
*****************************************************************************/
/*#define FSU_SUPPORTED*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Activate the automatic PortDataCheck management.
*
* When defined, the Read/Write PortDataCheck requets will be intercept and automatically managed:
* When a connection is established an IO-Controller can add some peers to be checked by IO-Device. In this case
* the IO-Device have to ensure that its peers are those expected by IO-Controller. If it is not, an alarm shall be sent from
* IO-Device to IO-Controller.
*
* @warning PROFINET IO-Device stack will based its tests on Alias name registered into the stack to check peers.
* @warning Stack will only manage the subblock CheckPeers automatically. All other subslocks will be sent to DUA.
*
* @hideinitializer
*****************************************************************************/
#define PN_TOPOLOGY_ACTIVATED

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of port checked for each Port.
*
* This constant defines the number of port checked by IO-Device per Port. This define is
* related to @ref PN_TOPOLOGY_ACTIVATED define. It shall be set to 1
*
* @hideinitializer
*****************************************************************************/
#define PN_MAX_PEER_CHECK_PORT             1

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Activate the automatic MRP management.
*
* When defined, the Read/Write MRP data will manage internally or using callback if additional information are necessary.
* When a connection is established an IO-Controller can try to change the MRP data current mode (Manager or Client).
*
* @warning PROFINET IO-Device stack will intercept requets and generate addional callback.
*
* @hideinitializer
*****************************************************************************/
#define RECORD_INTERNAL_MRP

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of port checked for each AREP.
*
* This constant defines the number of peers stored by IO-Device. This define is
* related to @ref PN_TOPOLOGY_ACTIVATED define.
*
* @hideinitializer
*****************************************************************************/
#define PN_MAX_ALIAS_PER_PORT        WSE_LLDP_REMOTE_SYSTEM_PORT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of Alias name supported by the IO-Device.
*
* This constant defines the number of alias name supported by the IO-Device.
* MAX_ALIAS_PER_DEVICE is shared for all IO-Device ports.
*
* @hideinitializer
*****************************************************************************/
#define PN_MAX_ALIAS_PER_DEVICE        (WSE_LLDP_MAX_PORT*PN_MAX_ALIAS_PER_PORT)

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables the DUA to receive output values from the IO-Device only when the values of its outputs change.
*
* If this constant is defined, the DUA receives output values from the IO-Device only when the values of its outputs change. See @ref PND_Get_Input.
* * @hideinitializer
*****************************************************************************/
/*#define NEW_INPUT_DATA_MODE*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables shared memory mode management.
*
* If this constant is defined, the IO-Device Input/Output and status will only be available
* by shared memory. Shared memory allows to access data without the messaging mode, thus performance can be increased.
* However the stack no longer 'check' the IOXS status and user have to manualy generate alarm due to
* IO management (return of submodule alarm).
*
* @hideinitializer
*****************************************************************************/
#define SHARED_MEMORY_MODE

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Represents the number of alarm low/high that stack will be able to queue.
*
* This constant represents the number of alarms low/high that stack will be able to queue.
*
* One queue is created for alarm low and another one for alarm high.
*
* If the stack has no place available in alarm queue it will return an error.
*
* With a maximal set to 2, there can be one alarm in process and one stored
* in queue. The third one will be refused and returned as error.
* The PROFINET specification 2.3 required at least a queue with minimum deep of 3.
*
* @hideinitializer
*****************************************************************************/
#define ALARM_MAX_QUEUE_LEVEL     8

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Used to define the smallest value accepted by the Stack as Send_Clock_Factor.
*
* This constant is used to define the smallest value accepted by the Stack as Send_Clock_Factor.
*
* @hideinitializer
*****************************************************************************/
#define MIN_SEND_CLOCK_FACTOR_VALUE  8

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Used to define the largest value accepted by the Stack as Send_Clock_Factor.
*
* This constant is used to define the largest value accepted by the Stack as Send_Clock_Factor.
*
* @hideinitializer
*****************************************************************************/
#define MAX_SEND_CLOCK_FACTOR_VALUE  128

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Used to define the smallest value accepted by the Stack as Reduction Ratio.
*
* This constant is used to define the smallest value accepted by the Stack as Reduction Ratio.
*
* @hideinitializer
*****************************************************************************/
#define MIN_REDUCTION_RATIO_VALUE  1

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Used to define an additional queue for addin of PROFINET stack.
*
* This constant is used to define an additional queue for addin of PROFINET stack
* (eg: MRP).
*
* @hideinitializer
*****************************************************************************/
/*#define ADDIN_QUEUE*/

/*-----------------------------------------------------------------------*/
/* MRP related define (need ADDIN_QUEUE define)                          */
/*-----------------------------------------------------------------------*/
/* #define MRP_SUPPORTED */
/* #define MRM_SUPPORTED */
/* #define MRC_SUPPORTED */

#ifdef MRP_SUPPORTED
  #define LLDP_SUPPORT_MRP
#endif

/*-----------------------------------------------------------------------*/
/* System Redundancy related define                                      */
/*-----------------------------------------------------------------------*/
/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables the stack to support System Redundancy.
*
* If this constant is defined, it enables the stack to support System Redundancy.
*
* @hideinitializer
*****************************************************************************/
/*#define PN_SR_SUPPORTED*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables Stack IO-Device to manage RTC3 communication (PTCP, LLDP).
*
* If this constant is defined, it enables Stack IO-Device to manage PTCP and LLDP TLV.
* These define is mandatory for Profinet IO (Class C).
* These defines shall be declared with PN_RTC3_SUPPORTED :
* PN_TOPOLOGY_ACTIVATED
* RT_CLASS_USE_BY_DEVICE = RT_CLASS_3
*
* @hideinitializer
*****************************************************************************/
#define PN_RTC3_SUPPORTED

/*-----------------------------------------------------------------------*/
/* LLDP related define                                                   */
/*-----------------------------------------------------------------------*/
/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables the stack to start and initialize the LLDP stack.
*
* If this constant is defined, it enables the stack to start and initialize the LLDP stack. Please refer to LLDP
* document for more details about configuring the switch.
*
* @note Remember that support of LLDP is mandatory for Profinet IO-Device.
*
* @hideinitializer
*****************************************************************************/
#define LLDP_SUPPORTED

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables the reception and the process of LLDP frames.
*
* If this constant is defined, it enables the reception and the process of LLDP frames. Please refer to LLDP
* document for more details about configuring the switch.
*
* @hideinitializer
*****************************************************************************/
#define LLDP_RECEIVER

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables LLDP to manage Profinet IO TLV.
*
* If this constant is defined, it enables LLDP to manage Profinet IO TLV.
*
* @note These TLV are mandatory for Profinet IO.
*
* @hideinitializer
*****************************************************************************/
#define LLDP_SUPPORT_PN

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables LLDP to manage 8023 TLV.
*
* If this constant is defined, it enables LLDP to manage 8023 TLV.
*
* @note These TLV are mandatory for Profinet IO.
*
* @hideinitializer
*****************************************************************************/
#define LLDP_SUPPORT_8023

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables LLDP to manage description TLV.
*
* If this constant is defined, it enables LLDP to manage description TLV such as system name or port description.
* These TLV are not mandatory for Profinet IO.
*
* @hideinitializer
*****************************************************************************/
#define LLDP_STD_DESC_SUPPORT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of ports available on the Ethernet Interface.
*
* This constant defines the number of ports available on the Ethernet Interface.
*
* @hideinitializer
*****************************************************************************/
#define WSE_LLDP_MAX_PORT             2

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximal number of remote system managed per port.
*
* This constant defines the maximal number of remote system managed per port. It is related to LLDP receiver.
*
* @hideinitializer
*****************************************************************************/
#define WSE_LLDP_REMOTE_SYSTEM_PORT   4
/* WSE_LLDP_MAX_PORT* WSE_LLDP_REMOTE_SYSTEM_PORT shall be smaller than 255 !!! */

/* Default CHASSIS ID */
#define WSE_LLDP_CHASSIS_DEFAULT {0xcc, 0xaa, 0xcc, 0xaa, 0xcc, 0xaa }
#define WSE_LLDP_CHASSIS_DEFAULT_LEN 6

/* Nb maximal of TLV per LLDPDU */
#define MAX_TLV_PER_LLDPDU            25
#define MAX_UNKNOWN_TLV_PER_LLDPDU    MAX_TLV_PER_LLDPDU/5

#ifndef LLDP_SUPPORT_MRP
#ifdef PN_RTC3_SUPPORTED
#define MAX_PN_TLV_PER_LLDPDU         4
#else
#define MAX_PN_TLV_PER_LLDPDU         3
#endif
#endif

#ifdef LLDP_SUPPORT_MRP
#define MAX_PN_TLV_PER_LLDPDU         4
#endif

#define MAX_8023_TLV_PER_LLDPDU       1

/*-----------------------------------------------------------------------*/
/* Block version use to response to the record request                   */
/*-----------------------------------------------------------------------*/
#define RECORD_RSP_BLOCK_VERSION_HIGTH 1 /* Alowed value : 1*/
#define RECORD_RSP_BLOCK_VERSION_LOW   0 /* Alowed value : 0 or 1 */

/*------------------------------------------------------------------------------------------------*/
/* This value is the max time awaiting before sending a DCP response                              */
/* PN-Al-Service:                                                                                 */
/* Client Hold Time                                                                               */
/* This attribute shall contain the maximum value in seconds, which a server shall only allow     */
/* Get or Set services from the last active client. Service requests from other server shall be   */
/* ignored before.                                                                                */
/* Attribute Type: Unsigned16                                                                     */
/* Default Value: 3                                                                               */
/*------------------------------------------------------------------------------------------------*/
#define DCP_CLIENT_HOLD_TIME 3000 /* 3000 ms --> 3 seconde */

/*-----------------------------------------------------------------------*/
/* define RT_Class supported by device                                   */
/* Stack device Profinet IO WoodHead only support RT_CLASS_1 in this     */
/* version                                                               */
/*-----------------------------------------------------------------------*/
#define RT_CLASS_1    0x1
#define RT_CLASS_2    0x2 /*even if IOD is RT Class1 the RTCLASS 2 shall be used when GSML define pnio_verion to 2.3 */
#define RT_CLASS_3    0x3 /* Not supported*/
#define RT_CLASS_UDP  0x4 /* Not supported*/

#define RT_CLASS_USE_BY_DEVICE  RT_CLASS_3

/*-----------------------------------------------------------------------*/
/* ID of each Queue (Indirection between queue and Task/Thread)          */
/*-----------------------------------------------------------------------*/
/*We need this if in the future we would like to modify number of thread*/
#define ID_QUEUE_ERROR        0
#define TASK_NAME_ERROR       "ERROR"
#define ID_QUEUE_FALDEV       1
#define TASK_NAME_FALDEV      "FAL"
#define ID_QUEUE_CMDEV        2
#define TASK_NAME_CMDEV       "CMDEV"
#define ID_QUEUE_USER         3     /*virtual queue*/
#define TASK_NAME_USERR       "USER"
#define ID_QUEUE_RPC          4
#define TASK_NAME_RPC         "RPC"
#define ID_QUEUE_CPM          5
#define TASK_NAME_CPM         "CPM"
#define ID_QUEUE_PPM          6
#define TASK_NAME_PPM         "PPM"
#define ID_QUEUE_DCP          7
#define TASK_NAME_DCP         "DCP"
#define ID_QUEUE_ALM          8
#define TASK_NAME_ALM         "ALM"
#define ID_QUEUE_MS           9
#define TASK_NAME_MS          "MS"
#define ID_QUEUE_LMPM         10
#define TASK_NAME_LMPM        "LMPM"
#define ID_QUEUE_FSPMDEV      11
#define TASK_NAME_FSPMDEV     "FSPMDEV"
#define ID_QUEUE_DEBUG        12
#define TASK_NAME_DEBUG       "MONITORING"
#define ID_QUEUE_SCHEDULE     13
#define TASK_NAME_SCHEDULE    "SCHEDULE"
#define ID_QUEUE_ASESM        14
#define TASK_NAME_ASESM       "STTUP"
#define ID_QUEUE_ADDIN        15
#define TASK_NAME_ADDIN       "ADDIN"
#define ID_QUEUE_MRP          16
#define TASK_NAME_MRP         "MRP"
#define ID_QUEUE_LLDP         17
#define TASK_NAME_LLDP        "LLDP"
#define ID_QUEUE_WEB          18
#define TASK_NAME_WEB         "WEB"
#define ID_QUEUE_CMRPC        19
#define TASK_NAME_CMRPC       "CMRPC"
#define ID_QUEUE_CMPBE        20
#define TASK_NAME_CMPBE       "CMPBE"
#define ID_QUEUE_CMRDR        21
#define TASK_NAME_CMRDR       "CMRDR"
#define ID_QUEUE_CMWRR        22
#define TASK_NAME_CMWRR       "CMWRR"
#define ID_QUEUE_CMDEV_DA     23
#define TASK_NAME_CMDEV_DA    "CMDEV_DA"
#define ID_QUEUE_CMSU         24
#define TASK_NAME_CMSU        "CMSU"
#define ID_QUEUE_CMINA        25
#define TASK_NAME_CMINA       "CMINA"
#define ID_QUEUE_CMIO         26
#define TASK_NAME_CMIO        "CMIO"
#define ID_QUEUE_CMDMC        27
#define TASK_NAME_CMDMC       "CMDMC"
#define ID_QUEUE_CMSM         28
#define TASK_NAME_CMSM        "CMSM"
#define ID_QUEUE_PSM          29
#define TASK_NAME_PSM         "PSM"
#define ID_QUEUE_MAX          ID_QUEUE_PSM


/*#define _DEBUG_MAX_ALLOC_ 3000*/

/*-----------------------------------------------------------------------*/
/* acyclic                                                               */
/*-----------------------------------------------------------------------*/

/*Max acyclic request manage simultaneously / application should regulate this*/
#define MAX_SIMULTANEOUS_ACYC_REQ     2

/*Max data size of a rpc frag            */
#define RPC_DATA_FRAG_SIZE           (ETHERNET_MAX_FRAME_LENGTH - 14 - 8 - 20 - 8 - 80 - 4 )  /* Ethernet Frame size - Ethernet header - LLC frame for SNAP - ip header size - udp header size  - rpc header size - CRC size */

/*Max fragment per acyc request          */
#define MAX_ACYC_FRAG                4

/*Max rpc that could be call simultaneously (=> 1 socket by simultaneous rpc) */
#define MAX_RPCCLIENT (MAX_SIMULTANEOUS_ACYC_REQ * MAX_ARD_PER_DEVICE + 1)

/*Max rpc that could be received simultaneously (= 1 socket) */
#define MAX_RPCSERVER (MAX_SIMULTANEOUS_ACYC_REQ * MAX_ARD_PER_DEVICE + 1)

/*-----------------------------------------------------------------------*/
/* DCP                                                                   */
/*-----------------------------------------------------------------------*/
#define MAX_SIMULTANEOUS_DCP_REQ  2 /* A unicast one and a multicast one */

/*-----------------------------------------------------------------------*/
/* Sanity check                                                          */
/*-----------------------------------------------------------------------*/
#if defined IOCONTROLLER && defined IODEVICE
  #pragma error ("Platform.h-->Profinet IO role definition Error")
#endif

#if defined PLATFORM_BIG_ENDIAN && defined PLATFORM_LITTLE_ENDIAN
  #pragma error ("Platform.h-->Need only one platform define")
#endif

#ifndef PLATFORM_BIG_ENDIAN
  #ifndef PLATFORM_LITTLE_ENDIAN
  #pragma error ("Platform.h-->Need at least one platform define")
  #endif
#endif

#ifndef IOCONTROLLER
  #ifndef IODEVICE
    #pragma error ("Platform.h-->Profinet IO role shall be defined")
  #endif
#endif
#ifndef IODEVICE
    #pragma error ("Platform.h-->Profinet IO role shall be defined as IODEVICE")
#endif
#ifdef IODEVICE
  #if MAX_DEVICE == 0
    #pragma error ("Platform.h-->MAx device shall be greater than 0")
  #endif

  #if MAX_INSTANCE_PER_DEVICE == 0
    #pragma error ("Platform.h-->Device support at least one Instance")
  #endif
  #if MAX_INSTANCE_PER_DEVICE > 1
    #pragma error ("Platform.h-->Multi Instance not yet implemented")
  #endif

  #if MAX_ACYC_FRAG == 0
    #pragma error ("Platform.h-->Max Acyclic Fragment shall be greater than 0")
  #endif

  #if MAX_SIMULTANEOUS_ACYC_REQ == 0
    #pragma error ("Platform.h-->Max Simultaneous Acyclic request shall be greater than 0")
  #endif

  #if RT_CLASS_USE_BY_DEVICE != RT_CLASS_3
    #pragma error ("Platform.h-->RT_CLASS is not supported")
  #endif

  #if (API_RANGE_MIN > API_RANGE_MAX)
    #pragma error ("Platform.h-->MAX_SLOT_SUPPORTED shall be greater than 0")
  #endif

  #if (SLOT_RANGE_MIN > SLOT_RANGE_MAX)
    #pragma error ("Platform.h-->MAX_SUBSLOT_SUPPORTED shall be greater than 0")
  #endif

  #if (SUBSLOT_RANGE_MIN > SUBSLOT_RANGE_MAX)
    #pragma error ("Platform.h-->MAX_API_SUPPORTED shall be greater than 0")
  #endif

  #if MAX_IOC_CONNEXION_PER_INSTANCE == 0
    #pragma error ("Platform.h-->MAX_IOC_CONNEXION_PER_INSTANCE shall be greater than 0")
  #endif

  #if WSE_LLDP_MAX_PORT > 255
    #pragma error ("Platform.h-->WSE_LLDP_MAX_PORT shall be smaller than 255")
  #endif
#endif

/*-----------------------------------------------------------------------*/
/* Debug instrumentation for wse use only                                */
/*-----------------------------------------------------------------------*/
/* #define  _DEBUG_OS_ALLOC             */
/* #define  TEST_TIMER_PRECISION_WSE    */
/* #define  WEB_STAT                    */

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enables internal log.
*
* This constant enables internal log. This logger is managed by stack and it is filled with:
*
* Kind of event  | Error Code           | Error Decode          | Error Code1          | Error Code2
* -------------- | -------------------- | --------------------- | -------------------- | --------------------
* RPC Response   | Same as RPC response | Same as RPC response  | Same as RPC response | Same as RPC response
* Assert         | PNIO : 0x81          | PNIO : 0x81           | User specific: 255   | User specific: 1
* Layer 2 Event  | PNIO : 0x81          | PNIO : 0x81           | User specific: 255   | User specific: 2
* Stack callback | PNIO : 0x81          | PNIO : 0x81           | User specific: 255   | User specific: 3
*
* If this define is used, an additional record will be automatically support by stack.
*
* Index  | Description
* ------ | -------------
* 0xF830 | Device Log Data
*
* @warning
*   - The additional record is only managed when @ref RECORD_INTERNAL is defined.
*   - Layer 2 events received by stack are only stored if @ref ACTIVATE_LOG_L2_EVENT is defined.
*   - Stack callbacks are only stored if @ref ACTIVATE_LOG_CBF_EVENT is defined.
*
* @hideinitializer
*****************************************************************************/
#define ACTIVATE_LOG_EVENT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enable log of Layer 2.
*
* This constant enables log of Layer 2 (DPC, ALM ...) received frames. This constant works only if
* @ref ACTIVATE_LOG_EVENT is defined, in other case this define will be ignored.
*
* When this constant is defined, internal log is filled with additional information. This information can be read using
* log book data record. See @ref ACTIVATE_LOG_EVENT description
*
* @warning
*   - The additional record is only managed when @ref RECORD_INTERNAL is defined.
*   - This define is ignored if @ref ACTIVATE_LOG_EVENT is not defined.
*   - The IO-Data reception are not added in the Log Book Data.
*
* @hideinitializer
*****************************************************************************/
  #define ACTIVATE_LOG_L2_EVENT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Enable log of stack callbacks.
*
* This constant enable log of stack callbacks (@ref PND_CB_DcpDeviceBlinkInd, ...). This constant works
* only if @ref ACTIVATE_LOG_EVENT is defined, in other case this define will be ignored.
*
* When this constant is defined, internal log is filled with additional information. This information can be read using
* log book data record. See @ref ACTIVATE_LOG_EVENT description.
*
* @warning
*   - The additional record is only managed when @ref RECORD_INTERNAL is defined.
*   - This define is ignored if @ref ACTIVATE_LOG_EVENT is not defined.
*   - The calback @ref PND_CB_NewInputInd and @ref PND_CB_NewInputAPDUDataStatus are not logged.
*
* @hideinitializer
*****************************************************************************/
  #define ACTIVATE_LOG_CBF_EVENT

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the maximum number of log event.
*
* This constant defines the maximum number of log event that the IO-Device can manage.
*
* See @ref ACTIVATE_LOG_EVENT description for additional information about log.
*
* @warning
*   - This define is ignored if @ref ACTIVATE_LOG_EVENT is not defined.
*   - This define is mandatory if @ref ACTIVATE_LOG_EVENT is defined.
*
* @hideinitializer
*****************************************************************************/
#define MAX_EVENT_LOGGER 40

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the size of a diagnostic buffer.
*
* This constant defines the size of a diagnostic buffer.
* The stack only supprt "Alarm and diagnostic" format define in specification
* thus this define shall be set to 16 bytes.
*
* @hideinitializer
*****************************************************************************/

#define PN_DIAGNOSTIC_DATA_SIZE 16

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the number of a diagnostic buffer supported by stack.
*
* This constant defines the number of a diagnostic buffer supported by stack.
* This define depends on product diagnosis capabilities.
*
* @hideinitializer
*****************************************************************************/

#define PN_NB_DIAGNOSTIC_DATA  20

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines1 Type of Production/Consumption.
*
* This constant enable the RTx communication handled by a hardware acceleration
* or process like hardware component.(see hwacc.c for coding)
*
* @hideinitializer
*****************************************************************************/
#define PN_PROD_HWACC

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines Type of Production/Consumption.
*
* This constant enable the RTx communication in the software mode,
* frame production/consumption is managed by the scheduler. Only RTC1 is supported in this mode
*
* @hideinitializer
*****************************************************************************/
/*#define PN_PROD_CPU*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines Type of Production/Consumption.
*
*  This constant enables the selection of the mode for consumption/production (the RTx communication)
*  either in the CPU mode or the hardware acceleration mechanism mode.
*  A new callback will be fired in the connection phase: PND_CB_ArGetProductionMode
*
* @hideinitializer
*****************************************************************************/
/*#define PN_PROD_HYBRID*/

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the local Reception delay.
*
* This constant defines the delay necessary for the local port to receive a frame.
*
*
* @hideinitializer
*****************************************************************************/
#define PORT_RX_DELAY_LOCAL 184

/****************************************************************************/
/**
* @ingroup PNIO_DEFINE
* @brief Defines the local Transmission delay.
*
* This constant defines the delay necessary for the local port to send a frame.
*
*
* @hideinitializer
*****************************************************************************/
#define PORT_TX_DELAY_LOCAL 86

/*RT mode condition*/
#if !defined(PN_PROD_CPU) && !defined(PN_PROD_HWACC) && !defined(PN_PROD_HYBRID)
  #error IOD Compilation error : at least one of these defines must be declared (see platform.h)
#endif

#if (defined(PN_PROD_CPU) && defined(PN_PROD_HWACC)) ||  (defined(PN_PROD_CPU) && defined(PN_PROD_HYBRID)) || (defined(PN_PROD_HWACC) && defined(PN_PROD_HYBRID))
    #error IOD Compilation error : Only one of PN_PROD_XXXX constant must be declared(PN_PROD_CPU or PN_PROD_HWACC or PN_PROD_HYBRID).
#endif

#if defined(PN_PROD_CPU) && defined(PN_RTC3_SUPPORTED)
    #error IOD Compilation error : Only PN_RTC3_SUPPORTED shall be declared in PN_PROD_HWACC or PN_PROD_HYBRID).
#endif

#if (defined(PN_PROD_HWACC) && !defined(SHARED_MEMORY_MODE)) ||  (defined(PN_PROD_HYBRID) && !defined(SHARED_MEMORY_MODE))
    #error IOD Compilation error : Shared memory mode management shall be declared with PN_PROD_HWACC or PN_PROD_HYBRID).
#endif
#endif
