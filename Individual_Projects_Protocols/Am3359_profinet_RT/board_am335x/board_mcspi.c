/**
 * board_mcspi.c
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
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <board_am335x/include/board_mcspi.h>
#include <board_am335x/include/board_gpioLed.h>

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>
#include <ti/csl/csl_gpio.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/src/ip/mcspi/V0/mcspi.h>
#include <ti/csl/src/ip/mcspi/V0/hw_mcspi.h>
#include <ti/starterware/include/board.h>
#include <ti/starterware/include/device.h>
#include <board_am335x/include/board_spi.h>
#include <board_am335x/include/board_platform.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Steps done in a cycle of McSPI Read/Write
 *
 * \param
 *
 *
 * \return
 *
 **/
//void MCSPICycle(mcspiCfgObj_t *pCfgMcspi, uint32_t len);
void Board_MCSPICycle(mcspiCfgObj_t *pCfgMcspi, uint8_t *buff, uint32_t len);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Board_initMCSPI(mcspiCfgObj_t *pMcspiObj)
{
    McSPIMasterModeEnable(pMcspiObj->instAddr);
    McSPIMasterModeConfig(pMcspiObj->instAddr,
                          pMcspiObj->pCfg.channel,
                          pMcspiObj->pCfg.txRxMode,
                          pMcspiObj->pCfg.pinMode,
                          pMcspiObj->channelNum);

    /*
    ** Default granularity is used. Also as per my understanding clock mode
    ** 0 is proper.
    */
    McSPIClkConfig(pMcspiObj->instAddr,
                   pMcspiObj->inClk,
                   pMcspiObj->outClk,
                   pMcspiObj->channelNum,
                   pMcspiObj->pCfg.clkMode);
    /* Configure the word length.*/
    McSPIWordLengthSet(pMcspiObj->instAddr,
                       MCSPI_WORD_LENGTH(pMcspiObj->pCfg.wordLength),
                       pMcspiObj->channelNum);

    /* Set polarity of SPIEN to low.*/
    McSPICSPolarityConfig(pMcspiObj->instAddr,
                          pMcspiObj->pCfg.csPolarity,
                          pMcspiObj->channelNum);

    McSPIRxFIFOConfig(pMcspiObj->instAddr,
                      pMcspiObj->pCfg.rxFifoCfg,
                      pMcspiObj->channelNum);
    return 0;
}


void Board_readMCSPI(mcspiCfgObj_t *pCfgMcspi, uint8_t *buff, uint32_t len)
{
    Board_MCSPICycle(pCfgMcspi, buff, len);
}

void Board_writeMCSPI(mcspiCfgObj_t *pCfgMcspi,  uint8_t *buff, uint32_t len)
{
    Board_MCSPICycle(pCfgMcspi, buff, len);
}
/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

void Board_MCSPICycle(mcspiCfgObj_t *pCfgMcspi, uint8_t *buff, uint32_t len)
{
    volatile int loop;
    uint32_t itr = 0;
    uint32_t mcspi_chstat_txs_mask, mcspi_chstat_rxs_mask;
    /* Enable the MCSPI channel for communication.*/
    McSPIChannelEnable(pCfgMcspi->instAddr, pCfgMcspi->channelNum);

    /* SPIEN line is forced to low state.*/
    McSPICSAssert(pCfgMcspi->instAddr, pCfgMcspi->channelNum);

    mcspi_chstat_txs_mask = MCSPI_CH0STAT_TXS_MASK;
    mcspi_chstat_rxs_mask = MCSPI_CH0STAT_RXS_MASK;

    for(itr = 0; itr < len; itr++)
    {
        loop = 1000;

        /* Wait for transmit empty */
        while(((McSPIChannelStatusGet(pCfgMcspi->instAddr,
                                      pCfgMcspi->channelNum) & mcspi_chstat_txs_mask) == 0)
                && (loop--));    /* check if TX buffer is empty */

        if(loop == -1)
        {
            return;
        }

        /* Program the data to be transmitted. */
        McSPITransmitData(pCfgMcspi->instAddr,
                          buff[itr],
                          pCfgMcspi->channelNum);

        loop = 1000;

        /* check if RX buffer */
        while(((McSPIChannelStatusGet(pCfgMcspi->instAddr,
                                      pCfgMcspi->channelNum) & mcspi_chstat_rxs_mask) == 0)
                && (loop--));    /* check if RX buffer is full */

        if(loop == -1)
        {
            return;
        }

        buff[itr] = (uint8_t)(McSPIReceiveData(pCfgMcspi->instAddr,
                                               pCfgMcspi->channelNum) & 0xff);
    }

    /* Force SPIEN line to the inactive state.*/
    McSPICSDeAssert(pCfgMcspi->instAddr, pCfgMcspi->channelNum);
    /* Disable the McSPI channel.*/
    McSPIChannelDisable(pCfgMcspi->instAddr, pCfgMcspi->channelNum);
}
