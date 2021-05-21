/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2006 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:                                                          
                       
+------------------------------------------------------------------------------+
  $Workfile: adt_config.h $
  $Revision: 86 $
  $Date: 1/12/17 8:56 $
+------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
            mm/dd/yy
  1.0.0     08/26/08   C.D.     New file creation
+------------------------------------------------------------------------------+
*/

#ifndef __ADT_CONFIG_H
#define __ADT_CONFIG_H

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/
/* This file must be included first */
#include "compiler.h"
#include "user_timer.h"

/* Define or undef STACK_MULTI_TASKS to use the stack in multithreaded
 * configuration or not.
 */
#define STACK_MULTI_TASKS
/*#undef STACK_MULTI_TASKS*/

/* Define or undef STACK_UDP_OPTIMIZATIONS to use UDP optimizations or
 * standard way (like socket layer).
 */
#undef STACK_UDP_OPTIMIZATIONS

/* Define or undef QOS_SUPPORT to use the QoS Object
 */
#define QOS_SUPPORT

/* Define or undef MM_LITTLE_ENDIAN to use little endian processor or
 * not.
 */
#define MM_LITTLE_ENDIAN

/* Define or undef MM_BIG_ENDIAN to use big endian processor or
 * not.
 */
#undef MM_BIG_ENDIAN

/*
 * Common part
 */
#define OUT_DATA_SIZE 5000

#define IN_DATA_SIZE 5000

/* Task Define for Monotask */
#define EIP_PROCESS_TASK_PRIORITY	      12
#define EIP_PROCESS_TASK_STACK_SIZE     0x2000
#define EIP_PROCESS_TASK_OPTIONS        0

/* Task Define for Multitask  (STACK_MULTI_TASKS) */
#define EIP_STACK_TASK_PRIORITY	        7
#define EIP_APP_TASK_PRIORITY	          4
#define EIP_SO_TASK_PRIORITY	          3

#define EIP_STACK_TASK_STACK_SIZE       20000
#define EIP_APP_TASK_STACK_SIZE         10000
#define EIP_SO_TASK_STACK_SIZE          20000

#define EIP_STACK_TASK_OPTIONS          0
#define EIP_APP_TASK_OPTIONS            0
#define EIP_SO_TASK_OPTIONS             0


/*
 * enable/disable debug messages
 */
/* To activate debugging messages of USER layer */
#undef _DEBUG
/* To activate debugging messages of the stack */
#undef _DEBUG_STACK
/* To activate debugging messages of the socket layer */
#undef _DEBUG_SOCKETS

/*
 * Size of an EM in bytes (this size is limited to this value to be sure that
 * only one frame is sent ie no tcp fragmentation).
 */
 #define EM_MAX_SIZE               1414

/* Max size for IO Data Assembly                                                                    */
/* This value (1400) doesn't fit a full ethernet MTU, but is compatible with the EM_MAX_SIZE (1414) */
/* in order to support get attribute single of assembly data                                        */
#define IO_MAX_SIZE               1400

/* Number of objects class */
#define OM_NB_OBJ_MAX               32

/* Maximum number of mail usable */
#define NB_MAIL_LIST               256



/* Class Ethernet Number of Instance */
#define ETH_NUM_OBJ                  1

/* Number max of object managed by User Object  */
#define NB_OBJECT 		16
/* Number max of service per object managed by User Object */
#define NB_SERVICE 		16
/* Number max of instance per object managed by User Object */
#define NB_INSTANCE 	256
/* Number max of attribute per object managed by User Object */
#define NB_ATTRIBUTE 	256

/*
 * Scanner part
 */
/* Number of Device for Explicit messages */
#define NB_DEVICE_FOR_EM            32

/* Number max of device supported by the scanner */
#define NB_DEVICE_FOR_CNF          128

/* Number of explicit message blocks that can be used simultaneously  */
#define EM_NB_BLOCK                 16

/*
 * Adapter part
 */

/* Max Number of instances supported by the stack (5 is for INPUT, OUTPUT, CONF, IO, LO)*/
#define ASM_NB_INSTANCE              5

/* Number max of local slave managed by the adapter */
#define SERVER_NB_DEVICE             1

/* Number of IO connection managed by the adapter */
#define CIP_TARGET_IO                3

/* Number of Explicit message connection (class3) managed by the adapter : 6 is the minimum requested by EDITT */
#define CIP_TARGET_EM                6

/* Internal diagnostics class definition */
/* Class Id for stack diagnostic object */
#define OBJ_CLASS_DIAG_STACK        0x300
/* Class Id for adapter diagnostic object */
#define OBJ_CLASS_DIAG_ADAPTER      0x302
/* Class Id for EM diagnostic object */
#define OBJ_CLASS_DIAG_EM           0x303
/* Class Id for EM List diagnostic object */
#define OBJ_CLASS_DIAG_EM_LIST      0x304

