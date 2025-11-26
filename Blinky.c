/*---------------------------------------------------------------------------
 * Copyright (c) 2025 Arm Limited (or its affiliates). All rights reserved.
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
 *------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------*/

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS:Keil RTX5
#include "cmsis_vio.h"                  // CMSIS:VIO

static osThreadId_t tid_thrLED;         // Thread id of thread: LED

volatile     uint32_t g_ledSet = 0;     // Variable to store virtual LED value:
                                        // 0 = LED0 on
                                        // 1 = LED1 on
                                        // 2 = LED2 on 
                                        // 3 = LED3 on
                                        
// Create thread attributes to show thread names in the XRTOS viewer:
const osThreadAttr_t app_main_attr = {.name = "MainThread", .stack_size = 128U};   // Requires less stack
const osThreadAttr_t thrLED_attr   = {.name = "LEDThread"};

/*------------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
__NO_RETURN void thrLED (void *argument) {

  for (;;) {
      vioSetSignal(vioLED3, vioLEDoff);       // Switch LED off
      vioSetSignal(vioLED0, vioLEDon);        // Switch LED on
      g_ledSet = 0;
      osDelay (500U);                         // Delay 500 ms
      vioSetSignal(vioLED0, vioLEDoff);       // Switch LED off
      vioSetSignal(vioLED1, vioLEDon);        // Switch LED on
      g_ledSet = 1;
      osDelay (500U);                         // Delay 500 ms
      vioSetSignal(vioLED1, vioLEDoff);       // Switch LED off
      vioSetSignal(vioLED2, vioLEDon);        // Switch LED on
      g_ledSet = 2;
      osDelay (500U);                         // Delay 500 ms
      vioSetSignal(vioLED2, vioLEDoff);       // Switch LED off
      vioSetSignal(vioLED3, vioLEDon);        // Switch LED on
      g_ledSet = 3;
      osDelay (500U);                         // Delay 500 ms
  }
}

/*---------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
static void app_main_thread (void *argument) {
  (void)argument;

  tid_thrLED = osThreadNew(thrLED, NULL, &thrLED_attr);         // Create LED thread
  if (tid_thrLED == NULL) { /* add error handling */ }

  for (;;) { /* loop forever */ }
}

/*-----------------------------------------------------------------------------
 * Application initialization
 *----------------------------------------------------------------------------*/
int app_main (void) {
  osKernelInitialize();                         // Initialize CMSIS-RTOS2
  osThreadNew(app_main_thread, NULL, &app_main_attr);  // Create application main thread
  osKernelStart();                              // Start thread execution
  return 0;
}
