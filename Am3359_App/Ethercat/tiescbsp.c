/**
 * tiescbsp.c
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
#include <string.h> // For memset/memcpy
#if defined (__aarch64__)
#include <malloc.h> // For memalign
#endif



#include <Ethercat/Include_ethercat/tiesc_pruss_intc_mapping.h>
#include <Ethercat/Include_ethercat/tiescutils.h>
#include <Ethercat/Include_ethercat/tiescbsp.h>
#include <Ethercat/Include_ethercat/tieschw.h>
#include <Ethercat/Include_ethercat/tiesc_fw_cir_buff.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>

#include <Include/Board_am335x/soc_icss_header.h>

#include <osal/TaskP.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/drv/gpio/GPIO.h>

#ifndef USE_ECAT_TIMER
#include <osal/MiscP.h>
#endif

#ifdef SOC_AM65XX
#include <examples/board/include/board_dpphy.h>
#else
#include <Include/Board_am335x/board_tlkphy.h>
#endif
#include <Include/Board_am335x/board_phy.h>
#include <Include/Board_am335x/board_spi.h>
#include <Include/Board_am335x/board_gpioLed.h>
#include <Include/Board_am335x/delay_us.h>
#include <Ethercat/Include_ethercat/tiesc_spi.h>
#if !defined(TIESC_SPI_SLAVE_MODE) && !defined(ECAT_LIMITED_DEMO)
#include <ecat_def.h>
#include <esc.h>
#endif

#ifndef AMIC11X_ONCHIP_MEM_MODE
#ifdef TIESC_SPI_SLAVE_MODE
#include <tiesc_spi_eeprom.h> // header equivalent of ESI bin file
#elif CiA402_DEVICE
#include <CiA402_eeprom.h> // header equivalent of ESI bin file
#elif TIESC_APPLICATION
#ifdef ECAT_LIMITED_DEMO
//#include <demo_tiesc_eeprom.h> // header equivalent of ESI bin file
#include <Ethercat/Include_ethercat/tiesc_spi_eeprom.h>
#else
#include <tiesc_eeprom.h> // header equivalent of ESI bin file
#endif
#endif
#endif

/* DMEM 0 */
#define SYNC_PERMISSION_UPDATE_ADDR_OFFSET      0x500
#define SYNC_PERMISSION_UPDATE_ECAT_OFFSET      0x518
#define SYNC_PERMISSION_UPDATE_PDI_OFFSET       0x522
#define SYNC_PERMISSION_UPDATE_ECAT_SIZE        0x10
#define SYNC_PERMISSION_UPDATE_PDI_SIZE         0x10

#ifdef EDMA_LATENCY_ENHANCEMENT
#include <ti/sdo/edma3/drv/sample/bios6_edma3_drv_sample.h>
#include <ti/sysbios/family/arm/a9/Cache.h>

static t_sm_dma_properties sm_dma_properties[MAX_SYNC_MAN];
EDMA3_DRV_Handle hEdma;
#endif /*EDMA_LATENCY_ENHANCEMENT*/

#ifdef iceAMIC11x_onChip_mem_mode
extern const unsigned char tiesc_eeprom[] __attribute__((
            section(".eeprom_data1")));
#endif

static t_sm_properties sm_properties[MAX_SYNC_MAN];

static SemaphoreP_Handle semcmdlow_handle;

#ifndef USE_ECAT_TIMER
volatile uint32_t ecat_timer_inc_p_ms;
#endif
volatile uint32_t *pru_frame_proc = NULL;
volatile uint32_t *pru_host_proc = NULL;
volatile uint32_t pru_frame_proc_len = 0;
volatile uint32_t pru_host_proc_len = 0;

extern Board_flashHandle boardFlashHandle;

#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
static uint32_t fw_download_flag;
static uint32_t fw_write_offset;
#endif
static uint32_t current_low, prev_low;//Current and previous IEP time stamps

uint32_t pd_read_addr_err, pd_write_addr_err;
uint32_t pdi_read_fail_cnt, pdi_write_fail_cnt;

#if defined(TIESC_SPI_SLAVE_MODE) || defined(ECAT_LIMITED_DEMO)
uint8_t *pEEPROM;
#else
extern uint8_t *pEEPROM;
#endif
#ifdef EEPROM_SPI
static uint8_t eeprom_updated = 0;
static uint32_t eeprom_updated_time = 0;
#endif
#ifdef AMIC11X_ONCHIP_MEM_MODE
#define EEPROM_TI_HEADER_SIZE   8
uint8_t eeprom_ti_header[EEPROM_TI_HEADER_SIZE] __attribute__((
            section(".eeprom_data1")));
uint8_t eeprom_cache[TIESC_EEPROM_SIZE] __attribute__((
            section(".eeprom_data2")));
#else
uint8_t eeprom_cache[TIESC_EEPROM_SIZE];
#endif
extern PRUICSS_Handle pruIcss1Handle;


#ifdef ENABLE_PDI_REG_PERMISSIONS
uint16_t pdi_reg_perm_array[256];
#endif

#if ECAT_TIMER_INT
void APPL_1MsTimerIsr(void);
#endif

void bsp_esc_sync_reg_perm_update(PRUICSS_Handle pruIcssHandle);
void MDIO_enableLinkInterrupt(uint32_t mdioBaseAddress, uint32_t phyInst,
                              uint32_t phyNum, uint8_t linkSel);


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief Interrupt service routin    e for the interrupts from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////
#ifdef TIESC_SPI_SLAVE_MODE
#include <ti/starterware/include/hw/soc_am335x.h>

volatile uint32_t PruIcssIntcBaseAddr;
extern t_spi_slave_config SPISlaveConfig;

uint8_t bsp_spi_poll_pdi_isr(void)
{

    uint32_t intc_status = HW_RD_REG32(PruIcssIntcBaseAddr + CSL_ICSSINTC_SRSR0);

    if(intc_status  & (1 << PRU_ARM_EVENT0))
    {
        bsp_clear_digio_out(pruIcss1Handle, PDI_ISR_EDIO_NUM);
        HW_WR_REG16((PruIcssIntcBaseAddr + CSL_ICSSINTC_SICR), PRU_ARM_EVENT0);
        GPIO_write(SPI_SLAVE_PDI_INT_PIN, SPISlaveConfig.PDI_IRQ_active_polarity);
        GPIO_write(SPI_SLAVE_PDI_INT_PIN, SPISlaveConfig.PDI_IRQ_deactive_polarity);
        return 1;
    }

    return 0;
}
#endif

void EcatIsr(void *args)
{
#ifndef ENABLE_PDI_TASK
#ifndef ENABLE_PDI_SWI
    PDI_Isr();
#else
    PDI_Swi();
#endif
#endif
    bsp_clear_digio_out((PRUICSS_Handle)args, PDI_ISR_EDIO_NUM);
    PRUICSS_pruClearEvent((PRUICSS_Handle)args, PRU_ARM_EVENT0);

    ASSERT_DMB();
    ASSERT_DSB();
}

#ifndef SUPPORT_CMDACK_POLL_MODE
void EscCmdLowAckIsr(void *args)
{
    PRUICSS_pruClearEvent((PRUICSS_Handle)args, PRU_ARM_EVENT2);
    ASSERT_DMB();
    ASSERT_DSB();

}
#endif

void Sync0Isr(void *args)
{
    PRUICSS_pruClearEvent((PRUICSS_Handle)args, SYNC0_OUT_EVENT);
    ASSERT_DMB();
    ASSERT_DSB();

#ifndef ENABLE_SYNC_TASK
    Sync0_Isr();
#endif
}

inline void Sync1Isr(void *args)
{
    PRUICSS_pruClearEvent((PRUICSS_Handle)args, SYNC1_OUT_EVENT);
    ASSERT_DMB();
    ASSERT_DSB();

#ifndef ENABLE_SYNC_TASK
    Sync1_Isr();
#endif
}

inline uint32_t bsp_get_timer_register(void)
{
    uint32_t ret;
#ifdef USE_ECAT_TIMER
    bsp_get_local_sys_time(&current_low, 0);
#else
    current_low = OSAL_Timestamp_get32();
#endif

    if(current_low > prev_low)
    {
        ret = current_low - prev_low;
    }

    else
    {
        ret = (0xFFFFFFFF - prev_low) + current_low;
    }

    return ret;
}

inline void bsp_clear_timer_register(void)
{
#ifdef USE_ECAT_TIMER
    bsp_get_local_sys_time(&prev_low, 0);
#else
    prev_low = OSAL_Timestamp_get32();
#endif
}

void bsp_get_local_sys_time(uint32_t *systime_low, uint32_t *systime_high)
{
    uint32_t ecat_systime_low, ecat_systime_high;
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));

    if(systime_high)
    {
        bsp_send_command_to_firmware(pruIcss1Handle, CMD_DL_USER_READ_SYS_TIME, 0, 0);
        bsp_global_mutex_lock();
        ecat_systime_low = pHost2PruIntfc->system_time_low;
        ecat_systime_high = pHost2PruIntfc->system_time_high;
        bsp_global_mutex_unlock();
    }

    else
    {
        uint32_t iep_adjust_offset_value = 0;
#if  defined(SOC_AM335x) || defined (SOC_AM437x)
        iep_adjust_offset_value = HWREG((uint32_t)((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->prussPru0DramBase + DRIFTCTRL_TASK_SYNC_ZERO_CROSS_ADJUST);
#endif
        ecat_systime_low = bsp_pruss_iepreg_read(pruIcss1Handle, CSL_ICSSIEP_COUNT_REG0) - iep_adjust_offset_value;
    }

    if(systime_low)
    {
        *systime_low =  ecat_systime_low;
    }

    if(systime_high)
    {
        *systime_high = ecat_systime_high;
    }
}

void bsp_get_latch0_posedge_time(PRUICSS_Handle pruIcssHandle,
                                 uint32_t *systime_low, uint32_t *systime_high)
{
    if(0 == bsp_pdi_access_perm_array(ESC_ADDR_LATCH0_POS_EDGE, PDI_PERM_READ,
                                      8))
    {
        pdi_read_fail_cnt++;
        return;
    }

    while(bsp_hwspinlock_lock(0));

    bsp_global_mutex_lock();

    if(systime_low)
    {
        *systime_low  = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH0_POS_EDGE) >> 2)]);
    }

    if(systime_high)
    {
        *systime_high = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH0_POS_EDGE + 4) >> 2)]);
    }

    bsp_global_mutex_unlock();
    bsp_hwspinlock_unlock(0);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                 LATCH0_POS_EDGE, 0);
}

void bsp_get_latch0_negedge_time(PRUICSS_Handle pruIcssHandle,
                                 uint32_t *systime_low, uint32_t *systime_high)
{
    if(0 == bsp_pdi_access_perm_array(ESC_ADDR_LATCH0_NEG_EDGE, PDI_PERM_READ,
                                      8))
    {
        pdi_read_fail_cnt++;
        return;
    }

    while(bsp_hwspinlock_lock(0));

    bsp_global_mutex_lock();

    if(systime_low)
    {
        *systime_low  = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH0_NEG_EDGE) >> 2)]);
    }

    if(systime_high)
    {
        *systime_high = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH0_NEG_EDGE + 4) >> 2)]);
    }

    bsp_global_mutex_unlock();
    bsp_hwspinlock_unlock(0);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                 LATCH0_NEG_EDGE, 0);
}
void bsp_get_latch1_posedge_time(PRUICSS_Handle pruIcssHandle,
                                 uint32_t *systime_low, uint32_t *systime_high)
{
    if(0 == bsp_pdi_access_perm_array(ESC_ADDR_LATCH1_POS_EDGE, PDI_PERM_READ,
                                      8))
    {
        pdi_read_fail_cnt++;
        return;
    }

    while(bsp_hwspinlock_lock(0));

    bsp_global_mutex_lock();

    if(systime_low)
    {
        *systime_low  = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH1_POS_EDGE) >> 2)]);
    }

    if(systime_high)
    {
        *systime_high = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH1_POS_EDGE + 4) >> 2)]);
    }

    bsp_global_mutex_unlock();
    bsp_hwspinlock_unlock(0);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                 LATCH1_POS_EDGE, 0);
}
void bsp_get_latch1_negedge_time(PRUICSS_Handle pruIcssHandle,
                                 uint32_t *systime_low, uint32_t *systime_high)
{
    if(0 == bsp_pdi_access_perm_array(ESC_ADDR_LATCH1_NEG_EDGE, PDI_PERM_READ,
                                      8))
    {
        pdi_read_fail_cnt++;
        return;
    }

    while(bsp_hwspinlock_lock(0));

    bsp_global_mutex_lock();

    if(systime_low)
    {
        *systime_low  = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH1_NEG_EDGE) >> 2)]);
    }

    if(systime_high)
    {
        *systime_high = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                            pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                                    ESC_ADDR_LATCH1_NEG_EDGE + 4) >> 2)]);
    }

    bsp_global_mutex_unlock();
    bsp_hwspinlock_unlock(0);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                 LATCH1_NEG_EDGE, 0);
}

#ifdef SYSTEM_TIME_PDI_CONTROLLED
void bsp_pdi_write_system_time(PRUICSS_Handle pruIcssHandle, uint32_t systime)
{
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));
    *(uint32_t *)(&pHost2PruIntfc->resp1low) = systime;
    ASSERT_DMB();

    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_SYSTIME_PDI_CONTROL,
                                 WRITE_SYSTIME, 0);
}
void bsp_pdi_write_system_timeoffset(PRUICSS_Handle pruIcssHandle,
                                     unsigned long long systime)
{
    bsp_write(pruIcssHandle, (uint8_t *)&systime, ESC_ADDR_SYSTIME_OFFSET, 8);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_SYSTIME_PDI_CONTROL,
                                 WRITE_SYSTIME_OFFSET, 0);
}
void bsp_pdi_write_systime_delay(PRUICSS_Handle pruIcssHandle,
                                 uint32_t systime)
{
    bsp_write_dword(pruIcssHandle, systime, ESC_ADDR_SYSTIME_DELAY);
}
void bsp_pdi_write_filterconfig(PRUICSS_Handle pruIcssHandle,
                                uint16_t speedcount_start, uint8_t speedcount_filtdepth,
                                uint8_t systime_filtdepth)
{
    bsp_write_word(pruIcssHandle, speedcount_start, ESC_ADDR_SPEEDCOUNTER_START);
    bsp_write_word(pruIcssHandle,
                   (speedcount_filtdepth << 8 | systime_filtdepth),
                   ESC_ADDR_TIMEDIFF_FILTDEPTH);
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_SYSTIME_PDI_CONTROL,
                                 WRITE_FILTER_CONFIG, 0);
}

