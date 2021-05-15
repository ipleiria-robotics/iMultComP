/**
 * @file tiescbsp.h
 *
*/
/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
#ifndef _TIESCBSP_H_
#define _TIESCBSP_H_
/*! \mainpage EtherCAT API Guide
 *
 * \section intro_sec Introduction
 *
 * \version 1.0.7
 *
 * PRU-ICSS EtherCAT firmware implements EtherCAT slave controller layer2 functionality and provides EtherCAT
 * ASIC like functionality integrated into AM3357/AM3359/AM4377/AM4379/AM572x/AM571x/K2G/AMIC110 SOC with PRU-ICSS IP.
 *
 * This also may be used to build a jailhouse inmate. See the
 * http://processors.wiki.ti.com/index.php/Processor_SDK_Jailhouse_Hypervisor
 * for details.
 * Jailhouse inmate runs in a jailhouse cell with limited hardware resources,
 * it is not responsible for the entire board configuration.
 * To compile the application as jailhouse inmate use the "_JAILHOUSE_INMATE" macro.
 * This macro also disables the initialization code for peripherals which are
 * not alloted to the jailhouse inmate.
 *
 *
 * \section purpose Purpose and Scope
 * Purpose of this document is
 * to assist developers porting their EtherCAT slave stack to TI AM3357/9 or AM4377/9 or AM57xx or K2G or AMIC110 SOCs.
 *
 * \section audience Intended Audience
 * -System integrators using TI ESC in their products \n
 * -EtherCAT slave stack vendors to support TI ESC
 *
 *
 * \section ecat_arch Software Architecture
 * @image html TI_ESC_SW_architecture.JPG \n
 *
 * \section ecat_kickoff Procedure to kickoff ESC firmware
 * 1. Initialize memories (register protection, register reset values, EEPROM cache) and PRU-ICSS INTC \n
 * 2. Load ESC firmware into PRUs of PRU-ICSS \n
 * 3. Initialize the EtherCAT slave stack \n
 * 4. Start firmware \n
 * 5. Wait for AL Event Request and SYNC (in DC synchronous mode) interrupts from PRU and run EtherCAT stack
main loop for handling mailbox and ESC state m/c \n
 * 6. Handle the events as needed - Note that this is handled by Beckhoff SSC (Eg:- industrial SDK sample
application)or equivalent 3P stack \n
 *
 * \section ecat_fw_api Firmware APIs
 * These APIs implement the key interface between ESC emulation firmware and EtherCAT stack. They are
 * implemented in following source code files \n
 *
 * <b>TI h/w access layer to support EtherCAT slave using PRU-ICSS</b> \n
 * protocols\ethercat_slave\ecat_appl\EcatStack\tiescbsp.c \n
 * protocols\ethercat_slave\include\tiescbsp.h \n
 *
 * <b>Wrapper APIs for SSC</b> \n
 *
 * protocols\ethercat_slave\ecat_appl\EcatStack\tieschw.c \n
 * protocols\ethercat_slave\include\tieschw.h \n
 *
 * <b>PRU_ICSS EtherCAT firmware headers</b> \n
 * protocols\ethercat_slave\firmware\v1.0 : (AM335/AMIC110) \n
 * protocols\ethercat_slave\firmware\v2.0 : (AM437/AMIC120) \n
 * protocols\ethercat_slave\firmware\v2.1 : (AM57x) \n
 * protocols\ethercat_slave\firmware\v2.2 : (K2G) \n
 * protocols\ethercat_slave\firmware\g_v1.0 : (AM65xx) \n
 *
 * \section install_sec Installation
 *
 * This driver is installed as part of PRU-ICSS EtherCAT Slave Package
 *
 * \section useful_links Useful links
 * <ul>
 *  <li><a href="http://processors.wiki.ti.com/index.php/PRU_ICSS_EtherCAT_firmware_API_guide#PRU-ICSS_interrupt_controller_setup_for_EtherCAT_firmware">PRU-ICSS interrupt controller setup for EtherCAT firmware</a>
 *  <li><a href="http://processors.wiki.ti.com/index.php/PRU_ICSS_EtherCAT_firmware_API_guide#Firmware_Update">Firmware Update</a>
 *  <li><a href="http://processors.wiki.ti.com/index.php/PRU_ICSS_EtherCAT_firmware_API_guide#PRU-ICSS_EtherCAT_Register_List">PRU-ICSS EtherCAT Register List</a>
 *  <li><a href="http://processors.wiki.ti.com/index.php/PRU_ICSS_EtherCAT_firmware_API_guide#AM335x.2FAM437x_SOC_pins_used_for_EtherCAT_functionality">SOC pins used for EtherCAT functionality</a>
 *  <li><a href="http://processors.wiki.ti.com/index.php/PRU_ICSS_EtherCAT_firmware_API_guide#Application_Examples">Application Examples</a>
 * </ul>
 * \section Documentation
 * <a href="modules.html">API Documentation</a>
 * \section terms_abbrevs Term and Abbreviations
 * Abbreviation  | Expansion
 * ------------- | -------------
 * PRUSS         | Programmable RealTime Unit Sub System
 * PRU-ICSS      | Programmable RealTime Unit - Industrial Communication Sub System - PRUSS with industrial communication support
 * ESC           | EtherCAT Slave Controller
 * ECAT          | EtherCAT
 * PDI           | Process Data Interface (Host interface to ESC)
 * HRT           | High Real Time
 * SRT           | Soft Real Time
 */
/**
 * @defgroup ECAT_INITIALIZATION Initialization of stack-fimrware interface
 * @defgroup ECAT_ESC_FW_CMD     ESC Firmware Command API
 * @defgroup ECAT_ESI_EEPROM     ESI EEPROM Emulation support
 * @defgroup ECAT_SYNC_PROP      Sync Manager properties management
 * @defgroup ECAT_SYNC_ACC_CTRL  Sync Manager access and control APIs
 * @defgroup ECAT_SYNC_MAILBOX   Sync Manager Mailbox mode handling
 * @defgroup ECAT_SYNC_BUFFER    Sync Manager Buffer mode handling
 * @defgroup ECAT_ESC_REG_ACCESS ESC Register/Memory access - Special handling APIs
 * @defgroup ECAT_PRU_MDIO       PRU-ICSS MDIO host side APIs
 * @defgroup ECAT_PRU_DIGIO      PRU-ICSS DIGIO host side APIs
 * @defgroup ECAT_SPINLOCK       Spinlock APIs for concurrent Host/Firmware shared memory access
 * @defgroup ECAT_SYSTIME_PDI    System Time PDI controlled APIs
 * @defgroup ECAT_TIMER          Timer APIs for EtherCAT stack
 * @defgroup ECAT_MUTEX          Mutex APIs for EtherCAT stack
 * @defgroup ECAT_INTMGMNT       Interrupt management APIs for EtherCAT stack
 * @defgroup ECAT_MISC           Misc APIs for SSC
 * @defgroup ECAT_PRU_FW         PRU Firmware header mapping API
 * @defgroup ECAT_FW_UPGRADE     Firmware Upgrade APIs for EtherCAT stack
 * @defgroup ECAT_PHY_CONFIG     Phy configuration APIs
 * @defgroup ECAT_TIESC_SPI      TI ESC SPI Mode APIs
 * @defgroup ECAT_EDMA_ENHANCE   EDMA based access latency enhancement APIs
 */

/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/
#ifndef DISABLE_UART_PRINT
#include <stdio.h>
#endif

#include <Ethercat/Include_ethercat/tiesc_pruss_intc_mapping.h>
#include <ti/drv/pruss/pruicss.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>

#if !defined(ECAT_LIMITED_DEMO) && !defined(TIESC_SPI_SLAVE_MODE)
/*
ecat_def.h is included only to get CiA402_DEVICE macro definition status.
This is not needed for limited demo application and hence ignored in that case
*/
#include <ecat_def.h>
#endif




#if defined(ECAT_LIMITED_DEMO) || defined(TIESC_SPI_SLAVE_MODE)
//Stack related defines

#ifndef TIESC_APPLICATION
/*TIESC_APPLICATION: Should be set if the Slave Sample Code runs on an ICE/IDK EVM. */
#define TIESC_APPLICATION                         1
#endif

#ifndef AL_EVENT_ENABLED
#define AL_EVENT_ENABLED                          1
#endif

#ifndef ESC_EEPROM_SIZE
#define ESC_EEPROM_SIZE                           0x800
#endif

