/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Adapted for the LPC17xx by Opendous Inc. - www.MicropendousX.org

  Permission to use, copy, modify, distribute, and sell this 
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in 
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting 
  documentation, and that the name of the author not be used in 
  advertising or publicity pertaining to distribution of the 
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef __USBINTERRUPT_H__
#define __USBINTERRUPT_H__

	/* Includes: */
		#include <LPC17xx.h>
		#include <stdbool.h>
		
		#include "../../../Common/Common.h"
		#include "../LowLevel/LowLevel.h"
		#include "USBMode.h"
		#include "Events.h"
		
	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include LUFA/Drivers/USB.h instead.
		#endif
		
	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			#define USB_INT_Enable(int)			MACROS{ USB_INT_GET_EN_REG(int)   |=   USB_INT_GET_EN_MASK(int);   }MACROE
			#define USB_INT_Disable(int)			MACROS{ USB_INT_GET_EN_REG(int)   &= ~(USB_INT_GET_EN_MASK(int));  }MACROE
			#define USB_INT_Clear(int)			MACROS{ USB_INT_GET_INT_CLR_REG(int)  &= ~(USB_INT_GET_INT_MASK(int)); }MACROE
			#define USB_INT_IsEnabled(int)		((USB_INT_GET_EN_REG(int)   &    USB_INT_GET_EN_MASK(int)) ? true : false)
			#define USB_INT_HasOccurred(int)	((USB_INT_GET_INT_REG(int)  &    USB_INT_GET_INT_MASK(int)) ? true : false)

			#define USB_INT_GET_EN_REG(a, b, c, d, e)		a
			#define USB_INT_GET_EN_MASK(a, b, c, d, e)		b
			#define USB_INT_GET_INT_REG(a, b, c, d, e)		c
			#define USB_INT_GET_INT_MASK(a, b, c, d, e)	d
			#define USB_INT_GET_INT_CLR_REG(a, b, c, d, e)	e

			// Enable Register, Enable Mask, Interrupt Register, Interrupt Mask

			// AVR VBUSTE - VBUS transition
			#define USB_INT_VBUS				(LPC_USB->USBDevIntEn), 0, (LPC_USB->USBDevIntSt), 0, (LPC_USB->USBDevIntClr)
			// AVR IDTE - ID Transition = no LPC equivalent (alter functions which use this)
			#define USB_INT_IDTI				(LPC_USB->USBDevIntEn), 0, (LPC_USB->USBDevIntSt), 0, (LPC_USB->USBDevIntClr)
			// AVR WAKEUPE - Wake Up CPU = no LPC equivalent (alter functions which use this)
			#define USB_INT_WAKEUP				(LPC_USB->USBDevIntEn), 0, (LPC_USB->USBDevIntSt), 0, (LPC_USB->USBDevIntClr)
			// AVR SUSPE - Suspend = LPC Device Status Change (Reset, Suspend, Connect), USBDevIntEn, USBDevIntSt, DEV_STAT=3
			#define USB_INT_SUSPEND				(LPC_USB->USBDevIntEn) ,	(1 << 3) , (LPC_USB->USBDevIntSt) , (1 << 3), (LPC_USB->USBDevIntClr)
			// AVR EORSTE - End of RESET = LPC Device Status Change (Reset, Suspend, Connect), USBDevIntEn, USBDevIntSt, DEV_STAT=3
			#define USB_INT_EORSTI				(LPC_USB->USBDevIntEn) ,	(1 << 3) , (LPC_USB->USBDevIntSt) , (1 << 3), (LPC_USB->USBDevIntClr)
			// AVR SOFE - Start of Frame = no LPC equivalent (alter functions which use this)
			#define USB_INT_SOFI				(LPC_USB->USBDevIntEn), 0, (LPC_USB->USBDevIntSt), 0, (LPC_USB->USBDevIntClr)
			// AVR RXSTPE - received SETUP packet = no LPC equivalent (alter functions which use this)
			#define USB_INT_RXSTPI				(LPC_USB->USBDevIntEn), 0, (LPC_USB->USBDevIntSt), 0, (LPC_USB->USBDevIntClr)

// TODO - define the following for the LPC for HOST and OTG modes
//			#define USB_INT_BCERRI				OTGIEN,	(1 << BCERRE) , OTGINT, (1 << BCERRI)
//			#define USB_INT_VBERRI				OTGIEN,	(1 << VBERRE) , OTGINT, (1 << VBERRI)
//			#define USB_INT_DDISCI				UHIEN ,	(1 << DDISCE) , UHINT , (1 << DDISCI)
//			#define USB_INT_SRPI				OTGIEN,	(1 << SRPE)   , OTGINT, (1 << SRPI)
//			#define USB_INT_DCONNI				UHIEN ,	(1 << DCONNE) , UHINT , (1 << DCONNI)
//			#define USB_INT_HSOFI				UHIEN,	(1 << HSOFE)  , UHINT , (1 << HSOFI)
//			#define USB_INT_RSTI				UHIEN,	(1 << RSTE)   , UHINT , (1 << RSTI)
	
		/* Function Prototypes: */
			void USB_INT_ClearAllInterrupts(void);
			void USB_INT_DisableAllInterrupts(void);
	#endif
	
	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif
