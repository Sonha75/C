#include "usart_sim.h"
#include "zigbee.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

extern sensor_data_t receive_msg;


void print(){
              printf("Temp - %.2f \r\n", receive_msg.temperature);
              printf("Humi - %.2f \r\n", receive_msg.humidity);
              printf("Longitude - %.2f \r\n", receive_msg.longitude);
              printf("Latitude - %.2f \r\n", receive_msg.latitude);
              printf("Node - %s \r\n", receive_msg.node_name);
              printf("distance - %.2f \r\n", receive_msg.distance);
              printf("pressure - %.2f \r\n", receive_msg.pressure);
              printf("index - %d \r\n", receive_msg.index);
}