#ifndef MEM_ADDR
#define MEM_ADDR                                  uint8_t
#endif

#ifndef EEPROM_READ_SIZE
#define EEPROM_READ_SIZE                          0x8
#endif

#ifndef EEPROM_WRITE_SIZE
#define EEPROM_WRITE_SIZE                         0x2
#endif

#ifndef ESC_EEPROM_EMULATION
#define ESC_EEPROM_EMULATION                      1
#endif

#define ESC_SYSTEMTIME_OFFSET                   0x0910
#define ESC_DC_SYNC_ACTIVATION_OFFSET           0x0981
#define ESC_DC_SYNC0_CYCLETIME_OFFSET           0x09A0
#define ESC_DC_SYNC1_CYCLETIME_OFFSET           0x09A4

#define ESC_EEPROM_CONTROL_OFFSET               0x0502
#define ESC_EEPROM_BUSY_MASK                    0x8000
#define ESC_EEPROM_CMD_MASK                     0x0700
#define ESC_EEPROM_ADDRESS_OFFSET               0x0504
#define ESC_EEPROM_DATA_OFFSET                  0x0508

#define     STATE_INIT                        0x01
#define     STATE_PREOP                       0x02
#define     STATE_BOOT                        0x03
#define     STATE_SAFEOP                      0x04
#define     STATE_OP                          0x08

#endif

#ifdef TIESC_SPI_MASTER_MODE
#define ESC_EEPROM_EMULATION                      0
#endif


/*Single datagram accessing contiguous multiple FMMU mapped areas in  a single slave for process data
is supported now by TI ESC firmware.
Process path latency in TI ESC is high when this support is active
For specific use cases (4SM with 3 FMMUs or multiple FMMUs (in a given ESC) are not accessed in a single datagram)
process path latency improvement can be achieved by disabling below define */
#define ENABLE_MULTIPLE_SM_ACCESS_IN_SINGLE_DATAGRAM     0

#if (!CiA402_DEVICE ) && (!_JAILHOUSE_INMATE)
#define EEPROM_SPI /*Comment this to enable volatile EEPROM support for debug*/
#endif

/** @brief PRU 0 firmware binary file location in SPI flash - at an offset
   of 480 KB from starting location */
#define SPI_APPL_BIN_OFFSET     0x20000             /* Verify from Starterware Bootloader */

#define QSPI_APPL_BIN_OFFSET    0x80000             /* Verify from Starterware Bootloader */

/*SPI Flash offset at which application binary downloaded over FOE will be stored*/
#ifdef SOC_AM335x
#define FOE_APPL_BIN_OFFSET (SPI_APPL_BIN_OFFSET)
#define FOE_FLASH_SECTOR_SIZE (0xFFF)
#elif defined(SOC_AM437x) || defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_K2G) || defined(SOC_AM65XX)
#define FOE_APPL_BIN_OFFSET (QSPI_APPL_BIN_OFFSET)
#define FOE_FLASH_SECTOR_SIZE (0xFFFF)
#endif

#define PROTO extern

#define SWAPWORD
#define SWAPDWORD


#define ICSS_MDIO_USRPHYSEL_LINKINT_ENABLE 0x40
#define ICSS_MDIO_USRPHYSEL_LINKSTAT_MLINK 0x80

#define TIESC_PERM_RW       0x0
#define TIESC_PERM_WRITE_ONLY 0x1
#define TIESC_PERM_READ_ONLY 0x2

#define TIESC_PERM_WRITE TIESC_PERM_WRITE_ONLY
#define TIESC_PERM_READ TIESC_PERM_READ_ONLY

#define PDI_PERM_RW        0x0
#define PDI_PERM_READ_ONLY 0x1

#define PDI_PERM_WRITE PDI_PERM_RW
#define PDI_PERM_READ PDI_PERM_READ_ONLY

#define TIESC_MDIO_CLKDIV   79 //For 2.5MHz MDIO clock: 200/(TIESC_MDIO_CLKDIV+1)

#define TIESC_MDIO_RX_LINK_DISABLE  0 //Slow MDIO state m/c based link detection
#define TIESC_MDIO_RX_LINK_ENABLE   1 //Fast link detect using RXLINK forward from PHY to MDIO MLINK
#define TIESC_LINK_POL_ACTIVE_LOW       1
#define TIESC_LINK_POL_ACTIVE_HIGH      0

/**
 * @def PDI_WD_TRIGGER_RX_SOF
 *  Watchdog RX Start of Frame Trigger
 */
#define PDI_WD_TRIGGER_RX_SOF       (0 << 4)

/**
 * @def PDI_WD_TRIGGER_LATCH_IN
 *  Watchdog LATCH IN Trigger
 */
#define PDI_WD_TRIGGER_LATCH_IN     (1 << 4)

/**
 * @def PDI_WD_TRIGGER_SYNC0_OUT
 *  Watchdog SYNC0 Trigger
 */
#define PDI_WD_TRIGGER_SYNC0_OUT    (2 << 4)

/**
 * @def PDI_WD_TRIGGER_SYNC1_OUT
 *  Watchdog SYNC1 Trigger
 */
#define PDI_WD_TRIGGER_SYNC1_OUT    (3 << 4)

#if ENABLE_MULTIPLE_SM_ACCESS_IN_SINGLE_DATAGRAM
#define TIESC_PORT0_TX_DELAY    0x98
#else
#define TIESC_PORT0_TX_DELAY    0x48
#endif
#define TIESC_PORT1_TX_DELAY    TIESC_PORT0_TX_DELAY

#define PDI_ISR_EDIO_NUM    7    //GPMC_CSN(2) -> pr1_edio_data_out7 for ICEv2.J4.Pin21

/* PDI  side register protection using register permission table (4KB) in memory - disable if you care for performance and memory foot print */
/* #define ENABLE_PDI_REG_PERMISSIONS */
#ifdef TIESC_SPI_SLAVE_MODE
#define ENABLE_PDI_REG_PERMISSIONS
#endif

/* Use ESC system time instead of SYS/BIOS Timestamp_get32 for timing info */
#ifndef TIESC_SPI_MASTER_MODE
#define USE_ECAT_TIMER
#endif

#ifndef _JAILHOUSE_INMATE
#define ENABLE_ONLINE_FIRMWARE_UPGRADE
#endif

/* Uncomment following to enable DC feature of system time compensation via PDI interface instead of ECAT interface
   for synchronizing two independent EtherCAT networks */
//#define SYSTEM_TIME_PDI_CONTROLLED
/*Comment to following to enable PDI  ISR and SYNC ISR in HWI context */
#define  ENABLE_PDI_TASK
/* #define ENABLE_PDI_SWI */
#define  ENABLE_SYNC_TASK

/* Default to interrupt mode for SYNC and PDI tasks for bare metal app */
#ifdef BARE_METAL
#undef ENABLE_PDI_TASK
#undef ENABLE_PDI_SWI
#undef ENABLE_SYNC_TASK

/* McSPI configuration with C2000 F28379d launchpad */
/* #define ENABLE_SPIA_TASK */

/* Enable GPMC on AMIC110 custom board only as GPMC pins are mux-ed as GPIO on TI AMIC110 ICE */
/* #define ENABLE_GPMC_TASK */
#endif //BARE_METAL

/**
 * @def SUPPORT_CMDACK_POLL_MODE
 * If PDI and SYNC ISR is handled in HWI context (similar to SSC)
 * interrupt mode of CMDACK won't work as they are low priority than
 * SYNC and PDI ISR - use polling instead
 */
/* #define  SUPPORT_CMDACK_POLL_MODE */
#ifdef TIESC_SPI_SLAVE_MODE
#define  SUPPORT_CMDACK_POLL_MODE
#endif

#if defined (__aarch64__)
#define ASSERT_DMB()  __asm("    dmb ISH")
#define ASSERT_DSB()  __asm("    dsb ISH")
#else
#define ASSERT_DMB() __asm("    dmb")
#define ASSERT_DSB() __asm("    dsb")
#endif

#ifdef USE_ECAT_TIMER
#define ECAT_TIMER_INC_P_MS 1000000
#else
#define ECAT_TIMER_INC_P_MS ecat_timer_inc_p_ms /* ARM frequency: Will be detected during bsp_init*/
extern volatile uint32_t ecat_timer_inc_p_ms;
#endif

#define ESC_SYSTEMTIME_OFFSET_OFFSET        0x0920
#define ESC_SPEED_COUNTER_START_OFFSET      0x0930
#define ESC_DC_START_TIME_CYCLIC_OFFSET     0x0990

#define DRIFTCTRL_TASK_SYNC_ZERO_CROSS_ADJUST   0xE0 //PRU_DMEM0

typedef void (*bsp_function_handler)(void);

/**
 * @brief Struct for host to PRU-ICSS command interface
 */
typedef struct
{
    uint8_t sm_buf_index;     /**< Sync manager buff index */
    uint8_t lock_state;       /**< Lock state. Can have @ref LOCK_PD_BUF_AVAILABLE_FOR_HOST, @ref LOCK_PD_BUF_HOST_ACCESS_START, @ref LOCK_PD_BUF_HOST_ACCESS_FINISH      */
    uint16_t addr;            /**< Address */
} t_sm_processdata;

/**
 * @def LOCK_PD_BUF_AVAILABLE_FOR_HOST
 *  LOCK available for HOST
 */
#define LOCK_PD_BUF_AVAILABLE_FOR_HOST  0
/**
 * @def LOCK_PD_BUF_HOST_ACCESS_START
 *  Lock available for host access start
 */
#define LOCK_PD_BUF_HOST_ACCESS_START   1
/**
 * @def LOCK_PD_BUF_HOST_ACCESS_FINISH
 *  Lock available for host access finish
 */
#define LOCK_PD_BUF_HOST_ACCESS_FINISH  2

/**
 * @brief Struct for host to PRU-ICSS command interface
 * Starts at PRU0 DMEM @ 0x4a300000
 */
typedef struct
{
    uint8_t reserved1[0x90];          /**< Reserved */
    uint32_t system_time_low;         /**< System Time low */
    uint32_t system_time_high;        /**< System Time high */
    uint8_t sm_config_ongoing;        /**< Sync manageer config */
    uint8_t reserved2[7];             /**< Reserved */
    uint16_t cmdlow;                  /**< CMD low */
    uint16_t cmdlow_ack;              /**< CMD low ack */
    uint16_t param1low;               /**< PARAM1 low */
    uint16_t param2low;               /**< PARAM2 low */
    uint16_t resp1low;                /**< RESP1 low */
    uint16_t resp2low;                /**< RESP2 low */
#ifndef SYSTEM_TIME_PDI_CONTROLLED
    uint8_t reserved3[212];           /**< Reserved */
#else
    uint8_t reserved3[24];            /**< Reserved */
    uint32_t systime_offset_low;      /**< System time offset low */
    uint32_t systime_offset_high;     /**< System time offset high */
    uint8_t reserved4[180];           /**< Reserved */
#endif
    t_sm_processdata sm_processdata[6]; /**< Sync manager process data */
} t_host_interface;

/**
 * @brief Struct for register permission array
 */
typedef struct
{
    uint8_t reserved[1024];       /**< Reserved */
    uint8_t reg_properties[4096]; /**< Register properties */
} t_register_properties;

typedef struct
{
    uint16_t physical_start_addr;
    uint16_t length;
} t_sm_properties;

/**
 * @brief Struct for MDIO initialization parameters
 */
typedef struct
{
    uint16_t clkdiv;      /**< MDIO clkdiv. MDIO clock = 200/(clkdiv+1) */
    uint8_t addr0;        /**< Address of the PHY hooked to PRU-ICSS MII0 */
    uint8_t addr1;        /**< Address of the PHY hooked to PRU-ICSS MII1 */
    uint8_t link0pol; /* LINK_MII signal polarity of PHY hooked to PRU-ICSS MII0. 1: Active lLow 0: Active High */
    uint8_t link1pol; /* LINK_MII signal polarity of PHY hooked to PRU-ICSS MII1. 1: Active lLow 0: Active High */
    uint8_t enhancedlink_enable;  /**< Enable enhanced link detection using MII RXLINK and TLK110 enhanced link detection features*/
} t_mdio_params;


#ifdef EDMA_LATENCY_ENHANCEMENT
#include <ti/sdo/edma3/drv/sample/bios6_edma3_drv_sample.h>

#define DMA_SM_BUF_MAX_INDEX            2
#define SM_DIRECTION_READ               0
#define SM_DIRECTION_WRITE              1
#define SM_BUFFERED                     0x00
#define SM_MAILBOX                      0x10

/* DMEM 0 */
#define SM_DMA_PRU_STRUCT_OFFSET         0x600
#define SM_DMA_PRU_STRUCT_SIZE           0x8

/**
 * @brief Struct for maintaining the OCMC memory pointer for ocmc_malloc
 */
typedef struct
{
    uint32_t free_memory_pointer;
} ocmc_malloc_struct_t;


/**
 * @brief Struct for PRU firmware to get DMA address and read/write counters for each SM
 */
typedef struct
{
    uint16_t edma_param_dst_addr;
    uint8_t pru_counter;
    uint8_t dma_copied_counter;
    uint8_t arm_counter;
} t_sm_dma_pru_struct;

/**
 * @brief Struct for storing DMA information for each SM
 */
typedef struct
{
    volatile EDMA3_DRV_ParamentryRegs *dma_param_handle;
    volatile t_sm_dma_pru_struct *sm_dma_pru_handle;
    uint32_t ddr_buf_addr[DMA_SM_BUF_MAX_INDEX];
    uint32_t pru_buf_addr[DMA_SM_BUF_MAX_INDEX];
    uint32_t dma_tcc;
    uint32_t dma_channel_id;
    uint16_t sm_length;
    uint8_t sm;
    uint8_t sm_direction;           /**< 0 : Read SM, ECAT read, PDI write | 1 : Write SM, ECAT write, PDI read */
    uint8_t ready_buf_index;
    uint32_t counter_dma_channel_id;
    uint32_t counter_dma_tcc;
} t_sm_dma_properties;

/**
 * @def RxPDO_CACHED
 *  Update this macro to "1" if RxPDO needs to be cached
 */
#define RxPDO_CACHED    0

/**
 * @def TxPDO_CACHED
 *  Update this macro to "1" if TxPDO needs to be cached
 */
#define TxPDO_CACHED    0

#if (RxPDO_CACHED == 1)
#define sm_read_dma_malloc(alignment, length)       memalign(alignment, length)
#else
#define sm_read_dma_malloc(alignment, length)       ocmc_malloc(alignment, length)
#endif

#if (TxPDO_CACHED == 1)
#define sm_write_dma_malloc(alignment, length)      memalign(alignment, length)
#else
#define sm_write_dma_malloc(alignment, length)      ocmc_malloc(alignment, length)
#endif

/** @addtogroup ECAT_EDMA_ENHANCE
 @{ */
/**
*  @brief Setup the EDMA params during SAFEOP to OP transition in Buffer mode
*
*  @param pruIcssHandle PRUSS Handle
*  @param hEdma EDMA handle
*  @param sm_dma_settings structure to store DMA channel details
*/
void bsp_init_sm_dma(PRUICSS_Handle pruIcssHandle, EDMA3_DRV_Handle hEdma,
                     t_sm_dma_properties *sm_dma_settings);

/**
*  @brief Write the complete SM local buffer to the Shared memory using DMA
*
*  @param address ESC address of the buffer
*  @param length length of the buffer
*
*  @retval 0 : if write successful | -1 : if write failed
*/
int8_t bsp_dma_sm_write(uint16_t address, uint16_t len);

/**
*  @brief Write the given buffer to the Shared memory using DMA
*
*  @param address ESC address of the buffer
*  @param length length of the buffer
*  @param data pointer to the buffer
*
*  @retval 0 : if write successful | -1 : if write failed
*/
int8_t bsp_dma_sm_write_at_offset(uint16_t address, uint16_t len, void *data);

/**
*  @brief Get the SM data into local SM buffer
*
*  @param address ESC address of the buffer
*  @param length length of the buffer
*
*  @retval 0 : if write successful | -1 : if write failed
*/
int8_t bsp_dma_sm_read(uint16_t address, uint16_t len);

/**
*  @brief Read the SM data into given local buffer
*
*  @param address ESC address of the buffer
*  @param length length of the buffer
*  @param data pointer to the buffer
*
*  @retval 0 : if write successful | -1 : if write failed
*/
int8_t bsp_dma_sm_read_at_offset(uint16_t address, uint16_t len, void *data);

