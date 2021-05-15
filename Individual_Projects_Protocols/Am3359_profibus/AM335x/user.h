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
***     Author       : Holger BÅttner
***     Filename     : example.h
***
*****************************************************************************
***
***     description :  Interface to application
***
*****************************************************************************
***
***     Version        Date       Name         Comment
***
***     1.0            20.09.93   HBu          first version
***
****************************************************************************/
#ifndef _USERH_
#define _USERH_

#include "puffer.h"

/****************************************************************************
**
**     Data Types / Defines
*/

#define min(a,b) (((a)<=(b))?(a):(b))

/*     end of definitions
**
****************************************************************************/

/****************************************************************************
**
**     declaration of global variables
*/

extern byte XDATA max_cfg_len;
extern byte XDATA max_cfg_and_diag_len;
extern byte XDATA max_user_prm_len;
extern byte XDATA max_rem_data_len;
extern byte XDATA max_dp_inp_len;
extern byte XDATA max_dp_outp_len;

extern byte XDATA max_ds_rdwr_len;

extern word realtime_clock_inc;



/*     end of declarations
**
****************************************************************************/

/****************************************************************************
**
**                  public functions of communication software
**		    which are used in application software
**                  prototypes
*/

/****************************************************************************
*
*      Name          : pb_reset
*      Parameter     : station   Stationsadresse
*      Rueckgabewert : OK
*      Beschreibung  : this function resets the stack to its initial values
*
****************************************************************************/
void pb_reset(byte station);


/****************************************************************************
*
*      Name          : init_dp
*      Parameter     : cfg_len - length of configuration data
*                      dp_mode - Bit 0 = 1: Add_Change_Disabled
*                      dp_mode - Bit 1 = 1: Real_No_Add_Changed
*                      dp_mode - Bit 4 = 1: Freeze_Not_Supported
*                      dp_mode - Bit 5 = 1: Sync_Not_Supported
*                      ident_number - pointer to Ident_Number
*      Return Value  : CONFIG_SUCCESS,BAD_CONFIGURATION
*      Description   : This function initializes the DP-communication software
*
****************************************************************************/

byte init_dp(byte cfg_len, byte dp_mode, byte XDATA * ident_number);

/****************************************************************************
*
*      Name          : do_dp
*      Parameter     : --
*      Return Value  : --
*      Description   : This function handles the DP-services and calls the
*                      user functions
*
****************************************************************************/

void do_dp(void);

/****************************************************************************
*
*      Name          : get_cfg_buffer
*      Parameter     : --
*      Return Value  : buffer for user configuration data
*      Description   : This function must be called once before calling init_dp
*		       to get a buffer for the Real_Cfg_Data
*
****************************************************************************/

byte XDATA *get_cfg_buffer(void);

/****************************************************************************
*
*      Name          : get_ext_diag_buffer
*      Parameter     : --
*      Return Value  : user buffer for Ext_Diag_Data
*      Description   : This function must be called once before calling
*		       update_diag_data to get the first buffer for Ext_Diag_Data
*
****************************************************************************/

byte XDATA *get_ext_diag_buffer(void);

/****************************************************************************
*
*      Name          : update_diag_data
*      Parameter     : ext_diag_len - length of Ext_Diag_Data
*                    : status Bit 0 - Prm_Req
*                    : status Bit 1 - Stat_Diag
*		     : status Bit 3 - Ext_Diag
*		     : status Bit 7 - Ext_Diag_Overflow
*      Return Value  : -
*      Description   : This function updates the Diag_Data and the
*		       Slave_Diag-SAP
*
****************************************************************************/

void update_diag_data(byte ext_diag_len, byte status);

/****************************************************************************
*
*      Name          : get_input_buffer
*      Rueckgabewert : Zeiger auf aktuellen u-Puffer fÅr Responses
*      Beschreibung  : Diese Funktion liefert einen Zeiger auf den aktuellen
*                      user-Puffer im Layer2
*
****************************************************************************/

extern TBufPtr in_user;         /* buffer which is used by user for inputs */
#define get_input_buffer() &in_user->lli_pdu[0]

/****************************************************************************
*
*      Name          : update_inputs
*      Rueckgabewert : Zeiger auf den neuen u-Puffer
*      Beschreibung  : Diese Funktion aktualisiert die Input-Daten im Layer2 und
*                      liefert als Ergebnis einen Zeiger auf den neuen User-
*                      puffer.
*
****************************************************************************/
extern byte XDATA *update_inputs(void);

/****************************************************************************
*
*      Name          : update_outputs
*      Rueckgabewert : Zeiger auf die Empfangsdaten im u-puffer
*                      Seiteneffekt: indication_is_new
*      Beschreibung  : Diese Routine prÅft nach, ob neue Output-Daten
*                      empfangen wurde, und liefert eine Zeiger auf die
*                      aktuellsten Daten.
*                      Die globale Variable indication_is_new
*                      wird auf True gesetzt, wenn die Daten neu sind.
*
****************************************************************************/
extern byte XDATA *update_outputs(void);






#endif
