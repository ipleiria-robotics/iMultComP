

#include "app.h"




/****************************************************************************************
 *               Function init peripheral
 ****************************************************************************************/

void init_peripheral(){

    Board_init(BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK | BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO);

    TaskP_Params_init(&taskParams);
    taskParams.priority = 15;
    taskParams.stacksize = 2048;
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

    char buff[6];
    uint16_t ch_task_init = 0;
    uint8_t task_ethercat_create = 0;

    bool task_profinet_create = 0;


    /* Create mailbox ethercat */
    mailbox_Handle_Ethercat = Mailbox_create(sizeof(uint8_t), 1,NULL, NULL);
    if(mailbox_Handle_Ethercat == NULL){
        UART_printf("mailbox_ethercat_fail ");
    }
    /* Create mailbox ethercat */
    mailbox_Handle_profinet = Mailbox_create(sizeof(uint8_t), 1,NULL, NULL);
    if(mailbox_Handle_profinet == NULL){
       UART_printf("mailbox_profinet_fail ");
    }

    //------------------------------------

    UART_printf("Board Am3359 Init \n");
    TaskP_sleep(100);
    memset(buff,0,sizeof(buff));




    while(1){

            /* Scan input word from user */
             UART_scanFmt("%s", &buff);

             if(!strcmp(buff,"exit")) ch_task_init = 1;
             if(!strcmp(buff,"cat")) ch_task_init = 2;
             if(!strcmp(buff,"ecat")) ch_task_init = 3;

             if(!strcmp(buff,"pnet")) ch_task_init = 5;
             if(!strcmp(buff,"epnet")) ch_task_init = 6;

             if(!strcmp(buff,"rdd")) ch_task_init = 30;
             if(!strcmp(buff,"wdd")){
                 ch_task_init = 31;
                 memset(buff,0,sizeof(buff));
             }

             if(!strcmp(buff,"pninit")) ch_task_init = 40;
             if(!strcmp(buff,"pnexit")) ch_task_init = 41;


    // ------------------------------------------------------------------------------
             switch(ch_task_init){
             case 1:   // delete app
                 UART_printf("Board Am3359 exit");
                 clear();                // clear display
                 OSAL_OS_exit(0);
                 TaskP_delete(&tasK_m);
                 break;
        // -----------------------------------------------------------------------------------------------------
             case 2:  // create task ethercat
                 if(!task_ethercat_create){
                     task_ethercat_create = 1;
                     // create params task
                     TaskP_Params_init(&task_params_ethercat);
                     task_params_ethercat.priority = 10;
                     task_params_ethercat.stacksize = 2048*TIESC_TASK_STACK_SIZE_MUL;
                     // task1 create -> function implement in tiescutils.c
                     task_handle_ethercat = TaskP_create(task1, &task_params_ethercat);
                 }
                 break;
         // -----------------------------------------------------------------------------------------------------
             case 3:  // exit task ethercat
                 task_ethercat_create = 0;
                 uint8_t del_aux = 3;        // 3 -> variable to delete task inside task_ethercat -> send to mailbox
                 Mailbox_post(mailbox_Handle_Ethercat,&del_aux, BIOS_NO_WAIT);
                 TaskP_sleep(60);
                 clear();                // clear display
                 //TaskP_delete(&task_handle_ethercat);
                 UART_printf("    taskP_delete ethercat \n    ");
                 setline(0);
                 setOrientation(1);
                 printstr((int8_t *)"Industry Comunication");
                 setline(1);
                 printstr((int8_t *)"   IPLeiria   ");
                 del_aux = 0;
                 break;
        // -----------------------------------------------------------------------------------------------------
             case 4:

             break;
        // -----------------------------------------------------------------------------------------------------
             case 5:

             break;
      // -----------------------------------------------------------------------------------------------------
            case 30:
                // read info ethercat
                UART_printf("%d\n",_read_word);
                _read_word = 0;   // clear
                UART_printf("%d\n",addr_len_eth);
                addr_len_eth = 0;   // clear
            break;

        // -----------------------------------------------------------------------------------------------------
           case 31:
               // write info ethercat
               UART_scanFmt("%d", &_write_word);
               UART_printf("ttt: %d\n",_write_word);

           break;


        // -----------------------------------------------------------------------------------------------------
           case 40:
              if(!task_profinet_create){
                 task_profinet_create = 1;
                 // create params task
                 TaskP_Params_init(&task_params_profinet);
                 task_params_profinet.priority = 10;
                 task_params_profinet.stacksize = 3000;
                 task_handle_profinet = TaskP_create(task_main_profinet, &task_params_profinet);
              }


           break;

           // -----------------------------------------------------------------------------------------------------
           case 41:
               task_profinet_create = 0;
               uint8_t del_aux_2 = 3;        // 3 -> variable to delete task inside task_ethercat -> send to mailbox
               Mailbox_post(mailbox_Handle_profinet,&del_aux_2, BIOS_NO_WAIT);
               TaskP_sleep(10);
               clear();                // clear display
              // TaskP_delete(&task_handle_profinet);
               setline(0);
               setOrientation(1);
               printstr((int8_t *)"Industry Comunication");
               setline(1);
               printstr((int8_t *)"   IPLeiria   ");
               del_aux_2 = 0;
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




