/***************************************************************************//**
 * @file app.c
 * @brief Callbacks implementation and application specific code.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "app/framework/include/af.h"
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif

#ifdef SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT
#include "network_test_config.h"
#endif //SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT

#if (LARGE_NETWORK_TESTING == 0)
#ifndef EZSP_HOST

#include "zigbee_sleep_config.h"
#endif
#include "app/framework/include/af.h"
#include "usart_sim.h"
#include "network-creator.h"
#include "network-creator-security.h"

#include "find-and-bind-target.h"
#include "zll-commissioning.h"
#include "usart_sim.h"
#include "zigbee.h"
#if defined(SL_CATALOG_LED0_PRESENT)
#include "sl_led.h"
#include "sl_simple_led_instances.h"
#define led_turn_on(led) sl_led_turn_on(led)
#define led_turn_off(led) sl_led_turn_off(led)
#define led_toggle(led) sl_led_toggle(led)
#define COMMISSIONING_STATUS_LED (&sl_led_led0)
#define ON_OFF_LIGHT_LED         (&sl_led_led0)
#else // !SL_CATALOG_LED0_PRESENT
#define led_turn_on(led)
#define led_turn_off(led)
#define led_toggle(led)
#endif // SL_CATALOG_LED0_PRESENT

#define LED_BLINK_PERIOD_MS      2000
#define LIGHT_ENDPOINT           1
#define BUTTON1                  (1)





static sl_zigbee_event_t commissioning_led_event;
static sl_zigbee_event_t finding_and_binding_event;

//---------------
// Event handlers


sensor_data_t receive_msg;


//
//void copy_to_struct(uint8_t message_uni[7], sensor_data_t *sensor_data) {
//    // Clear the message array
//    //memset(message_uni, 0, sizeof(uint8_t) * 7 * 7);
//    //printf("%02x",message_uni);
//    // Copy temperature
//    memcpy(&message_uni[0], &sensor_data->temperature, sizeof(sensor_data->temperature));
//    memcpy(&sensor_data->temperature, &message_uni, sizeof(sensor_data->temperature));
//
//    // Copy node name
//    memcpy(&message_uni[1], &sensor_data->node_name, sizeof(sensor_data->node_name));
//
//    // Copy pressure
//    message_uni[2][0] = sensor_data->pressure;
//
//    // Copy humidity
//    memcpy(&message_uni[3], &sensor_data->humidity, sizeof(sensor_data->humidity));
//
//    // Copy distance
//    memcpy(&message_uni[4], &sensor_data->distance, sizeof(sensor_data->distance));
//
//    // Copy eCO2
//    message_uni[5][0] = sensor_data->eCO2;
//
//    // Copy index
//    memcpy(&message_uni[6], &sensor_data->index, sizeof(sensor_data->index));
//}
//transfer từ mảng vào struct
static void parse_received_data(EmberMessageBuffer message, sensor_data_t *parsed_data) {

  uint16_t messageLen = emberMessageBufferLength(message);
  printf("len=%d",messageLen);
  uint8_t mang[EMBER_AF_MAXIMUM_INTERPAN_LENGTH];
  emberCopyFromLinkedBuffers(message,
                              0,
                              mang,
                              messageLen);

   if(mang[0] == 01){
       memcpy(&parsed_data->temperature,mang+1, messageLen-1);

   }else if(mang[0] == 02){
         memcpy(&parsed_data->node_name,mang+1, messageLen-1);

     }else if(mang[0] == 03){
         memcpy(&parsed_data->pressure,mang+1, messageLen-1);

     }else if(mang[0] == 04){
         memcpy(&parsed_data->humidity,mang+1, messageLen-1);

     }else if(mang[0] == 05){
         memcpy(&parsed_data->longitude,mang+1, messageLen-1);

     }
     else if(mang[0] == 06){
         memcpy(&parsed_data->latitude,mang+1, messageLen-1);
     }
     else if(mang[0] == 07){
         memcpy(&parsed_data->index,mang+1, messageLen-1);

     }else if(mang[0] == 8){
         memcpy(&parsed_data->distance,mang+1, messageLen-1);
                }
    printf("\n");
//    for(uint16_t i=0;i<messageLen;i++){
//    printf("%02x ",mang[i]);
//}

//   printf("\n");
//   printf("Temp - %f \r\n", (float)(parsed_data->temperature));
//   printf("Humi - %f \r\n", (float)parsed_data->humidity);
//   printf("eCO2 - %d \r\n", (int)parsed_data->eCO2);
//   printf("Node - %s \r\n", (char*)parsed_data->node_name);
//   printf("distance - %f \r\n", (float)parsed_data->distance);
//   printf("pressure - %f \r\n", (float)parsed_data->pressure);

}





static void commissioning_led_event_handler(sl_zigbee_event_t *event)
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    uint16_t identifyTime;
    emberAfReadServerAttribute(LIGHT_ENDPOINT,
                               ZCL_IDENTIFY_CLUSTER_ID,
                               ZCL_IDENTIFY_TIME_ATTRIBUTE_ID,
                               (uint8_t *)&identifyTime,
                               sizeof(identifyTime));
    if (identifyTime > 0) {
      led_toggle(COMMISSIONING_STATUS_LED);
      sl_zigbee_event_set_delay_ms(&commissioning_led_event,
                                   LED_BLINK_PERIOD_MS << 1);
    } else {
      led_turn_on(COMMISSIONING_STATUS_LED);
    }
  } else {

  }
}

static void finding_and_binding_event_handler(sl_zigbee_event_t *event)
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    sl_zigbee_event_set_inactive(&finding_and_binding_event);

    sl_zigbee_app_debug_println("Find and bind target start: 0x%02X",
                                emberAfPluginFindAndBindTargetStart(LIGHT_ENDPOINT));
  }
}

//----------------------
// Implemented Callbacks

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action. The framework
 * will always process the stack status after the callback returns.
 */
