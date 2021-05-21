/*
+-----------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                                   |
| -------------------------------                                                   |
|                                                                                   |
|  Copyright 2006-2007 (c)   Woodhead Software & Electronics. All rights reserved.  |
+-----------------------------------------------------------------------------------+
  Description .......: Ethernet/IP type definitions for supported operating 
					   systems
                       
+-----------------------------------------------------------------------------------+
  $Workfile: eip_types.h $
  $Revision: 60 $
  $Date: 19/11/15 16:27 $
+-----------------------------------------------------------------------------------+
  Version | Date     | Author | Comments
			mm/dd/yy
  1.0.0     08/03/06   J.B.     Ethernet/IP stack types
  1.0.1	    02/28/07   J.B.	    Added OS specific definitions
+-----------------------------------------------------------------------------------+
*/

#ifndef __EIP_TYPES_H
#define __EIP_TYPES_H

#include "compiler.h"

/*-- Boolean definition ------------------------------------------------------*/

#define EIP_FALSE 0
#define EIP_TRUE  1

/*============================================================================*/
/*                  SPECIFIC DEFINITIONS FOR EACH OS                          */
/*============================================================================*/
#define EIP_NEAR
#define EIP_FAR

/*-- VOID definition ---------------------------------------------------------*/
#define VOID void
typedef void EIP_VOID, EIP_NEAR * EIP_PVOID, EIP_FAR * EIP_LPVOID;

/*-- EIP_NULL definition ---------------------------------------------------------*/
#define EIP_NULL   (void *)0

/*============================================================================*/
/* standard data type definition                                              */
/*============================================================================*/
typedef struct in_addr EIP_ST_INADDR, EIP_NEAR *EIP_PST_INADDR, EIP_FAR *EIP_LPST_INADDR;
/* ### ATTENTION ### these might be different under various OS */
typedef int             EIP_BOOL,     EIP_NEAR * EIP_PBOOL,     EIP_FAR * EIP_LPBOOL;   /* 0-EIP_FALSE, 1-EIP_TRUE	*/
typedef int             EIP_INT,      EIP_NEAR * EIP_PINT,      EIP_FAR * EIP_LPINT;    /* 0-EIP_FALSE, 1-EIP_TRUE	*/

typedef char            EIP_CHAR,     EIP_NEAR * EIP_PCHAR,     EIP_FAR * EIP_LPCHAR;	  /* signed 8-bit		  */
typedef unsigned char   EIP_BYTE,     EIP_NEAR * EIP_PBYTE,     EIP_FAR * EIP_LPBYTE;	  /* unsigned 8-bit	  */

typedef short int       EIP_SHORT,    EIP_NEAR * EIP_PSHORT,    EIP_FAR * EIP_LPSHORT;  /* signed 16-bit	  */
typedef unsigned short  EIP_WORD,     EIP_NEAR * EIP_PWORD,     EIP_FAR * EIP_LPWORD;	  /* unsigned 16-bit  */

typedef long            EIP_LONG,     EIP_NEAR * EIP_PLONG,     EIP_FAR * EIP_LPLONG;	  /* signed 32-bit	  */
typedef unsigned long   EIP_DWORD,    EIP_NEAR * EIP_PDWORD,    EIP_FAR * EIP_LPDWORD;	/* unsigned 32-bit  */

typedef unsigned short EIP_USHORT,EIP_NEAR * EIP_PUSHORT , EIP_FAR * EIP_LPUSHORT;      /* entier 16 bits non-signes */
typedef unsigned long  EIP_ULONG, EIP_NEAR * EIP_PULONG , EIP_FAR * EIP_LPULONG;        /* entier 32 bits non-signes */
typedef unsigned char  EIP_UCHAR, EIP_NEAR * EIP_PUCHAR , EIP_FAR * EIP_LPUCHAR;        /* char 4 bits non-signes    */

#define NTW_INVALID_HANDLE ((EIP_HANDLE)(SOCKET)(~0))

typedef void * EIP_HANDLE;

/*-- CIP data type definitions ---------------------------------------------------------*/
#define SINT			EIP_CHAR		/* signed	 8-bit		-128	-> 127		*/
#define INT				EIP_SHORT		/* signed	16-bit		-32768	-> 32767	*/
#define DINT			EIP_LONG		/* signed	32-bit		-2^31	-> 2^31-1	*/
#define LINT			EIP_LONGLONG	/* signed	64-bit		-2^63	-> 2^63-1	*/

