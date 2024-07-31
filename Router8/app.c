/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "app_iostream_usart.h"
#include "zigbee.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_device.h"
#include "em_chip.h"
#include "uartdrv.h"
#include "app_iostream_usart.h"
#include "app.h"


void app_init(void)
{
    CHIP_Init();
    initCMU();
    initGPIO();
    initUSART0();
    GPIO_PinModeSet(LEDPORT,LEDPIN,gpioModePushPull,0);
    usart_init();





}


/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
