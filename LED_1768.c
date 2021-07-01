/*-----------------------------------------------------------------------------
 * Copyright (c) 2013 - 2019 Arm Limited (or its affiliates). All
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
 *-----------------------------------------------------------------------------
 * Name:    LED_MCB1700.c
 * Purpose: LED interface for MCB1700 evaluation board
 * Rev.:    1.0.2
   ---------------------------------------------------------------------------*/


#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"

#include "Board_LED.h"

#define LED_COUNT (4U)
 
/* LED pins:
   - LED1: P1_18 = GPIO1[18]
   - LED2: P1_20 = GPIO1[20]
   - LED3: P1_21 = GPIO1[21]
   - LED4: P1_23 = GPIO1[23]
*/

static const PIN LED_PIN[] = {
  {1U, 18U},
  {1U, 20U},
  {1U, 21U},
  {1U, 23U}
};

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
  uint32_t n;

  /* Enable GPIO clock */
  GPIO_PortClock     (1U);

  /* Configure pins: Output Mode with Pull-down resistors */
  for (n = 0U; n < LED_COUNT; n++) {
    PIN_Configure (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
    GPIO_SetDir   (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, GPIO_DIR_OUTPUT);
    GPIO_PinWrite (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, 0U);
  }

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize (void) {
  uint32_t n;

  /* Unconfigure pins: turn off Pull-up/down resistors */
  for (n = 0U; n < LED_COUNT; n++) {
    PIN_Configure (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, 0U , 0U, 0U);
  }

  return 0;
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite (LED_PIN[num].Portnum, LED_PIN[num].Pinnum, 1U);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite (LED_PIN[num].Portnum, LED_PIN[num].Pinnum, 0U);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0U; n < LED_COUNT; n++) {
    if (val & (1 << n)) LED_On (n);
    else                LED_Off(n);
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {

  return LED_COUNT;
}
