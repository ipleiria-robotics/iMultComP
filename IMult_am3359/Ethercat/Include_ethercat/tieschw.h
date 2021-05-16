/**
 * @file tieschw.h
 *
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
#ifndef _TIESCHW_H_
#define _TIESCHW_H_

/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/

#include <Ethercat/Include_ethercat/tiescbsp.h>
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>
#ifdef TIESC_SPI_MASTER_MODE
#include <Ethercat/Include_ethercat/tiesc_spi.h>
#endif

/*-----------------------------------------------------------------------------------------
------
------  Defines and Types
------
-----------------------------------------------------------------------------------------*/
#define DISABLE_ESC_INT()               ;//bsp_global_mutex_lock();
#define ENABLE_ESC_INT()                ;//bsp_global_mutex_unlock();

#define    MAX_SYNC_MAN                         8
#define    SIZEOF_SM_REGISTER                   8

#ifdef ECAT_LIMITED_DEMO
//Lib Mode
#define TIESC_REVISION_NUMBER               0x00000001
#else
//Patch mode
#define TIESC_REVISION_NUMBER               0x00000011
#endif

#define TIESC_EEPROM_SIZE         0x800

#define    MAILBOX_WRITE                        0
#define    MAILBOX_READ                         1
#define    PROCESS_DATA_OUT                     2
#define    PROCESS_DATA_IN                      3

#define     MBX_WRITE_EVENT                 ((uint16_t) 0x0100)
#define     MBX_READ_EVENT                  ((uint16_t) 0x0200)

//Below constants are not defined in esc.h
#define ESC_ADDR_REV_TYPE       0x000
#define ESC_ADDR_BUILD          0x002

#define ESC_ADDR_CONFIG_STATION_ALIAS   0x012
#define ESC_ADDR_DLSTATUS       0x110
#define ESC_ADDR_ALCONTROL      0x120
#define ESC_ADDR_ALSTATUS       0x130
#define ESC_ADDR_PDI_CONTROL    0x140
#define ESC_PDI_CONTROL_ELD_ALL_PORTS_MASK (1 << 1)
#define ESC_ADDR_PDI_CONFIG     0x150
#define ESC_ADDR_AL_EVENT_MASK  0x204
#define ESC_ADDR_AL_EVENT_REQ   0x220
#define ESC_ADDR_SM_WD_STATUS   0x440
#define ESC_ADDR_EEPROM_CTRL    0x502
#define ESC_ADDR_MI_ECAT_ACCESS 0x516
#define ESC_ADDR_MI_PDI_ACCESS  0x517

#define ESC_EEPROM_CMD_MASK                     0x0700 //Description (0x502.8:10): Command bit mask
#define ESC_EEPROM_CMD_READ_MASK                0x0100 //Description (0x502.8): Currently executed read command
#define ESC_EEPROM_CMD_WRITE_MASK               0x0200 //Description (0x502.9): Initialize Write Command
#define ESC_EEPROM_CMD_RELOAD_MASK              0x0400 //Description (0x502.10): Trigger EEPROM reload
#define ESC_EEPROM_ERROR_MASK                   0x7800 //Description : Mask all EEPROM error bits; Checksum error (0x0502.11); EEPROM not loaded (0x0502.12); Missing EEPROM Acknowledge (0x0502.13); Write Error (0x0502.14)
#define ESC_EEPROM_ERROR_CRC                    0x0800 //Description (0x502.11): EEPROM CRC Error
#define ESC_EEPROM_ERROR_CMD_ACK                0x2000 //Description (0x502.13): EEPROM Busy
#define ESC_EEPROM_BUSY_MASK                    0x8000  //Description (0x502.15): EEPROM Busy

#define ESC_ADDR_SYNCMAN 0x800

#define ESC_ADDR_SM1_STATUS 0x80D
#define SM_STATUS_MBX_FULL  0x08

#define ESC_ADDR_SM0_STATUS     0x805
#define ESC_ADDR_SM0_ACTIVATE   0x806
#define ESC_ADDR_SM1_ACTIVATE   0x806+8
#define ESC_ADDR_SM2_ACTIVATE   0x806+8*2
#define ESC_ADDR_SM3_ACTIVATE   0x806+8*3
#define ESC_ADDR_SM4_ACTIVATE   0x806+8*4
#define ESC_ADDR_SM5_ACTIVATE   0x806+8*5
#define ESC_ADDR_SM6_ACTIVATE   0x806+8*6
#define ESC_ADDR_SM7_ACTIVATE   0x806+8*7
#define ESC_ADDR_SM0_PDI_CONTROL    0x807
#define ESC_ADDR_SM1_PDI_CONTROL    0x807+8
#define ESC_ADDR_SM2_PDI_CONTROL    0x807+8*2
#define ESC_ADDR_SM3_PDI_CONTROL    0x807+8*3
#define ESC_ADDR_SM4_PDI_CONTROL    0x807+8*4
#define ESC_ADDR_SM5_PDI_CONTROL    0x807+8*5
#define ESC_ADDR_SM6_PDI_CONTROL    0x807+8*6
#define ESC_ADDR_SM7_PDI_CONTROL    0x807+8*7

