/**
 * \file board_tlkphy.c
 * \brief Contains ICSS PHY (TLK) Specific APIs
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <examples/board/include/board_tlkphy.h>
#include <examples/board/include/soc_icss_header.h>
#include <ti/drv/icss_emac/src/phy.h>

#ifdef INCLUDE_TLK_WORKAROUND
#include <examples/osal/TaskP.h>

/**Task Handle for the TLK thread which swaps MDI/MDI-X bits for link*/
void *tlkTask;

/**
* @internal
* @brief API to Swap the MDIO/MDIX setting if Link is down and Phy is in forced mode
*
*       This API is used by TLK Task to do Software MDI/X. This Functin checks for Link status.Incase down,it checks for
*       Autonegotiation status. If the PHY is in forced mode, MDI/X configuration of the PHY is swapped until
*       link is detected
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum PHY Address of the Port
*
* @retval none
*/
void Board_tlkMDIXSwap(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @internal
* @brief Main Task which implements MDI/MDI-X for the PHY in Software
*
*       This Task Does the software MDI/X for the TLK PHY. Once created the task  will constantly
*       Check for Link UP. Incase Link is down, the taks will check whether the PHY is in forced mode.
*       If in Forced mode, the AutoMDIX is disabled and  Software MDI/X is done. It will continuosuly swap
*       MDI/MDIX bit until the Link is established.
*
* @param a0     [IN] Argument passed (MDIO Base Address)
* @param a1     [IN] Argument passed (None)
*
*  @retval none
*/
void Board_tlkMDIXTask(UArg a0, UArg a1);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
* @internal
* @brief API to Swap the MDIO/MDIX setting if Link is down and Phy is in forced mode
*
*       This API is used by TLK Task to do Software MDI/X. This Functin checks for Link status.Incase down,it checks for
*       Autonegotiation status. If the PHY is in forced mode, MDI/X configuration of the PHY is swapped until
*       link is detected
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum PHY Address of the Port
*
* @retval none
*/
void Board_tlkMDIXSwap(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    if(!CSL_MDIO_phyLinkStatus(mdioBaseAddress, phyNum))
    {
        if(!Board_getAutoNegStat(mdioBaseAddress, phyNum))
        {
            if(PHY_ENABLE_FORCE_MDIX == Board_getPhyMDIXStat(mdioBaseAddress, phyNum))
            {
                Board_setPhyMDIX(mdioBaseAddress, phyNum, PHY_ENABLE_FORCE_MDI);
            }

            else if(PHY_ENABLE_FORCE_MDI == Board_getPhyMDIXStat(mdioBaseAddress, phyNum))
            {
                Board_setPhyMDIX(mdioBaseAddress, phyNum, PHY_ENABLE_FORCE_MDIX);
            }

            else
            {
                Board_setPhyMDIX(mdioBaseAddress, phyNum, PHY_ENABLE_FORCE_MDI);
            }
        }
    }
}

/**
* @brief Fix for issue where the partner link change from 10 half to 100 was not detected by TLK PHY
*
*        The Link does not come up in TLK PHY when the partner configuration is changed from 10 half to
*        100 full/half. The Fix is to write value 0x5668 to NLP register
*
*       MDIO init shall be done before using this function
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] PHY Address of the Port
*
*  @retval none
*/
void Board_phyNLPFix(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_REGCR_REG,
                         EXT_REG_ADDRESS_ACCESS);
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_ADDR_REG, NLP_DET_REG);
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_REGCR_REG,
                         EXT_REG_DATA_NORMAL_ACCESS);
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_ADDR_REG,
                         NLP_DET_CONFIG_REGVAL);
}
/**
* @internal
* @brief Main Task which implements MDI/MDI-X for the TLK PHY in Software
*
*       This Task Does the software MDI/X for the TLK PHY. Once created the task  will constantly
*       Check for LInk UP. In case link is down, the task will check whether the PHY is in forced mode.
*       In this case the AutoMDIX is disabled and  Software MDI/X is done. It will continuously swap
*       MDI/MDIX bit until the Link is established.
*
*       The NLP Fix is also enabled in the task
*
* @param a0     [IN] Argument passed (MDIO Base Address)
* @param a1     [IN] Argument passed (None)
*
*  @retval none
*/
void Board_tlkMDIXTask(UArg a0, UArg a1)
{
    uint32_t i = 0;
    tlkMDIXTaskParam_t params;
    memcpy((void *)&params, (void *)a0, sizeof(tlkMDIXTaskParam_t));

    TaskP_sleep(MDIX_FIX_TASKSLEEP_TICK);

    for(i = 0; i < (params.numPorts); i++)
    {
        Board_phyNLPFix(params.mdioBaseAddress, (uint32_t)(params.phyAddress[i]));
    }

    while(1)
    {
        for(i = 0; i < (params.numPorts); i++)
        {
            Board_tlkMDIXSwap(params.mdioBaseAddress, (uint32_t)(params.phyAddress[i]));
            TaskP_sleep(MDIX_FIX_TASKSLEEP_TICK);
        }
    }
}

