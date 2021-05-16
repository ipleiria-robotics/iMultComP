

#include "app.h"

#include <stdio.h>
#include <string.h>
/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>



int main(){

    char _protocol_choose = 0;
    char buff[6];


    Board_init(BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK | BOARD_INIT_ICSS_PINMUX | BOARD_INIT_UART_STDIO |  BOARD_INIT_ICSS_ETH_PHY);
    UART_init();
    delay_us(1000);
    /* Scan input word from user */
    memset(buff,0,sizeof(buff));
    delay_us(1000);
    UART_printf("\nInsert pnr to run profinet RT \n");
    delay_us(1000);
    UART_printf("Insert eth to run ethercat \n");
    delay_us(1000);

    while(_protocol_choose == 0){
        UART_scanFmt("%s", &buff);
        if(!strcmp(buff,"pnr")) _protocol_choose = 1;   // profinet rt
        if(!strcmp(buff,"eth")) _protocol_choose = 2;   // ethercar
        memset(buff,0,sizeof(buff));
    }

    if(_protocol_choose == 1) _main_profinet_();
    if(_protocol_choose == 2) init_ethercat();


    OSAL_OS_start();


    return(0);

}


