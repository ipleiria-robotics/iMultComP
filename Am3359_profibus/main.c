/*
 * @file    main.c
 * @brief   This implements the main function of the profibus slave application
 *          Creates two tasks ProfibusTask and TimerTask
 *          TimerTask gives tickcount of 5ms using Task_sleep function
 *          ProfibusTask calls UART function for getting station address, and then calls
 *          the main of profibus stack
 *          It also has the profibus stack funtion calls to initiate stack and update input/output
 */
/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/* SysBios header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* C headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

/* Stack header files */
#include "basictyp.h"
#include "L2.h"
#include "user.h"

#include "dpe.h"
#include "dpc1.h"
#include "profibus_timer.h"
#include "iandm.h"
#include "targetdefs.h"
#include "fdl.h"
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <profi_hw_utils.h>
#include <profi_misc.h>
#include <profi_soc.h>
#include "version.h"
#include <examples/board/include/board_i2cLed.h>
/****************************************************************************
 *                      FUNCTION PROTOTYPES
 ****************************************************************************/
extern void profi_timer_isr(void);
extern int BSP_main(int argc, char **argv);
void send_alarm(void);
extern int bsp_msleep(unsigned long milisec);
void hexDumpInDbgUart(unsigned char *tStr, int n);
/****************************************************************************
 *                      GLOBAL VARIABLES
 ****************************************************************************/
extern unsigned long baseAddr;
unsigned char uartInstance = 0;
unsigned char bIsInDataExchange = 0;
int32_t uartPresent = 0;
signed char rxData = 0;
int32_t TimerCount = 0;
char rxBuffer[200];

/** \brief PRU-ICSS LLD handle */
PRUICSS_Handle pruIcssHandle;
/****************************************************************************
 *                      Defines
 ****************************************************************************/
#define EFUSE_REG 0x44e10604

/****************************************************************************
 *
 * definition of PROFIBUS-DP variables
 *
 ****************************************************************************/
#define MANUFACTURER_ID      452

const TIAndM0 IAndM0 =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     /*Header*/
    MANUFACTURER_ID,            /*Manufacturer Ident Number*/
    "AM57x              ",     /*Order ID*/
    {"xxxxxxxxxxxxxxxx"},
    0x0100,                     /*HW revision*/
    {'V', '1', '0', '0'},
    0x0000,
    0x0000,                     /*Profile*/
    {0x0, 0x0},                 /*block object=0 , parent class=0 (only used for PROFIBUS PA*/
    {1, 1},                     /*IM version*/
    {0x00, 0x00}                /*IM supported*/
};



/* ----------- slave specific parameters ------------ */
#define INPUT_LEN        8
#define OUTPUT_LEN       8


#define MAX_CFG_DIAG_LEN 20

#if INPUT_LEN == 0
const unsigned char myCfg1[] = { 0x27 };
#define IDENT_NR_H       0x47
#define IDENT_NR_L       0x11
#else
const unsigned char myCfg1[] = { 0x17, 0x27 };
#define IDENT_NR_H       0x0C
#define IDENT_NR_L       0xDA
#endif

byte rcode;

/* DP-buffer */
/* INPUT_LEN bytes digital inputs */
#if INPUT_LEN > 0
byte dp_inputs[INPUT_LEN];
#endif

/* OUTPUT_LEN bytes digital outputs */
byte dp_outputs[OUTPUT_LEN];

/* buffer for configuration */
byte *config_buffer;

/* buffer for user specific diagnosis */
byte *ext_diag_buffer;

/* buffer for inputs */
byte *input_buffer;

/* buffer for outputs */
byte *output_buffer;

/* Ident_Number */
byte ident_number[2];

#define  BUF_SIZE  250
byte dpv1_test_buffer[BUF_SIZE];


int bOutputsChanged = 0;
int bUpdateDiagData = 0;
int bSendAlarm = 0;
int slave_addr = -1;

jmp_buf myJumpBuf;
int ErrorOccured = 0;

extern PRUICSS_Config pruss_config[2 + 1];
/****************************************************************************
*
* functions provided by the user
*
****************************************************************************/

/****************************************************************************
*
*      Name          : user_error
*      Parameters    : error_code
*                      USER_ERROR_NOMEM no memory available
*      Return Values : --
*      Description   : function will be called, if there is an initialization
*                      error
*
****************************************************************************/
void user_error(byte error_code)
{
    ErrorOccured = error_code;
    longjmp(myJumpBuf, error_code);
}



/****************************************************************************
*
*      name          : make_dpe_error
*      description   : generates the error pdu for dpv1
*
****************************************************************************/
static void make_dpe_error(int result, TBufPtr p)
{
    p->lli_pdu[0] |= 0x80;      /* set error bit at function code*/
    p->laenge = 4;              /* set length for error pdu*/
    p->lli_pdu[1] = 128;        /* error-decode = DPv1*/
    p->lli_pdu[2] = result;     /* error code 1*/
    p->lli_pdu[3] = 0;          /* set error_code_2*/
}

