/**
 * tiescphy_tlk110.c
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

#include <ti/csl/src/ip/icss/V0/cslr_icssm_iep.h>
#include <ti/csl/src/ip/icss/V0/cslr_icssm_mii_mdio.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/src/ip/mdio/V2/cslr_mdio.h>
#include <ti/csl/src/ip/mdio/V2/csl_mdio.h>
//#include <ti/csl/src/ip/mdio/V2/csl_mdioAux.h>

#include <Ethercat/Include_ethercat/tiescbsp.h>
#include <Include/Board_am335x/board_tlkphy.h>
#include <Include/Board_am335x/board_spi.h>


void bsp_ethphy_init(PRUICSS_Handle pruIcssHandle, uint8_t phy0addr,
                     uint8_t phy1addr, uint8_t enhancedlink_enable)
{

    while(!Board_getPhyIdentifyStat((((PRUICSS_HwAttrs *)(
                                          pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr))
    {
    }

    while(!Board_getPhyIdentifyStat((((PRUICSS_HwAttrs *)(
                                          pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr))
    {
    }

    //SWSCR2
    //Bit2: Enable RXERR during IDLE detection
    //Bit1: Disable detection of transmit error in odd-nibble boundary for odd nibble insertion)
    //Bit5: Force Full-Duplex while working with link partner in forced 100B-TX. When the
    //PHY is set to Auto-Negotiation or Force 100B-TX and the link partner is operated
    //in Force 100B-TX, the link is always Full Duplex
    //For EtherCAT : Disable enhanced LED link function

    Board_phyExtFDEnable((((PRUICSS_HwAttrs *)(
                               pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr);
    Board_phyExtFDEnable((((PRUICSS_HwAttrs *)(
                               pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr);
    Board_phyODDNibbleDetEnable((((PRUICSS_HwAttrs *)(
                                      pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr);
    Board_phyODDNibbleDetEnable((((PRUICSS_HwAttrs *)(
                                      pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr);
    /*Board_phyRxErrIdleEnable((((PRUICSS_HwAttrs *)(
                                   pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr);
    Board_phyRxErrIdleEnable((((PRUICSS_HwAttrs *)(
                                   pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr);*/

    //Enable enhanced link detection in TLK110 for EtherCAT
    //Bit3: Drop the link based on RX Error count of the MII interface, when a predefined number
    // of 32 RX Error occurrences in a 10us interval is reached, the link will be dropped
    // Bit0: Drop the link based on Signal/Energy loss indication, when the Energy detector
    //indicates Energy Loss, the link will be dropped. Typical reaction time is 10us.
#if !defined(icev2AM335x)/*AM3 PDK's Board_phyLedConfig function doesn't have MODE3 option*/
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
                             pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr, LED_CFG_MODE3);
    Board_phyLedConfig((((PRUICSS_HwAttrs *)(
                             pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr, LED_CFG_MODE3);
#else
    Board_TLK110_phyLedConfigMode3((((PRUICSS_HwAttrs *)(
                                         pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr);
    Board_TLK110_phyLedConfigMode3((((PRUICSS_HwAttrs *)(
                                         pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr);
#endif

    Board_phyLedBlinkConfig((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr, LED_BLINK_200);
    Board_phyLedBlinkConfig((((PRUICSS_HwAttrs *)(
                                  pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr, LED_BLINK_200);
    Board_phyFastLinkDownDetEnable((((PRUICSS_HwAttrs *)(
                                         pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr,
                                   FAST_LINKDOWN_SIGENERGY | FAST_LINKDOWN_RXERR);
    Board_phyFastLinkDownDetEnable((((PRUICSS_HwAttrs *)(
                                         pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr,
                                   FAST_LINKDOWN_SIGENERGY | FAST_LINKDOWN_RXERR);

    /*Board_phyFastRXDVDetEnable( ( ( ( PRUICSS_HwAttrs * )(
                                        pruIcssHandle->hwAttrs ) )->prussMiiMdioRegBase ), phy0addr, NULL );
    Board_phyFastRXDVDetEnable( ( ( ( PRUICSS_HwAttrs * )(
                                        pruIcssHandle->hwAttrs ) )->prussMiiMdioRegBase ), phy1addr, NULL );*/
    Board_phySwStrapConfigDone((((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy0addr);
    Board_phySwStrapConfigDone((((PRUICSS_HwAttrs *)(
                                     pruIcssHandle->hwAttrs))->prussMiiMdioRegBase), phy1addr);
}
