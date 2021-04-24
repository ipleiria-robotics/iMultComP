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
//SYS_EVT_16-31 can be used for generating interrupts for IPC with hosts/prus etc
#define IEP_TIM_CAP_CMP_EVENT	7
#define WATCHDOG1_EVENT			10
#define WATCHDOG2_EVENT			9
#define SYNC1_OUT_EVENT      13
#define SYNC0_OUT_EVENT      14
#define PRU_ARM_EVENT0       18 //SYS_EVT_AL_EVENT_REQUEST
#define PRU_ARM_EVENT1       17 //
#define ARM_PRU_EVENT        16 //SYS_EVT_HOST_CMD
#define PRU0_RX_ERR32_EVENT  33
#define PORT0_TX_UNDERFLOW	 39
#define PORT0_TX_OVERFLOW	 40
#define MII_LINK0_EVENT      41
#define PRU1_RX_ERR32_EVENT  45
#define PORT1_TX_UNDERFLOW	 51
#define PORT1_TX_OVERFLOW	 53
#define MII_LINK1_EVENT      53

#define CHANNEL0                0
#define CHANNEL1                1
#define CHANNEL2                2
#define CHANNEL3                3
#define CHANNEL4                4
#define CHANNEL5                5
#define CHANNEL6                6
#define CHANNEL7                7
#define CHANNEL8                8
#define CHANNEL9                9

#define PRU0                    0
#define PRU1                    1
#define PRU_EVTOUT0             2
#define PRU_EVTOUT1             3
#define PRU_EVTOUT2             4
#define PRU_EVTOUT3             5
#define PRU_EVTOUT4             6
#define PRU_EVTOUT5             7
#define PRU_EVTOUT6             8
#define PRU_EVTOUT7             9

#define PRU0_HOSTEN_MASK            0x0001
#define PRU1_HOSTEN_MASK            0x0002
#define PRU_EVTOUT0_HOSTEN_MASK     0x0004
#define PRU_EVTOUT1_HOSTEN_MASK     0x0008
#define PRU_EVTOUT2_HOSTEN_MASK     0x0010
#define PRU_EVTOUT3_HOSTEN_MASK     0x0020
#define PRU_EVTOUT4_HOSTEN_MASK     0x0040
#define PRU_EVTOUT5_HOSTEN_MASK     0x0080
#define PRU_EVTOUT6_HOSTEN_MASK     0x0100
#define PRU_EVTOUT7_HOSTEN_MASK     0x0200

#define SYS_EVT_POLARITY_LOW        0
#define SYS_EVT_POLARITY_HIGH       1

#define SYS_EVT_TYPE_PULSE          0
#define SYS_EVT_TYPE_EDGE           1

#define PRUSS_PRU0_INTC_INITDATA {   \
{ ARM_PRU_EVENT, PRU_ARM_EVENT0, WATCHDOG1_EVENT, PRU_ARM_EVENT1, IEP_TIM_CAP_CMP_EVENT, PORT0_TX_UNDERFLOW, PORT0_TX_OVERFLOW, -1  },  \
{ {ARM_PRU_EVENT,			CHANNEL1, 	SYS_EVT_POLARITY_HIGH,	SYS_EVT_TYPE_EDGE }, \
  {/*WATCHDOG1_EVENT*/PRU_ARM_EVENT0,			CHANNEL2, 	SYS_EVT_POLARITY_HIGH,	SYS_EVT_TYPE_EDGE }, \
  {PRU_ARM_EVENT1,			CHANNEL1, 	SYS_EVT_POLARITY_HIGH,	SYS_EVT_TYPE_EDGE}, \
  {IEP_TIM_CAP_CMP_EVENT, 	CHANNEL1, 	SYS_EVT_POLARITY_HIGH,	SYS_EVT_TYPE_EDGE }, \
  {0xFF, 0xFF, 0xFF, 0xFF}},  \
{ {CHANNEL0,PRU0}, {CHANNEL1, PRU1}, {CHANNEL2, PRU_EVTOUT0}, {CHANNEL3, PRU_EVTOUT1}, {CHANNEL4, PRU_EVTOUT2}, {-1,-1} },  \
(PRU0_HOSTEN_MASK | PRU1_HOSTEN_MASK | PRU_EVTOUT0_HOSTEN_MASK | PRU_EVTOUT1_HOSTEN_MASK | PRU_EVTOUT2_HOSTEN_MASK) /*Enable PRU0/1, PRU_EVTOUT0/1/2 */ \
} 