/****************************************************************************
*
*      name          : set_prm_data
*      parameter     : user_prm_data - pointer to user specific parameters
*                      user_prm_len - length of user specific paramters
*      return value  : CONFIG_SUCCESS, BAD_PARAMETER
*      description   : routine that checks and handles the
*                                user specific parameters received
*
****************************************************************************/

byte set_prm_data(byte XDATA *user_prm_data, byte user_prm_len)
{
    /* if C1-connections used : octets 8-10 are medatory with user_prm_len <= 3 */
    UserPrmLen = user_prm_len + 7;      /* don't change this variable */

    DPV1Status[0] = user_prm_data[0];
    DPV1Status[1] = user_prm_data[1];
    DPV1Status[2] = user_prm_data[2];

    if(user_prm_len == 3)       /* DPV1_Enable bit must not be set */
    {
        /* we don't check for reserved bits because of the certification process */
        /* we check only for invalid bits                                       */
#ifdef C1_ALARMS
        if((DPV1Status[0] & 0x20) || (DPV1Status[1] & 0x2)
                || (DPV1Status[2] & 0xf8))
#else
        if((DPV1Status[0] & 0x20) || (DPV1Status[1] & 0xff)
                || (DPV1Status[2] & 0xff))
#endif
        {
            return BAD_PARAMETER;
        }

        else if((!(DPV1Status[0] & 0x80))
                && ((DPV1Status[1] & 0xff) || (DPV1Status[2] & 0xff)))
        {
            return BAD_PARAMETER;
        }

        else
        {
            return (CONFIG_SUCCESS);
        }
    }

    return (BAD_PARAMETER);
}


/****************************************************************************
*
*      name          : check_cfg_data
*      parameter     : cfg_data - pointer to configuration data received
*                      cfg_len - length of configuration data received
*      return value  : CONFIG_SUCCESS, BAD_CONFIGURATION, UPDATE_CONFIGURATION
*      description   : checks the configuration data received
*
****************************************************************************/

byte check_cfg_data(byte XDATA *cfg_data, byte cfg_len)
{
    int i;
    byte Result = UPDATE_CONFIGURATION;

    /* check configuration 1. Must be the same length and data */
    if(cfg_len != sizeof(myCfg1))
    {
        Result = BAD_CONFIGURATION;
    }

    else
        for(i = 0; i < sizeof(myCfg1); i++)
            if(cfg_data[i] != myCfg1[i])
            {
                Result = BAD_CONFIGURATION;
            }

    return (Result);
}


/****************************************************************************
*
*      name          : dp_global_control
*      parameter     : command global control command which has been received
*      return value  : --
*      description   : This function shall store the data and must call
*                                init_dp again, but is not supported in this example.
*                      The DP software is configured not to support
*                                set_slave_add (see init_dp(4,0) in main)
*
****************************************************************************/
void dp_global_control(byte command)
{
    if(command & 2)
    {
        /* dp is in state CLEAR, set outputs to safe outputs */
        bIsInDataExchange = 0;
    }

    else
    {
        /* dp connection is in state DATA_EXCHANGE, use outputs */
        bIsInDataExchange = 1;
    }

}
/****************************************************************************
*
*      name          : set_slave_add
*      parameter     : buffer - received data from master
*                      buffer[0] = new_slave_add
*                      buffer[1..2] = ident_number
*                      buffer[3] = no_add_chg
*                      buffer[4-n] = rem_slave_data
*                      rem_slave_bufferlen - length of rem_slave_data
*      return value  : --
*      description   : This function shall store the data and must call
*                                init_dp again, but is not supported in this example.
*                      The DP software is configured not to support
*                                set_slave_add (see init_dp(4,0) in main)
*
****************************************************************************/

void set_slave_add(byte XDATA *buffer, byte rem_slave_data_len)
{
}

/****************************************************************************
*
*      name          : dpe_initiate_abort
*      parameter     : p - buffer with initiate_ind frame
*                      instance_reason - instance and reason code
*      return value  : --
*      description   : This function is called each time an initiate_ind
*                      has to be aborted.
*
****************************************************************************/
void dpe_initiate_abort(TBufPtr p, byte instance_reason)
{
    byte UsedSap;
    UsedSap = p->osap;
    put_sap_puffer(UsedSap, p);   /* free buffer              */
    sys_abort_req(UsedSap, 0, instance_reason);   /* instance_reason for abort */
}

