
#include "zigbee.h"

#include "usart_sim.h"
#include "ustimer.h"

#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

extern sensor_data_t receive_msg;

// Size of the buffer for received data
#define BUFLEN  80

// Receive data buffer
uint8_t buffer[BUFLEN];

/*chuyen receive_msg thanh chuoi json de truyen (KHONG TRUYEN NODE_NAME) sau do truyen bang sendcommand(json)*/
char *json_data()
{
    char *json = (char *)malloc(256);
    sprintf(json,"{\"temp\":%d,\"weight\":%d,\"humid\":%d,\"full\":%d,\"lng\":%d,\"lat\":%d,\"smell\":%d}",
    (int)receive_msg.temperature,(int)receive_msg.pressure,(int)receive_msg.humidity,(int)receive_msg.distance,(int)receive_msg.longitude,(int)receive_msg.latitude,(int)receive_msg.index);
    return json;
}

// define GPIO port and pin for USART0
#define USART0_TX_PORT gpioPortC
#define USART0_TX_PIN 2
#define USART0_RX_PORT gpioPortC
#define USART0_RX_PIN 3

// Current position ins buffer
uint32_t inpos = 0;
uint32_t outpos = 0;

// True while receiving data (waiting for CR or BUFLEN characters)
bool receive = true;

/**************************************************************************//**
 * @brief
 *    CMU initialization
 *****************************************************************************/

void initCMU(void)
{
  // Enable clock to GPIO and USART0
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);
}

/**************************************************************************//**
 * @brief
 *    GPIO initialization
 *****************************************************************************/

void initGPIO(void)
{
    // Configure the USART TX pin to the board controller as an output
    GPIO_PinModeSet(USART0_TX_PORT, USART0_TX_PIN, gpioModePushPull, 1);

    // Configure the USART RX pin to the board controller as an input
    GPIO_PinModeSet(USART0_RX_PORT, USART0_RX_PIN, gpioModeInput, 0);
}

/**************************************************************************//**
 * @brief
 *    USART0 initialization
 *****************************************************************************/