#define SM_PDI_CONTROL_SM_DISABLE      1

#define ESC_ADDR_SYSTIME            0x910
#define ESC_ADDR_SYSTIME_HIGH       0x914
#define ESC_ADDR_SYSTIME_OFFSET     0x920
#define ESC_ADDR_SYSTIME_DELAY      0x928
#define ESC_ADDR_SPEEDCOUNTER_START 0x930
#define ESC_ADDR_TIMEDIFF_FILTDEPTH 0x934
#define ESC_ADDR_SPEEDDIFF_FILTDEPTH 0x935
#define ESC_ADDR_SYNC_PULSE_LENGTH  0x982
#define ESC_ADDR_SYNC_STATUS        0x98E
#define ESC_ADDR_LATCH0_CONTROL     0x9A8
#define ESC_ADDR_LATCH1_CONTROL     0x9A9
#define ESC_ADDR_LATCH0_POS_EDGE    0x9B0
#define ESC_ADDR_LATCH0_NEG_EDGE    0x9B8
#define ESC_ADDR_LATCH1_POS_EDGE    0x9C0
#define ESC_ADDR_LATCH1_NEG_EDGE    0x9C8
#define ESC_ADDR_TI_PORT0_ACTIVITY  0xE00
#define ESC_ADDR_TI_PORT1_ACTIVITY  0xE04
#define ESC_ADDR_TI_PORT0_PHYADDR   0xE08
#define ESC_ADDR_TI_PORT1_PHYADDR   0xE09
#define ESC_ADDR_TI_PDI_ISR_PINSEL  0xE0A
#define ESC_ADDR_TI_PHY_LINK_POLARITY   0XE0C
#define ESC_ADDR_TI_PORT0_TX_START_DELAY    0xE10
#define ESC_ADDR_TI_PORT1_TX_START_DELAY    0xE12
#define ESC_ADDR_TI_ESC_RESET       0xE14
#define ESC_ADDR_TI_EDMA_LATENCY_ENHANCEMENT    0xE24
#define TI_ESC_RST_CMD_U    0x545352
#define TI_ESC_RST_CMD_L    0x747372

#define ESC_ADDR_MEMORY         0x1000

#define CMD_DL_USER_CLEAR_AL_EVENT_HIGH     0x0
#define CMD_DL_USER_GET_BUFFER_READ_ADDR    0x1
#define CMD_DL_USER_GET_BUFFER_WRITE_ADDR   0x2
#define CMD_DL_USER_SET_BUFFER_WRITE_DONE   0x3
/**
 * @def CMD_DL_USER_ACK_MBX_READ
 *  Mailbox read ACK
 */
#define CMD_DL_USER_ACK_MBX_READ            0x4
/**
 * @def CMD_DL_USER_ACK_MBX_WRITE
 *  Mailbox write ACK
 */
#define CMD_DL_USER_ACK_MBX_WRITE           0x5
/**
 * @def CMD_DL_USER_EEPROM_CMD_ACK
 *  User EEPROM ACK
 */
#define CMD_DL_USER_EEPROM_CMD_ACK          0x6
/**
 * @def CMD_DL_USER_READ_SYNC_STATUS
 *  User Read sync status
 */
#define CMD_DL_USER_READ_SYNC_STATUS        0x7
#define SYNC0   0
#define SYNC1   1
/**
 * @def CMD_DL_USER_READ_AL_CONTROL
 *  User Read AL Control
 */
#define CMD_DL_USER_READ_AL_CONTROL         0x8
/**
 * @def CMD_DL_USER_WRITE_AL_STATUS
 *  User Read AL Status
 */
#define CMD_DL_USER_WRITE_AL_STATUS         0x9
/**
 * @def CMD_DL_USER_READ_PD_WD_STATUS
 *  User Read PD_WD Status
 */
