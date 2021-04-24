/**
 * \file device.c
 * \brief Contains Ethernetip Adapter device specific functions
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
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include "eip_main.h"
#include <examples/board/include/board_i2cLed.h>
#include <third_party/protocols/ethernetip_adapter/include/eip_device.h>
#include <third_party/protocols/ethernetip_adapter/include/opt_api.h>
#include <protocols/ethernetip_adapter/include/icss_dlr.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/**Responce for Get Attribute All of User Obj*/
EIP_BYTE Response[] = {0x81, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

extern dlrStruct dlrObj;
extern EIP_BOOL DLRbInit();


/**Shared Memory for IN Data*/
EIP_BYTE DataIn[IN_DATA_SIZE];
/**Shared Memory for OUT Data*/
EIP_BYTE DataOut[OUT_DATA_SIZE];
/**Control Table for Connection*/
EIP_BYTE CtrlTable[EIP_NB_IO_CNXN_MAX / 8 + 1];
/**Health Table for Connection*/
EIP_BYTE HealthTable[EIP_NB_IO_CNXN_MAX / 8 + 1];
/**IN Status*/
EIP_SHORT StatusIn[EIP_NB_IO_CNXN_MAX + 1];
/**Out Status*/
EIP_SHORT StatusOut[EIP_NB_IO_CNXN_MAX + 1];
/**IN Updated*/
EIP_BYTE UpdateIn[EIP_NB_IO_CNXN_MAX + 1];
/**Out Updated*/
EIP_BYTE UpdateOut[EIP_NB_IO_CNXN_MAX + 1];
/**Handshake byte for data exchange between Application and stack*/
EIP_BYTE Handshake[2];

/**Max offset for OUtData*/
EIP_WORD  wMaxOffsetOut = 0;

/**Flag for new OUT data to copy into the stack*/
EIP_BOOL gbNewOutData  = EIP_FALSE;
/** Flag to copy data In into data OUT*/
EIP_BOOL gbCopyINtoOUT = EIP_FALSE;
/** Fag to increment OUT automatically*/
EIP_BOOL gbDataInc     = EIP_FALSE;

/**IO Data Connection Parameters*/
ST_CNXN_PARAMS  stCnxnParams[EIP_NB_IO_CNXN_MAX];
/**
 *  @brief  Get Attribute call for the User object
 *
 *  @param  wTransacID   [in]
 *  @param  pstEIP_EmReq [in]
 *  @param  pstEIP_EmRes [in] Response
 *
 *  @retval  EIP_TRUE - On success
 *
 */
EIP_BOOL EIPDEV_userOBJGetAttributeAll(EIP_WORD wTransacID,
                                       LP_ST_EIP_EM_REQ pstEIP_EmReq, LP_ST_EIP_EM_RES pstEIP_EmRes);


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/**
 *  @brief  The function returns the Vendor ID
 *
 *  @param  none
 *
 *  @retval  Vendor ID
 *
 */
EIP_WORD EIPDEV_getVendorId()
{
    return CIP_VENDOR_ID;
}
/**
 *  @brief  The function returns the Device Type
 *
 *  @param  none
 *
 *  @retval  Device Type
 *
 */
EIP_WORD EIPDEV_getDeviceType()
{
    return CIP_DEVICE_TYPE;
}
/**
 *  @brief  The function returns the Product Code
 *
 *  @param  none
 *
 *  @retval  Product Code
 *
 */
EIP_WORD EIPDEV_getProductCode()
{
    return CIP_PRODUCT_CODE;
}
/**
 *  @brief  The function returns the Revision number Major
 *
 *  @param  none
 *
 *  @retval  Major revision number
 *
 */
EIP_BYTE EIPDEV_getMajorRevision()
{
    return REVISION_MAJOR;
}
/**
 *  @brief  The function returns the Revision number Minor
 *
 *  @param  none
 *
 *  @retval  Minor revision number
 *
 */
EIP_BYTE EIPDEV_getMinorRevision()
{
    return REVISION_MINOR;
}
/**
 *  @brief  The function returns the Serial number
 *
 *  @param  none
 *
 *  @retval  Serial number
 *
 */
EIP_DWORD EIPDEV_getSerialNum()
{
    return CIP_SERIAL_NUMBER;
}
/**
 *  @brief  The function returns the Product name
 *
 *  @param  pProductName [in] Product Name
 *
 *  @retval  Length of Product name
 *
 */
EIP_BYTE EIPDEV_getProductName(EIP_CHAR *pProductName)
{
    EIP_BYTE wLen;

    wLen = (EIP_BYTE) min(strlen(PRODUCT_NAME), MAX_PRODUCT_NAME_LENGTH);
    memcpy(pProductName, PRODUCT_NAME, wLen);
    return wLen;
}

/**
 *  @brief  The function initializes the  shared memory IO Data exchange
 *
 *  @param  none
 *
 *  @retval  Shared memory param structure
 *
 */
ST_SHM_PARAMS *EIPDEV_getSharedMemoryPtr()
{
    ST_SHM_PARAMS *pstSHMParams;

    pstSHMParams = malloc(sizeof(ST_SHM_PARAMS));

    if(pstSHMParams == EIP_NULL)
    {
        return EIP_NULL;
    }

    pstSHMParams->lpvDataIn       = DataIn;
    pstSHMParams->lpvDataOut      = DataOut;
    pstSHMParams->lpvCtrlTable    =
        EIP_NULL;
    pstSHMParams->lpvHealthTable  = HealthTable;
    pstSHMParams->lpvStatusIn     = StatusIn;
    pstSHMParams->lpvStatusOut    = StatusOut;
    pstSHMParams->lpvUpdateIn     = UpdateIn;
    pstSHMParams->lpvUpdateOut    = UpdateOut;
    pstSHMParams->lpbyHandshake[0] = &Handshake[0];
    pstSHMParams->lpbyHandshake[1] = &Handshake[1];

    return pstSHMParams;
}
/**
 *  @brief  Function sets the size and offset used for IN data
 *
 *  @param  wConnectionID [in] Connection id
 *  @param  wSize         [in] Connection size
 *  @param  wOffset       [in] Offset used for data exchange
 *
 *  @retval  none
 *
 */
EIP_VOID EIPDEV_sHMSetInParams(EIP_WORD wConnectionID, EIP_WORD wSize,
                               EIP_WORD wOffset)
{
    stCnxnParams[wConnectionID].wOffsetIn   = wOffset;
    stCnxnParams[wConnectionID].wSizeIn     = wSize;
}
/**
 *  @brief  Function sets the size and offset used for Out data
 *
 *  @param  wConnectionID [in] Connection id
 *  @param  wSize         [in] Connection size
 *  @param  wOffset       [in] Offset used for data exchange
 *
 *  @retval  none
 *
 */
EIP_VOID EIPDEV_sHMSetOutParams(EIP_WORD wConnectionID, EIP_WORD wSize,
                                EIP_WORD wOffset)
{
    stCnxnParams[wConnectionID].wOffsetOut   = wOffset;
    stCnxnParams[wConnectionID].wSizeOut     = wSize;

    if(stCnxnParams[wConnectionID].wOffsetOut +
            stCnxnParams[wConnectionID].wSizeOut > wMaxOffsetOut)
    {
        wMaxOffsetOut = stCnxnParams[wConnectionID].wOffsetOut +
                        stCnxnParams[wConnectionID].wSizeOut;
    }
}
/**
 *  @brief  Function is called to copy indata to outdata
 *
 *  @param  wCId [in] Connection id
 *
 *  @retval  none
 *
 */
EIP_VOID EIPDEV_copyINtoOUTData(EIP_WORD wCId)
{
    EIP_WORD wSize;
    EIP_BYTE led_status;
    wSize = min(stCnxnParams[wCId].wSizeIn, stCnxnParams[wCId].wSizeOut);

    memcpy(&DataOut[stCnxnParams[wCId].wOffsetOut],
           &DataIn[stCnxnParams[wCId].wOffsetIn], wSize);

    led_status = DataIn[stCnxnParams[1].wOffsetIn];

    Board_setDigOutput(led_status);

    /* Inform that Cid has changed*/
    UpdateOut[wCId + 1] = 1;

    /* If OUT areas is handled by USER toggle now else flag it*/
    if((USER_OUT) == (STACK_OUT))
    {
        gbNewOutData = EIP_FALSE;
        TOGGLE_USER_OUT;
    }

    else
    {
        gbNewOutData = EIP_TRUE;
    }

}
/**
 *  @brief  Function increments the OUT Data
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
EIP_VOID EIPDEV_incrementOUTData()
{
    int i;

    for(i = 0 ; i < wMaxOffsetOut; i++)
    {
        DataOut[i] = DataOut[i] + 1;
    }

    for(i = 0 ; i < EIP_NB_IO_CNXN_MAX; i++)
    {
        UpdateOut[i] = 1;
    }
}
/**
 *  @brief  This function sets the Device in Copyinout mode
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMCopyInOut(void)
{
    EIP_WORD i;

    gbCopyINtoOUT = !gbCopyINtoOUT;

    if(gbCopyINtoOUT)
    {
        if(gbDataInc)
        {
            gbDataInc = EIP_FALSE;
            UART_printf("\n\r EIP Data constant");
        }

        UART_printf("\n\r Copy In -> OUT");

        if((USER_OUT) == (STACK_OUT))
        {
            TOGGLE_USER_OUT;
        }

        /*Copy value intialized first*/
        for(i = 0; i < EIP_NB_IO_CNXN_MAX; i++)
        {
            if(UpdateIn[i] == 1)
            {
                EIPDEV_copyINtoOUTData(i);
            }
        }
    }

    else
    {
        UART_printf("\n\r Stop Copy In -> OUT");
    }
}

