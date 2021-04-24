/**
 *  \file   board_spi.c
 *
 *  \brief  AM335x ICEV2 Board specific GPIO parameters.
 *
 *   This file contains the GPIO hardware parameters specific to board.
 */

/*
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdbool.h>
#include <ti/csl/soc.h>
#include <ti/drv/gpio/GPIO.h>
#include <examples/board/include/board_spi.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/starterware/include/board.h>
#include <ti/starterware/include/device.h>
/* Flash header file */
#include <examples/board/include/board_mcspi.h>
#include <examples/board/include/board_platform.h>
#include <examples/board/include/delay_us.h>
#include <examples/board/include/board_gpioLed.h>

Board_flashHandle boardFlashHandle;

/* Flash header file */


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** \brief */
#define SPIFLASH_SIZE             0x00800000
/** \brief */
#define SPIFLASH_BASE             0x00000000
/** \brief */
#define SPIFLASH_PAGESIZE         256
/** \brief */
#define SPIFLASH_SECTORSIZE       4096

/** \brief */
#define SPIFLASH_PAGEMASK         0xffffffc0
/** \brief */
#define SPIFLASH_MANUF_ID         0xEF

/** \brief */
#define MFID_DEVID_LEN      6
/** \brief */
#define SPI_WPEN_LEN        1
/** \brief */
#define SPI_STATUS_LEN      2
/** \brief */
#define SPI_ERASE_LEN       4
/** \brief */
#define TX_EMPTY_FLAG       0x02
/** \brief */
#define RX_FULL_FLAG        0x01
/** \brief */
#define SHIFT_BYTE          8
/** \brief */
#define SHIFT_WORD          16

/* SPI FLASH Commands */
/** \brief */
#define SPIFLASH_CMD_WRSR         0x01
/** \brief */
#define SPIFLASH_CMD_WRITE        0x02
/** \brief */
#define SPIFLASH_CMD_READ         0x03
/** \brief */
#define SPIFLASH_CMD_WRDI         0x04
/** \brief */
#define SPIFLASH_CMD_RDSR         0x05
/** \brief */
#define SPIFLASH_CMD_WREN         0x06
/** \brief */
#define SPIFLASH_CMD_READID       0x90
/** \brief */
#define SPIFLASH_CMD_ERASE        0xD8
/** \brief */
#define SPIFLASH_CMD_SCTR_ERASE   0x20

/** \brief */
#define FLASH_STATUS        0x01
/** \brief Write Enable Latch*/
#define FLASH_WEL_STATUS        0x02


/** \brief Chip select */
#define SPI_FLASH_CS 0

/** \brief Clock Divide Value For Clock 24MHZ*/
#define SPI_FLASH_CD 2

/** \brief DIRECTION PINS FOR SPI*/
#define SPI_FLASH_DIR 0


/*************************************************************************/
/** \brief SPI Flash intialization status macros. */
#define SPI_FALSH_NOT_INITALIZED    0
#define SPI_FLASH_INITALIZED        1

/** \brief SPI Flash device instance number */
#define MCSPI_FLASH_INST_NUM     (0U)


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/** \brief */
static uint8_t spiFlashTxBuf[SPIFLASH_PAGESIZE + 5];
/** \brief */
static uint8_t spiFlashRxBuf[SPIFLASH_PAGESIZE + 5];
/** \brief */
static uint8_t statusbuf[8];

/** \brief */
volatile uint8_t spiFlashInitStatus = SPI_FALSH_NOT_INITALIZED;

mcspiCfgObj_t *pFlashMcspiObj;

extern SPI_Handle handle;                   /* SPI handle */