#define CMD_DL_USER_READ_PD_WD_STATUS       0xA
/**
 * @def CMD_DL_USER_READ_SM_ACTIVATE
 *  User Read SM Activate
 */
#define CMD_DL_USER_READ_SM_ACTIVATE        0xB
/**
 * @def CMD_DL_USER_WRITE_SM_PDI_CTRL
 *  User Write SM PDI control
 */
#define CMD_DL_USER_WRITE_SM_PDI_CTRL       0xC
/**
 * @def CMD_DL_USER_READ_LATCH_TIME
 *  User Read latch time
 */
#define CMD_DL_USER_READ_LATCH_TIME         0xD
#define LATCH0_POS_EDGE 0
#define LATCH0_NEG_EDGE 1
#define LATCH1_POS_EDGE 2
#define LATCH1_NEG_EDGE 3

/**
 * @def CMD_DL_USER_READ_SYS_TIME
 *  User Read sys time
 */
#define CMD_DL_USER_READ_SYS_TIME           0xE
/**
 * @def CMD_DL_USER_CLEAR_AL_EVENT_LOW
 *  User clear AL event low
 */
#define CMD_DL_USER_CLEAR_AL_EVENT_LOW      0xF
#ifdef SYSTEM_TIME_PDI_CONTROLLED
/**
* @def CMD_DL_USER_SYSTIME_PDI_CONTROL
*  User SYStime PDI control
*/
#define CMD_DL_USER_SYSTIME_PDI_CONTROL    0x10
#define WRITE_SYSTIME              0
#define WRITE_SYSTIME_OFFSET       1
#define WRITE_FILTER_CONFIG        2
#endif

#ifndef TIMER_INT_HEADER
#define TIMER_INT_HEADER
#endif


#ifndef EE_BUSY_TIMEOUT_VALUE
#define EE_BUSY_TIMEOUT_VALUE 0x2000
#endif

#define ESC_RD                  0x02    /* read acces to ESC */
#define ESC_WR                  0x04    /* write acces to ESC */


#define ESC_PDI_INTERFACE_ON_CHIP_BUS       0x80
#define ESC_PDI_INTERFACE_SPI_SLAVE         0x05


#ifdef  _TIESCHW_
#undef PROTO
#define PROTO
#else
#define PROTO extern
#endif

/*-----------------------------------------------------------------------------------------
------
------  variables
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------  function prototypes
------
-----------------------------------------------------------------------------------------*/

/** @addtogroup ECAT_MISC
@{ */
/**
*  @brief     Initializes EtherCAT slave interface by calling bsp_init. Wait for firmware to be ready. Reset AL event mask.
*  Register PDI and SYNC interrupt handlers. Disables all SyncManagers
*
*  @retval 0
*/
PROTO uint8_t HW_Init(void);
/**
*  @brief    Wrapper API for bsp_exit, delete RTOS resources, flush ESI EEPROM, disable PRUs
*
*/
PROTO void HW_Release(void);
/**
*  @brief    Perform the WARM RST of AM335x
*
*  <b>IMPORTANT NOTE</b> \n
*  This will result in application state to be lost and disrupt ongoing EtherCAT communication. Typical use is to upgrade the EtherCAT firmware
*/
PROTO void HW_RestartTarget(void);
#if UC_SET_ECAT_LED
/**
*  @brief    API to control onboard Run and ERR LEDs
*
*  @param RunLed 1 : on 0 : off
*  @param ErrLed   1 : on 0 : off
*/
PROTO void HW_SetLed(uint8_t RunLed, uint8_t ErrLed);
#endif
/**
@}
*/
#ifdef TIESC_SPI_MASTER_MODE
/**
*  @brief  Used by application to ALEventRegister from ESC memory
*
*/
PROTO uint16_t HW_GetALEventRegister();

#define HW_GetALEventRegister_Isr() HW_GetALEventRegister()
#else
#define HW_GetALEventRegister() (bsp_read_word(pruIcss1Handle, ESC_ADDR_AL_EVENT_REQ))
#define HW_GetALEventRegister_Isr() (bsp_read_word_isr(pruIcss1Handle, ESC_ADDR_AL_EVENT_REQ))
#define HW_GetALEventRegisterDword_Isr() (bsp_read_dword_isr(pruIcss1Handle, ESC_ADDR_AL_EVENT_REQ))
#endif

/** @addtogroup ECAT_INTMGMNT
 @{ */