void emberAfStackStatusCallback(EmberStatus status)
{
  // Note, the ZLL state is automatically updated by the stack and the plugin.
  if (status == EMBER_NETWORK_DOWN) {
    led_turn_off(COMMISSIONING_STATUS_LED);
  } else if (status == EMBER_NETWORK_UP) {
    led_turn_on(COMMISSIONING_STATUS_LED);
    sl_zigbee_event_set_active(&finding_and_binding_event);
  }
}

/** @brief Init
 * Application init function
 */



//khởi tạo khi reset device
void emberAfMainInitCallback(void)
{
  EmberStatus status;



  sl_zigbee_event_init(&commissioning_led_event, commissioning_led_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event, finding_and_binding_event_handler);

  sl_zigbee_event_set_active(&commissioning_led_event);

  EmberMulticastTableEntry* tableEntry=&(sl_zigbee_get_multicast_table()[0]);
  tableEntry->multicastId=0xFF11;
  tableEntry->endpoint=1;

}

/** @brief Complete network steering.
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt.
 *
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID.
 *
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network.
 *
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete.
 */
//void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
//                                                  uint8_t totalBeacons,
//                                                  uint8_t joinAttempts,
//                                                  uint8_t finalState)
//{
//  sl_zigbee_app_debug_println("Join network complete: 0x%02X", status);
//
//  if (status != EMBER_SUCCESS) {
//    // Initialize our ZLL security now so that we are ready to be a touchlink
//    // target at any point.
//    status = emberAfZllSetInitialSecurityState();
//    if (status != EMBER_SUCCESS) {
//      sl_zigbee_app_debug_println("Error: cannot initialize ZLL security: 0x%02X", status);
//    }
//
//    status = emberAfPluginNetworkCreatorStart(false); // distributed
//    sl_zigbee_app_debug_println("Form network start: 0x%02X", status);
//  }
//}

/** @brief Complete the network creation process.
 *
 * This callback notifies the user that the network creation process has
 * completed successfully.
 *
 * @param network The network that the network creator plugin successfully
 * formed.
 *
 * @param usedSecondaryChannels Whether or not the network creator wants to
 * form a network on the secondary channels.
 */
void emberAfPluginNetworkCreatorCompleteCallback(const EmberNetworkParameters *network,
                                                 bool usedSecondaryChannels)
{
  sl_zigbee_app_debug_println("Form Network Complete: 0x%02X",
                              EMBER_SUCCESS);
}

/** @brief Post Attribute Change
 *
 * This function is called by the application framework after it changes an
 * attribute value. The value passed into this callback is the value to which
 * the attribute was set by the framework.
 */
void emberAfPostAttributeChangeCallback(uint8_t endpoint,
                                        EmberAfClusterId clusterId,
                                        EmberAfAttributeId attributeId,
                                        uint8_t mask,
                                        uint16_t manufacturerCode,
                                        uint8_t type,
                                        uint8_t size,
                                        uint8_t* value)
{
  if (clusterId == ZCL_ON_OFF_CLUSTER_ID
      && attributeId == ZCL_ON_OFF_ATTRIBUTE_ID
      && mask == CLUSTER_MASK_SERVER) {
    bool onOff;
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff))
        == EMBER_ZCL_STATUS_SUCCESS) {
      if (onOff) {
        led_turn_on(ON_OFF_LIGHT_LED);
      } else {
        led_turn_off(ON_OFF_LIGHT_LED);
      }
    }
  }
}


