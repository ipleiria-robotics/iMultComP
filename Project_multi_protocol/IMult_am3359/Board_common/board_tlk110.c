/**
 * \file board_tlk110.c
 * \brief Contains TLK 110 PHY Specific APIs
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

#include <Include/Board_am335x/board_tlkphy.h>
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>

/**
* @brief Function to do S/w Strap configuration of TLK110
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
*
* @retval none
*/
void Board_phySwStrapConfigDone(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;
    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_CR1_REG, &phyregval);
    phyregval |= SWSTRAP_CONFIG_DONE;
    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_CR1_REG, phyregval);
}

/**
* @brief Function does the  LED Configuration of TLK 110
*
* @param mdioBaseAddress MDIO Base Address
* @param phyNum Phy address of the port
* @param mode Led Config mode
*
* @retval none
*/

void Board_phyLedConfig(uint32_t mdioBaseAddress, uint32_t phyNum,
                        uint8_t mode)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregval);

    switch(mode)
    {
        case LED_CFG_MODE1:
            phyregval |= (1 << 5);   //Set LED_CFG[0] to 1
            break;

        case LED_CFG_MODE2:
            phyregval &= 0xFFCF; //Set LED_CFG[0],LED_CFG[1] to 0
            break;

        case LED_CFG_MODE3:
            phyregval &= 0xFFDF; //Set LED_CFG[0] to 0
            phyregval |= 0x0040; //Set LED_CFG[1] to 1
            break;
    }

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, phyregval);
}


void Board_TLK110_phyLedConfigMode3(uint32_t mdioBaseAddress, uint32_t phyNum)
{
    uint16_t phyregval = 0;

    CSL_MDIO_phyRegRead(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, &phyregval);

    //LED_CFG_MODE3:
    phyregval &= 0xFFDF; //Set LED_CFG[0] to 0
    phyregval |= 0x0040; //Set LED_CFG[1] to 1

    CSL_MDIO_phyRegWrite(mdioBaseAddress, phyNum, TLKPHY_PHYCR_REG, phyregval);
}
