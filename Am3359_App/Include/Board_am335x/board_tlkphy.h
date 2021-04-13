/**
 * \file board_tlkphy.h
 * \brief
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
#ifndef _BOARD_TLKPHY_H_
#define _BOARD_TLKPHY_H_

#include <stdint.h>

#include <ti/drv/icss_emac/src/phy.h>

#define MDIX_FIX_TASKSLEEP_TICK       2500

#define PHY_BCR_REG                           (0u)
#define PHY_BSR_REG                           (1u)
#define PHY_ID1_REG                           (2u)
#define PHY_ID2_REG                           (3u)
#define PHY_AUTONEG_ADV_REG                   (4u)
#define PHY_LINK_PARTNER_ABLTY_REG            (5u)
#define PHY_1000BT_CONTROL_REG                (9u)
#define PHY_1000BT_STATUS_REG                 (0x0A)

#define TLKPHY_RCSR_REG                           (0x17)

#define TLKPHY_CR1_REG                            (0x9)
#define TLKPHY_CR2_REG                            (0xA)
#define TLKPHY_CR3_REG                            (0xB)
#define TLK105_EXT_MLEDCR_REG                     (0x0025)
#define DP83822_EXT_LEDCFG1_REG                   (0x0460)
#define DP83822_EXT_IOCTRL1_REG                   (0x0462)


/**
* @def MII_PSMODE_ENABLE
*
*/
#define TLKPHY_PSMODE_ENABLE   (1<<14)
/**
* @def MII_PSMODE_BIT1
*
*/
#define TLKPHY_PSMODE_BIT1   (1<<12)
/**
* @def MII_PSMODE_BIT2
*
*/
#define TLKPHY_PSMODE_BIT2   (1<<13)

#define TLKPHY_AUTOMDIX_ENABLE                       (1u<<15)
#define TLKPHY_FORCEMDIX_ENABLE                      (1u<<14)


#define TLKPHY_SPEED_STATUS                     (1<<1)
#define TLKPHY_DUPLEX_STATUS                    (1<<2)

/**
* @def MII_PSMODE_ENABLE
*
*/
#define TLKPHY_PSMODE_ENABLE   (1<<14)
/**
* @def MII_PSMODE_BIT1
*
*/
#define TLKPHY_PSMODE_BIT1   (1<<12)
/**
* @def MII_PSMODE_BIT2
*
*/
#define TLKPHY_PSMODE_BIT2   (1<<13)


/**
* @def NLP_DET_REG
*      NLP Parallel Detect Configuration register
*/
#define NLP_DET_REG   0x0020
/**
* @def NLP_DET_CONFIG_REGVAL
*      NLP Parallel Detect Configuration Value
*/
#define NLP_DET_CONFIG_REGVAL  0x5668

/**
* @def EXT_REG_ADDRESS_ACCESS
*      Extended reg address access value
*/
#define EXT_REG_ADDRESS_ACCESS 0x001F
/**
* @def EXT_REG_DATA_NORMAL_ACCESS
*      Extended reg data access value
*/
#define EXT_REG_DATA_NORMAL_ACCESS 0x401F


/**
* @def TLKPHY_REGCR_REG
*      Value reg for accessing extended registers
*/
#define TLKPHY_REGCR_REG  0x000D
/**
* @def TLKPHY_ADDR_REG
*      ADDR reg for accessing extended registers
*/
#define TLKPHY_ADDR_REG   0x000E

/*PHY LED Modes*/
#define LED_CFG_MODE0     0
#define LED_CFG_MODE1     1
#define LED_CFG_MODE2     2
#define LED_CFG_MODE3     3

/*PHY LED BLINK Rates*/
#define LED_BLINK_500     500
#define LED_BLINK_200     200
#define LED_BLINK_100     100
#define LED_BLINK_50      50

#define PHY_MLED_100  0u
#define PHY_RXTX_ACT  1u
#define PHY_DIS_MLED  2u
#define PHY_MLED_PIN17_100  3u
#define PHY_BLINK_RXTX_ACT  4u