/**
* @brief Initializes the workarounds for TLK issues. This API should be called to make the TLK PHY
*           work in Forced Mode.
*
*       This API implements the TLK specific workarounds.
*       1) AutoMDIX workaround
*        A task is created which will constantly check whether the PHY is in forced mode, In this case
*        the AutoMDIX is disabled and Software MDI/X is done.
*       2)NLP Fix
*        Fix for issue where the Link is not detected when the partner configuration is changed from 10 half to
*        100 full/Half
*
*        MDIO init shall be done before using this function
*
* @param mdioBaseAddress    [IN] MDIO Base Address
*
*  @retval none
*/
void Board_phyMDIXFixInit(tlkMDIXTaskParam_t *params)
{
    TaskP_Params taskParams;

    uint8_t taskname[] = "tlkMDIXTask";
    TaskP_Params_init(&taskParams);
    taskParams.priority = 3;
    taskParams.stacksize = 0x1000;
    taskParams.name = (uint8_t *)taskname;
    taskParams.arg0 = (void *)params;

    tlkTask = TaskP_create(Board_tlkMDIXTask, &taskParams);
}
/**
* @brief Shut down the TLK issue workaround
*
*       Deletes the TLK issue workaround task
*
*
*  @retval none
*/
void Board_phyMDIXFixDeInit()
{
    TaskP_delete((TaskP_Handle *)&tlkTask);
}

#endif
void Board_disablePhyAutoMDIX(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregVal = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregVal);
    phyregVal &= ~(TLKPHY_AUTOMDIX_ENABLE);
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, phyregVal);
}

#ifndef iceAMIC11x
void Board_enablePhyAutoMDIX(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregval);
    phyregval |= TLKPHY_AUTOMDIX_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, phyregval);
}
#endif /*iceAMIC11x*/

void Board_enablePhyMII(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_RCSR_REG, phyregval);
}

uint8_t Board_getPhyMDIXStat(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregval);

    if(phyregval & TLKPHY_AUTOMDIX_ENABLE)
    {
        return PHY_ENABLE_AUTO_MDIX;
    }

    else if(phyregval & TLKPHY_FORCEMDIX_ENABLE)
    {
        return PHY_ENABLE_FORCE_MDIX;
    }

    else
    {
        return PHY_ENABLE_FORCE_MDI;
    }
}

void Board_setPhyMDIX(uint32_t mdioBaseAddress, uint32_t phyNum,
                      uint8_t mdiState)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregval);

    switch(mdiState)
    {
        case PHY_ENABLE_FORCE_MDI:
            phyregval &= ~(TLKPHY_AUTOMDIX_ENABLE);
            phyregval &= ~(TLKPHY_FORCEMDIX_ENABLE);
            break;

        case PHY_ENABLE_FORCE_MDIX:
            phyregval &= ~(TLKPHY_AUTOMDIX_ENABLE);
            phyregval |= TLKPHY_FORCEMDIX_ENABLE;
            break;

        case PHY_ENABLE_AUTO_MDIX:
        default:
            phyregval |= TLKPHY_AUTOMDIX_ENABLE;
            break;
    }

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, phyregval);
}
void Board_phyEnablePowerSaveMode(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYSCR_REG, &phyregval);
    phyregval |= TLKPHY_PSMODE_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYSCR_REG, phyregval);
}

