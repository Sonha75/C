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

#include "find-and-bind-initiator.h"
#include "sl_simple_button.h"
#include "sl_simple_button_instances.h"
#include "custom-token.h"
#include "app_rht.h"

#include "sl_i2cspm_inst0_config.h"
#include "sl_si70xx.h"
#include "sl_i2cspm_instances.h"
#include "sl_udelay.h"
#include "app/framework/plugin/find-and-bind-initiator/find-and-bind-initiator.h"
#if defined(SL_CATALOG_LED0_PRESENT)
#include "sl_led.h"
#include "sl_simple_led_instances.h"
#define led_turn_on(led) sl_led_turn_on(led)
#define led_turn_off(led) sl_led_turn_off(led)
#define led_toggle(led) sl_led_toggle(led)
#define COMMISSIONING_STATUS_LED     (&sl_led_led0)
#else // !SL_CATALOG_LED0_PRESENT
#define led_turn_on(led)
#define led_turn_off(led)
#define led_toggle(led)
#endif // SL_CATALOG_LED0_PRESENT


#if SL_I2CSPM_INST0_SPEED_MODE == 0
#define SL_I2CSPM_INST0_HLR i2cClockHLRStandard
#define SL_I2CSPM_INST0_MAX_FREQ I2C_FREQ_STANDARD_MAX
#elif SL_I2CSPM_INST0_SPEED_MODE == 1
#define SL_I2CSPM_INST0_HLR i2cClockHLRAsymetric
#define SL_I2CSPM_INST0_MAX_FREQ I2C_FREQ_FAST_MAX
#elif SL_I2CSPM_INST0_SPEED_MODE == 2
#define SL_I2CSPM_INST0_HLR i2cClockHLRFast
#define SL_I2CSPM_INST0_MAX_FREQ I2C_FREQ_FASTPLUS_MAX
#endif


#define TRANSITION_TIME_DS           20
#define FINDING_AND_BINDING_DELAY_MS 3000
#define INTER_MEASUREMENT_DELAY_MS 10000 //Time between measurements (ms)
#define LED_BLINK_PERIOD_MS          2000

//#define SWITCH_ENDPOINT              1
#define RHT_MEASUREMENT_ENDPOINT    1

bool reporting=true;
uint8_t onOff;
uint32_t reporting_time;
uint32_t appHumData = 0;
int32_t appTempData = 0;
volatile bool rht_ongoing_delay = false;
static bool commissioning = false; // Holds the commissioning status
static bool binding = false;


//static sl_zigbee_event_t si7021_event;
static sl_zigbee_event_t led_event;
static sl_zigbee_event_t finding_and_binding_event;
static sl_zigbee_event_t network_steering_event_control;
static sl_zigbee_event_t leave_network_event_control;
static sl_zigbee_event_t attribute_report_event_control;
sl_zigbee_event_t  send_message_event;



#define BUTTON0                  (0)
#define FACTORY_RESET_BUTTON     (1)
#define REPORT_TIME_DEFAULT      (5) //in seconds

//---------------
// Event handlers

static void network_steering_event_handler(sl_zigbee_event_t *event);
static void finding_and_binding_event_handler(sl_zigbee_event_t *event);
static void leave_network_event_handler(sl_zigbee_event_t *event);
static void attribute_report_event_handler(sl_zigbee_event_t *event);
static void led_event_handler(sl_zigbee_event_t *event);
static uint8_t binding_table_unicast_binding_count(void);
static void binding_table_print(void);

uint8_t bindings = 0;

//-------------------------------------------------------------------------------
//Struct
typedef  struct {
    uint8_t node_name[6];
    int temperature;
    int mass; // can nang
    int humidity;
    int height; //do cao
//    float longitude ; //kinh do
//    float latitude;   //vi do
    int id;
    int battery;
    int smell;
    //uint8_t feedback;
}sensor_data_t;

sensor_data_t sensor_data={
           .node_name = "node2",
           .mass =0,

           .id=2,
           .height=0,
           .battery=0,
           .smell=0,

       };

