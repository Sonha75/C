#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[CMU]
// [CMU]$

// $[LFXO]
// LFXO LFXTAL_I on PD01
#ifndef LFXO_LFXTAL_I_PORT                      
#define LFXO_LFXTAL_I_PORT                       gpioPortD
#endif
#ifndef LFXO_LFXTAL_I_PIN                       
#define LFXO_LFXTAL_I_PIN                        1
#endif

// LFXO LFXTAL_O on PD00
#ifndef LFXO_LFXTAL_O_PORT                      
#define LFXO_LFXTAL_O_PORT                       gpioPortD
#endif
#ifndef LFXO_LFXTAL_O_PIN                       
#define LFXO_LFXTAL_O_PIN                        0
#endif

// [LFXO]$

// $[PRS.ASYNCH0]
// [PRS.ASYNCH0]$

// $[PRS.ASYNCH1]
// [PRS.ASYNCH1]$

// $[PRS.ASYNCH2]
// [PRS.ASYNCH2]$

// $[PRS.ASYNCH3]
// [PRS.ASYNCH3]$

// $[PRS.ASYNCH4]
// [PRS.ASYNCH4]$

// $[PRS.ASYNCH5]
// [PRS.ASYNCH5]$

// $[PRS.ASYNCH6]
// [PRS.ASYNCH6]$

// $[PRS.ASYNCH7]
// [PRS.ASYNCH7]$

// $[PRS.ASYNCH8]
// [PRS.ASYNCH8]$

// $[PRS.ASYNCH9]
// [PRS.ASYNCH9]$

// $[PRS.ASYNCH10]
// [PRS.ASYNCH10]$

// $[PRS.ASYNCH11]
// [PRS.ASYNCH11]$

// $[PRS.ASYNCH12]
// [PRS.ASYNCH12]$

// $[PRS.ASYNCH13]
// [PRS.ASYNCH13]$

// $[PRS.ASYNCH14]
// [PRS.ASYNCH14]$

// $[PRS.ASYNCH15]
// [PRS.ASYNCH15]$

// $[PRS.SYNCH0]
// [PRS.SYNCH0]$

// $[PRS.SYNCH1]
// [PRS.SYNCH1]$

// $[PRS.SYNCH2]
// [PRS.SYNCH2]$

// $[PRS.SYNCH3]
// [PRS.SYNCH3]$

// $[GPIO]
// GPIO SWV on PA03
#ifndef GPIO_SWV_PORT                           
#define GPIO_SWV_PORT                            gpioPortA
#endif
#ifndef GPIO_SWV_PIN                            
#define GPIO_SWV_PIN                             3
#endif

// [GPIO]$

// $[TIMER0]
// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[TIMER4]
// [TIMER4]$

// $[USART0]
// USART0 RX on PC03
#ifndef USART0_RX_PORT                          
#define USART0_RX_PORT                           gpioPortC
#endif
#ifndef USART0_RX_PIN                           
#define USART0_RX_PIN                            3
#endif

// USART0 TX on PC02
#ifndef USART0_TX_PORT                          
#define USART0_TX_PORT                           gpioPortC
#endif
#ifndef USART0_TX_PIN                           
#define USART0_TX_PIN                            2
#endif

// [USART0]$

// $[I2C1]
// [I2C1]$

// $[EUSART1]
// [EUSART1]$

// $[KEYSCAN]
// [KEYSCAN]$

// $[LETIMER0]
// [LETIMER0]$

// $[IADC0]
// [IADC0]$

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[VDAC0]
// [VDAC0]$

// $[VDAC1]
// [VDAC1]$

// $[PCNT0]
// [PCNT0]$

// $[HFXO0]
// [HFXO0]$

// $[I2C0]
// [I2C0]$

// $[EUSART0]
// EUSART0 CTS on PA09
#ifndef EUSART0_CTS_PORT                        
#define EUSART0_CTS_PORT                         gpioPortA
#endif
#ifndef EUSART0_CTS_PIN                         
#define EUSART0_CTS_PIN                          9
#endif

// EUSART0 RTS on PA08
#ifndef EUSART0_RTS_PORT                        
#define EUSART0_RTS_PORT                         gpioPortA
#endif
#ifndef EUSART0_RTS_PIN                         
#define EUSART0_RTS_PIN                          8
#endif

// EUSART0 RX on PA06
#ifndef EUSART0_RX_PORT                         
#define EUSART0_RX_PORT                          gpioPortA
#endif
#ifndef EUSART0_RX_PIN                          
#define EUSART0_RX_PIN                           6
#endif

// EUSART0 TX on PA05
#ifndef EUSART0_TX_PORT                         
#define EUSART0_TX_PORT                          gpioPortA
#endif
#ifndef EUSART0_TX_PIN                          
#define EUSART0_TX_PIN                           5
#endif

// [EUSART0]$

// $[PTI]
// PTI DFRAME on PC07
#ifndef PTI_DFRAME_PORT                         
#define PTI_DFRAME_PORT                          gpioPortC
#endif
#ifndef PTI_DFRAME_PIN                          
#define PTI_DFRAME_PIN                           7
#endif

// PTI DOUT on PC06
#ifndef PTI_DOUT_PORT                           
#define PTI_DOUT_PORT                            gpioPortC
#endif
#ifndef PTI_DOUT_PIN                            
#define PTI_DOUT_PIN                             6
#endif

// [PTI]$

// $[MODEM]
// [MODEM]$

// $[CUSTOM_PIN_NAME]
#ifndef _PORT                                   
#define _PORT                                    gpioPortA
#endif
#ifndef _PIN                                    
#define _PIN                                     0
#endif

#ifndef LEDPIN_PORT                             
#define LEDPIN_PORT                              gpioPortB
#endif
#ifndef LEDPIN_PIN                              
#define LEDPIN_PIN                               2
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

