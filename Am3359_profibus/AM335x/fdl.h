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
***     Zielsystem   : portabel
***     Hilfsmittel  : Borland-C++ V3.1,Keil-C V3.35,Intel-C V4.5
***
*****************************************************************************
***
***     Autor        : Holger BÅttner
***     Dateiname    : fdl.h
***
*****************************************************************************
***
***     Beschreibung : Dieses Modul enthaelt Typdefinitionen und
***                    Funktionsprototypen sowie die anderen Modulen
***                    bekannt zu machenden Variablen des Moduls
***                    fdl.c
***
*****************************************************************************
***
***     Version       Date       Name         Comment
***
***     1.0           20.09.93   HBu          erste Version
***
***            Typdefinitionen in fdl.h
***
***     1.1           09.06.94   HBu          zweite Version
***
****************************************************************************/


#ifndef _FDLH_
#define _FDLH_

#include "puffer.h"

/****************************************************************************
**
**     Datentypes / Defines
*/

#define OK 0                    /* This values are from the FDL spec. */

/*     Ende Datentypen/Defines
**
****************************************************************************/


/****************************************************************************
**
**      Funktions-Prototypen
*/

#if (SW_DMA == 0)
#else
extern void DmaCpy(void XDATA * pDest, void XDATA * pSource, u32 n);
extern void DmaSet(void XDATA * pDest, byte by, u32 n);
#endif

/****************************************************************************
*
*      Name          : set_value
*      Parameter     : var_name      = variable index
*                      desired_value = value to be set
*      Rueckgabewert : OK
*      Beschreibung  : Diese Funktion setzt veerschiedene Schicht 2-Variablen
*
****************************************************************************/
void set_value(byte variable_name, byte XDATA desired_value);


/****************************************************************************
*
*      Name          : fdl_reply_update
*      Parameter     : point = pointer to a buffer
*      Rueckgabewert : OK
*      Beschreibung  : Diese Funktion uebeergibt einen FDL_Reply_Update-Request
*                      an die Schicht 2
*
****************************************************************************/
void fdl_reply_update(TBufPtr point);


/****************************************************************************
*
*      Name          : sap_reset
*      Parameter     : sap_nr         = index of the SAP
*      Rueckgabewert : OK
*      Beschreibung  : Diese Funktion haengt alle Puffer aus der NP- bzw. der
*                      HP-Schlange in die leere Schlange der SAP-Verwaltung des SAPs
*
****************************************************************************/
void sap_reset(byte sap_nr);

/****************************************************************************
*
*      Name          : get_sap_puffer
*      Parameter     : sap     SAP
*      Rueckgabewert : Puffer
*      Beschreibung  : Diese Funktion gibt einen Puffer aus der leeren Schlange der
*                      der SAP-Verwaltung eines SAPs zurueck
*
****************************************************************************/
extern TBufPtr get_sap_puffer(byte sap_nr);

/****************************************************************************
*
*      Name          : put_sap_puffer
*      Parameter     : sap   SAP
*                      p     Puffer
*      Rueckgabewert : --
*      Beschreibung  : Diese Funktion haengt den uebergebenen Puffer in die
*                      leere Schlange der SAP-Verwaltung des SAPs
*
****************************************************************************/
#define put_sap_puffer(nr,p) inqueue(&(g_pL2GlobVar->empty_queue),p);

/****************************************************************************
*
*      Name          : set_input_priority
*      Parameter     : 0 = Low-Prior, 1 = High-Prior
*      Beschreibung  : Diese Funktion setzt die PrioritÑt fÅr die Input-
*                      Daten.
*
****************************************************************************/
extern void set_input_priority(byte prior);

/****************************************************************************
*
*      Name          : update_diag
*      Parameter     : Zeiger auf die Diagnosedaten
*      Beschreibung  : Diese Funktion aktualisiert die Diagnose-daten
*
****************************************************************************/
extern void update_diag(TDiagBuffer * pdiag);

/****************************************************************************
*
*      Name          : update_dpv1
*      Parameter     : Zeiger auf die DPV1-Daten
*      Beschreibung  : Diese Funktion sendet DPV1-Daten zurÅck
*
****************************************************************************/
extern void update_dpv1(TBufOfs buffer);

/****************************************************************************
*
*      Name          : update_diagstatus
*      Parameter     : Zeiger auf die Diagnosedaten
*      Beschreibung  : Diese Funktion aktualisiert die ersten 3 Status-Bytes
*                      in der Diagnose (Daten stehen in der Variablen
*                      diag_buffer.status1/2/3
*
****************************************************************************/
extern void update_diagstatus(void);

/****************************************************************************
*
*      Name          : update_config
*      Parameter     : Zeiger auf die Config-daten
*      Beschreibung  : Diese Funktion aktualisiert die Config-daten
*
****************************************************************************/
extern void update_config(TBufPtr pconf);

/****************************************************************************
*
*      Name          : user_error
*      Parameters    : error_code
*                      HEAP_OVERFLOW - the INTERNAL_HEAP_SIZE must be increased
*      Return Values : --
*      Description   : function will be called, if there is an initialization
*                      error
*
****************************************************************************/
void user_error(byte error_code);

/****************************************************************************
*
*      name          : fdl_sync
*      description   : called from DP at receipt of SYNC or UNSYNC.
*                      If there are new outputs, they will be delivered to
*                      the user.
*
****************************************************************************/
extern void fdl_sync(void);

/****************************************************************************
*
*      name          : fdl_freeze
*      description   : called from DP at receipt of FREEZE or UNFREEZE
*                      if there are new inputs from the user, they will be
*                      activated for the next response
*
*
****************************************************************************/
extern void fdl_freeze(void);


/****************************************************************************
*
*      name          : QL7Dispatch
*      description   : called from mainloop to dispatch the frames and events
*                      which are sent from layer 2 to layer 7
*
*
****************************************************************************/
extern void QL7Dispatch(void);



/****************************************************************************
*
*      name          : timeout_timer
*      description   : called from software-timer when there was no valid
*                      start delimiter during the last interval. The baudrate
*                      will be changed.
*
****************************************************************************/
extern void timeout_timer(void);


/****************************************************************************
*
*      name          : watchdog_timer
*      description   : called from software-timer when the cyclic connection
*                      monitoring timer occurs.
*
****************************************************************************/
extern void watchdog_timer(void);


extern void inqueue(TQuPtr s, TBufPtr p);
extern TBufPtr outqueue(TQuPtr s);
extern TBufPtr exchange_queue(TQuPtr s, TBufPtr p);

#define set_diag_status2_or(value) set_diag_status2_and_or(0xff,value)
#define set_diag_status2_and(value) set_diag_status2_and_or(value,0)
extern void set_diag_status2_and_or(byte value_and, byte value_or);
extern void set_dp_status_or(byte value);
extern void set_dp_status_and(byte value);


/*     Ende Funktions-Prototypen
**
****************************************************************************/

#endif
