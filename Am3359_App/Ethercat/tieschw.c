/**
 * tieschw.c
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

/*
*  This file is derived from MCI h/w source
*/

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/
#ifndef DISABLE_UART_PRINT
#include <stdio.h>
#endif

#include <Ethercat/Include_ethercat/tieschw.h>
#include <Ethercat/Include_ethercat/tiescutils.h>

#if !defined(TIESC_SPI_SLAVE_MODE) && !defined(ECAT_LIMITED_DEMO)
#include <ecat_def.h>
#endif
#include <Include/Board_am335x/board_gpioLed.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>

extern PRUICSS_Handle pruIcss1Handle;
extern uint32_t pd_read_addr_err, pd_write_addr_err;

#ifdef TIESC_SPI_MASTER_MODE
extern void SPIMaster_open(void);
extern void AppPdiGpioCallbackfxn(void);
extern void AppSync1GpioCallbackfxn(void);
extern void AppSync0GpioCallbackfxn(void);
#endif

/*--------------------------------------------------------------------------------------
------
------  local Types and Defines
------
--------------------------------------------------------------------------------------*/
typedef struct
{
    uint16_t PhysicalStartAddress;
    uint16_t Length;
    uint8_t Settings[4];
} __attribute__((packed)) TSYNCMAN;



#ifdef SOC_K2G
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK                 (0x3C000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_SHIFT                (26U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_RESETVAL             (0x00000000U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MAX                  (0x00000004U)
#define CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MII_MODE_VAL         (2u)
#endif
/*-----------------------------------------------------------------------------------------
------
------  local variables and constants
------
-----------------------------------------------------------------------------------------*/
TSYNCMAN TmpSyncMan;
uint16_t nAlEventMask;

/*-----------------------------------------------------------------------------------------
------
------  local functions
------
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------  functions
------
-----------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return  0 if initialization was successful

           \brief   This function intializes the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

uint8_t HW_Init(void)
{
    uint16_t u16PdiCtrl;
#ifdef TIESC_SPI_MASTER_MODE
    SPIMaster_open();

    /* Set the callback function for PDI GPIO pin*/
    GPIO_setCallback(SPI_MASTER_PDI_INT_PIN, AppPdiGpioCallbackfxn);
    /* Enable GPIO interrupt on the specific gpio pin */
    GPIO_enableInt(SPI_MASTER_PDI_INT_PIN);

    /* Set the callback function SYNC0 GPIO pin*/
    GPIO_setCallback(SPI_MASTER_SYNC0_INT_PIN, AppSync0GpioCallbackfxn);
    /* Enable GPIO interrupt on the specific gpio pin */
    GPIO_enableInt(SPI_MASTER_SYNC0_INT_PIN);

    /* Set the callback function SYNC1 GPIO pin*/
    GPIO_setCallback(SPI_MASTER_SYNC1_INT_PIN, AppSync1GpioCallbackfxn);
    /* Enable GPIO interrupt on the specific gpio pin */
    GPIO_enableInt(SPI_MASTER_SYNC1_INT_PIN);

#endif

    /* the memory interface to the ESC, the ESC-interrupt and the ECAT-timer for the
       watchdog monitoring should be initialized here microcontroller specific */


#ifdef SOC_K2G
    /* Setting the Pin Mux for MII mode for PRU0 in the GPCFG register */
    uint32_t regVal = *(volatile uint32_t *)
                      (((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->prussCfgRegBase
                       + CSL_ICSSCFG_GPCFG1);
    regVal &= (~CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK);
    regVal |= (CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MII_MODE_VAL
               << CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_SHIFT)
              & CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK;

    *(volatile uint32_t *)
    (((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->prussCfgRegBase
     + CSL_ICSSCFG_GPCFG1) = regVal;

    /* Setting the Pin Mux for MII mode for PRU1 in the GPCFG register */
    regVal = *(volatile uint32_t *)
             (((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->prussCfgRegBase
              + CSL_ICSSCFG_GPCFG0);
    regVal &= (~CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK);
    regVal |= (CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MII_MODE_VAL
               << CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_SHIFT)
              & CSL_ICSSCFG_GPCFG0_PRU0_GP_MUX_SEL_MASK;

    *(volatile uint32_t *)
    (((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->prussCfgRegBase
     + CSL_ICSSCFG_GPCFG0) = regVal;
    ASSERT_DMB();
#endif

    bsp_init(pruIcss1Handle);

    /* we have to wait here, until the ESC is started */
    do
    {
        HW_EscReadWord(u16PdiCtrl, ESC_ADDR_PDI_CONTROL);
        u16PdiCtrl = SWAPWORD(u16PdiCtrl) & 0xFF;

    }
    while((u16PdiCtrl != ESC_PDI_INTERFACE_ON_CHIP_BUS)
            && (u16PdiCtrl !=
                ESC_PDI_INTERFACE_SPI_SLAVE));    //Looking for onchip bus or SPI Slave

    /* Reading ESC Revision and Build */
    uint16_t build, revision;
    HW_EscReadWord(revision, ESC_ADDR_REV_TYPE);
    HW_EscReadWord(build, ESC_ADDR_BUILD);
    display_esc_version(revision, build);

#ifndef TIESC_SPI_MASTER_MODE
    bsp_start_esc_isr(pruIcss1Handle);
#endif

    return 0;
}

void HW_Release(void)
{
#ifndef TIESC_SPI_MASTER_MODE
    bsp_exit(pruIcss1Handle);
#endif
}

void HW_EscRead(uint8_t *pData, uint16_t Address, uint16_t Len)
{
#ifndef TIESC_SPI_MASTER_MODE
    bsp_read(pruIcss1Handle, pData, Address, Len);
    bsp_pdi_post_read_indication(pruIcss1Handle, Address, Len);
#else
    spi_master_bsp_read(Address, pData, Len);
#endif
}

void HW_EscReadIsr(uint8_t *pData, uint16_t Address, uint16_t Len)
{
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_read_at_offset(Address, Len, pData) == 0)
    {
        return;
    }

#endif

    Int16 sm_index;
    uint16_t ActAddress = bsp_get_process_data_address(pruIcss1Handle, Address, Len,
                          &sm_index);

    if(ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return;
    }

    bsp_read(pruIcss1Handle, pData, ActAddress, Len);

    bsp_process_data_access_complete(pruIcss1Handle, Address, Len, sm_index);
#else
    spi_master_bsp_read(Address, pData, Len);
#endif
}

uint8_t __HW_EscReadByteIsr(PRUICSS_Handle pruIcssHandle, uint16_t Address)
{
    uint8_t ByteValue;
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_read_at_offset(Address, 1, &ByteValue) == 0)
    {
        return ByteValue;
    }

#endif

    Int16 sm_index;
    uint16_t ActAddress = bsp_get_process_data_address(pruIcssHandle, Address, 1,
                          &sm_index);

    if(ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }

    ByteValue = bsp_read_byte_isr(pruIcssHandle, ActAddress);

    bsp_process_data_access_complete(pruIcssHandle, Address, 1, sm_index);
#else
    spi_master_bsp_read(Address, &ByteValue, 1);
#endif
    return ByteValue;
}
uint16_t __HW_EscReadWordIsr(PRUICSS_Handle pruIcssHandle, uint16_t Address)
{
    uint16_t WordValue;
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_read_at_offset(Address, 2, &WordValue) == 0)
    {
        return WordValue;
    }

#endif

    Int16 sm_index;
    uint16_t  ActAddress;
    ActAddress = bsp_get_process_data_address(pruIcssHandle,
                 Address, 2, &sm_index);

    if(ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }

    WordValue = bsp_read_word_isr(pruIcssHandle, ActAddress);

    bsp_process_data_access_complete(pruIcssHandle, Address, 2, sm_index);
#else
    spi_master_bsp_read(Address, (uint8_t *) &WordValue, 2);
#endif
    return WordValue;
}

uint32_t __HW_EscReadDWordIsr(PRUICSS_Handle pruIcssHandle, uint16_t Address)
{
    uint32_t DWordValue;
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_read_at_offset(Address, 4, &DWordValue) == 0)
    {
        return DWordValue;
    }

#endif

    Int16 sm_index;
    uint16_t ActAddress = bsp_get_process_data_address(pruIcssHandle, Address, 4,
                          &sm_index);


    if(ActAddress < ESC_ADDR_MEMORY)
    {
        pd_read_addr_err++;
        return 0;
    }

    DWordValue = bsp_read_dword_isr(pruIcssHandle, ActAddress);

    bsp_process_data_access_complete(pruIcssHandle, Address, 4, sm_index);
#else
    spi_master_bsp_read(Address, (uint8_t *) &DWordValue, 4);
#endif
    return DWordValue;
}
void HW_EscReadMbxMem(uint8_t *pData, uint16_t Address, uint16_t Len)
{
#ifndef TIESC_SPI_MASTER_MODE
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_WRITE);
    bsp_pdi_mbx_read_start(pruIcss1Handle);
    bsp_read(pruIcss1Handle, pData, Address, Len);

    if(Len >= p_sm_properties->length - 2)
    {
        bsp_pdi_mbx_read_complete(pruIcss1Handle);
    }

#else
    spi_master_bsp_read(Address, pData, Len);
#endif
}

void HW_EscWrite(uint8_t *pData, uint16_t Address, uint16_t Len)
{
#ifndef TIESC_SPI_MASTER_MODE
    bsp_write(pruIcss1Handle, pData, Address, Len);
    bsp_pdi_write_indication(pruIcss1Handle, Address, Len, 0);
#else
    spi_master_bsp_write(Address, pData, Len);
#endif
}

void HW_EscWriteIsr(uint8_t *pData, uint16_t Address, uint16_t Len)
{
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_write_at_offset(Address, Len, pData) == 0)
    {
        return;
    }

#endif

    Int16 sm_index;
    uint16_t ActualAddr = bsp_get_process_data_address(pruIcss1Handle, Address, Len,
                          &sm_index);

    if(ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }

    bsp_write(pruIcss1Handle, pData, ActualAddr, Len);

    bsp_process_data_access_complete(pruIcss1Handle, Address, Len, sm_index);
#else
    spi_master_bsp_write(Address, pData, Len);
#endif
}

void HW_EscWriteDWord(uint32_t DWordValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_dword(pruIcss1Handle, DWordValue, Address);

    if(Address == (p_sm_properties->physical_start_addr + p_sm_properties->length
                   - 4))
    {
        bsp_pdi_mbx_write_complete(pruIcss1Handle);
    }

    else
    {
        bsp_pdi_write_indication(pruIcss1Handle, Address, sizeof(uint32_t),
                                 (uint16_t)DWordValue);
    }

#else
    spi_master_bsp_write(Address, (uint8_t *) &DWordValue, sizeof(uint32_t));
#endif
}

void HW_EscWriteDWordIsr(uint32_t DWordValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_write_at_offset(Address, 4, &DWordValue) == 0)
    {
        return;
    }

#endif

    Int16 sm_index;
    uint16_t ActualAddr = bsp_get_process_data_address(pruIcss1Handle, Address, 4,
                          &sm_index);

    if(ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }

    bsp_write_dword(pruIcss1Handle, DWordValue, ActualAddr);
    bsp_process_data_access_complete(pruIcss1Handle, Address, 4, sm_index);
#else
    spi_master_bsp_write(Address, (uint8_t *) &DWordValue, 4);
#endif
}
void HW_EscWriteWord(uint16_t WordValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_word(pruIcss1Handle, WordValue, Address);

    if(Address == (p_sm_properties->physical_start_addr + p_sm_properties->length
                   - 2))
    {
        bsp_pdi_mbx_write_complete(pruIcss1Handle);
    }

    else
    {
        bsp_pdi_write_indication(pruIcss1Handle, Address, sizeof(uint16_t), WordValue);
    }

#else
    spi_master_bsp_write(Address, (uint8_t *) &WordValue, sizeof(uint16_t));
#endif
}

void HW_EscWriteWordIsr(uint16_t WordValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_write_at_offset(Address, 2, &WordValue) == 0)
    {
        return;
    }

#endif

    Int16 sm_index;
    uint16_t ActualAddr = bsp_get_process_data_address(pruIcss1Handle, Address, 2,
                          &sm_index);

    if(ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }

    bsp_write_word(pruIcss1Handle, WordValue, ActualAddr);
    bsp_process_data_access_complete(pruIcss1Handle, Address, 2, sm_index);
#else
    spi_master_bsp_write(Address, (uint8_t *) &WordValue, 2);
#endif
}

void HW_EscWriteByte(uint8_t ByteValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);
    bsp_write_byte(pruIcss1Handle, ByteValue, Address);

    if(Address == (p_sm_properties->physical_start_addr + p_sm_properties->length
                   - 1))
    {
        bsp_pdi_mbx_write_complete(pruIcss1Handle);
    }

    else
    {
        bsp_pdi_write_indication(pruIcss1Handle, Address, sizeof(uint8_t), ByteValue);
    }

#else
    spi_master_bsp_write(Address, &ByteValue, sizeof(uint8_t));
#endif
}

void HW_EscWriteByteIsr(uint8_t ByteValue, uint16_t Address)
{
#ifndef TIESC_SPI_MASTER_MODE

#ifdef EDMA_LATENCY_ENHANCEMENT

    if(bsp_dma_sm_write_at_offset(Address, 1, &ByteValue) == 0)
    {
        return;
    }

#endif


    Int16 sm_index;
    uint16_t ActualAddr  = bsp_get_process_data_address(pruIcss1Handle, Address, 1,
                           &sm_index);

    if(ActualAddr < ESC_ADDR_MEMORY)
    {
        pd_write_addr_err++;
        return;
    }

    bsp_write_byte(pruIcss1Handle, ByteValue, ActualAddr);

    bsp_process_data_access_complete(pruIcss1Handle, Address, 1, sm_index);
#else
    spi_master_bsp_write(Address, &ByteValue, 1);
#endif
}

void HW_EscWriteMbxMem(uint8_t *pData, uint16_t Address, uint16_t Len)
{

#ifndef TIESC_SPI_MASTER_MODE
    t_sm_properties *p_sm_properties = bsp_get_sm_properties(MAILBOX_READ);

    //Do not write to mailbox if already full
    if((bsp_read_byte(pruIcss1Handle,
                      ESC_ADDR_SM1_STATUS) & SM_STATUS_MBX_FULL))
    {
        return;
    }

    bsp_pdi_mbx_write_start(pruIcss1Handle);
    bsp_write(pruIcss1Handle, pData, Address, Len);

    if(Len >= p_sm_properties->length - 2)
    {
        bsp_pdi_mbx_write_complete(pruIcss1Handle);
    }

#else
    spi_master_bsp_write(Address, pData, Len);
#endif
}

void HW_RestartTarget(void)
{
#ifndef TIESC_SPI_MASTER_MODE
    bsp_soft_reset();
#endif
}
void HW_SetLed(uint8_t RunLed, uint8_t ErrLed)
{
    Board_setTriColorLED(BOARD_RUN_LED, RunLed);
    Board_setTriColorLED(BOARD_ERROR_LED, ErrLed);
}

unsigned int HW_GetTimer()
{
    return bsp_get_timer_register();
}

void HW_ClearTimer()
{
    bsp_clear_timer_register();
}

UInt16 HW_EepromReload()
{
    uint16_t retval;
    retval = bsp_eeprom_emulation_reload(pruIcss1Handle);
    return retval;
}

#ifdef TIESC_SPI_MASTER_MODE
uint16_t HW_GetALEventRegister()
{
    uint16_t reg_val;
    spi_master_bsp_read(ESC_ADDR_AL_EVENT_REQ, (uint8_t *)&reg_val, 2);
    return reg_val;
}
#endif
