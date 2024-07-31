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

#include "zigbee.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_device.h"
#include "em_chip.h"
#include "app.h"
#include "usart_sim.h"
#include "error-def.h"
#include "app/framework/include/af.h"
#define NETWORK_PAN_ID 0x2222
#define NETWORK_CHANNEL 20
#define NETWORK_TX_POWER 11
void configureNetworkParameters(EmberNetworkParameters* networkParams) {
  networkParams->panId = NETWORK_PAN_ID;
  networkParams->radioChannel = NETWORK_CHANNEL;
  networkParams->radioTxPower = NETWORK_TX_POWER;
  networkParams->joinMethod = EMBER_USE_MAC_ASSOCIATION; // Sử dụng MAC Association để tham gia mạng
}
void startNetwork(void) {
  EmberStatus status;
  EmberNetworkParameters networkParams;

  configureNetworkParameters(&networkParams);

  status = emberAfFormNetwork(&networkParams);
  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Mạng đã được khởi tạo thành công");
  } else {
    emberAfCorePrintln("Khởi tạo mạng thất bại: 0x%x", status);
  }
}
void app_init(void)
{
  startNetwork();
   printf("start");
  //app_iostream_eusart_init();
  initUART();
//  initSIM();



}
extern int flag;

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{


}
