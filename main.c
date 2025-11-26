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
 * Name:    main.c
 * Purpose: Main module
 *----------------------------------------------------------------------------*/

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "main.h"
#include "main.h"
#include "RTE_Components.h"
#include CMSIS_device_header

// #include "LPC17xx.h"                    // Device header
// #include "Board_LED.h"                  // ::Board Support:LED
#include "cmsis_vio.h"

/*----------------------------------------------------------------------------
 * main function
 *---------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockUpdate ();             // System Initialization
  // LED_Initialize ();                    // Initialize LEDs
  vioInit();
  app_main();
  // osKernelInitialize ();                // Initialize CMSIS-RTOS
  // osThreadNew (app_main, NULL, NULL);   // Create application main thread
  // osKernelStart ();                     // Start thread execution
  for (;;) {}
}