void bsp_pdi_latch0_control(PRUICSS_Handle pruIcssHandle, uint8_t val)
{
    uint32_t reg_val = HWREG(((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussIepRegBase +
                             CSL_ICSSIEP_CAP_CFG_REG);

    if(val & 0x1)  //Latch0 Pos Edge single shot
    {
        reg_val |= 0x40;
    }

    else
    {
        reg_val &= ~0x40;
    }

    if(val & 0x2)  //Latch0 Neg Edge single shot
    {
        reg_val |= 0x80;
    }

    else
    {
        reg_val &= ~0x80;
    }

    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_CAP_CFG_REG) = reg_val;
    ASSERT_DMB();

}

void bsp_pdi_latch1_control(PRUICSS_Handle pruIcssHandle, uint8_t val)
{
    uint32_t reg_val = HWREG(((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussIepRegBase +
                             CSL_ICSSIEP_CAP_CFG_REG);

    if(val & 0x1)   //Latch1 Pos Edge single shot
    {
        reg_val |= 0x100;
    }

    else
    {
        reg_val &= ~0x100;
    }

    if(val & 0x2)  //Latch1 Neg Edge single shot
    {
        reg_val |= 0x200;
    }

    else
    {
        reg_val &= ~0x200;
    }

    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_CAP_CFG_REG) = reg_val;
    ASSERT_DMB();

}
#endif
inline void bsp_set_pdi_wd_trigger_mode(PRUICSS_Handle pruIcssHandle,
                                        uint32_t mode)
{
    //PDI WD trigger also depends on DIGIO h/w events in ICSS and by default triggered on RX SOF events on both ports
    //This function can be used to override this behaviour in h/w - it is not possible to disable DIGIO dependency and make
    //PDI WD trigger manually in firmware for every command send - so allow user to configure - trigger mechansim
    //LATCH_IN  event or SYNC0/1_out - New application default is LATCH_IN event
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_DIGIO_CTRL_REG) = mode;
    ASSERT_DMB();

}

inline void bsp_set_digio_sw_dataout_enable(PRUICSS_Handle pruIcssHandle)
{
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_DIGIO_EXP_REG) |= 0x1;
    ASSERT_DMB();

}

inline void bsp_set_digio_out(PRUICSS_Handle pruIcssHandle, uint8_t num)
{
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_DIGIO_DATA_OUT_REG) |= (1 << num);
    ASSERT_DMB();

}

inline void bsp_clear_digio_out(PRUICSS_Handle pruIcssHandle, uint8_t num)
{
    HWREG(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          CSL_ICSSIEP_DIGIO_DATA_OUT_REG) &= ~(1 << num);
    ASSERT_DMB();

}

void bsp_eeprom_emulation_init(void)
{
#ifndef AMIC11X_ONCHIP_MEM_MODE
    uint32_t itr = 0;

#ifdef EEPROM_SPI
    uint16_t u16Crc = 0x00FF, i, j;
    int invalid_crc_flag = 0;


    uint32_t ioMode;
    ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;

    if(BOARD_FLASH_EOK != Board_flashRead(boardFlashHandle,
                                          SPI_EEPROM_DATA_OFFSET, (uint8_t *)eeprom_cache,
                                          TIESC_EEPROM_SIZE, (void *)(&ioMode)))
    {
        invalid_crc_flag = 1;
    }

    else
    {
        uint32_t temp_reg = 0;

        for(i = 0; i < 14; i++)
        {
            u16Crc ^= eeprom_cache[i];

            for(j = 0; j < 8; j++)
            {
                if(u16Crc & 0x80)
                {
                    u16Crc = (u16Crc << 1) ^ 0x07;
                }

                else
                {
                    u16Crc <<= 1;
                }
            }
        }

        /*only low Byte shall be written*/
        u16Crc &= 0x00FF;

        if(u16Crc != eeprom_cache[14])
        {
            invalid_crc_flag = 1;
        }

        temp_reg = *(uint32_t *)((uint8_t *)(eeprom_cache + (0xC << 1)));

        temp_reg = SWAPDWORD(temp_reg);

        if(temp_reg != (uint32_t)TIESC_REVISION_NUMBER)
        {
            invalid_crc_flag = 1;
        }
    }

    if(1 == invalid_crc_flag)
    {
#endif

        for(itr = 0; itr < TIESC_EEPROM_SIZE; itr++)
        {
            eeprom_cache[itr] = tiesc_eeprom[itr];
        }

#ifdef EEPROM_SPI
    }

#endif
#endif
#if ESC_EEPROM_EMULATION
    pEEPROM = eeprom_cache;
#endif
}

Int32 bsp_eeprom_load_esc_registers(PRUICSS_Handle pruIcssHandle,
                                    Int32 reload_flag)
{
    //Validate CRC before loading to ESC registers
    uint16_t u16Crc = 0x00FF, i, j;
    Int32 invalid_crc_flag = 0;

    for(i = 0; i < 14; i++)
    {
        u16Crc ^= eeprom_cache[i];

        for(j = 0; j < 8; j++)
        {
            if(u16Crc & 0x80)
            {
                u16Crc = (u16Crc << 1) ^ 0x07;
            }

            else
            {
                u16Crc <<= 1;
            }
        }
    }

    /*only low Byte shall be written*/
    u16Crc &= 0x00FF;

    if(u16Crc != eeprom_cache[14])
    {
        //printf("Error: EEPROM validation failed for config area\n");
        invalid_crc_flag = 1;
    }

    /*write new claculated Crc to Esc Config area*/
    if(!reload_flag)
    {
        //Not a reload operation - but initial loading
        //0x150 and 0x152 not reloaded according to beckhoff
        if(!invalid_crc_flag)
        {
            bsp_write_byte(pruIcssHandle, eeprom_cache[8], ESC_ADDR_CONFIG_STATION_ALIAS);
            bsp_write_byte(pruIcssHandle, eeprom_cache[9],
                           ESC_ADDR_CONFIG_STATION_ALIAS + 1);
            bsp_write_byte(pruIcssHandle, eeprom_cache[0], ESC_ADDR_PDI_CONTROL);
            bsp_write_byte(pruIcssHandle, eeprom_cache[1], ESC_ADDR_PDI_CONTROL + 1);
#ifdef TIESC_SPI_SLAVE_MODE
            bsp_write_byte(pruIcssHandle, eeprom_cache[2], ESC_ADDR_PDI_CONFIG);
            bsp_write_byte(pruIcssHandle, eeprom_cache[3], ESC_ADDR_PDI_CONFIG + 1);
#endif
        }

        else
        {
            return -1;
        }
    }

    if(!invalid_crc_flag)
    {
        bsp_write_byte(pruIcssHandle, eeprom_cache[4], ESC_ADDR_SYNC_PULSE_LENGTH);
        bsp_write_byte(pruIcssHandle, eeprom_cache[5],
                       ESC_ADDR_SYNC_PULSE_LENGTH + 1);
    }

    else
    {
        return -1;
    }

    return 0;
}
Int32 bsp_eeprom_emulation_reload(PRUICSS_Handle pruIcssHandle)
{
    return bsp_eeprom_load_esc_registers(pruIcssHandle, 1);
}

void bsp_eeprom_emulation_command_ack(PRUICSS_Handle pruIcssHandle)
{
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_EEPROM_CMD_ACK, 0, 0);
}

void bsp_eeprom_emulation_flush(void)
{
#ifdef EEPROM_SPI
    uint32_t ioMode;
    ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;

#ifdef AMIC11X_ONCHIP_MEM_MODE
    Board_flashRead(boardFlashHandle, SPI_EEPROM_DATA_OFFSET,
                    (uint8_t *)eeprom_ti_header, EEPROM_TI_HEADER_SIZE, (void *)(&ioMode));
#endif

    uint32_t blockNum, pageNum;      /* Block, page number */

    Board_flashOffsetToBlkPage(boardFlashHandle, SPI_EEPROM_DATA_OFFSET,
                               &blockNum, &pageNum);

    Board_flashEraseBlk(boardFlashHandle, blockNum);

#ifdef AMIC11X_ONCHIP_MEM_MODE
    Board_flashWrite(boardFlashHandle, SPI_EEPROM_DATA_OFFSET,
                     (uint8_t *)eeprom_ti_header, TIESC_EEPROM_SIZE + EEPROM_TI_HEADER_SIZE,
                     (void *)(&ioMode));
#else
    Board_flashWrite(boardFlashHandle, SPI_EEPROM_DATA_OFFSET,
                     (uint8_t *)eeprom_cache, TIESC_EEPROM_SIZE, (void *)(&ioMode));
#endif
#endif
}

void bsp_eeprom_emulation_exit(void)
{
    bsp_eeprom_emulation_flush();
}

void bsp_esc_reg_perm_init(PRUICSS_Handle pruIcssHandle)
{
    int i, j;
    (void) j; /*To remove unused variable warning */
    volatile t_register_properties *pRegPerm = (volatile t_register_properties *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(1));

    for(i = 0; i < 10; i++)
    {
        pRegPerm->reg_properties[i] = TIESC_PERM_READ_ONLY;
    }

    pRegPerm->reg_properties[0x10] = TIESC_PERM_RW; //Configured Station address
    pRegPerm->reg_properties[0x11] = TIESC_PERM_RW; //Configured Station address
    pRegPerm->reg_properties[0x12] =
        TIESC_PERM_READ_ONLY; //Configured Station alias
    pRegPerm->reg_properties[0x13] =
        TIESC_PERM_READ_ONLY; //Configured Station alias
    pRegPerm->reg_properties[0x100] = TIESC_PERM_RW; //DL control
    pRegPerm->reg_properties[0x101] = TIESC_PERM_RW; //DL control
    pRegPerm->reg_properties[0x102] = TIESC_PERM_RW; //DL control
    pRegPerm->reg_properties[0x103] = TIESC_PERM_RW; //DL control
    pRegPerm->reg_properties[0x108] = TIESC_PERM_RW; //Physical RW offset
    pRegPerm->reg_properties[0x109] = TIESC_PERM_RW; //Physical RW offset
    pRegPerm->reg_properties[0x110] = TIESC_PERM_READ_ONLY; //ESC DL status
    pRegPerm->reg_properties[0x111] = TIESC_PERM_READ_ONLY; //ESC DL status
    pRegPerm->reg_properties[0x120] = TIESC_PERM_RW; //AL control
    pRegPerm->reg_properties[0x121] = TIESC_PERM_RW; //AL control
    pRegPerm->reg_properties[0x130] = TIESC_PERM_READ_ONLY; //ESC AL status
    pRegPerm->reg_properties[0x131] = TIESC_PERM_READ_ONLY; //ESC AL status
    pRegPerm->reg_properties[0x134] = TIESC_PERM_READ_ONLY; //ESC AL status code
    pRegPerm->reg_properties[0x135] = TIESC_PERM_READ_ONLY; //ESC AL status code
    pRegPerm->reg_properties[0x140] = TIESC_PERM_READ_ONLY; //ESC PDI control
    pRegPerm->reg_properties[0x141] = TIESC_PERM_READ_ONLY; //ESC configuration
    pRegPerm->reg_properties[0x150] = TIESC_PERM_READ_ONLY; //Onchip configuration
    pRegPerm->reg_properties[0x151] =
        TIESC_PERM_READ_ONLY; //SYNC/LATCH PDI configuration
    pRegPerm->reg_properties[0x152] =
        TIESC_PERM_READ_ONLY; //Onchip extended configuration
    pRegPerm->reg_properties[0x153] =
        TIESC_PERM_READ_ONLY; //Onchip extended configuration
    pRegPerm->reg_properties[0x200] = TIESC_PERM_RW; //ECAT event mask
    pRegPerm->reg_properties[0x201] = TIESC_PERM_RW; //ECAT event mask
    pRegPerm->reg_properties[0x204] = TIESC_PERM_READ_ONLY; //AL event mask
    pRegPerm->reg_properties[0x205] = TIESC_PERM_READ_ONLY; //AL event mask
    pRegPerm->reg_properties[0x206] = TIESC_PERM_READ_ONLY; //AL event mask
    pRegPerm->reg_properties[0x207] = TIESC_PERM_READ_ONLY; //AL event mask
    pRegPerm->reg_properties[0x210] = TIESC_PERM_READ_ONLY; //ECAT event request
    pRegPerm->reg_properties[0x211] = TIESC_PERM_READ_ONLY; //ECAT event request
    pRegPerm->reg_properties[0x220] = TIESC_PERM_READ_ONLY; //AL event request
    pRegPerm->reg_properties[0x221] = TIESC_PERM_READ_ONLY; //AL event request
    pRegPerm->reg_properties[0x222] = TIESC_PERM_READ_ONLY; //AL event request
    pRegPerm->reg_properties[0x223] = TIESC_PERM_READ_ONLY; //AL event request
    pRegPerm->reg_properties[0x300] = TIESC_PERM_RW; //Invalid frame counter Port0
    pRegPerm->reg_properties[0x301] = TIESC_PERM_RW; //RX_ERR counter Port0
    pRegPerm->reg_properties[0x302] = TIESC_PERM_RW; //Invalid frame counter Port1
    pRegPerm->reg_properties[0x303] = TIESC_PERM_RW; //RX_ERR counter Port1
    pRegPerm->reg_properties[0x308] = TIESC_PERM_RW; //Forwarded Error Port0
    pRegPerm->reg_properties[0x309] = TIESC_PERM_RW; //Forwarded Error Port1
    pRegPerm->reg_properties[0x30C] = TIESC_PERM_RW; //ECAT processing unit counter
    pRegPerm->reg_properties[0x310] = TIESC_PERM_RW; //Link lost counter Port0
    pRegPerm->reg_properties[0x311] = TIESC_PERM_RW; //Link lost counter Port1
    pRegPerm->reg_properties[0x400] = TIESC_PERM_RW; //Watchdog divider
    pRegPerm->reg_properties[0x401] = TIESC_PERM_RW; //Watchdog divider
    pRegPerm->reg_properties[0x410] = TIESC_PERM_RW; //Watchdog time PDI
    pRegPerm->reg_properties[0x411] = TIESC_PERM_RW; //Watchdog time PDI
    pRegPerm->reg_properties[0x420] = TIESC_PERM_RW; //Watchdog time PD
    pRegPerm->reg_properties[0x421] = TIESC_PERM_RW; //Watchdog time PD
    pRegPerm->reg_properties[0x440] = TIESC_PERM_READ_ONLY; //Watchdog process data
    pRegPerm->reg_properties[0x441] = TIESC_PERM_READ_ONLY; //Watchdog process data
    pRegPerm->reg_properties[0x442] = TIESC_PERM_RW; //Watchdog counter PD
    pRegPerm->reg_properties[0x443] = TIESC_PERM_RW; //Watchdog counter PDI
    pRegPerm->reg_properties[0x500] = TIESC_PERM_RW; //EEPROM configuration
    pRegPerm->reg_properties[0x501] =
        TIESC_PERM_READ_ONLY; //EEPROM PDI access state

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x502 + i] = TIESC_PERM_RW;
    }

    for(i = 0; i < 6; i++)
    {
        pRegPerm->reg_properties[0x50A + i] = TIESC_PERM_READ_ONLY;
    }

    for(i = 0; i < 6; i++)
    {
        pRegPerm->reg_properties[0x510 + i] = TIESC_PERM_RW;
    }

    for(i = 0; i < 8; i++)    //8 FMMUs
    {
        tiesc_memset((void *)&pRegPerm->reg_properties[0x600 + i * 16], TIESC_PERM_RW,
               13);
    }

    for(i = 0; i < 8; i++)    //8 SMs
    {
        tiesc_memset((void *)&pRegPerm->reg_properties[0x800 + i * 8], TIESC_PERM_RW, 5);
        tiesc_memset((void *)&pRegPerm->reg_properties[0x805 + i * 8],
               TIESC_PERM_READ_ONLY, 1);
        tiesc_memset((void *)&pRegPerm->reg_properties[0x806 + i * 8], TIESC_PERM_RW, 1);
        tiesc_memset((void *)&pRegPerm->reg_properties[0x807 + i * 8],
               TIESC_PERM_READ_ONLY, 1);
    }

    pRegPerm->reg_properties[0x900] = TIESC_PERM_RW;

    for(i = 0; i < 15; i++)
    {
        pRegPerm->reg_properties[0x901 + i] = TIESC_PERM_READ_ONLY;
    }