/****************************************************************************
*
*      name          : dpe_initiate_ind
*      parameter     : p - buffer with initiate_ind frame
*      return value  : --
*      description   : This function is called each time an initiate_ind
*                      has been received.
*
****************************************************************************/
typedef struct
{
    byte FC;                    /* must be 0x57 */
    byte Reserved[3];           /* must be 0 */
    byte SendTimeout[2];
    byte FeaturesSupported[2];  /* must be 0x01,0x00 */
    byte ProfileFeaturesSupported[2];
    byte ProfileIdentNumber[2]; /* must be 2 */
    byte AddAddrParam[200];     /* Rest of frame */
} TInitiateReq;
typedef struct
{
    byte FC;                    /* must be 0x57 */
    byte MaxLenDataUnit;
    byte FeaturesSupported[2];  /* must be 0x01,0x00 */
    byte ProfileFeaturesSupported[2];
    byte ProfileIdentNumber[2]; /* must be 0x40 */
    byte AddAddrParam[200];     /* Rest of frame */
} TInitiateRes;
void dpe_initiate_ind(TBufPtr p)
{
    byte i;
    /* cast pointer for convenience*/
    byte slen;
    TInitiateReq XDATA *PI = (TInitiateReq XDATA *) & p->lli_pdu[0];
    TInitiateRes XDATA *PR = (TInitiateRes XDATA *) PI;
    byte Slen = p->lli_pdu[13];
    byte Dlen = p->lli_pdu[15];
    byte DApi = p->lli_pdu[16 + Slen];
    byte DScl = p->lli_pdu[16 + Slen];

    if((Dlen < 2) || (Slen < 2))
    {
        dpe_initiate_abort(p, DDLM_ABT_IA);
        return;
    }

    /* check the data slen and dlen*/
    if(Slen + Dlen + 16 != p->laenge)
    {
        dpe_initiate_abort(p, DDLM_ABT_IA);
        return;
    }

    if(DApi != 0 || DScl != 0)
    {
        dpe_initiate_abort(p, DDLM_ABT_IA);
        return;
    }

    /* the next parameters are just copied from the receiving frame*/
    PR->FeaturesSupported[0] = PI->FeaturesSupported[0];
    PR->FeaturesSupported[1] = PI->FeaturesSupported[1];
    PR->ProfileFeaturesSupported[0] = PI->ProfileFeaturesSupported[0];
    PR->ProfileFeaturesSupported[1] = PI->ProfileFeaturesSupported[1];
    PR->MaxLenDataUnit = Max_Len_Data_Unit;     /* maximum length for data*/
    PR->ProfileIdentNumber[0] = 0;
    PR->ProfileIdentNumber[1] = 0;

    PR->AddAddrParam[2] = PI->AddAddrParam[0];  /* D-Type*/
    PR->AddAddrParam[3] = PI->AddAddrParam[1];  /* D-Len*/
    PR->AddAddrParam[0] = 0;    /* S-Type*/
    PR->AddAddrParam[1] = 2;    /* S-Len*/
    PR->AddAddrParam[4] = 0;    /* S-Addr/API*/
    slen = PI->AddAddrParam[1];
    PR->AddAddrParam[5] = PI->AddAddrParam[5 + slen];   /* S-Addr/SCL*/

    for(i = 0; i < slen; i++)
    {
        PR->AddAddrParam[6 + i] = PI->AddAddrParam[4 + i];    /* D-ADDR*/
    }

    /* Initialise I&M Parameters for this connection */
    InitiateAREPSingle(p->osap);

    p->laenge -= 4;             /* correct length*/
    sys_response(p);
}


/****************************************************************************
*
*      name          : dpe_abort_ind
*      parameter     : p - buffer with abort_ind frame
*      return value  : --
*      description   : This function is called each time an abort_ind
*                      has been received.
*
****************************************************************************/
void dpe_abort_ind(byte pa, byte res_sap, byte subnet,
                   byte instance_reason, byte locally)
{
    if(pa);

    if(subnet);

    if(instance_reason);

    if(locally);

    /* reset I&M Parameters for this connection */
    InitiateAREPSingle(res_sap);
}


/****************************************************************************
*
*      name          : dpe_read_ind
*      parameter     : p - buffer with read_ind frame
*      return value  : --
*      description   : This function is called each time an read_ind
*                      has been received.
*
****************************************************************************/
void dpe_read_ind(TBufPtr p)
{
    byte slot;
    byte index;

    slot = p->lli_pdu[1];
    index = p->lli_pdu[2];

    if(index == 255)
    {
        /* I&M functions */
        byte result = IAndMdpe_read_ind(p->osap, p);

        if(result != DM_SUCCESS)
        {
            make_dpe_error(result, p);
        }
    }

    else if(slot == 1)
    {
        int i;
        int length;
        length = p->lli_pdu[3];

        if(length > Max_Len_Data_Unit - 6)
        {
            length = Max_Len_Data_Unit - 6;     /*  no header, no SAPs */
        }

        if(index == 1)          /* copy from test buffer */
        {
            for(i = 0; i < length; ++i)
            {
                p->lli_pdu[4 + i] = dpv1_test_buffer[i];
            }
        }

        else
        {
            for(i = 0; i < length; ++i)
            {
                p->lli_pdu[4 + i] = (0xF0 + i);
            }
        }

        p->laenge += length;
        p->lli_pdu[3] = length;
    }

    else                        /* slot = 0 */
    {

        /*--- Res (-) ----------*/
        make_dpe_error(0xB2,
                       p);       /* Error_Class=11 :access | Error_Code=2: invalid slot */
    }

    sys_response(p);
}



