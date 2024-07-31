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


    float temperature;
    float pressure;
    float humidity;
    float distance;

    float longitude;
    float latitude;
    int index;

    //uint8_t feedback;
}sensor_data_t;


#endif /* ZIGBEE_H_ */
