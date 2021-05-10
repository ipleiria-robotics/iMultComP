
#ifndef _L2_H_
#define _L2_H_


/****************************************************************************
***
***     TMG i-tec Gesellschaft fuer industrielle Informationstechnik mbH
***
*****************************************************************************
***
***     Author       : Joerg Kuehner
***     Filename     : UARTFDL.H
***
*****************************************************************************
***
***     Description  : This file contains definitions for memory interface
***                    layer 7<=>2
***
****************************************************************************/
/** 
 *  @file       L2.h
 *
 *  @brief      This file contains definitions for memory interface
 *              layer 7<=>2
 *
 * 
 * 
 */
 /** @addtogroup PB_ARM_STACK_TO_FIRMWARE
 * The IPC between these two happens through a shared memory structure @ref TL2GlobVar which is maintained 
 * in ICSS shared RAM (base address: 0x4b210000) 
 @{ */
#include "basictyp.h"
#include "puffer.h"
 
/**
 * 
 * @brief Used to specify Optimisation
 */
#define SW_RX_OPTIMIZATION  1

/**
 * 
 * @brief Used to specify L2 Version
 */
#define VERSION_L2_IFACE  0x01000000


/**
 * 
 * @brief Lowest SAP
 */
#define LOWEST_SAP            40        /* DPV1 */

/**
 * 
 * @brief Highest SAP
 */
#define HIGHEST_SAP           66        /* 63 + NILSAP + SMSAP */


/**
 * 
 * @brief Telegram buffer size
 */
#define TELEGRAM_BUFFER_MAX   21        /* 10k */

#ifndef MAX_FDL_BUFFER_LEN
/**
 * 
 * @brief FDL maximum buffer length
 */
#define MAX_FDL_BUFFER_LEN 255
#endif



/* Mask for active_services byte */
#define SM_BIT       1
#define SDN_BIT      2
#define SRD_BIT      4
#define IND_DATA     8
#define DPV1_BIT     8
#define PUB_BIT   0x10
#define SUB_BIT   0x20
#define DP_BIT    0x40
#define CONF_BIT  0x80
#define TOO_LONG  0x10


/**
 * 
 * @brief Access definitions, ALL = OPEN for all partners
 */
#define ALL 255


/*----------------- Defines for Indications/Confirmations -----------------*/

/**
 * 
 * @brief Defines for Indications/Confirmations:Send
 */
#define SEND          0x01
/**
 * 
 * @brief Defines for Indications/Confirmations:Receive
 */
#define RECEIVE       0x02
/**
 * 
 * @brief Defines for Indications/Confirmations:send_receive
 */
#define SEND_RECEIVE  0x03
/**
 * 
 * @brief Defines for Indications/Confirmations:Initiate Indication
 */
#define INITIATE_IND  0x04

/*-------------------- RM_Registry - all saps are in use ------------------*/

/**
 * 
 * @brief RM_Registry - all saps are in use
 */
#define RM_OUTOF_SAP 0xFF



/**
 * 
 * @brief SetValue parameter - Station
 */
#define STATION       1
/**
 * 
 * @brief SetValue parameter - Baud rate
 */
#define BAUD_RATE     2
/**
 * 
 * @brief SetValue parameter - User select
 */
#define UART_SEL      3

/**
 * 
 * @brief SetValue parameter - Min TSDR
 */
#define MIN_TSDR      7

/**
 * 
 * @brief Contains the actual diagnosis buffer of layer 7
 */
typedef struct {
    /* this part must be identical to TBuffer */
    TBufOfs next;               ///< pointer to next entry                     
    byte reservedlli,           ///< Reserved for LLI                         
     modus,                     ///< Modus for Transmit und Priority         
     laenge,                    ///< length of FDL-data                       
     pa,                        ///< remote Address                            
     osap,                      ///< local SAP                                
     psap;                      ///< remote SAP                               
    /* this part is diag specific lli_pdu[245] */
    byte status1, status2, status3, master_add;///<status and master address
    byte ident_number[2];///< ident number
    byte ext_diag_data[238];///< diagonal data
} TDiagBuffer;


/**
 * 
 * @brief In queue command request
 */
#define CMD_INQUEUE_REQ            0xA5
/**
 * 
 * @brief In queue command response
 */
#define CMD_INQUEUE_RES            0xAA
/**
 * 
 * @brief out queue command request
 */
#define CMD_OUTQUEUE_REQ           0xA6
/**
 * 
 * @brief out queue command response
 */
#define CMD_OUTQUEUE_RES           0xA9
/**
 * 
 * @brief set value command request
 */
#define CMD_SET_VALUE_REQ          0x10
/**
 * 
 * @brief set value command response
 */
