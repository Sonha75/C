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
#include "network-steering.h"
#include "zll-commissioning.h"
#include "find-and-bind-initiator.h"
#include <string.h>
#include "app_iostream_usart.h"
#if defined(SL_CATALOG_LED0_PRESENT)
#include "sl_led.h"
#include "sl_simple_led_instances.h"
#include "zigbee.h"
#define led_turn_on(led) sl_led_turn_on(led)
#define led_turn_off(led) sl_led_turn_off(led)
#define led_toggle(led) sl_led_toggle(led)
#define COMMISSIONING_STATUS_LED     (&sl_led_led0)
#else // !SL_CATALOG_LED0_PRESENT
#define led_turn_on(led)
#define led_turn_off(led)
#define led_toggle(led)
#endif // SL_CATALOG_LED0_PRESENT



#define LED_BLINK_PERIOD_MS          2000
#define TRANSITION_TIME_DS           20
#define FINDING_AND_BINDING_DELAY_MS 3000
#define BUTTON0                      0
#define BUTTON1                      1
#define SWITCH_ENDPOINT              1

static bool commissioning = false;
static uint8_t lastButton;

static sl_zigbee_event_t commissioning_event;
static sl_zigbee_event_t led_event;
static sl_zigbee_event_t finding_and_binding_event;
sl_zigbee_event_t  send_message_event;
sl_zigbee_event_t  send_message_event1;
//---------------

extern sensor_data_t sensor_data;
//typedef  struct {
//    uint8_t node_name[6];
//    float temperature;
//    float pressure;
//    float humidity;
//    float distance;
//    float eCO2;
//    int index;
//
//    //uint8_t feedback;
//}sensor_data_t;


void print_bytes(const uint8_t* start, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", start[i]);
    }
    printf("\n");
}
//void copy_sensor_data_to_message(const sensor_data_t* sensor_data, uint8_t* message) {
//    uint8_t* ptr = message;
//
//    // Copy temperature (4 bytes)
//    memcpy(ptr, &sensor_data->temperature, sizeof(sensor_data->temperature));
//    printf("Temperature: ");
//    print_bytes(ptr, sizeof(sensor_data->temperature));
//    ptr += sizeof(sensor_data->temperature);
//
//    // Copy node_name (20 bytes)
//    memcpy(ptr, &sensor_data->node_name, sizeof(sensor_data->node_name));
//    printf("Node Name: ");
//    print_bytes(ptr, sizeof(sensor_data->node_name));
//    ptr += sizeof(sensor_data->node_name);
//
//    // Copy pressure (4 bytes)
//    memcpy(ptr, &sensor_data->pressure, sizeof(sensor_data->pressure));
//    printf("Pressure: ");
//    print_bytes(ptr, sizeof(sensor_data->pressure));
//    ptr += sizeof(sensor_data->pressure);
//
//    // Copy humidity (4 bytes)
//    memcpy(ptr, &sensor_data->humidity, sizeof(sensor_data->humidity));
//    printf("Humidity: ");
//    print_bytes(ptr, sizeof(sensor_data->humidity));
//    ptr += sizeof(sensor_data->humidity);
//
//    // Copy eCO2 (4 bytes)
//    memcpy(ptr, &sensor_data->eCO2, sizeof(sensor_data->eCO2));
//    printf("eCO2: ");
//    print_bytes(ptr, sizeof(sensor_data->eCO2));
//    ptr += sizeof(sensor_data->eCO2);
//
//    // Copy index (4 bytes)
//    memcpy(ptr, &sensor_data->index, sizeof(sensor_data->index));
//    printf("Index: ");
//    print_bytes(ptr, sizeof(sensor_data->index));
//    ptr += sizeof(sensor_data->index);
//
//    // Copy distance (4 bytes)
//    memcpy(ptr, &sensor_data->distance, sizeof(sensor_data->distance));
//    printf("Distance: ");
//    print_bytes(ptr, sizeof(sensor_data->distance));
//    ptr += sizeof(sensor_data->distance);
//}