const mcspiCfgObj_t MCSPI_DEFAULT =
{
    0U,                                      /* instNum.*/
    0U,                                      /* instAddr.*/
    48000000U,                               /* inClk.*/
    1000000U,                                /* outClk.*/
    0U,                                      /* channelNum.*/
    0U,                                      /* dataLength.*/
    NULL,                              /* pTx.*/
    NULL,                              /* rTx.*/
    {
        MCSPI_MODULCTRL_SINGLE_SINGLE,                     /* channel.*/
        MCSPI_TX_RX_MODE,                   /* txRxMode.*/
        MCSPI_DATA_LINE_COMM_MODE_7,         /* pinMode. */
        MCSPI_CLK_MODE_2,                    /* clkMode.*/
        8U,                                  /* wordLength.*/
        MCSPI_CS_POL_LOW,                    /* csPolarity.*/
        MCSPI_TX_FIFO_ENABLE,                                /* txFifoCfg.*/
        MCSPI_RX_FIFO_DISABLE,                                /* rxFifoCfg.*/
        0             /* interrupt.*/
    },
    {
        INTC_TRIG_HIGH_LEVEL,                /* trigType.*/
        0U,                                  /* intrLine.*/
        10U,                                 /* intrPriority.*/
        FALSE,                               /* isIntrSecure.*/
        NULL,                                /* pTxBuf.*/
        NULL,                                /* pRxBuf.*/
        NULL                                 /* pFnIntrHandler.*/
    },
    {
        TRUE,                                /* csFlag.*/
        0,                   /* modeFlag.*/
        MCSPI_INTERRUPT_MODE                 /* comFlag.*/
    }
};




void Board_readSpiFlashPage(uint32_t src, uint32_t dst,
                            uint32_t length, mcspiCfgObj_t *pMcspiObj);

uint8_t Board_readSpiFlashID(mcspiCfgObj_t *pMcspiObj);

uint8_t Board_eraseSpiFlash(uint32_t offset, mcspiCfgObj_t *pMcspiObj);

void Board_writeSpiFlashPage(uint32_t src, uint32_t dst,
                             uint32_t length,  mcspiCfgObj_t *pMcspiObj);

uint8_t Board_eraseSpiFlashSector(uint32_t mode, uint32_t offset,
                                  uint32_t length, mcspiCfgObj_t *pMcspiObj);


void LoadData(void)
{
    GPIO_write(6, 0);
    delay_us(1);
    GPIO_write(6, 1);
    delay_us(1);
    return;
}

void Board_readHVS(uint8_t *switches)
{
    /* MCSPI params required */
    /* Buffer containing the known data that needs to be written to flash */
    uint8_t txBuf[1U];
    /* Buffer containing the received data */
    uint8_t rxBuf[1U] = {0xFFU};
    /* Transfer length */
    uint32_t transferLength;
    SPI_Transaction transaction;         /* SPI transaction */

    /* Load data */
    LoadData();

    /* Initiate transfer */
    txBuf[0] = 0xAAU;
    transferLength = 1U;

    transaction.count = transferLength;
    transaction.txBuf = &txBuf[0];
    transaction.rxBuf = &rxBuf[0];
    SPI_transfer(handle, &transaction);

    /*ICev2 has 7 pins*/
    *switches = *rxBuf & 0x7F;

}


Board_flashHandle Board_flashOpen(uint32_t  deviceId,
                                  uint32_t  portNum,
                                  void     *params)
{
    uint32_t retVal;
    uint32_t  instAddr;
    uint32_t mcspiInstNum;
    uint32_t mcspiCs;

    pFlashMcspiObj = (mcspiCfgObj_t *)malloc(sizeof(mcspiCfgObj_t));
    *pFlashMcspiObj = MCSPI_DEFAULT;

    Board_initPlatform(CHIPDB_MOD_ID_MCSPI, (uint32_t)DEVICE_ID_W25Q64BV,
                       MCSPI_FLASH_INST_NUM,
                       &mcspiInstNum, &mcspiCs, &instAddr);

    pFlashMcspiObj->instNum = mcspiInstNum;
    pFlashMcspiObj->channelNum  = mcspiCs;
    pFlashMcspiObj->instAddr = instAddr;

    /* Initialize the SPI interface */
    pFlashMcspiObj->pCfg.channel = MCSPI_MODULCTRL_SINGLE_SINGLE;
    pFlashMcspiObj->pCfg.pinMode = MCSPI_DATA_LINE_COMM_MODE_1;
    pFlashMcspiObj->outClk = 24000000U;
    pFlashMcspiObj->pCfg.clkMode = MCSPI_CLK_MODE_3;

    Board_initMCSPI(pFlashMcspiObj);


    if(Board_readSpiFlashID(pFlashMcspiObj))
    {
        return -1;
    }

    spiFlashInitStatus = SPI_FLASH_INITALIZED;
    retVal = (uint32_t) pFlashMcspiObj;
    return retVal;
}

