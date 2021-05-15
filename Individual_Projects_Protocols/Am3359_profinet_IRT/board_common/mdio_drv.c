/**
 * @file   mdio_drv.c
 *
 * @brief  MDIO Driver source file for unit test
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <xdc/std.h>
#include <ti/csl/hw_types.h>

#if defined SOC_K2G
#include <ti/csl/src/ip/mdio/V1/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V1/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V1/csl_mdioAux.h>
#else
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>
#endif


#include <board_am335x/include/mdio_drv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define TLKPHY_PHYSTS_REG                         (0x10)
#define TLK_SPEED_STATUS                     (1<<1)
#define TLK_DUPLEX_STATUS                    (1<<2)


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if !defined(icev2AM335x) && !defined(iceAMIC11x)
/**
* @brief Function to enable the MDIO Link interrupt
*
* @param mdioBaseAddress MDIO Base Address
* @param phyInst O/1 to select the User phy select option
* @param phyNum Phy address of the port
* @param linkSel Flag to select to use MDIO mode or MLINK mode
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

    HWREG(mdioBaseAddress + CSL_MDIO_USER_PHY_SEL_REG(0) + (phyInst * 8)) = PhySel;
}
#endif
/**
* @brief Function to disable the MDIO Link interrupt
*
* @param mdioBaseAddress MDIO Base Address
* @param phyInst O/1 to select the User phy select option
* @param phyNum Phy address of the port
* @param linkSel Flag to select to use MDIO mode or MLINK mode
*
*  @retval none
*/
void MDIO_disableLinkInterrupt(uint32_t mdioBaseAddress, uint32_t phyInst,
                               uint32_t phyNum, uint8_t linkSel)
{
    uint32_t PhySel;
    PhySel = phyNum;
    PhySel &=  ~(0x40);
    HWREG(mdioBaseAddress + CSL_MDIO_USER_PHY_SEL_REG(0) + (phyInst * 8)) = PhySel;
}

/**
* @brief This function returns the PHY Link status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
*
*  @retval 1 On Linkup
*             0 On Linkdown
*/
uint8_t MDIO_getPhyLinkStatus(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    return CSL_MDIO_phyLinkStatus(mdioBaseAddress, phyNum);
}


/**
* @brief API to Change Phy Configuration
*
* @param mdioBaseAddress MDIO Base Address
* @param phyMode    PHY_CONFIG_AUTONEG
*                   PHY_CONFIG_100FD
*                   PHY_CONFIG_10FD
*                   PHY_CONFIG_100HD
* @param phyNum Phy address of the port
*
* @retval none
*/
void MDIO_setPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                       uint8_t phyMode)
{
    uint16_t regStatus = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, PHY_BMCR_REG, &regStatus);

    switch(phyMode)
    {
        case PHY_CONFIG_AUTONEG:
            regStatus |= MII_AUTO_NEGOTIATE_EN;
            break;

        case PHY_CONFIG_100FD:
            regStatus &= ~(MII_AUTO_NEGOTIATE_EN);
            regStatus |= MII_SPEEDSEL_100;
            regStatus |= MII_DUPLEXMODE_FULL;
            break;

        case PHY_CONFIG_10FD:
            regStatus &= ~(MII_AUTO_NEGOTIATE_EN);
            regStatus &= ~(MII_SPEEDSEL_100);
            regStatus |= MII_DUPLEXMODE_FULL;
            break;

        case PHY_CONFIG_100HD:
            regStatus &= ~(MII_AUTO_NEGOTIATE_EN);
            regStatus |= MII_SPEEDSEL_100;
            regStatus &= ~(MII_DUPLEXMODE_FULL);
            break;

        case PHY_CONFIG_10HD:
            regStatus &= ~(MII_AUTO_NEGOTIATE_EN);
            regStatus &= ~(MII_SPEEDSEL_100);
            regStatus &= ~(MII_DUPLEXMODE_FULL);
            break;

        default:
            regStatus |= MII_AUTO_NEGOTIATE_EN;
            break;
    }

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, PHY_BMCR_REG, regStatus);
}
/**
* @brief Function to get the PHY Speed and duplexity
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
*
*  @retval PHY_CONFIG_10FD 10 Mbps and Full duplex
*          PHY_CONFIG_10HD 10 Mbps and Half duplex
*          PHY_CONFIG_100FD 100 Mbps and Full duplex
*          PHY_CONFIG_100HD 100 Mbps and Half duplex
*/
uint8_t MDIO_getPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t regStatus = 0;;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYSTS_REG, &regStatus);

    if(regStatus & TLK_SPEED_STATUS)  /*Speed is 10*/
    {
        if(regStatus & TLK_DUPLEX_STATUS)
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
        if(regStatus & TLK_DUPLEX_STATUS)
        {
            return PHY_CONFIG_100FD;
        }

        else
        {
            return PHY_CONFIG_100HD;
        }
    }
}

/**
* @brief Function to get Autoneg/ Forced setting of the PHY
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
*
*  @retval TRUE if Phy is in Autoneg mode
*             FALSE if Phy is in Forced mode
*/
uint8_t MDIO_getAutoNegStat(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_BMCR_REG, &regStatus);

    if(regStatus & MII_AUTO_NEGOTIATE_EN)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}
/**
* @brief Function to get the PHY ready status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
*
*  @retval TRUE if Phy is ready
*             FALSE if Phy not ready
*/
uint8_t MDIO_getPhyIdentifyStat(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_PHYIDR1_REG, &regStatus);

    if(regStatus == 0x2000)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}

/**
* @brief Function to get Autoneg complete status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
*
*  @retval TRUE if Autoneg is complete
*             FALSE if Autoneg is not complete
*/
uint8_t MDIO_getAutoNegCompleteStat(uint32_t mdioBaseAddress, uint32_t phyAddr)
{
    uint16_t regStatus = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_BMSR_REG, &regStatus);

    if(regStatus & PHY_AUTONEG_COMPETE)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}
/**
* @brief Function to get the Link Partner Autoneg ability
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
*
*  @retval TRUE if parter in Autoneg
*             FALSE if parter in forced mode
*/
uint8_t MDIO_getLinkPartnerAutonegAble(uint32_t mdioBaseAddress,
                                       uint32_t phyAddr)
{
    uint16_t regStatus = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyAddr, PHY_ANER_REG, &regStatus);

    if(regStatus & PHY_LNKPTNR_AUTONEG)
    {
        return TRUE;
    }

    else
    {
        return FALSE;
    }
}

