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

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// Protocol IDs - must be the same on the PRU side
#define PROTOCOL_INVAL 0xffff
#define PROTOCOL_POWERLINK 0xab88
#define PROTOCOL_PROFINET 0x9288
#define PROTOCOL_ETHERCAT 0xa488
#define PROTOCOL_ENIP 0x0002
#define PROTOCOL_IP 0x0008
#define PROTOCOL_TCP 0x06
#define PROTOCOL_UDP 0x11
#define PROTOCOL_SERCOS3 0xcd88
#define PROTOCOL_VLAN 0x0081
#define PROTOCOL_PROFIBUS 0x1337

#define LINK_STATUS_UP 0x0000
#define LINK_STATUS_DOWN 0x0001

#define ADDR_PROTOCOL_TYPE (0x4a310000 + 0)
#define SIZE_PROTOCOL_TYPE 2

#define RTC_SS 0x44E3E000
#define RTC_SCRATCH0_REG (RTC_SS + 0x60)
#define RTC_SS_KICK0 (RTC_SS + 0x6c)
#define RTC_SS_KICK1 (RTC_SS + 0x70)
#define GPIO_OE 0x134

enum app_selector
{
	SELECTOR_APP = 0,
	ETHERCAT = 1,
	PROFINET = 2,
	ETHERNETIP = 3,
	POWERLINK = 4,
	SERCOS3 = 5
};

#define UNLOCK_RTC_SS() \
	HWREG(RTC_SS_KICK0) = 0x83e70b13; \
	HWREG(RTC_SS_KICK1) = 0x95a4f1e0

#define LOCK_RTC_SS() \
		HWREG(RTC_SS_KICK0) = 0x83e70b13;
		//HWREG() = 0x95a4f1e0

#endif /* PROTOCOL_H_ */