#ifndef SYSTEM_TIME_PDI_CONTROLLED

    for(i = 0; i < 4; i++)
    {
        pRegPerm->reg_properties[0x910 + i] = TIESC_PERM_RW;
    }

    for(i = 0; i < 4; i++)
    {
        pRegPerm->reg_properties[0x914 + i] = TIESC_PERM_READ_ONLY;
    }

#else

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x910 + i] = TIESC_PERM_READ_ONLY;
    }

#endif

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x918 + i] = TIESC_PERM_READ_ONLY;
    }

#ifndef SYSTEM_TIME_PDI_CONTROLLED

    for(i = 0; i < 12; i++)
    {
        pRegPerm->reg_properties[0x920 + i] = TIESC_PERM_RW;
    }

#else

    for(i = 0; i < 12; i++)
    {
        pRegPerm->reg_properties[0x920 + i] = TIESC_PERM_READ_ONLY;
    }

#endif
    pRegPerm->reg_properties[0x92C] = TIESC_PERM_READ_ONLY; //System Time Difference
    pRegPerm->reg_properties[0x92D] = TIESC_PERM_READ_ONLY; //System Time Difference
    pRegPerm->reg_properties[0x92E] = TIESC_PERM_READ_ONLY; //System Time Difference
    pRegPerm->reg_properties[0x92F] = TIESC_PERM_READ_ONLY; //System Time Difference
#ifndef SYSTEM_TIME_PDI_CONTROLLED
    pRegPerm->reg_properties[0x930] = TIESC_PERM_RW; //Speed counter Start
    pRegPerm->reg_properties[0x931] = TIESC_PERM_RW; //Speed counter Start
#else
    pRegPerm->reg_properties[0x930] = TIESC_PERM_READ_ONLY; //Speed counter Start
    pRegPerm->reg_properties[0x931] = TIESC_PERM_READ_ONLY; //Speed counter Start
#endif
    pRegPerm->reg_properties[0x932] = TIESC_PERM_READ_ONLY; //Speed counter Diff
    pRegPerm->reg_properties[0x933] = TIESC_PERM_READ_ONLY; //Speed counter Diff

#ifndef SYSTEM_TIME_PDI_CONTROLLED
    pRegPerm->reg_properties[0x934] =
        TIESC_PERM_RW; //System Time Difference Filter Depth
    pRegPerm->reg_properties[0x935] = TIESC_PERM_RW; //Speed counter Filter Depth
#else
    pRegPerm->reg_properties[0x934] =
        TIESC_PERM_READ_ONLY; //System Time Difference Filter Depth
    pRegPerm->reg_properties[0x935] =
        TIESC_PERM_READ_ONLY; //Speed counter Filter Depth
#endif
    pRegPerm->reg_properties[0x980] = TIESC_PERM_RW;
#ifndef SYSTEM_TIME_PDI_CONTROLLED
    pRegPerm->reg_properties[0x981] = TIESC_PERM_RW; //Sync Activation
#else
    pRegPerm->reg_properties[0x981] = TIESC_PERM_READ_ONLY; //Sync Activation
#endif

    pRegPerm->reg_properties[0x982] =
        TIESC_PERM_READ_ONLY; //Pulse length of Sync Signals
    pRegPerm->reg_properties[0x983] =
        TIESC_PERM_READ_ONLY; //Pulse length of Sync Signals
    pRegPerm->reg_properties[0x98E] = TIESC_PERM_READ_ONLY; //SYNC0 status
    pRegPerm->reg_properties[0x98F] = TIESC_PERM_READ_ONLY; //SYNC1 status

#ifndef SYSTEM_TIME_PDI_CONTROLLED

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x990 + i] = TIESC_PERM_RW;
    }

#else

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x990 + i] = TIESC_PERM_READ_ONLY;
    }

#endif

    for(i = 0; i < 8; i++)
    {
        pRegPerm->reg_properties[0x998 + i] = TIESC_PERM_READ_ONLY;
    }

#ifndef SYSTEM_TIME_PDI_CONTROLLED

    for(i = 0; i < 10; i++)
    {
        pRegPerm->reg_properties[0x9A0 + i] = TIESC_PERM_RW;
    }

#else

    for(i = 0; i < 10; i++)
    {
        pRegPerm->reg_properties[0x9A0 + i] = TIESC_PERM_READ_ONLY;
    }

#endif

    for(i = 0; i < 34; i++)
    {
        pRegPerm->reg_properties[i + 0x9AE] = TIESC_PERM_READ_ONLY;
    }

    //TI vendor specific registers
    for(i = 0; i < 16; i++)
    {
        pRegPerm->reg_properties[0xE00 + i] = TIESC_PERM_READ_ONLY;
    }

    //TI vendor specific registers
    for(i = 0; i < 22; i++)
    {
        pRegPerm->reg_properties[0xE10 + i] = TIESC_PERM_RW;
    }

    pRegPerm->reg_properties[0xEE0] = TIESC_PERM_RW; //APP_RELOAD_FLAG_REG

#ifdef ENABLE_PDI_REG_PERMISSIONS
    tiesc_memset(&pdi_reg_perm_array[0], 0, 128);

    for(i = 0; i < 10; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array, i); //PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x10));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x12));// PDI_PERM_RW;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x100));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x108));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x110));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x120));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x130));// PDI_PERM_RW;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x134));// PDI_PERM_RW;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x140));// PDI_PERM_RW;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array,
                                    (i + 0x150));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x200));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x204));// PDI_PERM_RW;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x210));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x220));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 13; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x300));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x310));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x400));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x410));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x420));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x440));// PDI_PERM_READ_ONLY;
    }

    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x500);// PDI_PERM_READ_ONLY;

    for(i = 0; i < 15; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x501));// PDI_PERM_RW;
    }

    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x510);// PDI_PERM_READ_ONLY;

    for(i = 0; i < 6; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x511));// PDI_PERM_RW;
    }

    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x516);// PDI_PERM_READ_ONLY;
    bsp_set_pdi_perm_read_write(pdi_reg_perm_array, 0x517);// PDI_PERM_RW;

    for(i = 0; i < 8; i++)    //8 FMMUs
    {
        for(j = 0; j < 16; j++)
        {
            bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                       (0x600 + i * 16 + j));    //PDI_PERM_READ_ONLY
        }
    }

    for(i = 0; i < 8; i++)    //8 SMs
    {
        for(j = 0; j < 7; j++)
        {
            bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                       (0x800 + i * 8 + j));    //PDI_PERM_READ_ONLY
        }

        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (0x807 + i * 8)); //PDI_PERM_RW
    }

#ifndef SYSTEM_TIME_PDI_CONTROLLED

    for(i = 0; i < 54; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x900));// PDI_PERM_READ_ONLY;
    }

#else

    for(i = 0; i < 16; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x900));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 8; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x910));// PDI_PERM_RW;
    }

    for(i = 0; i < 8; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x918));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 12; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x920));// PDI_PERM_RW;
    }

    for(i = 0; i < 4; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x92c));// PDI_PERM_READ_ONLY;
    }

    bsp_set_pdi_perm_read_write(pdi_reg_perm_array, 0x930);// PDI_PERM_RW;
    bsp_set_pdi_perm_read_write(pdi_reg_perm_array, 0x931);// PDI_PERM_RW;
    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x932);// PDI_PERM_READ_ONLY;
    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x933);// PDI_PERM_READ_ONLY;
    bsp_set_pdi_perm_read_write(pdi_reg_perm_array, 0x934);// PDI_PERM_RW;
    bsp_set_pdi_perm_read_write(pdi_reg_perm_array, 0x935);// PDI_PERM_RW;
#endif

    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x981);// PDI_PERM_READ_ONLY;

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0x982));// PDI_PERM_RW;
    }

    bsp_set_pdi_perm_read_only(pdi_reg_perm_array, 0x984);// PDI_PERM_READ_ONLY;

    for(i = 0; i < 2; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x98E));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 26; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x990));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 34; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0x9AE));// PDI_PERM_READ_ONLY;
    }

    //TI vendor specific registers
    for(i = 0; i < 8; i++)
    {
        bsp_set_pdi_perm_read_only(pdi_reg_perm_array,
                                   (i + 0xE00));// PDI_PERM_READ_ONLY;
    }

    for(i = 0; i < 30; i++)
    {
        bsp_set_pdi_perm_read_write(pdi_reg_perm_array, (i + 0xE08));// PDI_PERM_RW;
    }

#endif
    ASSERT_DMB();


    bsp_esc_sync_reg_perm_update(pruIcssHandle);
}


void bsp_esc_sync_reg_perm_update(PRUICSS_Handle pruIcssHandle)
{
    uint16_t register_properties_base_offset = sizeof(((t_register_properties *)
            0)->reserved);

    uint8_t *pRegPermUpdateAddr = (uint8_t *)
                                  ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
                                   PRU_ICSS_DATARAM(0)) + SYNC_PERMISSION_UPDATE_ADDR_OFFSET;

    /*  Register System Time */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_SYSTEMTIME_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x04;
    pRegPermUpdateAddr += 2;

    /*  Register System Time Offset */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_SYSTEMTIME_OFFSET_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x04;
    pRegPermUpdateAddr += 2;

    /*  Register Speed Counter Start */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_SPEED_COUNTER_START_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x06;
    pRegPermUpdateAddr += 2;

    /*  Register SYNC Activation register */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_DC_SYNC_ACTIVATION_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x01;
    pRegPermUpdateAddr += 2;

    /*  Register Start Time Cyclic Operation */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_DC_START_TIME_CYCLIC_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x08;
    pRegPermUpdateAddr += 2;

    /*  Register SYNCx Cycle Time */
    HWREGH(pRegPermUpdateAddr) = (uint16_t) ESC_DC_SYNC0_CYCLETIME_OFFSET +
                                 register_properties_base_offset;
    pRegPermUpdateAddr += 2;
    HWREGH(pRegPermUpdateAddr) = (uint16_t) 0x0A;
    pRegPermUpdateAddr += 2;




    pRegPermUpdateAddr = (uint8_t *)
                         ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
                          PRU_ICSS_DATARAM(0)) + SYNC_PERMISSION_UPDATE_ECAT_OFFSET;

    tiesc_memset(pRegPermUpdateAddr, TIESC_PERM_RW, SYNC_PERMISSION_UPDATE_ECAT_SIZE);

    pRegPermUpdateAddr = (uint8_t *)
                         ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
                          PRU_ICSS_DATARAM(0)) + SYNC_PERMISSION_UPDATE_PDI_OFFSET;

    tiesc_memset(pRegPermUpdateAddr, TIESC_PERM_READ_ONLY,
           SYNC_PERMISSION_UPDATE_PDI_SIZE);
    ASSERT_DMB();

}


Int16 bsp_pruss_mdio_phy_read(PRUICSS_Handle pruIcssHandle, uint8_t phyaddr,
                              uint8_t regoffset, uint16_t *regval)
{
    if((bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_ECAT_ACCESS) & 0x1)
            || (bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_PDI_ACCESS) & 0x2))
    {
        return -1;
    }

    //Acquire PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, bsp_read_byte(pruIcssHandle,
                   ESC_ADDR_MI_PDI_ACCESS) | 1, ESC_ADDR_MI_PDI_ACCESS);
    CSL_MDIO_phyRegRead((((PRUICSS_HwAttrs *)(
                              pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phyaddr, regoffset, regval);
    //Release PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, bsp_read_byte(pruIcssHandle,
                   ESC_ADDR_MI_PDI_ACCESS) & ~1, ESC_ADDR_MI_PDI_ACCESS);
    return 0;
}

Int16 bsp_pruss_mdio_phy_write(PRUICSS_Handle pruIcssHandle, uint8_t phyaddr,
                               uint8_t regoffset, uint16_t regval)
{
    if((bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_ECAT_ACCESS) & 0x1)
            || (bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_PDI_ACCESS) & 0x2))
    {
        return -1;
    }

    //Acquire PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, bsp_read_byte(pruIcssHandle,
                   ESC_ADDR_MI_PDI_ACCESS) | 1, ESC_ADDR_MI_PDI_ACCESS);
    CSL_MDIO_phyRegWrite((((PRUICSS_HwAttrs *)(
                               pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phyaddr, regoffset, regval);
    //Release PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, bsp_read_byte(pruIcssHandle,
                   ESC_ADDR_MI_PDI_ACCESS) & ~1, ESC_ADDR_MI_PDI_ACCESS);
    return 0;
}

/**MDIO register value to select MLINk mode of Link detection*/
#define MDIO_LINKSEL_ENABLE                 1
/**MDIO register value to select MDIO mode of Link detection*/
#define MDIO_LINKSEL_DISABLE                0

#ifndef iceAMIC11x
/**
* @internal
* @brief Function to write into MDIOUSERPHYSEL0/MDIOUSERPHYSEL1 TODO Temp- PDK doesn't have this API
*
* @param baseAddr   [IN] MDIO Base Address
* @param phyinst    [IN] MDIOUSERPHYSEL0/MDIOUSERPHYSEL1
* @param val        [IN] value to be written
*
*  @retval none
*/
void MDIO_userPhySel(unsigned int baseAddr, unsigned int phyinst,
                     unsigned int val)
{
    HWREG(baseAddr + CSL_MDIO_USER_PHY_SEL_REG(0) + (phyinst * 8)) = val;
}

/**
* @brief Function to enable the MDIO Link interrupt TODO Temp- PDK doesn't have this API
*
*       Used to enable MDIO Link Interrupt.
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyInst            [IN] O/1 to select the User phy select option
* @param phyNum             [IN] Phy address of the port
* @param linkSel            [IN] Flag to select to use MDIO mode or MLINK mode
*
*  @retval none
*/
void MDIO_enableLinkInterrupt(uint32_t mdioBaseAddress, uint32_t phyInst,
                              uint32_t phyNum, uint8_t linkSel)
{
    uint32_t PhySel;

    PhySel = phyNum;
    PhySel |=  0x40;

    if(MDIO_LINKSEL_ENABLE == linkSel)
    {
        PhySel |= 0x80;
    }

    MDIO_userPhySel(mdioBaseAddress, phyInst, PhySel);
}
#else
extern void my_MDIO_userPhySel(unsigned int baseAddr, unsigned int phyinst,
                               unsigned int val);
#endif /*iceAMIC11x*/

#define TLKPHY_AUTOMDIX_ENABLE                       (1u<<15)
#define TLKPHY_FORCEMDIX_ENABLE                      (1u<<14)

Int16 bsp_pruss_mdio_init(PRUICSS_Handle pruIcssHandle,
                          t_mdio_params *pmdio_params)
{
    if((bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_ECAT_ACCESS) & 0x1)
            || (bsp_read_byte(pruIcssHandle, ESC_ADDR_MI_PDI_ACCESS) & 0x2))
    {
        return -1;
    }

    //Acquire PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, bsp_read_byte(pruIcssHandle,
                   ESC_ADDR_MI_PDI_ACCESS) | 1, ESC_ADDR_MI_PDI_ACCESS);
    //Configure MDIO clock = 200/(clkdiv+1)
    // Disable preamble , enable fault detection

    //For AM65XX MDIO init is done by board_init. CSL API not available