/**
*  @brief Allocate aligned memory buffer from OCMC memory
*
*  @param alignment alignment size for the buffer to be alloted
*  @param length length of the buffer to be alloted
*
*  @retval Pointer to the start of the buffer
*/
uint32_t ocmc_malloc(uint16_t alignment, uint16_t length);

/**
@}
*/

#endif /*EDMA_LATENCY_ENHANCEMENT*/


PROTO int32_t bsp_main(void);

/** @addtogroup ECAT_INITIALIZATION
 @{ */
/**
*  @brief Initialize stack firmware interface.
*
*  Setup PRU-ICSS interrupt controller for EtherCAT application. \n
*  Initialize the PRU data memories. \n
*  Setup register permissions by invoking bsp_esc_reg_perm_init. \n
*  Load and start EtherCAT PRU firmware. \n
*  Initialize EEPROM emulation \n
*  Initialize command semaphore and bsp_mutex GateAll \n
*
*  @param pruIcssHandle  PRU-ICSS Handle
*
*/
PROTO void bsp_init(PRUICSS_Handle pruIcssHandle);

/**
*  @brief Sets up register permissions for ECAT side access for TI ESC,
*  if ENABLE_PDI_REG_PERMISSIONS is defined in tiescbsp.h, then this function
*  also initializes register permissions for PDI side access from stack/application.
*
*  NOTE : ENABLE_PDI_REG_PERMISSIONS
*  This feature is not enabled by default in Industrial SDK for optimizing
*  memory and performance. TI_ESC has Onchip PDI interface where Host CPU
*  has direct access to ESC registers as they are emulated using PRU_ICSS
*  shared data memory.If ENABLE_PDI_REG_PERMISSIONS is defined and bsp_read/write_XXX
*  are used by application for all register accesses then ONLY PDI access
*  check is enforced. If application bypasses the use of bsp_read/write_xxx API
*  (say HWREG to ESC register in PRU_ICSS memory), whole scheme won't work.
*  ENABLE_PDI_REG_PERMISSIONS is a software scheme and has overhead of 4KB data memory
*  to maintain permission array and additional overhead of access check for every read/write
*  to registers. ENABLE_PDI_REG_PERMISSIONS is enabled and bsp_read/write is enabled
*  customer needs to set correct permissions in pdi_reg_perm_array, say for implementing
*  vendor specific register for ESC to activate a host side feature.
*
*  Struct for register permission array
*  Starts at PRU1 DMEM @ 0x4a302000
*  @ref t_register_properties;
*
*  @param pruIcssHandle  PRU-ICSS Handle
*
*/
PROTO void bsp_esc_reg_perm_init(PRUICSS_Handle
                                 pruIcssHandle);  //Internal API - invoked by bsp_init

/**
*  @brief Register IRQ handlers for various PRU-ICSS interrupts from firmware
*  to host to clear corresponding events in PRU-ICSS INTC
*
*  @param pruIcssHandle  PRU-ICSS Handle
*
*/
PROTO void bsp_start_esc_isr(PRUICSS_Handle pruIcssHandle);

/**
*  @brief Cleanup of TI ESC HAL
* Delete command semaphore and bsp_mutex GateAll
* Flush the EEPROM cache to SPI flash if present
* Disable PRUs
*
*  @param pruIcssHandle  PRU-ICSS Handle
*
*/
PROTO void bsp_exit(PRUICSS_Handle pruIcssHandle);

/**
*  @brief Configure PDI WD trigger mode,
*  PDI WD is triggered automatically by h/w on RX_SOF(port0/port1), latch0/1 input high,
*  SYNC0/1 out high. PDI WD is also triggered whenever host sends a command to firmware.
*  PDI WD may not expire if host stops sending commands to firmware alone, this will occur
*  only if configured h/w events do not occur during WD period
*
*  @param pruIcssHandle  PRU-ICSS Handle
*  @param mode   @ref PDI_WD_TRIGGER_RX_SOF \n
*                @ref PDI_WD_TRIGGER_LATCH_IN \n
*                @ref PDI_WD_TRIGGER_SYNC0_OUT \n
*                @ref PDI_WD_TRIGGER_SYNC1_OUT \n
*
*/
PROTO void bsp_set_pdi_wd_trigger_mode(PRUICSS_Handle pruIcssHandle,
                                       uint32_t mode);
/**
@}
*/
/** @addtogroup ECAT_ESC_FW_CMD
 @{ */

/**
*  @brief Send command and parameters from stack to firmware to perform some
*  action based on stack state or in response to AL event interrupt or SYNC interrupt from ESC
*
*  @param pruIcssHandle  PRU-ICSS Handle
*  @param command    Command Id to be executed by ESC firmware
*  @param param1     parameter1 corresponding to the command
*  @param param2     parameter2 corresponding to the command
*
*  @details
*  Command                             | Id            | Param1                                    | Param 2       | Return Val    | Remarks                                                                              |
*  ----------------------------------- | ------------- | ----------------------------------------- | ------------- | ------------- | ------------------------------------------------------------------------------------ |
*  @ref CMD_DL_USER_ACK_MBX_READ       | 4             | address                                   | length        | NA            | Indicate PDI side mailbox read completion to firmware                                |
*  @ref CMD_DL_USER_ACK_MBX_WRITE      | 5             | address                                   | length        | NA            | Indicate PDI side mailbox write to firmware                                          |
*  @ref CMD_DL_USER_EEPROM_CMD_ACK     | 6             | NA                                        | NA            | NA            | Acknowledge EEEPROM emulation command execution by ARM host stack                    |
*  @ref CMD_DL_USER_READ_SYNC_STATUS   | 7             | sync_sel                                  | NA            | NA            | Indicate SYNC0/1 status register read by PDI to firmware                             |
*  @ref CMD_DL_USER_READ_AL_CONTROL    | 8             | NA                                        | NA            | NA            | Indicate AL control register read by PDI to firmware                                 |
*  @ref CMD_DL_USER_WRITE_AL_STATUS    | 9             | NA                                        | NA            | NA            | Indicate AL status register write by PDI to firmware                                 |
*  @ref CMD_DL_USER_READ_PD_WD_STATUS  | 10            | NA                                        | NA            | NA            | Indicate PD watchdog status register read by PDI to firmware updating register status|
*  @ref CMD_DL_USER_READ_SM_ACTIVATE   | 11            | sm_index                                  | NA            | NA            | Indicate SM activate register read by PDI to firmware                                |
*  @ref CMD_DL_USER_WRITE_SM_PDI_CTRL  | 12            | (sm_index<<8) OR pdi control reg value    | NA            | NA            | Indicate SM PDI control write to control SM from PDI to firmware                     |
*  @ref CMD_DL_USER_READ_LATCH_TIME    | 13            | latch_sel                                 | NA            | NA            | Indicate Latch0/1 time registest read by PDI to firmware                             |
*  @ref CMD_DL_USER_READ_SYS_TIME      | 14            | NA                                        | NA            | NA            | Indicate System Time register read by PDI to firmware                                |
*  @ref CMD_DL_USER_CLEAR_AL_EVENT_LOW | 15            | event_mask                                | NA            | NA            | Clear events not set in event_mask in AL event request register                      |
*  @ref CMD_DL_USER_SYSTIME_PDI_CONTROL | 16            | reg_sel                                   | NA           | NA            | Indicate to the firmware DC register update from PDI side                            |
*
*  Struct for hot to PRU ICSS command interface - @ref t_host_interface
*
*  IMPORTANT NOTE on @ref SUPPORT_CMDACK_POLL_MODE
* EtherCAT firmware supports both poll mode and interrupt mode for command acknowledge.
* It is found that poll mode offers better latency in given scenarios but will waste CPU cycles busy waiting
* for command response from PRU. If you need to switch to command ack interrupt mode comment the define for
* @ref SUPPORT_CMDACK_POLL_MODE in tiescbsp.h
*/
PROTO void bsp_send_command_to_firmware(PRUICSS_Handle pruIcssHandle,
                                        uint32_t command, uint16_t param1, uint16_t param2);
/**
@}
*/
/** @addtogroup ECAT_ESI_EEPROM
 @{ */

/**
*  @brief  Initialize the EEPROM cache in volatile RAM - if SPI flash
*  has valid EEPROM load from SPI flash, otherwise load predefined EEPROM in application
*
*/
PROTO void bsp_eeprom_emulation_init(void);

