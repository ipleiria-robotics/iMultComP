/**
 *  \file   board_rotary_switch.c
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

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/uart/UART_stdio.h>
#include <Include/Board_am335x/delay_us.h>

#include <ti/board/src/icev2AM335x/include/board_cfg.h>


/** Macros for I2C slave address and instance */
#define ROTARY_SWITCH_SLAVE_ADDR                 (0x41U)
#define ROTARY_SWITCH_I2C_INSTANCE               (0U)


extern I2C_Handle i2cLedhandle;
I2C_Transaction i2cTransaction;
uint8_t  regRdBuf;
uint8_t  regWrBuf;
void Board_initRotarySwitch(void)
{
    /*I2C Init */
    I2C_Params i2cParams;
    I2C_init();
    I2C_Params_init(&i2cParams);

    if(i2cLedhandle == NULL)
    {
        i2cLedhandle = I2C_open(ROTARY_SWITCH_I2C_INSTANCE, &i2cParams);
    }

    if(i2cLedhandle == NULL)
    {
        UART_printf("I2C Handle open failed");
    }


    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = ROTARY_SWITCH_SLAVE_ADDR;
    i2cTransaction.writeBuf = &regWrBuf;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = &regRdBuf;
    i2cTransaction.readCount = 0;
    regWrBuf = 0x0;
    regRdBuf = 0x0;
    I2C_transfer(i2cLedhandle, &i2cTransaction);
    delay_us(1000);
    i2cTransaction.writeCount = 0;
    i2cTransaction.readCount = 1;

    I2C_transfer(i2cLedhandle, &i2cTransaction);

    if(regRdBuf == 0x0)
    {
        UART_printf("Rotary switch Detection Failed!\n");
    }

    return;
}

void Board_readRotarySwitch(uint8_t *readBuf)
{
    regRdBuf = 0x0;
    I2C_transfer(i2cLedhandle, &i2cTransaction);
    regRdBuf = regRdBuf & 0x0F;
    *readBuf =
        regRdBuf; //taking only the lower 4 bits which reflect the input ports
    return;
}
