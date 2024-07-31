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
#include "em_timer.h"




#include "em_device.h"



#include "uartdrv.h"
//#include "ustimer.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define USART0_TX_PORT gpioPortC

#define USART0_TX_PIN 2

#define USART0_RX_PORT gpioPortC

#define USART0_RX_PIN 3
#define BUFLEN  50



sensor_data_t sensor_data;

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


void initCMU()

{

  // Enable clock for GPIO module

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Enable clock for USART0 module

  CMU_ClockEnable(cmuClock_USART0, true);



}


void timer_led(){
  while (1) {

      GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 1);

      for(volatile long i=0; i<100000; i++)
      ;

      GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0);

      for(volatile long i=0; i<100000; i++)
      ;

      }
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
void app_iostream_usart_action(void)
{

//  uint8_t c = 0;
  //static uint8_t index = 0;
//
// sensor_data.index=05;
// send_message_test();
////  /* Retrieve characters, print local echo and full line back */
//  //c = getchar();
//  c=EUSART_Rx(EUSART1);
// while(c != '@');
// while(c != '&'){
//     //c=getchar();
//     c=EUSART_Rx(EUSART1);
//     buffer[index]=c;
//     index++;
//
// }
//
// for (int j = 0; j < 20; j++)
//      EUSART_Tx(EUSART1, buffer[j]);

// char * p;
//  p = strtok(buffer, " ");
//  //putchar("hello");
//  putchar(p);
//  int id = atoi(p);
//  //putchar(c);
//  func_fix(id);
//  //printf("id = %d\n", id);
//  while(p != NULL)
//    {
//
//         p = strtok(NULL, " ");
//
//          float data = atof(p);
//          i++;
//          func_fix(data);
//    //printf(" data = %f\n", data);
//    //putchar('\n');
//    //putchar(p);
//     }
//
// buffer[index]='\0';
//i=0;
//  if (c > 0) {
//
//    if (c == '\r' || c == '\n') {
//      //putchar('\r');
//      //putchar('\n');
//      buffer[index] = '\0';
//      //printf("\r\nYou wrote: %s\r\n> ", buffer);
//      index = 0;
//    } else {
//      if (index < BUFSIZE - 1) {
//        buffer[index] = c;
//        index++;
//      }
//      /* Local echo */
//      //putchar(c);
//    }

  // Zero out buffer
  uint8_t buffer[BUFLEN];

     uint32_t i;
        for (i = BUFLEN; i > 0; --i)   //memset cho ban tin trong
          buffer[i] = 0;



      // Receive BUFLEN characters unless a new line is received first

      do
      {

        // Wait for a character

        buffer[i] = USART_Rx(USART0);



        // Exit loop on new line

        if (buffer[i] != '\r')

          i++;

        else

          break;

      }while (i < BUFLEN);



//      for (j = 0; j < i; j++)
//
//              USART_Tx(USART0, (uint8_t*)buffer[j]);



      USART_Tx(USART0, 'a');
uint8_t chuoi[8][9];
//int max=i;
//int m = 0;
//while(m<=max)
//  {
//    for(int j=0;j<8;j++){
//    int k=0;
//    while(buffer[m] != ' ')
//      {
//        USART_Tx(USART0, 'e');
//       chuoi[j][k]=buffer[m];
//       ++m;
//       ++k;
//    }
//    m++;
//    }
//}

uint32_t max = i;
i = 0;
int m = 0, n = 0;
memset(chuoi,0,72);
while((i <= max) && (m <= 7))
  {
    while(buffer[i] != ' ' && i <= max)
      {
        chuoi[m][n]=buffer[i];
        USART_Tx(USART0, chuoi[m][n]);
        ++n;
        ++i;
      }
    USART_Tx(USART0, '\n');
    ++m;
    n=0;
    ++i;
  }
//USART_Tx(USART0, 'b');
float array_1d[8];
for(int i = 0; i < 8; i++) {
       array_1d[i] = atof((char*)chuoi[i]);
   }




//int index=atoi(chuoi[0]);
USART_Tx(USART0, 'c');
sensor_data.index=array_1d[0];
sensor_data.temperature=array_1d[1];
sensor_data.pressure=array_1d[2];
sensor_data.humidity=array_1d[3];
sensor_data.longitude=array_1d[4];
sensor_data.latitude=array_1d[5];
sensor_data.distance=array_1d[6];
memcpy(sensor_data.node_name,"node2",6);
send_message_test();
USART_Tx(USART0, 'd');

//func_fix(chuoi);

      // Output received characters

//      for (j = 0; j < i; j++)
//uint32_t i,j
//        USART_Tx(USART0, buffer[j]);
      //func_fix(i);
//      char * p;
//                  p = strtok(buffer, " ");
//                  for(int i=0;i<3;i++){
//      USART_Tx(USART0, p[i]);
//                  }
////                  putchar(p);
////
//        int id = atoi(p);
//        USART_Tx(USART0, '\n');
//        func_fix(id);
////
//        while(p != NULL)
//          {
//
//               p = strtok(NULL, " ");
//               for(int i=0;i<5;i++){
//                    USART_Tx(USART0, p[i]);
//                                }
//               USART_Tx(USART0, '\n');
//                float data = atof(p);
//                i++;
//                func_fix(data);




}




