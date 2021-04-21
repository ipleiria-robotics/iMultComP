
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


/* ----------------- INCLUDE PROFINET ------------------------------------------*/

#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/stkmain.h>
#include <ti/osal/osal.h>

// ti header

#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_intc.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_pru_ctrl.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>
#include <ti/drv/pruss/soc/pruicss_v1.h>
#include <ti/drv/icss_emac/soc/icss_emacSoc.h>

//#include <ti/drv/icss_emac/src/icss_emacLoc.h>  // neste arquivo foi acrescentado #undef -> #include <ti/csl/src/ip/icss/V1/cslr_icss_iep.h>

#include <ti/drv/icss_emac/icss_emacFwInit.h>
#include <ti/drv/icss_emac/icss_emacLearning.h>
#include <ti/drv/icss_emac/icss_emacStatistics.h>
#include <ti/drv/icss_emac/icss_emacDrv_Def.h>
#include <ti/drv/icss_emac/icss_emacDrv.h>
#include <ti/drv/icss_emac/icss_emacStormControl.h>

#include <ti/transport/ndk/nimu_icss/example/src/osdrv_ndkdeviceconfig.h>
#include <ti/transport/ndk/nimu_icss/nimu_icssEth.h>
#include <examples/osal/TaskP.h>
#include <examples/osal/OSP.h>
#include <examples/board/include/board_gpioLed.h>
#include <examples/board/include/board_spi.h>
#include <examples/board/include/board_phy.h>
#include <examples/board/include/board_misc.h>
#include <examples/board/include/board_tlkphy.h>
#include <examples/board/include/board_i2cLed.h>

//Application header files

#include "pn_soc.h"
#include "firmware_version.h"
#include <protocols/snmp/include/snmp_ndk_interface.h>
#include <protocols/profinet_slave/version.h>
#include <protocols/profinet_slave/include/PN_Handle.h>
#include <protocols/profinet_slave/include/PN_HandleDef.h>
#include <protocols/profinet_slave/include/iPnOs.h>
#include <protocols/profinet_slave/include/iPNDrv.h>
#include <protocols/profinet_slave/include/iRtcDrv2.h>
#include <protocols/profinet_slave/include/iPNLegacy.h>
#include <protocols/profinet_slave/include/iPNIsoMDrv.h>
#include <third_party/protocols/profinet_slave/include/os.h>
#include <third_party/protocols/profinet_slave/include/types_pn.h>
#include <third_party/protocols/profinet_slave/include/system.h>
#include <platform_pn.h>
#include <examples/profinet_slave/tiswitch_pruss_intc_mapping.h>
#include <protocols/profinet_slave/firmware/icss_emacFwConfig.h>


// -------------------------------------------------------------------- END profinet ------------------------------------------------


/*             Global Variables                   */
Board_IDInfo boardInfo;
Mailbox_Handle mailbox_Handle_Ethercat;
Mailbox_Handle mailbox_Handle_profinet;

uint8_t _read_word;
uint32_t _write_word;
uint32_t addr_len_eth;



/*              Function declaretion               */
void init_peripheral();
void task_main();
void task_ethercat_main();

void task_main_profinet();
void init_peripheral_profinet();


/*  Params And handles     */
TaskP_Params taskParams;
TaskP_Handle tasK_m;

/* Params and handles ethercar */
TaskP_Params task_params_ethercat;
TaskP_Handle task_handle_ethercat;

/* Params and Handles profinet */
TaskP_Params task_params_profinet;
TaskP_Handle task_handle_profinet;









