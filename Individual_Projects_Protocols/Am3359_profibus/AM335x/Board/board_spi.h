/**
 *  \file   board_spi.h
 *
 *  \brief:
 *  Board SPI APIs
 *
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
#ifndef _BOARD_SPI_H_
#define _BOARD_SPI_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifdef SOC_K2G
#include <ti/board/src/flash/include/board_flash.h>
#elif defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM437x)
#include <ti/drv/spi/SPIver.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>
#endif

typedef int32_t Board_flash_STATUS;       /** Board Flash API return type */

#define BOARD_FLASH_EINVALID      -3      /**< Error code for invalid parameters */
#define BOARD_FLASH_EUNSUPPORTED  -2      /**< Error code for unsupported feature */
#define BOARD_FLASH_EFAIL         -1      /**< General failure code */
#define BOARD_FLASH_EOK           0       /**< General success code */


#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM335x) || defined(SOC_AM437x)

/**
 * @brief Options to set qspi flash read/write access mode
 */
typedef enum
{
BOARD_FLASH_QSPI_IO_MODE_SINGLE,
/**< QSPI flash read/write access on single I/O line */
BOARD_FLASH_QSPI_IO_MODE_DUAL,
/**< QSPI flash read/write access on two I/O lines */
BOARD_FLASH_QSPI_IO_MODE_QUAD
/**< QSPI flash read/write access on four I/O lines */
} Board_flashQspiIoMode;


#if defined(SOC_AM335x)
typedef uint32_t Board_flashHandle;
#else

typedef S25FL_Handle Board_flashHandle;


/***********************************************************************/
/* Functions                                                           */
/***********************************************************************/
/**
 * @brief API used for writing data into QSPI
 *
 * @param flashHandle QSPI Flash handle
 * @param offset QSPI offset to write
 * @param length Lenght to write
 * @param buffer Data to write
 *  @param[in] eraseFlag
 *      If set to 1, this API will handling erasing the flash before writing
 *      If set to 0, application should handle this scenario
 * @retval TRUE if success
 */
bool Board_writeFlashStorage(S25FL_Handle flashHandle, uint32_t offset,
                             uint32_t length, uint8_t *buffer, uint8_t eraseFlag);

/**
 * @brief API used for reading data from QSPI
 *
 * @param flashHandle QSPI Flash handle
 * @param offset QSPI offset to read
 * @param length Lenght to read
 * @param buffer Data to read
 * @retval TRUE if success
 */
bool Board_readFlashStorage(S25FL_Handle flashHandle, uint32_t offset,
                            uint32_t length, uint8_t *buffer);

#endif
#endif

#if defined(SOC_AM572x) || defined(SOC_AM571x) || defined(SOC_AM335x) || defined(SOC_AM437x)

/**
 *  @brief       Opens a flash device for use
 *
 *  @param[in]   deviceId  Flash device ID to open
 *  @param[in]   portNum   Peripheral port number attached to the flash device
 *  @param[in]   params    configuration parameters for the periperal interface
 *
 *  @retval      NULL or Board_flashHandle.
 *
 *  @remark
 *               On success a handle is returned in which should be used in
 *               all subsequent calls. As of now, the devices are not virtualized and only
 *               one open may exist at a time for a particular device.
 *
 */
Board_flashHandle Board_flashOpen(uint32_t  deviceId,
                                  uint32_t  portNum,
                                  void     *params);

/**
 *  @brief       Closes the device
 *
 *  @param[in]   handle  Handle to the device as returned in the open call.
 *
 *  @retval      BOARD_FLASH_EOK on Success
 *
 */
Board_flash_STATUS Board_flashClose(Board_flashHandle handle);

/**
 *  @brief       Reads a page from the device
 *
 *  @param[in]   handle Flash device handle from the open
 *  @param[in]   offset Offset to start the read from
 *  @param[in]   buf    Pointer to a buffer to read the data into
 *  @param[in]   len    Amount of data to read
 *
 *
 *  @retval      BOARD_FLASH_EOK on Success
 *
 *  @remark      The buffer size should be page_size + spare_size
 *               The application should not write into the spare area
 *
 */
Board_flash_STATUS Board_flashRead(Board_flashHandle  handle,
                                   uint32_t           offset,
                                   uint8_t           *buf,
                                   uint32_t           len,
                                   void              *params);

/**
 *  @brief      Write the data to the device
 *
 *  @param[in]  offset      Offset to start writing the data at.
 *  @param[in]  buf         Pointer to  data to write
 *  @param[in]  len         Length of the data pointed to by buf
 *
 *  @retval     BOARD_FLASH_EOK on Success
 *
 */
Board_flash_STATUS Board_flashWrite(Board_flashHandle  handle,
                                    uint32_t           offset,
                                    uint8_t           *buf,
                                    uint32_t           len,
                                    void              *params);


/**
 *  @brief       erase a block on the flash block
 *
 *  @param[in]   handle  Flash device handle from the open
 *
 *  @param[in]   block_number Block ID to erase
 *
 *  @retval      BOARD_FLASH_EOK on Success
 *
 */
Board_flash_STATUS Board_flashEraseBlk(Board_flashHandle handle,
                                       uint32_t          block_number);



/**
 *  @brief      Convert the offset to block and page number
 *
 *  @param[in]  handle  Handle to the device as returned by open
 *
 *  @param[in]  offset      Offset to start writing the data at.
 *
 *  @param[in]  block       Pointer to the block number
 *
 *  @param[in]  page        Pointer to the Page number (not implemented)
 *
 *  @retval     BOARD_FLASH_EOK on Success
 *
 *
 */
Board_flash_STATUS Board_flashOffsetToBlkPage(Board_flashHandle  handle,
        uint32_t           offset,
        uint32_t          *block,
        uint32_t          *page);


/**
 * @brief Crossbar config for QSPI
 */
void QSPI_board_crossbarInit(void);

#endif

/**
 * @brief Initializes QSPI module. The handle is global in this file @flashHandle, and can be used by the application.
 *
 */
void QSPI_init();

void LoadData(void);

void Board_readHVS(uint8_t *switches);

#endif /* _BOARD_SPI_H_*/
