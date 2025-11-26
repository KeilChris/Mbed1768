/******************************************************************************
 * @file     vio_Mbed_LPC1768.c
 * @brief    Virtual I/O implementation for board Mbed LPC1768
 * @version  V1.0.0
 * @date     26. November 2025
 ******************************************************************************/
/*
 * Copyright (c) 2021-2025 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \page vio_Mbed_LPC1768 Physical I/O Mapping

The table below lists the physical I/O mapping of this CMSIS-Driver VIO implementation.

Virtual Resource  | Variable       | Physical Resource on Mbed LPC1768 |
:-----------------|:---------------|:----------------------------------|
vioLED0           | vioSignalOut.0 | P1.18: LED1 (GPIO1[18])           |
vioLED1           | vioSignalOut.1 | P1.20: LED2 (GPIO1[20])           |
vioLED2           | vioSignalOut.2 | P1.21: LED3 (GPIO1[21])           |
vioLED3           | vioSignalOut.3 | P1.23: LED4 (GPIO1[23])           |
*/

/* History:
 *  Version 1.0.0
 *    Initial version for Mbed LPC1768
 */

#include <string.h>
#include "cmsis_vio.h"

#include "RTE_Components.h"
#include CMSIS_device_header

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"

// VIO input, output definitions
#define VIO_VALUE_NUM           1U          // Number of values

// VIO input, output variables
__USED uint32_t vioSignalIn;                // Memory for incoming signal
__USED uint32_t vioSignalOut;               // Memory for outgoing signal
__USED int32_t  vioValue[VIO_VALUE_NUM];    // Memory for value used in vioGetValue/vioSetValue

// LED pin definitions (from LED_1768.c)
#define LED_COUNT               4U
static const PIN LED_PIN[] = {
  {1U, 18U},  // LED0: P1.18 = GPIO1[18]
  {1U, 20U},  // LED1: P1.20 = GPIO1[20]
  {1U, 21U},  // LED2: P1.21 = GPIO1[21]
  {1U, 23U}   // LED3: P1.23 = GPIO1[23]
};

/*-----------------------------------------------------------------------------
  LED_Initialize: Initialize LEDs (from LED_1768.c)
 *----------------------------------------------------------------------------*/
static void LED_Initialize(void)
{
  uint32_t n;

  /* Enable GPIO clock */
  GPIO_PortClock(1U);

  /* Configure pins: Output Mode with Pull-down resistors */
  for (n = 0U; n < LED_COUNT; n++) {
    PIN_Configure(LED_PIN[n].Portnum, LED_PIN[n].Pinnum, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
    GPIO_SetDir(LED_PIN[n].Portnum, LED_PIN[n].Pinnum, GPIO_DIR_OUTPUT);
    GPIO_PinWrite(LED_PIN[n].Portnum, LED_PIN[n].Pinnum, 0U);
  }
}

// Initialize test input, output.
void vioInit (void) {
  vioSignalIn  = 0U;
  vioSignalOut = 0U;

  memset(vioValue, 0, sizeof(vioValue));
  
  LED_Initialize();    // Initialize LEDs
}

// Set signal output.
void vioSetSignal (uint32_t mask, uint32_t signal) {
  vioSignalOut &= ~mask;
  vioSignalOut |=  mask & signal;

  // Output signals to LEDs using GPIO_PinWrite (like LED_1768.c)
  if ((mask & vioLED0) != 0U) {
    GPIO_PinWrite(LED_PIN[0].Portnum, LED_PIN[0].Pinnum, (vioSignalOut & vioLED0) ? 1U : 0U);
  }
  if ((mask & vioLED1) != 0U) {
    GPIO_PinWrite(LED_PIN[1].Portnum, LED_PIN[1].Pinnum, (vioSignalOut & vioLED1) ? 1U : 0U);
  }
  if ((mask & vioLED2) != 0U) {
    GPIO_PinWrite(LED_PIN[2].Portnum, LED_PIN[2].Pinnum, (vioSignalOut & vioLED2) ? 1U : 0U);
  }
  if ((mask & vioLED3) != 0U) {
    GPIO_PinWrite(LED_PIN[3].Portnum, LED_PIN[3].Pinnum, (vioSignalOut & vioLED3) ? 1U : 0U);
  }
}

// Get signal input.
uint32_t vioGetSignal (uint32_t mask) {
  uint32_t signal;

  signal = vioSignalIn & mask;

  return signal;
}

// Set value output.
void vioSetValue (uint32_t id, int32_t value) {
  uint32_t index = id;

  if (index >= VIO_VALUE_NUM) {
    return;                             /* return in case of out-of-range index */
  }

  vioValue[index] = value;
}

// Get value input.
int32_t vioGetValue (uint32_t id) {
  uint32_t index = id;
  int32_t  value = 0;

  if (index >= VIO_VALUE_NUM) {
    return value;                       /* return default in case of out-of-range index */
  }

  value = vioValue[index];

  return value;
}