void initUSART0(void)
{
    // Default asynchronous initializer (mainly for simplicity)
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

    // Route USART0 TX and RX to the specified pins
    GPIO->USARTROUTE[0].TXROUTE = (USART0_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
                                    | (USART0_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].RXROUTE = (USART0_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                    | (USART0_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

    // Enable RX and TX signals now that they have been routed
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

    // Configure the USART0 module
    USART_InitAsync(USART0, &init);

    // enable NVIC USART source
    NVIC_ClearPendingIRQ(USART0_RX_IRQn);
    NVIC_EnableIRQ(USART0_RX_IRQn);
    NVIC_ClearPendingIRQ(USART0_TX_IRQn);
    NVIC_EnableIRQ(USART0_TX_IRQn);
}

/**************************************************************************//**
 * @brief
 *    USART0 receive interrupt handler
 *****************************************************************************/

void USART0_RX_IRQHandler(void)
{
    // Get the character just received
    buffer[inpos] = USART0->RXDATA;

    // Exit loop on new line or buffer full
    if ((buffer[inpos] != '\n') && (inpos < BUFLEN))
        inpos++;
    else
        receive = false;   // Stop receiving on CR
}

/**************************************************************************//**
 * @brief
 *    USART0 transmit interrupt handler
 *****************************************************************************/

void USART0_TX_IRQHandler(void)
{
    // Send a previously received character
    if (outpos < inpos)
        USART0->TXDATA = buffer[outpos++];
    else
    /*
    * Need to disable the transmit buffer level interrupt in this IRQ
    * handler when done or it will immediately trigger again upon exit
    * even though there is no data left to send.
    */
    {
        receive = true;   // Go back into receive when all is sent
        USART_IntDisable(USART0, USART_IEN_TXBL);
    }
}


void initUART(void)
{



    // Initialize the USART0 module
    initCMU();
    initGPIO();
    initUSART0();
    USTIMER_Init();
}

// Phan nay code gui command bang UART_TX
void sendcommand(char *command)
{
  // Send command
  for (int i = 0; i < strlen(command); i++)
    USART_Tx(USART0, command[i]);
  USART_Tx(USART0, '\r');
  USART_Tx(USART0, '\n');
}

void initSIM()
{
    // Send AT command to check if SIM is ready
    sendcommand("AT");
    USTIMER_DelayIntSafe(1000);

    // Send ATE0 command to turn off echo
//    sendcommand("ATE0");
//    USTIMER_DelayIntSafe(1000);

    // Send AT+CGREG? command to check if SIM is registered
    sendcommand("AT+CGREG?");
    USTIMER_DelayIntSafe(1000);
    //USTIMER_DelayIntSafe(1000000);

    // Send AT+NETOPEN
    sendcommand("AT+NETOPEN");
    USTIMER_DelayIntSafe(1000);

       // httpPUT(json);

    char *json = json_data();
    httpPUT(json);
}

void httpPUT(const char *json)
{
    // neu node gui du lieu la node01
//    if(strcmp((const char *)receive_msg.node_name, "Node01") == 0)
//    {
        // Send AT+HTTPINIT
        sendcommand("AT+HTTPINIT");
        USTIMER_DelayIntSafe(10000);
        USTIMER_DelayIntSafe(1000);

        // Send AT+HTTPPARA
        // neu node gui du lieu la node01
        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash1
        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash1.json\"");
        USTIMER_DelayIntSafe(30000);
        USTIMER_DelayIntSafe(1000);

        // send HTTPPARA
        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
        USTIMER_DelayIntSafe(30000);
        USTIMER_DelayIntSafe(1000);

        // du lieu
        char command[256];
        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
        sendcommand(command);
        USTIMER_DelayIntSafe(1000);
        USTIMER_DelayIntSafe(30000);

        // send data
        sendcommand(json);
        USTIMER_DelayIntSafe(1000);
        USTIMER_DelayIntSafe(1000);
        USTIMER_DelayIntSafe(50000);

        // send AT+HTTPACTION
        sendcommand("AT+HTTPACTION=4");
        USTIMER_DelayIntSafe(50000);
        USTIMER_DelayIntSafe(1000);
        USTIMER_DelayIntSafe(1000);

        // sed AT+HTTPTERM
        sendcommand("AT+HTTPTERM");
        USTIMER_DelayIntSafe(5000);
//    }

    // neu node gui du lieu la node02
//    else if(strcmp((const char *)receive_msg.node_name, "node2") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node02
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash2
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash2.json\"");
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//        USTIMER_DelayIntSafe(1000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000);
//    }
//
//    // neu node gui du lieu la node03
//    else if(strcmp((const char *)receive_msg.node_name, "Node03") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node03
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash3
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash3.json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000000);
//    }
//
//    // neu node gui du lieu la node04
//    else if(strcmp((const char *)receive_msg.node_name, "Node04") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node04
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash4
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash4.json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000000);
//    }
//
//    // neu node gui du lieu la node05
//    else if(strcmp((const char *)receive_msg.node_name, "Node05") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node05
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash5
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash5.json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000000);
//    }
//
//    // neu node gui du lieu la node06
//    else if(strcmp((const char *)receive_msg.node_name, "Node06") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node06
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash6
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash6.json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000000);
//    }
//
//    // neu node gui du lieu la node07
//    else if(strcmp((const char *)receive_msg.node_name, "Node07") == 0)
//    {
//        // Send AT+HTTPINIT
//        sendcommand("AT+HTTPINIT");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // Send AT+HTTPPARA
//        // neu node gui du lieu la node07
//        // gui du lieu len firebase https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash7
//        sendcommand("AT+HTTPPARA=\"URL\",\"https://iot-chale-default-rtdb.asia-southeast1.firebasedatabase.app/trash/trash7.json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send HTTPPARA
//        sendcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // du lieu
//        char command[256];
//        snprintf(command, sizeof(command), "AT+HTTPDATA=%d,10000", strlen(json));
//        sendcommand(command);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send data
//        sendcommand(json);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // send AT+HTTPACTION
//        sendcommand("AT+HTTPACTION=4");
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//        USTIMER_DelayIntSafe(1000000);
//
//        // sed AT+HTTPTERM
//        sendcommand("AT+HTTPTERM");
//        USTIMER_DelayIntSafe(1000000);
//    }
}

//void print(){
//              printf("Temp - %.2f \r\n", receive_msg.temperature);
//              printf("Humi - %.2f \r\n", receive_msg.humidity);
//              printf("Longitude - %.2f \r\n", receive_msg.longitude);
//              printf("Latitude - %.2f \r\n", receive_msg.latitude);
//              printf("Node - %s \r\n", receive_msg.node_name);
//              printf("distance - %.2f \r\n", receive_msg.distance);
//              printf("pressure - %.2f \r\n", receive_msg.pressure);
//              printf("index - %d \r\n", receive_msg.index);
//}