/****************************************************************************
*
*      name          : dpe_write_ind
*      parameter     : p - buffer with write_ind frame
*      return value  : --
*      description   : This function is called each time an write_ind
*                      has been received.
*
****************************************************************************/
void dpe_write_ind(TBufPtr p)
{
    byte slot;
    byte index;

    slot = p->lli_pdu[1];
    index = p->lli_pdu[2];

    if(index == 255)
    {
        byte result;

        if(slot == 0)
            /* I&M functions */
        {
            result = IAndMdpe_write_ind(p->osap, p);
        }

        else
        {
            result = DM_INVALID_SLOT;
        }

        if(result != DM_SUCCESS)
        {
            make_dpe_error(result, p);
        }

        else
        {
            p->laenge = 4;
        }
    }

    else
    {
        int i;
        byte length;

        if(slot == 1)           /* Res (+) */
        {
            p->laenge = 4;

            if(index == 1)      /* copy to test buffer */
            {
                length = p->lli_pdu[3];

                for(i = 0; i < length && i < Max_Len_Data_Unit; ++i)
                {
                    dpv1_test_buffer[i] = p->lli_pdu[4 + i];
                }
            }
        }

        else                    /* slot = 0 */
        {

            make_dpe_error(0xB2,
                           p);   /* Error_Class=11 :access | Error_Code=2: invalid slot */
        }
    }

    sys_response(p);
}

/****************************************************************************
*
*      name          : msac1_read_ind
*      parameter     : p - buffer with read_ind frame
*      return value  : --
*      description   : This function is called each time an msac1_read_ind
*                      has been received.
*
****************************************************************************/
void msac1_read_ind(TBufPtr p)
{
    int i;
    byte length;
    byte slot;
    byte index;

    slot = p->lli_pdu[1];
    index = p->lli_pdu[2];

    if(index == 255)
    {
        byte result;

        if(slot == 0)
            /* I&M functions */
        {
            result = IAndMdpe_read_ind(p->osap, p);
        }

        else
        {
            result = DM_INVALID_SLOT;
        }

        if(result != DM_SUCCESS)
        {
            make_dpe_error(result, p);
        }
    }

    else if(slot == 1)
    {
        length = p->lli_pdu[3];

        if(length > 63)
        {
            length = 63 - 4;
        }

        /* fill buffer */
        if(index == 1)          /* copy from test buffer */
        {
            for(i = 0; i < length && i < Max_Len_Data_Unit; ++i)
            {
                p->lli_pdu[4 + i] = dpv1_test_buffer[i];
            }
        }

        else
        {
            for(i = 0; i < length && i < Max_Len_Data_Unit; ++i)
            {
                p->lli_pdu[4 + i] = (1 + i);
            }
        }

        p->laenge += i;

    }

    else                        /* slot = 0 */
    {

        /*--- Res (-) ----------*/
        make_dpe_error(0xB2,
                       p);       /* Error_Class=11 :access | Error_Code=2: invalid slot */
    }

    sys_c1_response(p);

}

/****************************************************************************
*
*      name          : msac1_write_ind
*      parameter     : p - buffer with write_ind frame
*      return value  : --
*      description   : This function is called each time an msac1_write_ind
*                      has been received.
*
****************************************************************************/
void msac1_write_ind(TBufPtr p)
{
    int i;
    byte slot;
    byte index;
    byte length;
    slot = p->lli_pdu[1];
    index = p->lli_pdu[2];

    if(index == 255)
    {
        /* I&M functions */
        byte result;

        if(slot == 0)
        {
            result = IAndMdpe_write_ind(p->osap, p);
        }

        else
        {
            result = DM_INVALID_SLOT;
        }

        if(result != DM_SUCCESS)
        {
            make_dpe_error(result, p);
        }

        else
        {
            p->laenge = 4;
        }
    }

    else
    {
        if(slot == 1)
        {
            p->laenge = 4;

            if(index == 1)      /* copy to test buffer */
            {
                length = p->lli_pdu[3];

                for(i = 0; i < length && i < Max_Len_Data_Unit; ++i)
                {
                    dpv1_test_buffer[i] = p->lli_pdu[4 + i];
                }
            }
        }

        else                    /* slot = 0 */
        {

            /*--- Res (-) ----------*/
            make_dpe_error(0xB2,
                           p);   /* Error_Class=11 :access | Error_Code=2: invalid slot */
        }
    }

    sys_c1_response(p);

}

/****************************************************************************
*
*      name          : msac1_alarm_ack_ind
*      parameter     : p - buffer with write_ind frame
*      return value  : --
*      description   : This function is called each time an msac1_alarm_ack_ind
*                      has been received.
*
****************************************************************************/
void msac1_alarm_ack_ind(TBufPtr p)
{
    byte alarm_type;
    byte slot_number;
    byte seq_nr;

    slot_number = p->lli_pdu[1];
    alarm_type = p->lli_pdu[2];
    seq_nr = p->lli_pdu[3];


    /* check MSAL1 alarm conditions don't change !!! */
    if(check_msac1_alarm_ack_ind(slot_number, alarm_type, seq_nr))
    {
        sys_c1_response(p);
    }

    else
    {
        /* IGNORE CASE */
        /* there is no negative confirmation, free buffer with : put_sap_puffer (p->osap, p) */
        put_sap_puffer(p->osap, p);     /* free buffer */
        /*

           use always a WD, otherwise the master will go on polling !!!

         */

        /*** Chk DAR in order to deactivte the alarm SAP */
        c1_stop_and_abort(DPV1_CONNECTIONS + p->osap - C1_ALARM_SAP, p);

        /* --- free buffer and deactivte the alarm SAP --- */
        /* msac1s_stop_req (); *//* deactivate the alarm SAP */

    }

}

