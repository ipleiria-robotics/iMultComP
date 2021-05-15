/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *
 */

/**
*
* \file    board_oled.h
*
* \brief This contains oled specific structure, typedefs, function prototypes
*
******************************************************************************/
#ifndef _ICEK2G_OLED_H_
#define _ICEK2G_OLED_H_

/************************
 * Include Files
 ************************/
#include "icek2g_oledbitmapDb.h"

/************************
 * Defines and Macros
 ************************/
/** Macros definitons for return types */
#define OLED_ERR                    (-1)
#define OLED_SUCCESS                (0)

typedef int16_t OLED_RET;

/************************
 * Function declarations
 ************************/

OLED_RET Board_oledInit(void);
OLED_RET init(void);
OLED_RET clear(void);
uint8_t printchar(uint8_t a);
OLED_RET printstr(int8_t string[]);
void noDisplay(void);
void display(void);
OLED_RET scrollDisplayLeft(void);
OLED_RET scrollDisplayLeftLine(uint8_t line);
OLED_RET scrollDisplayRight(void);
OLED_RET scrollDisplayRightLine(uint8_t line);
OLED_RET flip(void);
OLED_RET setOrientation(uint8_t newDir);
OLED_RET autoscroll(void);
OLED_RET noAutoscroll(void);
OLED_RET setline(uint8_t line);
OLED_RET setRolling(uint8_t row, uint8_t status);
OLED_RET resetCursor(uint8_t line);

#endif /* _ICEK2G_OLED_H_ */