#define USINT			EIP_BYTE        /* unsigned  8-bit		0		-> 255      */
#define UINT			EIP_WORD		/* unsigned 16-bit		0		-> 65535	*/
#define UDINT			EIP_DWORD       /* unsigned 32-bit		0		-> 2^32-1   */

#define BYTE			EIP_BYTE		/*  8-bit value	*/
#define WORD			EIP_WORD		/* 16-bit value	*/
#define DWORD			EIP_DWORD		/* 32-bit value	*/

#define EPATH			EIP_LPBYTE      /* Ptr on CIP path segments              */
#define SHORT_STRING	EIP_CHAR        // Character string (1byte/char, 1 byte length indicator)
#define STRING			EIP_CHAR        // Character string (1byte/char)

// Object TCP (0xF5) attribut
#define TCP_ATTR_ALL                      0
#define TCP_ATTR_STATUS                   1
#define TCP_ATTR_CONFIGURATION_CAPABILITY 2
#define TCP_ATTR_CONFIGURATION_CONTROL    3
#define TCP_ATTR_PHYSICAL_LINK_OBJECT     4
#define TCP_ATTR_INTERFACE_CONFIGURATION  5
#define TCP_ATTR_HOST_NAME                6
#define TCP_ATTR_SAFETY_NETWORK_NUMBER    7
#define TCP_ATTR_TTL_VALUE                8
#define TCP_ATTR_MCAST_CONFIG             9
#define TCP_ATTR_SELECT_ACD              10
#define TCP_ATTR_LAST_CONFLICT_DETECTED  11
#define TCP_ATTR_EIP_QUICK_CONNECT       12
#define TCP_ATTR_TCP_INACTIVITY_TIMEOUT  13

// Object Ethernet Link (0xF6) attribut
#define ETH_ATTR_ALL                      0
#define ETH_ATTR_INTERFACE_SPEED          1
#define ETH_ATTR_INTERFACE_FLAGS          2
#define ETH_ATTR_PHYSICAL_ADDRESS         3
#define ETH_ATTR_INTERFACE_COUNTERS       4
#define ETH_ATTR_MEDIA_COUNTERS           5
#define ETH_ATTR_INTERFACE_CONTROL        6
#define ETH_ATTR_INTERFACE_TYPE           7
#define ETH_ATTR_INTERFACE_STATE          8
#define ETH_ATTR_ADMIN_STATE              9
#define ETH_ATTR_INTERFACE_LABEL          10
#define ETH_ATTR_INTERFACE_CAPABILITY     11

// Object Identity (0x01) attribut
#define ID_ATTR_ALL                       0
#define ID_ATTR_VENDOR_ID                 1
#define ID_ATTR_DEVICE_TYPE               2
#define ID_ATTR_PRODUCT_CODE              3
#define ID_ATTR_REVISION                  4
#define ID_ATTR_STATUS                    5
#define ID_ATTR_SERIAL_NUMBER             6
#define ID_ATTR_PRODUCT_NAME              7

#define ETH_ATTR_INTERFACE_LABEL_SIZE    64
#define TCPIP_ATTR_DOMAINE_NAME_SIZE     48
#define TCPIP_ATTR_HOST_NAME_SIZE        64

/*---------------------------------------------------------------------------*/
/* PUBLIC STRUCTURES                                                         */
/*---------------------------------------------------------------------------*/

#define CIP_STATUS_MAX_COUNT 10

/*-- CIP Status structure ---------------------------------------------------------*/
typedef struct
{
  /*! Status */
  EIP_BYTE byStatus;
  EIP_BYTE reserved;
  /*! Number of extended word contained in wExtendedTab */
  EIP_BYTE byNbExtended;
  /*! Extendes words */
  EIP_WORD wExtendedTab[CIP_STATUS_MAX_COUNT];
} ST_CIP_STATUS, * PST_ST_CIP_STATUS,EIP_FAR * LPST_CIP_STATUS;

/*-- Class Tcp/Ip structure -------------------------------------------------------*/

/* Status attribute
 * Interface configuration status :
 * 0 = The interface configuration attribute has not been configured
 * 1 = The interface configuration attribute contains valid configuration obtained
 * from bootp, dhcp or non volatile storage.
 * 2 = The IP address member of the interface configuration attribute contains
 * valid configuration, obtained from hard settings.
 * 
 * Bit 4 : Multicast pending
 *      1 = Indicates a pending configuration change in the TTL and/or Mcast config
 *          attributes. 
 */
