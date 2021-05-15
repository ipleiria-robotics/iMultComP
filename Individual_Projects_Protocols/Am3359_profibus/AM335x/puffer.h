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
***     Tools        : Borland-C++ (Ansi) / IAR-C (68HC11) / Keil-C (8031)
***            Intel-C (V25,80188)
***
*****************************************************************************
***
***     Author       : Dirk Brauner
***     Filename     : puffer.h
***
*****************************************************************************
***
***     Description  :
***
***     This Modul contains the structures and macros for the management
***     of PDU-buffers
***
*****************************************************************************
***
***     Version       Date       Name         Comment
***
***     1.0           30.06.93   DBr/HBu      first release
***           04.10.94   DBr          inqueue and outqueue: functions
***                       firsout and lastout: macros
***
****************************************************************************/
#ifndef _PUFFERH_
#define _PUFFERH_


#define PRIOR_BIT    2          /* Bits in Modus            */

/****************************************************************************
**
**     Datentypen/Defines
*/

typedef u32 TBufOfs;

typedef struct {
     TBufOfs next;               /* offset of next entry */
     byte reservedlli,           /* Reserved for LLI    */
     modus,                     /* Modus for Transmit und Priority  */
     laenge,                    /* length of FDL-data  */
     pa,                        /* remote Adress    */
     osap,                      /* local SAP      */
     psap;                      /* remote SAP      */
    byte lli_pdu[245];          /* Array for the LLI-PDU  */
} TBuffer;

typedef TBuffer XDATA *TBufPtr;


/**** Description ************************************************
** this definition contains the strutrure of a PDU-buffer
**** End of description *****************************************/

typedef u32 TQuOfs;

typedef struct {                /* Definition of queues    */
    TBufOfs erster,             /* pointer to first entry  */
    letzter;                   /* pointer to last entry */
} TQueue, XDATA * TQuPtr;



/*** Description of structure ********************************
** This definition describes the structure of a queue
**** End of description **************************/


typedef struct {
    byte access,                /* Access for adressing  */
     access_sap,                /* Sap des Partners   */
     active_services,           /* activated services */
     max_fdl_laenge_s,          /* max. length of a send buffer  */
     max_fdl_laenge_r;          /* max. length of a receive buffer  */
    TQueue send_queue;          /* send queue */
} TSap;


/*** Description of structure ********************************
** This definition describes the structure of a sap management
**** End of Description ********************/

#define own_sap      osap
#define lli_reserved reservedlli

/****************************************************************************
*
*      Name          : inqueue
*      Parameters    : S = Queue
*                      P = pointer to buffer
*      Return Values : --
*      Description   : The macro inserts the buffer P at the end of qeueu S
*
****************************************************************************/

#if 0
extern void inqueue(TQuPtr s, TBufPtr p);
/****************************************************************************
*
*      Name          : outqueue
*      Parameters    : S = Queue
*                      P = Pointer for results
*      Return Values : P = pointer to the buffer
*      Description   : The macro gets one buffer from the begin of the queue S
*
****************************************************************************/

extern TBufPtr outqueue(TQuPtr s);
#endif

/****************************************************************************
*
*      Name          : firstout
*      Parameter     : S = Queue
*      Return Values : first entry in queue
*
****************************************************************************/
#define firstout(s) BufPtr((s).erster)


/****************************************************************************
*
*      Name          : exchange_queue
*      Parameters    : S = Queue
*                      P = Pointer for input and output buffer
*      Return Values : P = pointer to the old buffer, 0 if no existed
*      Description   : The macro exhanges a buffer in a queue with another
*
****************************************************************************/
extern TBufPtr exchange_queue(TQuPtr s, TBufPtr p);

/*    End of Definitions
**
****************************************************************************/
#endif