void copy_to_messaget(uint8_t **message_uni, sensor_data_t *sensor_data) {

    // Copy temperature
   // message_uni[0][0]=01;
   *(*(message_uni+0)+0)=01;
    memcpy(*(message_uni+0)+1, &sensor_data->temperature, sizeof(sensor_data->temperature));

    // Copy node name
   // message_uni[1][0]=02;
    *(*(message_uni+1)+0)=02;
    memcpy(*(message_uni+1)+1, &sensor_data->node_name, sizeof(sensor_data->node_name));

    // Copy pressure
    *(*(message_uni+2)+0)=03;
    memcpy(*(message_uni+2)+1, &sensor_data->pressure, sizeof(sensor_data->pressure));
    // Copy humidity
    *(*(message_uni+3)+0)=04;
    memcpy(*(message_uni+3)+1, &sensor_data->humidity, sizeof(sensor_data->humidity));

    // Copy kinh do
    *(*(message_uni+4)+0)=05;
    memcpy(*(message_uni+4)+1, &sensor_data->longitude, sizeof(sensor_data->longitude));

    // Copy vi do
       *(*(message_uni+5)+0)=06;
       memcpy(*(message_uni+5)+1, &sensor_data->latitude, sizeof(sensor_data->latitude));

    // Copy index
    *(*(message_uni+6)+0)=07;
    memcpy(*(message_uni+6)+1, &sensor_data->index, sizeof(sensor_data->index));


    // Copy distance
      *(*(message_uni+7)+0)=8;
      memcpy( *(message_uni+7)+1, &sensor_data->distance, sizeof(sensor_data->distance));
}

//void copy_to_message(uint8_t message_uni[7][7], sensor_data_t *sensor_data) {
//    // Clear the message array
//    memset(message_uni, 0, sizeof(uint8_t) * 7 * 7);
//
//    // Copy temperature
//    message_uni[0][0]=01;
//    memcpy(message_uni[0]+1, &sensor_data->temperature, sizeof(sensor_data->temperature));
//
//    // Copy node name
//    message_uni[1][0]=02;
//    memcpy(message_uni[1]+1, &sensor_data->node_name, sizeof(sensor_data->node_name));
//
//    // Copy pressure
//    message_uni[2][0]=03;
//    message_uni[2][1] = sensor_data->pressure;
//
//    // Copy humidity
//    message_uni[3][0]=04;
//    memcpy(&message_uni[3]+1, &sensor_data->humidity, sizeof(sensor_data->humidity));
//
//    // Copy eCO2
//    message_uni[4][0]=05;
//    message_uni[4][1] = sensor_data->eCO2;
//
//    // Copy index
//    message_uni[5][0]=06;
//    message_uni[5][1] = sensor_data->index;
//
//    // Copy distance
//    message_uni[6][0]=07;
//    memcpy(message_uni[6]+1, &sensor_data->distance, sizeof(sensor_data->distance));
//}
//handler-> chu dong goi
//callback->bi dong khi event xay ra