/**
*  @brief For loading ESC registers from EEPROM during first boot/reload after validating CRC
*
*  @param pruIcssHandle PRUSS handle
*  @param reload_flag  If set reload command is being executed else first boot in progress
*
*  @retval 0: On successful load of registers -1: On CRC error
*
*/
PROTO int32_t bsp_eeprom_load_esc_registers(PRUICSS_Handle pruIcssHandle, int32_t reload_flag);

/**
*  @brief Perform reload operation after validating EEPROM CRC
*
*  @param pruIcssHandle PRUSS handle
*
*  @retval 0: On successful load of registers -1: On CRC error
*
*/
PROTO int32_t bsp_eeprom_emulation_reload(PRUICSS_Handle pruIcssHandle);

/**
*  @brief Perform reload operation after validating EEPROM CRC, Wrapper API for SSC
*
*  @param pruIcssHandle PRUSS handle
*
*  @retval 0: On successful load of registers -1: On CRC error
*
*/
PROTO void bsp_eeprom_emulation_command_ack(PRUICSS_Handle pruIcssHandle);

/**
*  @brief Flush the EEPROM cache to non-volatile storage (say SPI flash) if available
*
*/
PROTO void bsp_eeprom_emulation_flush(void);

/**
*  @brief Call EEPROM flush on exit
*
*
*/
PROTO void bsp_eeprom_emulation_exit(void);

/**
*  @brief  Return pointer to volatile EEPROM cache in TI ESC HAL for processing to access the EEPROM
*
*  @retval Pointer to volatile EEPROM cache start to the stack/application for EEPROM access
*
*/
PROTO uint8_t *bsp_get_eeprom_cache_base(void);

/**
*  @brief Set EEPROM update time
*
*/
void bsp_set_eeprom_updated_time(void);

/**
*  @brief Get EEPROM Updated time
*
*  @retval EEPROM updated time
*
*/
uint32_t bsp_get_eeprom_updated_time(void);

/**
*  @brief Indicate to TI ESC HAL whether EEPROM is written for flushing to non-volatile storage
*
*  @param status Update the TI ESC HAL EEPROM update status - typically on EEPROM write detection from stack
*
*/
PROTO void bsp_set_eeprom_update_status(uint8_t status);

/**
*  @brief Read the EEPROM update status from TI ESC HAL - typically from low priority task periodically check EEPROM dirty status for flush
*
*  @retval EEPROM updated status
*
*/
PROTO uint8_t bsp_get_eeprom_update_status(void);

/**
@}
*/

/** @addtogroup ECAT_SYNC_PROP
 @{ */
/**
*  @brief Set the address, length info from register to TI ESC HAL layer. During INIT to PREOP transition in Mailbox mode.
*  During SAFEOP to OP transition in Buffer mode
*
*  @param pruIcssHandle PRUSS Handle
*  @param sm SyncManager index to set
*  @param address physical start address of SyncManager
*  @param len length of start address of SyncManager
*
*
*/
PROTO void bsp_set_sm_properties(PRUICSS_Handle pruIcssHandle, uint8_t sm,
                                 uint16_t address, uint16_t len);
/**
*  @brief Get the pointer to requested SM properties - used for Buffer/Mailbox read/write detection
*  from Host PDI interface to indicate to the firmware
*
*  @param sm SyncManager index to get properties from TI ESC HAL
*
*  @retval pointer to requested sm_properties array element in TI ESC HAL
*
*/
PROTO t_sm_properties *bsp_get_sm_properties(uint8_t sm);
/**
@}
*/
/** @addtogroup ECAT_SYNC_ACC_CTRL
 @{ */
/**
*  @brief Checks whether firmware has finished updating internal state for SM configuration change initiated by stack/PDI
*
*  @param pruIcssHandle PRUSS Handle
*
*  @retval 0 : SM configuration finished Otherwise : SM configuration ongoing
*
*/
PROTO uint8_t bsp_pdi_sm_config_ongoing(PRUICSS_Handle pruIcssHandle);
/**
@}
*/

/** @addtogroup ECAT_SYNC_MAILBOX
 @{ */

/**
*  @brief Indicates to the firmware that PDI side read from write mailbox has started
*
*  @param pruIcssHandle PRUSS Handle
*/
PROTO void bsp_pdi_mbx_read_start(PRUICSS_Handle pruIcssHandle);
/**
*  @brief Indicates to the firmware that PDI side read from write mailbox has completed
*
*  @param pruIcssHandle PRUSS Handle
*/
PROTO void bsp_pdi_mbx_read_complete(PRUICSS_Handle pruIcssHandle);
/**
*  @brief Indicates to the firmware that PDI side write to read mailbox has started
*
*  @param pruIcssHandle PRUSS Handle
*
*/
PROTO void bsp_pdi_mbx_write_start(PRUICSS_Handle pruIcssHandle);
/**
*  @brief  Indicates to the firmware that PDI side write to read mailbox has completed
*
*  @param pruIcssHandle PRUSS Handle
*
*/
PROTO void bsp_pdi_mbx_write_complete(PRUICSS_Handle pruIcssHandle);
/**
@}
*/

/** @addtogroup ECAT_SYNC_BUFFER
 @{ */
/**
*  @brief  Get the actual address of the buffer for PDI side read/write from host MPU in 3-buffer mode
*
*  @param pruIcssHandle PRUSS Handle
*  @param address SM buffer address for PDI side access
*  @param len SM Buffer length for PDI side access
*  @param p_sm_index Matching SM index corresponding to address
*
*  @retval actual PDI address in memeory
*
*/
PROTO uint16_t bsp_get_process_data_address(PRUICSS_Handle pruIcssHandle, uint16_t address, uint16_t len, int16_t *p_sm_index);
/**
*  @brief  This API is invoked after PDI side completes read/write to PD address returned by bsp_get_process_data_address
*  to indicate this to firmware for swapping buffers etc
*
*  @param pruIcssHandle PRUSS Handle
*  @param address SM buffer address for PDI side access
*  @param len SM buffer length for PDI side access
*  @param sm_index SM index of this process data buffer
*
*/
PROTO void bsp_process_data_access_complete(PRUICSS_Handle pruIcssHandle,
        uint16_t address, uint16_t len, int16_t sm_index);
/**
@}
*/

/** @addtogroup ECAT_ESC_REG_ACCESS
 @{ */
/**
*  @brief  Read a byte value at 'Address' from ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC address to read from PDI
*
*  @retval Value at address
*
*/
PROTO uint8_t bsp_read_byte(PRUICSS_Handle pruIcssHandle, uint16_t address);
/**
*  @brief  Read a 16-bit value at 'Address' from ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC address to read from PDI
*
*  @retval Value at address
*
*/
PROTO uint16_t bsp_read_word(PRUICSS_Handle pruIcssHandle, uint16_t address);
/**
*  @brief  Read a 32-bit value at 'Address' from ESC memory - SM mailbox (single buffer) mapped or register area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC address to read from PDI
*
*  @retval Value at address
*
*/
PROTO uint32_t bsp_read_dword(PRUICSS_Handle pruIcssHandle, uint16_t address);
/**
*  @brief   Read a byte array at 'address' from ESC memory
*
*  @param pruIcssHandle PRUSS Handle
*  @param pdata Pointer to the array in application
*  @param address ESC address to read from PDI
*  @param len Number of bytes to read from ESC memory
*
*/
PROTO void bsp_read(PRUICSS_Handle pruIcssHandle, uint8_t *pdata,
                    uint16_t address,
                    uint16_t len);
/**
*  @brief Read a byte value at 'Address' from ESC process data memory - SM buffer (3-buffer) mapped area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC Address to read from PDI
*  @retval Value at address
*
*/
PROTO uint8_t bsp_read_byte_isr(PRUICSS_Handle pruIcssHandle, uint16_t address);
/**
*  @brief  Read a 16-bit value at 'Address' from ESC process data memory - SM buffer (3-buffer) mapped area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC address to read from PDI
*  @retval Value at address
*
*/
PROTO uint16_t bsp_read_word_isr(PRUICSS_Handle pruIcssHandle,
                                 uint16_t address);
/**
*  @brief   Read a 32-bit value at 'Address' from ESC process data memory - SM buffer (3-buffer) mapped area
*
*  @param pruIcssHandle PRUSS Handle
*  @param address ESC address to read from PDI
*  @retval Value at address
*
*/
PROTO uint32_t bsp_read_dword_isr(PRUICSS_Handle pruIcssHandle,
                                  uint16_t address);
