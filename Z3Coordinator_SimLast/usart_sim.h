/*
 * usart_sim.h
 *
 *  Created on: Jul 12, 2024
 *      Author: Admin
 */

#ifndef USART_SIM_H_
#define USART_SIM_H_

#include "zigbee.h"
#include "stdint.h"

void initCMU(void);
void initGPIO(void);
void initUSART0(void);
void USART0_RX_IRQHandler(void);
void USART0_TX_IRQHandler(void);
void initUART(void);
void initSIM(void);
void sendcommand(char *command);
void httpPUT(const char *json);

#endif /* USART_SIM_H_ */