//Function
void copy_to_messaget(uint8_t **message_uni, sensor_data_t *sensor_data) {

    // Copy temperature
   // message_uni[0][0]=01;
   *(*(message_uni+0)+0)=00;
    memcpy(*(message_uni+0)+1, &sensor_data->id, sizeof(sensor_data->id));

    // Copy node name
   // message_uni[1][0]=02;
    *(*(message_uni+1)+0)=01;
    memcpy(*(message_uni+1)+1, &sensor_data->temperature, sizeof(sensor_data->temperature));

    // Copy pressure
    *(*(message_uni+2)+0)=02;
    memcpy(*(message_uni+2)+1, &sensor_data->humidity, sizeof(sensor_data->humidity));
    // Copy humidity
    *(*(message_uni+3)+0)=03;
    memcpy(*(message_uni+3)+1, &sensor_data->mass, sizeof(sensor_data->mass));

    // Copy kinh do
    *(*(message_uni+4)+0)=04;
    memcpy(*(message_uni+4)+1, &sensor_data->height, sizeof(sensor_data->height));

    // Copy vi do
    *(*(message_uni+5)+0)=05;
    memcpy(*(message_uni+5)+1, &sensor_data->smell, sizeof(sensor_data->smell));

    // Copy index
    *(*(message_uni+6)+0)=06;
    memcpy(*(message_uni+6)+1, &sensor_data->battery, sizeof(sensor_data->battery));

    // Copy distance
    *(*(message_uni+7)+0)=07;
    memcpy( *(message_uni+7)+1, &sensor_data->node_name, sizeof(sensor_data->node_name));

}

void send_message_test(){
 sl_udelay_wait(1000000);
 EmberApsFrame apsFrame;
static int num=0;
 EmberStatus status;

 uint8_t ** message_uni;

 message_uni = (uint8_t **)malloc(8 * sizeof(uint8_t *));

     // Cấp phát bộ nhớ cho các cột

         message_uni[0] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[1] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[2] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[3] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[4] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[5] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[6] = (uint8_t *)malloc(5 * sizeof(uint8_t));
         message_uni[7] = (uint8_t *)malloc(6 * sizeof(uint8_t));

        int kich_thuoc[8]={5,5,5,5,5,5,5,6};

  //sensor_data_t sensor_data;
//       sensor_data_t sensor_data={
//           .node_name = "node1",
//           .mass =22,
//
//           .id=2,
//           .height=10.6,
//           .battery=30.3,
//           .smell=50,
//
//       };
//       sensor_data.temperature=
//       sensor_data.humidity=
 copy_to_messaget(message_uni,&sensor_data);

 apsFrame.clusterId = 0x0003;
 apsFrame.destinationEndpoint =1;
 apsFrame.options = EMBER_APS_OPTION_NONE;
 apsFrame.sequence = 0;
 apsFrame.sourceEndpoint = emberAfPrimaryEndpoint();

for(int i=0;i<8;i++){

status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, 0x0000, &apsFrame, kich_thuoc[i], *(message_uni + i) ); //gia tri con tro chi den o nho dau tien->dia chi o nho dau tien

}
num++;
printf("\nDem:%d\n",num);

}























//-----------------------------------------------------------------------------
static void network_steering_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status;

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    // Check if the device has successfully established bindings, if not do so
    if (!binding) {
      sl_zigbee_event_set_active(&finding_and_binding_event);
    }

  } else {
    // If not in a network, attempt to join one
    status = emberAfPluginNetworkSteeringStart();
    sl_zigbee_app_debug_print("%s network %s: 0x%X\n",
                              "Join",
                              "start",
                              status);


  }
}
// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/** @brief Report Attribute
 *
 * This function reports the specified attribute from the specified cluster
 * through a valid binding in the binding table. Therefore, a valid binding
 * entry with a  matching cluster must exist for the report to be successful.
 *
 */