/**
* @def PHY_ENABLE_AUTO_MDIX
*       Enable AutoMDI/X
*/
#define PHY_ENABLE_AUTO_MDIX  0u
/**
* @def PHY_ENABLE_FORCE_MDI
*      Force MDI mode
*/
#define PHY_ENABLE_FORCE_MDI  1u
/**
* @def PHY_ENABLE_FORCE_MDIX
*      Force MDIX mode
*/
#define PHY_ENABLE_FORCE_MDIX  2u
/**
* @def PHY_POWERMODE_NORMAL
*      Enable Normal Power mode
*/
#define PHY_POWERMODE_NORMAL 0u
/**
* @def PHY_POWERMODE_DOWN
*      Enable Normal down mode
*/
#define PHY_POWERMODE_DOWN 1u
/**
* @def PHY_POWERMODE_ACTIVE_SLEEP
*      Enable Active Sleep  Power mode
*/
#define PHY_POWERMODE_ACTIVE_SLEEP 2u
/**
* @def PHY_POWERMODE_PASSIVE_SLEEP
*      Enable Passive Sleep Power mode
*/
#define PHY_POWERMODE_PASSIVE_SLEEP 3u


#define FASTRXDV_DET_ENABLE           (1u<<1)
#define SWSTRAP_CONFIG_DONE           (1u<<15)

#define ODDNIBBLE_DET_ENABLE           (1u<<1)
#define RXERROR_IDLE_ENABLE            (1u<<2)
#define ENH_LEDLINK_ENABLE             (1u<<4)
#define EXT_FD_ENABLE                  (1u<<5)

#define FAST_LINKDOWN_SIGENERGY        1u
#define FAST_LINKDOWN_LOWSNR           (1u<<1)
#define FAST_LINKDOWN_MLT3ERR          (1u<<2)
#define FAST_LINKDOWN_RXERR            (1u<<3)

/**Configure PHY in AutoNeg mode*/
#define  PHY_CONFIG_AUTONEG         0u
/**Force PHY to 100 FullDuplex*/
#define  PHY_CONFIG_100FD           1u
/**Force PHY to 10 FullDuplex*/
#define  PHY_CONFIG_10FD            2u
/**Force PHY to 100 halfDuplex*/
#define  PHY_CONFIG_100HD           3u
/**Force PHY to 100 halfDuplex*/
#define  PHY_CONFIG_10HD            4u

/**Auto neg Enable bit in PHY_BMCR_REG*/
#define MII_AUTO_NEGOTIATE_EN   (1<<12)

/* #define AUTO_NEG_ADV_VAL    (0x01E1) */
#define PHY_ANAR_100FD      (1u<<8)
#define PHY_ANAR_100HD      (1u<<7)
#define PHY_ANAR_10FD       (1u<<6)
#define PHY_ANAR_10HD       (1u<<5)

#define MDIO_MAX_PHYNUM    32

/**Structure to pass to the MDIX workaround task*/
typedef struct tlkMDIXTaskParam
{
    uint32_t mdioBaseAddress;
    uint8_t numPorts;
    uint8_t phyAddress[MDIO_MAX_PHYNUM];
} tlkMDIXTaskParam_t;


/** @defgroup ISDK_BOARD_FUNCTIONS MDIO/PHY Driver Board APIs
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *
 *  ISDK Overview here
 *
 */


/**
@defgroup PHY_TLK_WORKAROUND_FUNCTIONS  TLK Phy WorkAround APIs
@ingroup ISDK_BOARD_FUNCTIONS
*/

/**
@defgroup PHY_VENDOR_SPECIFIC_FUNCTIONS  PHY Vendor Specific Register APIs
@ingroup ISDK_BOARD_FUNCTIONS
*/

/**
@defgroup PHY_TLK105_SPECIFIC_FUNCTIONS  TLK 105 Specific Register APIs
@ingroup ISDK_BOARD_FUNCTIONS
*/

/**
@defgroup PHY_TLK110_SPECIFIC_FUNCTIONS  TLK 110 Specific Register APIs
@ingroup ISDK_BOARD_FUNCTIONS
*/

/**
@defgroup PHY_RESET_FUNCTIONS  PHY Reset APIs
@ingroup ISDK_BOARD_FUNCTIONS
*/


/** @addtogroup PHY_TLK_WORKAROUND_FUNCTIONS
 @{ */

/**
* @brief Fix for issue where the partner link change from 10 half to 100 was not detected by TLK PHY
*
*        The Link doesnot comeup in TLK PHY when the partner configuration is changed from 10 half to
*        100 full/hallf. The Fix is to write value 0x5668 to NLP register
*
*       MDIO init shall be done before using this function
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] PHY Address of the Port
*
*  @retval none
*/
void Board_phyNLPFix(uint32_t mdioBaseAddress, uint32_t phyNum);

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
void Board_phyMDIXFixInit(tlkMDIXTaskParam_t *params);
/**
* @brief Shutdown the TLK issue workaround
*
*       Deletes the TLK issue workaround task
*
*
*  @retval none
*/
void Board_phyMDIXFixDeInit();

