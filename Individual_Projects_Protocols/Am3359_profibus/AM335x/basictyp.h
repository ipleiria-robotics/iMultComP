/****************************************************************************
***
***     TMG i-tec Gesellschaft fuer industrielle Informationstechnik mbH
***
***     Telefon: +49-721-96410-0
***     Telefax: +49-721-96410-10
***
***
*****************************************************************************
***
***     Targetsystem : portable
***     Tools        : Borland-C++ (Ansi) / IAR-C (68HC11) / keil-C (8031)
***            Intel-C (V25,80188)
***
*****************************************************************************
***
***     Author       : Dirk Brauner
***     Filename     : basictyp.h
***
*****************************************************************************
***
***     Description : This module contains all basic types which are
***                   needed by the ali and the application.
***                   Of course, it is possible to add own datatypes
***                   to the application.
***
*****************************************************************************
***
***     Version        Date       Name         Comment
***
***     1.0            30.06.93   JAl/DBr/HBu  first release
***
****************************************************************************/
#ifndef _BASICTYPH_
#define _BASICTYPH_
/****************************************************************************
**
**     Interface -- Relations
*/
/*     End Interface -- Relations
**
****************************************************************************/

/****************************************************************************
**
**     Datatypes/Defines
*/

/* global defines */
#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

/* definition of constants */
#define CONST(datatype) const datatype
/* dummy define */
#define XDATA
/* dummy define */

/*----------  internal Datatypes -----------*/

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long longword;


/*----------  Global Constants  ------------*/
#ifndef TRUE
#define TRUE 0xFF
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif
#define HIGH 2
#define LOW 0
#define MULTIPLE 1
#define SINGLE 0


/* Uebergabeparameter an user_error */
#define USER_ERROR_NOMEM 1
#define USER_ERROR_SYSTEM 2
#define USER_ERROR_ALARM_MODE_SLAVE   3

#define NIL_SAP 64

#define BAUD_9K 0               /* 9.6 KBaud */
#define BAUD_19K 1              /* 19.2 KBaud */
#define BAUD_45K 2              /* 45.45 KBaud */
#define BAUD_93K 3              /* 93.75 KBaud */
#define BAUD_187K 4             /* 187.5 KBaud */
#define BAUD_500K 5             /* 500 KBaud */
#define BAUD_1_5M 6              /*  1,5 MBaud */
#define BAUD_3M 7               /*  3 MBaud */
#define BAUD_6M 8               /*  6 MBaud */
#define BAUD_12M 9              /* 12 MBaud  */

#define CONFIG_SUCCESS 10
#define BAD_PARAMETER 40
#define BAD_CONFIGURATION 41
#define UPDATE_CONFIGURATION 42


/* Bitmasks diagnosis status1 */
#define STATION_NOT_READY 0x02
#define NO_STATION_NOT_READY 0xFD
#define CFG_FAULT 0x04
#define NO_CFG_FAULT 0xFB
#define EXT_DIAG 0x08
#define NO_EXT_DIAG 0xF7
#define NOT_SUPPORTED 0x10
#define NO_NOT_SUPPORTED 0xEF
#define PRM_FAULT 0x40
#define NO_PRM_FAULT 0xBF

/* Bitmasks diagnosis status2 */
#define PRM_REQ 0x01
#define NO_PRM_REQ 0xFE
#define STAT_DIAG 0x02
#define NO_STAT_DIAG 0xFD
#define PROFIBUS_DP 0x04
#define WD_ON 0x08
#define NO_WD_ON 0xF7
#define FREEZE_MODE 0x10
#define NO_FREEZE_MODE 0xEF
#define SYNC_MODE 0x20
#define NO_SYNC_MODE 0xDF

/* Bitmasks diagnosis status3 */
#define EXT_DIAG_OVERFLOW 0x80
#define NO_EXT_DIAG_OVERFLOW 0x7F

/* Bitmasks prm_data status */
#define PRM_RESERVED 0x07
#define FREEZE_REQ 0x10
#define SYNC_REQ 0x20
#define UNLOCK 0x40
#define DPLOCK 0x80

/* Bitmasks control_command */
#define CONTROL_RESERVED 0xC1
#define CLEAR 0x02
#define UNFREEZE 0x04
#define FREEZE 0x08
#define UNSYNC 0x10
#define SYNC 0x20

/* Bitmasks dp_status */
#define STATUS_TIMEOUT 0x01
#define STATUS_NO_TIMEOUT 0xFE
#define STATUS_CLEAR 0x02
#define STATUS_NO_CLEAR 0xFD
#define STATUS_LEAVE 0x04
#define STATUS_NO_LEAVE 0xFB
#define STATUS_BAUDRATE_FOUND 0x08
#define STATUS_NO_BAUDRATE_FOUND 0xF7
#define STATUS_FREEZE 0x10
#define STATUS_NO_FREEZE 0xEF
#define STATUS_SYNC 0x20
#define STATUS_NO_SYNC 0xDF
#define STATUS_OUTPUTS_NEW       0x40
#define STATUS_NO_OUTPUTS_NEW    0xBF
#define STATUS_DATA_EXCH 0x80
#define STATUS_NO_DATA_EXCH 0x7F

/* Bitmasks dp_mode */
#define ADD_CHANGE_DISABLED 0x01
#define REAL_NO_ADD_CHANGED 0x02
#define FREEZE_NOT_SUPPORTED 0x10
#define SYNC_NOT_SUPPORTED 0x20

#define INVALID 0xFF

/*------------------------------------------------------------------
   the following section contains the standard datatypes which rely
   on the Fieldbus Message Specification of the ISP-Protocol.
 -------------------------------------------------------------------
        datatype        definition        Name-Prefix   Type-Index
 ----------------------------------------------------------------- */
typedef unsigned char boolean;  /* b                 1   */
typedef signed char integer8;   /* i8                2   */
typedef int integer16;          /* i16               3   */
typedef long integer32;         /* i32               4   */
typedef unsigned char unsigned8;        /* u8                5   */
typedef word unsigned16;        /* u16               6   */
typedef unsigned long unsigned32;       /* u32               7   */
                                                                                                                                                                                  /* #define float           float *//* f                 8   */
typedef unsigned char visible_string;   /* vs                9   */
typedef unsigned char octet_string;     /* os               10   */
typedef unsigned char bit_string;       /* bs               14   */

typedef unsigned8 u08;
typedef unsigned16 u16;
typedef unsigned32 u32;

typedef int TBool;

#include "targetdefs.h"

/*     Ende Datatypes
**
****************************************************************************/
#endif
