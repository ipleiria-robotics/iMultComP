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
 *
 *EtherCAT Slave Controllers (ESC)
 * -----------------------------------------------------------------------------------------------------------------
 * The EtherCAT frame contains one or more datagrams. The datagram header indicates what type of access the master device would like to execute:
 *      -> Read, write, read-write
 *      -> Access to a specific slave device through direct addressing, or access to multiple slave devices through logical addressing (implicit addressing)
 *                         ( ................... Demo_StateTrans ..............)
 *
 * EtherCAT Slave Controllers are available from multiple manufacturers, with the size of the
 * internal DPRAM and the number of Fieldbus Memory Management Units (FMMUs)
 * depending on the variation. Different Process Data Interfaces (PDI) for external
 * access from the application controller to the application memory are also available:
 *
 * this example use:
 *  ->  The Serial Peripheral Interface (SPI) is targeted at applications with small amounts of
 *       process data, such as analog I/O devices, encoders, or simple drives.
 *
 *
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

#include "app.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** \brief Global Structure pointer holding PRUSS0 memory Map. */
PRUICSS_Handle pruIcss1Handle;

#define ESC_ADDR_SM0_PHYS_ADDR      (0x800)
#define ESC_ADDR_SM1_PHYS_ADDR      (ESC_ADDR_SM0_PHYS_ADDR + 8)
#define ESC_ADDR_SM2_PHYS_ADDR      (ESC_ADDR_SM0_PHYS_ADDR + 2 * 8)
#define ESC_ADDR_SM3_PHYS_ADDR      (ESC_ADDR_SM0_PHYS_ADDR + 3 * 8)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
uint8_t info_mailbox;

void Demo_Application(void);
void Demo_StateTrans(unsigned short state);

extern uint8_t HardwareOpened;


TaskP_Handle pdiTask; // ECAT sw ISR
TaskP_Handle uartTask;           // UART processing
TaskP_Handle ledTaskHndl;        // LED Control Task
TaskP_Handle sync0Task;          // ECAT SYNC0 ISR


void Sync0task(uint32_t arg0, uint32_t arg1);
TaskP_Handle sync1Task; // ECAT SYNC1 ISR
void Sync1task(uint32_t arg0, uint32_t arg1);

uint32_t appState = 0;
pECAT_SLAVE_INTERFACE pSlaveInterface = NULL;

#define PD_INPUT_SIZE 4
#define PD_OUTPUT_SIZE 4

/****************************************************************************************
 *         ---------------   task1 ---------------
 ****************************************************************************************/