#define EIP_TCPIP_ATTRB_STATUS_NOT_CONFIGURED  0
#define EIP_TCPIP_ATTRB_STATUS_VALID_CONF      1
#define EIP_TCPIP_ATTRB_STATUS_VALID_HARD_CONF 2

#define EIP_TCPIP_ATTRB_STATUS_MCAST_PENDING    (1 << 4)
#define EIP_TCPIP_ATTRB_STATUS_IF_CONF_PENDING  (1 << 5)
#define EIP_TCPIP_ATTRB_STATUS_ACD_STATUS       (1 << 6)
#define EIP_TCPIP_ATTRB_STATUS_ACD_FAULT        (1 << 7)

/*
 * Configuration capability :
 * Bit 0 : BOOTP Client
 * Bit 1 : DNS Client
 * Bit 2 : DHCP Client
 * Bit 3 : DHCP-DNS Update
 * Bit 4 : Configuration Settable
 * Bit 5 : Hardware Configurable
 */
#define EIP_TCPIP_CNF_CAPA_BOOTP_CLIENT    (1 << 0)
#define EIP_TCPIP_CNF_CAPA_DNS_CLIENT      (1 << 1)
#define EIP_TCPIP_CNF_CAPA_DHCP_CLIENT     (1 << 2)
#define EIP_TCPIP_CNF_CAPA_DHCP_DNS_UPDATE (1 << 3)
#define EIP_TCPIP_CNF_CAPA_CONF_SETTABLE   (1 << 4)
#define EIP_TCPIP_CNF_CAPA_HARD_CONF       (1 << 5)
#define EIP_TCPIP_CNF_CAPA_REQUIRE_RESET   (1 << 6)
#define EIP_TCPIP_CNF_CAPA_ACD_CAPABLE     (1 << 7)

/*
 * Configuration control :
 * Startup configuration :
 * 0 : The device shall use the interface configuration values previously stored.
 * 1 : The device shall obtain its interface configuration values via bootp.
 * 2 : The device shall obtain its interface configuration values via dhcp.
 * 
 * Bit 4 : DNS enable
 */
#define EIP_TCP_IPCNF_CTRL_STORED_VALUE 0
#define EIP_TCP_IPCNF_CTRL_BOOTP        1
#define EIP_TCP_IPCNF_CTRL_DHCP         2
#define EIP_TCP_IPCNF_CTRL_DNS_ENABLE  (1 << 4)

#define EIP_TCP_IPCNF_RESERVED_BITS  0xFFE0

typedef struct _CLASS_TCP_IP_PHYSICAL_LINK_OBJECT
{
  UINT      Path_Size;
  USINT     Path[12];
}CLASS_TCP_IP_PHYSICAL_LINK_OBJECT;

typedef struct _CLASS_TCP_IP_INTERFACE_CONFIGURATION
{
  UDINT     IP_Address;
  UDINT     Network_Mask;
  UDINT     Gateway_Address;
  UDINT     Name_Server;
  UDINT     Name_Server_2;
  UINT      Domain_Name_Length;
  STRING    Domain_Name[TCPIP_ATTR_DOMAINE_NAME_SIZE];
}CLASS_TCP_IP_INTERFACE_CONFIGURATION;

typedef struct _CLASS_TCP_IP_HOST_NAME
{
  UINT      Length;
  STRING    Name[TCPIP_ATTR_HOST_NAME_SIZE];
}CLASS_TCP_IP_HOST_NAME;

typedef struct _CLASS_TCP_MCAST_CONFIG
{
  USINT AllocControl;
  USINT Reserved;
  UINT  NumMast;
  UDINT McastStartAddr;
}CLASS_TCP_MCAST_CONFIG;

typedef struct _CLASS_TCP_LAST_CONFLICT_DETECTED
{
  USINT AcdActivity;
  USINT RemoteMAC[6];
  USINT ArpPdu[28];
}CLASS_TCP_LAST_CONFLICT_DETECTED;

typedef struct
{
  DWORD       Status;
  DWORD       Configuration_Capability;
  DWORD       Configuration_Control;
  
  CLASS_TCP_IP_PHYSICAL_LINK_OBJECT Physical_Link_Object;
  
  CLASS_TCP_IP_INTERFACE_CONFIGURATION Interface_Configuration;
  
  CLASS_TCP_IP_HOST_NAME HostName;

  USINT       SafetyNetworkNumber[6];
  USINT       TTL_Value;
  CLASS_TCP_MCAST_CONFIG  McastConfig;
  BYTE        SelectAcd;
  CLASS_TCP_LAST_CONFLICT_DETECTED LastConflictDetected;
  BYTE        EipQuickConnect;
  UINT        TcpInactivityTimeOut;
}CLASS_TCP_IP, EIP_NEAR * PCLASS_TCP_IP, EIP_FAR * LPCLASS_TCP_IP;

