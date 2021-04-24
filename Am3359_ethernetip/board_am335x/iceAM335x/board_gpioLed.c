/**
 *  \file   board_gpioLed.c
 *
 *  \brief:
 *  GPIO LED configurations
 *
 *
 */

/* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#include <ti/csl/cslr.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/starterware/include/hw/soc_am335x.h>

#include <examples/board/include/board_gpioLed.h>

/***********************************************************************/
/* Macros                                        */
/***********************************************************************/

/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] =
{
    /* Output pin : AM335X_ICE_RED0_LED */
    GPIO_DEVICE_CONFIG(0 + 1, 17) |
    GPIO_CFG_OUTPUT ,

    /* Output pin :AM335X_ICE_GRN0_LED */
    GPIO_DEVICE_CONFIG(0 + 1, 16) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM335X_ICE_YEL0_LED */
    GPIO_DEVICE_CONFIG(3 + 1, 9) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM335X_ICE_RED1_LED */
    GPIO_DEVICE_CONFIG(1 + 1, 30) |
    GPIO_CFG_OUTPUT ,

    /* Output pin : AM335X_ICE_GRN1_LED */
    GPIO_DEVICE_CONFIG(0 + 1, 20) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM335X_ICE_YEL1_LED */
    GPIO_DEVICE_CONFIG(0 + 1, 19) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM335X_ICE_HVS_PIN OR AMIC11x_RUN_LED*/
    GPIO_DEVICE_CONFIG(3 + 1, 18) |
    GPIO_CFG_OUTPUT,

#ifdef iceAMIC11x
    /* Output pin : AMIC11x ERROR_LED */
    GPIO_DEVICE_CONFIG(3 + 1, 20) |
    GPIO_CFG_OUTPUT,
#endif

#ifdef TIESC_SPI_SLAVE_MODE
    /* Output pin : AMIC110_PDI_ISR */
    GPIO_DEVICE_CONFIG(3 + 1, 16) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AMIC110_FIRMWARE_LOADED */
    GPIO_DEVICE_CONFIG(3 + 1, 17) |
    GPIO_CFG_OUTPUT,
#endif

};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] =
{
    NULL
};

/* GPIO Driver configuration structure */
GPIO_v1_Config GPIO_v1_config =
{
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    0x2U,
};

void gpioLedPinmuxConfig()
{
}

void  Board_setTriColorLED(uint32_t gpioLeds, uint8_t value)
{
    if(gpioLeds & BOARD_TRICOLOR0_RED)
    {
        GPIO_write(0, value);
    }

    if(gpioLeds & BOARD_TRICOLOR0_GREEN)
    {
        GPIO_write(1, value);
    }

    if(gpioLeds & BOARD_TRICOLOR0_YELLOW)
    {
        GPIO_write(2, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_RED)
    {
        GPIO_write(3, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_GREEN)
    {
        GPIO_write(4, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_YELLOW)
    {
        GPIO_write(5, value);
    }

#ifdef iceAMIC11x

    if(gpioLeds & BOARD_AMIC_RUN_LED)
    {
        GPIO_write(6, value);
    }

    if(gpioLeds & BOARD_AMIC_ERROR_LED)
    {
        GPIO_write(7, value);
    }

#endif
}
