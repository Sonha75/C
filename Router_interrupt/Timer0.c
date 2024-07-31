/*
 * Timer0.c
 *
 *  Created on: Jul 12, 2024
 *      Author: Admin
 */
#include "Timer0.h"




bool Timer0_OF=false;











void Timer0_Enable(){
  uint32_t flags=TIMER_IntGet(TIMER1);
  TIMER_IntClear(TIMER1,flags);
  TIMER_Enable(TIMER1,true);
}
void Timer0_Disable(){
  uint32_t flags=TIMER_IntGet(TIMER0);
  TIMER_IntClear(TIMER1,flags);
  TIMER_Enable(TIMER1,false);
}
void Timer0_Init(){
  TIMER_Init_TypeDef timerInit=TIMER_INIT_DEFAULT;

  timerInit.enable=false;
  CMU_ClockEnable( cmuClock_TIMER1, true);


  TIMER_Init(TIMER1,&timerInit);

  //timer top value
  uint32_t timerfreq=CMU_ClockFreqGet(cmuClock_TIMER1)/(timerInit.prescale+1);
  uint32_t timerTopval=timerfreq/TIMER0_FREQUENCY;
  TIMER_TopSet(TIMER1,timerTopval);



//interrupt enable
  TIMER_IntEnable(TIMER1,TIMER_IEN_OF);


  NVIC_EnableIRQ(TIMER1_IRQn);

}

void Timer0_IRQHandler(){
  uint32_t flags=TIMER_IntGet(TIMER1);
    TIMER_IntClear(TIMER1,flags);
    Timer0_OF=true;
};