// ============================================================================
//  ---------------------------[ Object Definition ]---------------------------
// ============================================================================

// ----------------------------------------------------------------------------
// + CLASS ETHERNET LINK
// ----------------------------------------------------------------------------

/* Control bits
 *  bit 0: 0 = Auto negotiation disabled
 *         1 = Auto negotiation enabled
 *  bit 1: 0 = force half duplex (if bit 0 = 0)
 *         1 = force full duplex (if bit 0 = 0)
 */
#define EIP_ETH_CTRL_BITS_AUTO_NEG_ON 0x0001
#define EIP_ETH_CTRL_BITS_HALF_DUPLEX 0x0000
#define EIP_ETH_CTRL_BITS_FULL_DUPLEX 0x0002

/* Interface flags
 * bit 0 : 0 = Link down
 *         1 = Link up
 * bit 1 : 0 = Half duplex
 *         1 = Full duplex
 * bits 2-4 : Negotiation status
 *         0 = Auto-negotiation in progress
 *         1 = Auto-negotiation and speed detection failed
 *         2 = Auto-negotiation failed but detected speed
 *         3 = Successfully negociated speed and duplex
 *         4 = Auto-negotiation not attempted
 * bit 5 : 0 = Indicates the interface can activate changes to link parameters
 *         1 = Indicates the device requires a Reset
 * bit 6 : 0 = Indicates no hardware fault
 *         1 = Indicates a local hardware fault
 */
#define EIP_ETH_IF_FLAG_LINK_UP                 (1 << 0)
#define EIP_ETH_IF_FLAG_DUPLEX                  (1 << 1)
#define EIP_ETH_IF_FLAG_AUTO_NEG_IN_PROGRESS    (0 << 2)
#define EIP_ETH_IF_FLAG_AUTO_NEG_SPEED_FAILED   (1 << 2)
#define EIP_ETH_IF_FLAG_AUTO_NEG_DUPLEX_FAILED  (2 << 2)
#define EIP_ETH_IF_FLAG_AUTO_NEG_SUCCESS        (3 << 2)
#define EIP_ETH_IF_FLAG_AUTO_NEG_NOT_ATTEMPTED  (4 << 2)
#define EIP_ETH_IF_FLAG_MANUAL_SETTING          (1 << 5)
#define EIP_ETH_IF_FLAG_LOCAL_HARD_FAULT        (1 << 6)

/*
 * Interface type :
 * 0 = Unknown interface type
 * 1 = Internal interface
 * 2 = Twisted-pair
 * 3 = Optical fiber 
 */
#define EIP_ETH_IF_TYPE_UNKNOWN 0
#define EIP_ETH_IF_TYPE_INTERNAL 1
#define EIP_ETH_IF_TYPE_TWISTED_PAIR 2
#define EIP_ETH_IF_TYPE_OPTICAL_FIBER 3

/*
 * Interface state :
 * 0 = Unknown interface state
 * 1 = Interface enabled
 * 2 = Interface disabled
 * 3 = Interface is testing
 */
#define EIP_ETH_IF_STATE_UNKNOWN  0
#define EIP_ETH_IF_STATE_ENABLED  1
#define EIP_ETH_IF_STATE_DISABLED 2
#define EIP_ETH_IF_STATE_TESTING  3

/*
 * Admin state :
 * 0 = Unknown interface state
 * 1 = Interface enabled
 * 2 = Interface disabled
 * 3 = Interface is testing
 */
#define EIP_ETH_ADMIN_STATE_RESERVED 0
#define EIP_ETH_ADMIN_STATE_ENABLE   1
#define EIP_ETH_ADMIN_STATE_DISABLE  2

typedef struct _CLASS_ETH_LINK_INTERFACE_COUNTERS
{
  UDINT     In_Octets;
  UDINT     In_Ucast_Packets;
  UDINT     In_NUcast_Packets;
  UDINT     In_Discards;
  UDINT     In_Errors;
  UDINT     In_Unknown_Protos;
  UDINT     Out_Octets;
  UDINT     Out_Ucast_Packets;
  UDINT     Out_NUcast_Packets;
  UDINT     Out_Discards;
  UDINT     Out_Errors;
}CLASS_ETH_LINK_INTERFACE_COUNTERS;

