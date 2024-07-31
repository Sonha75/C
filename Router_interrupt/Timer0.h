/*
 * Timer0.h
 *
 *  Created on: Jul 12, 2024
 *      Author: Admin
 */

#ifndef TIMER0_H_
#define TIMER0_H_
#include "em_cmu.h"
#include "em_timer.h"


#define TIMER0_FREQUENCY 1 //Hz and 1s tang lam led nhay nhanh hon


void Timer0_Init();
void Timer0_Enable();
void Timer0_Disable();
void Timer0_IRQHandler();




#endif /* TIMER0_H_ */