#ifndef SOC_AM65XX
    CSL_MDIO_init((((PRUICSS_HwAttrs *)(
                        pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), 200000000, 2200000u);
#endif

    //Indicate PHY address to firmware via vendor specfic registers
    bsp_write_byte(pruIcssHandle, pmdio_params->addr0, ESC_ADDR_TI_PORT0_PHYADDR);
    bsp_write_byte(pruIcssHandle, pmdio_params->addr1, ESC_ADDR_TI_PORT1_PHYADDR);
    bsp_write_byte(pruIcssHandle, (1 << PDI_ISR_EDIO_NUM),
                   ESC_ADDR_TI_PDI_ISR_PINSEL);

    bsp_write_dword(pruIcssHandle,
                    (pmdio_params->link0pol << pmdio_params->addr0) |
                    (pmdio_params->link1pol << pmdio_params->addr1),
                    ESC_ADDR_TI_PHY_LINK_POLARITY);
#ifdef iceAMIC11x

    while(!Board_getPhyIdentifyStat((((PRUICSS_HwAttrs *)(
                                          pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr0));

    while(!Board_getPhyIdentifyStat((((PRUICSS_HwAttrs *)(
                                          pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr1));

#endif
    Board_enablePhyAutoMDIX((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr0);
    Board_enablePhyAutoMDIX((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr1);

    /* Turning OFF RMII mode and selecting MII mode */
    Board_enablePhyMII((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr0);
    Board_enablePhyMII((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), pmdio_params->addr1);

    bsp_ethphy_init(pruIcssHandle, pmdio_params->addr0, pmdio_params->addr1,
                    pmdio_params->enhancedlink_enable);

    //Select PHY address enable link change interrupt at MDIOLinkIntMasked and MDIOLinkIntRaw
    if(pmdio_params->enhancedlink_enable == 0)
    {
        MDIO_enableLinkInterrupt((((PRUICSS_HwAttrs *)(
                                       pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), 0, pmdio_params->addr0,
                                 MDIO_LINKSEL_DISABLE);
        MDIO_enableLinkInterrupt((((PRUICSS_HwAttrs *)(
                                       pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), 1, pmdio_params->addr1,
                                 MDIO_LINKSEL_DISABLE);
    }

    else
    {
        MDIO_enableLinkInterrupt((((PRUICSS_HwAttrs *)(
                                       pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), 0, pmdio_params->addr0,
                                 MDIO_LINKSEL_ENABLE);
        MDIO_enableLinkInterrupt((((PRUICSS_HwAttrs *)(
                                       pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), 1, pmdio_params->addr1,
                                 MDIO_LINKSEL_ENABLE);

    }

    //Release PDI access over MDIO/MI interface
    bsp_write_byte(pruIcssHandle, (bsp_read_byte(pruIcssHandle,
                                   ESC_ADDR_MI_PDI_ACCESS) & ~1), ESC_ADDR_MI_PDI_ACCESS);
    return 0;
}

uint32_t bsp_pruss_mdio_phy_link_state(PRUICSS_Handle pruIcssHandle,
                                       uint8_t phyaddr)
{
    volatile uint32_t regval;
    regval = bsp_pruss_mdioreg_read(pruIcssHandle, CSL_ICSSMIIMDIO_LINK);
#if TIESC_MDIO_RX_LINK_ENABLE
    regval ^= bsp_read_dword(pruIcssHandle, ESC_ADDR_TI_PHY_LINK_POLARITY);
#endif
    return (regval & (1 << phyaddr));
}

PRUICSS_IntcInitData pruss1_intc_initdata = PRU_ICSS1_INTC_INITDATA;

void bsp_init(PRUICSS_Handle pruIcssHandle)
{
    SemaphoreP_Params semParams;
#ifndef USE_ECAT_TIMER
    TypesP_FreqHz  frg;
#endif
    t_mdio_params mdioParamsInit;

    // init timer data
    current_low = 0;
    pd_read_addr_err = pd_write_addr_err = 0;
    pdi_read_fail_cnt = pdi_write_fail_cnt = 0;
    prev_low = 0;
#ifndef USE_ECAT_TIMER
    OSAL_getCpuFreq(&frg);
    ecat_timer_inc_p_ms = (frg.lo / 1000);
#endif

#ifndef TIESC_SPI_MASTER_MODE
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));
    volatile t_register_properties *pRegPerm = (volatile t_register_properties *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(1));

#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
    fw_download_flag = 0;
    fw_write_offset = FOE_APPL_BIN_OFFSET;
#endif
    PRUICSS_enableOCPMasterAccess(pruIcssHandle);
    bsp_hwspinlock_init();

#ifndef SOC_AM65XX
    /* set OCP_EN in PRUSS_IEPCLK */
    HW_WR_REG32(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussCfgRegBase +
                CSL_ICSSCFG_IEPCLK,
                1);
#endif

    PRUICSS_pruIntcInit(pruIcssHandle, &pruss1_intc_initdata);

    /* initialize ESC DPRAM pointer microcontroller specific to the beginning of the physical memory of the ESC,
       the macro MAKE_PTR_TO_ESC should be defined in tieschw.h */
    tiesc_memset((void *)sm_properties, 0, sizeof(sm_properties));

    PRUICSS_pruInitMemory(pruIcssHandle, PRU_ICSS_SHARED_RAM);
    PRUICSS_pruInitMemory(pruIcssHandle, PRU_ICSS_DATARAM(0));
    PRUICSS_pruInitMemory(pruIcssHandle, PRU_ICSS_DATARAM(1));
    tiesc_memset((void *)&pRegPerm->reg_properties, 3,
           4 * 1024);  //Init PRU1 data ram
    ASSERT_DMB();


    bsp_pruss_cmd_intfc_write_word(0xFF, &pHost2PruIntfc->cmdlow);

    bsp_write_word(pruIcssHandle, TIESC_PORT0_TX_DELAY,
                   ESC_ADDR_TI_PORT0_TX_START_DELAY);
    bsp_write_word(pruIcssHandle, TIESC_PORT1_TX_DELAY,
                   ESC_ADDR_TI_PORT1_TX_START_DELAY);

    mdioParamsInit.clkdiv = TIESC_MDIO_CLKDIV * 2;

    /* Configuring PHY Addresses PRUICSS_INSTANCE == PRUICSSx_PHY_ADDRESS */
    PRUICSS_V1_Object *object;
    object = (PRUICSS_V1_Object *)pruIcssHandle->object;
    mdioParamsInit.addr0 = Board_getPhyAddress(object->instance, 1);
    mdioParamsInit.addr1 = Board_getPhyAddress(object->instance, 2);

    mdioParamsInit.enhancedlink_enable =
        TIESC_MDIO_RX_LINK_ENABLE;//TIESC_MDIO_RX_LINK_DISABLE;

    if(TIESC_MDIO_RX_LINK_ENABLE == mdioParamsInit.enhancedlink_enable)
    {
        //Enhanced link detection enabled
        mdioParamsInit.link0pol = TIESC_LINK0_POL;
        mdioParamsInit.link1pol = TIESC_LINK1_POL;
    }

    else
    {
        //Enhanced link detection disabled
        mdioParamsInit.link0pol = TIESC_LINK_POL_ACTIVE_HIGH;
        mdioParamsInit.link1pol = TIESC_LINK_POL_ACTIVE_HIGH;
    }

    bsp_pruss_mdio_init(pruIcssHandle, &mdioParamsInit);

#ifdef EDMA_LATENCY_ENHANCEMENT
    EDMA3_DRV_Result edmaResult = EDMA3_DRV_SOK;
    hEdma = edma3init(0, &edmaResult);
    bsp_write_word(pruIcssHandle, 0x01, ESC_ADDR_TI_EDMA_LATENCY_ENHANCEMENT);
#else
    bsp_write_word(pruIcssHandle, 0x00, ESC_ADDR_TI_EDMA_LATENCY_ENHANCEMENT);
#endif /*EDMA_LATENCY_ENHANCEMENT*/

    bsp_esc_reg_perm_init(pruIcssHandle);
    //Trigger PDI WD on  LATCH_IN or every command send to firmware
    bsp_set_pdi_wd_trigger_mode(pruIcssHandle, PDI_WD_TRIGGER_LATCH_IN);
    bsp_set_digio_sw_dataout_enable(pruIcssHandle);
    PRUICSS_pruReset(pruIcssHandle, 0);
    PRUICSS_pruReset(pruIcssHandle, 1);

    ASSERT_DMB();
#ifndef AMIC11X_ONCHIP_MEM_MODE
    PRUICSS_pruDisable(pruIcssHandle, 0);
    PRUICSS_pruDisable(pruIcssHandle, 1);

    /* PRU firmwares are loaded as header files in appliation  */
    PRUICSS_pruWriteMemory(pruIcssHandle, PRU_ICSS_IRAM(0) , 0,
                           (uint32_t *) pru_frame_proc,
                           pru_frame_proc_len);

    PRUICSS_pruWriteMemory(pruIcssHandle, PRU_ICSS_IRAM(1) , 0,
                           (uint32_t *) pru_host_proc,
                           pru_host_proc_len);

    PRUICSS_pruEnable(pruIcssHandle, 1);
    PRUICSS_pruEnable(pruIcssHandle, 0);
#else
    PRUICSS_pruEnable(pruIcssHandle, 1);
    PRUICSS_pruEnable(pruIcssHandle, 0);
#endif

    bsp_eeprom_emulation_init();        //Load eeprom file to memory

    if(bsp_eeprom_load_esc_registers(pruIcssHandle, 0) == -1)
    {
        uint16_t EEPROMReg = 0;
        HW_EscReadWord(EEPROMReg, ESC_ADDR_EEPROM_CTRL);
        EEPROMReg = SWAPWORD(EEPROMReg);
        EEPROMReg |= ESC_EEPROM_ERROR_CRC;
        EEPROMReg = SWAPWORD(EEPROMReg);
        bsp_write_word(pruIcssHandle, EEPROMReg, ESC_ADDR_EEPROM_CTRL);
    }

#endif /* TIESC_SPI_MASTER_MODE */

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    semcmdlow_handle = SemaphoreP_create(1, &semParams);

#ifdef TIESC_SPI_SLAVE_MODE
    PruIcssIntcBaseAddr = (((PRUICSS_HwAttrs *)(
                                pruIcss1Handle->hwAttrs))->prussIntcRegBase);
#endif
}

void bsp_exit(PRUICSS_Handle pruIcssHandle)
{
    SemaphoreP_delete(&semcmdlow_handle);

    bsp_eeprom_emulation_exit();        //Flush the EEPROM cache to file

    PRUICSS_pruDisable(pruIcssHandle, 0);
    PRUICSS_pruDisable(pruIcssHandle, 1);
}


void bsp_start_esc_isr(PRUICSS_Handle pruIcssHandle)
{
    /* enable the ESC-interrupt microcontroller specific,
       the makro ENABLE_ESC_INT should be defined in ecat_def.h */
    // enable RTOS int
    uint32_t evtOutNum;
    uint32_t pruIsrNum = 0;
    uint8_t wait_enable = 0;

#ifndef TIESC_SPI_SLAVE_MODE
#ifdef ENABLE_PDI_TASK
    wait_enable = 1;
#else
    wait_enable = 0;
#endif

    pruIsrNum = HOST_AL_EVENT + getARMInterruptOffset();
    evtOutNum = HOST_AL_EVENT - 20;

#ifdef SOC_K2G
    PRUICSS_IntrCfg pruicss_intr_config;
    pruicss_intr_config.eventId = 0;
    pruicss_intr_config.intNum = pruIsrNum;
    pruicss_intr_config.intcMuxInEvent = 0;
    pruicss_intr_config.intcMuxNum = PRUSS_INVALID_INTC_MUX_NUM;
    pruicss_intr_config.intcMuxOutEvent = 0;
    pruicss_intr_config.irqHandler = &EcatIsr;
    pruicss_intr_config.pruEvtoutNum = evtOutNum;
    pruicss_intr_config.waitEnable = wait_enable;
    PRUICSS_registerIrqHandler2(pruIcssHandle, &pruicss_intr_config);
#else
    PRUICSS_registerIrqHandler(pruIcssHandle,
                               evtOutNum,
                               pruIsrNum,
                               1,
                               wait_enable,
                               &EcatIsr
                              );
#endif
#endif


#ifndef SUPPORT_CMDACK_POLL_MODE
    pruIsrNum = HOST_CMD_LOW_ACK_EVENT + getARMInterruptOffset();
    evtOutNum = HOST_CMD_LOW_ACK_EVENT - 20;

#ifdef SOC_K2G
    pruicss_intr_config.eventId = 0;
    pruicss_intr_config.intNum = pruIsrNum;
    pruicss_intr_config.intcMuxInEvent = 0;
    pruicss_intr_config.intcMuxNum = PRUSS_INVALID_INTC_MUX_NUM;
    pruicss_intr_config.intcMuxOutEvent = 0;
    pruicss_intr_config.irqHandler = &EscCmdLowAckIsr;
    pruicss_intr_config.pruEvtoutNum = evtOutNum;
    pruicss_intr_config.waitEnable = 1;
    PRUICSS_registerIrqHandler2(pruIcssHandle, &pruicss_intr_config);
#else
    PRUICSS_registerIrqHandler(pruIcssHandle,
                               evtOutNum,
                               pruIsrNum,
                               1,
                               1,
                               &EscCmdLowAckIsr);
#endif
#endif

#ifndef TIESC_SPI_SLAVE_MODE
#ifdef ENABLE_SYNC_TASK
    wait_enable = 1;
#else
    wait_enable = 0;
#endif

    pruIsrNum = HOST_SYNC0_EVENT + getARMInterruptOffset();
    evtOutNum = HOST_SYNC0_EVENT - 20;

#ifdef SOC_K2G
    pruicss_intr_config.eventId = 0;
    pruicss_intr_config.intNum = pruIsrNum;
    pruicss_intr_config.intcMuxInEvent = 0;
    pruicss_intr_config.intcMuxNum = PRUSS_INVALID_INTC_MUX_NUM;
    pruicss_intr_config.intcMuxOutEvent = 0;
    pruicss_intr_config.irqHandler = &Sync0Isr;
    pruicss_intr_config.pruEvtoutNum = evtOutNum;
    pruicss_intr_config.waitEnable = wait_enable;
    PRUICSS_registerIrqHandler2(pruIcssHandle, &pruicss_intr_config);
#else
    PRUICSS_registerIrqHandler(pruIcssHandle,
                               evtOutNum,
                               pruIsrNum,
                               1,
                               wait_enable,
                               &Sync0Isr);
#endif

    pruIsrNum = HOST_SYNC1_EVENT + getARMInterruptOffset();
    evtOutNum = HOST_SYNC1_EVENT - 20;

#ifdef SOC_K2G
    pruicss_intr_config.eventId = 0;
    pruicss_intr_config.intNum = pruIsrNum;
    pruicss_intr_config.intcMuxInEvent = 0;
    pruicss_intr_config.intcMuxNum = PRUSS_INVALID_INTC_MUX_NUM;
    pruicss_intr_config.intcMuxOutEvent = 0;
    pruicss_intr_config.irqHandler = &Sync1Isr;
    pruicss_intr_config.pruEvtoutNum = evtOutNum;
    pruicss_intr_config.waitEnable = wait_enable;
    PRUICSS_registerIrqHandler2(pruIcssHandle, &pruicss_intr_config);
#else
    PRUICSS_registerIrqHandler(pruIcssHandle,
                               evtOutNum,
                               pruIsrNum,
                               1,
                               wait_enable,
                               &Sync1Isr);
#endif
#endif

}

void bsp_send_command_to_firmware(PRUICSS_Handle pruIcssHandle,
                                  uint32_t command,
                                  uint16_t param1, uint16_t param2)
{
    uint32_t evtoutNum = 0;
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));
    {
#if defined(ENABLE_PDI_TASK) || defined(ENABLE_PDI_SWI)
        SemaphoreP_pend(semcmdlow_handle, SemaphoreP_WAIT_FOREVER);

#else
        uintptr_t key1 = HwiP_disable();
#endif
        bsp_pruss_cmd_intfc_write_word(command, &pHost2PruIntfc->cmdlow);
        bsp_pruss_cmd_intfc_write_word(param1, &pHost2PruIntfc->param1low);
        bsp_pruss_cmd_intfc_write_word(param2, &pHost2PruIntfc->param2low);
#ifdef SUPPORT_CMDACK_POLL_MODE
        bsp_pruss_cmd_intfc_write_word(1, &pHost2PruIntfc->cmdlow_ack);
#endif
        PRUICSS_pruSendEvent(pruIcss1Handle, ARM_PRU_EVENT1);
        ASSERT_DMB();
        ASSERT_DSB();
        {
#ifdef SUPPORT_CMDACK_POLL_MODE
            volatile uint16_t ack;

            do
            {
                ack = bsp_pruss_cmd_intfc_read_word(&pHost2PruIntfc->cmdlow_ack);
            }
            while(ack);

#else
            evtoutNum = HOST_CMD_LOW_ACK_EVENT - 20;
#if !(defined(ENABLE_PDI_TASK) || defined(ENABLE_PDI_SWI))
            HwiP_restore(key1);
#endif
            PRUICSS_pruWaitEvent((PRUICSS_Handle)pruIcss1Handle, evtoutNum);
#endif
        }
        bsp_pruss_cmd_intfc_write_word(0xFF,    &pHost2PruIntfc->cmdlow);
#if defined(ENABLE_PDI_TASK) || defined(ENABLE_PDI_SWI)
        SemaphoreP_post(semcmdlow_handle);
#endif
    }
}

void bsp_pdi_post_read_indication(PRUICSS_Handle pruIcssHandle,
                                  uint16_t address, uint16_t length)
{

    if((address <= ESC_ADDR_SM_WD_STATUS)
            && ((address + length) > ESC_ADDR_SM_WD_STATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_PD_WD_STATUS, 0,
                                     0);
    }

    else if((address <= ESC_ADDR_SYNC_STATUS)
            && ((address + length) > ESC_ADDR_SYNC_STATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SYNC_STATUS, SYNC0,
                                     0);
    }

    else if((address <= (ESC_ADDR_SYNC_STATUS + 1))
            && ((address + length) > ESC_ADDR_SYNC_STATUS + 1))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SYNC_STATUS, SYNC1,
                                     0);
    }

    else if((address <= ESC_ADDR_ALCONTROL)
            && ((address + length) > ESC_ADDR_ALCONTROL))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_AL_CONTROL, 0, 0);
    }

    else if(((address <= ESC_ADDR_SM0_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM0_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM1_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM1_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM2_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM2_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM3_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM3_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM4_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM4_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM5_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM5_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM6_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM6_ACTIVATE)) ||
            ((address <= ESC_ADDR_SM7_ACTIVATE)
             && ((address + length) > ESC_ADDR_SM7_ACTIVATE)))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SM_ACTIVATE,
                                     (address - ESC_ADDR_SYNCMAN) >> 3, 0);
    }

    else if((address <= ESC_ADDR_LATCH0_POS_EDGE)
            && ((address + length) > ESC_ADDR_LATCH0_POS_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH0_POS_EDGE, 0);
    }

    else if((address <= ESC_ADDR_LATCH0_NEG_EDGE)
            && ((address + length) > ESC_ADDR_LATCH0_NEG_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH0_NEG_EDGE, 0);
    }

    else if((address <= ESC_ADDR_LATCH1_POS_EDGE)
            && ((address + length) > ESC_ADDR_LATCH1_POS_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH1_POS_EDGE, 0);
    }

    else if((address <= ESC_ADDR_LATCH1_NEG_EDGE)
            && ((address + length) > ESC_ADDR_LATCH1_NEG_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH1_NEG_EDGE, 0);
    }

    if(address == sm_properties[MAILBOX_WRITE].physical_start_addr)
    {
        bsp_pdi_mbx_read_start(pruIcssHandle);
    }

}


void bsp_pdi_write_indication(PRUICSS_Handle pruIcssHandle, uint16_t address,
                              uint16_t length, uint16_t value)
{
    uint16_t regval = 0;

    if((address <= ESC_ADDR_ALSTATUS) && ((address + length) > ESC_ADDR_ALSTATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_WRITE_AL_STATUS, 0, 0);
    }

    else if(((address <= ESC_ADDR_SM0_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM0_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM1_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM1_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM2_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM2_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM3_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM3_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM4_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM4_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM5_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM5_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM6_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM6_PDI_CONTROL)) ||
            ((address <= ESC_ADDR_SM7_PDI_CONTROL)
             && ((address + length) > ESC_ADDR_SM7_PDI_CONTROL)))
    {
        uint8_t channel = (address - ESC_ADDR_SYNCMAN) >> 3;
        uint16_t sm_address = ESC_ADDR_SYNCMAN + (channel * SIZEOF_SM_REGISTER);
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_WRITE_SM_PDI_CTRL,
                                     (channel << 8) | value, 0);

        while(bsp_pdi_sm_config_ongoing(pruIcssHandle));

        bsp_set_sm_properties(pruIcssHandle, channel, bsp_read_word(pruIcssHandle,
                              sm_address),
                              bsp_read_word(pruIcssHandle, (sm_address + 2)));
    }

    else if((address <= ESC_ADDR_EEPROM_CTRL)
            && ((address + length) > ESC_ADDR_EEPROM_CTRL))
    {
        /* Note - Here we are assuming that, Command is always two byte  */
        regval = SWAPWORD(value);

        if((regval & ESC_EEPROM_CMD_WRITE_MASK) &&
                !(regval & ESC_EEPROM_ERROR_CMD_ACK))
        {
            bsp_set_eeprom_update_status(1);
            bsp_set_eeprom_updated_time();
        }

        bsp_eeprom_emulation_command_ack(pruIcssHandle);
    }

#ifdef SYSTEM_TIME_PDI_CONTROLLED

    else if((address <= ESC_ADDR_LATCH0_CONTROL)
            && ((address + length) > ESC_ADDR_LATCH0_CONTROL))
    {
        bsp_pdi_latch0_control(pruIcssHandle, (value & 0xFF));
    }

    else if((address <= ESC_ADDR_LATCH1_CONTROL)
            && ((address + length) > ESC_ADDR_LATCH1_CONTROL))
    {
        bsp_pdi_latch1_control(pruIcssHandle, (value & 0xFF));
    }

#endif

    if(address == sm_properties[MAILBOX_READ].physical_start_addr)
    {
        bsp_pdi_mbx_write_start(pruIcssHandle);
    }

}

void bsp_pdi_mbx_read_start(PRUICSS_Handle pruIcssHandle)
{
    uint16_t  ALEvent = HW_GetALEventRegister_Isr();

    if(ALEvent & MBX_WRITE_EVENT)
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_CLEAR_AL_EVENT_LOW,
                                     ~MBX_WRITE_EVENT, 0);
    }
}

void bsp_pdi_mbx_read_complete(PRUICSS_Handle pruIcssHandle)
{
    /* get address of the receive mailbox sync manager */
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_ACK_MBX_READ,
                                 sm_properties[MAILBOX_WRITE].physical_start_addr,
                                 sm_properties[MAILBOX_WRITE].length);
}
void bsp_pdi_mbx_write_start(PRUICSS_Handle pruIcssHandle)
{
    uint16_t  ALEvent = HW_GetALEventRegister_Isr();

    if(ALEvent & MBX_READ_EVENT)
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_CLEAR_AL_EVENT_LOW,
                                     ~MBX_READ_EVENT, 0);
    }
}
void bsp_pdi_mbx_write_complete(PRUICSS_Handle pruIcssHandle)
{
    /* get address of the send mailbox sync manager */
    bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_ACK_MBX_WRITE,
                                 sm_properties[MAILBOX_READ].physical_start_addr,
                                 sm_properties[MAILBOX_READ].length);
}

__inline Int16 bsp_get_sm_index(uint16_t address, uint16_t len)
{
    Int16 sm_index = -1, i;

    for(i = 2 ; i < MAX_SYNC_MAN; i++)
    {
        if((address >= sm_properties[i].physical_start_addr)
                && (address + len <= sm_properties[i].physical_start_addr +
                    sm_properties[i].length))
        {
            sm_index = i;
            break;
        }
    }

    return sm_index;
}

__inline uint16_t bsp_get_process_data_address(PRUICSS_Handle pruIcssHandle,
        uint16_t address, uint16_t len, Int16 *p_sm_index)
{
    uint16_t addr = 0;
    Int16 sm_index = bsp_get_sm_index(address, len);
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));

    //Find corresponding SYNC manager and store index in sm_index
    if(sm_index == -1)
    {
        return 0;
    }

    if(p_sm_index)
    {
        *p_sm_index = sm_index;
    }

    /* called while SM is disabled ?! should never happen; just in case... */
    if(sm_properties[sm_index].physical_start_addr == 0)
    {
        return 0;
    }

    /* are we already accessing this sm ?! in this case "lock_state" will be
     * "LOCK_PD_BUF_HOST_ACCESS_START" and we do not need the "while()"-loop... */
    if(pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state !=
            LOCK_PD_BUF_HOST_ACCESS_START)
    {
        do
        {
            /* May we access the buffer (LOCK_PD_BUF_AVAILABLE_FOR_HOST)? */
            if(pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state ==
                    LOCK_PD_BUF_AVAILABLE_FOR_HOST)
            {
                pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state =
                    LOCK_PD_BUF_HOST_ACCESS_START;
                ASSERT_DMB();
                break;
            }

            else
            {
#ifndef BARE_METAL
                TaskP_yield();
#else
                break;
#endif //BARE_METAL
            }
        }
        while(1);
    }

    if(pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state ==
            LOCK_PD_BUF_HOST_ACCESS_START)
    {
        addr = pHost2PruIntfc->sm_processdata[sm_index - 2].addr;
        addr = addr + (address - sm_properties[sm_index].physical_start_addr);
    }

    return addr;
}

