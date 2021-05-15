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
/**     Tools        :                                                    **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Author       : Detlef Arndt                                       **/
/**     Filename     : DPC1.H                                             **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Decription   : This module contains the datatypes, defines,       **/
/**                    global variables and global functions of the       **/
/**                    module DPC1.C                                      **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/**     Version        Date       Name         Comment                    **/
/**                                                                       **/
/**     1.0            23.06.98   DAr          first release              **/
/**                                                                       **/
/***************************************************************************/
#ifndef _DPC1H_
#define _DPC1H_


/***************************************************************************/
/**                                                                       **/
/**     Datatypes / Defines                                               **/
/**                                                                       **/
/***************************************************************************/
/*--------------------- structure of user-defined api-table ---------------*/
typedef struct {
    byte max_l_sdu_length_req_low;
    byte max_l_sdu_length_req_high;
    byte max_l_sdu_length_ind_low;
    byte max_l_sdu_length_ind_high;
} ALARM_LIST;

typedef struct {
    byte alarm_status[8][32];
    byte alarm_type[8][32];
} ALARM_TABLE;

typedef struct {
    byte alarm_class;
    byte alarm_seq;
} ALARM_INDEX;


/*--------------------- structure of user-defined ALARM-PDU ---------------*/
typedef struct {
    byte headerbyte;
    byte alarmtype;
    byte slot_number;
    byte specifier;
    byte diag_data[60];
} ALARM_DIAG;

typedef struct {
    byte alarmtype;
    byte slot_number;
    byte sequence;
    byte specifier;
} ALARM_PDU, STATUS_PDU;

typedef struct {
    byte fill_number;
    byte first_index;
    byte last_index;
    ALARM_DIAG alarm_pdu[32];
} ALARM_FIFO;

typedef struct {
    byte alarm_pdu_len;
    byte status_pdu_len;
    byte ext_diag_buffer_len;
    byte XDATA *alarm_buffer;
    byte XDATA *status_buffer;
    byte XDATA *ext_diag_buffer;
} DIAG_BLOCK;



/*----------------- SAP Defines -------------------------------------------*/
#define C1_ALARM_SAP  50
#define C1_SERVER_SAP 51
#define C1_MASTER_SERVER_SAP 51

/***************************************************************************/
/**                                                                       **/
/**     End Datatype / Defines                                            **/
/**                                                                       **/
/***************************************************************************/
/*------------ additional states used in structure SAP_STATE for MSAC1S ---*/
#define ALARM_ACK_SEND_RES         14
#define ALARM_ACK_WAIT_RES         15

/*------------ additional states used in structure SAP_STATE for MSAC1S ---*/
#define ALARM_ACK_REQ_PDU 0x5C
#define READ_REQ_PDU      0x5E
#define WRITE_REQ_PDU     0x5F

/*------------ additional states used in msal1s_alarm_state for MSAL1 SM --*/
#define W_START           0x00
#define W_DIA_UPD         0x01
#define W_FETCHED         0x02
#define CHK_TRANSMIT      0x03
#define RESET_PENDING     0x04
#define RESET_P_LEAVE     0x05


#define WRITE_RES_PDU_LEN 4
#define ERROR_RES_PDU_LEN 4

#define IsValidReadWriteServiceRES(p)  ((p->lli_pdu[0]==0x5E)|| \
                                       (p->lli_pdu[0]==0xDE)||  \
                                       (p->lli_pdu[0]==0x5F)||  \
                                       (p->lli_pdu[0]==0xDF))
/*------------ operation_mode ---------------------------------------------*/
#define V0 0
#define V1 1
/*------------ start_states -----------------------------------------------*/
#define IDLE_STATE 0
#define RUN_STATE  1
#define B_STATE    2
#define E_STATE    3
#define BE_STATE   4
#define EB_STATE   5
#define BEB_STATE  6


#define NOT_PENDING 0
#define PENDING     1
#define EMPTY       0
#define NOT_EMPTY   1

/*------------ return codes Alarm SM  -------------------------------------*/
#define NO_START                1
#define NOT_ENABLED             2
#define LIMIT_EXPIRED           3
#define SEQUENCE_NR_PENDING     4
#define MSAL1S_STOPPED          5
#define ALARM_PENDING           0xFF


/*-------- macros ---------------------------------------------------------*/
#define LOCAL_BUFFER_LEN  64
#define STATUS_BUFFER_LEN 64

/*-------- set_prm extensions (user_prm_data) ------------------------------*/
#define DPV1_Status_1 (DPV1Status[0])
#define DPV1_Status_2 (DPV1Status[1])
#define DPV1_Status_3 (DPV1Status[2])
#define DPV1_Enable   (DPV1_Status_1 &0x80)
#define Fail_Safe_Bit (DPV1_Status_1 &0x40)
#define WD_Base_1ms (DPV1_Status_1 &0x04)
#define Enable_Pull_Plug_Alarm             (DPV1_Status_2 & 0x80)
#define Enable_Process_Alarm               (DPV1_Status_2 & 0x40)
#define Enable_Diagnostic_Alarm            (DPV1_Status_2 & 0x20)
#define Enable_Manufacturer_Specific_Alarm (DPV1_Status_2 & 0x10)
#define Enable_Status_Alarm                (DPV1_Status_2 & 0x08)
#define Enable_Update_Alarm                (DPV1_Status_2 & 0x04)
#define Check_Cfg_Mode                     (DPV1_Status_2 & 0x01)
#define Alarm_Mode                         (DPV1_Status_3 & 0x07)