// ham xử lí khi có bản tin đến
void emberAfIncomingMessageCallback (EmberIncomingMessageType type, EmberApsFrame *apsFrame, EmberMessageBuffer message)
{

  emberAfCorePrintln("\nMessage profileID_source - 0x%02X \r\n", apsFrame->profileId);
//  emberAfCorePrintln("Message groupID - 0x%02X \r\n", apsFrame->groupId);
//  emberAfCorePrintln("Message des_enpoint - 0x%02X \r\n", apsFrame->destinationEndpoint);
//  emberAfCorePrintln("Message source_enpoint - 0x%02X \r\n", apsFrame->sourceEndpoint);
  static long long flag=0;

  if(type==EMBER_INCOMING_UNICAST){
      flag++;
      uint16_t messageLen =emberMessageBufferLength(message);
      //emberAfCorePrintln("unicast receive data");
      uint8_t messageContents[EMBER_AF_MAXIMUM_INTERPAN_LENGTH];
      emberCopyFromLinkedBuffers(message,
                                 0,
                                 messageContents,
                                 messageLen);
      //emberAfCorePrintln("\n%d\n",messageLen);
      for (uint16_t i = 0; i < messageLen; i++)
              {
                     emberAfCorePrint("%02x ", messageContents[i]);
                    // emberAfCorePrint("Addr: %02x ", &messageContents[i]);


               }
     // emberAfCorePrint("\nAddr1:  %02x \n", messageContents+1);
//      uint8_t reply[10]="da nhan ";
//      EmberStatus state=emberSendReply(apsFrame->clusterId,reply);

      parse_received_data(message,&receive_msg);


      if(messageContents[0]==07 ){


          printf("\n");

             printf("Temp - %.2f \r\n", receive_msg.temperature);
             printf("Humi - %.2f \r\n", receive_msg.humidity);
             printf("Longitude - %.2f \r\n", receive_msg.longitude);
             printf("Latitude - %.2f \r\n", receive_msg.latitude);
             printf("Node - %s \r\n", receive_msg.node_name);
             printf("distance - %.2f \r\n", receive_msg.distance);
             printf("pressure - %.2f \r\n", receive_msg.pressure);
             printf("index - %d \r\n", receive_msg.index);
             printf("Flag=%lld\n",flag);
      }

      //sim and usart
if(flag==20)    {
       initSIM();
       flag=0;
}

  }

  if(type==EMBER_INCOMING_MULTICAST){
      uint16_t messageLen =emberMessageBufferLength(message);
       if (messageLen > EMBER_AF_MAXIMUM_INTERPAN_LENGTH) {
               emberAfCorePrintln("Error: Message too long");
               return;
           }
     // int row=7;
     // int col=7;
       uint8_t messageContents[EMBER_AF_MAXIMUM_INTERPAN_LENGTH];


       emberCopyFromLinkedBuffers(message,
                                  0,
                                  messageContents,
                                  messageLen);
  if(apsFrame->groupId == 0xFF11){
      emberAfCorePrintln("multicast received");
      for (uint16_t i = 0; i < messageLen; i++)
        {
               emberAfCorePrint("%02x ", messageContents[i]);
         }
  }
  }
}

/** @brief On/off Cluster Server Post Init
 *
 * Following resolution of the On/Off state at startup for this endpoint, perform any
 * additional initialization needed; e.g., synchronize hardware state.
 *
 * @param endpoint Endpoint that is being initialized
 */
void emberAfPluginOnOffClusterServerPostInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.
  emberAfPostAttributeChangeCallback(endpoint,
                                     ZCL_ON_OFF_CLUSTER_ID,
                                     ZCL_ON_OFF_ATTRIBUTE_ID,
                                     CLUSTER_MASK_SERVER,
                                     0,
                                     0,
                                     0,
                                     NULL);
}

/** @brief
 *
 * Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  #ifndef EZSP_HOST
  sl_mac_calibrate_current_channel();
  #endif
}

#if defined(SL_CATALOG_SIMPLE_BUTTON_PRESENT) && (SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0)
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
#include "force-sleep-wakeup.h"
#endif //SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
/***************************************************************************//**
 * A callback called in interrupt context whenever a button changes its state.
 *
 * @remark Can be implemented by the application if required. This function
 * can contain the functionality to be executed in response to changes of state
 * in each of the buttons, or callbacks to appropriate functionality.
 *
 * @note The button state should not be updated in this function, it is updated
 * by specific button driver prior to arriving here
 *
   @param[out] handle             Pointer to button instance
 ******************************************************************************/
void openNetwork(uint8_t duration) {
    EmberStatus status;
    status = emberAfPermitJoin(duration, true); // Mở mạng trong `duration` giây

    if (status == EMBER_SUCCESS) {
        emberAfCorePrintln("Mạng đã được mở cho các thiết bị mới tham gia");
    } else {
        emberAfCorePrintln("Mở mạng thất bại: 0x%x", status);
    }
}
void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle) {
      if ( sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {


          openNetwork(60);

      }
  }
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
    sl_zigbee_event_set_active(&finding_and_binding_event);
    #ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
    sl_zigbee_app_framework_force_wakeup();
    #endif //SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
  }
}
#endif // SL_CATALOG_SIMPLE_BUTTON_PRESENT && SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0

//Internal testing stuff
#if defined(EMBER_TEST)
void emberAfHalButtonIsrCallback(uint8_t button, uint8_t state)
{
  if (state == BUTTON_RELEASED) {
    sl_zigbee_event_set_active(&finding_and_binding_event);
  }
}
#endif // EMBER_TEST

#ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
void sli_zigbee_app_framework_force_sleep_callback(void)
{
  // Do other things like turn off LEDs etc
  sl_led_turn_off(&sl_led_led0);
}
#endif // SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
#endif //#if (LARGE_NETWORK_TESTING == 0)
