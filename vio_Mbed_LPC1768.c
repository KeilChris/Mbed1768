/******************************************************************************
 * @file     vio_Mbed_LPC1768.c
 * @brief    Virtual I/O implementation for board Mbed LPC1768
 * @version  V2.0.0
 * @date     25. November 2025
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
vioBUTTON0        | vioSignalIn.0  | P0.14: User Button (GPIO0[14])    |
vioLED0           | vioSignalOut.0 | P1.18: LED1 (GPIO1[18])           |
vioLED1           | vioSignalOut.1 | P1.20: LED2 (GPIO1[20])           |
vioLED2           | vioSignalOut.2 | P1.21: LED3 (GPIO1[21])           |
vioLED3           | vioSignalOut.3 | P1.23: LED4 (GPIO1[23])           |
*/

/* History:
 *  Version 2.0.0
 *    Updated to API 1.0.0
 */

#include <string.h>
#include "cmsis_vio.h"

#include "RTE_Components.h"                 // Component selection
#include CMSIS_device_header
#include "LPC17xx.h"                        // For direct register access

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"

// VIO input, output definitions
#define VIO_VALUE_NUM           1U          // Number of values

// VIO input, output variables
__USED uint32_t vioSignalIn;                // Memory for incoming signal
__USED uint32_t vioSignalOut;               // Memory for outgoing signal
__USED int32_t  vioValue[VIO_VALUE_NUM];    // Memory for value used in vioGetValue/vioSetValue

// Debug counter to verify vioSetSignal is being called
volatile uint32_t vioSetSignalCallCount = 0;

// LED pin definitions
#define LED_COUNT               4U
static const PIN LED_PIN[] = {
  {1U, 18U},  // LED0: P1.18 = GPIO1[18]
  {1U, 20U},  // LED1: P1.20 = GPIO1[20]
  {1U, 21U},  // LED2: P1.21 = GPIO1[21]
  {1U, 23U}   // LED3: P1.23 = GPIO1[23]
};

// Button pin definition
#define BUTTON_PORT             0U
#define BUTTON_PIN              14U   // P0.14 = GPIO0[14]

/*-----------------------------------------------------------------------------
  LED_Initialize: Initialize LEDs
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
  
  // Test: Turn on LED0 to verify GPIO works at init
  GPIO_PinWrite(LED_PIN[0].Portnum, LED_PIN[0].Pinnum, 1U);
  
  // Test: Turn on LED2 using direct register access
  LPC_GPIO1->FIOSET = (1UL << 21);
}

/*-----------------------------------------------------------------------------
  Button_Initialize: Initialize Button
 *----------------------------------------------------------------------------*/
static void Button_Initialize(void)
{
  /* Enable GPIO clock */
  GPIO_PortClock(BUTTON_PORT);

  /* Configure button pin: Input Mode with Pull-up resistor */
  PIN_Configure(BUTTON_PORT, BUTTON_PIN, PIN_FUNC_0, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);
  GPIO_SetDir(BUTTON_PORT, BUTTON_PIN, GPIO_DIR_INPUT);
}

// Initialize test input, output.
void vioInit (void) {
  vioSignalIn  = 0U;
  vioSignalOut = 0U;

  memset(vioValue, 0, sizeof(vioValue));
  LED_Initialize();    // Initialize LEDs
  Button_Initialize(); // Initialize Button
}

// Set signal output.
void vioSetSignal (uint32_t mask, uint32_t signal) {
  vioSetSignalCallCount++;  // Increment counter
  
  vioSignalOut &= ~mask;
  vioSignalOut |=  mask & signal;

  // Output signals to LEDs using direct register access
  if ((mask & vioLED0) != 0U) {
    if (vioSignalOut & vioLED0) {
      LPC_GPIO1->FIOSET = (1UL << 18);  // LED0 on (P1.18)
    } else {
      LPC_GPIO1->FIOCLR = (1UL << 18);  // LED0 off
    }
  }
  if ((mask & vioLED1) != 0U) {
    if (vioSignalOut & vioLED1) {
      LPC_GPIO1->FIOSET = (1UL << 20);  // LED1 on (P1.20)
    } else {
      LPC_GPIO1->FIOCLR = (1UL << 20);  // LED1 off
    }
  }
  if ((mask & vioLED2) != 0U) {
    if (vioSignalOut & vioLED2) {
      LPC_GPIO1->FIOSET = (1UL << 21);  // LED2 on (P1.21)
    } else {
      LPC_GPIO1->FIOCLR = (1UL << 21);  // LED2 off
    }
  }
  if ((mask & vioLED3) != 0U) {
    if (vioSignalOut & vioLED3) {
      LPC_GPIO1->FIOSET = (1UL << 23);  // LED3 on (P1.23)
    } else {
      LPC_GPIO1->FIOCLR = (1UL << 23);  // LED3 off
    }
  }
}

// Get signal input.
uint32_t vioGetSignal (uint32_t mask) {
  uint32_t signal;

  // Get input signals from buttons
  // User Button on P0.14 (active low)
  if ((mask & vioBUTTON0) != 0U) {
    if (GPIO_PinRead(BUTTON_PORT, BUTTON_PIN) == 0U) {
      vioSignalIn |=  vioBUTTON0;
    } else {
      vioSignalIn &= ~vioBUTTON0;
    }
  }

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