Board_flash_STATUS Board_flashClose(Board_flashHandle handle)
{
    return BOARD_FLASH_EOK;
}

Board_flash_STATUS Board_flashRead(Board_flashHandle  handle,
                                   uint32_t           offset,
                                   uint8_t           *buf,
                                   uint32_t           len,
                                   void              *params)
{
    int32_t   no_of_pages;
    short   i, itr = 0  ;
    uint8_t  *p8;
    uint32_t bytes_to_copy = 0;

    if(SPI_FALSH_NOT_INITALIZED == spiFlashInitStatus || (buf == NULL))
    {
        return BOARD_FLASH_EFAIL;
    }

    pFlashMcspiObj->dataLength = len;

    no_of_pages = (pFlashMcspiObj->dataLength) / SPIFLASH_PAGESIZE + (((
                      pFlashMcspiObj->dataLength) % SPIFLASH_PAGESIZE) ? 1 : 0);


    i = (offset / SPIFLASH_PAGESIZE);
    p8 = (uint8_t *)buf;

    while(itr < no_of_pages)
    {
        bytes_to_copy = (pFlashMcspiObj->dataLength) - (itr * SPIFLASH_PAGESIZE);

        if(bytes_to_copy > 256)
        {
            bytes_to_copy = 256;
        }

        Board_readSpiFlashPage((i * SPIFLASH_PAGESIZE), (uint32_t)p8,
                               bytes_to_copy, pFlashMcspiObj);

        p8 += bytes_to_copy;
        i++;
        itr++;
    }

    return BOARD_FLASH_EOK;

}


Board_flash_STATUS Board_flashWrite(Board_flashHandle  handle,
                                    uint32_t           offset,
                                    uint8_t           *buf,
                                    uint32_t           len,
                                    void              *params)
{
    int32_t   no_of_pages;
    short   i, itr = 0  , itr2 = 0;
    uint8_t tx[256];
    uint8_t  *p8;
    uint32_t bytes_to_copy = 0;

    if(SPI_FALSH_NOT_INITALIZED == spiFlashInitStatus)
    {
        return BOARD_FLASH_EFAIL;
    }

    pFlashMcspiObj->dataLength = len;
    pFlashMcspiObj->pTx = buf;

    no_of_pages = (pFlashMcspiObj->dataLength) / SPIFLASH_PAGESIZE + (((
                      pFlashMcspiObj->dataLength) % SPIFLASH_PAGESIZE) ? 1 : 0);

    i = (offset / SPIFLASH_PAGESIZE);

    while(itr < no_of_pages)
    {
        p8 = (uint8_t *) tx;

        bytes_to_copy = len - (itr * SPIFLASH_PAGESIZE);

        if(bytes_to_copy > 256)
        {
            bytes_to_copy = 256;
        }

        for(itr2 = 0 ; itr2 < bytes_to_copy; itr2++)
        {
            p8[itr2] = pFlashMcspiObj->pTx[(itr * SPIFLASH_PAGESIZE) + itr2];
        }


        Board_writeSpiFlashPage((uint32_t)p8 , i * SPIFLASH_PAGESIZE,
                                bytes_to_copy, pFlashMcspiObj);

        i++;
        itr++;
    }

    return BOARD_FLASH_EOK;
}




Board_flash_STATUS Board_flashEraseBlk(Board_flashHandle handle,
                                       uint32_t          block_number)
{
    Board_eraseSpiFlashSector(0, block_number * SPIFLASH_SECTORSIZE,
                              SPIFLASH_SECTORSIZE, pFlashMcspiObj);
    return BOARD_FLASH_EOK;
}




Board_flash_STATUS Board_flashOffsetToBlkPage(Board_flashHandle  handle,
        uint32_t           offset,
        uint32_t          *block,
        uint32_t          *page)
{
    *block = offset / SPIFLASH_SECTORSIZE;
    *page = 0;

    return BOARD_FLASH_EOK;
}





static uint8_t Board_statusSpiFlash(mcspiCfgObj_t *pMcspiObj)
{
    /* Issue read status command */
    statusbuf[0] = SPIFLASH_CMD_RDSR;
    statusbuf[1] = 0;
    delay_us(20);

    Board_readMCSPI(pMcspiObj, statusbuf, SPI_STATUS_LEN);
    return statusbuf[1];
}


