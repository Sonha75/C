/***************************************************************************//**
 * @file
 * @brief iostream usart examples functions
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

#ifndef APP_IOSTREAM_USART_H
#define APP_IOSTREAM_USART_H
#include "stdint.h"
#include "zigbee_app_framework_event.h"
/***************************************************************************//**
 * Initialize iostream usart
 ******************************************************************************/
void app_iostream_usart_init(void);

/***************************************************************************//**
 * iostream usart ticking function
 ******************************************************************************/
void app_iostream_usart_action(sl_zigbee_event_t *event);

void func_fix(float data);
void initCMU(void);
void initGPIO(void);
void initUSART0(void);
void app_iostream_usart_init(void);
void usart_init(void);
typedef  struct {
    uint8_t node_name[6];
    float temperature;
    float pressure; // can nang
    float humidity;
    float distance; //do cao
    float longitude ; //kinh do
    float latitude;   //vi do
    int index;

    //uint8_t feedback;
}sensor_data_t;
#endif  // APP_IOSTREAM_USART_H
