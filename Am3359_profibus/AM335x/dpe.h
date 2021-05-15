/***************************************************************************/
/**                                                                       **/
/**     TMG i-tec Gesellschaft fuer industrielle Informationstechnik mbH  **/
/**                                                                       **/
/**     Telefon: +49-721-96410-0                                          **/
/**     Telefax: +49-721-96410-10                                         **/
/**                                                                       **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Targetsystem : portable                                           **/
/**     Tools        : Borland-C++ (Ansi) / IAR-C (68HC11) / keil-C (8031)**/
/**                    Intel-C (V25,80188)                                **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Author       : Oliver H. Wirkus                                   **/
/**     Filename     : dpe.h                                              **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Decription   : This module contains the datatypes, defines,       **/
/**                    global variables and global functions of the       **/
/**                    module dpe.c                                       **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Version        Date       Name         Comment                    **/
/**                                                                       **/
/**     1.0            18.03.97   OWi          first release              **/
/**                                                                       **/
/***************************************************************************/
#ifndef _DPEH_
#define _DPEH_


/***************************************************************************/
/**                                                                       **/
/**     Datatypes / Defines                                               **/
/**                                                                       **/
/***************************************************************************/
#include "basictyp.h"
#include "puffer.h"

#define DPV1_CONNECTIONS    2


/*------------ these are the states used in structure SAP_STATE -----------*/
#define CLOSED                      0
#define INITIATE_WAIT_RES           2
#define INITIATE_WAIT_UPDATE        3
#define INITIATE_SEND_RES           4
#define IDLE_WAIT_UPDATE            5
#define IDLE_SEND_REQ               6
#define WAIT_IDLE_CON               7
#define SEND_ABORT_SEND_RES         8
#define CLEAR_UPDATE                9
#define OPEN                       10
#define VALID_SERVICE_WAIT_RES     11
#define VALID_SERVICE_WAIT_UPDATE  12
#define VALID_SERVICE_SEND_RES     13

/*--------------- Abort Codes for INSTANCE and REASON-CODE ----------------*/
#define NO_SUBNET 0

#define DDLM_ABT_SE  0x11
#define DDLM_ABT_FE  0x12
#define DDLM_ABT_TO  0x13
#define DDLM_ABT_RE  0x14
#define DDLM_ABT_IV  0x15
#define DDLM_ABT_STO 0x16
#define DDLM_ABT_IA  0x17
#define DDLM_ABT_OC  0x18

/*----------------- Defines for Indications/Confirmations -----------------*/
#define SEND          0x01
#define RECEIVE       0x02
#define SEND_RECEIVE  0x03
#define INITIATE_IND  0x04

/*------------- Defines for RETURN_STATE in struct SAP_STATE --------------*/
#define INITIATE      0
#define VALID_SERVICE 1

/*----------------- Defines for flags in struct SAP_STATE -----------------*/
#define GoAbort    0x01
#define BufferFull 0x02

/*-------------------- RM_Registry - all saps are in use ------------------*/
#define RM_OUTOF_SAP 0xFF

/*------------------- structure of state-machine for dpe-saps -------------*/
typedef struct {
    byte server_sap;            /* Server SAP */
    byte state;                 /* one of the above states */
    byte req_add;               /* address of master */
    byte timer_stat;            /* Timer Status */
    byte timer_ident;           /* Ident-Code of Timer */
    long timer_val;             /* Value of Timer */
    byte service_header;        /* Service Header */
    byte flags;                 /* Flags */
    TBufPtr service_buffer;     /* Service-Buffer */
    byte abort_subnet;          /* stored ABORT-Param. */
    byte abort_instance_reason; /* stored ABORT-Param. */
    byte abort_identifier;      /* stored ABORT-Param. */
    byte return_state;          /* RETURN-state (idle) */
} SAP_STATE;

extern SAP_STATE sap_state[DPV1_CONNECTIONS + 2];

/**ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿**/
/**³Description of flags:                                                ³**/
/**³                                   Meaning when set:                 ³**/
/**³                                   -----------------                 ³**/
/**³                     Bit 0 (LSB) : Go_Abort                          ³**/
/**³                     Bit 1       : Service Buffer not empty          ³**/
/**³                     Bit 2       : Valid RM-initiate                 ³**/
/**ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ**/



/*--------------- checks if pdu contains a 'valid service' ----------------*/
#define IsValidServiceRES(p)   ((p->lli_pdu[0]==0x5E)|| \
                                (p->lli_pdu[0]==0xDE)|| \
                                (p->lli_pdu[0]==0x5F)|| \
                                (p->lli_pdu[0]==0xDF)|| \
                                (p->lli_pdu[0]==0x51)|| \
                                (p->lli_pdu[0]==0xD1))

/*---------------- checks if pdu contains an initiate-response ------------*/
#define IsInitiateRES(p)       ((p->lli_pdu[0]==0x57)|| \
                                (p->lli_pdu[0]==0xD7))

/***************************************************************************/
/**                                                                       **/
/**     End Datatype / Defines                                            **/
/**                                                                       **/
/***************************************************************************/


/***************************************************************************/
/**                                                                       **/
/**     Global Variables                                                  **/
/**                                                                       **/
/***************************************************************************/

extern word Min_Send_Timeout;
extern byte Max_Len_Data_Unit;
/***************************************************************************/
/**                                                                       **/
/**     End Global Variables                                              **/
/**                                                                       **/
/***************************************************************************/



/***************************************************************************/
/**                                                                       **/
/**     Function Prototypes                                               **/
/**                                                                       **/
/***************************************************************************/
void do_dpe(void);
void pb_init_dpe(void);
void sys_initiate_ind(TBufPtr);
void sys_abort_ind(TBufPtr);
void sys_alarm_ind(TBufPtr);
void sys_valid_service_ind(TBufPtr);
void sys_idle_ind(TBufPtr);
void sys_abort_req(byte, byte, byte);
void sys_illegal_pdu(TBufPtr);
void sys_confirm_pdu(TBufPtr);
void sys_response(TBufPtr);
void gen_abort_pdu(TBufPtr, byte, byte);
void gen_idle_pdu(TBufPtr);
void insert_RM_answer(TBufPtr, byte);
void free_api(byte);

extern void dpe_timer_overflow(byte);

/***************************************************************************/
/**                                                                       **/
/**     User-Settings                                                     **/
/**                                                                       **/
/***************************************************************************/

/***************************************************************************/
/**                                                                       **/
/**     End of User-Settings                                              **/
/**                                                                       **/
/***************************************************************************/

#endif