/**
*  @brief  Invoked after reading a register or mailbox buffer from PDI side
*
*  @details Handles following registers \n
*  SM WD status \n
*  AL control   \n
*  SYNC0/1 status   \n
*  SM Activate registers    \n
*  Latch0/1 time Postive/Negative edge  \n
*  Also detects read mailbox start address access from PDI side
*
*  @param pruIcssHandle PRUSS Handle
*  @param address Start address of ESC read from PDI
*  @param length Length of ESC read from PDI
*
*/
PROTO void bsp_pdi_post_read_indication(PRUICSS_Handle pruIcssHandle,
                                        uint16_t address, uint16_t length);
/**
*  @brief  Invoked after writing a register or mailbox buffer from PDI side
*
*  @details Handles following registers \n
*  AL status \n
*  SM PDI control registers \n
*  Also detects write mailbox start address access from PDI side
*
*  @param pruIcssHandle PRUSS Handle
*  @param address Start address of ESC write from PDI
*  @param length Length of ESC write from PDI
*  @param value Value to be written?
*
*/
PROTO void bsp_pdi_write_indication(PRUICSS_Handle pruIcssHandle,
                                    uint16_t address, uint16_t length,
                                    uint16_t value);
/**
*  @brief   Write a byte value at 'address' in ESC memory
*
*  @param pruIcssHandle PRUSS Handle
*  @param val 8-bit value to write
*  @param address ESC address read from PDI
*
*/
PROTO void bsp_write_byte(PRUICSS_Handle pruIcssHandle, uint8_t val,
                          uint16_t address);
/**
*  @brief  Write a 16-bit value at 'address' in ESC memory
*
*  @param pruIcssHandle PRUSS Handle
*  @param val 16-bit value to write
*  @param address ESC address read from PDI
*
*/
PROTO void bsp_write_word(PRUICSS_Handle pruIcssHandle, uint16_t val,
                          uint16_t address);
/**
*  @brief  Write a 32-bit value at 'address' in ESC memory
*
*  @param pruIcssHandle PRUSS Handle
*  @param val 32-bit value to write
*  @param address ESC address read from PDI
*
*/
PROTO void bsp_write_dword(PRUICSS_Handle pruIcssHandle, uint32_t val,
                           uint16_t address);
/**
*  @brief  Write 'len' bytes from pdata to 'address' in ESC memory
*
*  @param pruIcssHandle PRUSS Handle
*  @param pdata Pointer to byte array in application
*  @param address ESC address read from PDI
*  @param len Length of data
*
*/
PROTO void bsp_write(PRUICSS_Handle pruIcssHandle, uint8_t *pdata,
                     uint16_t address, uint16_t len);
/**
*  @brief  Read a 32-bit value from PRU-ICSS MDIO register at 'regoffset'
*
*  @param pruIcssHandle PRUSS Handle
*  @param regoffset PRU-ICSS MDIO register offset
*  @retval Value read
*
*/
PROTO uint32_t bsp_pruss_mdioreg_read(PRUICSS_Handle pruIcssHandle,
                                      uint32_t regoffset);
/**
*  @brief  Write a 32-bit value from PRU-ICSS MDIO register at 'regoffset'
*
*  @param pruIcssHandle PRUSS Handle
*  @param val 32-bit value to write to PRU-ICSS MDIO
*  @param regoffset PRU-ICSS MDIO register offset
*
*/
PROTO void bsp_pruss_mdioreg_write(PRUICSS_Handle pruIcssHandle, uint32_t val,
                                   uint32_t regoffset);
/**
*  @brief Read a 32-bit value from PRU-ICSS IEP register at 'regoffset'
*
*  @param pruIcssHandle PRUSS Handle
*  @param regoffset PRU-ICSS IEP register offset
*  @retval Read value
*
*/
PROTO uint32_t bsp_pruss_iepreg_read(PRUICSS_Handle pruIcssHandle,
                                     uint32_t regoffset);
/**
*  @brief  Write a 32-bit value from PRU-ICSS IEP register at 'regoffset'
*
*  @param pruIcssHandle PRUSS Handle
*  @param val 32-bit value to write to PRU-ICSS IEP
*  @param regoffset PRU-ICSS MDIO register offset
*
*/
PROTO void bsp_pruss_iepreg_write(PRUICSS_Handle pruIcssHandle, uint32_t val,
                                  uint32_t regoffset);
/**
*  @brief  Read a 16-bit value from PRU-ICSS IEP command interface
*
*  @param val 16-bit value to write
*  @param ptr Pointer to Host - PRU-ICSS command interface in PRU-ICSS data memory
*
*/
PROTO void bsp_pruss_cmd_intfc_write_word(uint16_t val, volatile uint16_t *ptr);
/**
*  @brief   Read a 16-bit value from PRU-ICSS IEP command interface
*
*  @param ptr PRU-ICSS command interface pointer
*  @retval 16-bit value read
*
*/
PROTO uint16_t bsp_pruss_cmd_intfc_read_word(volatile uint16_t *ptr);
/**
*  @brief  Checks if the PDI register [byte] has the requested access permission and returns the result
*
*  @param address PDI Register address
*  @param access  Permission to be checked ( Read, Write, Read&Write )
*  @retval 1 if the register has the requested permission, 0 if otherwise
*
*/
PROTO uint8_t  bsp_get_pdi_access_perm(uint16_t address, uint8_t access);
/**
*  @brief  Checks if the PDI register [Two bytes] has the requested access permission and returns the result
*
*  @param address PDI Register address
*  @param access  Permission to be checked ( Read, Write, Read&Write )
*  @retval 1 if the register has the requested permission, 0 if otherwise
*
*/
PROTO uint8_t  bsp_pdi_access_perm_word(uint16_t address, uint8_t access);
/**
*  @brief  Checks if the PDI register [Four bytes] has the requested access permission and returns the result
*
*  @param address PDI Register address
*  @param access  Permission to be checked ( Read, Write, Read&Write )
*  @retval 1 if the register has the requested permission, 0 if otherwise
*
*/
PROTO uint8_t  bsp_pdi_access_perm_dword(uint16_t address, uint8_t access);
/**
*  @brief  Checks if all PDI registers starting from 'address' has the requested access permission and returns the result
*
*  @param address PDI Register address
*  @param access  Permission to be checked ( Read, Write, Read&Write )
*  @param size Number of register bytes
*  @retval 1 if the register has the requested permission, 0 if otherwise
*
*/
PROTO uint8_t  bsp_pdi_access_perm_array(uint16_t address, uint8_t access,
        uint16_t size);

/**
*  @brief  Set the PDI register [byte] access permission to read only
*
*  @param perm_array  Pointer to the Permissions array
*  @param address PDI Register address
*
*/
PROTO void bsp_set_pdi_perm_read_only(uint16_t *perm_array, uint16_t address);

/**
*  @brief  Set the PDI register [byte] access permission to read and write
*
*  @param perm_array  Pointer to the Permissions array
*  @param address PDI Register address
*
*/
PROTO void bsp_set_pdi_perm_read_write(uint16_t *perm_array, uint16_t address);

/**
*  @brief  Checks if the PDI register [byte] has read only access permission and returns the result
*
*  @param perm_array  Pointer to the Permissions array
*  @retval 1 if the register has read only permission, 0 if otherwise
*
*/
PROTO uint8_t bsp_is_pdi_perm_read_only(uint16_t *perm_array, uint16_t address);


/**
*  @brief  Returns the count of PDI read access failures
*
*  @retval PDI read access fail count
*
*/
PROTO uint32_t bsp_get_pdi_read_access_fail_cnt();

/**
*  @brief  Returns the count of PDI write access failures
*
*  @retval PDI write access fail count
*
*/
PROTO uint32_t bsp_get_pdi_write_access_fail_cnt();

/**
@}
*/

/** @addtogroup ECAT_PRU_MDIO
 @{ */

/**
*  @brief  Initializes PRU-ICSS MDIO for EtherCAT firmware to communicate with PHYs. Must be called after powering on
*  PRU-ICSS domain and before PRU firmware is loaded and executed on both PRUs
*
*  @param pruIcssHandle PRUSS Handle
*  @param pmdio_params  MDIO configuration
*  @retval 0 on success, <0 on failure
*
*/
PROTO int16_t bsp_pruss_mdio_init(PRUICSS_Handle pruIcssHandle,
                                  t_mdio_params *pmdio_params);

