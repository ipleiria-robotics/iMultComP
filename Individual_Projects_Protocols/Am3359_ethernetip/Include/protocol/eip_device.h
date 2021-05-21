/*
+------------------------------------------------------------------------------+
| Woodhead Software & Electronics                                              |
| -------------------------------                                              |
|                                                                              |
|  Copyright 2007 (c)   Woodhead Software & Electronics. All rights reserved.  |
+------------------------------------------------------------------------------+
  Description .......:  User implementation of the EIP sample

+------------------------------------------------------------------------------+
  $Workfile: eip_device.h $
  $Revision: 8 $
  $Date: 27/06/11 11:18 $
+------------------------------------------------------------------------------+
  Version | Date     | Author               | Comments
  1.0.0     05/01/11   E.LUDE
+------------------------------------------------------------------------------+
*/

#ifndef DEVICE__H
#define DEVICE__H

// ----------------------------------------------------------------------------
// .......................App includes.........................................
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// .......................Definitions..........................................
// ----------------------------------------------------------------------------

#define CIP_SERIAL_NUMBER   0xFFFFFFFF
#define CIP_VENDOR_ID       806
#define CIP_DEVICE_TYPE     0x0C  //..............Communication adapter
#define CIP_PRODUCT_CODE    0x402

#define REVISION_MAJOR      1
#define REVISION_MINOR      1
#define PRODUCT_NAME        "TI/Molex EIP Adapter"

#define MAX_PRODUCT_NAME_LENGTH 32

// Shared Memory Management
#define TOGGLE_USER_OUT (Handshake[0] ^= 0x02)
#define TOGGLE_USER_IN  (Handshake[0] ^= 0x01)

#define USER_IN  (Handshake[0] & 0x01)
#define STACK_IN (Handshake[1] & 0x01)
#define USER_OUT  (Handshake[0] & 0x02)
#define STACK_OUT (Handshake[1] & 0x02)

#define EIP_CONN_NONE  0
#define EIP_CONN_AVAIL  1
#define EIP_CONN_TIMEOUT  2
// ----------------------------------------------------------------------------
// .......................Functions..........................................
// ----------------------------------------------------------------------------

EIP_BYTE USER_Get_ConnectionStat();
EIP_BYTE USER_Set_ACDConflictDetails(CLASS_TCP_LAST_CONFLICT_DETECTED gstAcdConflictMacid);
/**
 *  @brief  The function returns the Vendor ID
 *
 *  @param  none
 *
 *  @retval  Vendor ID
 *
 */
EIP_WORD EIPDEV_getVendorId();
/**
 *  @brief  The function returns the Device Type
 *
 *  @param  none
 *
 *  @retval  Device Type
 *
 */
EIP_WORD EIPDEV_getDeviceType();
/**
 *  @brief  The function returns the Product Code
 *
 *  @param  none
 *
 *  @retval  Product Code
 *
 */
EIP_WORD EIPDEV_getProductCode();
/**
 *  @brief  The function returns the Revision number Major
 *
 *  @param  none
 *
 *  @retval  Major revision number
 *
 */
EIP_BYTE EIPDEV_getMajorRevision();
/**
 *  @brief  The function returns the Revision number Minor
 *
 *  @param  none
 *
 *  @retval  Minor revision number
 *
 */
EIP_BYTE EIPDEV_getMinorRevision();
/**
 *  @brief  The function returns the Serial number
 *
 *  @param  none
 *
 *  @retval  Serial number
 *
 */
EIP_DWORD EIPDEV_getSerialNum();
/**
 *  @brief  The function returns the Product name
 *
 *  @param  pProductName [in] Product Name
 *
 *  @retval  Length of Product name
 *
 */
EIP_BYTE EIPDEV_getProductName(EIP_CHAR * pProductName);
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
EIP_VOID EIPDEV_sHMSetInParams (  EIP_WORD wConnectionID, EIP_WORD wSize, EIP_WORD wOffset );
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
EIP_VOID EIPDEV_sHMSetOutParams(  EIP_WORD wConnectionID, EIP_WORD wSize, EIP_WORD wOffset );
/**
 *  @brief  This function will be called by stack whenever there is an IN data
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMNewDataIn(void);
/**
 *  @brief  This function is an acknowledgment from Stack when OUT data is transmittted.
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMDataOutCopied(void);
/**
 *  @brief  This function sets the Device in Copyinout mode
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMCopyInOut(void);
/**
 *  @brief  This function sets the Device in Incrementing mode
 *
 *  @param  none
 *
 *  @retval  none
 *
 */
void EIPDEV_sHMIncOut(void);

/**
 *  @brief  The function initializes the  shared memory IO Data exchange
 *
 *  @param  none
 *
 *  @retval  Shared memory param structure
 *
 */
ST_SHM_PARAMS * EIPDEV_getSharedMemoryPtr();
/**
 *  @brief  Initialize the User objects
 *
 *  @param  none
 *
 *  @retval  EIP_TRUE  - On success
 *           EIP_FALSE - On failure
 *
 */
EIP_BOOL EIPDEV_userOBJInit();
EIP_VOID RegisterUDPOptCallBack();

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // DEVICE__H

/*---------------------------------------------------------------------------*/
/*----------------------------- END OF FILE ---------------------------------*/
/*---------------------------------------------------------------------------*/