static EmberStatus reportAttribute(EmberAfClusterId cluster,
                                   EmberAfAttributeId attributeID,
                                   uint8_t attribute_type,
                                   uint8_t buff[2])
{
  EmberStatus status = EMBER_SUCCESS;

  // Fill attribute record - See af-structs.h for details of ReportAttributeRecord
  // contents.
  uint8_t attribute_record[] = {
    LOW_BYTE(attributeID),   //uint16_t attributeId
    HIGH_BYTE(attributeID),
    attribute_type,          //uint8_t attributeType;
    buff[0], buff[1],        //uint8_t* attributeLocation;
  };

  //Fill a ZCL global report attributes command buffer
  emberAfFillCommandGlobalServerToClientReportAttributes(
                                      cluster,
                                      attribute_record,
                                      sizeof(attribute_record)/sizeof(uint8_t));

  //Specify endpoints for command sending
  emberAfSetCommandEndpoints(RHT_MEASUREMENT_ENDPOINT, 3);

//  //Use binding table to send unicast command
//  status = emberAfSendCommandUnicastToBindings();

        // Report to Server
        status =  emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, 0x0000);
  if(status){
  printf("\nreported success\n");
  }else
    printf("faile");
  return status;
}


static uint8_t binding_table_unicast_binding_count(void)
{
  uint8_t i;
  EmberBindingTableEntry result;
  uint8_t bindings = 0;

  for (i = 0; i < EMBER_BINDING_TABLE_SIZE; i++) {
    EmberStatus status = emberGetBinding(i, &result);
    if (status == EMBER_SUCCESS
        && result.type == EMBER_UNICAST_BINDING) {
      bindings++;
    }
  }
  return bindings;
}

static void binding_table_print(void)
{
  uint8_t i;
  EmberBindingTableEntry result;

  PGM_P typeStrings[] = {
    "EMPTY",
    "UNICA",
    "M2ONE",
    "MULTI",
    "?    ",
  };
  uint8_t bindings = 0;

  emberAfCorePrintln("#  type   nwk  loc   rem   clus   node   eui");
  for (i = 0; i < EMBER_BINDING_TABLE_SIZE; i++) {
    EmberStatus status = emberGetBinding(i, &result);
    if (status == EMBER_SUCCESS) {
      if (result.type > EMBER_MULTICAST_BINDING) {
        result.type = 4;  // last entry in the string list above
      }
      if (result.type != EMBER_UNUSED_BINDING) {
        bindings++;
        emberAfCorePrint("%d: ", i);
        emberAfCorePrint("%p", typeStrings[result.type]);
        emberAfCorePrint("  %d    0x%x  0x%x  0x%2x 0x%2x ",
                        result.networkIndex,
                        result.local,
                        result.remote,
                        result.clusterId,
                        emberGetBindingRemoteNodeId(i));
        emberAfPrintBigEndianEui64(result.identifier);
        emberAfCorePrintln("");
      }
    } else {
      emberAfCorePrintln("0x%x: emberGetBinding Error: %x", status);
      emberAfAppFlush();
    }
    emberAfAppFlush();
  }
  emberAfCorePrintln("%d of %d bindings used",
      bindings,
      emberAfGetBindingTableSize());
}


static void attribute_report_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status = EMBER_SUCCESS;
  uint32_t rh;
  int32_t t;
  union {
    float t;
    float rh;
  }attribute;



//  if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
//    return;
//  }
    rht_init();
    rht_get_hum_and_temp(&rh, &t);
  if(SL_STATUS_OK == sl_si70xx_measure_rh_and_temp(sl_i2cspm_inst0, SI7021_ADDR, &rh, &t)){
      sl_zigbee_app_debug_print_string((char*)"\nSuccess measure\n");
      //send_unicast_msg(t);
  };
  if (SL_STATUS_OK == sl_si70xx_read_rh_and_temp(sl_i2cspm_inst0, SI7021_ADDR, &rh, &t)) {
    sl_zigbee_app_debug_print("\nRH: %f, Temperature: %f\n",(float)rh/1000,(float)t/1000);
    sensor_data.temperature=t;
    sensor_data.humidity=rh;
    sl_zigbee_app_debug_print("\nRH: %f, Temperature: %f\n",sensor_data.humidity,sensor_data.temperature);

  }
    // Attribute MeasuredValue = 100 x temperature in degrees Celsius.