/*F*************************************************************************/
/**                                                                       **/
/**     Name          : msal1s_abort_ind                                  **/
/**     Parameter     : -                                                 **/
/**     Return Value  : -                                                 **/
/**     Description   : This function indicates the abort of the  MSAL1S  **/
/**                     state machine                                     **/
/**                                                                       **/
/***************************************************************************/
void msal1s_abort_ind(void)
{
}


/****************************************************************************
*
*      name          : dpe_datatransport_ind
*      parameter     : p - buffer with write_ind frame
*      return value  : --
*      description   : This function is called each time an data_transport_ind
*                      has been received.
*
****************************************************************************/
void dpe_datatransport_ind(TBufPtr p)
{
    int i;
    byte slot;
    byte index;
    byte length;
    byte help;

    slot = p->lli_pdu[1];
    index = p->lli_pdu[2];

    if(slot != 1)
    {
        /*--- Res (-) ----------*/
        make_dpe_error(0xB2,
                       p);       /* Error_Class=11 :access | Error_Code=2: invalid slot */
    }

    else
    {
        if(index == 1)          /* copy from and to test buffer */
        {
            length = p->lli_pdu[3];

            for(i = 0; i < length && i < Max_Len_Data_Unit; ++i)
            {
                help = p->lli_pdu[4 + i];       /* interim buffer for received data */
                p->lli_pdu[4 + i] = dpv1_test_buffer[i];
                dpv1_test_buffer[i] = help;     /* copy from interim buffer for next cycle */
            }
        }

        else
        {
            for(i = 0; i < p->laenge; ++i)
            {
                p->lli_pdu[4 + i] = (1 + i);
            }
        }

    }

    sys_response(p);
}

/****************************************************************************
*
*      name          : user_dp_init_var
*      parameter     : --
*      return value  : --
*      description   : handles the initialization of the DP-variables
*
****************************************************************************/

void user_dp_init_var(void)
{
    /* Initialization of the DP-length-variables  */
    /* initialization of the stack variables      */

    max_cfg_len = sizeof(
                      myCfg1);           /* maximum length of Cfg_Data                   */
    max_cfg_and_diag_len =
        MAX_CFG_DIAG_LEN;    /* maximum length of Cfg_Data or Diag_Data      */
    max_user_prm_len = 70;      /* extensions of User_Prm_Data = 3 for DPV1_C1  */
    max_rem_data_len = 0;       /* maximum length of Rem_Slave_Data = 0         */
    max_dp_inp_len = INPUT_LEN; /* maximum length of Inp_Data                   */
    max_dp_outp_len =
        OUTPUT_LEN;       /* maximum length of Outp_Data                  */

    Min_Send_Timeout = 100;     /* minimum connection monitoring time           */
    Max_Len_Data_Unit = 239;    /* max. length of Acyclic data transfer         */
}


/****************************************************************************
*
*      name          : user_init_dpc1_vars
*      parameter     : --
*      return value  : --
*      description   :  initialization of global DPV1C1-variables
*
****************************************************************************/
void user_init_dpc1_vars(void)
{
    byte len;

    length_list = min(63, Max_Len_Data_Unit);

    DiagBlock.status_pdu_len = 0;       /* no status PDU */
    DiagBlock.ext_diag_buffer_len = 0;  /* no ext diag */
    DiagBlock.alarm_buffer = alarm_buffer;
    DiagBlock.ext_diag_buffer = local_diag_buffer;
    DiagBlock.status_buffer = status_buffer;
    DiagBlock.alarm_pdu_len = 0;        /* no alarm */
    alarm_ptr = (ALARM_DIAG XDATA *) DiagBlock.alarm_buffer;

    /* see techn. guideline page 122 ff. */
    /* ------ no alarm -------------------- */
    DiagBlock.alarm_buffer[0] = 0;
    DiagBlock.alarm_buffer[1] = 0;
    DiagBlock.alarm_buffer[2] = 0;
    DiagBlock.alarm_buffer[3] = 0;

    DiagBlock.alarm_pdu_len = alarm_ptr->headerbyte;

    len =
        DiagBlock.ext_diag_buffer_len + DiagBlock.status_pdu_len +
        DiagBlock.alarm_pdu_len;
    update_diag_data(len, 0);   /* no EXT_DIAG: no alarms */

}

/****************************************************************************
*
*      name          : user_dp_init
*      parameter     : --
*      return value  : --
*      description   : handles the initialization of DP
*
****************************************************************************/

void user_dp_init(void)
{
    int i;


    /* get buffer for configuration */
    config_buffer = get_cfg_buffer();

    for(i = 0; i < sizeof(myCfg1); i++)
    {
        config_buffer[i] = myCfg1[i];
    }

    ident_number[0] = IDENT_NR_H;
    ident_number[1] = IDENT_NR_L;

    /* init DP with cfg_len = CFG_LEN, Add_Change_Disabled = FALSE,
       SYNC_NOT_SUPPORTED = FALSE, FREEZE_NOT_SUPPORTED = FALSE */
    init_dp(sizeof(myCfg1), ADD_CHANGE_DISABLED, ident_number);


    /* get buffer for first inputs */
    input_buffer = get_input_buffer();

    /* get buffer for first diagnosis  */
    ext_diag_buffer = get_ext_diag_buffer();
}


