/*
  Copyright 2010-07 By Opendous Inc. (www.MicropendousX.org)

  Basic demo for LPC Cortex-M3 ICs.  Uses the SysTick
  interrupt to blink an LED at 100ms intervals.

  This file is hereby released under a modified MIT license
  which does not require attribution:

  Permission to use, copy, modify, and distribute this source file
  and its documentation for any purpose and without fee is hereby
  granted, provided that the following warranty disclaimer appear
  in supporting documentation, and that the name of the author(s)
  not be used in advertising or publicity pertaining to distribution
  of the software without specific, written prior permission.

  The author(s) disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/
#include "LPC17xx.h"


volatile uint32_t SysTickCount;


/* Naming a function SysTick_Handler will place it appropriately into the vector table */
void SysTick_Handler(void)
{
	SysTickCount++;		// increment the SysTick counter
}


/* Busyloop for delayTime number of SysTicks */
inline static void delay(uint32_t delayTime)
{
	uint32_t currentSysTickCount;

	currentSysTickCount = SysTickCount;
	while ((SysTickCount - currentSysTickCount) < delayTime);
}



int main(void)
{

	// CMSIS Function to set up the SysTick Timer for 1ms interrupts
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);		// wait until SysTick interrupt is configured
	}

	SystemCoreClockUpdate();	// CMSIS Function that updates SystemCoreClock after it is set up

	// Configure the LED - use USB_Connect pin which already has an LED - P1.18
	LPC_PINCON->PINSEL4 = ((0 << 18) | (0 << 19));	// set pin P1.18 as GPIO Pin (0b00)
	LPC_GPIO1->FIODIR = (1 << 18);	// set pin P1.18 as output (Output = 1, Input = 0)                   

	// Flash the LED at 100ms intervals
	while(1) {
		LPC_GPIO1->FIOPIN |=  (1 << 18);		// Turn On  LED - set pin P1.18 to HIGH
		delay(100);											// delay  100 ms (SysTick configured for 1ms interrupts)
		LPC_GPIO1->FIOPIN &= ~(1 << 18);	// Turn Off LED - clear pin P1.18 to LOW
		delay(100);											// delay  100 ms (SysTick configured for 1ms interrupts)
	}

}