//    attribute.t =(float) t / 1000;
//
//
//
//    // Send report directly
//    status = reportAttribute( ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
//                              ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID,
//                              ZCL_INT16S_ATTRIBUTE_TYPE,
//                              (uint8_t *)&attribute.t);
//
//    // Just update attribute data to send report to bindings
//    status = emberAfWriteServerAttribute(RHT_MEASUREMENT_ENDPOINT,
//                                         ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
//                                         ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID,
//                                         (uint8_t *)&attribute.t,
//                                         ZCL_INT16S_ATTRIBUTE_TYPE);
//
//    sl_zigbee_app_debug_print("\nTemp reported: %f\n",attribute.t);
//
//    // Attribute MeasuredValue = 100 x Relative humidity
//    attribute.rh =(float) rh / 1000;
//
//    // Send report directly
//    status = reportAttribute( ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,
//                              ZCL_RELATIVE_HUMIDITY_MEASURED_VALUE_ATTRIBUTE_ID,
//                              ZCL_INT16U_ATTRIBUTE_TYPE,
//                              (uint8_t *)&attribute.rh);
//
//    // Just update attribute data to send report to bindings
//    status = emberAfWriteServerAttribute(RHT_MEASUREMENT_ENDPOINT,
//                                         ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,
//                                         ZCL_RELATIVE_HUMIDITY_MEASURED_VALUE_ATTRIBUTE_ID,
//                                         (uint8_t *)&attribute.rh,
//                                         ZCL_INT16U_ATTRIBUTE_TYPE);
//
//    sl_zigbee_app_debug_print("\nHumidity reported: %f\n",attribute.rh);
//
//  } else {
//    sl_zigbee_app_debug_print("Failed to read RHT sensor\n");
//  }
  sl_udelay_wait(1000000);
   EmberApsFrame apsFrame;
  static int num=0;


   uint8_t ** message_uni;

   message_uni = (uint8_t **)malloc(8 * sizeof(uint8_t *));

       // Cấp phát bộ nhớ cho các cột

           message_uni[0] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[1] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[2] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[3] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[4] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[5] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[6] = (uint8_t *)malloc(5 * sizeof(uint8_t));
           message_uni[7] = (uint8_t *)malloc(6 * sizeof(uint8_t));

          int kich_thuoc[8]={5,5,5,5,5,5,5,6};

    //sensor_data_t sensor_data;
  //       sensor_data_t sensor_data={
  //           .node_name = "node1",
  //           .mass =22,
  //
  //           .id=2,
  //           .height=10.6,
  //           .battery=30.3,
  //           .smell=50,
  //
  //       };
  //       sensor_data.temperature=
  //       sensor_data.humidity=
   copy_to_messaget(message_uni,&sensor_data);

   apsFrame.clusterId = 0x0003;
   apsFrame.destinationEndpoint =1;
   apsFrame.options = EMBER_APS_OPTION_NONE;
   apsFrame.sequence = 0;
   apsFrame.sourceEndpoint = emberAfPrimaryEndpoint();

  for(int i=0;i<8;i++){

  status = emberAfSendUnicast(EMBER_OUTGOING_DIRECT, 0x0000, &apsFrame, kich_thuoc[i], *(message_uni + i) ); //gia tri con tro chi den o nho dau tien->dia chi o nho dau tien

  }
  num++;
  printf("\nDem:%d\n",num);
  sl_zigbee_event_set_delay_ms(&attribute_report_event_control,
                                 20000);
}