/****************************************************************************
*
*      name          : dp_main
*      parameter     : --
*      return value  : --
*      description   : handles the main routine of DP
*
****************************************************************************/
void dp_main(void)
{
    byte i;
    byte bChange = 0;
    /* get new output buffer */
    output_buffer = update_outputs();

    profi_soc_set_digoutput(output_buffer[3]);
    
    /* COPY_OUTPUTS_FROM_OUTPUT_BUFFER */
    for(i = 0; i < OUTPUT_LEN; i++)
    {
        if(dp_outputs[i] != output_buffer[i])
        {
            bChange = 1;
            dp_outputs[i] = output_buffer[i];
        }
    }

#if (SW_HEXDUMP == 2)

    if(bChange)
    {
        bChange = 0;
        UART_printf("\n\rOutputs changed ");

        hexDumpInDbgUart(&dp_outputs[0],
                         OUTPUT_LEN);
    }

#endif


    /* COPY_INPUTS_IN_INPUT_BUFFER */
#if INPUT_LEN > 0

    for(i = 0; i < INPUT_LEN; i++)
    {
        input_buffer[i] = dp_inputs[i];
    }

#endif
    /* deliver new inputs to the communication software */
    input_buffer = update_inputs();

    do_dp();

    /* dp_inputs = dp_outputs */
    do_dpe();

    if(g_pL2GlobVar->dp_status & STATUS_CLEAR)
    {
        set_dp_status_and(STATUS_NO_CLEAR);
    }

    if(g_pL2GlobVar->dp_status & STATUS_LEAVE)
    {
        /* Initialise I&M Parameters for this connection */
        InitiateAREPSingle(51);

        set_dp_status_and(STATUS_NO_LEAVE);
    }

    if(g_pL2GlobVar->dp_status & STATUS_FREEZE)
    {
        /* slave is in Freeze state */
    }

    if(g_pL2GlobVar->dp_status & STATUS_SYNC)
    {
        /* slave is in sync state */
    }

#ifdef C1_ALARMS

    if(bSendAlarm)
    {
        send_alarm();
    }

#endif

    switch(bUpdateDiagData)
    {
        case 0:
            break;

        case 1:
            update_diag_data(0, 0);
            bUpdateDiagData = 0;
            break;

        case 2:
            ext_diag_buffer[0] = 0x05;
            ext_diag_buffer[1] = 0x82;
            ext_diag_buffer[2] = 0x00;
            ext_diag_buffer[3] = 0x00;
            ext_diag_buffer[4] = 0x01;
            update_diag_data(5, EXT_DIAG);
            bUpdateDiagData = 0;
            break;

        case 3:
            update_diag_data(0, STAT_DIAG);
            bUpdateDiagData = 0;
            break;
    }

}

#ifdef C1_ALARMS
/****************************************************************************
*
*      name          : send_alarm
*      parameter     : --
*      return value  : --
*      description   : exanpmle for an alarm- and status-request, and
*                      additional external diagnosis
*
****************************************************************************/

void send_alarm(void)
{
    static byte SeqNrCnt = 0;

    if(rcode <= NO_START)       /* always start with SeqNr = 0 */
    {
        SeqNrCnt = 0;
    }

    if(alarm_sequence == FALSE)
    {
        SeqNrCnt = 0;
    }


    /*----------- create Status-PDU -------------*/
    if(SeqNrCnt == 0)           /* order of status apearance can be changed here */
    {
        status_buffer[0] = 4;   /* Block-laenge */
        status_buffer[1] = 0x81;        /* status-type : Bit7 = 1, Status-Message */
        status_buffer[2] = 1;   /* slot */
        status_buffer[3] = 1;   /* status aufgetreten */
    }

    /*----------- set ALARM --------------------------------------------------------*/
    if(DPV1_Status_2)
    {
        rcode = msal1_alarm_req(126,    /* Alarm_Type = 126: manufacture specific */
                                1,      /* Slot_Number */
                                SeqNrCnt,       /* Seq_Nr */
                                0,      /* Alarm_Specifier */
                                FALSE,  /* Add_Ack = FALSE */
                                (byte XDATA *) 0);      /* no Diagnostic_User_Data */

        if(rcode == ALARM_PENDING)
        {
            bSendAlarm = 0;
        }

        SeqNrCnt = (SeqNrCnt - 1) & 0x1f;
    }

    else
    {
        bSendAlarm = 0;
    }
}
#endif





#if (SW_HEXDUMP > 0)


char HexdumpBuffer[2048];

/* @brief Hexdump

 dumps data in hex format with prefix linewise

 @param[out]   sOut        buffer to dump into
 @param[in]    sIntro      string as preprint
 @param[in]    nDisplay    length of byte in one line
 @param[in]    p           pointer to data to be displayed
 @param[in]    n           length of data to be displayed
*/