void Board_setPhyPowerSaveMode(uint32_t mdioBaseAddress, uint32_t phyNum,
                               uint8_t phyPowerMode)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYSCR_REG, &phyregval);

    switch(phyPowerMode)
    {
        case PHY_POWERMODE_NORMAL:
            phyregval &= (~(TLKPHY_PSMODE_BIT1) & ~(TLKPHY_PSMODE_BIT2));
            break;

        case PHY_POWERMODE_DOWN:
            phyregval &= ~(TLKPHY_PSMODE_BIT2);
            phyregval |= TLKPHY_PSMODE_BIT1;
            break;

        case PHY_POWERMODE_ACTIVE_SLEEP:
            phyregval &= ~(TLKPHY_PSMODE_BIT1);
            phyregval |= TLKPHY_PSMODE_BIT2;
            break;

        case PHY_POWERMODE_PASSIVE_SLEEP:
            phyregval |= (TLKPHY_PSMODE_BIT2 | TLKPHY_PSMODE_BIT1);

        default:
            phyregval &= (~(TLKPHY_PSMODE_BIT1) & ~(TLKPHY_PSMODE_BIT2));
            break;
    }

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYSCR_REG, phyregval);
}


#ifndef iceAMIC11x
void Board_phyLedBlinkConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                             uint16_t val)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_LEDCR_REG, &phyregval);

    switch(val)
    {
        case LED_BLINK_500:
            phyregval &= 0xF9FF;
            phyregval |= 0x0600;
            break;

        case LED_BLINK_200:
            phyregval &= 0xF9FF;
            phyregval |= 0x0400;
            break;

        case LED_BLINK_100:
            phyregval &= 0xF9FF;
            phyregval |= 0x0200;
            break;

        case LED_BLINK_50:
            phyregval &= 0xF9FF;
            break;
    }

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_LEDCR_REG, phyregval);
}
#endif /*iceAMIC11x*/

void Board_phyLedSpeedLinkActPolarity(uint32_t mdioBaseAddress, uint32_t phyNum,
                                      uint16_t mask)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_LEDCR_REG, &phyregval);

    phyregval |= (mask << 6);

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_LEDCR_REG, phyregval);
}

void Board_phyFastRXDVDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR1_REG, &phyregval);
    phyregval |= FASTRXDV_DET_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR1_REG, phyregval);
}

#ifndef iceAMIC11x
void Board_phyFastLinkDownDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum,
                                    uint8_t val)
{
    uint16_t phyregval = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR3_REG, &phyregval);
    phyregval |= val;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR3_REG, phyregval);
}

void Board_phyExtFDEnable(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, &phyregval);
    phyregval |= EXT_FD_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, phyregval);
}

void Board_phyEnhLEDLinkEnable(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, &phyregval);
    phyregval |= ENH_LEDLINK_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, phyregval);
}

void Board_phyODDNibbleDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, &phyregval);
    phyregval |= ODDNIBBLE_DET_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, phyregval);
}

void Board_phyRxErrIdleEnable(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, &phyregval);
    phyregval |= RXERROR_IDLE_ENABLE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR2_REG, phyregval);
}

uint8_t Board_getPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t regStatus = 0;;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYSTS_REG, &regStatus);

    if(regStatus & TLKPHY_SPEED_STATUS)    /*Speed is 10*/
    {
        if(regStatus & TLKPHY_DUPLEX_STATUS)
        {
            return PHY_CONFIG_10FD;
        }

        else
        {
            return PHY_CONFIG_10HD;
        }
    }

    else/*Speed is 100*/
    {
        if(regStatus & TLKPHY_DUPLEX_STATUS)
        {
            return PHY_CONFIG_100FD;
        }

        else
        {
            return PHY_CONFIG_100HD;
        }
    }
}


uint8_t Board_getAutoNegStat(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_BCR_REG, &regStatus);

    if(regStatus & MII_AUTO_NEGOTIATE_EN)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}

uint8_t Board_getPhyIdentifyStat(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_ID1_REG, &regStatus);

    if(regStatus == 0x2000)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}
#endif /*iceAMIC11x*/


void Board_phyAutoNegAdvConfigFD(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_AUTONEG_ADV_REG, &regStatus);

    regStatus |= PHY_ANAR_100FD;
    regStatus |= PHY_ANAR_100HD;
    regStatus |= PHY_ANAR_10FD;
    regStatus |= PHY_ANAR_10HD;

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyAddr, PHY_AUTONEG_ADV_REG, regStatus);
}