//HANDLER FUNCTION
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
  EmberStatus status = emberAfPluginFindAndBindInitiatorStart(RHT_MEASUREMENT_ENDPOINT); //initator endpoint
  sl_zigbee_app_debug_println("Find and bind initiator %s: 0x%02X", "start", status);

}


static void leave_network_event_handler(sl_zigbee_event_t *event)
{
  EmberStatus status;

  // Clear binding table
  status = emberClearBindingTable();
  sl_zigbee_app_debug_print("%s 0x%x", "Clear binding table\n", status);

  // Leave network
  status = emberLeaveNetwork();
  sl_zigbee_app_debug_print("%s 0x%x", "leave\n", status);

  commissioning = false;
  binding = false;
}
//----------------------
// Implemented Callbacks
void emberAfMainInitCallback(void)
{
  GPIO_PinModeSet(gpioPortC,9,gpioModePushPull,1);

  I2CSPM_Init_TypeDef init_inst0 = {
    .port = SL_I2CSPM_INST0_PERIPHERAL,
    .sclPort = SL_I2CSPM_INST0_SCL_PORT,
    .sclPin = SL_I2CSPM_INST0_SCL_PIN,
    .sdaPort = SL_I2CSPM_INST0_SDA_PORT,
    .sdaPin = SL_I2CSPM_INST0_SDA_PIN,
    .i2cRefFreq = 0,
    .i2cMaxFreq = SL_I2CSPM_INST0_MAX_FREQ,
    .i2cClhr = SL_I2CSPM_INST0_HLR
  };

  I2CSPM_Init(&init_inst0);
  sl_status_t sc = sl_si70xx_init(sl_i2cspm_inst0, SI7021_ADDR);
    if (sc != SL_STATUS_OK) {
      sl_zigbee_app_debug_print("Relative Humidity and Temperature sensor initialization failed.\n");
    }

  sl_zigbee_event_init(&led_event, led_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event, finding_and_binding_event_handler);

   sl_zigbee_event_init(&network_steering_event_control, network_steering_event_handler);
   //sl_zigbee_event_init(&send_message_event, send_message_test);
    sl_zigbee_event_init(&leave_network_event_control, leave_network_event_handler);
    sl_zigbee_event_init(&attribute_report_event_control, attribute_report_event_handler);

  sl_zigbee_event_set_active(&network_steering_event_control);

    binding_table_print();

//    if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
//
//    sl_zigbee_event_set_active(&attribute_report_event_control);
//
//    }

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
    if (binding_table_unicast_binding_count() > 0) {
          binding = true;
          // If already in a network and bindings are valid, report attributes
          sl_zigbee_event_set_active(&attribute_report_event_control);
        }
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
      // On successful join, do find and bind after a short delay
    sl_zigbee_event_set_delay_ms(&finding_and_binding_event,
                                 FINDING_AND_BINDING_DELAY_MS);
  }
}

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

  if (status == EMBER_SUCCESS) {
      sl_zigbee_event_set_active(&attribute_report_event_control);
      commissioning = false;
    } else {
      sl_zigbee_app_debug_print("Ensure a valid binding target!\n");
      sl_zigbee_event_set_active(&attribute_report_event_control);

      binding = false;
    }


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
    if ( sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
        sl_zigbee_app_debug_println("Button 0 got pressed");
       // sl_zigbee_event_set_active(&attribute_report_event_control);
        sl_zigbee_event_set_active(&network_steering_event_control);
      //  sl_zigbee_event_set_active(&finding_and_binding_event);
     // sl_zigbee_event_set_active(&commissioning_event);
    }
  } else if (SL_SIMPLE_BUTTON_INSTANCE(FACTORY_RESET_BUTTON) == handle) {
    if ( sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
        sl_zigbee_app_debug_println("Factory button got pressed");
        sl_zigbee_event_set_active(& leave_network_event_control);
      //sl_zigbee_event_set_active(&commissioning_event);
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
   // sl_zigbee_event_set_active(&commissioning_event);
  }
}
#endif // EMBER_TEST