/*---------------------------------------------------------------------------*/
/* Internal definitions                                                      */
/*---------------------------------------------------------------------------*/

/* Number of List Identity response delayed */
#define NB_LIST_IDENTITY_RESPONSE   10

/* Number of Speed/Duplex pairs Max : 10 Half, 10 Full, 100 Half, 100 Full, etc) */
#define ETH_NUM_SPEED_DUPLEX_PAIR_MAX    10

/* Nb of devices managed by the Client */
#define CLIENT_NB_DEVICE              (NB_DEVICE_FOR_CNF + NB_DEVICE_FOR_EM)

/* Nb of Explict connection per device */
#define CM_NB_CNXN_EM                3

/* Total number of Coco Instance */
#define EIP_NB_CNXN_COCO_MAX          (CIP_ORIGINATOR_IO) 

/* Number of server connection */
#define CM_NB_SERVER_LINK             (CIP_TARGET_IO + CIP_TARGET_EM)  

/* Number of simultaneous connection supported by IO DATA */
#define EIP_NB_CNXN_BY_IO_DATA         IO_NB_PRODUCTION

/* Number of supported Target device */
#define UCMM_NB_BLOCK               32
#define NB_TARGET_DEVICE              (CIP_TARGET_EM + UCMM_NB_BLOCK)

/* total number of devices */
#define EIP_NB_DEVICE                 (SERVER_NB_DEVICE)    

/* Number of UDP sockets for IO (depending of the usage of QOS_SUPPORT) */
#ifdef QOS_SUPPORT
  #define NB_UDP_CNXN         5
#else
  #define NB_UDP_CNXN         1
#endif

#define DSCP_INDEX_URGENT     1 
#define DSCP_INDEX_SCHEDULED  2 
#define DSCP_INDEX_HIGH       3
#define DSCP_INDEX_LOW        4

/* Sockets used by mails for internal communication (client/server) */
#define SOCKETS_MAIL          2

/* Sockets used by some OSes to properly close sockets (client/server) */
#define SOCKETS_CLOSER        2

/* Sockets used by port 44818 (1 client (for network detection) + 1 server) */
#define SOCKETS_EIP           2

/* Number of Sockets on UDP */
#define EIP_NB_SOCKET_UDP (SOCKETS_EIP + NB_UDP_CNXN + SOCKETS_MAIL + SOCKETS_CLOSER)

/* Number of socket on TCP (CLIENT_NB_DEVICE + SERVER_NB_DEVICE) + NB_TARGET_DEVICE + 1 (server port 44818) */
#define EIP_NB_SOCKET_TCP (EIP_NB_DEVICE + NB_TARGET_DEVICE + 1)
#define EIP_NB_SOCKET_MAX             (EIP_NB_SOCKET_TCP + EIP_NB_SOCKET_UDP)

/* Total number of IO connexions */
#define EIP_NB_IO_CNXN_MAX            (CIP_TARGET_IO)

#define CM_NB_CLIENT_LINK             (CLIENT_NB_DEVICE * CM_NB_CNXN_EM)

/* Size of the stack message queue */
#define STACK_QUEUE_ITEM_COUNT 256

/* Nb of available application stream example : PCCC et PCCC_DH */
#define OA_NB_APPLICATION_FLUX 129

#define EIP_EM_FORWARD_OPEN_BOUND 511

/* Internal port definition used to communicate between tasks. This number is a
 * starting point and is increased at each mail creation of type MAIL_SOCKET */
#define EIP_MAIL_INTERNAL_PORT 0x5650

/* Memomy model checks */
#if defined (MM_LITTLE_ENDIAN) && defined (MM_BIG_ENDIAN)
  #error "Memory type shall be MM_LITTLE_ENDIAN (for a Pentium processor) or MM_BIG_ENDIAN (for PPC processor) and not both"
#elif defined MM_LITTLE_ENDIAN 
#elif defined MM_BIG_ENDIAN
#else
  #error "Memory type unknown ! Define MM_LITTLE_ENDIAN (for a Pentium processor) or MM_BIG_ENDIAN (for a PPC processor)"
#endif

/* Maximum mail size (in bytes, should be equal to an Ethernet Frame, could be reduce upto 512 */
#define MAIL_OVERHEAD             100
#define SYS_BUFFER_BLOCK_SIZE     (EM_MAX_SIZE + MAIL_OVERHEAD)

#include "os_inc.h"

/*---------------------------------------------------------------------------*/
#endif