/**
 *  @brief  This function sets the Device in Incrementing mode
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMIncOut(void)
{
    gbDataInc = !gbDataInc;

    if(gbDataInc)
    {
        if(gbCopyINtoOUT)
        {
            gbCopyINtoOUT = EIP_FALSE;
            UART_printf("\n\r Stop Copy In -> OUT");
        }

        UART_printf("\n\r EIP Data incrementing");

        if((USER_OUT) == (STACK_OUT))
        {
            TOGGLE_USER_OUT;
        }
    }

    else
    {
        UART_printf("\n\r EIP Data constant");
    }
}
/**
 *  @brief  This function will be called by stack whenever there is an IN data
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMNewDataIn(void)
{

    EIP_WORD i;
#ifdef ENABLE_CONN_INFO_DISPLAY
    EIP_WORD j;
    char sTemp[256];
#endif


    /* Receive New Data */

    /* check which IO connection */
    for(i = 0; i < EIP_NB_IO_CNXN_MAX; i++)
    {
        if(UpdateIn[i] == 1)
        {
#ifdef ENABLE_CONN_INFO_DISPLAY
            CONSOLEUtilsPrintf("\n\r");
            sprintf(sTemp, " Cnxn #%u - Status (%d:%d) - Data: ", i, StatusIn[i],
                    StatusOut[i]);
            CONSOLEUtilsPrintf(sTemp);

            for(j = 0; j < stCnxnParams[i].wSizeIn; j++)
            {
                sprintf(sTemp, "%.2X ", DataIn[stCnxnParams[i].wOffsetIn + j]);
                CONSOLEUtilsPrintf(sTemp);
            }

#endif

            if(gbCopyINtoOUT)
            {
                EIPDEV_copyINtoOUTData(i - 1);
            }
        }
    }

    TOGGLE_USER_IN;
}