void send_message_test(){

 EmberApsFrame apsFrame;
static int num=0;
 EmberStatus status;

 uint8_t ** message_uni;

 message_uni = (uint8_t **)malloc(8 * sizeof(uint8_t *));

     // Cấp phát bộ nhớ cho các cột

         message_uni[0] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[1] = (uint8_t *)malloc(6 * sizeof(uint8_t));
         message_uni[2] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[3] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[4] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[5] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[6] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[7] = (uint8_t *)malloc(5 * sizeof(uint8_t));
        int kich_thuoc[8]={5,6,5,5,5,5,5,5};

  extern sensor_data_t sensor_data;

//      sensor_data.temperature=31;
//      sensor_data.node_name = "node1";
//      sensor_data.pressure =11;
//      sensor_data.humidity=11.4;
//      sensor_data.longitude=22;
//      sensor_data.latitude=22;
//      sensor_data.index=1;
//      sensor_data.distance=10.6;
//
//      sensor_data_t sensor_data={
//          .node_name = "node1",
//          .pressure =11,
//          .humidity=11.4,
//          .longitude=22,
//          .latitude=22,
//          .index=1,
//          .distance=10.6
//      };


 copy_to_messaget(message_uni,&sensor_data);



 apsFrame.clusterId = 0x0003;

 apsFrame.destinationEndpoint =0;




 apsFrame.options = EMBER_APS_OPTION_NONE;



 apsFrame.sequence = 0;

 apsFrame.sourceEndpoint = emberAfPrimaryEndpoint();

for(int i=0;i<8;i++){


status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, 0x0000, &apsFrame, kich_thuoc[i], *(message_uni + i) ); //gia tri con tro chi den o nho dau tien->dia chi o nho dau tien

//bool state=emberAfMessageSentCallback(EMBER_OUTGOING_DIRECT,0x0000,&apsFrame,kich_thuoc[i],*(message_uni + i),status);
//if(state==true){
//    printf("true");
//}
//else{
//    printf("false");
//}
}
num++;
printf("\nDem:%d\n",num);
//sl_zigbee_event_set_inactive(&send_message_event);
sl_zigbee_event_set_delay_ms(&send_message_event, 5000);
if(num==5){
    sl_zigbee_event_set_inactive(&send_message_event);
}

}
// Event handlers
//void send_message(){
//
// EmberApsFrame apsFrame;
//
// EmberStatus status;
//
// uint8_t message_uni[7][7]={0};
// uint8_t message[50]={0};
// sensor_data_t sensor_data={
//    .temperature=31,
//    .node_name = "node3",
//    .pressure =0,
//    .humidity=22.4,
//    .longitude=11,
//    .index=3,
//    .distance=31.6,
//};
//
//
// copy_to_message(message_uni,&sensor_data);
//
// int messagelength = sizeof(message);
// printf("\n%d\n",messagelength);
//
//
// printf("Message array contents:\n");
//     for (int i = 0; i < 7; i++) {
//         printf("message[%d]: ", i);
//         for (int j = 0; j < 7; j++) {
//             printf("%02x ", message_uni[i][j]);
//             printf("\t");
//             printf("%p ", &message_uni[i][j]);
//         }
//         printf("\n");
//     }
//
//
// apsFrame.clusterId = 0x0003;
//
// apsFrame.destinationEndpoint =0x0000;
//
//
//
// apsFrame.options = EMBER_APS_OPTION_NONE;
//
//
//
// apsFrame.sequence = 0;
//
// apsFrame.sourceEndpoint = emberAfPrimaryEndpoint();
//
// //status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, 0x0000, &apsFrame, 7, *(message_uni + 0) );
// //sl_zigbee_app_debug_println("\nMessage sourceID_endpoint unicast - 0x%02X \r\n", apsFrame.sourceEndpoint);
//
// //sl_zigbee_app_debug_println("Message desID_enpoint unicast- 0x%02X \r\n", apsFrame.destinationEndpoint);
//for(int i=0;i<7;i++){
//
//
//status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, 0x0000, &apsFrame, 7, *(message_uni + i) ); //truyen tung dong tu dia chi dau cua dong
//
//
//}
//
//
//
//// sl_zigbee_app_debug_print("%02x ", (uint8_t*)message);
// sl_zigbee_app_debug_println("\nstatus: 0x%02X",status);
//
// sl_zigbee_event_set_delay_ms(&send_message_event, 3000);
//
//
//}
//
//void send_multicast_message(){
//
//
// EmberApsFrame apsFrame;
//
// EmberStatus status;
//
// uint8_t message[] = {66,33,22,11,34, '.' }; //so nay la decimal in ra hexa ra 42 21 16 0b 22
//
// int messagelength = sizeof(message);
// sl_zigbee_app_debug_println("%d",messagelength);
// sli_legacy_buffer_manager_allocate_buffer(sizeof(message));
//
//
// apsFrame.clusterId = 0x0000;
//
// apsFrame.destinationEndpoint = emberAfPrimaryEndpoint();
//
// apsFrame.groupId = 0xFF11;
//
// apsFrame.options = EMBER_APS_OPTION_NONE;
//
//
//
// apsFrame.sequence = 0;
//
// apsFrame.sourceEndpoint = 2;
//
// sl_zigbee_app_debug_println("Message sourceId_enpoint_multicast - 0x%02X \r\n", apsFrame.sourceEndpoint);
//
// status =   emberAfSendMulticast(0xFF11,&apsFrame,sizeof(message),message+1); //tham so message la con tro tro vao vi tri bat dau truyen
// //sl_zigbee_app_debug_println("%02x \n",message); //in ra address cua message[0] => data=0x42<=>66
//for(uint8_t i=0;i<sizeof(message);i++){
// sl_zigbee_app_debug_println("%02x",message[i]);
// //status =   emberAfSendMulticast(0xFF11,&apsFrame,sizeof(message),message);
//}
// sl_zigbee_event_set_delay_ms(&send_message_event1, 4000);
//
//
//}
bool emberAfMessageSentCallback(EmberOutgoingMessageType type,
                                int16u indexOrDestination,
                                EmberApsFrame* apsFrame,
                                int16u msgLen,
                                int8u* message,
                                EmberStatus status){
  return true;


}


