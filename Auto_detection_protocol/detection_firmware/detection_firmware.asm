;/* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
; * 
; * 
; *  Redistribution and use in source and binary forms, with or without 
; *  modification, are permitted provided that the following conditions 
; *  are met:
;*
;*    Redistributions of source code must retain the above copyright 
; *    notice, this list of conditions and the following disclaimer.
;*
;*    Redistributions in binary form must reproduce the above copyright
;*    notice, this list of conditions and the following disclaimer in the 
; *    documentation and/or other materials provided with the   
; *    distribution.
;*
;*    Neither the name of Texas Instruments Incorporated nor the names of
;*    its contributors may be used to endorse or promote products derived
;*    from this software without specific prior written permission.
;*
;*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
; *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
; *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
; *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
; *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
; *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
; *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
; *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;*
;*/
;
; 	file:   detection_firmware.asm
;
; 	brief:
;
;	Version		Description								Author
; 	0.1     	Created								Thomas Mauer
;  	0.2			Modified the receive function		Thomas Mauer
;	0.3			Added industrial protocol detection	Fabian Fischer
;   0.4			Ported project to CCSv6				Thomas Mauer


struct_MAC .struct
ctlreg32	.long
resL32		.long
resH32		.long
op1_32		.long
op2_32		.long
		.endstruct
sMAC	.sassign	R25, struct_MAC

		.asg	R25.b0, sMAC_ctlreg32_b0

;*****************************
; Defines Section
;*****************************

PRU_ARM_EVENT0			.set	(18 - 16)		; SYS_EVENT_18 is pr1_pru_mst_intr<2>_int_req
PRU0_ARM_IRQ			.set	(0x20 | PRU_ARM_EVENT0)

;R31 Command masks
D_TX_CRC_ERR			.set	0x8000
D_RESET_TXFIFO			.set	0x4000
D_TX_EOF				.set	0x2000
D_PUSH_ERR_NIBBLE_CMD	.set	0x1000
D_PUSH_CRC_MSWORD_CMD	.set	0x0800
D_PUSH_CRC_LSWORD_CMD	.set	0x0400
D_PUSH_WORD_CMD			.set	0x0200
D_PUSH_BYTE_CMD			.set	0x0100
D_RX_ERROR_CLEAR		.set	0x0080
D_RX_SOF_CLEAR			.set	0x0040
D_RX_SFD_CLEAR			.set	0x0020
D_RX_EOF_CLEAR			.set	0x0010
D_RESET_RXFIFO			.set	0x0004
D_POP_WORD_CMD			.set	0x0002
D_POP_BYTE_CMD			.set	0x0001


RTC_SS					.set	0x44E3E000
RTC_SCRATCH0_REG 		.set	(RTC_SS + 0x60)
RTC_SS_KICK0 			.set	(RTC_SS + 0x6c)
RTC_SS_KICK1 			.set	(RTC_SS + 0x70)

OFFSET_MDIO_LINK 		.set	0x0c
PORT0 					.set	1
PORT1 					.set	2

; Protocol IDs - must be the same on the ARM side
PROTOCOL_INVAL 			.set	0xffff
PROTOCOL_POWERLINK 		.set	0xab88
PROTOCOL_PROFINET 		.set	0x9288
PROTOCOL_ETHERCAT 		.set	0xa488
PROTOCOL_ENIP 			.set	0x0002
PROTOCOL_IP 			.set	0x0008
PROTOCOL_TCP 			.set	0x06
PROTOCOL_UDP 			.set	0x11
PROTOCOL_SERCOS3 		.set	0xcd88
PROTOCOL_VLAN 			.set	0x0081
PROTOCOL_ARP 			.set	0x0608

LINK_STATUS_UP 			.set	0x0000
LINK_STATUS_DOWN 		.set	0x0001

OFFSET_PROTOCOL_TYPE 	.set	0x00
SIZE_PROCOTOL_TYPE 		.set	2

APP_SELECTOR 			.set	0
APP_ETHERCAT 			.set	1
APP_PROFINET 			.set	2
APP_ETHERNETIP 			.set	3
APP_POWERLINK 			.set	4
APP_SERCOS3 			.set	5

MAC_DEV_ID				.set	0
MAC_MODE_MPY			.set	0
MAC_MODE_MAC			.set	1

;*****************************
; Includes Section
;*****************************


