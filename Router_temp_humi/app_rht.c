/***************************************************************************//**
 * @file app_rht.c
 * @brief RHT sensor MCU example - RHT
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "app_rht.h"

#include "sl_i2cspm.h"
#include "sl_i2cspm_instances.h"
#include "sl_si70xx.h"
#include "sl_i2cspm_inst0_config.h"
#include "sl_sleeptimer.h"

#include "printf.h"
// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static void timer_expiration_callback(sl_sleeptimer_timer_handle_t *handle,
                                   void *data);
// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

//Sleep timer instance
static sl_sleeptimer_timer_handle_t delay_timer;

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------
/***************************************************************************//**
 * @brief
 *     Verifies if the RHT sensor can be reached by I2C
 ******************************************************************************/
void rht_init(void)
{
  sl_status_t status = SL_STATUS_OK;

  status = sl_si70xx_init(sl_i2cspm_inst0, 0x40);

  if (status != SL_STATUS_OK)
  {
      printf("Error 0x%lx during RHT sensor init!", status);
  }
}

/***************************************************************************//**
 * @brief
 *     Retrieves data from the RHT sensor, both RH and temperature
 *
 * @param[in] rdData
 *     Pointer to buffer that holds the relative humidity data
 *
 * @param[in] tempData
 *     Pointer to buffer that holds the temperature data
 ******************************************************************************/
void rht_get_hum_and_temp(uint32_t *rhData, int32_t *tempData)
{
  sl_status_t status = SL_STATUS_OK;

  status = sl_si70xx_measure_rh_and_temp(sl_i2cspm_inst0,
                                           SI7021_ADDR,
                                           rhData,
                                           tempData);

   if (status != SL_STATUS_OK)
   {
       printf("Error 0x%lx retrieving sensor data!", status);
   }
}

/***************************************************************************//**
 * @brief
 *     Initiates a timer used to space the RHT measurements. The system goes to
 *     sleep in the meantime.
 *
 * @param[in] time_ms
 *     Time in miliseconds between measurements
 *
 * @param[in] ongoing_delay
 *     Control flag to indicate if a measurement delay is ongoing
 ******************************************************************************/
void rht_intermeasurement_delay(uint16_t time_ms, volatile bool *ongoing_delay)
{
  sl_status_t error_code;

  *ongoing_delay = true; //Set callback variable to true

  uint32_t delay = sl_sleeptimer_ms_to_tick(time_ms); //doi ms sang ticks

  error_code = sl_sleeptimer_start_timer(&delay_timer,
                                         delay,
                                         timer_expiration_callback,
                                         (void *)ongoing_delay,
                                         0,
                                         SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);

  if (error_code != SL_STATUS_OK){
      while(1); //We shouldn't reach this state
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------
/*******************************************************************************
 * Timer expiration callback.
 *
 * @param[in] handle
 *     Pointer to sleeptimer handle.
 *
 * @param[in] data
 *     Pointer to callback data.
 ******************************************************************************/
static void timer_expiration_callback(sl_sleeptimer_timer_handle_t *handle,
                                   void *data)
{

  volatile bool *wait_flag = (bool *)data;

  (void)&handle;  // Unused parameter.

  *wait_flag = false;
}