#define CMD_SET_VALUE_RES          0x1F
/**
 * 
 * @brief exchange command request
 */
#define CMD_EXCHQUEUE_REQ          0x33

/**
 * 
 * @brief exchange command response
 */
#define CMD_EXCHQUEUE_RES          0x3C
/**
 * 
 * @brief Value to be AND and OR ’ed
 *        with diag_buffer.status2. (L7
 *        write)
 */
#define CMD_SET_STATUS2_AND_OR_REQ 0x66
/**
 * 
 * @brief Value to be OR’ed with dp_status. (L7 write)
 */
#define CMD_SET_STATUS2_AND_OR_RES  0x69
/**
 * 
 * @brief Value to be OR’ed with dp_status. (L7 write)
 */
#define CMD_SET_DP_STATUS_OR_REQ   0x60
/**
 * 
 * @brief Response to CMD_SET_DP_STATUS_OR_REQ
 */
#define CMD_SET_DP_STATUS_OR_RES   0x6F
/**
 * 
 * @brief Value to be AND'ed with dp_status. (L7 write)
 */
#define CMD_SET_DP_STATUS_AND_REQ  0x63
/**
 * 
 * @brief Response to CMD_SET_DP_STATUS_AND_REQ
 */
#define CMD_SET_DP_STATUS_AND_RES  0x6C


/**
 * 
 * @brief command structure
 */
typedef struct {
    u08 Cmd; ///<command
} TTeleCmd;

/**
 * 
 * @brief In queue request structure
 */
typedef struct {
    u08 Cmd; ///<command
    u08 x1, x2, x3;///<for Alignment
    TQuOfs s; ///< Queue offset
    TBufOfs p; ///< buffer offset
} TTeleInqueueReq;

/**
 * 
 * @brief In queue response structure
 */
typedef struct {
    u08 Cmd; ///<command
    u08 x1, x2, x3;///< for Alignment
} TTeleInqueueRes;

/**
 * 
 * @brief Out queue request structure
 */
typedef struct {
    u08 Cmd;///<command
    u08 x1, x2, x3;///<for Alignment
    TQuOfs s; ///< Queue offset
} TTeleOutqueueReq;

/**
 * 
 * @brief Out queue response structure
 */
typedef struct {
    u08 Cmd; ///<command
    u08 x1, x2, x3;///< for Alignment
    TBufOfs out;///< buffer offset
} TTeleOutqueueRes;

/**
 * 
 * @brief Exchange request structure
 */
typedef struct {
    u08 Cmd;///<command
    u08 x1, x2, x3;///< for Alignment
    TQuOfs s;///< Queue offset
    TBufOfs InOut;///< buffer offset
} TTeleExchqueue;

/**
 * 
 * @brief set value request structure
 */
typedef struct {
    u08 Cmd;///<command
    u08 variable_name;///<variable name
    u08 desired_value;///<desired value to be set
    u08 x1;///< for Alignment
    u32 mintsdr;///<Min tsdr
} TTeleSetValue;

/**
 * 
 * @brief set status structure
 */
typedef struct {
    u08 Cmd;///<command
    u08 Value;///<value
    u08 Value2;///<value2
    u08 x1;///< for Alignment
} TTeleSetStatus;

/**
 * 
 * @brief structure of command type
 */
typedef union {
    TTeleCmd Cmd;///<command
    TTeleInqueueReq InqueueReq; ///< in queue request
    TTeleInqueueRes InqueueRes;///< in queue response
    TTeleOutqueueReq OutqueueReq;///< out queue request
    TTeleOutqueueRes OutqueueRes;///< out queue response
    TTeleExchqueue Exchqueue;///< exchange 
    TTeleSetValue SetValue;///< set value 
    TTeleSetStatus SetStatus;///< set status
} TCmd;


#define RM_REUSE   0xF9
#define RM_FIND    0xFA


/**
 * 
 * @brief shared memory structure which is maintained in L3 RAM
 * The stack APIs can appropriately access this structure to communicate with FDL 
 */