/**
 *  @brief  This function is an acknowledgment from Stack when OUT data is transmittted.
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMDataOutCopied(void)
{
    /* Check if new data need to be copied into the stack*/

    if(gbNewOutData)
    {
        gbNewOutData = EIP_FALSE;
        TOGGLE_USER_OUT;
    }

    if(gbDataInc)
    {
        EIPDEV_incrementOUTData();
        TOGGLE_USER_OUT;
    }
}
/**
 *  @brief  Initialize the User objects
 *
 *  @param  none
 *
 *  @retval  EIP_TRUE  - On success
 *           EIP_FALSE - On failure
 *
 */
EIP_BOOL EIPDEV_userOBJInit()
{
    EIP_WORD wClassId;

    /*Creation of the class  #101*/
    wClassId = 101;

    if(!OBJ_CreateObject(wClassId))
    {
        return EIP_FALSE;
    }

    /* Creation of the instance #1*/
    if(!OBJ_AddInstance(wClassId, 1))
    {
        return EIP_FALSE;
    }

    /* Add service 0x01 - get_attribute_all on instance*/
    if(!OBJ_AddInstanceService(wClassId, 1,
                               (LP_OBJ_CALL_BACK)EIPDEV_userOBJGetAttributeAll))
    {
        return EIP_FALSE;
    }

    if(! ETH_AddInstance(2))
    {
        return EIP_FALSE;
    }

    if(!DLRbInit())
    {
        return EIP_FALSE;
    }



    return EIP_TRUE;
}

/**
 *  @brief  Get Attribute call for the User object
 *
 *  @param  wTransacID   [in]
 *  @param  pstEIP_EmReq [in]
 *  @param  pstEIP_EmRes [in] Response
 *
 *  @retval  EIP_TRUE - On success
 *
 */
EIP_BOOL EIPDEV_userOBJGetAttributeAll(EIP_WORD wTransacID,
                                       LP_ST_EIP_EM_REQ pstEIP_EmReq, LP_ST_EIP_EM_RES pstEIP_EmRes)
{
    pstEIP_EmRes->lpResFrame    = Response;
    pstEIP_EmRes->wResFrameSize = sizeof(Response);
    return EIP_TRUE;
}

/*---------------------------------------------------------------------------*/
/*----------------------------- END OF FILE ---------------------------------*/
/*---------------------------------------------------------------------------*/
