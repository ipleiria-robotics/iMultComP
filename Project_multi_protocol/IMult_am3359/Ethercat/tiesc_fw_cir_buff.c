/**
 * tiesc_fw_cir_buff.c
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


#include <Ethercat/Include_ethercat/tiesc_fw_cir_buff.h>


#define FW_CIRC_BUFF_LEN  2048

volatile static uint32_t write_indx = 0;
volatile static uint32_t read_indx = 0;
volatile static uint8_t cir_buff[FW_CIRC_BUFF_LEN];



void incr_cir_buff_index(uint32_t *index)
{
    (*index)++;

    if(*index == FW_CIRC_BUFF_LEN)
    {
        *index = 0;
    }
}

void write_to_cir_buff(uint8_t d_byte)
{
    cir_buff[ write_indx ] = d_byte;
    incr_cir_buff_index((uint32_t *)&write_indx);

    if(write_indx == read_indx)
    {
        incr_cir_buff_index((uint32_t *)&read_indx);
    }
}

uint32_t get_cir_buff_avail_bytes()
{
    if(write_indx >= read_indx)
    {
        return write_indx - read_indx;
    }

    else
    {
        return ((FW_CIRC_BUFF_LEN - read_indx) + write_indx);
    }
}

uint32_t read_from_cir_buff(uint8_t *outBuff, uint32_t buff_size)
{
    uint32_t itr1 = 0;
    uint32_t avail_bytes = get_cir_buff_avail_bytes();


    if(avail_bytes == 0)
    {
        return 0;
    }

    for(itr1 = 0 ; (itr1 < avail_bytes) && (itr1 < buff_size) ; itr1++)
    {
        outBuff[itr1] = cir_buff[read_indx];
        incr_cir_buff_index((uint32_t *)&read_indx);
    }

    return avail_bytes;
}