static uint8_t Board_writeOKSPIFlash(mcspiCfgObj_t *pMcspiObj)
{
    return (Board_statusSpiFlash(pMcspiObj) & FLASH_WEL_STATUS);
}





void Board_writeSpiFlashPage(uint32_t src, uint32_t dst,
                             uint32_t length,  mcspiCfgObj_t *pMcspiObj)
{
    int32_t i;
    int32_t bytesLeft;
    int32_t bytesToProgram;
    uint8_t *psrc;
    uint16_t quitLoop;

    /* Establish source */
    psrc = (uint8_t *)src;
    bytesLeft = length;

    while(bytesLeft > 0)
    {
        bytesToProgram = bytesLeft;

        /* Most to program is SPIFLASH_CMD_BLOCKSIZE */
        if(bytesToProgram > SPIFLASH_PAGESIZE)
        {
            bytesToProgram = SPIFLASH_PAGESIZE;
        }

        /* Make sure you don't run off the end of a block */
        if((dst & SPIFLASH_PAGEMASK) != ((dst + bytesToProgram)
                                         & SPIFLASH_PAGEMASK))

            bytesToProgram -= (dst + bytesToProgram) -
                              ((dst + bytesToProgram) & SPIFLASH_PAGEMASK);

        quitLoop = 0;

        /* Issue WPEN */
        if(!Board_writeOKSPIFlash(pMcspiObj))
        {
            spiFlashTxBuf[0] = SPIFLASH_CMD_WREN;
            Board_writeMCSPI(pMcspiObj, spiFlashTxBuf, SPI_WPEN_LEN);

            while(!(Board_writeOKSPIFlash(pMcspiObj)) && (quitLoop++ < 200));
        }

        /* Create command block for program operation */
        spiFlashTxBuf[0] = SPIFLASH_CMD_WRITE;
        spiFlashTxBuf[1] = (uint8_t)(dst >> SHIFT_WORD);
        spiFlashTxBuf[2] = (uint8_t)(dst >> SHIFT_BYTE);
        spiFlashTxBuf[3] = (uint8_t)(dst);

        delay_us(20);

        for(i = 0 ; i < bytesToProgram ; i++)
        {
            spiFlashTxBuf[4 + i] = *psrc++;
        }

        /* Execute write command */
        Board_writeMCSPI(pMcspiObj, spiFlashTxBuf, bytesToProgram + 4);

        /* Wait while busy */
        while(Board_writeOKSPIFlash(pMcspiObj));

        /* Get ready for next iteration */
        bytesLeft -= bytesToProgram;
        dst += bytesToProgram;
    }
}




uint8_t Board_verifySpiFlashErasedSector(uint32_t offset,
        mcspiCfgObj_t *pMcspiObj)
{
    uint32_t itr = 0, itr2 = 0;
    uint8_t t_buff[256];

    for(itr = offset ; itr < (offset + SPIFLASH_SECTORSIZE) ;)
    {
        Board_readSpiFlashPage(itr ,
                               (uint32_t)t_buff,
                               SPIFLASH_PAGESIZE, pMcspiObj);

        for(itr2 = 0 ; itr2 < SPIFLASH_PAGESIZE ; itr2++)
        {
            if(0xFF != t_buff[itr2])
            {
#ifdef DEBUG_CONSOLE_PRINT
                printf(" Erase failed at offset %d ,page %d, byte :%d \n", itr,
                       (itr / SPIFLASH_PAGESIZE), itr2);
#endif
                break;
            }

        }

        itr += SPIFLASH_PAGESIZE;

    }

    return 0;
}