__inline void bsp_process_data_access_complete(PRUICSS_Handle pruIcssHandle,
        uint16_t address, uint16_t len, Int16 sm_index)
{
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));

    //Find correspodning SYNC manager and store index in sm_index
    if(((sm_index < 0) || (sm_index >= MAX_SYNC_MAN)))
    {
        return;
    }

    if((address >= sm_properties[sm_index].physical_start_addr) &&
            (address + len >= sm_properties[sm_index].  physical_start_addr +
             sm_properties[sm_index].length))
    {
        /* Assumption : sm_index > 1 as first two SMs are Mailbox SMs */
        if(pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state ==
                LOCK_PD_BUF_HOST_ACCESS_START)
        {
            pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state =
                LOCK_PD_BUF_HOST_ACCESS_FINISH;
            ASSERT_DMB();
        }
    }
}

#if defined (ECAT_LIMITED_DEMO) && defined (__TI_ARM__)
void bsp_set_sm_properties(PRUICSS_Handle pruIcssHandle, uint8_t sm,
                                  uint16_t address, uint16_t length)

#else
inline void bsp_set_sm_properties(PRUICSS_Handle pruIcssHandle, uint8_t sm,
                                  uint16_t address, uint16_t length)
#endif
{
    sm_properties[sm].physical_start_addr = address;
    sm_properties[sm].length = length;

#ifdef EDMA_LATENCY_ENHANCEMENT

    uint16_t sm_ctrl_address = ESC_ADDR_SYNCMAN + (sm * SIZEOF_SM_REGISTER) + 0x4;
    uint8_t sm_ctrl_val = bsp_read_byte(pruIcssHandle, sm_ctrl_address);

    if((sm_ctrl_val & 0x03) !=
            SM_BUFFERED) /*check if syncmanager is buffered or mailbox*/
    {
        return;
    }

    if((sm_dma_properties[sm].sm != sm) && (length != 0))
    {
        sm_dma_properties[sm].pru_buf_addr[0] = ((((PRUICSS_HwAttrs *)(
                pruIcssHandle->hwAttrs))->baseAddr) + PRU_ICSS_SHARED_RAM) + address;
        sm_dma_properties[sm].pru_buf_addr[1] = ((((PRUICSS_HwAttrs *)(
                pruIcssHandle->hwAttrs))->baseAddr) + PRU_ICSS_SHARED_RAM) + address + length;

        sm_dma_properties[sm].sm = sm;
        sm_dma_properties[sm].sm_length = length;

        sm_dma_properties[sm].ready_buf_index = 0;

        sm_dma_properties[sm].sm_direction = (sm_ctrl_val >> 2) & 0x01;

        if(sm_dma_properties[sm].sm_direction == SM_DIRECTION_READ)
        {
            sm_dma_properties[sm].ddr_buf_addr[0] = sm_read_dma_malloc(
                    EDMA3_CACHE_LINE_SIZE_IN_BYTES, length);
            sm_dma_properties[sm].ddr_buf_addr[1] = sm_read_dma_malloc(
                    EDMA3_CACHE_LINE_SIZE_IN_BYTES, length);
        }

        else
        {
            sm_dma_properties[sm].ddr_buf_addr[0] = sm_write_dma_malloc(
                    EDMA3_CACHE_LINE_SIZE_IN_BYTES, length);
            sm_dma_properties[sm].ddr_buf_addr[1] = sm_write_dma_malloc(
                    EDMA3_CACHE_LINE_SIZE_IN_BYTES, length);
        }

        sm_dma_properties[sm].sm_dma_pru_handle = (t_sm_dma_pru_struct *)
                (((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) + PRU_ICSS_DATARAM(
                      0))
                 + SM_DMA_PRU_STRUCT_OFFSET + sm * SM_DMA_PRU_STRUCT_SIZE);

        bsp_init_sm_dma(pruIcssHandle, hEdma, &sm_dma_properties[sm]);

        t_sm_dma_pru_struct sm_dma_pru_properties;

        if(sm_dma_properties[sm].sm_direction == SM_DIRECTION_READ)
        {
            sm_dma_pru_properties.edma_param_dst_addr = ((uint32_t)
                    sm_dma_properties[sm].dma_param_handle) + 0xC;
        }

        else
        {
            sm_dma_pru_properties.edma_param_dst_addr = ((uint32_t)
                    sm_dma_properties[sm].dma_param_handle) + 0x4;
        }

        sm_dma_pru_properties.pru_counter = 0;
        sm_dma_pru_properties.arm_counter = 0;

        tiesc_memcpy((void *) sm_dma_properties[sm].sm_dma_pru_handle, &sm_dma_pru_properties,
               sizeof(t_sm_dma_pru_struct));
    }

#endif /*EDMA_LATENCY_ENHANCEMENT*/
}

inline t_sm_properties *bsp_get_sm_properties(uint8_t sm)
{
    return &sm_properties[sm];
}

/** @addtogroup ECAT_ESC_REG_ACCESS
 @{ */

inline uint32_t bsp_get_pdi_read_access_fail_cnt()
{
    return pdi_read_fail_cnt;
}

inline uint32_t bsp_get_pdi_write_access_fail_cnt()
{
    return pdi_write_fail_cnt;
}
/**
@}
*/

inline void bsp_set_pdi_perm_read_only(uint16_t *perm_array, uint16_t address)
{
    *(perm_array + (address >> 4)) |= (1 << (address & 0x0F));
}

inline void bsp_set_pdi_perm_read_write(uint16_t *perm_array, uint16_t address)
{
    *(perm_array + (address >> 4)) &= ~(1 << (address & 0x0F));
}

