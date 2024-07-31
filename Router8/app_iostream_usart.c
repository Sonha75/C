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
 *  EFR32xG24 UART0 SIM7680 LTE push and pull data from Firebase server using AT command via HTTP protocol
 * Reference: https://www.silabs.com/documents
 *            https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/usart/async_polled
 *            https://github.com/TDLOGY/TDM2309-A7680C-4G-Replace-SIM800L
 *            https://linhkienthuduc.com/nang-cap-tu-sim800-2g-len-sim7600-a76xx-4g/
 *            https://www.micropeta.com/video58
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "em_chip.h"
#include "sl_iostream.h"
#include "sl_iostream_init_instances.h"
#include "sl_iostream_handles.h"
#include "app_iostream_usart.h"
#include "em_gpio.h"
#include "zigbee.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "zigbee_app_framework_event.h"
#include "app.h"



#include "em_device.h"



#include "uartdrv.h"
//#include "ustimer.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
#define REPORT_TIME_DEFAULT      (5)
#define USART0_TX_PORT gpioPortC

#define USART0_TX_PIN 2

#define USART0_RX_PORT gpioPortC

#define USART0_RX_PIN 3
#define BUFLEN  50

uint8_t buffer[BUFLEN];
uint32_t inpos = 0;
uint32_t period,report_time;
sensor_data_t sensor_data;
//bool receive = true;
static sl_zigbee_event_t usart_event;
/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

/* Input buffer */
//static char buffer[BUFLEN];

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Initialize example.
 ******************************************************************************/
void usart_init(void){
  period=REPORT_TIME_DEFAULT;
  report_time=period*5000;
  sl_zigbee_event_init(&usart_event,app_iostream_usart_action);




      sl_zigbee_event_set_active(&usart_event);
      //sl_zigbee_event_set_delay_ms(&usart_event, 10000);




}









void initCMU()

{

  // Enable clock for GPIO module

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Enable clock for USART0 module

  CMU_ClockEnable(cmuClock_USART0, true);



}



void initGPIO()

{

  // Configure GPIO pins

  GPIO_PinModeSet(USART0_TX_PORT, USART0_TX_PIN, gpioModePushPull, 1);

  GPIO_PinModeSet(USART0_RX_PORT, USART0_RX_PIN, gpioModeInput, 0);

}



void initUSART0()

{

  // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)

  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;



  // Route USART0 TX and RX to the defined pins

  GPIO->USARTROUTE[0].TXROUTE = (USART0_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)

            | (USART0_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

  GPIO->USARTROUTE[0].RXROUTE = (USART0_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)

            | (USART0_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);



  // Enable RX and TX signals now that they have been routed

  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;



  // Configure and enable USART0

  USART_InitAsync(USART0, &init);

  USART_InitAsync(USART0, &init);
  NVIC_ClearPendingIRQ(USART0_RX_IRQn);
    NVIC_EnableIRQ(USART0_RX_IRQn);
    NVIC_ClearPendingIRQ(USART0_TX_IRQn);
    NVIC_EnableIRQ(USART0_TX_IRQn);
}
//static int k=0;
//void func_fix(uint8_t chuoi[8][9])
//{
//if(k==0){
//    memcpy(&sensor_data.index,chuoi[k]+0,sizeof(chuoi[k]));
////    sensor_data.index=data;
//    send_message_test();
//}
//else if(k==1){
//    memcpy(&sensor_data.temperature,chuoi[k]+0,sizeof(chuoi[k]));
//}
//else if(k==2){
//    memcpy(&sensor_data.distance,chuoi[k]+0,sizeof(chuoi[k]));
//}
//else if(k==3){
//    memcpy(&sensor_data.longitude,chuoi[k]+0,sizeof(chuoi[k]));
//}
//else if(k==4){
//    memcpy(&sensor_data.latitude,chuoi[k]+0,sizeof(chuoi[k]));
//}
//else if(k==5){
//    memcpy(&sensor_data.humidity,chuoi[k]+0,sizeof(chuoi[k]));
//}
//else if(k==6){
//    memcpy(&sensor_data.pressure,chuoi[k]+0,sizeof(chuoi[k]));
//    send_message_test();
//}
//
//}
/***************************************************************************//**
 * Example ticking function.
 ******************************************************************************/
void USART0_RX_IRQHandler(void)
{
  // Get the character just received
  buffer[inpos] = USART0->RXDATA;

  // Exit loop on new line or buffer full
  if ((buffer[inpos] != '&') && (inpos < BUFLEN))
    inpos++;
  else{
      uint8_t chuoi[8][9];
      //tach chuoi
      uint32_t max = inpos;
      inpos = 0;
      int m = 0, n = 0;
      memset(chuoi,0,72);
      while((inpos <= max) && (m <= 7))
        {
          while(buffer[inpos] != ' ' && inpos <= max)
            {
              chuoi[m][n]=buffer[inpos];
              USART_Tx(USART0, chuoi[m][n]);
              ++n;
              ++inpos;
            }
          USART_Tx(USART0, '\n');
          ++m;
          n=0;
          ++inpos;
        }

      //USART_Tx(USART0, 'b');
      float array_1d[8];
      for(int i = 0; i < 8; i++) {
             array_1d[i] = atof((char*)chuoi[i]);
         }




      //int index=atoi(chuoi[0]);
     // USART_Tx(USART0, 'c');
      sensor_data.index=02;
      sensor_data.temperature=array_1d[1]*100;
      sensor_data.pressure=array_1d[2]*100;
      sensor_data.humidity=array_1d[3]*100;
      sensor_data.longitude=array_1d[4]*100;
      sensor_data.latitude=array_1d[5]*100;
      sensor_data.distance=array_1d[6]*100;
      memcpy(sensor_data.node_name,"node2",6);
      send_message_test();

      inpos = 0;
  }
    //receive = false;   // Stop receiving on CR




}
void app_iostream_usart_action(sl_zigbee_event_t *event)
{

  GPIO_PinOutToggle(LEDPORT,LEDPIN);


printf("bat dau doc");

send_message_test();
  // Zero out buffer
  uint8_t buffer[BUFLEN];

   uint32_t i;
      for (i = BUFLEN; i > 0; --i)   //memset cho ban tin trong
        buffer[i] = 0;
      USART_IntEnable(USART0, USART_IEN_RXDATAV);


      // Receive BUFLEN characters unless a new line is received first


 printf("mm");

//uint32 j;
//      for (j = 0; j < i; j++)
//
//              USART_Tx(USART0, (uint8_t*)buffer[j]);




GPIO_PinOutToggle(LEDPORT,LEDPIN);
//sl_zigbee_event_set_inactive(&usart_event);

sl_zigbee_event_set_delay_ms(&usart_event,report_time);

}




