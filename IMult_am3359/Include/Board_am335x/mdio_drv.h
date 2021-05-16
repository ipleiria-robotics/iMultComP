/**
 * @file   mdio_drv.h
 *
 *  @brief
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

#ifndef _MDIO_DRV_H
#define _MDIO_DRV_H

#include "stdint.h"
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*Phy register offset*/
#define PHY_BMCR_REG                            (0x0)
#define PHY_BMSR_REG                            (0x1)
#define PHY_PHYIDR1_REG                         (0x2)
#define PHY_ANER_REG                            (0x6)

#define MII_AUTO_NEGOTIATE_EN   (1<<12)
#define MII_SPEEDSEL_100        (1<<13)
#define MII_DUPLEXMODE_FULL     (1<<8)


#define PHY_AUTONEG_COMPETE     (1<<5)
#define PHY_LNKPTNR_AUTONEG     (1)
/*MDIO offsets*/
#define MDIO_USERPHYSEL_LINKSEL             (1 << 7)

#define MDIO_LINKSEL_ENABLE                 1
#define MDIO_LINKSEL_DISABLE                0

#define MDIO_NUM_MSGSIZE 5

/**
* @def PHY_CONFIG_AUTONEG
*      Configure PHY in AutoNeg mode
*/
#define  PHY_CONFIG_AUTONEG 0u
/**
* @def PHY_CONFIG_100FD
*      Force PHY to 100 FullDuplex
*/
#define  PHY_CONFIG_100FD 1u
/**
* @def PHY_CONFIG_10FD
*      Force PHY to 10 FullDuplex
*/
#define  PHY_CONFIG_10FD 2u
/**
* @def PHY_CONFIG_100HD
*      Force PHY to 100 halfDuplex
*/
#define  PHY_CONFIG_100HD 3u
/**
* @def PHY_CONFIG_10HD
*      Force PHY to 100 halfDuplex
*/
#define  PHY_CONFIG_10HD 4u

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables Declarations                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


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
                              uint32_t phyNum, uint8_t linkSel);
/**
* @brief This function returns the PHY Link status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
*
*  @retval 1 On Linkup
*          0 On Linkdown
*/
uint8_t MDIO_getPhyLinkStatus(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief API to Change Phy Configuration
*
* @param mdioBaseAddress MDIO Base Address
* @param phyMode    PHY_CONFIG_AUTONEG
*                   PHY_CONFIG_100FD
*                   PHY_CONFIG_10FD
*                   PHY_CONFIG_100HD
* @param phyNum Phy address of the port
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used
*
* @retval none
*/
void MDIO_setPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                       uint8_t phyMode);
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
uint8_t MDIO_getPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to get Autoneg/ Forced setting of the PHY
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used
*
*  @retval TRUE if Phy is in Autoneg mode
*          FALSE if Phy is in Forced mode
*/
uint8_t MDIO_getAutoNegStat(uint32_t mdioBaseAddress, uint32_t phyAddr);
/**
* @brief Function to get the PHY ready status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used
*
*  @retval TRUE if Phy is ready
*          FALSE if Phy not ready
*/
uint8_t MDIO_getPhyIdentifyStat(uint32_t mdioBaseAddress, uint32_t phyAddr);
/**
* @brief Function to get Autoneg complete status
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used
*
*  @retval TRUE if Autoneg is complete
*          FALSE if Autoneg is not complete
*/
uint8_t MDIO_getAutoNegCompleteStat(uint32_t mdioBaseAddress, uint32_t phyAddr);
/**
* @brief Function to get the Link Partner Autoneg ability
*
* @param mdioBaseAddress MDIO Base Address
* @param phyAddr Phy address of the port
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used
*
*  @retval TRUE if parter in Autoneg
*          FALSE if parter in forced mode
*/
uint8_t MDIO_getLinkPartnerAutonegAble(uint32_t mdioBaseAddress,
                                       uint32_t phyAddr);
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
                               uint32_t phyNum, uint8_t linkSel);
#endif

