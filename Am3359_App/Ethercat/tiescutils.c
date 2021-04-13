/**
 * tiescutils.c
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

#include <stdint.h>
#ifndef AMIC11X_ONCHIP_MEM_MODE
// get the firmware loaded as header files!


#include <Ethercat/Firmware/v1.0/ecat_frame_handler_bin.h>
#include <Ethercat/Firmware/v1.0/ecat_host_interface_bin.h>

#endif

#include <osal/TaskP.h>
#include <osal/OSP.h>


#include <Ethercat/Include_ethercat/tiescutils.h>
#include <Ethercat/Include_ethercat/tiescmiscappl.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>
#include <Include/Board_am335x/board_misc.h>


#include <Ethercat/Lib/EtherCATSampleLibrary.h>


#include <Ethercat/Version/version.h>

/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#ifndef DISABLE_UART_PRINT
#include <ti/drv/uart/UART_stdio.h>
#endif
#include <ti/board/board.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>


#include <Include/Board_am335x/soc_icss_header.h>

#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>

#include <Include/Board_am335x/board_gpioLed.h>
#include <Include/Board_am335x/board_misc.h>
#include <Include/Board_am335x/board_spi.h>
#include <Include/Board_am335x/board_i2cLed.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** \brief Global Structure pointer holding PRUSS0 memory Map. */
PRUICSS_Handle pruIcss1Handle;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


extern void Demo_Application(void);
extern void Demo_StateTrans(unsigned short state);
extern uint8_t HardwareOpened;

#ifdef iceAMIC11x
extern int32_t Board_phyReset(uint8_t numPorts);
#endif
TaskP_Handle tsk1;               // ECAT mainloop
TaskP_Handle pdiTask; // ECAT sw ISR
TaskP_Handle uartTask;           // UART processing
TaskP_Handle ledTaskHndl;        // LED Control Task
TaskP_Handle sync0Task;          // ECAT SYNC0 ISR

TaskP_Handle sync0Task; // ECAT SYNC0 ISR
void Sync0task(uint32_t arg0, uint32_t arg1);
TaskP_Handle sync1Task; // ECAT SYNC1 ISR
void Sync1task(uint32_t arg0, uint32_t arg1);

uint32_t appState = 0;
pECAT_SLAVE_INTERFACE pSlaveInterface = NULL;

#define PD_INPUT_SIZE 4
#define PD_OUTPUT_SIZE 4