inline uint8_t bsp_is_pdi_perm_read_only(uint16_t *perm_array, uint16_t address)
{
    uint16_t i, pos;

    i = address >> 4;
    pos = 1 << (address & 0x0F);

    if(*(perm_array + i) & pos)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

inline uint8_t  bsp_get_pdi_access_perm(uint16_t address, uint8_t access)
{
#ifdef ENABLE_PDI_REG_PERMISSIONS
    uint8_t retval = 1;

    if(address < 4096)
    {
        retval = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, address);
        retval = !(retval & !access);
    }

    return retval;
#else
    return 1;
#endif
}

inline uint8_t  bsp_pdi_access_perm_word(uint16_t address, uint8_t access)
{
#ifdef ENABLE_PDI_REG_PERMISSIONS
    uint8_t retval = 1;

    if((address < 4095) && (address + 1 < 4096))
    {
        uint8_t t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, address);
        retval = !(t_perm & !access);

        t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, (address + 1));
        retval &= !(t_perm & !access);
    }

    return retval;
#else
    return 1;
#endif

}

inline uint8_t  bsp_pdi_access_perm_dword(uint16_t address, uint8_t access)
{
#ifdef ENABLE_PDI_REG_PERMISSIONS
    uint8_t retval = 1;

    if((address < 4093) && (address + 3 < 4096))
    {
        uint8_t t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, address);
        retval = !(t_perm & !access);

        t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, (address + 1));
        retval &= !(t_perm & !access);

        t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, (address + 2));
        retval &= !(t_perm & !access);

        t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, (address + 3));
        retval &= !(t_perm & !access);
    }

    return retval;
#else
    return 1;
#endif


}

inline uint8_t  bsp_pdi_access_perm_array(uint16_t address, uint8_t access,
        uint16_t size)
{
#ifdef ENABLE_PDI_REG_PERMISSIONS
    uint8_t retval = 1;

    if((address < 4096 - size) && ((address + size - 1) < 4096))
    {
        uint16_t itr = 1;
        uint8_t t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, address);

        retval = !(t_perm & !access);

        for(; itr < size ; itr++)
        {
            t_perm = bsp_is_pdi_perm_read_only(pdi_reg_perm_array, (address + itr));
            retval &= !(t_perm & !access);
        }
    }

    return retval;
#else
    return 1;
#endif

}

uint32_t bsp_read_dword(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint32_t DWordValue;
#ifndef TIESC_SPI_MASTER_MODE
    uint32_t end_addr = sm_properties[MAILBOX_WRITE].physical_start_addr +
                        sm_properties[MAILBOX_WRITE].length;

    if(0 == bsp_pdi_access_perm_dword(address, PDI_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }

    DWordValue = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                             address) >> 2)]);

    if((address >= (end_addr - 4)) &&
            (address < end_addr))
    {
        bsp_pdi_mbx_read_complete(pruIcssHandle);
    }

    else
    {
        bsp_pdi_post_read_indication(pruIcssHandle, address, sizeof(uint32_t));
    }

#else
    spi_master_bsp_read(address, (uint8_t *)&DWordValue, sizeof(uint32_t));
#endif
    return DWordValue;
}

uint32_t bsp_read_dword_isr(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint32_t DWordValue;
#ifndef TIESC_SPI_MASTER_MODE
    DWordValue = (((uint32_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                             address) >> 2)]);
#else
    spi_master_bsp_read(address, (uint8_t *)&DWordValue, sizeof(uint32_t));
#endif
    return DWordValue;
}

