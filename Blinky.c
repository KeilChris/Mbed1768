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

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS:Keil RTX5
#include "Board_LED.h"                  // Board Support:LED

static osThreadId_t tid_thrLED;         // Thread id of thread: LED

/*------------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/
__NO_RETURN void thrLED (void *argument) {

  for (;;) {
      LED_On (3U);                                // Switch LED on
      osDelay (500U);                             // Delay 500 ms
      LED_Off (3U);                               // Switch off
      osDelay (500U);                             // Delay 500 ms
  }
}

/*------------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
void app_main (void *argument) {

  tid_thrLED = osThreadNew (thrLED, NULL, NULL);  // Create LED thread
  if (tid_thrLED == NULL) { /* add error handling */ }

  osThreadExit();
}