void task1(uint32_t arg0, uint32_t arg1)
{
    Bool bRunApplication = TRUE;
    TaskP_Params taskParams;

#ifndef DISABLE_UART_PRINT
    UART_printf("\nVersion - ");
    UART_printf(IND_PKG_VERSION);

    Board_IDInfo boardInfo;
#ifndef SOC_K2G
    Board_getIDInfo(&boardInfo);
#endif

    UART_printf("\nBoard name \t: ");
    UART_printf(boardInfo.boardName);

    UART_printf("\n\rBoard Revision \t: ");
    UART_printf(boardInfo.version);

    if(isEtherCATDevice())
    {
        UART_printf("EtherCAT Device\n\r");
    }

    else
    {
        UART_printf("Non-EtherCAT Device\n\r");
    }

    UART_printf("\n\rSYS/BIOS EtherCAT Demo application");
    UART_printf(APPL_BUILD_VER);
    UART_printf("\r\nBuild Timestamp      : %s %s", __DATE__, __TIME__);
#endif

    bsp_soc_evm_init();

#ifndef AMIC11X_ONCHIP_MEM_MODE
    /* map the array which contains pru firmware instrcutions.
       If the application is built to execute from internal RAM completely,then
       pru instructions are expcted to be stored in SPI flash and this mapping is
       not required*/

    bsp_set_pru_firmware((uint32_t *)FrameProc, sizeof(FrameProc),
                         (uint32_t *)HostProc, sizeof(HostProc));

#endif
    pSlaveInterface = Ecat_Open(PD_INPUT_SIZE, PD_OUTPUT_SIZE);

    tiesc_foe_eoe_init();

    if(pSlaveInterface != NULL)
    {
        //assign application function pointer
        pSlaveInterface->pApplication = (void (*)(void))&Demo_Application;
        pSlaveInterface->pStateTrans = &Demo_StateTrans;

        /* Create tasks */
        /* Create tree tasks that share a resource*/
        TaskP_Params_init(&taskParams);
        taskParams.priority = 6;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        pdiTask = TaskP_create(PDItask, &taskParams);

        TaskP_Params_init(&taskParams);
        taskParams.priority = 4;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        ledTaskHndl = TaskP_create(LEDtask, &taskParams);

        TaskP_Params_init(&taskParams);
        taskParams.priority = 8;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        sync0Task = TaskP_create(Sync0task, &taskParams);

        TaskP_Params_init(&taskParams);
        taskParams.priority = 8;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        sync1Task = TaskP_create(Sync1task, &taskParams);

        do
        {
            Ecat_OnTimer();
            TaskP_yield();
        }
        while(bRunApplication == TRUE);
    }

    Ecat_Close();

    OSAL_OS_exit(0);
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief Interrupt service routine for the interrupts from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////

void HW_EcatIsr(void)
{
#ifdef PROFILE_ECAT_STACK
    uint32_t pdi_start, pdi_stop;
    /* get the AL event register */
    bsp_get_local_sys_time(&pdi_start,  0);
#endif
    PDI_Isr();
#ifdef PROFILE_ECAT_STACK
    bsp_get_local_sys_time(&pdi_stop,   0);
    pdi_delta = pdi_stop - pdi_start;

    if(pdi_delta > pdi_max)
    {
        pdi_max = pdi_delta;
    }

#endif
}


void PDItask(uint32_t arg1, uint32_t arg2)
{
    TaskP_sleep(10 * OS_TICKS_IN_MILLI_SEC);
    uint32_t evtOutNum = HOST_AL_EVENT - 20;

    while(1)
    {
        PRUICSS_pruWaitEvent((PRUICSS_Handle)arg1, evtOutNum);
        /* ISR processing */
        HW_EcatIsr();
    }
}



void LEDtask(uint32_t arg0, uint32_t arg1)
{
#ifdef ENABLE_STARTUP_DIGOUT_ANIMATION
    VARVOLATILE uint8_t state = STATE_INIT;
    static uint16_t digout_led_mask = 1;

    while(STATE_INIT == state)
    {
        I2CSetLed(&gI2CObj, digout_led_mask);
        digout_led_mask <<= 1;

        if(digout_led_mask >= 256)
        {
            digout_led_mask = 1;
        }

        state = bsp_read_byte(0x130);
        TaskP_sleep(200 * OS_TICKS_IN_MILLI_SEC);
    }

#else
    TaskP_sleep(200 * OS_TICKS_IN_MILLI_SEC);
    Board_setDigOutput(0x6a);

    TaskP_sleep(100 * OS_TICKS_IN_MILLI_SEC);
#endif

    while(1)
    {
        uint32_t reset_reg_val;
        TaskP_sleep(50 * OS_TICKS_IN_MILLI_SEC);

        if(bsp_get_eeprom_update_status())
        {
            uint32_t t_cur_time;
            bsp_get_local_sys_time(&t_cur_time, NULL);
            uint32_t t_last_time = bsp_get_eeprom_updated_time();
            t_cur_time = ((t_cur_time >= t_last_time) ? (t_cur_time - t_last_time) :
                          t_cur_time + (0xFFFFFFFF - t_last_time));

            if(t_cur_time >= (ECAT_TIMER_INC_P_MS * 100))
            {
                bsp_set_eeprom_update_status(0);
                bsp_eeprom_emulation_flush();
            }
        }

        reset_reg_val = bsp_read_dword(pruIcss1Handle, ESC_ADDR_TI_ESC_RESET);

        if((reset_reg_val == TI_ESC_RST_CMD_U) ||
                (reset_reg_val == TI_ESC_RST_CMD_L))
        {
            //EtherCAT master has requested S/W RESET
            HW_RestartTarget();
        }
    }

}

void Sync0task(uint32_t arg1, uint32_t arg2)
{
#ifdef PROFILE_ECAT_STACK
    uint32_t sync_start, sync_stop;
#endif
#ifndef EXEC_FROM_IRAM
#ifndef NO_UART_MSG_APP
    UART_printf("SYNC0 task started\n\r");
#endif
#endif
    uint32_t evtOutNum = HOST_SYNC0_EVENT - 20;

    while(1)
    {
        PRUICSS_pruWaitEvent((PRUICSS_Handle)arg1, evtOutNum);
        //Do sync0 event handling
        DISABLE_ESC_INT();
#ifdef PROFILE_ECAT_STACK
        bsp_get_local_sys_time(&sync_start, 0);
#endif
        Sync0_Isr();
#ifdef PROFILE_ECAT_STACK
        bsp_get_local_sys_time(&sync_stop,  0);
        sync_delta = sync_stop - sync_start;

        if(sync_delta > sync_max)
        {
            sync_max = sync_delta;
        }

#endif
        ENABLE_ESC_INT();
    }
}

void Sync1task(uint32_t arg1, uint32_t arg2)
{
#ifdef PROFILE_ECAT_STACK
    uint32_t sync_start, sync_stop;
#endif
#ifndef EXEC_FROM_IRAM
#ifndef NO_UART_MSG_APP
    UART_printf("SYNC1 task started\n\r");
#endif
#endif
    uint32_t evtOutNum = HOST_SYNC1_EVENT - 20;

    while(1)
    {
        PRUICSS_pruWaitEvent((PRUICSS_Handle)arg1, evtOutNum);
        //Do sync1 event handling
        DISABLE_ESC_INT();
        Sync1_Isr();
        ENABLE_ESC_INT();
    }
}
void common_main()
{
    TaskP_Params taskParams;
#ifdef iceAMIC11x
    Board_init(BOARD_INIT_MODULE_CLOCK);

    Board_phyReset(2);

    /* mux PRU MII after PHY reset in case PRU drives signals
     * on the pins and interfere with the PHY bootstrap configurations
    */
#ifndef DISABLE_UART_PRINT
    Board_init(BOARD_INIT_UART_STDIO | BOARD_INIT_ICSS_PINMUX);
#else
    Board_init(BOARD_INIT_ICSS_PINMUX);

#endif
#else
    Board_init(BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO);
#endif
    TaskP_Params_init(&taskParams);
    taskParams.priority = 4;
    taskParams.stacksize = 2048*TIESC_TASK_STACK_SIZE_MUL;
    tsk1 = TaskP_create(task1, &taskParams);
    OSAL_OS_start();
}