/**
*  @brief  API to read PHY register via PRU-ICSS MDIO
*
*  @param pruIcssHandle PRUSS Handle
*  @param phyaddr Select the PHY to read using PHY address
*  @param regoffset  Register offset in PHY to read
*  @param regval  Pointer to the variable to hold the register value read
*  @retval  0 : Success -1 : MDIO access error
*
*/
PROTO int16_t bsp_pruss_mdio_phy_read(PRUICSS_Handle pruIcssHandle,
                                      uint8_t phyaddr, uint8_t regoffset, uint16_t *regval);

/**
*  @brief  API to write PHY register via PRU-ICSS MDIO
*
*  @param pruIcssHandle PRUSS Handle
*  @param phyaddr Select the PHY to read using PHY address
*  @param regoffset  Register offset in PHY to read
*  @param regval  Value to write to the PHY register
*  @retval  0 : Success -1 : MDIO access error
*
*/
PROTO int16_t bsp_pruss_mdio_phy_write(PRUICSS_Handle pruIcssHandle,
                                       uint8_t phyaddr, uint8_t regoffset, uint16_t regval);
/**
*  @brief  Get the link status for selected PHY, this API considers MII_link signal polarity differences and recommended
*  when TIESC_MDIO_RX_LINK_ENABLE is enabled for enhanced link detection
*
*  @param pruIcssHandle PRUSS Handle
*  @param phyaddr Select the PHY to read using PHY address
*  @retval  0: Link Down Otherwise: Link Up
*
*/
PROTO uint32_t bsp_pruss_mdio_phy_link_state(PRUICSS_Handle pruIcssHandle,
        uint8_t phyaddr);
/**
@}
*/

/** @addtogroup ECAT_PRU_DIGIO
 @{ */

/**
*  @brief  Configure digio for sw controlled dataout mode
*
*  @param pruIcssHandle PRUSS Handle
*/
PROTO  void bsp_set_digio_sw_dataout_enable(PRUICSS_Handle pruIcssHandle);
/**
*  @brief  Set selected digital output pin
*
*  @param pruIcssHandle PRUSS Handle
*  @param num digital output selection (num =0 to 7)
*/
PROTO void bsp_set_digio_out(PRUICSS_Handle pruIcssHandle, uint8_t num);
/**
*  @brief  Clear selected digital output pin
*
*  @param pruIcssHandle PRUSS Handle
*  @param num digital output selection (num =0 to 7)
*/
PROTO void bsp_clear_digio_out(PRUICSS_Handle pruIcssHandle, uint8_t num);
/**
@}
*/

/** @addtogroup ECAT_SPINLOCK
 @{ */
/**
*  @brief   Initialize SOC spinlock, enable clocks and init spinlock instance 0 through 7 to unlocked state
*/
PROTO void bsp_hwspinlock_init(void);
/**
*  @brief   Acquire selected spinlock instance
*
*  @param num Spinlock instance(0: DC latch 2: SM2 3: SM3 4: SM4 5: SM5 6: SM6 7: SM7)[Upto ISDK 1.1.0.6].Spinlock instance(0: DC latch ) [From ISDK  1.1.0.8]
*  @retval 0: success 1: already locked
*/
PROTO uint32_t bsp_hwspinlock_lock(int num);
/**
*  @brief  Release selected spinlock instance
*
*  @param num Spinlock instance
*/
PROTO void bsp_hwspinlock_unlock(int num);
/**
*  @brief  Returns the Spinlock Clock Ctrl register offset
*
*  @retval register offset
*/
uint32_t getSpinlockClkCtrlOffset();
/**
*  @brief  Returns spinlock Reg 0 register offset
*
*  @retval register offset
*/
uint32_t getSpinlockLockReg0Offset();
/**
@}
*/

/** @addtogroup ECAT_SYSTIME_PDI
 @{ */
#ifdef SYSTEM_TIME_PDI_CONTROLLED
/**
*  @brief  Enable latch single shot/continuous mode for Latch0 via PDI interface If bit is clear latch will be configured in continuous mode
*
*  @param pruIcssHandle PRUSS Handle
*  @param val Bitmask to enable singleshot mode [Bit0 : posedge Bit1: negedge]
*/
PROTO void bsp_pdi_latch0_control(PRUICSS_Handle pruIcssHandle, uint8_t val);

/**
*  @brief  Enable latch single shot/continuous mode for Latch1 via PDI interface If bit is clear latch will be configured in continuous mode
*
*  @param pruIcssHandle PRUSS Handle
*  @param val Bitmask to enable singleshot mode [Bit0 : posedge Bit1: negedge]
*/
PROTO void bsp_pdi_latch1_control(PRUICSS_Handle pruIcssHandle, uint8_t val);

/**
*  @brief  Write 32-bit system time to firmware to trigger drift compensation
*
*  @param pruIcssHandle PRUSS Handle
*  @param systime 32-bit system time value
*/
PROTO void bsp_pdi_write_system_time(PRUICSS_Handle pruIcssHandle,
                                     uint32_t systime);
/**
*  @brief Write 64-bit System Time Offset (0x920) register and indicate to the firmware
*
*  @param pruIcssHandle PRUSS Handle
*  @param systime 64-bit system time value
*/
PROTO void bsp_pdi_write_system_timeoffset(PRUICSS_Handle pruIcssHandle,
        unsigned long long systime);
/**
*  @brief Write 32-bit System Time Delay (0x928) register
*
*  @param pruIcssHandle PRUSS Handle
*  @param systime 32-bit system time delay value
*/
PROTO void bsp_pdi_write_systime_delay(PRUICSS_Handle pruIcssHandle,
                                       uint32_t systime);
/**
*  @brief Initialize the System Time and SpeedCounter Diff filters from PDI side
*
*  @param pruIcssHandle PRUSS Handle
*  @param speedcount_start 16-bit Speed Counter Start value at (0x930)
*  @param speedcount_filtdepth Filter depth 0-16 (0x935)
*  @param systime_filtdepth Filter depth 0-16 (0x934)
*/
PROTO void bsp_pdi_write_filterconfig(PRUICSS_Handle pruIcssHandle,
                                      uint16_t speedcount_start,
                                      uint8_t speedcount_filtdepth, uint8_t systime_filtdepth);
#endif
/**
@}
*/

/** @addtogroup ECAT_TIMER
 @{ */
/**
*  @brief Returns the time difference from last call of "bsp_clear_timer_register" to this "bsp_get_timer_register". handles overflow
*
*  @retval 32-bit time elapsed since last call to bsp_clear_timer_register
*/
PROTO uint32_t bsp_get_timer_register(void);
/**
*  @brief Update the time when "bsp_clear_timer_register" last invoked.This is a wrapper API used by SSC. Implemented using SYS/BIOS API
*/
PROTO void bsp_clear_timer_register(void);
/**
*  @brief  Return EtherCAT time base for application use
*
*  @param systime_low stores system time low (32-bit LSW)
*  @param systime_high stores system time high (32-bit MSW)
*/
PROTO void bsp_get_local_sys_time(uint32_t *systime_low,
                                  uint32_t *systime_high);
/**
*  @brief  Return latch0 posedge timestamp for application use(nanosec resolution)
*
*  @param pruIcssHandle PRUSS handle
*  @param systime_low stores system time low (32-bit LSW)
*  @param systime_high stores system time high (32-bit MSW)
*/
PROTO void bsp_get_latch0_posedge_time(PRUICSS_Handle pruIcssHandle,
                                       uint32_t *systime_low, uint32_t *systime_high);

/**
*  @brief  Return latch0 negedge timestamp for application use(nanosec resolution)
*
*  @param pruIcssHandle PRUSS handle
*  @param systime_low stores system time low (32-bit LSW)
*  @param systime_high stores system time high (32-bit MSW)
*/
PROTO void bsp_get_latch0_negedge_time(PRUICSS_Handle pruIcssHandle,
                                       uint32_t *systime_low, uint32_t *systime_high);

/**
*  @brief  Return latch1 posedge timestamp for application use(nanosec resolution)
*
*  @param pruIcssHandle PRUSS handle
*  @param systime_low stores system time low (32-bit LSW)
*  @param systime_high stores system time high (32-bit MSW)
*/
PROTO void bsp_get_latch1_posedge_time(PRUICSS_Handle pruIcssHandle,
                                       uint32_t *systime_low, uint32_t *systime_high);