/**
*  @brief  Reset AL event mask in AL event mask register (0x204), write AND of
*  intMask and current AL event mask value to AL event mask register
*
*  @param intMask AL event mask
*/
PROTO void HW_ResetALEventMask(uint16_t intMask);
/**
*  @brief  Set AL event mask in AL event mask register (0x204), write OR of
*  intMask and current AL event mask value to AL event mask register
*
*  @param intMask AL event mask
*/
PROTO void HW_SetALEventMask(uint16_t intMask);
/**
*  @brief   Return the current AL event mask used in Stack
*
*  @retval Current AL event mask
*/
PROTO uint16_t HW_GetALEventMask(void);
/**
*  @brief    PDI IRQ handler in the EtherCAT slave stack
*/
PROTO void HW_EcatIsr(void);
/**
*  @brief    PDI IRQ handler in the EtherCAT slave stack
*/
PROTO void PDI_Isr(void);
/**
*  @brief    SYNC0 IRQ handler in the EtherCAT slave stack
*/
PROTO void Sync0_Isr(void);
/**
*  @brief    SYNC1 IRQ handler in the EtherCAT slave stack
*/
PROTO void Sync1_Isr(void);
/**
*  @brief    SYNC1 IRQ handler in the EtherCAT slave stack
*/
/**
@}
*/

PROTO void HW_EscRead(uint8_t *pData, uint16_t Address, uint16_t Len);

/** @addtogroup ECAT_ESC_REG_ACCESS
 @{ */
/**
*  @brief  Used by application to read process data byte from ESC memory
*
*  @param Address ESC address to read from PDI
*  @param ByteValue at address
*
*/
PROTO void HW_EscReadByteIsr(uint8_t ByteValue, uint16_t Address);
/**
*  @brief  Used by application to read process data byte from ESC memory
*
*  @param Address ESC address to read from PDI
*  @param WordValue at address
*
*/
PROTO void HW_EscReadWordIsr(uint16_t WordValue, uint16_t Address);
/**
*  @brief  Used by application to read process data byte from ESC memory
*
*  @param Address ESC address to read from PDI
*  @param DWordValue at address
*
*/
PROTO void HW_EscReadDWordIsr(uint32_t DWordValue, uint16_t Address);
/**
*  @brief  Used by application to read process data byte from ESC memory
*
*  @param pData Pointer to the data array to read
*  @param Address ESC address to read from PDI
*  @param Len Length
*
*/
PROTO void HW_EscReadIsr(uint8_t *pData, uint16_t Address, uint16_t Len);

/**
*  @brief  Write a byte value at 'address' in ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param ByteValue Value to write
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteByte(uint8_t ByteValue, uint16_t Address);

/**
*  @brief  Write a byte value at 'address' in ESC memory
*
*  @param ByteValue Value to write
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteByteIsr(uint8_t ByteValue, uint16_t Address);

/**
*  @brief   Write a 16-bit value to 'address' in ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param WordValue Value at address
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteWord(uint16_t WordValue, uint16_t Address);

/**
*  @brief    Write a word value at 'address' in ESC process data memory - SM buffer (3-buffer) mapped area
*
*  @param WordValue Value to write
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteWordIsr(uint16_t WordValue, uint16_t Address);

/**
*  @brief    Write a 32-bit value to 'address' in ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param DWordValue Value at address
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteDWord(uint32_t DWordValue, uint16_t Address);

/**
*  @brief   Write a 32-bit dword value at 'address' in ESC process data memory - SM buffer (3-buffer) mapped area
*
*  @param DWordValue Value to write
*  @param Address ESC address to write from PDI
*
*/
PROTO void HW_EscWriteDWordIsr(uint32_t DWordValue, uint16_t Address);

/**
*  @brief   Write 'len' bytes to 'address' in ESC memory from pdata
*
*  @param pData Pointer to data array in application
*  @param Address ESC address to write from PDI
*  @param Len Length of data array in application
*
*/
PROTO void HW_EscWrite(uint8_t *pData, uint16_t Address, uint16_t Len);

/**
*  @brief   Write 'len' bytes to 'address' in ESC memory from pdata, used to copy process data input values from slave application to ESC memory
*  for sending to EtherCAT master
*
*  @param pData Pointer to data array in application
*  @param Address ESC address to write from PDI
*  @param Len Length of data array in application
*
*/
PROTO void HW_EscWriteIsr(uint8_t *pData, uint16_t Address, uint16_t Len);
/**
@}
*/


PROTO uint8_t __HW_EscReadByteIsr(PRUICSS_Handle pruIcssHandle,
                                  uint16_t Address);
PROTO uint16_t __HW_EscReadWordIsr(PRUICSS_Handle pruIcssHandle,
                                   uint16_t Address);
