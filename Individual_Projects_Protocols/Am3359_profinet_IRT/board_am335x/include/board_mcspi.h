/**
 * board_mcspi.h
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
#ifndef BOARD_MCSPI_H_
#define BOARD_MCSPI_H_

#ifdef __cplusplus
extern "C" {
#endif
//#include "board_platform.h"
#include <ti/csl/src/ip/mcspi/V0/mcspi.h>
#include <stdlib.h>
#include <ti/starterware/include/interrupt.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/** \brief Structure holding MCSPI configuration parameters. */
typedef struct mcspiCfg
{
    uint32_t channel;
    /**< Single/Multi channel of operation. */
    uint32_t txRxMode;
    /**< Transmit/receive mode. */
    uint32_t pinMode;
    /**< Pin configuration of data lines. */
    uint32_t clkMode;
    /**< phase and polarity configuration of clock. */
    uint32_t wordLength;
    /**< Word length to be used for SPI transfer. */
    uint32_t csPolarity;
    /**< Polarity of chip select. */
    uint32_t txFifoCfg;
    /**< Configure Tx FIFO. */
    uint32_t rxFifoCfg;
    /**< Configure Rx FIFO. */
    uint32_t intrMask;
    /**< Mask to configure MCSPI interrupts. */
} mcspiCfg_t;

/** @brief MCSPI Flag config */
typedef struct mcspiFlag
{
    uint32_t csFlag;
    /**< Enable/Disable chip select . */
    uint32_t modeFlag;
    /**< Master/Slave mode of operation. */
    uint32_t comFlag;
    /**< Communication flag to select Interrupt or DMA mode of operation. */
} mcspiFlag_t;

/** @brief MCSPI Intr config */
typedef struct mcspiIntrCfgObj
{
    intcTrigType_t trigType;
    /**< Type of interrupt Edge/Level. */
    uint32_t intrLine;
    /**< Interrupt line number. */
    uint32_t intrPriority;
    /**< Interrupt priority level. */
    uint32_t isIntrSecure;
    /**< Secure Interrupt or not */
    uint8_t *pTxBuf;
    /**< Pointer to Tx buffer. */
    uint8_t *pRxBuf;
    /**< Pointer to Rx buffer. */
    void (*pFnIntrHandler)(uint32_t intrId, uint32_t cpuId, void *pUserParam);
    /**< Function pointer to Interrupt handler which needs to be
     *  registered with the interrupt Controller.- Application needs
     *  to initialize this. If set to null then polling Mode is assumed
     */
} mcspiIntrCfgObj_t;

/** @brief Structure holding the MCSPI object data structure. */
typedef struct mcspiCfgObj
{
    uint32_t instNum;
    /**< MCSPI instance number. */
    uint32_t instAddr;
    /**< MCSPI instance address. */
    uint32_t inClk;
    /**< Input clock to MCSPI in MHz. */
    uint32_t outClk;
    /**< Output clock on the bus in MHz. */
    uint32_t channelNum;
    /**< MCSPI channel number. */
    uint32_t dataLength;
    /**< Data transfer length. */
    uint8_t *pTx;
    /**< Pointer to Tx buffer. */
    uint8_t *pRx;
    /**< Pointer to Rx buffer. */
    mcspiCfg_t pCfg;
    /**< MCSPI configuration structure.*/
    mcspiIntrCfgObj_t intrCfg;
    /**< MCSPI Interrupt configuration structure.*/
    mcspiFlag_t pFlag;
    /**< MCSPI flag structure which shall allow user to
         1) Enable/disable chip select.
         2) Configure Master/Slave mode.
         3) Configure Interrupt/DMA mode
    */
} mcspiCfgObj_t;

/**
 *
 *  \brief    Enumerates the values used to determine the MCSPI communication
 *            mode.
 */
typedef enum mcspiCommMode
{
    MCSPI_INTERRUPT_MODE = 0U,
    /**< Select Interrupt mode of MCSPI. */
    MCSPI_DMA_MODE = 1U
                     /**< Select DMA mode of MCSPI. */
} mcspiCommMode_t;



/**
 * \brief    MCSPI IP configuration initialization function.
 *
 * \details  This function configures the MCSPI controller
 *
 * \param    None
 *
 * \retVal   S_PASS        MCSPI initialization successful.
 * \retVal   E_FAIL        MCSPI initialization failed.
 */
int32_t Board_initHVSDigIn(mcspiCfgObj_t *pCfgMcspi);

/**
 * \brief    MCSPI HVS882 Read function
 *
 * \details  This function configures the MCSPI controller. pRx contains the read value
 *
 * \param    None
 *
 * \retVal   S_PASS        MCSPI initialization successful.
 * \retVal   E_FAIL        MCSPI initialization failed.
 */
void Board_getHVSDigIn(mcspiCfgObj_t *pCfgMcspi);

/**
 * \brief    Init MCSPI
 *
 * \details  This function configures the MCSPI controller. p
 *
 * \param    pMcspiObj MCSPI Configuration
 *
 * \retVal   S_PASS        MCSPI initialization successful.
 * \retVal   E_FAIL        MCSPI initialization failed.
 */
int32_t Board_initMCSPI(mcspiCfgObj_t *pMcspiObj);

void Board_readMCSPI(mcspiCfgObj_t *pCfgMcspi, uint8_t *buff, uint32_t len);
void Board_writeMCSPI(mcspiCfgObj_t *pCfgMcspi,  uint8_t *buff, uint32_t len);

#endif /* OSDRV_MCSPI_H_ */