typedef struct _CLASS_ETH_MEDIA_COUNTERS
{
  UDINT     Alignment_Errors;
  UDINT     FCS_Errors;
  UDINT     Single_Collisions;
  UDINT     Multiple_Collisions;
  UDINT     SQE_Test_Errors;
  UDINT     Deferred_Transmissions;
  UDINT     Late_Collisions;
  UDINT     Excessive_Collisions;
  UDINT     MAC_Transmit_Errors;
  UDINT     Carrier_Sense_Errors;
  UDINT     Frame_Too_Long;
  UDINT     MAC_Receive_Errors;
}CLASS_ETH_MEDIA_COUNTERS;

typedef struct _CLASS_ETH_INTERFACE_CONTROL
{
  EIP_WORD  Control_Bits;
  UINT      Forced_Interface_Speed;  
}CLASS_ETH_INTERFACE_CONTROL;

typedef struct _CLASS_ETH_INTERFACE_SPEED_DUPLEX
{
  UINT  Interface_Speed;
  USINT Interface_Duplex_Mode;
}CLASS_ETH_INTERFACE_SPEED_DUPLEX;

typedef struct _CLASS_ETH_INTERFACE_CAPABILIY
{
  EIP_DWORD  Capability_Bits;
  struct
  {
    USINT                             SpeedDuplexArrayCount;
    CLASS_ETH_INTERFACE_SPEED_DUPLEX  SpeedDuplex[ETH_NUM_SPEED_DUPLEX_PAIR_MAX];
  }SpeedDuplexOptions;
}CLASS_ETH_INTERFACE_CAPABILIY;

typedef struct
{
  UDINT       Interface_Speed;
  DWORD       Interface_Flags;
  USINT       Physical_Address[6];

  CLASS_ETH_LINK_INTERFACE_COUNTERS Interface_Counters;
  
  CLASS_ETH_MEDIA_COUNTERS Media_Counters;

  CLASS_ETH_INTERFACE_CONTROL Interface_Control;

  USINT         Interface_Type;
  USINT         Interface_State;
  USINT         Admin_State;
  USINT         Interface_Label_Length;
  SHORT_STRING  Interface_Label[ETH_ATTR_INTERFACE_LABEL_SIZE];
  CLASS_ETH_INTERFACE_CAPABILIY Interface_Capability;
}CLASS_ETHERNET_LINK, EIP_NEAR * PCLASS_ETHERNET_LINK, EIP_FAR * LPCLASS_ETHERNET_LINK;


//----------------------------------------------------------------------------
//+ CLASS QOS
//----------------------------------------------------------------------------
typedef struct
{
  USINT byDscpUrgent;
  USINT byDscpScheduled;
  USINT byDscpHigh;
  USINT byDscpLow;
  USINT byDscpExplicit;
}CLASS_QOS, EIP_NEAR * PCLASS_QOS, EIP_FAR * LPCLASS_QOS;

typedef struct
{
  UINT          Vendor_ID;
  UINT          Device_Type;
  UINT          Product_Code;

  struct
  {
    USINT       Major_Revision;
    USINT       Minor_Revision;
  }Revision;
  
  WORD          Status;
  UDINT         Serial_Number;
  USINT         Product_Name_Length;
  SHORT_STRING  Product_Name[32];
}CLASS_IDENTITY, EIP_NEAR * PCLASS_IDENTITY, EIP_FAR * LPCLASS_IDENTITY;

/* Macros */
#ifndef NOMINMAX
  #ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
  #endif

  #ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
  #endif
#endif  /* NOMINMAX */

#define EIP_ALIGN_SUP(Val, Boundary) \
	((EIP_DWORD)((((EIP_DWORD)(Val)-1) & (~((Boundary)-1))) + (Boundary)))

//*****************************************************************************

#include "pack_begin.h"

typedef struct _EPIC_SOCKADDR 
{
  EIP_SHORT   Sin_Familly FIELD_PACKED;
  EIP_WORD	  Sin_Port FIELD_PACKED;
  EIP_DWORD   Sin_Addr FIELD_PACKED;
  EIP_BYTE    Sin_Zero[8] FIELD_PACKED;
}STRUCT_PACKED ST_EPIC_SOCKADDR;

#include "pack_end.h"

//*****************************************************************************

#endif //__EIP_TYPES_H
