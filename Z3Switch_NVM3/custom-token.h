/*
 * custom-token.h
 *
 *  Created on: Jul 17, 2024
 *      Author: Admin
 */



// Define token names here
#define NVM3KEY_LED1_ON_OFF     (NVM3KEY_DOMAIN_USER | 0x0001)
#define NVM3KEY_REPORTING_TIME   (NVM3KEY_DOMAIN_USER | 0x0002)

#if defined(DEFINETYPES)
// Include or define any typedef for tokens here
typedef bool ledOnOff;
typedef uint32_t report_time;
#endif //DEFINETYPES

#ifdef DEFINETOKENS
// Define the actual token storage information here
DEFINE_BASIC_TOKEN(LED1_ON_OFF,ledOnOff,0)
DEFINE_BASIC_TOKEN(REPORTING_TIME,report_time,5)
#endif