/**
@}
*/


/** @addtogroup PHY_VENDOR_SPECIFIC_FUNCTIONS
 @{ */

/**
* @brief Function to disable AutoMDIX
*
*       MDIO init shall be done before using this function.The Sem handle need to be passed if theThread safe MDIO read is used,
*       Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
*  @retval none
*/
void Board_disablePhyAutoMDIX(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Enable AutoMDIX for the particular Port
*
*       MDIO init shall be done before using this function.The Sem handle need to be passed if theThread safe MDIO read is used,
*       Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
*  @retval none
*/

void Board_enablePhyAutoMDIX(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to get the MDIX status of a particular PHY
*
*       MDIO init shall be done before using this function.The Sem handle need to be passed if theThread safe MDIO read is used,
*       Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
*  @retval PHY_ENABLE_AUTO_MDIX if PHY is in Auto MDIX
*          PHY_ENABLE_FORCE_MDIX if PHY is in Forced MDIX
*          PHY_ENABLE_FORCE_MDI if PHY is in Forced MDI
*/
uint8_t Board_getPhyMDIXStat(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to Configure MDI/X Mode of PHY
*
*       API to enable MDI/MDIX or AutoMDIX mode
*       MDIO init shall be done before using this function.The Sem handle need to be passed if theThread safe MDIO read is used,
*       Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
* @param mdiState           [IN] MDI/MDIX mode to be set
*
* @retval none
*/
void Board_setPhyMDIX(uint32_t mdioBaseAddress, uint32_t phyNum,
                      uint8_t mdiState);
/**
* @brief Function to Enable Power Saving modes of the PHY
*
*       This function should be called in advance to use the Power save mode of the PHY.
*       MDIO init shall be done before using this function.The Sem handle need to be passed if theThread safe MDIO read is used,
*       Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
* @retval none
*/
void Board_phyEnablePowerSaveMode(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to force Power Saving mode in PHY
*
*       Following modes are supported
*       PHY_POWERMODE_NORMAL        Normal opearion with PHY fully functional
*       PHY_POWERMODE_DOWN          Shuts down all internal circuitry except SMI functionality
*       PHY_POWERMODE_ACTIVE_SLEEP  Shuts down all internal circuitry except SMI and energy detect functionalities.
*                                   In this mode the PHY sends NLP every 1.4 Sec to wake up link-partner.
*                                   Automatic power-up is done when link partner is detected
*       PHY_POWERMODE_PASSIVE_SLEEP Shuts down all internal circuitry except SMI and energy detect functionalities.
*                                   Automatic power-up is done when link partner is detected
*
*       Power Save mode should be enabled to use the Power Saving feaures. MDIO init shall be done before using this function
*       The Sem handle need to be passed if theThread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
* @param phyPowerMode       [IN] Mode to be set
*
* @retval none
*/
void Board_setPhyPowerSaveMode(uint32_t mdioBaseAddress, uint32_t phyNum,
                               uint8_t phyPowerMode);



/**
* @brief Function to enable Fast RXDV Detection
*
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
* @retval none
*/
void Board_phyFastRXDVDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to enable Fast Link Down Detection
*
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
* @param val                [IN] Phy address of the port
*
* @retval none
*/
void Board_phyFastLinkDownDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum,
                                    uint8_t val);
/**
* @brief Function to enable Extended Full Duplex ability
*
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
* @retval none
*/
void Board_phyExtFDEnable(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to enable Enhanced LED Link Functionality
*
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
* @retval none
*/
void Board_phyEnhLEDLinkEnable(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function to enable ODD Nibble detection
*
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
* @retval none
*/
void Board_phyODDNibbleDetEnable(uint32_t mdioBaseAddress, uint32_t phyNum);

/**
* @brief Enables Detection of Receive Symbol Error During IDLE State
*
*       Function is used to enable Receive Symbol Error During IDLE State
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port

* @retval none
*/
void Board_phyRxErrIdleEnable(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
* @brief Function does the  LED Configuration of PHY
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
* @param mode Led Config mode

* @retval none
*/
void Board_phyLedConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                        uint8_t mode);

/**
* @brief Function does the  LED Configuration in Mode 3 of PHY
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
*
* @retval none
*/
void Board_TLK110_phyLedConfigMode3(uint32_t mdioBaseAddress, uint32_t phyNum);


/**
* @brief Function to Configure EthernetLED Blink rate
*
*       Function can be used to configure the PHY LED Blink rate.Valid values incase of TLK PHYs are
*       500ms,200ms,100ms and 50 ms. MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
* @param val                [IN] Supported rate mode
*
* @retval none
*/
void Board_phyLedBlinkConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                             uint16_t val);

/**
* @brief Function to Configure Ethernet LED polarities
*
*       Function can be used to configure polarity of SPEED(Bit2), LINK (Bit1) and ACT (Bit0) LEDs
*       See tlk datasheet for more details MDIO init shall be done before using this function,
*       the Sem handle need to be passed if the Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
* @param mask               [IN] Speed | Link | Activity. Set mask bit for Active HIGH
*
* @retval none
*/

void Board_phyLedSpeedLinkActPolarity(uint32_t mdioBaseAddress, uint32_t phyNum,
                                      uint16_t mask);
/**
* @brief Function to get the PHY Speed and duplexity
*
*       This API returns the Speed and Duplexity Configuration of the PHY
*       MDIO init shall be done before using this function, The Sem handle need to be passed if the
*       Thread safe MDIO read is used, Pass NULL otherwise
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyNum             [IN] Phy address of the port
*
*  @retval PHY_CONFIG_10FD 10 Mbps and Full duplex
*          PHY_CONFIG_10HD 10 Mbps and Half duplex
*          PHY_CONFIG_100FD 100 Mbps and Full duplex
*          PHY_CONFIG_100HD 100 Mbps and Half duplex
*/

uint8_t Board_getPhyConfig(uint32_t mdioBaseAddress, uint32_t phyNum);

/**
@}
*/

/** @addtogroup PHY_TLK105_SPECIFIC_FUNCTIONS
 @{ */

/**
* @brief Function does the  MLED Configuration of PHY
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
* @param mode MLed Config mode

* @retval none
*/
void Board_phyMLEDConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                         uint16_t mode);

/**
@}
*/

/** @addtogroup PHY_TLK110_SPECIFIC_FUNCTIONS
 @{ */

/**
* @brief Function to do S/w Strap configuration
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port

* @retval none
*/
void Board_phySwStrapConfigDone(uint32_t mdioBaseAddress, uint32_t phyNum);
/**
@}
*/

/**
* @brief Function to get the PHY ready status
*
**      Function returns whether the PHY is identified and ready
*
*       MDIO init shall be done before using this function
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyAddr            [IN] Phy address of the port
*
*  @retval TRUE if Phy is ready
*          FALSE if Phy not ready
*/
uint8_t Board_getPhyIdentifyStat(uint32_t mdioBaseAddress, uint32_t phyAddr);
/**
* @brief Function returns AutoNeg status of the PHY
*
**      Function returns whether the PHY is identified and ready
*
*       MDIO init shall be done before using this function,
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyAddr            [IN] Phy address of the port
*
*  @retval TRUE if Phy is ready
*          FALSE if Phy not ready
*/
uint8_t Board_getAutoNegStat(uint32_t mdioBaseAddress, uint32_t phyAddr);

/**
* @brief Function does the  LED_1 Configuration of DP83822
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
* @param mode LED_1 Config mode
* @param mdioSemhandle Semaphore handle if thread safe MDIO access is used

* @retval none
*/
void Board_phyLED1Config(uint32_t mdioBaseAddress, uint32_t phyNum,
                         uint16_t mode);

/**
* @brief Function to set AM_PHY1_RXCLK and AM_PHY1_LED1 as inputs
*
*       MDIO init shall be done before using this function,
*  @retval none
*/
void Board_phySetGPIORxclkLedInput();

/**
* @brief Function to drive BootConfig_FET_EN high
*
*       MDIO init shall be done before using this function,
*  @retval none
*/
void Board_phyEnableFETswitch();
#endif

/**
* @brief Function to set ANAR register of the PHY to support 100Base-TX Full-Duplex
*
*       MDIO init shall be done before using this function,
*
* @param mdioBaseAddress    [IN] MDIO Base Address
* @param phyAddr            [IN] Phy address of the port
*
*  @retval none
*/
void Board_phyAutoNegAdvConfigFD(uint32_t mdioBaseAddress, uint32_t phyAddr);

void Board_enablePhyMII(uint32_t mdioBaseAddress, uint32_t phyNum);

