/* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
*
*    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "plat_mux.h"
#include "osdrv_mmu.h"

#ifndef _S3_APPL_CNFG_H
#define _S3_APPL_CNFG_H

#if 0
AM335X_INDCOMM_MUX idkMux[] = {
    { 0x0820 , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm2A_mux1	gpmc_ad8 ==> PWM C
    { 0x0824 , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm2B_mux1	gpmc_ad9 ==> PWM D
    { 0x082C , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm0_synco_mux1	gpmc_ad11 ==> PWM SyncOut
    { 0x0840 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mt1_clk	gpmc_a0
    { 0x0844 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd3	gpmc_a1
    { 0x0848 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd2	gpmc_a2
    { 0x084C , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd1	gpmc_a3
    { 0x0850 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd0	gpmc_a4
    { 0x0854 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd3	gpmc_a5
    { 0x0858 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd2	gpmc_a6
    { 0x085C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd1	gpmc_a7
    { 0x0860 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd0	gpmc_a8
    { 0x0864 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr1_clk	gpmc_a9
    { 0x0868 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxdv	gpmc_a10
    { 0x086C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxer	gpmc_a11
    { 0x0874 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txen	gpmc_wpn
    { 0x0878 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxlink	gpmc_be1n
    { 0x0888 , 5 , AM335X_PIN_INPUT_PULLUP },	//	pr1_mdio_data	gpmc_csn3
    { 0x088C , 5 , AM335X_PIN_OUTPUT_PULLUP },	//	pr1_mdio_mdclk	gpmc_clk
    { 0x08A0 , 2 , AM335X_PIN_INPUT },		//	pr1_mii_mt0_clk_mux1	lcd_data0
    { 0x08A4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txen_mux1	lcd_data1
    { 0x08A8 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd3_mux1	lcd_data2
    { 0x08AC , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd2_mux1	lcd_data3
    { 0x08B0 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd1_mux1	lcd_data4
    { 0x08B4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd0_mux1	lcd_data5
    { 0x08B8 , 7 , AM335X_PIN_OUTPUT },	//	gpio2[12]	lcd_data6  ==> ECAT RUNLED
    { 0x08D0 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxlink	lcd_data12
    { 0x08D4 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxer	lcd_data13
    { 0x08D8 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr0_clk	lcd_data14
    { 0x08DC , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxdv	lcd_data15
    { 0x08E0 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[3]	lcd_vsync
    { 0x08E4 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[2]	lcd_hsync
    { 0x08E8 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x08EC , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[29]	rmii1_refclk  ==> ECAT LED ACT1
    { 0x0950 , 3 , AM335X_PIN_OUTPUT },	//	ehrpwm0A_mux1	spi0_sclk ==> PWM A
    { 0x0954 , 3 , AM335X_PIN_OUTPUT },	//	ehrpwm0B_mux1	spi0_d0   ==> PWM B
    { 0x0958 , 2 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C1_SDA_mux3	spi0_d1
    { 0x095C , 2 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C1_SCL_mux3	spi0_cs0
    { 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out	uart0_ctsn
    { 0x096C , 7 , AM335X_PIN_OUTPUT },	//	gpio1[9]	uart0_rtsn 	{ 0x09b0 , 4 , AM335X_PIN_OUTPUT;	//	spi1_cs1_mux4/CS	XDMA_EVENT_INTR0 -- muxed in the CPLD!!
    // HVS882 pinmux
    { 0x097C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[13]	uart1_rtsn -- INDUS_LDn (load pulse for HVS882)
    { 0x0980 , 5 , AM335X_PIN_INPUT },	    //	pr1_uart0_rxd_mux1	uart1_rxd
    { 0x0984 , 5 , AM335X_PIN_OUTPUT },	//	pr1_uart0_txd_mux1	uart1_txd
    // SPI pinmux
    { 0x0994 , 3 , AM335X_PIN_OUTPUT },	//	spi1_d0_mux2/SIMO	mcasp0_fsx
    { 0x0998 , 3 , AM335X_PIN_INPUT_PULLUP },	//	spi1_d1_mux2/SOMI	mcasp0_axr0
    { 0x0990 , 3 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT_PULLDOWN },	//	spi1_sclk_mux2/CLK	mcasp0_aclkx . 
    { 0x099c , 3 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4/CS	mcasp0_ahclkr -- muxed in the CPLD!!
    { 0x09A4 , 5 , AM335X_PIN_OUTPUT },	//	pr1_pru0_pru_r30[5]	mcasp0_fsr
    { 0x09A8 , 7 , AM335X_PIN_OUTPUT },	//	gpio3[20]	mcasp0_axr1    ==> ECAT LED ERR
    { 0x09AC , 7 , AM335X_PIN_OUTPUT },	//	gpio3[21]	mcasp0_ahclkx  ==> ECAT LED ACT0
    { 0x09b0 , 7 , AM335X_PIN_OUTPUT },	//	spi1_cs1_mux4/CS	XDMA_EVENT_INTR0 -- muxed in the CPLD!!
    {0xFFFFFFFF,0,0}
};


AM335X_INDCOMM_MUX iceMux[] = {
    { 0x0800 , 0 , AM335X_PIN_OUTPUT }, 	//	gpmc_ad0	gpmc_ad0
    { 0x0804 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad1	gpmc_ad1
    { 0x0808 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad2	gpmc_ad2
    { 0x080C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad3	gpmc_ad3
    { 0x0810 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad4	gpmc_ad4
    { 0x0814 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad5	gpmc_ad5
    { 0x0818 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad6	gpmc_ad6
    { 0x081C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad7	gpmc_ad7
    { 0x0820 , 0 , AM335X_PIN_OUTPUT },	//  gpmc_ad8
    { 0x0824 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad9
    { 0x0828 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad10
    { 0x082C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad11
    { 0x0830 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad12
    { 0x0834 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad13
    { 0x0838 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad14
    { 0x083C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad15
    { 0x0840 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mt1_clk	gpmc_a0
    { 0x0844 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd3	gpmc_a1
    { 0x0848 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd2	gpmc_a2
    { 0x084C , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd1	gpmc_a3
    { 0x0850 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd0	gpmc_a4
    { 0x0854 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd3	gpmc_a5
    { 0x0858 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd2	gpmc_a6
    { 0x085C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd1	gpmc_a7
    { 0x0860 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd0	gpmc_a8
    { 0x0864 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr1_clk	gpmc_a9
    { 0x0868 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxdv	gpmc_a10
    { 0x086C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxer	gpmc_a11
    { 0x0870 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_wait0	gpmc_wait0
    { 0x0874 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txen	gpmc_wpn
    { 0x0878 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxlink	gpmc_be1n
    { 0x087C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_csn0
#ifdef ICSS_EDIO_ENABLED
    { 0x0880 , 4 , AM335X_PIN_OUTPUT },	// 	pr1_edio_data_out6 // TM: Debug only
#else
    { 0x0880 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_csn1
#endif
    { 0x0884 , 7 , AM335X_PIN_OUTPUT },	//	gpmc_csn2 - GPIO1[31] -> TLK110 Port1 Reset
    { 0x0888 , 5 , AM335X_PIN_INPUT_PULLUP },	//	pr1_mdio_data	gpmc_csn3
    { 0x088C , 5 , AM335X_PIN_OUTPUT_PULLUP },	//	pr1_mdio_mdclk	gpmc_clk
    { 0x0890 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_advn_ale	gpmc_advn_ale
    { 0x0894 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_oen_ren	gpmc_oen_ren
    { 0x0898 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_wen	gpmc_wen
    { 0x089C , 7 , AM335X_PIN_OUTPUT },	//	gpmc_be0n_cle	GPIO2[5] -> TLK110 Port0 Reset
    { 0x08A0 , 2 , AM335X_PIN_INPUT },		//	pr1_mii_mt0_clk_mux1	lcd_data0
    { 0x08A4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txen_mux1	lcd_data1
    { 0x08A8 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd3_mux1	lcd_data2
    { 0x08AC , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd2_mux1	lcd_data3
    { 0x08B0 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd1_mux1	lcd_data4
    { 0x08B4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd0_mux1	lcd_data5

#ifdef ICE_ALPHA
    { 0x08CC , 7 , AM335X_PIN_OUTPUT },	//	lcd_data11 GPIO2[17] -> HVS_CS
#else
    { 0x08CC , 7 , AM335X_PIN_OUTPUT },	//	lcd_data11 GPIO2[17] -> LCD_power_enable
#endif
    { 0x08D0 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxlink	lcd_data12
    { 0x08D4 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxer	lcd_data13
    { 0x08D8 , 5 , AM335X_PIN_INPUT },	//	pr1_mii_mr0_clk	lcd_data14
    { 0x08DC , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08E0 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[3]	lcd_vsync
    { 0x08E4 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[2]	lcd_hsync
    { 0x08E8 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x08EC , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x0908 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[0]	mii1_col	-> ECAT LED1
    { 0x090C ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[1]	mii1_crs	-> ECAT LED2
    { 0x0910 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[2]	mii1_rxerr	-> ECAT LED3
    { 0x0914 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[3]	mii1_txen	-> ECAT LED4
    { 0x091C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[16]	mii1_txd3 -> LED 5a Green
    { 0x0920 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[17]	mii1_txd2 -> LED 5b Red
    { 0x0948 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[0]	mdio_data -> LED 6a
    { 0x094C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[1]	mdio_clk  -> LED 6b
    { 0x0950 , 0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	spi0_sclk
#ifdef ICSS_EDIO_ENABLED
    { 0x0954 , 7 , AM335X_PIN_OUTPUT },	//	GPIO0[3] // debug signal only
    { 0x0958 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edio_data_out0 // TM: Debug signal only

#else
    { 0x0954 , 0 , AM335X_PIN_INPUT },	//	spi0_DO
    { 0x0958 , 0 , AM335X_PIN_OUTPUT },	//	spi0_DI
#endif

    { 0x095C , 0 , AM335X_PIN_OUTPUT },	//	spi0_cs0
    // enable ECAT SYNC0 for DC Tests
    //{ 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out -> DC Test!
    { 0x0968 , 7 , AM335X_PIN_OUTPUT },	//	gpio1[8]
    { 0x096C , 7 , AM335X_PIN_OUTPUT },	//	gpio1[9]	uart0_rtsn -> DC test GPIO
#ifdef ICE_ALPHA
    { 0x0978 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[12] uart1_ctsn -> LED 5c Blue (Alpha)
#else
    { 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[12] uart1_ctsn -> TLK_SYSBOOT_FET (Beta)
#endif
    { 0x097C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[13] uart1_rtsn -> LED 6c Blue
    { 0x0988 ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SDA	-> config by boot code ?
    { 0x098C ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SCL
    // SPI pinmux
    { 0x0994 , 3 , AM335X_PIN_OUTPUT },	//	spi1_d0_mux2/SIMO	mcasp0_fsx
    { 0x0998 , 3 , AM335X_PIN_INPUT_PULLUP },	//	spi1_d1_mux2/SOMI	mcasp0_axr0
    { 0x0990 , 3 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT_PULLDOWN },	//	spi1_sclk_mux2/CLK	mcasp0_aclkx . IMPORTATN: You need to enable both, input and output. Input clock is feed back into the RX logic of SPI.
#ifdef ICE_ALPHA
	//{ 0x099c , 3 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4/CS	mcasp0_ahclkr -- muxed in the CPLD!!
#else
	{ 0x099c , 7 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4: HVS SPI1 CS in GPIO mode
#endif
	{ 0x09A0 , 7 , AM335X_PIN_OUTPUT },	//	gpio3[18]	mcasp0_aclkr -> INDUS_LDn
    {0xFFFFFFFF,0,0}
};
#endif
MUX_CONFIG idkMux[] = {
    { 0x0820 , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm2A_mux1	gpmc_ad8 ==> PWM C
    { 0x0824 , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm2B_mux1	gpmc_ad9 ==> PWM D
    { 0x082C , 4 , AM335X_PIN_OUTPUT },	//	ehrpwm0_synco_mux1	gpmc_ad11 ==> PWM SyncOut
    { 0x0840 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mt1_clk	gpmc_a0
    { 0x0844 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd3	gpmc_a1
    { 0x0848 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd2	gpmc_a2
    { 0x084C , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd1	gpmc_a3
    { 0x0850 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd0	gpmc_a4
    { 0x0854 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd3	gpmc_a5
    { 0x0858 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd2	gpmc_a6
    { 0x085C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd1	gpmc_a7
    { 0x0860 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd0	gpmc_a8
    { 0x0864 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr1_clk	gpmc_a9
    { 0x0868 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxdv	gpmc_a10
    { 0x086C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxer	gpmc_a11
    { 0x0874 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txen	gpmc_wpn
    { 0x0878 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxlink	gpmc_be1n
    { 0x0880 , 7 , AM335X_PIN_OUTPUT },	    		//	GPIO1[30] gmpc_cs1n			// FW test pin
    { 0x0888 , 5 , AM335X_PIN_INPUT_PULLUP },	//	pr1_mdio_data	gpmc_csn3
    { 0x088C , 5 , AM335X_PIN_OUTPUT_PULLUP },	//	pr1_mdio_mdclk	gpmc_clk
    { 0x08A0 , 2 , AM335X_PIN_INPUT },		//	pr1_mii_mt0_clk_mux1	lcd_data0
    { 0x08A4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txen_mux1	lcd_data1
    { 0x08A8 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd3_mux1	lcd_data2
    { 0x08AC , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd2_mux1	lcd_data3
    { 0x08B0 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd1_mux1	lcd_data4
    { 0x08B4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd0_mux1	lcd_data5
    { 0x08B8 , 7 , AM335X_PIN_OUTPUT },	//	gpio2[12]	lcd_data6  ==> ECAT RUNLED
    { 0x08D0 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxlink	lcd_data12
    { 0x08D4 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxer	lcd_data13
    { 0x08D8 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr0_clk	lcd_data14
    { 0x08DC , 5 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxdv	lcd_data15
    { 0x08E0 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[3]	lcd_vsync
    { 0x08E4 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[2]	lcd_hsync
    { 0x08E8 , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x08EC , 6 , AM335X_PIN_INPUT },	    //	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[29]	rmii1_refclk  ==> ECAT LED ACT1
    { 0x0950 , 3 , AM335X_PIN_OUTPUT },	//	ehrpwm0A_mux1	spi0_sclk ==> PWM A
    { 0x0954 , 3 , AM335X_PIN_OUTPUT },	//	ehrpwm0B_mux1	spi0_d0   ==> PWM B
    { 0x0958 , 2 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C1_SDA_mux3	spi0_d1
    { 0x095C , 2 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C1_SCL_mux3	spi0_cs0
    { 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out	uart0_ctsn
#ifdef DC_SYNC_IRQ_LATENCY_DEBUG
    { 0x096C , 7 , AM335X_PIN_OUTPUT },	//	gpio1[9]	uart0_rtsn 	{ 0x09b0 , 4 , AM335X_PIN_OUTPUT;	//	spi1_cs1_mux4/CS	XDMA_EVENT_INTR0 -- muxed in the CPLD!!
#else
    { 0x096C , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync1_out	uart0_rtsn
#endif
    { 0x978,   6,  AM335X_PIN_INPUT}, // pr1_edc_latch0_in        uart1_ctsn
    // HVS882 pinmux
    { 0x097C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[13]	uart1_rtsn -- INDUS_LDn (load pulse for HVS882)
    { 0x0980 , 5 , AM335X_PIN_INPUT },	    //	pr1_uart0_rxd_mux1	uart1_rxd
    { 0x0984 , 5 , AM335X_PIN_OUTPUT },	//	pr1_uart0_txd_mux1	uart1_txd
    // SPI pinmux
    { 0x0990 , 3 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT_PULLDOWN },	//	spi1_sclk_mux2/CLK	mcasp0_aclkx .
    { 0x0994 , 3 , AM335X_PIN_OUTPUT },	//	spi1_d0_mux2/SIMO	mcasp0_fsx
    { 0x0998 , 3 , AM335X_PIN_INPUT_PULLUP },	//	spi1_d1_mux2/SOMI	mcasp0_axr0
    { 0x099c , 3 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4/CS	mcasp0_ahclkr -- muxed in the CPLD!!
    { 0x09A4 , 5 , AM335X_PIN_OUTPUT },	//	pr1_pru0_pru_r30[5]	mcasp0_fsr
    { 0x09A8 , 7 , AM335X_PIN_OUTPUT },	//	gpio3[20]	mcasp0_axr1    ==> ECAT LED ERR
    { 0x09AC , 7 , AM335X_PIN_OUTPUT },	//	gpio3[21]	mcasp0_ahclkx  ==> ECAT LED ACT0
    { 0x09b0 , 7 , AM335X_PIN_OUTPUT },	//	spi1_cs1_mux4/CS	XDMA_EVENT_INTR0 -- muxed in the CPLD!!
    {0xFFFFFFFF,0,0}
};

#if 0
MUX_CONFIG iceMux[] = {
    { 0x0840 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mt1_clk	gpmc_a0
    { 0x0844 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd3	gpmc_a1
    { 0x0848 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd2	gpmc_a2
    { 0x084C , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd1	gpmc_a3
    { 0x0850 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd0	gpmc_a4
    { 0x0854 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd3	gpmc_a5
    { 0x0858 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd2	gpmc_a6
    { 0x085C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd1	gpmc_a7
    { 0x0860 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd0	gpmc_a8
    { 0x0864 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr1_clk	gpmc_a9
    { 0x0868 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxdv	gpmc_a10
    { 0x086C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxer	gpmc_a11
    { 0x0874 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txen	gpmc_wpn
    { 0x0878 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxlink	gpmc_be1n
    { 0x087C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_csn0
    { 0x0884 , 7 , AM335X_PIN_OUTPUT },	//	gpmc_csn2 - GPIO1[31] -> TLK110 Port1 Reset
    { 0x0888 , 5 , AM335X_PIN_INPUT_PULLUP },	//	pr1_mdio_data	gpmc_csn3
    { 0x088C , 5 , AM335X_PIN_OUTPUT_PULLUP },	//	pr1_mdio_mdclk	gpmc_clk
    { 0x0890 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_advn_ale	gpmc_advn_ale
    { 0x0894 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_oen_ren	gpmc_oen_ren
    { 0x0898 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_wen	gpmc_wen
    { 0x089C , 7 , AM335X_PIN_OUTPUT },	//	gpmc_be0n_cle	GPIO2[5] -> TLK110 Port0 Reset
    { 0x08A0 , 2 , AM335X_PIN_INPUT },		//	pr1_mii_mt0_clk_mux1	lcd_data0
    { 0x08A4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txen_mux1	lcd_data1
    { 0x08A8 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd3_mux1	lcd_data2
    { 0x08AC , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd2_mux1	lcd_data3
    { 0x08B0 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd1_mux1	lcd_data4
    { 0x08B4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd0_mux1	lcd_data5
    { 0x08D0 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxlink	lcd_data12
    { 0x08D4 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxer	lcd_data13
    { 0x08D8 , 5 , AM335X_PIN_INPUT },	//	pr1_mii_mr0_clk	lcd_data14
    { 0x08DC , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x091C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[16]	mii1_txd3 -> LED 5a Green
    { 0x0920 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[17]	mii1_txd2 -> LED 5b Red
    { 0x0950 , 0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	spi0_sclk
    { 0x0954 , 7 , AM335X_PIN_OUTPUT },	//	GPIO0[3] // debug signal only
#ifdef ICSS_EDIO_ENABLED
    { 0x0958 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edio_data_out0 // TM: Debug only When we use IO profile, the SPI flash init will overwrite this.
#else
    { 0x0958 , 0 , AM335X_PIN_OUTPUT },	//	spi0_DI
#endif
    { 0x095C , 0 , AM335X_PIN_OUTPUT },	//	spi0_cs0
    { 0x096C , 7 , AM335X_PIN_OUTPUT },	//	gpio1[9]	uart0_rtsn -> DC test GPIO
    { 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[12] uart1_ctsn -> TLK_SYSBOOT_FET (Beta)
    { 0x0988 ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SDA	-> config by boot code ?
    { 0x098C ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SCL
    { 0x0994 , 3 , AM335X_PIN_OUTPUT },	//	spi1_d0_mux2/SIMO	mcasp0_fsx
    { 0x0998 , 3 , AM335X_PIN_INPUT_PULLUP },	//	spi1_d1_mux2/SOMI	mcasp0_axr0
    { 0x0990 , 3 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT_PULLDOWN },	//	spi1_sclk_mux2/CLK	mcasp0_aclkx . IMPORTATN: You need to enable both, input and output. Input clock is feed back into the RX logic of SPI.
	{ 0x099c , 7 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4: HVS SPI1 CS in GPIO mode
    {0xFFFFFFFF,0,0}
};
#endif
MUX_CONFIG iceMux[] = {
    { 0x0840 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mt1_clk	gpmc_a0
    { 0x0844 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd3	gpmc_a1
    { 0x0848 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd2	gpmc_a2
    { 0x084C , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd1	gpmc_a3
    { 0x0850 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txd0	gpmc_a4
    { 0x0854 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd3	gpmc_a5
    { 0x0858 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd2	gpmc_a6
    { 0x085C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd1	gpmc_a7
    { 0x0860 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxd0	gpmc_a8
    { 0x0864 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii_mr1_clk	gpmc_a9
    { 0x0868 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxdv	gpmc_a10
    { 0x086C , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxer	gpmc_a11
    { 0x0874 , 5 , AM335X_PIN_OUTPUT },	//	pr1_mii1_txen	gpmc_wpn
    { 0x0878 , 5 , AM335X_PIN_INPUT },	    //	pr1_mii1_rxlink	gpmc_be1n
    { 0x087C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_csn0
    { 0x0884 , 7 , AM335X_PIN_OUTPUT },	//	gpmc_csn2 - GPIO1[31] -> TLK110 Port1 Reset
    { 0x0888 , 5 , AM335X_PIN_INPUT_PULLUP },	//	pr1_mdio_data	gpmc_csn3
    { 0x088C , 5 , AM335X_PIN_OUTPUT_PULLUP },	//	pr1_mdio_mdclk	gpmc_clk
    { 0x0890 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_advn_ale	gpmc_advn_ale
    { 0x0894 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_oen_ren	gpmc_oen_ren
    { 0x0898 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_wen	gpmc_wen
    { 0x089C , 7 , AM335X_PIN_OUTPUT },	//	gpmc_be0n_cle	GPIO2[5] -> TLK110 Port0 Reset
    { 0x08A0 , 2 , AM335X_PIN_INPUT },		//	pr1_mii_mt0_clk_mux1	lcd_data0
    { 0x08A4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txen_mux1	lcd_data1
    { 0x08A8 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd3_mux1	lcd_data2
    { 0x08AC , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd2_mux1	lcd_data3
    { 0x08B0 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd1_mux1	lcd_data4
    { 0x08B4 , 2 , AM335X_PIN_OUTPUT },	//	pr1_mii0_txd0_mux1	lcd_data5
    { 0x08D0 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxlink	lcd_data12
    { 0x08D4 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxer	lcd_data13
    { 0x08D8 , 5 , AM335X_PIN_INPUT },	//	pr1_mii_mr0_clk	lcd_data14
    { 0x08DC , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x091C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[16]	mii1_txd3 -> LED 5a Green
    { 0x0920 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[17]	mii1_txd2 -> LED 5b Red
    { 0x0950 , 0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	spi0_sclk
    { 0x0954 , 7 , AM335X_PIN_OUTPUT },	//	GPIO0[3] // debug signal only
#ifdef ICSS_EDIO_ENABLED
    { 0x0958 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edio_data_out0 // TM: Debug only When we use IO profile, the SPI flash init will overwrite this.
#else
    { 0x0958 , 0 , AM335X_PIN_OUTPUT },	//	spi0_DI
#endif
    { 0x095C , 0 , AM335X_PIN_OUTPUT },	//	spi0_cs0
    { 0x096C , 7 , AM335X_PIN_OUTPUT },	//	gpio1[9]	uart0_rtsn -> DC test GPIO
    { 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[12] uart1_ctsn -> TLK_SYSBOOT_FET (Beta)
    { 0x0988 ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SDA	-> config by boot code ?
    { 0x098C ,  0 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT },	//	I2C0_SCL
    { 0x0994 , 3 , AM335X_PIN_OUTPUT },	//	spi1_d0_mux2/SIMO	mcasp0_fsx
    { 0x0998 , 3 , AM335X_PIN_INPUT_PULLUP },	//	spi1_d1_mux2/SOMI	mcasp0_axr0
    { 0x0990 , 3 , AM335X_PIN_OUTPUT | AM335X_PIN_INPUT_PULLDOWN },	//	spi1_sclk_mux2/CLK	mcasp0_aclkx . IMPORTATN: You need to enable both, input and output. Input clock is feed back into the RX logic of SPI.
	{ 0x099c , 7 , AM335X_PIN_OUTPUT },	//	spi1_cs0_mux4: HVS SPI1 CS in GPIO mode
    {0xFFFFFFFF,0,0}
};

/* Pinmux for ICE V1 */
MUX_CONFIG ice_v1Mux[] = {
    { 0x0800 , 0 , AM335X_PIN_OUTPUT }, //	gpmc_ad0	gpmc_ad0
    { 0x0804 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad1	gpmc_ad1
    { 0x0808 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad2	gpmc_ad2
    { 0x080C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad3	gpmc_ad3
    { 0x0810 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad4	gpmc_ad4
    { 0x0814 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad5	gpmc_ad5
    { 0x0818 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad6	gpmc_ad6
    { 0x081C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad7	gpmc_ad7
    { 0x0820 , 0 , AM335X_PIN_OUTPUT },	//  gpmc_ad8
    { 0x0824 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad9
    { 0x0828 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad10
    { 0x082C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad11
    { 0x0830 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad12
    { 0x0834 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad13
    { 0x0838 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad14
    { 0x083C , 0 , AM335X_PIN_OUTPUT },	//	gpmc_ad15
    { 0x0870 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_wait0	gpmc_wait0
#ifdef ICSS_EDIO_ENABLED
    { 0x0880 , 4 , AM335X_PIN_OUTPUT },	// 	pr1_edio_data_out6 // firmware test output pin
#else
    { 0x0880 , 0 , AM335X_PIN_OUTPUT },	//	gpmc_csn1
#endif
    { 0x08CC , 7 , AM335X_PIN_OUTPUT },	//	lcd_data11 GPIO2[17] -> LCD_power_enable
    { 0x08E0 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[3]	lcd_vsync
    { 0x08E4 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[2]	lcd_hsync
    { 0x08E8 , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x08EC , 6 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x0908 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[0]	mii1_col	-> ECAT LED1
    { 0x090C ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[1]	mii1_crs	-> ECAT LED2
    { 0x0910 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[2]	mii1_rxerr	-> ECAT LED3
    { 0x0914 ,  7 , AM335X_PIN_OUTPUT },	//	gpio3[3]	mii1_txen	-> ECAT LED4
	{ 0x0944 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[12] uart1_ctsn -> TLK_SYSBOOT_FET (Beta)
    { 0x0948 , 7 , AM335X_PIN_OUTPUT },	//	gpio0[0]	mdio_data -> LED 6a
    { 0x094C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[1]	mdio_clk  -> LED 6b
    { 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out -> DC mode - uart0_ctsn
    { 0x097C , 7 , AM335X_PIN_OUTPUT },	//	gpio0[13] uart1_rtsn -> LED 6c Blue
    { 0x09A0 , 7 , AM335X_PIN_OUTPUT },	//	gpio3[18]	mcasp0_aclkr -> INDUS_LDn
    {0xFFFFFFFF,0,0}
};

/* Pinmux for ICE V2 */
#if 0
MUX_CONFIG ice_v2Mux[] = {
	{ 0x0870 , 5 , AM335X_PIN_OUTPUT },	//	gpmc_wait0	gpmc_wait0
	{ 0x0880 , 7 , AM335X_PIN_OUTPUT },	//	GPMC_CSN(1) -> LED_4
    { 0x08C0 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08C4 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08C8 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08CC , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
	{ 0x08EC , 2 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x08E8 , 2 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x0918 , 7 , AM335X_PIN_OUTPUT },	//  ICE V2.1 GPIO3_4 for CTL_SYS_BOOT of U37 and U38 to isolate the SYSBOOT Resistors from the PHY pins
    { 0x092C , 7 , AM335X_PIN_OUTPUT }, //  MII1_TXCLK -> LED_3
    { 0x0930 , 7 , AM335X_PIN_OUTPUT }, //  GPIO3_10 for MUX control configuration 0- PRU-MII1 , 1- RMII2
    { 0x0960 , 0 , AM335X_PIN_INPUT },	/* McSPI CS 1 */
	{ 0x0964 , 7 , AM335X_PULL_UP },	/* GPIO0_7 for FET_nOE */
    { 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out -> DC mode - uart0_ctsn
    { 0x09B0 , 7 , AM335X_PIN_OUTPUT }, //  XDMA_EVENT_INTR(0) -> LED_5
    { 0x09B4 , 7 , AM335X_PIN_OUTPUT }, //  XDMA_EVENT_INTR(1) -> LED_6
    { 0x0A1C , 7 , AM335X_PULL_UP },	/* DDR_VTT_EN - GPIO0_18 */
    { 0x09A0 , 7 , AM335X_PIN_OUTPUT_PULLUP },	//	gpio3[18]	mcasp0_aclkr -> INDUS_LDn
    {0xFFFFFFFF,0,0}
};
#endif
MUX_CONFIG ice_v2Mux[] = {
	{ 0x0870 , 5 , AM335X_PIN_OUTPUT },	//	gpmc_wait0	gpmc_wait0
	{ 0x0880 , 7 , AM335X_PIN_OUTPUT },	//	GPMC_CSN(1) -> LED_4
    { 0x08C0 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08C4 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08C8 , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
    { 0x08CC , 5 , AM335X_PIN_INPUT },	//	pr1_mii0_rxdv	lcd_data15
	{ 0x08EC , 2 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[0]	lcd_ac_bias_en
    { 0x08E8 , 2 , AM335X_PIN_INPUT },	//	pr1_mii0_rxd_post[1]	lcd_pclk
    { 0x0918 , 7 , AM335X_PIN_OUTPUT },	//  ICE V2.1 GPIO3_4 for CTL_SYS_BOOT of U37 and U38 to isolate the SYSBOOT Resistors from the PHY pins
    { 0x092C , 7 , AM335X_PIN_OUTPUT }, //  MII1_TXCLK -> LED_3
    { 0x0930 , 7 , AM335X_PIN_OUTPUT }, //  GPIO3_10 for MUX control configuration 0- PRU-MII1 , 1- RMII2
    { 0x0960 , 0 , AM335X_PIN_INPUT },	/* McSPI CS 1 */
	{ 0x0964 , 7 , AM335X_PULL_UP },	/* GPIO0_7 for FET_nOE */
    { 0x0968 , 6 , AM335X_PIN_OUTPUT },	//	pr1_edc_sync0_out -> DC mode - uart0_ctsn
    { 0x09B0 , 7 , AM335X_PIN_OUTPUT }, //  XDMA_EVENT_INTR(0) -> LED_5
    { 0x09B4 , 7 , AM335X_PIN_OUTPUT }, //  XDMA_EVENT_INTR(1) -> LED_6
    { 0x0A1C , 7 , AM335X_PULL_UP },	/* DDR_VTT_EN - GPIO0_18 */
    { 0x09A0 , 7 , AM335X_PIN_OUTPUT_PULLUP },	//	gpio3[18]	mcasp0_aclkr -> INDUS_LDn
    {0xFFFFFFFF,0,0}
};


SYS_MMU_ENTRY applMmuEntries[] = {
		{(void*)0x08000000,SYS_MMU_CACHEABLE},  //NOR - Non bufferable| Cacheable
		    {(void*)0x08100000,SYS_MMU_CACHEABLE},  //NOR - Non bufferable| Cacheable
		    {(void*)0x08200000,SYS_MMU_CACHEABLE},  //NOR - Non bufferable| Cacheable
		    {(void*)0x40300000,0},  //L3 - Non bufferable| Non Cacheable
		    {(void*)0x48300000,0},  //PWM - Non bufferable| Non Cacheable
		    {(void*)0x48200000,0},  //INTCPS,MPUSS - Non Bufferable| Non Cacheable
		    {(void*)0x48100000,0},  //I2C2,McSPI1,UART3,UART4,UART5, GPIO2,GPIO3,MMC1 - Non bufferable| Non Cacheable
		    {(void*)0x48000000,0},  //UART1,UART2,I2C1,McSPI0,McASP0 CFG,McASP1 CFG,DMTIMER,GPIO1 -Non bufferable| Non Cacheable
		    {(void*)0x44E00000,0},  //Clock Module, PRM, GPIO0, UART0, I2C0, - Non bufferable| Non Cacheable
		    {(void*)0x4A300000,0},  //PRUSS1 -Non Bufferable| Non Cacheable
		    {(void*)0x49000000,0},  //EDMA3 - Non bufferable| Non Cacheable
		    {(void*)0x4A000000,0},  //L4 FAST CFG- Non bufferable| Non Cacheable
		    {(void*)0xFFFFFFFF,0xFFFFFFFF}
};

#endif