;*****************************
; Macro Section
;*****************************
M_SET_CMD .macro param
	LDI		R31.w2, param
	.endm

MAC_setmode .macro mode
		LDI		sMAC.ctlreg32.b0, (mode)
		xout	MAC_DEV_ID, &sMAC.ctlreg32, 1
	.endm

MAC_calc .macro
		or		sMAC_ctlreg32_b0, sMAC_ctlreg32_b0, 2
		xout	MAC_DEV_ID, &sMAC.ctlreg32, 1
		xor		sMAC_ctlreg32_b0, sMAC_ctlreg32_b0, 2
		xout	MAC_DEV_ID, &sMAC.ctlreg32, 1
		xin		MAC_DEV_ID, &sMAC.resL32, 8
	.endm


;*****************************
; Assembler Directives Section
;*****************************
	.sect	".text"
	.retain 	; Required for building .out with assembly file
	.retainrefs ; Required for building .out with assembly file
	.global	main


main:
	; Frame Parser Initialization
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR | D_RESET_TXFIFO | D_TX_EOF
	ZERO	&R0, 124
	;MAC_setmode		MAC_MODE_MPY Todo: Is this needed?

FRAME_PROCESSING_START:

	; Invalidate protocol number at host side
	LDI		R24.w0, LINK_STATUS_DOWN
	SBCO	&R24.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		IDLE_TASK

NO_CABLE:
	; update only if previously a cable was connected
	LBCO	&R24.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBEQ	IDLE_TASK, R24.w0, LINK_STATUS_DOWN
	; Invalidate protocol number at host side
	LDI		R24.w0, LINK_STATUS_DOWN
	SBCO	&R24.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	; generate host interrupt
	LDI		R31.w0, PRU0_ARM_IRQ

IDLE_TASK:
	; Check if cable in PORT0 is plugged in
	LBCO	&R24, C21, OFFSET_MDIO_LINK, 4
	AND		R24, R24, (1<<PORT0)
	QBEQ	NO_CABLE, R24, 0x00
	; Cable is plugged in - update only if there was previously no cable plugged in
	LBCO	&R24.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBNE	IDLE_TASK_CHECK_FOR_DATA, R24.w0, LINK_STATUS_DOWN
	LDI		R24.w0, LINK_STATUS_UP
	SBCO	&R24.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	; generate host interrupt
	LDI		R31.w0, PRU0_ARM_IRQ

IDLE_TASK_CHECK_FOR_DATA:
	; FRAME_PROCESSING_WAIT_FOR_SOF:
	; PRU is idle, can do other tasks
	; one task is to check if a new frame is getting received
	XIN		20, &R2, (4 * 17) ; Read in RX L2 FIFO, this will update registers R2 - R18

	JAL		R29.w2, FN_FRAME_PROCESSING_CHECK_FOR_EOF
	; Check for Error
	QBBS	IDLE_TASK_CHECK_FOR_DATA_ERROR, R20, 0
	QBBS	IDLE_TASK_CHECK_FOR_DATA_ERROR, R20, 2
	QBBS	IDLE_TASK_CHECK_FOR_DATA_ERROR, R20, 7
	QBA		IDLE_TASK_CHECK_FOR_DATA_NO_ERROR
IDLE_TASK_CHECK_FOR_DATA_ERROR:
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR | D_RESET_TXFIFO | D_TX_EOF
	QBA		IDLE_TASK
IDLE_TASK_CHECK_FOR_DATA_NO_ERROR:
	; check in status area that RX_SFD is set
	QBBC	IDLE_TASK, R10, 4

	; reset frame byte counter
	ZERO	&R21.w0, 2

FRAME_PROCESSING_BANK0:
	; this loop waits until 32 bytes have been received from BANK0 or the RX_EOF bit is set

	; extracts status byte of current byte counter into R20.b0
	; LDI		R31.w0,	PRU0_ARM_IRQ		; generate host interrupt
	JAL		R29.w2, FN_FRAME_PROCESSING_CHECK_FOR_EOF
	; check if frame is done by checking if RX_EOF bit set
	QBBS	FRAME_PROCESSING_EOF_BANK0, R20, 3
	; Check for Error
	QBBS	FRAME_PROCESSING_BANK0_ERROR, R20, 0
	QBBS	FRAME_PROCESSING_BANK0_ERROR, R20, 2
	QBBS	FRAME_PROCESSING_BANK0_ERROR, R20, 7
	QBA		FRAME_PROCESSING_BANK0_NO_ERROR