uint16_t bsp_read_word(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint16_t WordValue;
#ifndef TIESC_SPI_MASTER_MODE
    uint16_t end_addr = sm_properties[MAILBOX_WRITE].physical_start_addr +
                        sm_properties[MAILBOX_WRITE].length;

    if(0 == bsp_pdi_access_perm_word(address, PDI_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }

    WordValue = (((uint16_t *)(((PRUICSS_HwAttrs *)(
                                    pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                            address) >> 1)]);

    if((address >= (end_addr - 2)) &&
            (address < end_addr))
    {
        bsp_pdi_mbx_read_complete(pruIcssHandle);
    }

    else
    {
        bsp_pdi_post_read_indication(pruIcssHandle, address, sizeof(uint16_t));
    }

#else
    spi_master_bsp_read(address, (uint8_t *)&WordValue, sizeof(uint16_t));
#endif
    return WordValue;
}

uint16_t bsp_read_word_isr(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint16_t WordValue;
#ifndef TIESC_SPI_MASTER_MODE
    WordValue = (((uint16_t *)(((PRUICSS_HwAttrs *)(
                                    pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM))[((
                                            address) >> 1)]);
#else
    spi_master_bsp_read(address, (uint8_t *)&WordValue, sizeof(uint16_t));
#endif
    return WordValue;
}

uint8_t bsp_read_byte(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint8_t ByteValue;

#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_get_pdi_access_perm(address, PDI_PERM_READ))
    {
        pdi_read_fail_cnt++;
        return 0;
    }

    uint8_t *pEsc = (uint8_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM);
    ByteValue = pEsc[address];

    if(address == (sm_properties[MAILBOX_WRITE].physical_start_addr +
                   sm_properties[MAILBOX_WRITE].length - 1))
    {
        bsp_pdi_mbx_read_complete(pruIcssHandle);
    }

    else
    {
        bsp_pdi_post_read_indication(pruIcssHandle, address, sizeof(uint8_t));
    }

#else
    spi_master_bsp_read(address, &ByteValue, sizeof(uint8_t));
#endif
    return ByteValue;
}

inline uint8_t bsp_read_byte_isr(PRUICSS_Handle pruIcssHandle, uint16_t address)
{
    uint8_t ByteValue;
#ifndef TIESC_SPI_MASTER_MODE
    uint8_t *pEsc = (uint8_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM);
    ByteValue = pEsc[address];
#else
    spi_master_bsp_read(address, &ByteValue, sizeof(uint8_t));
#endif
    return ByteValue;
}

void bsp_read(PRUICSS_Handle pruIcssHandle, uint8_t *pdata, uint16_t address,
              uint16_t len)
{
#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_pdi_access_perm_array(address, PDI_PERM_READ, len))
    {
        pdi_read_fail_cnt++;
        return;
    }

    uint8_t *pEsc = (uint8_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM);
    tiesc_memcpy(pdata, &pEsc[address], len);
#else
    spi_master_bsp_read(address, pdata, len);
#endif
}
void bsp_write_dword(PRUICSS_Handle pruIcssHandle, uint32_t val,
                     uint16_t address)
{
#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_pdi_access_perm_dword(address, PDI_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

    (((uint32_t *)(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr
                   + PRU_ICSS_SHARED_RAM))[((address) >> 2)]) = val;
    ASSERT_DMB();
#else
    spi_master_bsp_write(address, (uint8_t *)&val, 4);
#endif
}
void bsp_write_word(PRUICSS_Handle pruIcssHandle, uint16_t val,
                    uint16_t address)
{
#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_pdi_access_perm_word(address, PDI_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

    (((uint16_t *)(((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr
                   + PRU_ICSS_SHARED_RAM))[((address) >> 1)]) = val;
    ASSERT_DMB();
#else
    spi_master_bsp_write(address, (uint8_t *)&val, 2);
#endif
}
void bsp_write_byte(PRUICSS_Handle pruIcssHandle, uint8_t val, uint16_t address)
{
#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_get_pdi_access_perm(address, PDI_PERM_WRITE))
    {
        pdi_write_fail_cnt++;
        return;
    }

    uint8_t *pEsc = (uint8_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM);
    pEsc[address] = val;
    ASSERT_DMB();
#else
    spi_master_bsp_write(address, (uint8_t *)&val, 1);
#endif
}
void bsp_write(PRUICSS_Handle pruIcssHandle, uint8_t *pdata, uint16_t address,
               uint16_t len)
{
#ifndef TIESC_SPI_MASTER_MODE

    if(0 == bsp_pdi_access_perm_array(address, PDI_PERM_WRITE, len))
    {
        pdi_write_fail_cnt++;
        return;
    }

    uint8_t *pEsc = (uint8_t *)(((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM);
    tiesc_memcpy(&pEsc[address], pdata, len);
    ASSERT_DMB();

#else
    spi_master_bsp_write(address, pdata, len);
#endif
}
inline uint32_t bsp_pruss_mdioreg_read(PRUICSS_Handle pruIcssHandle,
                                       uint32_t regoffset)
{
    uint32_t regval;
    regval = HWREG((uint32_t)(((PRUICSS_HwAttrs *)(
                                   pruIcssHandle->hwAttrs))->prussMiiMdioRegBase) + regoffset);
    return regval;
}
inline void bsp_pruss_mdioreg_write(PRUICSS_Handle pruIcssHandle, uint32_t val,
                                    uint32_t regoffset)
{
    HWREG((uint32_t)(((PRUICSS_HwAttrs *)(
                          pruIcssHandle->hwAttrs))->prussMiiMdioRegBase) + regoffset) = val;
    ASSERT_DMB();
}
uint32_t bsp_pruss_iepreg_read(PRUICSS_Handle pruIcssHandle, uint32_t regoffset)
{
    uint32_t regval;
    regval = HWREG((uint32_t)((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussIepRegBase + regoffset);
    return regval;
}
inline void bsp_pruss_iepreg_write(PRUICSS_Handle pruIcssHandle, uint32_t val,
                                   uint32_t regoffset)
{
    HWREG((uint32_t)((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->prussIepRegBase +
          regoffset) = val;
    ASSERT_DMB();
}
inline uint16_t bsp_pruss_cmd_intfc_read_word(volatile uint16_t *ptr)
{
    uint16_t val;
    val = *ptr;
    return val;
}
inline void bsp_pruss_cmd_intfc_write_word(uint16_t val, volatile uint16_t *ptr)
{
    *ptr = val;
    ASSERT_DMB();
}

inline uint8_t bsp_pdi_sm_config_ongoing(PRUICSS_Handle pruIcssHandle)
{
    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcssHandle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));
    return pHost2PruIntfc->sm_config_ongoing;
}
inline void bsp_hwspinlock_init(void)
{
    uint32_t regval;
#if !defined(SOC_K2G) && !defined(SOC_AM65XX)
    HWREG(getSpinlockClkCtrlOffset()) =
        2; //Enable spinlock clock in CM_PER_SPINLOCK_CLKCTRL
#elif defined(SOC_AM65XX)
    initSpinlock();
#endif


    for(regval = 0; regval < 8; regval++)
    {
        bsp_hwspinlock_unlock(regval);
    }

}

inline uint32_t bsp_hwspinlock_lock(int num)
{
    if(HWREG(getSpinlockLockReg0Offset() + (num <<
                                            2)) == SPINLOCK_GRANTED)
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

inline void bsp_hwspinlock_unlock(int num)
{
    HWREG(getSpinlockLockReg0Offset() + (num << 2)) = SPINLOCK_UNLOCK;
}

void bsp_global_mutex_lock(void)
{
    //escGlobalIArg = GateAll_enter (escGlobalGateHandle);
    //Disable PDI and SYNC0 ISR at ARM INTC (rather than global IRQ disable)
    HwiP_disableInterrupt(HOST_AL_EVENT + getARMInterruptOffset());
    HwiP_disableInterrupt(HOST_SYNC0_EVENT + getARMInterruptOffset());
    HwiP_disableInterrupt(HOST_SYNC1_EVENT + getARMInterruptOffset());
}

void bsp_global_mutex_unlock(void)
{
    //GateAll_leave (escGlobalGateHandle, escGlobalIArg);
    //Enable back PDI and SYNC0 ISR at ARM INTC
    HwiP_enableInterrupt(HOST_AL_EVENT + getARMInterruptOffset());
    HwiP_enableInterrupt(HOST_SYNC0_EVENT + getARMInterruptOffset());
    HwiP_enableInterrupt(HOST_SYNC1_EVENT + getARMInterruptOffset());

}

void bsp_set_eeprom_update_status(uint8_t status)
{
#ifdef EEPROM_SPI
    eeprom_updated = status;
#endif
}

inline uint8_t bsp_get_eeprom_update_status(void)
{
#ifdef EEPROM_SPI
    return eeprom_updated;
#else
    return 0;
#endif
}

inline uint8_t *bsp_get_eeprom_cache_base(void)
{
    return eeprom_cache;
}

inline void bsp_set_eeprom_updated_time()
{
#ifdef EEPROM_SPI
#ifdef USE_ECAT_TIMER
    bsp_get_local_sys_time(&eeprom_updated_time, NULL);
#else
    eeprom_updated_time = Timestamp_get32();
#endif
#endif
}

inline uint32_t bsp_get_eeprom_updated_time()
{
#ifdef EEPROM_SPI
    return eeprom_updated_time;
#else
    return 0;
#endif
}

inline void bsp_set_pru_firmware(uint32_t *frameProc, uint32_t frameProcLen,
                                 uint32_t *hostProc, uint32_t hostProcLen)
{
    pru_frame_proc = frameProc;
    pru_host_proc = hostProc;
    pru_frame_proc_len = frameProcLen;
    pru_host_proc_len = hostProcLen;
}

#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE

void bsp_start_fw_download(uint32_t password)
{
#if !defined (EXEC_FROM_IRAM) && !defined (XIP_NOR)
    fw_download_flag = 1;
#ifndef NO_UART_MSG_APP
    UART_printf("\n\r FW downlaod started");
#endif
#endif
}

void bsp_store_fw_data(uint16_t *pData, uint16_t Size)
{
#if !defined (EXEC_FROM_IRAM) && !defined (XIP_NOR)

    uint32_t itr1 = 0;
    uint8_t *temp_ptr = (uint8_t *)pData;
    uint8_t data_buff[256];
    uint32_t blockNum, pageNum;      /* Block, page number */

    for(itr1 = 0 ; itr1 < Size  ; itr1++)
    {
        write_to_cir_buff(temp_ptr[itr1]);
    }

    while(get_cir_buff_avail_bytes() >= 256)
    {

        //Call Flash write code from here
        if((fw_write_offset & FOE_FLASH_SECTOR_SIZE) == 0)
        {
            //Erase Flash sector
            Board_flashOffsetToBlkPage(boardFlashHandle, fw_write_offset,
                                       &blockNum, &pageNum);

            Board_flashEraseBlk(boardFlashHandle, blockNum);
        }

        if(read_from_cir_buff(data_buff, 256) >= 256)
        {
            uint32_t ioMode;
            ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
            Board_flashWrite(boardFlashHandle, fw_write_offset,
                             data_buff, 256, (void *)(&ioMode));
            fw_write_offset += 256;
        }
    }

#endif

}

void bsp_boot_2_init_handler()
{
#if !defined (EXEC_FROM_IRAM) && !defined (XIP_NOR)

    uint8_t data_buff[256];
    uint32_t blockNum, pageNum;      /* Block, page number */

    /*
     * Make sure that firware is completely written to SPI flash
     */
    while(get_cir_buff_avail_bytes() > 0)
    {


        //Call Flash write code from here
        if((fw_write_offset & 0xFFFF) == 0)
        {
            //Erase Flash sector
            Board_flashOffsetToBlkPage(boardFlashHandle, fw_write_offset,
                                       &blockNum, &pageNum);

            Board_flashEraseBlk(boardFlashHandle, blockNum);
        }

        if(read_from_cir_buff(data_buff, 256) > 0)
        {
            uint32_t ioMode;
            ioMode = BOARD_FLASH_QSPI_IO_MODE_QUAD;
            Board_flashWrite(boardFlashHandle, fw_write_offset,
                             data_buff, 256, (void *)(&ioMode));
            fw_write_offset += 256;
        }
    }

    /*
     * Set FW reload flag from here if a new binary is recieved
     */
    if(fw_download_flag)
    {
#ifndef NO_UART_MSG_APP
        UART_printf("\n\r FW Download completed\n\r");
#endif
        //reset the flag
        fw_download_flag = 0;
        //Set Restart flag
    }

#endif
}

#else
/*
 * when we undefine ENABLE_ONLINE_FIRMWARE_UPGRADE stack library still has
 * it defined and calls some bsp functions. So, we need to implement
 * stubs for them
 */
void bsp_boot_2_init_handler() {}
void bsp_store_fw_data(uint16_t *pData, uint16_t Size) {}
void bsp_start_fw_download(uint32_t password) {}
#endif

#ifdef TIESC_SPI_SLAVE_MODE

volatile uint16_t spi_bsp_read_address = 0;
volatile uint16_t spi_bsp_read_sm_access = 1;
volatile uint16_t spi_bsp_read_sm_state_update = 0;
volatile int16_t spi_bsp_read_sm_index = -1;
volatile t_host_interface *my_pHost2PruIntfc;
volatile uint16_t spi_bsp_write_sm_access = 1;
volatile int16_t spi_bsp_write_sm_index = -1;


void spi_bsp_eeprom_event_handler(void)
{
    uint16_t result = 0;
    uint16_t eeprom_reg = 0;

    HW_EscReadWord(eeprom_reg, ESC_EEPROM_CONTROL_OFFSET);
    eeprom_reg = SWAPWORD(eeprom_reg);

    if(eeprom_reg & ESC_EEPROM_BUSY_MASK)
    {
        uint32_t cmd = eeprom_reg  & ESC_EEPROM_CMD_MASK;
        uint32_t addr;
        HW_EscReadDWord(addr, ESC_EEPROM_ADDRESS_OFFSET);
        addr = SWAPDWORD(addr);

        switch(cmd)
        {
            case 0x00:
                //Clear error bits
                eeprom_reg &= ~(ESC_EEPROM_ERROR_MASK);
                break;

            case ESC_EEPROM_CMD_READ_MASK:
                {
                    if(addr <= ESC_EEPROM_SIZE && (pEEPROM != NULL))
                    {
                        uint16_t *p_data = (uint16_t *)pEEPROM;
                        HW_EscWrite((MEM_ADDR *)&p_data[(addr)], ESC_EEPROM_DATA_OFFSET,
                                    EEPROM_READ_SIZE);
                        //Clear error bits
                        eeprom_reg &= ~(ESC_EEPROM_ERROR_MASK);
                    }

                    else
                    {
                        //Set Error
                        eeprom_reg |= ESC_EEPROM_ERROR_CMD_ACK;
                    }
                }
                break;

            case ESC_EEPROM_CMD_WRITE_MASK:
                {
                    if(addr <= ESC_EEPROM_SIZE && (pEEPROM != NULL) && (eeprom_reg & 1))
                    {
                        uint16_t *p_data = (uint16_t *)pEEPROM;
                        HW_EscRead((MEM_ADDR *)&p_data[(addr)], ESC_EEPROM_DATA_OFFSET,
                                   EEPROM_WRITE_SIZE);
                        //Clear error bits
                        eeprom_reg &= ~(ESC_EEPROM_ERROR_MASK);
                    }

                    else
                    {
                        //Set Error
                        eeprom_reg |= ESC_EEPROM_ERROR_CMD_ACK;
                    }
                }
                break;

            case ESC_EEPROM_CMD_RELOAD_MASK:
                {
                    result = HW_EepromReload();

                    if(result != 0)
                    {
                        //copy the configured station alias
                        HW_EscWriteWord(((uint16_t *)pEEPROM)[0x4], ESC_EEPROM_DATA_OFFSET);
                        eeprom_reg |= ESC_EEPROM_ERROR_CRC;
                    }
                }
                break;

            default:
                eeprom_reg |= ESC_EEPROM_ERROR_CMD_ACK;
                break;
        }

        uint16_t tmp_data = SWAPWORD(eeprom_reg);
        HW_EscWriteWord(tmp_data, ESC_EEPROM_CONTROL_OFFSET);
    }
}

uint8_t spi_bsp_slave_read_init(void)
{
    my_pHost2PruIntfc = (volatile t_host_interface *)
                        ((((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->baseAddr) +
                         PRU_ICSS_DATARAM(0));
    return 0;
}



uint8_t spi_bsp_slave_read_start(uint16_t address, uint8_t *read_buf)
{
    if(address < ESC_ADDR_MEMORY)
    {
        //Register Access
        tiesc_memcpy(read_buf, (uint8_t *)(((PRUICSS_HwAttrs *)(
                                          pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address), 4);
        spi_bsp_read_address = address + 4;
    }

    else
    {
        //Process Data Access
        Int16 sm_index = bsp_get_sm_index(address, 1);
        spi_bsp_read_sm_index = sm_index;

        //Find corresponding SYNC manager and store index in sm_index
        if(sm_index != -1)
        {
            t_sm_processdata curr_processdata = my_pHost2PruIntfc->sm_processdata[sm_index -
                                                2];

            if(curr_processdata.lock_state != LOCK_PD_BUF_HOST_ACCESS_FINISH
                    && !(sm_properties[sm_index].physical_start_addr != 0
                         && curr_processdata.addr == 0))
            {
                address = curr_processdata.addr + (address -
                                                   sm_properties[sm_index].physical_start_addr);
                tiesc_memcpy(read_buf, (uint8_t *)(((PRUICSS_HwAttrs *)(
                                                  pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address), 4);
                spi_bsp_read_address = address + 4;
                spi_bsp_read_sm_state_update = 1;
                return 0;
            }

            else
            {
                spi_bsp_read_sm_access = 0;
                *(uint32_t *)read_buf = 0;
                return 0;
            }
        }

        tiesc_memcpy(read_buf, (uint8_t *)(((PRUICSS_HwAttrs *)(
                                          pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address), 4);
        spi_bsp_read_address = address + 4;
    }

    return 0;
}


uint8_t spi_bsp_slave_read_next(uint16_t start_address, uint8_t *read_buf,
                                uint16_t num_bytes)
{
    if(spi_bsp_read_sm_access)
    {
        if(spi_bsp_read_sm_state_update)
        {
            my_pHost2PruIntfc->sm_processdata[spi_bsp_read_sm_index - 2].lock_state =
                LOCK_PD_BUF_HOST_ACCESS_START;
            spi_bsp_read_sm_state_update = 0;
        }

        tiesc_memcpy(read_buf, (uint8_t *)(((PRUICSS_HwAttrs *)(
                                          pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM +
                                     spi_bsp_read_address), num_bytes);
        spi_bsp_read_address += num_bytes;
    }

    else
    {
        tiesc_memset(read_buf, 0, num_bytes);
    }

    return 0;
}


uint8_t spi_bsp_slave_read_end(uint16_t start_address, uint16_t length)
{
    if(start_address < ESC_ADDR_MEMORY)
    {
        //Register Access
        //        bsp_pdi_post_read_indication(pruIcss1Handle, start_address);
        bsp_spi_pdi_post_read_indication(pruIcss1Handle, start_address, length);
    }

    else
    {
        if(spi_bsp_read_sm_access)
        {
            //Process Data Access
            uint16_t mbx_start = sm_properties[MAILBOX_WRITE].physical_start_addr;
            uint16_t mbx_end = mbx_start + sm_properties[MAILBOX_WRITE].length;

            if((start_address >= mbx_start) && (start_address + length <= mbx_end))
            {
                if(start_address == mbx_start)
                {
                    bsp_pdi_mbx_read_start(pruIcss1Handle);
                }

                if((start_address + length) == mbx_end)
                {
                    bsp_pdi_mbx_read_complete(pruIcss1Handle);
                }
            }

            else
            {
                bsp_process_data_access_complete(pruIcss1Handle, start_address, length,
                                                 spi_bsp_read_sm_index);
            }
        }

        //reset flags
        spi_bsp_read_sm_access = 1;
        spi_bsp_read_sm_index = -1;
    }

    spi_bsp_read_address = 0;

    return 0;
}



void bsp_spi_pdi_post_read_indication(PRUICSS_Handle pruIcssHandle,
                                      uint16_t start_address, uint16_t length)
{

    if((start_address <= ESC_ADDR_SM_WD_STATUS)
            && (start_address + length > ESC_ADDR_SM_WD_STATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_PD_WD_STATUS, 0,
                                     0);
    }

    else if((start_address <= ESC_ADDR_ALCONTROL)
            && (start_address + length > ESC_ADDR_ALCONTROL))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_AL_CONTROL, 0,
                                     0);
    }

    else if(((start_address <= ESC_ADDR_SM0_ACTIVATE)
             && (start_address + length > ESC_ADDR_SM0_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM1_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM1_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM2_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM2_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM3_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM3_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM4_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM4_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM5_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM5_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM6_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM6_ACTIVATE))
            || ((start_address <= ESC_ADDR_SM7_ACTIVATE)
                && (start_address + length > ESC_ADDR_SM7_ACTIVATE)))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SM_ACTIVATE,
                                     (start_address - ESC_ADDR_SYNCMAN) >> 3, 0);
    }

    else if((start_address <= ESC_ADDR_LATCH0_POS_EDGE)
            && (start_address + length > ESC_ADDR_LATCH0_POS_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH0_POS_EDGE, 0);
    }

    else if((start_address <= ESC_ADDR_LATCH0_NEG_EDGE)
            && (start_address + length > ESC_ADDR_LATCH0_NEG_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH0_NEG_EDGE, 0);
    }

    else if((start_address <= ESC_ADDR_LATCH1_POS_EDGE)
            && (start_address + length > ESC_ADDR_LATCH1_POS_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH1_POS_EDGE, 0);
    }

    else if((start_address <= ESC_ADDR_LATCH1_NEG_EDGE)
            && (start_address + length > ESC_ADDR_LATCH1_NEG_EDGE))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_LATCH_TIME,
                                     LATCH1_NEG_EDGE, 0);
    }

    else if((start_address <= ESC_ADDR_SYNC_STATUS)
            && (start_address + length > ESC_ADDR_SYNC_STATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SYNC_STATUS,
                                     SYNC0, 0);
    }

    if((start_address <= ESC_ADDR_SYNC_STATUS + 1)
            && (start_address + length > ESC_ADDR_SYNC_STATUS + 1))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_READ_SYNC_STATUS,
                                     SYNC1, 0);
    }


}

void bsp_spi_pdi_write_indication(PRUICSS_Handle pruIcssHandle,
                                  uint16_t start_address, uint16_t length, uint8_t *write_buf)
{
    uint16_t regval = 0;

    if((start_address <= ESC_ADDR_ALSTATUS)
            && (start_address + length > ESC_ADDR_ALSTATUS))
    {
        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_WRITE_AL_STATUS, 0,
                                     0);
    }

    else if(((start_address <= ESC_ADDR_SM0_PDI_CONTROL)
             && (start_address + length > ESC_ADDR_SM0_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM1_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM1_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM2_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM2_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM3_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM3_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM4_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM4_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM5_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM5_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM6_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM6_PDI_CONTROL))
            || ((start_address <= ESC_ADDR_SM7_PDI_CONTROL)
                && (start_address + length > ESC_ADDR_SM7_PDI_CONTROL)))
    {
        uint8_t channel = (start_address - ESC_ADDR_SYNCMAN) >> 3;
        uint16_t sm_address = ESC_ADDR_SYNCMAN + (channel * SIZEOF_SM_REGISTER);
        uint8_t value = write_buf[sm_address + (ESC_ADDR_SM0_PDI_CONTROL -
                                                ESC_ADDR_SYNCMAN) - start_address];

        bsp_send_command_to_firmware(pruIcssHandle, CMD_DL_USER_WRITE_SM_PDI_CTRL,
                                     (channel << 8) | value, 0);

        while(bsp_pdi_sm_config_ongoing(pruIcssHandle));

        bsp_set_sm_properties(pruIcssHandle, channel, bsp_read_word(pruIcssHandle,
                              sm_address),
                              bsp_read_word(pruIcssHandle, (sm_address + 2)));
    }

    else if((start_address <= ESC_ADDR_EEPROM_CTRL)
            && (start_address + length > ESC_ADDR_EEPROM_CTRL))
    {
        uint8_t index = ESC_ADDR_EEPROM_CTRL - start_address;
        uint16_t value = write_buf[index] + (write_buf[index + 1] << 8);

        /* Note - Here we are assuming that, Command is always two byte  */
        regval = SWAPWORD(value);

        if((regval & ESC_EEPROM_CMD_WRITE_MASK) &&
                !(regval & ESC_EEPROM_ERROR_CMD_ACK))
        {
            bsp_set_eeprom_update_status(1);
            bsp_set_eeprom_updated_time();
        }

        bsp_eeprom_emulation_command_ack(pruIcssHandle);
    }

#ifdef SYSTEM_TIME_PDI_CONTROLLED

    else if((start_address <= ESC_ADDR_LATCH0_CONTROL)
            && (start_address + length > ESC_ADDR_LATCH0_CONTROL))
    {
        bsp_pdi_latch0_control(pruIcssHandle, (value & 0xFF));
    }

    else if((start_address <= ESC_ADDR_LATCH1_CONTROL)
            && (start_address + length > ESC_ADDR_LATCH1_CONTROL))
    {
        bsp_pdi_latch1_control(pruIcssHandle, (value & 0xFF));
    }

#endif
}


uint8_t spi_bsp_slave_write_start(uint16_t address, uint32_t *physical_address)
{
    if(address < ESC_ADDR_MEMORY)
    {
        *physical_address = (uint32_t)(((PRUICSS_HwAttrs *)
                                        (pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address);
    }

    else
    {
        //Process Data Access
        Int16 sm_index = bsp_get_sm_index(address, 1);
        spi_bsp_write_sm_index = sm_index;

        //Find corresponding SYNC manager and store index in sm_index
        if(sm_index != -1)
        {
            t_sm_processdata curr_processdata = my_pHost2PruIntfc->sm_processdata[sm_index -
                                                2];

            if(curr_processdata.lock_state != LOCK_PD_BUF_HOST_ACCESS_FINISH
                    && !(sm_properties[sm_index].physical_start_addr != 0
                         && curr_processdata.addr == 0))
            {
                address = curr_processdata.addr + (address -
                                                   sm_properties[sm_index].physical_start_addr);
                *physical_address = (uint32_t)(((PRUICSS_HwAttrs *)
                                                (pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address);
                my_pHost2PruIntfc->sm_processdata[sm_index - 2].lock_state =
                    LOCK_PD_BUF_HOST_ACCESS_START;
                return 0;
            }

            else
            {
                spi_bsp_write_sm_access = 0;
                *physical_address = 0;
                return -1;
            }
        }

        *physical_address = (uint32_t)(((PRUICSS_HwAttrs *)
                                        (pruIcss1Handle->hwAttrs))->baseAddr + PRU_ICSS_SHARED_RAM + address);
    }

    return 0;
}


inline uint8_t spi_bsp_slave_write_byte(uint16_t ecat_addr,
                                        uint8_t *write_physical_addr_ptr, uint8_t data)
{
    if(ecat_addr < ESC_ADDR_MEMORY)
    {
        if(bsp_is_pdi_perm_read_only(pdi_reg_perm_array,
                                     ecat_addr) != PDI_PERM_READ_ONLY)
        {
            *write_physical_addr_ptr = data;
        }
    }

    else
    {
        *write_physical_addr_ptr = data;
    }

    return 0;
}


uint8_t spi_bsp_slave_write_end(uint16_t start_address, uint8_t *write_buf,
                                uint16_t length)
{
    if(start_address < ESC_ADDR_MEMORY)
    {
        //Register Access
        bsp_spi_pdi_write_indication(pruIcss1Handle, start_address, length, write_buf);
    }

    else
    {
        if(spi_bsp_write_sm_access)
        {
            //Process Data Access
            uint16_t mbx_start = sm_properties[MAILBOX_READ].physical_start_addr;
            uint16_t mbx_end = mbx_start + sm_properties[MAILBOX_READ].length;

            if((start_address >= mbx_start) && (start_address + length <= mbx_end))
            {
                if(start_address == mbx_start)
                {
                    bsp_pdi_mbx_write_start(pruIcss1Handle);
                }

                if((start_address + length) == mbx_end)
                {
                    bsp_pdi_mbx_write_complete(pruIcss1Handle);
                }
            }

            else
            {
                bsp_process_data_access_complete(pruIcss1Handle, start_address, length,
                                                 spi_bsp_write_sm_index);
            }
        }

        //reset flags
        spi_bsp_write_sm_access = 1;
        spi_bsp_write_sm_index = -1;
    }


    return 0;
}

uint8_t bsp_spi_get_slave_configuration(void)
{
    return eeprom_cache[2];
}

#endif


#ifdef EDMA_LATENCY_ENHANCEMENT
extern  UINT16 *volatile aPdOutputData;
extern  UINT16 *volatile aPdInputData;

ocmc_malloc_struct_t ocmc_malloc_struct = {SOC_ON_CHIP_MEM_OFFSET};

uint32_t ocmc_malloc(uint16_t alignment, uint16_t length)
{
    uint32_t ret_val = 0;

    if(ocmc_malloc_struct.free_memory_pointer % alignment != 0)
    {
        ocmc_malloc_struct.free_memory_pointer += alignment -
                (ocmc_malloc_struct.free_memory_pointer % alignment);
    }

    ret_val = ocmc_malloc_struct.free_memory_pointer;
    ocmc_malloc_struct.free_memory_pointer += length;

    return ret_val;
}


void bsp_init_sm_dma(PRUICSS_Handle pruIcssHandle, EDMA3_DRV_Handle hEdma,
                     t_sm_dma_properties *sm_dma_settings)
{
    /* Request any DMA channel and any TCC */
    sm_dma_settings->dma_channel_id = EDMA3_DRV_QDMA_CHANNEL_ANY;
    sm_dma_settings->dma_tcc = EDMA3_DRV_TCC_ANY;

    if(sm_dma_settings->sm_direction == SM_DIRECTION_READ)
    {
        EDMA3_DRV_requestChannel(hEdma, &sm_dma_settings->dma_channel_id,
                                 &sm_dma_settings->dma_tcc,
                                 (EDMA3_RM_EventQueue)0, NULL, NULL);

        /* Set QDMA Trigger Word as Source Address */
        EDMA3_DRV_setQdmaTrigWord(hEdma, sm_dma_settings->dma_channel_id,
                                  EDMA3_RM_QDMA_TRIG_SRC);
    }

    else
    {
        EDMA3_DRV_requestChannel(hEdma, &sm_dma_settings->dma_channel_id,
                                 &sm_dma_settings->dma_tcc,
                                 (EDMA3_RM_EventQueue)0, NULL, NULL);

        /* Temporarily Set QDMA Trigger Word as DST Address */
        EDMA3_DRV_setQdmaTrigWord(hEdma, sm_dma_settings->dma_channel_id,
                                  EDMA3_RM_QDMA_TRIG_DST);

        /* Write The upper two bytes of Source Address */
        EDMA3_DRV_setSrcParams(hEdma, sm_dma_settings->dma_channel_id,
                               ((((PRUICSS_HwAttrs *)
                                  (pruIcssHandle->hwAttrs))->baseAddr)
                                + PRU_ICSS_SHARED_RAM),
                               EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);

        /* Set QDMA Trigger Word as Source Address */
        EDMA3_DRV_setQdmaTrigWord(hEdma, sm_dma_settings->dma_channel_id,
                                  EDMA3_RM_QDMA_TRIG_SRC);
    }

    EDMA3_DRV_setSrcIndex(hEdma, sm_dma_settings->dma_channel_id,
                          sm_dma_settings->sm_length,
                          sm_dma_settings->sm_length);

    EDMA3_DRV_setDestIndex(hEdma, sm_dma_settings->dma_channel_id,
                           sm_dma_settings->sm_length,
                           sm_dma_settings->sm_length);

    EDMA3_DRV_setTransferParams(hEdma, sm_dma_settings->dma_channel_id,
                                sm_dma_settings->sm_length, 1, 1, 1,
                                EDMA3_DRV_SYNC_A);

    /* Set Source Transfer Mode as Increment Mode. */
    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_SAM, EDMA3_DRV_ADDR_MODE_INCR);

    /* Set Destination Transfer Mode as Increment Mode. */
    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_DAM, EDMA3_DRV_ADDR_MODE_INCR);

    /* Write to the Destination Address */
    if(sm_dma_settings->sm_direction == SM_DIRECTION_READ)
    {
        EDMA3_DRV_setDestParams(hEdma, sm_dma_settings->dma_channel_id,
                                (uint32_t)(sm_dma_settings->pru_buf_addr[1]),
                                EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);
    }

    else
    {
        EDMA3_DRV_setDestParams(hEdma, sm_dma_settings->dma_channel_id,
                                (uint32_t)(sm_dma_settings->ddr_buf_addr
                                           [sm_dma_settings->ready_buf_index]),
                                EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);
    }

    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_STATIC, 1u);

    EDMA3_DRV_unlinkChannel(hEdma, sm_dma_settings->dma_channel_id);

    uint32_t phyaddress;
    EDMA3_DRV_getPaRAMPhyAddr(hEdma, sm_dma_settings->dma_channel_id,
                              &phyaddress);
    sm_dma_settings->dma_param_handle = (EDMA3_DRV_ParamentryRegs *)phyaddress;



    /* Setup the increment counter DMA */
    /* Request any DMA channel and any TCC */
    sm_dma_settings->counter_dma_channel_id = EDMA3_DRV_DMA_CHANNEL_ANY;
    sm_dma_settings->counter_dma_tcc = EDMA3_DRV_TCC_ANY;

    EDMA3_DRV_requestChannel(hEdma, &sm_dma_settings->counter_dma_channel_id,
                             &sm_dma_settings->counter_dma_tcc,
                             (EDMA3_RM_EventQueue)0, NULL, NULL);

    EDMA3_DRV_setSrcIndex(hEdma, sm_dma_settings->counter_dma_channel_id,
                          1, 1);

    EDMA3_DRV_setDestIndex(hEdma, sm_dma_settings->counter_dma_channel_id,
                           1, 1);

    EDMA3_DRV_setTransferParams(hEdma, sm_dma_settings->counter_dma_channel_id,
                                1, 1, 1, 1, EDMA3_DRV_SYNC_A);

    /* Set Source Transfer Mode as Increment Mode. */
    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->counter_dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_SAM, EDMA3_DRV_ADDR_MODE_INCR);

    /* Set Destination Transfer Mode as Increment Mode. */
    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->counter_dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_DAM, EDMA3_DRV_ADDR_MODE_INCR);

    /* Write to the Destination Address */
    EDMA3_DRV_setDestParams(hEdma, sm_dma_settings->counter_dma_channel_id,
                            (uint32_t)(&sm_dma_settings->sm_dma_pru_handle->
                                       dma_copied_counter),
                            EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);

    /* Write to the Source Address */
    if(sm_dma_settings->sm_direction == SM_DIRECTION_READ)
    {
        EDMA3_DRV_setSrcParams(hEdma, sm_dma_settings->counter_dma_channel_id,
                               (uint32_t)(&sm_dma_settings->sm_dma_pru_handle->
                                          arm_counter),
                               EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);
    }

    else
    {
        EDMA3_DRV_setSrcParams(hEdma, sm_dma_settings->counter_dma_channel_id,
                               (uint32_t)(&sm_dma_settings->sm_dma_pru_handle->
                                          pru_counter),
                               EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT);
    }

    EDMA3_DRV_setOptField(hEdma, sm_dma_settings->counter_dma_channel_id,
                          EDMA3_DRV_OPT_FIELD_STATIC, 1u);

    EDMA3_DRV_unlinkChannel(hEdma, sm_dma_settings->counter_dma_channel_id);

    EDMA3_DRV_ChainOptions chain = {EDMA3_DRV_TCCHEN_DIS,
                                    EDMA3_DRV_ITCCHEN_DIS,
                                    EDMA3_DRV_TCINTEN_DIS,
                                    EDMA3_DRV_ITCINTEN_DIS
                                   };

    /* Transfer complete chaining enable. */
    chain.tcchEn = EDMA3_DRV_TCCHEN_EN;
    /* Intermediate transfer complete chaining enable. */
    chain.itcchEn = EDMA3_DRV_ITCCHEN_DIS;
    /* Transfer complete interrupt is enabled. */
    chain.tcintEn = EDMA3_DRV_TCINTEN_DIS;
    /* Intermediate transfer complete interrupt is disabled. */
    chain.itcintEn = EDMA3_DRV_ITCINTEN_DIS;

    /* Now chain the two channels together. */
    EDMA3_DRV_chainChannel(hEdma, sm_dma_settings->dma_channel_id,
                           sm_dma_settings->counter_dma_channel_id,
                           (EDMA3_DRV_ChainOptions *)&chain);

    if(sm_dma_settings->sm_direction == SM_DIRECTION_READ)
        aPdInputData = (uint16_t *)
                       sm_dma_settings->ddr_buf_addr[sm_dma_settings->ready_buf_index];

    else
        aPdOutputData = (uint16_t *)
                        sm_dma_settings->ddr_buf_addr[sm_dma_settings->ready_buf_index];
}


int8_t bsp_dma_sm_write(uint16_t address, uint16_t len)
{
    Int16 sm = bsp_get_sm_index(address, len);

    if(sm > 1 && (sm < MAX_SYNC_MAN))
    {
        if(sm_dma_properties[sm].sm == sm)
        {

            if((address + len) >= (sm_properties[sm].physical_start_addr +
                                   sm_properties[sm].length))
            {
                sm_dma_properties[sm].sm_dma_pru_handle->arm_counter += 1;
            }

#if (RxPDO_CACHED == 1)
            Cache_wb(aPdInputData, sm_properties[sm].length, Cache_Type_ALLD, true);
#endif /*RxPDO_CACHED*/
            sm_dma_properties[sm].dma_param_handle->SRC =
                sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index]; //trigger DMA
            sm_dma_properties[sm].ready_buf_index = (sm_dma_properties[sm].ready_buf_index +
                                                    1) & 0x1;
            aPdInputData = (uint16_t *)
                           sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];
            return 0;
        }
    }

    return -1;
}


int8_t bsp_dma_sm_write_at_offset(uint16_t address, uint16_t len, void *data)
{

    volatile t_host_interface *pHost2PruIntfc = (volatile t_host_interface *)
            ((((PRUICSS_HwAttrs *)(pruIcss1Handle->hwAttrs))->baseAddr) +
             PRU_ICSS_DATARAM(0));

    Int16 sm = bsp_get_sm_index(address, len);

    if(sm > 1 && (sm < MAX_SYNC_MAN))
    {
        if(sm_dma_properties[sm].sm == sm)
        {

            uint16_t offset = address - sm_properties[sm].physical_start_addr;
            tiesc_memcpy(&aPdInputData[offset], data, len);

            if((address + len) >= (sm_properties[sm].physical_start_addr +
                                   sm_properties[sm].length))
            {
                sm_dma_properties[sm].sm_dma_pru_handle->arm_counter += 1;
                pHost2PruIntfc->sm_processdata[sm - 2].lock_state =
                    LOCK_PD_BUF_HOST_ACCESS_FINISH;
                ASSERT_DMB();
            }

#if (RxPDO_CACHED == 1)
            Cache_wb(aPdInputData, sm_properties[sm].length, Cache_Type_ALLD, true);
#endif /*RxPDO_CACHED*/
            sm_dma_properties[sm].dma_param_handle->SRC =
                sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index]; //trigger DMA
            sm_dma_properties[sm].ready_buf_index = (sm_dma_properties[sm].ready_buf_index +
                                                    1) & 0x1;
            aPdInputData = (uint16_t *)
                           sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];
            return 0;
        }
    }

    return -1;
}


int8_t bsp_dma_sm_read(uint16_t address, uint16_t len)
{
    Int16 sm = bsp_get_sm_index(address, len);

    if(sm > 1 && (sm < MAX_SYNC_MAN))
    {
        if(sm_dma_properties[sm].sm == sm)
        {
            if(sm_dma_properties[sm].sm_dma_pru_handle->dma_copied_counter !=
                    sm_dma_properties[sm].sm_dma_pru_handle->arm_counter)
            {
                aPdOutputData = (uint16_t *)
                                sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];
#if (TxPDO_CACHED == 1)
                Cache_inv(aPdOutputData, sm_properties[sm].length, Cache_Type_ALLD, true);
#endif /*TxPDO_CACHED*/
                sm_dma_properties[sm].ready_buf_index = (sm_dma_properties[sm].ready_buf_index +
                                                        1) & 0x1;
                sm_dma_properties[sm].dma_param_handle->DST =
                    sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];

                if((address + len) >= (sm_properties[sm].physical_start_addr +
                                       sm_properties[sm].length))
                {
                    sm_dma_properties[sm].sm_dma_pru_handle->arm_counter =
                        sm_dma_properties[sm].sm_dma_pru_handle->dma_copied_counter;
                }

                return 0;
            }
        }
    }

    return -1;
}


int8_t bsp_dma_sm_read_at_offset(uint16_t address, uint16_t len, void *data)
{
    Int16 sm = bsp_get_sm_index(address, len);

    if(sm > 1 && (sm < MAX_SYNC_MAN))
    {
        if(sm_dma_properties[sm].sm == sm)
        {
            if(sm_dma_properties[sm].sm_dma_pru_handle->dma_copied_counter !=
                    sm_dma_properties[sm].sm_dma_pru_handle->arm_counter)
            {
                aPdOutputData = (uint16_t *)
                                sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];
#if (TxPDO_CACHED == 1)
                Cache_inv(aPdOutputData, sm_properties[sm].length, Cache_Type_ALLD, true);
#endif /*TxPDO_CACHED*/
                sm_dma_properties[sm].ready_buf_index = (sm_dma_properties[sm].ready_buf_index +
                                                        1) & 0x1;
                sm_dma_properties[sm].dma_param_handle->DST =
                    sm_dma_properties[sm].ddr_buf_addr[sm_dma_properties[sm].ready_buf_index];

                if((address + len) >= (sm_properties[sm].physical_start_addr +
                                       sm_properties[sm].length))
                {
                    sm_dma_properties[sm].sm_dma_pru_handle->arm_counter =
                        sm_dma_properties[sm].sm_dma_pru_handle->dma_copied_counter;
                }
            }

            uint16_t offset = address - sm_properties[sm].physical_start_addr;
            tiesc_memcpy(data, (void *)((uint32_t) aPdOutputData + offset), len);
            return 0;
        }
    }

    return -1;
}

#endif /*EDMA_LATENCY_ENHANCEMENT*/