void Hexdump(char *sOut, const char *sIntro, int nDisplay,
             const unsigned char *p, int n)
{
    int i;

    if(NULL == sOut)
    {
        return;
    }

    if((NULL == sIntro) || (NULL == p))
    {
        sprintf(sOut, "*** illegal hexdump ***");
        return;
    }

    sOut += sprintf(sOut, "%s", sIntro);

    for(i = 0; i < n; ++i)
    {
        if((nDisplay != 0) && (i != 0))
            if((i % nDisplay) == 0)
            {
                sOut += sprintf(sOut, "\n");
            }

        sOut += sprintf(sOut, "%02X ", *p++);
    }
}
#endif

/****************************************************************************
 *  name        : PrintMenue
 *  description : UART keydriven menu for changing input, output, and
 *                 checking DP status
 *
 ***************************************************************************/
void PrintMenue(void)
{
    UART_printf("\n\r---------------\n\rI - Change inputs\n\rS - Send static diagnosis");
    UART_printf("\n\rC - Clear diagnosis\n\rE - Send ext. diagnosis\n\rD - DP Status");

#ifdef C1_ALARMS
    /*UART_printf("\n\rA - Send alarm\n\r1 - Alarm state");*/
#endif
    UART_printf("\n\rP - Print Menu");
}

/****************************************************************************
 *  name        : printInHex
 *  description : prints the char in hex notation
 *
 *
 ***************************************************************************/
void printInHex(unsigned char val)
{
    unsigned char tChar = val % 16;

    if(tChar > 9)
    {
        tChar += ('0' + 7);
    }

    else
    {
        tChar += '0';
    }

    if(val)
    {
        val /= 16;
        printInHex(val);
        UART_putc(tChar);
    }
}

/****************************************************************************
 *  name        : hexDumpInDbgUart
 *  description : prints char and hex values
 *
 *
 ***************************************************************************/
void hexDumpInDbgUart(unsigned char *tStr, int n)
{
    int itr = 0;

    for(itr = 0 ; itr < n ; itr++)
    {
        if(tStr[itr] == 0)
        {
            UART_printf("0");
        }

        else
        {
            printInHex(tStr[itr]);
        }

        UART_printf(" ");
    }
}

/****************************************************************************
 *  name        : keyboard
 *  description : UART keydriven menu for changing input, output, and
 *                 checking DP status
 *
 ***************************************************************************/
static void keyboard(unsigned char rxData)
{
#if INPUT_LEN > 0
    int i;
#endif

    switch(rxData)
    {
        case 'p':
        case 'P':
            PrintMenue();
            break;
#if INPUT_LEN > 0

        case 'i':
        case 'I':
            UART_printf("\n\ri");

            for(i = 0; i < INPUT_LEN; i++)
            {
                dp_inputs[i]++;
            }

            break;
#endif

        case 's':
        case 'S':
            UART_printf("\n\rs");
            bUpdateDiagData = 3;
            break;

        case 'c':
        case 'C':
            UART_printf("\n\rc");
            bUpdateDiagData = 1;
            break;

        case 'e':
        case 'E':
            UART_printf("\n\re");
            bUpdateDiagData = 2;
            break;

        case 'd':
        case 'D':
            UART_printf("\n\rDP Status: ");
            hexDumpInDbgUart((unsigned char *)&g_pL2GlobVar->dp_status, 1);
            break;
#ifdef C1_ALARMS

        case 'a':
        case 'A':
            UART_printf("\n\ra");
            bSendAlarm = 1;
            break;

        case '1':
            UART_printf("\n\rAlarm States: ");
            UART_printf("\n\rdx_entered: ");
            profi_uart_itoa(dx_entered);
            UART_printf("\n\rmsal1s_alarm_state: ");
            profi_uart_itoa(msal1s_alarm_state);
            UART_printf("\n\ralarm_sequence: ");
            profi_uart_itoa(alarm_sequence);
            break;
            /*C1_ALARMS*/
#endif

        case 27:
        case 'q':
        case 'Q':
            UART_printf("\n\rQuit Commanded");
            break;
    }

    if(ErrorOccured)
    {
        UART_printf("\n\rUserError occured: ");
        profi_uart_itoa(ErrorOccured);
    }
}
/****************************************************************************
*   name        : Uarttask
*  description : UART keydriven menu for changing input, output, and
*                  checking DP status
*
***************************************************************************/
Void Uarttask(UArg a0, UArg a1)
{
    do
    {
        UART_scanFmt("%c\n", &rxData);
        uartPresent = 1;
        keyboard(rxData);

    }
    while(1);
}