FRAME_PROCESSING_BANK0_ERROR:
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR | D_RESET_TXFIFO | D_TX_EOF
	QBA		IDLE_TASK
FRAME_PROCESSING_BANK0_NO_ERROR:
	; XIN updated RX L2 bank 0
	XIN		20, &R2, (4 * 17) ; R2 - R18
	; have more than 32 bytes been received?
	QBGE	FRAME_PROCESSING_BANK0, R18.b0, 31
	; 32 bytes have been received
	; Now first check that STATUS_RDY bit of last byte is set
	; if not, ICSS HW has not yet updated the status bits, so firwmare needs to initiate another XIN
	QBBC	FRAME_PROCESSING_BANK0, R13.b3, 1
	JAL		R29.w2, PROCESS_BANK_0
	; now we are done with bank 0, go onto bank1

FRAME_PROCESSING_BANK1:
	; this loop waits until 32 bytes have been received from BANK0 or the RX_EOF bit is set

	; extracts status byte of current byte counter into R20.b0
	JAL		R29.w2, FN_FRAME_PROCESSING_CHECK_FOR_EOF
	; check if frame is done by checking if RX_EOF bit set
	QBBS	FRAME_PROCESSING_EOF_BANK1, R20, 3			;RX_EOF
	; Check for Error
	QBBS	FRAME_PROCESSING_BANK1_ERROR, R20, 0
	QBBS	FRAME_PROCESSING_BANK1_ERROR, R20, 2
	QBBS	FRAME_PROCESSING_BANK1_ERROR, R20, 7
	QBA		FRAME_PROCESSING_BANK1_NO_ERROR
FRAME_PROCESSING_BANK1_ERROR:
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR | D_RESET_TXFIFO | D_TX_EOF
	QBA		IDLE_TASK
FRAME_PROCESSING_BANK1_NO_ERROR:
	; XIN updated RX L2 bank 1
	XIN		21, &R2, (4 * 17) ; R2 - R18
	QBLT 	FRAME_PROCESSING_BANK1, R18.w0, 31	; check if 32 bytes or more are in L2 RX buffer
	; 32 bytes have been received
	; Now first check that STATUS_RDY bit of last byte is set
	; if not, ICSS HW has not yet updated the status bits, so firwmare needs to initiate another XIN
	JAL		R29.w2, PROCESS_BANK_1
	QBBC	FRAME_PROCESSING_BANK1, R13.b3, 1
	; now go back to process bank 0
	QBA		FRAME_PROCESSING_BANK0

PROCESS_BANK_0:
	; R0.b0 contains number of bytes to store, used by SBCO withing SAVE_FRAME
	LDI		R0.b0, 32
	QBA		SAVE_FRAME

PROCESS_BANK_1:
	; R0.b0 contains number of bytes to store, used by SBCO withing SAVE_FRAME
	LDI		R0.b0, 32
	; QBA		SAVE_FRAME	; not required because SAVE_FRAME is next

SAVE_FRAME:
	; store R2 to R9 in PRU0_DRAM (0x4A30.0000)
	SBCO	&R2, c24, R21.w0, b0
	; increment byte counter
	ADD		R21.w0, R21.w0, R0.b0
SAVE_FRAME_DONE:
	JMP		R29.w2

FRAME_PROCESSING_EOF_BANK0:
	XIN		20, &R2, (4 * 17) ; R2 to R18
	; check if more than 32 bytes are in the buffer
	QBGT	FRAME_PROCESSING_EOF_BANK0_REMAINDER, R18.b0, 32
	LDI		R0.b0, 32		; R0 will keep the number of data for SBCO
	JAL		R29.w2, SAVE_FRAME
	SUB		R18.b0, R18.b0, 32	; substract the already written byte count
	; XIN remaining bytes
	XIN		21, &R2, (4 * 12) ; R2 to R13, do not overwrite R18!

FRAME_PROCESSING_EOF_BANK0_REMAINDER:
	MOV		R0.b0, R18.b0	; R0 will keep the number of data for SBCO
	JAL		R29.w2, SAVE_FRAME
	; resets RX L2 FIFO, this also set the write pointer R18 to 0
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR
	; LDI		R31.w0,	PRU0_ARM_IRQ		; generate host interrupt
	; QBA		IDLE_TASK
	QBA		PROCESS_FRAME

