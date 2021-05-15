/*
 * app.h
 *
 *  Created on: 12/05/2021
 *      Author: valentim
 */

#ifndef APP_C_
#define APP_C_


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/ndk/inc/stkmain.h>

/* TI-RTOS Header files */
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>

#include <ti/drv/i2c/I2C.h>
#include <ti/drv/spi/SPI.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/spi/soc/QSPI_v1.h>
#include <ti/drv/spi/test/qspi_flash/src/Flash_S25FL/S25FL.h>

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

#include <osal/TaskP.h>
#include <osal/OSP.h>

#include <board_am335x/include/board_gpioLed.h>
#include <board_am335x/include/board_spi.h>
#include <board_am335x/include/board_phy.h>
#include <board_am335x/include/board_misc.h>
#include <board_am335x/include/board_tlkphy.h>
#include <board_am335x/include/board_i2cLed.h>
#include <board_am335x/include/soc_icss_header.h>
#include <board_am335x/include/oled_drv.h>
#include <board_am335x/include/board_oled.h>





/**
 * @brief funct main profinet
 */
void _main_profinet_();








#endif /* APP_C_ */