/**
*  @brief  Return latch0 negedge timestamp for application use(nanosec resolution)
*
*  @param pruIcssHandle PRUSS handle
*  @param systime_low stores system time low (32-bit LSW)
*  @param systime_high stores system time high (32-bit MSW)
*/
PROTO void bsp_get_latch1_negedge_time(PRUICSS_Handle pruIcssHandle,
                                       uint32_t *systime_low, uint32_t *systime_high);

/**
@}
*/

/** @addtogroup ECAT_MUTEX
 @{ */

/**
*  @brief  Critical section enter API using semaphore/mutex/interrupt disable primitives from RTOS.
*  Implemented using SYS/BIOS API "GateAll_enter" in SDK
*/
PROTO void bsp_global_mutex_lock(void);

/**
*  @brief  Critical section leave API using semaphore/mutex/interrupt enable primitives from RTOS.
*  Implemented using SYS/BIOS API "GateAll_leave" in SDK
*/
PROTO void bsp_global_mutex_unlock(void);
/**
@}
*/

/** @addtogroup ECAT_INTMGMNT
 @{ */
/**
*  @brief    SYNC0 IRQ handler
*/
void Sync0Isr(void *args);
/**
*  @brief    SYNC1 IRQ Handler
*/
void Sync1Isr(void *args);
/**
*  @brief    ECAT IRQ Handle ?
*/
void EcatIsr(void *args);

#ifndef SUPPORT_CMDACK_POLL_MODE
/**
*  @brief    ESC CMD Low ACK IRQ Handle ?
*/
void EscCmdLowAckIsr(void *args);
#endif

/**
*  @brief    Returns the ARM Interrupt offset
*
*  @retval ARM interrupt offset
*/
int16_t getARMInterruptOffset();
/**
@}
*/


/** @addtogroup ECAT_PRU_FW
 @{ */
/**
*  @brief    This function internally sets the location from which PRU firmwares can be loaded.If this API is not called,
*  Application will try to load PRU firmwares from SPI flash.
*
*  @param frameProc Pointer to a buffer containing HRT PRU firmware (ecat_frame_handler_bin.h)
*  @param frameProcLen Size of the HRT PRU firmware
*  @param hostProc Pointer to a buffer containing SRT PRU firmware (ecat_host_interface_bin.h)
*  @param hostProcLen Size of the SRT PRU firmware
*/
PROTO void bsp_set_pru_firmware(uint32_t *frameProc, uint32_t frameProcLen,
                                uint32_t *hostProc, uint32_t hostProcLen);
/**
@}
*/

/** @addtogroup ECAT_FW_UPGRADE
 @{ */
#ifdef ENABLE_ONLINE_FIRMWARE_UPGRADE
/**
*  @brief   Called by EtherCAT stack when a firmware download command is received from EtherCAT master
*
*  @param password password received from master
*/
PROTO void bsp_start_fw_download(uint32_t password);
/**
*  @brief   Stores the received data as part of firmware download from EtherCAT master. This may get called multiple times during firmware download. This function
*  performs SPI flash writes for every 256-bytes received from master.
*
*  @param pData Pointer to data buffer
*  @param Size  Number of bytes received
*/
PROTO void bsp_store_fw_data(uint16_t *pData, uint16_t Size);
/**
*  @brief    Initiates the Application restart when EtherCAT state is changed from BOOTSTRAP
*  to INIT after application update using FoE protocol. This function performs SPI flash write
*  for remaining bytes in receive data buffer once firmware download is finished by EtherCAT master
*/
PROTO void bsp_boot_2_init_handler(void);
#endif
/**
@}
*/

/** @addtogroup ECAT_PHY_CONFIG
 @{ */
/**
*  @brief    This API configures port0 and port1 phys for enhanced link detection, enable RXERR detection during IDLE,
*   disable detection of transmit error in odd-nibble boundary (for odd nibble insertion) and fast RXDV detection.
*   This is called only during initialization.
*
*   @param pruIcssHandle PRUSS Handle
*   @param phy0addr Phy address of port0
*   @param phy1addr Phy address of port1
*   @param enhancedlink_enable Fast link detect (enhanced link detection) enable/disable flag
*/
void bsp_ethphy_init(PRUICSS_Handle pruIcssHandle, uint8_t phy0addr,
                     uint8_t phy1addr,
                     uint8_t enhancedlink_enable);
/**
@}
*/


PROTO int16_t bsp_get_sm_index(uint16_t address, uint16_t len);
/** @addtogroup ECAT_MISC
@{ */

/**
*  @brief Checks if the SOC has ECAT capability
*
*  @retval 0 on failure, 1 on success
*/
uint8_t isEtherCATDevice(void);

/**
@}
*/


#ifdef TIESC_SPI_SLAVE_MODE
/** @addtogroup ECAT_TIESC_SPI
@{ */



/**
*  @brief    This function is used by the SPI slave thread to write to the ESC memory.
*
*  @param address ESC Address to write
*  @param write_buffer Pointer to buffer to be written to ESC memory
*  @param data_length Length of data to be written to ESC memory
*/
uint8_t spi_bsp_slave_write(uint16_t address, uint8_t *write_buffer,
                            uint8_t data_length);

/**
*  @brief    This function is used in TIESC SPI Mode for EEPROM handling on SPI slave side
*
*/
void spi_bsp_eeprom_event_handler(void);


/**
*  @brief    This function is used by the SPI slave during read transfer to initialize
*            the read state machine
*
*/
uint8_t spi_bsp_slave_read_init(void);

/**
*  @brief    This function is used by the SPI slave to do the first 4 bytes of read during
*            read transfer
*
*  @param address ESC Address to read
*  @param read_buf Pointer to buffer to write the read ESC memory data
*/
uint8_t spi_bsp_slave_read_start(uint16_t address, uint8_t *read_buf);

/**
*  @brief    This function is used by the SPI slave to do the next num_bytes bytes of read
*            after the first read
*
*  @param start_address ESC Address to read
*  @param read_buf Pointer to buffer to write the read ESC memory data
*  @param num_bytes Number of bytes to read
*/
uint8_t spi_bsp_slave_read_next(uint16_t start_address, uint8_t *read_buf,
                                uint16_t num_bytes);

/**
*  @brief    This function is used by the SPI slave to finish the read transfer
*
*  @param start_address ESC Address to read
*  @param length Number of bytes in the complete read transfer
*/
uint8_t spi_bsp_slave_read_end(uint16_t start_address, uint16_t length);

/**
*  @brief    This function is used by the SPI slave to get the Physical address of the
*            memory location to write the data
*
*  @param address ESC Address to write
*  @param physical_address Pointer to variable to return physical address
*/
uint8_t spi_bsp_slave_write_start(uint16_t address, uint32_t *physical_address);

/**
*  @brief    This function is used by the SPI slave to finish the write transfer
*
*  @param start_address ESC Address to write
*  @param write_buf Pointer to memory location where data was written
*  @param length Number of bytes in the complete write transfer
*/
uint8_t spi_bsp_slave_write_end(uint16_t start_address, uint8_t *write_buf,
                                uint16_t length);

/**
*  @brief    This function is used by the SPI slave to poll and service the PDI Interrupt received from PRU
*
*/
uint8_t bsp_spi_poll_pdi_isr(void);

/**
*  @brief    This function is used by the SPI slave to write a byte to ESC memory
*
*  @param ecat_addr ESC Address to write
*  @param write_physical_addr_ptr Pointer to memory location where data byte is to be written
*  @param data The data value to be written
*/
PROTO uint8_t spi_bsp_slave_write_byte(uint16_t ecat_addr,
                                       uint8_t *write_physical_addr_ptr, uint8_t data);

/**
*  @brief  Invoked by the SPI slave after reading a register or mailbox buffer from PDI side
*
*  @details Handles following registers \n
*  SM WD status \n
*  AL control   \n
*  SYNC0/1 status   \n
*  SM Activate registers    \n
*  Latch0/1 time Postive/Negative edge  \n
*  Also detects read mailbox start address access from PDI side
*
*  @param pruIcssHandle PRUSS Handle
*  @param start_address ESC address read from PDI
*  @param length Length of the data read from start_address
*
*/
void bsp_spi_pdi_post_read_indication(PRUICSS_Handle pruIcssHandle,
                                      uint16_t start_address, uint16_t length);

/**
*  @brief    This function is used by the SPI slave to get SPI initialization parameters from EEPROM Config data
*
*/
uint8_t bsp_spi_get_slave_configuration(void);
#endif

/**
@}
*/
#endif/*_TIESCBSP_H_ */