FRAME_PROCESSING_EOF_BANK1:
	XIN		21, &R2, (4 * 17) ; R2 to R18
	QBNE	FRAME_PROCESSING_EOF_BANK1_NO_R18_WRAPAROUND, R18.b0, 0 ; need to take care on R18.b0 wraparound
	LDI		R18.b0, 64
FRAME_PROCESSING_EOF_BANK1_NO_R18_WRAPAROUND:
	; check if more than 32 bytes are in the buffer
	QBLE	FRAME_PROCESSING_EOF_BANK1_REMAINDER, R18.b0, 32
	LDI		R0.b0, 32		; R0 will keep the number of data for SBCO
	JAL		R29.w2, SAVE_FRAME
	ADD		R18.b0, R18.b0, 32	; adjust byte count for remainder section
	; get remaining bytes
	XIN		20, &R2, (4 * 12) ; R2 to R13, do not overwrite R18!
FRAME_PROCESSING_EOF_BANK1_REMAINDER:
	SUB		R0.b0, R18.b0, 32	; remove offset
	JAL		R29.w2, SAVE_FRAME
	; resets RX L2 FIFO, this also set the write pointer R18 to 0
	M_SET_CMD	D_RESET_RXFIFO | D_RX_ERROR_CLEAR | D_RX_SOF_CLEAR | D_RX_SFD_CLEAR | D_RX_EOF_CLEAR
	;LDI		R31.w0,	PRU0_ARM_IRQ		; generate host interrupt
	;QBA		IDLE_TASK
	QBA			PROCESS_FRAME


; this function gets called to check for EOF condition
; the status byte is loaded into R20.b0, and has to be checked by the caller
FN_FRAME_PROCESSING_CHECK_FOR_EOF:
	XIN		20, &R10, (4 * 9) ; R10 to R18
	QBNE	FRAME_PROCESSING_CHECK_FOR_EOF_NO_R18_WRAPAROUND, R18.b0, 0
	LDI		R18.b0, 64	; adjust for wrap around
FRAME_PROCESSING_CHECK_FOR_EOF_NO_R18_WRAPAROUND:
	SUB		R18.b0, R18.b0, 1	; set write pointer to last valid entry
	QBGT	FRAME_PROCESSING_CHECK_FOR_EOF_BANK_GOOD, R18.b0, 32
	XIN		21, &R10, (4 *4) ; R10 to R13
	SUB		R18.b0, R18.b0, 32	; adjust byte index
FRAME_PROCESSING_CHECK_FOR_EOF_BANK_GOOD:
	LSR		R1.b0, R18.b0, 1	; compensate status byte merge from 16-bit to 8-bit
	ADD		R1.b0, R1.b0, (10*4) ; add offset to R10
	MVIB	R20.b0, *R1.b0		; status byte of current write pointer is placed into R20.b0
	JMP		R29.w2

;=================================================================================================
; Process frame here (protocol detection, etc...)
;=================================================================================================
PROCESS_FRAME:
; Check Protocol type (offset 0x0c)
	LBCO	&R22, C24, 0x0c, 2
; Check VLAN first, since it can embed other protocols
	LDI		R23.w0, PROTOCOL_VLAN
	QBEQ	PROCESS_PROTOCOL_VLAN, R22.w0, R23.w0
; No VLAN detected - TODO: Optional Add VLAN Tag insertion

PROCESS_OTHER_PROTOCOLS:
	LDI		R23.w0, PROTOCOL_PROFINET
	QBEQ	PROCESS_PROTOCOL_PROFINET, R22.w0, R23.w0
	LDI		R23.w0, PROTOCOL_POWERLINK
	QBEQ	PROCESS_PROTOCOL_POWERLINK, R22.w0, R23.w0
	LDI		R23.w0, PROTOCOL_ETHERCAT
	QBEQ	PROCESS_PROTOCOL_ETHERCAT, R22.w0, R23.w0
	LDI		R23.w0, PROTOCOL_SERCOS3
	QBEQ	PROCESS_PROTOCOL_SERCOS3, R22.w0, R23.w0
	LDI		R23.w0, PROTOCOL_IP
	QBEQ	PROCESS_PROTOCOL_IP, R22.w0, R23.w0

