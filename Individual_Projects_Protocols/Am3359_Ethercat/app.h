
/* STDIO */
#include <stdbool.h>
#include <stdint.h>


/*  INCLUDE BIOS/osl */
#include <osal/OSP.h>
#include <osal/TaskP.h>
#include <ti/sysbios/BIOS.h>


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




/*             Global Variables                   */
Board_IDInfo boardInfo;
Mailbox_Handle mailbox_Handle_Ethercat;


uint8_t _read_word;
uint32_t _write_word;
uint32_t addr_len_eth;



/*              Function declaretion               */
void init_peripheral();
void task_main();
void task_ethercat_main();





/*  Params And handles     */
TaskP_Params taskParams;
TaskP_Handle tasK_m;

/* Params and handles ethercar */
TaskP_Params task_params_ethercat;
TaskP_Handle task_handle_ethercat;










