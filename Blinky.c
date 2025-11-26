/*------------------------------------------------------------------------------
 * Copyright (c) 2019 Arm Limited (or its affiliates). All
 * rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1.Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   3.Neither the name of Arm nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS:Keil RTX5
// #include "Board_LED.h"                  // Board Support:LED
#include "cmsis_vio.h"                  // ARM::CMSIS:VIO
#include "GPIO_LPC17xx.h"               // For direct GPIO access
#include "LPC17xx.h"                    // For direct register access

static osThreadId_t tid_thrLED;         // Thread id of thread: LED
volatile     uint32_t g_ledSet = 0;     // Global variable to store LED values:
                                        // 0 = LED0 off, LED1 off
                                        // 1 = LED0 on,  LED1 off
                                        // 2 = LED0 off, LED1 on
                                        // 3 = LED0 on,  LED1 on

// External counter from vio to verify function calls
extern volatile uint32_t vioSetSignalCallCount;

// Create thread attribute to show thread name in the XRTOS viewer:
const osThreadAttr_t app_main_attr  = {.name = "MainThread"};
const osThreadAttr_t thrLED_attr    = {.name = "LEDThread"};

/*------------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
static __NO_RETURN void thrLED (void *argument) {

  (void)argument;
  
  // Re-initialize LED pins from within the thread
  LPC_GPIO1->FIODIR |= (1UL << 18) | (1UL << 20) | (1UL << 21) | (1UL << 23);  // Set as outputs

  for (;;) {
    // Store call count for debugging
    g_ledSet++;
    
    // Force register writes with volatile pointer and memory barrier
    __DMB();  // Data Memory Barrier
    LPC_GPIO1->FIOSET = (1UL << 18) | (1UL << 21);  // Turn on LED0 and LED2
    __DMB();
    
    vioSetSignal(vioLED0, vioLEDon);
    osDelay (500U);
    
    __DMB();
    LPC_GPIO1->FIOCLR = (1UL << 18) | (1UL << 21);  // Turn off LED0 and LED2
    __DMB();
    
    vioSetSignal(vioLED0, vioLEDoff);
    osDelay (500U);
  }
}

/*---------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
static void app_main_thread (void *argument) {
  (void)argument;

  tid_thrLED = osThreadNew(thrLED, NULL, &thrLED_attr);         // Create LED thread
  if (tid_thrLED == NULL) { /* add error handling */ }

  for (;;) {                            // Loop forever
  }
}

/*-----------------------------------------------------------------------------
  Application initialization
 *----------------------------------------------------------------------------*/
int app_main (void) {
  osKernelInitialize();                         /* Initialize CMSIS-RTOS2 */
  osThreadNew(app_main_thread, NULL, &app_main_attr);
  osKernelStart();                              /* Start thread execution */
  return 0;                                    /* Should never reach here */
}