typedef struct {
/* type     name                        description                                 written by Layer 7     Layer2           */
/* -------------------------------------------------------------------------------------------------------------------------*/
    u32 V;                      ///< VERSION_L2_IFACE                                                                 
    byte dp_mode;               ///< Address_Change / Sync / Freeze modes       
    byte dp_in_len;             ///< DP Inputs length                          
    byte dp_out_len;            ///< DP Output length                          
    byte dp_status;             ///<Status of Application                    

    byte last_sap;              ///< Next connection for dpv1                              
    byte control_command;       ///< Command of GCT                                        
    byte group_sel;             ///< Group select of new global control                    
    byte active_group_ident;    ///< Actual group                                          

    byte leave_master;          ///< disconnect request illegal tele length                 
    byte x1, x2, x3;            ///< for alignment                                          
    u32 TimerWatchdogReset;     ///< telegrams received                                     
    u32 StartDelimiterCounter;  ///< counts start delimiters for baud rate search           

#if (SW_DMA == 0)
    TBufOfs out_wait;           ///< buffer which is waiting for exchange                   
    TBufOfs out_user;           ///< buffer which is used by user for outputs               
    TBufOfs last_output_buffer; ///< last valid output buffer                               
#else
    TBufOfs out_user;           ///< buffer which is used by user for outputs              
    TBufOfs Out;                ///< cyclic out data for L2 (synced) write                                            
    TBufOfs OutSync;            ///< cyclic out data for L2 (direct) write, L7 read        
    TBufOfs In;
    TBufOfs InFreeze;
#endif

    byte outputs_new;           ///< Sync mode: is there a new indication ?                 
    byte outputs_waiting;       ///< Sync mode: is there a new indication ?                 

    byte ind_sapRM;             ///< RM_FIND or RM_REUSE, tells Layer 7 how to handle RMSap
    byte x4;                    ///< for alignment                                                                     
    TBufOfs ind_sap55;          ///< pointer to indication buffer for sap 55               
    byte ind_sap58;             ///< TRUE if indication for sap 58                         
    byte ind_sap60;             ///< TRUE if indication for sap 60                         
    byte x5, x6;                ///< for alignment                                                                
    TBufOfs ind_sap61;          ///< pointer to indication buffer for sap 61               
    TBufOfs ind_sap62;          ///< pointer to indication buffer for sap 62               

    TQueue ind_dpesaps;         ///< queue for acyclic indications                          

    TSap saptab[HIGHEST_SAP];   ///< table with configuration and management of saps        

    TQueue empty_queue;         ///< queue of currently unused telegrams                   

    TBuffer buftab[TELEGRAM_BUFFER_MAX];        ///< telegram buffers (either in empty_queue                
    /* or in sap.send_queue)                                                            */
    TBuffer ident_buffer;       ///< buffer containing ident informations                  

    TDiagBuffer diag_buffer;    ///< contains the actual diagnosis buffer of layer 7       

    TBuffer cfg_buffer;         ///< buffer for configuration data */
    TCmd Cmd;

    // ----------------------------------------------------

    // BSP Structures (not Stack)
    unsigned int PruDmaChannel; ///< Pru DMA channel
    byte DmaSetVal[8];          ///< Pattern used in DmaMemSet operation  
    u32 saptaboffset[HIGHEST_SAP];///< Lookup table for sap offsets
    unsigned int ArmDmaChannel;///< EDMA channel allocated by host for L7 transfers
    byte PruTxGpioNum;///< Set to n for pr1_pru0_pru_r30[n] used as TX GPIO for PROFIBUS
    byte x7, x8, x9;///< for alignment
    byte tempDmaSetVal[256];///< temp EDMA set value

    /*In case pru_gpo pin is not available for TX GPIO, GPIO pin from the SoC can be used. The following structure 
    members can be used for configuring the GPIO pin for TX. Pinmux configuration to use the pin as GPIO and the initialization of the particular GPIO module should be done for this scheme to work properly.
    Maximum throughput of 12Mbaud may not be achievable with this configuration due to the additional latency of the PRU accessing the GPIO */
    byte sel_GPO_GPIO ;  ///< TX enable pin is controlled by GPIO or PRU. 1 for PRU and 0 for GPIO
    byte x10, x11, x12;///< for alignment
    u32 gpio_address;  ///< GPIO DOUT register address
    u32 L3_mem_start_addr; ///< L3 memory start address
    u32 pru_base_addr; ///< PRU Data memory address for EDMA
    u32 edma_cc_base_addr; ///< EDMA base address
} TL2GlobVar;

/**
 * 
 * @brief Global data structure placed in shared memory 
 */
extern TL2GlobVar XDATA *g_pL2GlobVar;

#define sap_xaccess(nr,acc,sap) {g_pL2GlobVar->saptab[nr].access=acc;g_pL2GlobVar->saptab[nr].access_sap = sap;}


extern TBufOfs BufOfs(TBufPtr p);
extern TBufPtr BufPtr(TBufOfs o);
extern TQuOfs QuOfs(TQuPtr p);
extern TQuPtr QuPtr(TQuOfs o);



#define TIMER_WATCHDOG_RESET_L2 ++(g_pL2GlobVar->TimerWatchdogReset)    /* processdata time supervision restart */





/**
@}
*/

#endif                          /* _L2_H_ */
