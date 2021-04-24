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
 * \file  board_oled.c
 *
 * \brief This contains specific oled functions.
 *
 ******************************************************************************/

#include <examples/board/include/oled_drv.h>
#include <examples/board/include/board_oled.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <examples/board/include/delay_us.h>

static volatile uint8_t totalcols;
static volatile uint8_t row;
static volatile uint8_t dir;
static volatile uint8_t col[2];
static volatile uint8_t rolling0;
static volatile uint8_t rolling1;

const uint8_t *FontBitmap;
const FONT_CHAR_INFO *FontDescriptors;
const FONT_INFO *FontInfo;
extern const I2C_Config I2C_config[];

I2C_Params i2cParams;
extern I2C_Handle i2cLedhandle;

/******************************************************************************
 *
 * Function:    oledwrite
 *
 * Description: Writes a specified number of bytes to the given slave address.
 *
 * Parameters:  uint8_t *data          - Pointer to the buffer base address
 *              uint32_t numBytes      - Number of bytes of buffer
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *               OLED_RET - OLED_ERR
 *
 ******************************************************************************/
I2C_Transaction transaction;

static OLED_RET oledwrite(uint8_t *data, uint32_t numBytes)
{
    transaction.slaveAddress = OLED_SLAVE_ADDR;
    transaction.writeBuf = data;
    transaction.writeCount = numBytes;
    transaction.readCount = 0;
    I2C_transfer(i2cLedhandle, &transaction);

    return (OLED_SUCCESS);
}

/******************************************************************************
 *
 * Function:    send
 *
 * Description: Sends 2 bytes of data to the OSD9616
 *
 * Parameters:  uint8_t comdat            - Identifies command or data
 *              uint8_t data              - Data to be sent
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *               OLED_RET - OLED_ERR
 *
 ******************************************************************************/
static OLED_RET send(uint8_t comdat, uint8_t data)
{
    uint8_t cmd[2];
    OLED_RET ret;

    cmd[0] = comdat & 0x00FF;     // Specifies whether data is Command or Data
    cmd[1] = data;                // Command / Data
    delay_us(1);

    ret = oledwrite(cmd, 2);
    delay_us(4);

    if(ret == 0)
    {
        return (OLED_SUCCESS);
    }

    else
    {
        return (OLED_ERR);
    }

}

/******************************************************************************
 *
 * Function:    multiSend
 *
 * Description: Sends multiple bytes of data to the OSD9616
 *
 * Parameters:  uint8_t *data             - Pointer to start of I2C transfer
 *              uint32_t len              - Length of I2C transaction
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *               OLED_RET - OLED_ERR
 *
 ******************************************************************************/
static OLED_RET multiSend(uint8_t *data, uint32_t len)
{
    int8_t index;
    uint8_t cmd[10];
    OLED_RET ret;

    for(index = 0; index < len; index++)         // Command / Data
    {
        cmd[index] = data[index];
    }

    delay_us(1);

    ret = oledwrite(cmd, len);
    delay_us(4);

    if(ret == 0)
    {
        return (OLED_SUCCESS);
    }

    else
    {
        return (OLED_ERR);
    }
}


