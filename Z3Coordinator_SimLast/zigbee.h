/*
 * zigbee.h
 *
 *  Created on: Jul 12, 2024
 *      Author: Admin
 */

#ifndef ZIGBEE_H_
#define ZIGBEE_H_
#include "stdint.h"

typedef struct {
    uint8_t node_name[6];


    int temperature;
    int mass;
    int humidity;
    int height;

//    float longitude;
//    float latitude;
    int id;
    int battery;
    int smell;

    //uint8_t feedback;
}sensor_data_t;


#endif /* ZIGBEE_H_ */