#ifdef ENABLE_WATCHDOG_SUPPORT
Void Watchdogtask(UArg a0, UArg a1)
{
    profi_soc_watchdog_init(WATCHDOG_EXPIRE_TIME);
    while(1)
    {
       profi_soc_watchdog_reset(WATCHDOG_EXPIRE_TIME);
       Task_sleep((WATCHDOG_EXPIRE_TIME / 10) - (((WATCHDOG_EXPIRE_TIME / 10) / 2)));
    }
}
#endif
/****************************************************************************
*
*                       main routine of the profibus stack
*
****************************************************************************/
THREAD_RETURN user(THREAD_ARGS)
{
#if (MULTI_THREAD == 1)
    THREAD_INIT;
#endif

    /*------------ INITIALIZING  ------------------------------------------*/

    user_dp_init_var();         /* init DP-variables        */
    pb_reset((byte)(slave_addr & 0xff));
    user_dp_init();             /* init user DP variables   */

    pb_init_dpe();              /* init DPV1 layer          */
    InitiateAREP();             /* init I&M channels        */

    user_init_dpc1_vars();      /* init variables for MSC1  */
    pb_init_dpc1();             /* init msac1 layer         */

    mscy1s_init(FALSE);         /* initialize mscy1s        */

    rcode = msal1s_init(2,      /* Alarm_Mode_Slave : 4 alarms in total */
                        FALSE);  /* Default_Ext_Diag, */

    if(rcode)
    {
        user_error(rcode);
    }

    /*------------ MAIN LOOP START -----------------------------------------*/

    while(1)
    {
        int jmpret;

        if(ErrorOccured == 0)
        {
            jmpret = setjmp(myJumpBuf);

            if(jmpret == 0)
            {
                dp_main();      /* call user function to handle all dp mechanism  */
                TRIGGERWATCHDOG;        /* just in case you need a watchdog               */
            }
        }

        msleep(1);              /* sleep to allow other threads to run            */
    }

    /*------------ MAIN LOOP END -------------------------------------------*/
#if (MULTI_THREAD == 1)
    THREAD_EXIT;
#endif

    return 0;
}

/*     End functions
**
****************************************************************************/

/*
 *  ======== taskFxn and TaskHandle ========
 */

Void Profibustask(UArg a0, UArg a1);
Void Timertask(UArg a0, UArg a1);
Void Uarttask(UArg a0, UArg a1);

Task_Handle timerTask;
Task_Handle profibusTask;
Task_Handle uartTask;
Task_Handle watchdogTask;

/****************************************************************************
*  name        : Profibustask
*  description : UART keydriven menu for changing input, output, and
*                 checking DP status
*
***************************************************************************/
Void Profibustask(UArg a0, UArg a1)
{
    UART_printf("\n Press any key to continue\n");

    /*Wait for the UART data input from the user for 10 sec*/
    do
    {
        Task_sleep(1);
    }
    while((TimerCount < 1000) && (uartPresent == 0));

    if(uartPresent)
    {

        do
        {
            UART_printf("\n Enter the slave address between 1-125\n");
            UART_scanFmt("%s\n", rxBuffer);
            slave_addr = atoi((char *)rxBuffer);
        }
        while((slave_addr <= 0) || (slave_addr > 125));
    }

    else
    {
        slave_addr = 4;
    }

    /* Initializations for the application */
    profi_init_soc();

    /* Print Menu on UART console driven by keyboard keys */
    PrintMenue();


    /* Enter into the profibus stack */
    BSP_main(0, NULL);
}

/****************************************************************************
 *  name        : Timertask
 *  description : gives 5ms tick using Task_sleep(). Comment this if the DMTimer3
 *                is used for timertick .
 *
 ***************************************************************************/
Void Timertask(UArg a0, UArg a1)
{
    while(1)
    {
        Task_sleep(5);
        TimerCount++;
        profi_timer_isr();
    }
}

/****************************************************************************
 *  name        : main
 *  description : main function of the profibus slave application.
 *                creates tasks, mmu initialization, cache locking of NOR sector,
 *                setting PINUMX and UART console depending on board type,
 *                ICSS initialization, led application initialization,
 *                SPI initialization, and starting BIOS
 *
 ***************************************************************************/
int main()
{

    Board_IDInfo boardInfo;
    Task_Params taskParams;

    Board_init(BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_UART_STDIO);

    Board_getIDInfo(&boardInfo);

    UART_printf("\nVersion - ");
    UART_printf(IND_PKG_VERSION);


    UART_printf("\nProfibus slave Sample application running on ");


    UART_printf("\nBoard name \t: ");
    UART_printf(boardInfo.boardName);

    UART_printf("\n\rChip Revision \t: ");
    UART_printf(boardInfo.version);
    UART_printf("\r\nBuild Timestamp      : %s %s", __DATE__, __TIME__);


    pruIcssHandle = PRUICSS_create(pruss_config, PRUICSS_INSTANCE);

    PRUICSS_pruDisable(pruIcssHandle, 0);
    PRUICSS_pruDisable(pruIcssHandle, 1);

    profi_soc_hw_init();

    Task_Params_init(&taskParams);
    taskParams.priority = 10;
    taskParams.stackSize = 0x300;
    timerTask = Task_create(Timertask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 2;
    taskParams.stackSize = 0x800;
    profibusTask = Task_create(Profibustask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    taskParams.stackSize = 0x400;
    uartTask = Task_create(Uarttask, &taskParams, NULL);
#ifdef ENABLE_WATCHDOG_SUPPORT
    Task_Params_init(&taskParams);
    taskParams.priority = 2;
    taskParams.stackSize = 0x400;
    watchdogTask = Task_create(Watchdogtask, &taskParams, NULL);
#endif
    BIOS_start();     /* enable interrupts and start SYS/BIOS */
    return 0;
}