void task1(uint32_t arg0, uint32_t arg1)
{



    info_mailbox = 0;                      // Variable to delete task -> read to mailbox
    Bool bRunApplication = TRUE;
    TaskP_Params taskParams;

    UART_printf("\nVersion - ");UART_printf(IND_PKG_VERSION);

    if(isEtherCATDevice()) UART_printf("EtherCAT Device\n");
    else UART_printf("Non-EtherCAT Device\n");

    UART_printf("IPLeiria BIOS EtherCAT Demo application");
    UART_printf(APPL_BUILD_VER);
    UART_printf("\r\nBuild Timestamp      : %s %s \n", __DATE__, __TIME__);

    // init peripleral board
    bsp_soc_evm_init();
    UART_printf("bsp_soc_evm_init\n");

    /* map the array which contains pru firmware instrcutions.
           If the application is built to execute from internal RAM completely,then
           pru instructions are expcted to be stored in SPI flash and this mapping is
           not required

           -> frameProc Pointer to a buffer containing HRT PRU firmware (ecat_frame_handler_bin.h)
           -> hostProc Pointer to a buffer containing SRT PRU firmware (ecat_host_interface_bin.h)
    */

    bsp_set_pru_firmware((uint32_t *)FrameProc, sizeof(FrameProc),
                             (uint32_t *)HostProc, sizeof(HostProc));

    // define PDO -> configuration on Twincat -> Input and Output  -> ESI -> slave information xml file
    pSlaveInterface = Ecat_Open(PD_INPUT_SIZE, PD_OUTPUT_SIZE);

    // init satefy over ethercat
    // transfer of safety-critical control data through the same medium
    tiesc_foe_eoe_init();

    if(pSlaveInterface != NULL)
    {
        //assign application function pointer
        pSlaveInterface->pApplication = (void (*)(void))&Demo_Application;
        pSlaveInterface->pStateTrans = &Demo_StateTrans;

        /* Create tasks */
        /* Create tree tasks that share a resource*/
        // // -> The local host controller accesses the ESC DPRAM via the PDI (process data interface).
        TaskP_Params_init(&taskParams);
        taskParams.priority = 11;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        pdiTask = TaskP_create(PDItask, &taskParams);

        /* Create led Task */
        TaskP_Params_init(&taskParams);
        taskParams.priority = 11;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        ledTaskHndl = TaskP_create(LEDtask, &taskParams);

        // SYNC1 IRQ handler in the EtherCAT slave stack
        TaskP_Params_init(&taskParams);
        taskParams.priority = 13;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        sync0Task = TaskP_create(Sync0task, &taskParams);

        // SYNC1 IRQ handler in the EtherCAT slave stack
        TaskP_Params_init(&taskParams);
        taskParams.priority = 13;
        taskParams.stacksize = 1512*TIESC_TASK_STACK_SIZE_MUL;
        taskParams.arg0 = (void *)pruIcss1Handle;
        sync1Task = TaskP_create(Sync1task, &taskParams);

        do
        {

            Ecat_OnTimer();
            TaskP_yield();


            // verify mailbox to end Tasks
            Mailbox_pend(mailbox_Handle_Ethercat, &info_mailbox, BIOS_NO_WAIT);
            if(info_mailbox == 3){
                Board_setDigOutput(0);  // send info to led_board
                bRunApplication = FALSE;
                TaskP_delete(&pdiTask);
                TaskP_delete(&ledTaskHndl);
                TaskP_delete(&sync0Task);
                TaskP_delete(&sync1Task);
                Ecat_Close();
                UART_printf("    Terminate Ethercat protocol \n    ");

            }

            info_mailbox = 0;
        }while(bRunApplication == TRUE);

     }



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
/****************************************************************************************
 *
 ****************************************************************************************/

void PDItask(uint32_t arg1, uint32_t arg2)
{
    TaskP_sleep(10 * OS_TICKS_IN_MILLI_SEC);
    uint32_t evtOutNum = HOST_AL_EVENT - 20;

    while(1)
    {
        // arg1 -> pruIcss1Handle
        PRUICSS_pruWaitEvent((PRUICSS_Handle)arg1, evtOutNum);
        /* ISR processing */
        HW_EcatIsr();
    }
}

/****************************************************************************************
 *
 ****************************************************************************************/

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
           // UART_printf("bsp_get_eeprom_update_status\n");
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

/****************************************************************************************
 *         ---------------   task sysnc0task ---------------
 ****************************************************************************************/

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
        UART_printf("PRUICSS_pruWaitEvent\n");
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

/****************************************************************************************
 *         ---------------   task sysnc1task ---------------
 ****************************************************************************************/

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


/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////

void Demo_Application(void)
{
    /* Buffer containing the received data */
    uint8_t rxBuf[1U] = {0xFFU};

    static uint8_t prevState = 55;

    if(!pSlaveInterface)
    {
        return;
    }


    uint8_t LED = (*(pSlaveInterface->pOutput)) & 0xFF;
    Board_getDigInput(rxBuf);

    /*************/
    _read_word = LED;    // read word recebe word lida pelo slave
    /*************/


    uint32_t INPUT = (uint32_t)(*rxBuf & 0xFF);

    INPUT |=   0xaabbcc00;

    /*************/
    INPUT = _write_word;  // Input recebe word a escrecer  pelo slave
    /*************/


    *(pSlaveInterface->pInput) = INPUT;
    if(LED != prevState)          // se for difernete do estado anterior
    {
        Board_setDigOutput(LED);  // send info to led_board
    }

    prevState = LED;

}

void Demo_StateTrans(unsigned short state)
{

    uint32_t addr_len;

    /*
     * The state of the EtherCAT slave is controlled via the EtherCAT State Machine (ESM).
     * Depending upon the state, different functions are accessible or executable in the EtherCAT slave.
     * Specific commands must be sent by the EtherCAT master to the device in each state, particularly during the bootup of the slave.
     *
     *   A distinction is made between the following states:
     *
     *     -  Init
     *     -  Pre-Operational    ---->   //INIT_2_PREOP
     *     -  Safe-Operational and  ---> //PREOP_2_SAFEOP
     *     -  Operational        ----->  //SAFEOP_2_OP
     *     -  Boot              ------>  //OP_2_SAFEOP
     *
     * */

    switch(state)
    {
        //INIT_2_PREOP
        // During the transition between Init and Pre-Op the EtherCAT slave checks whether the mailbox was initialized correctly.
        // In Pre-Op state mailbox communication is possible, but not process data communication. The EtherCAT master initializes
        // (sync) / (FMMU) / PDO ....
        case 0x12:
            HW_EscReadDWord(addr_len, ESC_ADDR_SM0_PHYS_ADDR);
            bsp_set_sm_properties(pruIcss1Handle, MAILBOX_WRITE, (addr_len & 0xFFFF), (addr_len >> 16));

            HW_EscReadDWord(addr_len, ESC_ADDR_SM1_PHYS_ADDR);
            bsp_set_sm_properties(pruIcss1Handle, MAILBOX_READ, (addr_len & 0xFFFF), (addr_len >> 16));
            break;

        //PREOP_2_SAFEOP
        // During transition between Pre-Op and Safe-Op the EtherCAT slave checks whether the sync manager channels for process
         //data communication and, if required, the distributed clocks settings are correct
        case 0x24:
            HW_EscReadDWord(addr_len, ESC_ADDR_SM3_PHYS_ADDR);
            bsp_set_sm_properties(pruIcss1Handle, PROCESS_DATA_IN, (addr_len & 0xFFFF),
                                  (addr_len >> 16));
            break;

        //SAFEOP_2_OP
        case 0x48:
            HW_EscReadDWord(addr_len, ESC_ADDR_SM2_PHYS_ADDR);
            bsp_set_sm_properties(pruIcss1Handle, PROCESS_DATA_OUT, (addr_len & 0xFFFF),
                                  (addr_len >> 16));
            break;

        //OP_2_SAFEOP
        case 0x84:
            *(pSlaveInterface->pOutput) = 0;
            break;

        default:
            break;
    }

}