static void commissioning_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status;

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    emberAfGetCommandApsFrame()->sourceEndpoint = SWITCH_ENDPOINT;
    if (lastButton == BUTTON0) {


        //sl_zigbee_event_set_inactive(&send_message_event1);
        //sl_zigbee_event_set_active(&send_message_event);

//        send_message();
        //send_multicast_message();
    } else if (lastButton == BUTTON1) {
        //sl_zigbee_event_set_inactive(&send_message_event);

        //send_multicast_message();
     uint8_t nextLevel = (uint8_t)(0xFF & emberGetPseudoRandomNumber());
      emberAfFillCommandLevelControlClusterMoveToLevel(nextLevel, TRANSITION_TIME_DS, 0, 0);
    }
    status = emberAfSendCommandUnicastToBindings();
    sl_zigbee_app_debug_println("%s: 0x%02X", "Send to bindings", status);
  } else {
    bool touchlink = (lastButton == BUTTON1);
    status = (touchlink
              ? emberAfZllInitiateTouchLink()
              : emberAfPluginNetworkSteeringStart());
    sl_zigbee_app_debug_println("%s network %s: 0x%02X",
                                (touchlink ? "Touchlink" : "Join"),
                                "start",
                                status);

    sl_zigbee_event_set_active(&led_event);

    commissioning = true;
  }
}





static void led_event_handler(sl_zigbee_event_t *event)
{
  if (commissioning) {
    if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
      led_toggle(COMMISSIONING_STATUS_LED);
      sl_zigbee_event_set_delay_ms(&led_event, LED_BLINK_PERIOD_MS << 1);
    } else {
      led_turn_on(COMMISSIONING_STATUS_LED);
    }
  } else if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    led_turn_on(COMMISSIONING_STATUS_LED);
  }
}

static void finding_and_binding_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status = emberAfPluginFindAndBindInitiatorStart(SWITCH_ENDPOINT);
  sl_zigbee_app_debug_println("Find and bind initiator %s: 0x%02X", "start", status);
}

//----------------------
// Implemented Callbacks
void emberAfMainInitCallback(void) //device reset thi nhay vao day
{
  sl_zigbee_event_init(&commissioning_event, commissioning_event_handler);
  sl_zigbee_event_init(&led_event, led_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event, finding_and_binding_event_handler);

  sl_zigbee_event_init(&send_message_event, send_message_test);
  //sl_zigbee_event_init(&send_message_event1, send_multicast_message);

    EmberMulticastTableEntry* tableEntry=&(sl_zigbee_get_multicast_table()[0]);
    tableEntry->multicastId=0xFF11;
    tableEntry->endpoint=1;
}

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action. The framework
 * will always process the stack status after the callback returns.
 */
void emberAfStackStatusCallback(EmberStatus status)
{
  if (status == EMBER_NETWORK_DOWN) {
    led_turn_off(COMMISSIONING_STATUS_LED);
  } else if (status == EMBER_NETWORK_UP) {
    led_turn_on(COMMISSIONING_STATUS_LED);
    //sl_zigbee_event_set_active(&send_message_event);
  //  sl_zigbee_event_set_active(&send_message_event1);
  }
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
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X", "Join", "complete", status);

  if (status != EMBER_SUCCESS) {
    commissioning = false;
  } else {
    sl_zigbee_event_set_delay_ms(&finding_and_binding_event,
                                 FINDING_AND_BINDING_DELAY_MS);
  }
}
//void emberAfIncomingMessageCallback (EmberIncomingMessageType type, EmberApsFrame *apsFrame, EmberMessageBuffer message)
//{
//  uint16_t flatPacket[128];
//  uint16_t messageLen = emberMessageBufferLength(message);
//
//  emberCopyFromLinkedBuffers(message,
//                             0,
//                             flatPacket,
//                             messageLen);
//
//  emberAfCorePrintln("Message profileID_source - 0x%02X \r\n", apsFrame->profileId);
//  emberAfCorePrintln("Message groupID - 0x%02X \r\n", apsFrame->groupId);
//  emberAfCorePrintln("Message des_enpoint - 0x%02X \r\n", apsFrame->destinationEndpoint);
//  emberAfCorePrintln("Message source_enpoint - 0x%02X \r\n", apsFrame->sourceEndpoint);
//
//  if(type==EMBER_INCOMING_UNICAST){
//  emberAfCorePrintln("Message unicast - %s \r\n", flatPacket);
//  }
//  if(type==EMBER_INCOMING_MULTICAST){
//  if(apsFrame->groupId == 0xFF02){
//      emberAfCorePrintln("Message multicast - %s \r\n", (char*)flatPacket);
//    }
//  }
//}
/** @brief Touch Link Complete
 *
 * This function is called by the ZLL Commissioning Common plugin when touch linking
 * completes.
 *
 * @param networkInfo The ZigBee and ZLL-specific information about the network
 * and target. Ver.: always
 * @param deviceInformationRecordCount The number of sub-device information
 * records for the target. Ver.: always
 * @param deviceInformationRecordList The list of sub-device information
 * records for the target. Ver.: always
 */
