
#include "app.h"




/* Mailbox header file */
#include <ti/sysbios/knl/Mailbox.h>

/*  INCLUDE Board */
#include <Include/Board_am335x/board_misc.h>
#include <Include/Board_am335x/board_i2cLed.h>
#include <ti/board/board.h>
#include <Include/Board_am335x/board_gpioLed.h>
#include <Include/Board_am335x/board_misc.h>
#include <Include/Board_am335x/board_spi.h>
#include <Include/Board_am335x/board_i2cLed.h>
#include <Include/Board_am335x/soc_icss_header.h>
#include <Include/Board_am335x/oled_drv.h>
#include <Include/Board_am335x/board_oled.h>

/*  INCLUDE TI */
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>



/*  INCLUDE ETHERCAT */
#include <Ethercat/Include_ethercat/tiescutils.h>
#include <Include/Board_am335x/board_misc.h>
#include <Ethercat/Include_ethercat/tieschw.h>
//#include <Ethercat/Lib/EtherCATSampleLibrary.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>
#include <Ethercat/Include_ethercat/tiescmiscappl.h>
#include <Ethercat/Include_ethercat/tiesc_fw_cir_buff.h>

/*  INCLUDE ETHERCAT */
#include <Ethercat/Include_ethercat/tiescutils.h>
#include <Include/Board_am335x/board_misc.h>
#include <Ethercat/Include_ethercat/tieschw.h>
//#include <Ethercat/Lib/EtherCATSampleLibrary.h>
#include <Ethercat/Include_ethercat/tiesc_soc.h>
#include <Ethercat/Include_ethercat/tiescmiscappl.h>
#include <Ethercat/Include_ethercat/tiesc_fw_cir_buff.h>

/*             Global Variables                   */
Board_IDInfo boardInfo;
Mailbox_Handle mailbox_Handle_Ethercat;


/*              Function declaretion               */
void task_main();
void task_ethercat_main();

void info_comand();


/*  Params And handles     */
TaskP_Params taskParams;
TaskP_Handle tasK_m;

/* Params and handles ethercar */
TaskP_Params task_params_ethercat;
TaskP_Handle task_handle_ethercat;



/****************************************************************************************
 *               Function init peripheral
 ****************************************************************************************/

void init_ethercat(){


    TaskP_Params_init(&taskParams);
    taskParams.priority = 15;
    taskParams.stacksize = 10000;
    tasK_m = TaskP_create(task_main, &taskParams);

    // get ID info
    Board_getIDInfo(&boardInfo);
    UART_printf("\nBoard name \t: ");
    UART_printf(boardInfo.boardName);
    UART_printf("\n\rBoard Revision \t: ");
    UART_printf(boardInfo.version);

    UART_printf("\r\nBuild Timestamp      : %s %s", __DATE__, __TIME__);

    GPIO_init();
    Board_i2cLedInit();
    SPI_init();

    Board_oledInit();
    clear();
    setline(0);
    setOrientation(1);
    printstr((int8_t *)"Industry Comunication");
    setline(2);
    printstr((int8_t *)"   IPLeiria   ");
    scrollDisplayRight();


}

/****************************************************************************************
 *               Function init peripheral
 ****************************************************************************************/
void task_main(){

    info_comand();
    // create Task ethercat
     // create params task
     TaskP_Params_init(&task_params_ethercat);
     task_params_ethercat.priority = 10;
     task_params_ethercat.stacksize = 2048*TIESC_TASK_STACK_SIZE_MUL;
    // task1 create -> function implement in tiescutils.c
      task_handle_ethercat = TaskP_create(task1, &task_params_ethercat);


    char buff[6];
    uint16_t ch_task_init = 0;



    /* Create mailbox ethercat */
    mailbox_Handle_Ethercat = Mailbox_create(sizeof(uint8_t), 1,NULL, NULL);
    if(mailbox_Handle_Ethercat == NULL){
        UART_printf("mailbox_ethercat_fail ");
    }

    //------------------------------------
    UART_printf("Board Am3359 Init \n");
    TaskP_sleep(100);
    memset(buff,0,sizeof(buff));

    while(1){

            TaskP_sleep(500);
            info_comand();

            /* Scan input word from user */
             UART_scanFmt("%s", &buff);
             if(!strcmp(buff,"ethread")) ch_task_init = 30;
             if(!strcmp(buff,"ethwrite")){
                 ch_task_init = 31;
                 memset(buff,0,sizeof(buff));
             }
      // ------------------------------------------------------------------------------
             switch(ch_task_init){
      // -----------------------------------------------------------------------------------------------------
            case 30:
                // read info ethercat
                UART_printf("%d\n",_read_word);
                _read_word = 0;   // clear
            break;
     // -----------------------------------------------------------------------------------------------------
           case 31:
               // write info ethercat
               UART_scanFmt("%d", &_write_word);
           break;

     // -----------------------------------------------------------------------------------------------------
             default:

                break;
       }
// ------------------------------------------------------------------------------
         memset(buff,0,sizeof(buff));
         ch_task_init = 0;
    }
}


void info_comand(){
    UART_printf("\nInsert ethread to read the PLC \n");
    UART_printf("Insert ethwrite to write to the PLC\n");
}