/******************************************************************************
 *
 * Function:    Board_oledInit
 *
 * Description: Function initializes I2C Module and also LCD.
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET Board_oledInit(void)
{
    OLED_RET     returnValue;

    Board_oledGPIOInit();

    int32_t index;
    row = 0;
    totalcols = 0;

    for(index = 0; I2C_config[index].fxnTablePtr != NULL; index++)
    {
        ((I2C_HwAttrs *)I2C_config[index].hwAttrs)->enableIntr = false;
    }

    I2C_init();

    I2C_Params_init(&i2cParams);

    if(i2cLedhandle == NULL)
    {
        i2cLedhandle = I2C_open(BOARD_OLED_DISPLAY_INSTANCE, &i2cParams);
    }

    returnValue = init();

    if(returnValue != OLED_SUCCESS)
    {
        UART_printf("init: init Failed\n");
        return (returnValue);
    }

    FontBitmap = &arial_8ptBitmaps[0];
    FontDescriptors = &arial_8ptDescriptors[0];
    FontInfo = &arial_8ptFontInfo;

    return OLED_SUCCESS;

}

/******************************************************************************
 *
 * Function:    init
 *
 * Description: Function initializes I2C Module and also LCD.
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET init(void)
{
    uint8_t cmd[10];

    /* Note: Wire initialization will be done in main.cpp */
    /* Initialize OSD9616 display */
    send(0x00, 0x00); // Set low column address
    send(0x00, 0x10); // Set high column address

    send(0x00, 0xae); // Turn off oled panel

    send(0x00, 0xd5); // Set display clock divide ratio/oscillator frequency
    send(0x00, 0x80); // Set divide ratio

    cmd[0] = 0x00;   // Set multiplex ratio(1 to 16)
    cmd[1] = 0xa8;
    cmd[2] = 0x0f;
    multiSend(cmd, 3);

    send(0x00, 0xd3); // Set display offset
    send(0x00, 0x00); // Not offset

    send(0x00, 0x40); // Set Display start line address

    cmd[0] = 0x00;  //--set DC-DC enable
    cmd[1] = 0x8d; // Set Charge Pump
    cmd[2] = 0x14;
    multiSend(cmd, 3);

    send(0x00, 0xa1); // Set segment re-map 95 to 0
    send(0x00, 0xc8); // Set COM Output Scan Direction

    cmd[0] = 0x00;  // Set com pins hardware configuration
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    multiSend(cmd, 3);

    cmd[0] = 0x00;  // Set contrast control register
    cmd[1] = 0x81;
    cmd[2] = 0xaf;
    multiSend(cmd, 3);

    cmd[0] = 0x00;  // Set pre-charge period
    cmd[1] = 0xd9;
    cmd[2] = 0xf1;
    multiSend(cmd, 3);

    send(0x00, 0xdb); // Set vcomh
    send(0x00, 0x20); // 0.83*vref

    send(0x00, 0xa4); // Set entire display on/off
    send(0x00, 0xa6); // Set normal display

    send(0x00, 0xaf); // Turn on oled panel

    dir = 0;

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    setOrientation
 *
 * Description: Function to set the orientation of LCD.
 *
 * Parameters:  uint8_t newDir      - Direction of orientation
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET setOrientation(uint8_t newDir)
{
    if(newDir == 0)
    {
        /* Set divide ratio */
        send(0x00, 0xC8);
        dir = 1;
    }

    else if(newDir == 1)
    {
        /* Set divide ratio */
        send(0x00, 0xC0);
        dir = 0;
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    flip
 *
 * Description: Flips the screen vertically
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET flip(void)
{
    if(dir == 0)
    {
        /* Set divide ratios */
        send(0x00, 0xC0);
        send(0x00, 0xA1);
        dir = 1;
    }

    else if(dir == 1)
    {
        /* Set divide ratios */
        send(0x00, 0xC8);
        send(0x00, 0xA1);
        dir = 0;
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    setline
 *
 * Description: Sets the start line for the display
 *
 * Parameters:  uint8_t line     - Line number
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET setline(uint8_t line)
{
    if(line == 0)
    {
        /* Write to page 1 */
        send(0x00, 0x00);  // Set low column address
        send(0x00, 0x10);  // Set high column address
        send(0x00, 0xb0 + 1); // Set page for page 1
    }

    else if(line == 1)
    {
        /* Write to page 0 */
        send(0x00, 0x00);  // Set low column address
        send(0x00, 0x10);  // Set high column address
        send(0x00, 0xb0 + 0); // Set page for page 0

    }

    totalcols = 0;
    row = line;

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    setRolling
 *
 * Description: Sets the rolling parameters
 *
 * Parameters:  uint8_t row          - Row number
 *              uint8_t status       - Status of rolling
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET setRolling(uint8_t row, uint8_t status)
{
    if(row == 0)
    {
        if(status == 0)
        {
            rolling0 = 0;
        }

        else
        {
            rolling0 = 1;
        }
    }

    else if(row == 1)
    {
        if(status == 0)
        {
            rolling1 = 0;
        }

        else
        {
            rolling1 = 1;
        }
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    printstr
 *
 * Description: Prints a string to the LCD
 *
 * Parameters:  uint8_t string[]   - Pointer to the string to be printed
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET printstr(int8_t string[])
{
    int8_t index = 0;

    while(string[index] != '\0')
    {
        if(printchar(string[index]) == 0)
        {
            break;
        }

        index++;
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    printchar
 *
 * Description: Prints a character to the LCD
 *
 * Parameters:  uint8_t a   - Character to be printed
 *
 * Return Value: uint8_t - 0 Total columns are greater than 127
 *               uint8_t - 1 Printed a character
 *
 ******************************************************************************/
uint8_t printchar(uint8_t a)
{
    int16_t index1, index2;

    index2 = (uint8_t) a - 0x20;

    for(index1 = 0; index1 < (FontDescriptors[index2].widthBits); index1++)
    {
        if(totalcols > 127)
        {
            return 0;
        }

        else
        {
            totalcols++;
        }

        send(0x40, FontBitmap[FontDescriptors[index2].offset + index1]);
    }

    if(totalcols > 127)
    {
        return 0;
    }

    else
    {
        totalcols++;
    }

    send(0x40, 0x00);

    if(totalcols > 127)
    {
        return 0;
    }

    else
    {
        totalcols++;
    }

    send(0x40, 0x00); // 2 Spaces
    send(0x00, 0x2e); // Deactivate Scrolling
    return 1;

}

/******************************************************************************
 *
 * Function:    clear
 *
 * Description: Clears the LCD screen and positions the cursor in the upper-left corner
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET clear(void)
{
    int16_t index;
    totalcols = 0;
    //This part is to initialize and blank the LCD.
    /* Initialize OLED display */
    send(0x00, 0x2e); // Deactivate Scrolling

    /* Write to page 0 */
    send(0x00, 0x00);  // Set low column address
    send(0x00, 0x10);  // Set high column address
    send(0x00, 0xb0 + 0); // Set page for page 0 to page 5

    for(index = 0; index < 128; index++)
    {
        send(0x40, 0x00);
    }

    /* Write to page 1*/
    send(0x00, 0x00);  // Set low column address
    send(0x00, 0x10);  // Set high column address
    send(0x00, 0xb0 + 1); // Set page for page 0 to page 5

    for(index = 0; index < 128; index++)
    {
        send(0x40, 0x00);
    }

    /* Set cursor to upper left corner */
    send(0x00, 0x00);  // Set low column address
    send(0x00, 0x10);  // Set high column address
    send(0x00, 0xb0 + 1); // Set page for page 0 to page 5
    row = 0;

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    scrollDisplayLeft
 *
 * Description: Scrolls the contents of the display (text and cursor) to the left
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET scrollDisplayLeft(void)
{

    uint8_t cmd[10]; // For multibyte commands

    // These commands scroll the display without changing the RAM
    /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x27;  // Vertical and Right Horizontal Scroll 26 = left, 27 = right, 29 = vl, 2A = vr
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x00;  // Set time interval between each scroll step as 5 frames
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x00;  // Dummy byte
    cmd[7] = 0xFF;  // Dummy byte
    multiSend(cmd, 8);
    send(0x00, 0x2f);

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    scrollDisplayLeftLine
 *
 * Description: Scrolls the contents of a particular line, of the display (text and
 *      cursor) to the left
 *
 * Parameters:  uint8_t line      - Line number
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET scrollDisplayLeftLine(uint8_t line)
{

    uint8_t cmd[10]; // For multibyte commands

    if((0 == line) || (1 == line))
    {
        // These commands scroll the display without changing the RAM
        /* Set vertical and horizontal scrolling */
        cmd[0] = 0x00;
        cmd[1] = 0x27;  // Vertical and Right Horizontal Scroll 26 = left, 27 = right, 29 = vl, 2A = vr
        cmd[2] = 0x00;  // Dummy byte
        cmd[3] = !line; // Define start page address
        cmd[4] = 0x00;  // Set time interval between each scroll step as 5 frames
        cmd[5] = !line; // Define end page address
        cmd[6] = 0x00;  // Dummy byte
        cmd[7] = 0xFF;  // Dummy byte
        multiSend(cmd, 8);
        send(0x00, 0x2f);
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    autoscroll
 *
 * Description: Turns on automatic scrolling of the LCD
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET autoscroll(void)
{
    uint8_t cmd[10];    // For multibyte commands

    /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x2A;  // Vertical and Right Horizontal Scroll 26 = left, 27 = right, 29 = vl, 2A = vr
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x00;  // Set time interval between each scroll step as 5 frames
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    multiSend(cmd, 7);
    send(0x00, 0x2f);

    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    multiSend(cmd, 4);

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    noAutoscroll
 *
 * Description: Turns off automatic scrolling of the LCD
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET noAutoscroll(void)
{
    send(0x40, 0x00);
    send(0x40, 0x00); // 2 Spaces
    send(0x00, 0x2e); // Deactivate Scrolling

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    scrollDisplayRight
 *
 * Description: Scrolls the contents of the display (text and cursor) to the right
 *
 * Parameters:  void
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET scrollDisplayRight(void)
{
    uint8_t cmd[10];    // For multibyte commands

    /* Set horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x26;  // Vertical and Left Horizontal Scroll 26 = left, 27 = right, 29 = vl, 2A = vr
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x00;  // Set time interval between each scroll step as 5 frames
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x00;  // Dummy byte
    cmd[7] = 0xFF;  // Dummy byte
    multiSend(cmd, 8);
    send(0x00, 0x2f);

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    scrollDisplayRightLine
 *
 * Description: Scrolls the contents of a particular line, of the display (text and
 *      cursor) to the right
 *
 * Parameters:  uint8_t line   - Line number
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET scrollDisplayRightLine(uint8_t line)
{
    uint8_t cmd[10];    // For multibyte commands

    if((0 == line) || (1 == line))
    {
        /* Set horizontal scrolling */
        cmd[0] = 0x00;
        cmd[1] = 0x26;  // Vertical and Left Horizontal Scroll 26 = left, 27 = right, 29 = vl, 2A = vr
        cmd[2] = 0x00;  // Dummy byte
        cmd[3] = !line; // Define start page address
        cmd[4] = 0x00;  // Set time interval between each scroll step as 5 frames
        cmd[5] = !line; // Define end page address
        cmd[6] = 0x00;  // Dummy byte
        cmd[7] = 0xFF;  // Dummy byte
        multiSend(cmd, 8);
        send(0x00, 0x2f);
    }

    return OLED_SUCCESS;
}

/******************************************************************************
 *
 * Function:    noDisplay
 *
 * Description: Turns off the LCD display
 *
 * Parameters:  void
 *
 * Return Value: void
 *
 ******************************************************************************/
void noDisplay(void)
{
    send(0x00, 0xae); // Turn off oled panel
}

/******************************************************************************
 *
 * Function:    display
 *
 * Description: Turns on the LCD display
 *
 * Parameters:  void
 *
 * Return Value: void
 *
 ******************************************************************************/
void display(void)
{
    send(0x00, 0xaf); // Turn on oled panel
}

/******************************************************************************
 *
 * Function:    resetCursor
 *
 * Description: Resets the cursor position to the beginning in a given line
 *
 * Parameters:  uint8_t line   - Line number
 *
 * Return Value: OLED_RET - OLED_SUCCESS
 *
 ******************************************************************************/
OLED_RET resetCursor(uint8_t line)
{
    totalcols = 0;

    if(line == 0)
    {
        /* Write to line 0 */
        send(0x00, 0x00);  // Set low column address
        send(0x00, 0x10);  // Set high column address
        send(0x00, 0xb0 + 1); // Set page for page 0 to page 5
    }

    else if(line == 1)
    {
        /* Write to line 1*/
        send(0x00, 0x00);  // Set low column address
        send(0x00, 0x10);  // Set high column address
        send(0x00, 0xb0 + 0); // Set page for page 0 to page 5
    }

    return OLED_SUCCESS;
}