NOTHING_DETECTED:
; We did not detect anything!!
	;SET_APP	APP_SELECTOR, R23, R22
	LDI		R23.w0, PROTOCOL_INVAL
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_PROTOCOL_POWERLINK:
	;SET_APP	APP_POWERLINK, R23, R22
	LDI		R23.w0, PROTOCOL_POWERLINK
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_PROTOCOL_VLAN:
; Protocol number offset is 0x0c+0x04 = 0x10, just update R22.w0 (received protocol number)
	LBCO	&R22.w0, C24, 0x10, 2
; TODO: Optional VLAN Tag removal
	QBA		PROCESS_OTHER_PROTOCOLS

PROCESS_PROTOCOL_IP:
; For now, match source mac address to check for Ethernet/IP (Rockwell MAC: e49069******, offset 0x06)
;	Rockwell does not send ENIP packets if ARP request is not awnsered, so use mac address
	XOR		R22, R22, R22
	LBCO	&R22, C24, 0x06, 3
	LDI32	R23, 0x6990e4
	QBEQ	PROCESS_PROTOCOL_ENIP, R22, R23
	QBA		PROCESS_FRAME_END
;Get IP version (offset 0x0e) - upper 4 bits
	LBCO	&R22, C24, 0x0e, 1
	LSR		R22, R22, 4
	QBEQ	PROCESS_PROTOCOL_IP6, R22, 6

PROCESS_PROTOCOL_IP4:
; Get Header length (offset 0x0e) - lower 4 bits * 4
	LBCO	&R22.b0, C24, 0x0e, 1
	AND		R22.b0, R22.b0, 0x0f
	MOV		sMAC.op1_32, R22.b0
	LDI		sMAC.op2_32, 0x04
	MAC_calc
	MOV		R22.b0, sMAC.resL32

; Get upper protocol (offset 0x17)
	LBCO	&R22.b1, C24, 0x17, 1
	QBEQ	PROCESS_PROTOCOL_TCP, R22.b1, PROTOCOL_TCP
	QBEQ	PROCESS_PROTOCOL_UDP, R22.b1, PROTOCOL_UDP
	QBA		NOTHING_DETECTED

PROCESS_PROTOCOL_IP6:

PROCESS_PROTOCOL_UDP:
; Get dst port and match with ethernet/ip ports (2222, 44818) (offset header_length+0x0e+0x02)
	ADD		R22.b0, R22.b0, 0x10
	LBCO	&R22.w2, C24, R22.b0, 2
	LDI		R22.w0, 2222
	QBEQ	PROCESS_PROTOCOL_ENIP, R22.w2, R22.w0
	LDI		R22.w0, 44818
	QBEQ	PROCESS_PROTOCOL_ENIP, R22.w2, R22.w0
	QBA		NOTHING_DETECTED

PROCESS_PROTOCOL_TCP:
; Get dst port and match with ethernet/ip ports (2222, 44818)
	ADD		R22.b0, R22.b0, 0x10
	LBCO	&R22.w2, C24, R22.b0, 2
	LDI		R22.w0, 0xae08
	QBEQ	PROCESS_PROTOCOL_ENIP, R22.w2, R22.w0
	LDI		R22.w0, 0x12AF
	QBEQ	PROCESS_PROTOCOL_ENIP, R22.w2, R22.w0
	QBA		NOTHING_DETECTED

PROCESS_PROTOCOL_PROFINET:
	;SET_APP	APP_PROFINET, R23, R22
	LDI		R23.w0, PROTOCOL_PROFINET
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_PROTOCOL_ETHERCAT:
	;SET_APP	APP_ETHERCAT, R23, R22
	LDI		R23.w0, PROTOCOL_ETHERCAT
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_PROTOCOL_ENIP:
	;SET_APP	APP_ETHERNETIP, R23, R22
	LDI		R23.w0, PROTOCOL_ENIP
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_PROTOCOL_SERCOS3:
	;SET_APP	APP_SERCOS3, R23, R22
	LDI		R23.w0, PROTOCOL_SERCOS3
; Write protocol number to shared PRU Memory
	SBCO	&R23.w0, C28, OFFSET_PROTOCOL_TYPE, SIZE_PROCOTOL_TYPE
	QBA		PROCESS_FRAME_END

PROCESS_FRAME_END:
; generate host interrupt
	LDI		R31.w0, PRU0_ARM_IRQ
	QBA		IDLE_TASK