void emberAfPluginZllCommissioningCommonTouchLinkCompleteCallback(const EmberZllNetwork *networkInfo,
                                                                  uint8_t deviceInformationRecordCount,
                                                                  const EmberZllDeviceInfoRecord *deviceInformationRecordList)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X",
                              "Touchlink",
                              "complete",
                              EMBER_SUCCESS);

  sl_zigbee_event_set_delay_ms(&finding_and_binding_event,
                               FINDING_AND_BINDING_DELAY_MS);
}

/** @brief Touch Link Failed
 *
 * This function is called by the ZLL Commissioning Client plugin if touch linking
 * fails.
 *
 * @param status The reason the touch link failed. Ver.: always
 */
void emberAfPluginZllCommissioningClientTouchLinkFailedCallback(EmberAfZllCommissioningStatus status)
{
  sl_zigbee_app_debug_println("%s network %s: 0x%02X",
                              "Touchlink",
                              "complete",
                              EMBER_ERR_FATAL);

  commissioning = false;
}

/** @brief Find and Bind Complete
 *
 * This callback is fired by the initiator when the Find and Bind process is
 * complete.
 *
 * @param status Status code describing the completion of the find and bind
 * process Ver.: always
 */
void emberAfPluginFindAndBindInitiatorCompleteCallback(EmberStatus status)
{
  sl_zigbee_app_debug_println("Find and bind initiator %s: 0x%02X", "complete", status);

  commissioning = false;
}

#ifndef SL_CATALOG_ZIGBEE_EZSP_PRESENT
/** @brief
 *
 * Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  sl_mac_calibrate_current_channel();
}
#endif //SL_CATALOG_ZIGBEE_EZSP_PRESENT

#if defined(SL_CATALOG_SIMPLE_BUTTON_PRESENT) && (SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0)
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"

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
void sl_button_on_change(const sl_button_t *handle)
{
  if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON0) == handle) {
    if ( sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      lastButton = BUTTON0;
//     sl_zigbee_event_set_active(&commissioning_event);
//      sl_zigbee_event_set_active(&send_message_event1);



      //send_multicast_message();
   //sl_zigbee_event_set_active(&send_message_event);
       //      set_message();
        //      send_message(&sensor_data);



    }
  } else if (SL_SIMPLE_BUTTON_INSTANCE(BUTTON1) == handle) {
    if ( sl_button_get_state(handle) == SL_SIMPLE_BUTTON_RELEASED) {
      lastButton = BUTTON1;


      printf("flag=0");
      sl_zigbee_event_set_active(&commissioning_event);
//      sl_zigbee_event_set_inactive(&send_message_event1);
//      sl_zigbee_event_set_inactive(&send_message_event);
    }
  }
}
#endif // SL_CATALOG_SIMPLE_BUTTON_PRESENT && SL_ZIGBEE_APP_FRAMEWORK_USE_BUTTON_TO_STAY_AWAKE == 0

//Internal testing stuff
#if defined(EMBER_TEST)
void emberAfHalButtonIsrCallback(uint8_t button,
                                 uint8_t state)
{
  if (state == BUTTON_RELEASED) {
    lastButton = button;
    sl_zigbee_event_set_active(&commissioning_event);
  }
}
#endif // EMBER_TEST
