/**
 * @file tiesc_spi.h
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
#ifndef _TIESC_SPI_H_
#define _TIESC_SPI_H_

/*-----------------------------------------------------------------------------------------
------
------  Includes
------
-----------------------------------------------------------------------------------------*/
#ifndef DISABLE_UART_PRINT
#include <stdio.h>
#include <stdbool.h>
#endif

/*-----------------------------------------------------------------------------------------
------
------  Defines and Types
------
-----------------------------------------------------------------------------------------*/
#define MAX_SPI_LENGTH                  270
#define SPI_BIT_RATE                    4000000
/* These defines are used as the check bytes in the master slave transaction */
#define SLAVE_ADDR_READY_BYTE           0x75 /*Magic number*/
#define SLAVE_DATA_READY_BYTE           0x79 /*Magic number*/
#define SLAVE_BUSY_BYTE                 0x71 /*Magic number*/
#define MASTER_ADDR_READY_BYTE          0x19 /*Magic number*/
#define MASTER_DATA_READY_BYTE          0x15 /*Magic number*/
#define SLAVE_ADDR_LAST_BYTE            0x85 /*Magic number*/
#define SLAVE_DATA_LAST_BYTE            0x89 /*Magic number*/
#define MASTER_ADDR_LAST_BYTE           0x59 /*Magic number*/
#define MASTER_DATA_LAST_BYTE           0x55 /*Magic number*/

#ifdef TIESC_SPI_MASTER_MODE
#define STATUS_SLAVE_ADDR_READY         1
#define STATUS_SLAVE_DATA_READY         2

#define ADDR_MODE                       0
#define DATA_MODE                       1

#define READ_CMD(x)                     x = x & ~(0x01)
#define WRITE_CMD(x)                    x = x | (0x01)
#define SET_LENGTH(x, len)              x = x | (len << 1);

#define SLAVE_CHECK_MAX_COUNT           10000
#define SLAVE_RW_RETRY_MAX_COUNT        10
#define SPI_BSP_MAX_LENGTH              256
#define BOARD_MCSPI_MASTER_INSTANCE     2

#define MASTER_WAIT_STATE_BYTE          0x55
#endif
/*
 * Number of wait state bytes is dependent on SPI Baud Rate
 * Default value of 3 bytes corresponds to SPI Baud Rate of 4MHz
 * Refer to User Guide for number of wait state bytes needed
 * for different SPI Baud rates
 */
#define NUM_WAIT_STATE_BYTES            3

#define ADDRESSS_CMD_TWO_BYTES          2
#define ADDRESSS_CMD_THREE_BYTES        3
#define CHIP_SELECT_POLARITY_HIGH       1
#define CHIP_SELECT_POLARITY_LOW        0
#define CHIP_SELECT_POLARITY            CHIP_SELECT_POLARITY_LOW

#ifdef TIESC_SPI_SLAVE_MODE
#define SPI_HW_ACCESS_TASK_PRIORITY         3
#define SPI_HW_ACCESS_TASK_STACK_SIZE       4096
#define SPI_SLAVE_STATUS_TASK_PRIORITY      3
#define SPI_SLAVE_STATUS_TASK_STACK_SIZE    1024

#define STATUS_MASTER_ADDR_READY        1
#define STATUS_MASTER_DATA_READY        2

#define STATUS_ADDR_READY               SLAVE_ADDR_READY_BYTE
#define STATUS_DATA_READY               SLAVE_DATA_READY_BYTE
#define STATUS_DATA_BUSY                SLAVE_BUSY_BYTE

#define READ_WRITE_MASK                 0x01
#define SPI_READ_CMD                    0x00
#define SPI_WRITE_CMD                   0x01
#define GET_LENGTH(x)                   (x >> 1)

#define BOARD_MCSPI_SLAVE_INSTANCE      2

#define SPI_FIFO_RX_LEVEL               2
#define SPI_FIFO_TX_LEVEL               15

#define SPI_CS_GPIO_PORT                SOC_GPIO_0_REGS
#define SPI_CS_GPIO_PIN                 29
#endif


#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_SHIFT         0x00
#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_MASK          0x03
#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_0             0x00
#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_1             0x01
#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_2             0x02
#define PDI_SPI_SLAVE_CONFIG_SPI_MODE_3             0x03

#define PDI_SPI_SLAVE_CONFIG_SPI_IRQ_POL_SHIFT      0x02
#define PDI_SPI_SLAVE_CONFIG_SPI_IRQ_POL_MASK       0x0C

#define PDI_SPI_SLAVE_CONFIG_SPI_SEL_POL_SHIFT      0x04
#define PDI_SPI_SLAVE_CONFIG_SPI_SEL_POL_MASK       0x10


typedef struct
{
    uint8_t SPI_POL;
    uint8_t SPI_PHA;
    uint8_t SPI_CS_active_polarity;
    uint8_t PDI_IRQ_active_polarity;
    uint8_t PDI_IRQ_deactive_polarity;
} t_spi_slave_config;

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
#ifdef TIESC_SPI_MASTER_MODE
/** @addtogroup ECAT_TIESC_SPI
@{ */
/**
*  @brief    This function is used in TI ESC SPI Mode by the SPI master to read from SPI slave
*
*  @param address ESC Address to read from
*  @param readBuffer Pointer to buffer for data to be read from ESC memory
*  @param length Length of data to be read from ESC memory
*/
int8_t spi_master_bsp_read(uint16_t address, uint8_t *readBuffer,
                           uint16_t length);

/**
*  @brief    This function is used in TI ESC SPI Mode by the SPI master to write to SPI slave
*
*  @param address ESC Address to write
*  @param writeBuffer Pointer to buffer to be written to ESC memory
*  @param length Length of data to be written to ESC memory
*/
int8_t spi_master_bsp_write(uint32_t address, uint8_t *writeBuffer,
                            uint16_t length);

/**
*  @brief Setup SPI peripheral as Master for the TIESC SPI Master Mode EtherCAT Application
*
*/
void SPIMaster_open(void);

/**
@}
*/

#endif

#ifdef TIESC_SPI_SLAVE_MODE
/**
*  @brief Setup SPI peripheral as Slave for the TIESC SPI Slave Mode EtherCAT Application
*
*/
void SPISlave_open(void);

/**
*  @brief sysbios Task to handle the TIESC SPI Slave status transactions over SPI
*
*/
#ifdef iceAMIC11x_onChip_mem_mode
void SPISlave_statusTask(uint32_t arg0,
                         uint32_t arg1) __attribute__((section(".tasks")));
#else
void SPISlave_statusTask(uint32_t arg0, uint32_t arg1);
#endif

#endif

#endif /* _TIESC_SPI_H_ */