/***************************************************************************/
/**                                                                       **/
/**     Global Variables                                                  **/
/**                                                                       **/
/***************************************************************************/

extern word Min_Send_Timeout;
extern byte Max_Len_Data_Unit;

/*----------------- Globals of module DPE.H used here --------------------*/

extern TSap *pSap_Dpe[DPV1_CONNECTIONS + 2];
extern byte first_dpe_sap;
extern word Min_Send_Timeout;
extern byte Max_Len_Data_Unit;

/*----------------- Globals of module DPC1.C used here --------------------*/
extern ALARM_DIAG XDATA *alarm_ptr;
/*--- variables of the MSCY1S -----------------*/
extern boolean application_ready;
extern byte operation_mode;
extern boolean prm_pending;
extern boolean cfg_pending;
extern boolean dx_entered;
extern byte start_state;
extern byte enabled_alarms;
extern byte alarm_mode_master;

/*--- variables of the MSAL1S -----------------*/
extern boolean alarm_sequence;
extern boolean initial_alarm_sequence;
extern ALARM_TABLE outstanding_alarm_table;
extern byte alarm_max;
extern byte alarm_limit;
extern byte alarm_count;
extern byte req_alarm_fifo;
extern byte actual_enabled_alarms;
extern byte alarm_buffer[LOCAL_BUFFER_LEN];
extern byte local_diag_buffer[LOCAL_BUFFER_LEN];
extern byte status_buffer[STATUS_BUFFER_LEN];
extern boolean stored_status;
extern boolean stored_diag;
extern byte alarm_table_index;
extern byte msal1s_alarm_state;
/*--- global variables ------------------------*/
extern byte UserPrmLen;
extern byte DPV1Status[3];

/*-------- global variables of the MSAC1S ----------------------------------*/
extern byte c1_response_sap;
extern byte c1_alarm_sap;
/* extern byte service_header; */
extern byte length_list;
extern ALARM_LIST alarm_length_list;
extern DIAG_BLOCK DiagBlock;
extern byte aa_state;
extern byte vs_upd;

extern CONST(byte) alarm_decode[8];

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
void pb_init_dpc1(void);
void msac1s_start_req(byte master_add);
void sys_c1_ind(TBufPtr);
void sys_c1_response(TBufPtr);
void sys_confirm_c1_pdu(TBufPtr b);
void msac1s_stop_req(void);
void sys_c1_abort_req(byte osap, byte subnet, byte instance_reason);
void msac1_read_ind(TBufPtr p);
void msac1_write_ind(TBufPtr p);
void msac1_alarm_ack_ind(TBufPtr p);
void msal1s_abort_ind(void);
void msac1s_abort_ind(void);
void msac1s_stopped_ind(void);
void mscy1s_start_ind(byte enabled_alarms,
                      byte alarm_mode_master, boolean diag_flag);


void mscy1s_init(byte Ext_Diag_Flag);

void mscy1s_change_diag_data(byte Ext_Diag_Flag);
void mscy1s_abort_req(void);
void mscy1s_diag_fetched_ind(void);
void ddlm_data_exchange_ind(void);
void c1_stop_and_abort(int sm, TBufPtr p);


/* macros of MSCY1S state machine */
void stop_asm(void);
void check_alarm_start(void);
void set_operation_mode(void);
void set_enabled_alarms(void);
void set_alarm_mode_master(void);
void start_c1(void);
void stop_c1(void);
void start_c1_con(void);
void stop_c1_con(void);
void set_enabled_status(void);

/* MSAL1s state machine */
byte msal1s_init(byte Alarm_Mode_Slave, boolean Default_Ext_Diag);
void mscy1s_stop_ind(void);
void mscy1s_stopped_ind(void);
void msal1s_abort_req(void);
byte msal1_alarm_req(byte Alarm_Type,
                     byte Slot_Number,
                     byte Seq_Nr,
                     byte Alarm_Specifier,
                     boolean Add_Ack, byte XDATA * Diagnostic_User_Data);
boolean msal1s_status_req(byte XDATA * Status);
boolean check_msac1_alarm_ack_ind(byte Slot_Number, byte Alarm_Type,
                                  byte Seq_Nr);
void msal1s_start_ind(byte Enabled_Alarms, byte Alarm_Mode_Master,
                      boolean Diag_Flag);
void msal1s_stopped_ind(void);
void msal1s_diag_upd_req(boolean Diag_Flag, byte XDATA * Upd_Diag_Buffer);
void msal1_alarm_neg_con(byte Alarm_Type, byte Slot_Number, byte Seq_Nr,
                         byte status);

/*--------------------- MSAL1S-functions ----------------------------------*/
void fill_outstanding_alarm_table(byte state);
byte search_in_outstanding_alarm_table(byte alarm_state);
void reset_req_alarm_fifo(void);
void store_to_req_alarm_fifo(ALARM_DIAG XDATA * alarm_pdu);
ALARM_DIAG XDATA *load_from_req_alarm_fifo(void);
boolean req_alarm_fifo_state(void);
byte acls(byte alarm_type);
/*--------------------- MSAL1S-macros -------------------------------------*/
boolean alarm_enabled(byte alarm_type);






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
