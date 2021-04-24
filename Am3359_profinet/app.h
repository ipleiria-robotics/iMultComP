/*
 * app.h
 *
 *  Created on: 24/04/2021
 *      Author: varod
 */

#ifndef APP_H_
#define APP_H_

/* Mailbox header file */
#include <ti/sysbios/knl/Mailbox.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/stkmain.h>





/* Mailbox header file */
#include <ti/sysbios/knl/Mailbox.h>

/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_intc.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_pru_ctrl.h>
#include <ti/csl/src/ip/icss/V1/cslr_icss_cfg.h>

#include <ti/drv/pruss/soc/pruicss_v1.h>
#include <ti/drv/icss_emac/soc/icss_emacSoc.h>

#include <ti/drv/icss_emac/src/icss_emacLoc.h>
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
#include <examples/board/include/board_oled.h>
#include <examples/board/include/oled_drv.h>


/**Application header files*/


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

#include "pn_soc.h"
#include <examples/board/include/board_spi.h>
#include <protocols/profinet_slave/include/PN_Handle.h>
#include <protocols/profinet_slave/include/PN_HandleDef.h>
#include <third_party/protocols/profinet_slave/include/os.h>
#include <protocols/profinet_slave/include/iPNLegacy.h>



#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_v1.h>
#include <ti/csl/src/ip/gpio/V1/gpio_v2.h>
#include <ti/drv/spi/SPI.h>
#include <ti/board/board.h>
#include <ti/starterware/include/prcm.h>



#include <examples/board/include/oled_drv.h>
#include <examples/board/include/board_tlkphy.h>
#include <examples/board/include/board_i2cLed.h>
#include <examples/board/include/board_phy.h>
#include "pnDrvConfig.h"
#include "firmware_version.h"
#include <ti/drv/pruss/soc/pruicss_v1.h>

void init_peripheral();
void aux_main();
void task_main();

TaskP_Handle ledblink_profinet;
TaskP_Handle taskpruss_profinet;
TaskP_Handle appOutData_profinet;
TaskP_Handle EmacOsRxTask_profinet;

Mailbox_Handle mailbox_Handle_profinet;


/*  Params And handles     */
TaskP_Params taskParams;
TaskP_Handle tasK_m;

/* Params and Handles profinet */
TaskP_Params task_params_profinet;
TaskP_Handle task_handle_profinet;

#endif /* APP_H_ */