PROTO uint32_t __HW_EscReadDWordIsr(PRUICSS_Handle pruIcssHandle,
                                    uint16_t Address);
extern PRUICSS_Handle pruIcss1Handle;

#ifndef TIESC_SPI_MASTER_MODE
#define HW_EscReadByteIsr(ByteValue, Address) ((ByteValue) = __HW_EscReadByteIsr(pruIcss1Handle, Address))
#define HW_EscReadWordIsr(WordValue, Address) ((WordValue) = __HW_EscReadWordIsr(pruIcss1Handle, Address))
#define HW_EscReadDWordIsr(DWordValue, Address)((DWordValue) = __HW_EscReadDWordIsr(pruIcss1Handle, Address))

#define HW_EscReadByte(ByteValue, Address) ((ByteValue) = bsp_read_byte(pruIcss1Handle, Address))
#define HW_EscReadWord(WordValue, Address) ((WordValue) = bsp_read_word(pruIcss1Handle, Address))
#define HW_EscReadDWord(DWordValue, Address) ((DWordValue) = bsp_read_dword(pruIcss1Handle, Address))
#else
#define HW_EscReadByteIsr(ByteValue, Address) spi_master_bsp_read(Address, (uint8_t *) &ByteValue, 1)
#define HW_EscReadWordIsr(WordValue, Address) spi_master_bsp_read(Address, (uint8_t *) &WordValue, 2)
#define HW_EscReadDWordIsr(DWordValue, Address) spi_master_bsp_read(Address, (uint8_t *) &DWordValue, 4)

#define HW_EscReadByte(ByteValue, Address) spi_master_bsp_read(Address, (uint8_t *) &ByteValue, 1)
#define HW_EscReadWord(WordValue, Address) spi_master_bsp_read(Address, (uint8_t *) &WordValue, 2)
#define HW_EscReadDWord(DWordValue, Address) spi_master_bsp_read(Address, (uint8_t *)&DWordValue, 4)
#endif


/** @addtogroup ECAT_SYNC_MAILBOX
 @{ */
/**
*  @brief  Read 'len' bytes from ESC mailbox @ 'address' into application buffer pointed by 'pdata'
*
*  @param pData Pointer to data array in application
*  @param Address Mailbox address to read form PDI
*  @param Len Length of data array in application
*
*/
PROTO void HW_EscReadMbxMem(uint8_t *pData, uint16_t Address, uint16_t Len);
/**
*  @brief  Write 'len' bytes from application buffer pointed by 'pdata' and copy to ESC mailbox @ 'address'
*
*  @param pData Pointer to data array in application
*  @param Address Mailbox address to write form PDI
*  @param Len Length of data array in application
*
*/
PROTO void HW_EscWriteMbxMem(uint8_t *pData, uint16_t Address, uint16_t Len);
/**
@}
*/








/** @addtogroup ECAT_SYNC_ACC_CTRL
 @{ */
/**
*  @brief  Disables SYNC manager channel from PDI
*
*  @param channel channel number(0-7)
*
*/
PROTO void HW_DisableSyncManChannel(uint16_t channel);
/**
*  @brief Enables SYNC manager channel from PDI
*
*  @param channel channel number(0-7)
*
*/
PROTO void HW_EnableSyncManChannel(uint16_t channel);
/**
*  @brief  Return SM properties like (address, length, control byte) to application from corresponding ESC registers
*
*  @param channel channel number(0-7)
*
*  @retval Pointer to requested SM channel settings
*
*/
PROTO void *HW_GetSyncMan(uint8_t channel);
/**
@}
*/

/** @addtogroup ECAT_TIMER
 @{ */
/**
*  @brief Returns the time difference from last call of "HW_ClearTimer" to this "HW_GetTimer". handles overflow.
*  This is a wrapper API used by SSC.
*
*  @retval 32-bit time elapsed since last call to HW_ClearTimer
*/
PROTO unsigned int HW_GetTimer();
/**
*  @brief Update the time when "HW_ClearTimer" last invoked.This is a wrapper API
*/
PROTO void HW_ClearTimer();

/**
@}
*/

/** @addtogroup ECAT_ESI_EEPROM
 @{ */
/**
*  @brief  Perform reload operation after validating EEPROM CRC
*
*/
PROTO uint16_t HW_EepromReload();
/**
@}
*/


#ifdef ENABLE_PDI_SWI
PROTO void PDI_Swi(void);
#endif

#undef PROTO

#endif //_TIESCHW_H_