#define VERIFY_SECTOR_ERASE
uint8_t Board_eraseSpiFlashSector(uint32_t mode, uint32_t offset,
                                  uint32_t length, mcspiCfgObj_t *pMcspiObj)
{
    uint16_t quitLoop = 0;
    uint32_t itr;

#ifdef DEBUG_CONSOLE_PRINT
    printf("Flash erase offset = %d , length = %d \n", offset, length);
#endif

    if(offset % SPIFLASH_SECTORSIZE || length % SPIFLASH_SECTORSIZE)
    {
#ifdef DEBUG_CONSOLE_PRINT
        printf("SF: Erase offset/length not multiple of sector size\n");
#endif
        return -1;
    }

    length /= SPIFLASH_SECTORSIZE;

    /* Issue WPEN */
    for(itr = 0; itr < length; itr++)
    {
        if(!Board_writeOKSPIFlash(pMcspiObj))
        {
            spiFlashTxBuf[0] = SPIFLASH_CMD_WREN;
            Board_writeMCSPI(pMcspiObj, spiFlashTxBuf, SPI_WPEN_LEN);

            while(!(Board_writeOKSPIFlash(pMcspiObj)) && (quitLoop++ < 200));
        }

        /* Issue Erase */
        spiFlashTxBuf[0] = SPIFLASH_CMD_SCTR_ERASE;
        spiFlashTxBuf[1] = (uint8_t)(offset >> SHIFT_WORD);
        spiFlashTxBuf[2] = (uint8_t)(offset >> SHIFT_BYTE);
        spiFlashTxBuf[3] = (uint8_t)(offset);

        Board_writeMCSPI(pMcspiObj, spiFlashTxBuf, SPI_ERASE_LEN);

        while(Board_writeOKSPIFlash(pMcspiObj));

#ifdef VERIFY_SECTOR_ERASE
        Board_verifySpiFlashErasedSector(offset, pMcspiObj);
#endif

        offset += SPIFLASH_SECTORSIZE;
    }

    return 0;
}


uint8_t Board_eraseSpiFlash(uint32_t offset, mcspiCfgObj_t *pMcspiObj)
{
    int32_t   no_of_sectors;

    if(SPI_FALSH_NOT_INITALIZED == spiFlashInitStatus)
    {
        return -1;
    }

    no_of_sectors = ((pMcspiObj->dataLength) / SPIFLASH_SECTORSIZE + (((
                         pMcspiObj->dataLength) % SPIFLASH_SECTORSIZE) ? 1 : 0));

    Board_eraseSpiFlashSector(0,
                              (offset / SPIFLASH_SECTORSIZE)*SPIFLASH_SECTORSIZE,
                              no_of_sectors * SPIFLASH_SECTORSIZE,
                              pMcspiObj);
    return 0;
}




void Board_readSpiFlashPage(uint32_t src, uint32_t dst,
                            uint32_t length, mcspiCfgObj_t *pMcspiObj)
{
    int32_t i;
    uint8_t *psrc, *pdst;

    // Setup command
    spiFlashRxBuf[0] = SPIFLASH_CMD_READ;
    spiFlashRxBuf[1] = (src >> SHIFT_WORD);
    spiFlashRxBuf[2] = (src >> SHIFT_BYTE);
    spiFlashRxBuf[3] = (src >> 0);
    /* SPI TX data */
    Board_readMCSPI(pMcspiObj , spiFlashRxBuf, length + 5);
    // Copy returned data
    pdst = (uint8_t *)dst;
    psrc = spiFlashRxBuf + 4;

    for(i = 0 ; i < length ; i++)
    {
        *pdst++ = *psrc++;
    }
}

uint8_t Board_readSpiFlashID(mcspiCfgObj_t *pMcspiObj)
{
    int32_t retVal = 0;
    statusbuf[0] = SPIFLASH_CMD_READID;
    statusbuf[1] = 0x00;
    statusbuf[2] = 0x00;
    statusbuf[3] = 0x00;
    statusbuf[4] = 0x00;
    statusbuf[5] = 0x00;

    Board_readMCSPI(pMcspiObj, statusbuf, MFID_DEVID_LEN);

    /* The Manufacturrerer ID and the Device ID is read into offset 5 & 6 */
    if(SPIFLASH_MANUF_ID != statusbuf[4])
    {
        retVal = 1;
#ifdef DEBUG_CONSOLE_PRINT
        printf("Unknown manufacturer ID read: 0x%x.\r\n", statusbuf[4]);
#endif
    }

    else
    {
#ifdef DEBUG_CONSOLE_PRINT
        printf("Manufacturer ID read is 0x%x.\r\n", statusbuf[4]);
        printf("Device ID read is 0x%x.\r\n", statusbuf[5]);
#endif
    }

    return (retVal);
}
