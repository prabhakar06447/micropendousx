/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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

#define  __INCLUDE_FROM_USB_DRIVER
#include "../HighLevel/USBMode.h"

#if defined(USB_CAN_BE_DEVICE)

#define  __INCLUDE_FROM_ENDPOINT_C
#include "Endpoint.h"


volatile uint8_t _endpoinReadByteIndex = 0;
volatile uint32_t _endpointReadTempData = 0;
volatile uint8_t _endpoinWriteByteIndex = 0;
volatile uint32_t _endpointWriteTempData = 0;


#if !defined(FIXED_CONTROL_ENDPOINT_SIZE)
uint8_t USB_ControlEndpointSize = ENDPOINT_CONTROLEP_DEFAULT_SIZE;
#endif

uint8_t Endpoint_BytesToEPSizeMaskDynamic(const uint16_t Size)
{
	return Endpoint_BytesToEPSizeMask(Size);
}

bool Endpoint_ConfigureEndpoint_Prv(const uint8_t Number, const uint8_t UECFG0XData, const uint8_t UECFG1XData)
{
#warning TODO - Endpoint.c - CRITICAL - properly enable EPs - TODO

	Endpoint_SelectEndpoint(Number);
	Endpoint_EnableEndpoint();

	// TODO - allocate EP size and select number of banks
//	UECFG1X = 0;
//	UECFG0X = UECFG0XData;
//	UECFG1X = UECFG1XData;

	return Endpoint_IsConfigured();
}

void Endpoint_ClearEndpoints(void)
{
//	UEINT = 0;	// clear device interrupts

	for (uint8_t EPNum = 0; EPNum < ENDPOINT_TOTAL_ENDPOINTS; EPNum++)
	{
		Endpoint_SelectEndpoint(EPNum);	

	// TODO - clear all EP-related interrupts
#warning TODO - Endpoint.c - clear all EP-related interrupts - TODO
//		UEIENX = 0;
//		UEINTX = 0;

//		Endpoint_DeallocateMemory(); // no LPC equivalent
		Endpoint_DisableEndpoint();
	}
}

void Endpoint_ClearStatusStage(void)
{
	if (USB_ControlRequest.bmRequestType & REQDIR_DEVICETOHOST)
	{
		while (!(Endpoint_IsOUTReceived()))
		{
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			  return;
		}

		Endpoint_ClearOUT();
	}
	else
	{
		while (!(Endpoint_IsINReady()))
		{
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			  return;
		}
		
		Endpoint_ClearIN();
	}
}

#if !defined(CONTROL_ONLY_DEVICE)
uint8_t Endpoint_WaitUntilReady(void)
{
	#if (USB_STREAM_TIMEOUT_MS < 0xFF)
	uint8_t  TimeoutMSRem = USB_STREAM_TIMEOUT_MS;	
	#else
	uint16_t TimeoutMSRem = USB_STREAM_TIMEOUT_MS;
	#endif

	for (;;)
	{
		if (Endpoint_GetEndpointDirection())
		{
			if (Endpoint_IsINReady())
			  return ENDPOINT_READYWAIT_NoError;
		}
		else
		{
			if (Endpoint_IsOUTReceived())
			  return ENDPOINT_READYWAIT_NoError;		
		}
		
		if (USB_DeviceState == DEVICE_STATE_Unattached)
		  return ENDPOINT_READYWAIT_DeviceDisconnected;
		else if (Endpoint_IsStalled())
		  return ENDPOINT_READYWAIT_EndpointStalled;

		// check if this is Start-Of-Frame (SOF)
		// on LPC need to check if current Frame number has changed in SIE
		#warning TODO - Endpoint.c - account for start of frame - TODO
		_LUFAUSBFrameNumber = 0;
// LPC has no Start-Of-Frame Interrupt
/*
		if (USB_INT_HasOccurred(USB_INT_SOFI))
		{
			USB_INT_Clear(USB_INT_SOFI);

			if (!(TimeoutMSRem--))
			  return ENDPOINT_READYWAIT_Timeout;
		}
*/
	}
}

uint8_t Endpoint_Discard_Stream(uint16_t Length
#if !defined(NO_STREAM_CALLBACKS)
                                , StreamCallbackPtr_t Callback
#endif
								)
{
	uint8_t  ErrorCode;
	
	if ((ErrorCode = Endpoint_WaitUntilReady()))
	  return ErrorCode;

	#if defined(FAST_STREAM_TRANSFERS)
	uint8_t BytesRemToAlignment = (Endpoint_BytesInEndpoint() & 0x07);

	if (Length >= 8)
	{
		Length -= BytesRemToAlignment;

		switch (BytesRemToAlignment)
		{
			default:
				do
				{
					if (!(Endpoint_IsReadWriteAllowed()))
					{
						Endpoint_ClearOUT();

						#if !defined(NO_STREAM_CALLBACKS)
						if ((Callback != NULL) && (Callback() == STREAMCALLBACK_Abort))
						  return ENDPOINT_RWSTREAM_CallbackAborted;
						#endif

						if ((ErrorCode = Endpoint_WaitUntilReady()))
						  return ErrorCode;
					}

					Length -= 8;
					
					Endpoint_Discard_Byte();
			case 7: Endpoint_Discard_Byte();
			case 6: Endpoint_Discard_Byte();
			case 5: Endpoint_Discard_Byte();
			case 4: Endpoint_Discard_Byte();
			case 3: Endpoint_Discard_Byte();
			case 2: Endpoint_Discard_Byte();
			case 1:	Endpoint_Discard_Byte();
				} while (Length >= 8);	
		}
	}
	#endif

	while (Length)
	{
		if (!(Endpoint_IsReadWriteAllowed()))
		{
			Endpoint_ClearOUT();

			#if !defined(NO_STREAM_CALLBACKS)
			if ((Callback != NULL) && (Callback() == STREAMCALLBACK_Abort))
			  return ENDPOINT_RWSTREAM_CallbackAborted;
			#endif

			if ((ErrorCode = Endpoint_WaitUntilReady()))
			  return ErrorCode;
		}
		else
		{
			Endpoint_Discard_Byte();
			Length--;
		}
	}
	
	return ENDPOINT_RWSTREAM_NoError;
}

/* The following abuses the C preprocessor in order to copy-past common code with slight alterations,
 * so that the code needs to be written once. It is a crude form of templating to reduce code maintenance. */


// Memory functions redefined for ARM Cortex

uint8_t pgm_read_byte(uint8_t *addr);
uint8_t pgm_read_byte(uint8_t *addr) { return (uint8_t)(*addr); }

uint8_t eeprom_read_byte(uint8_t *__p);
uint8_t eeprom_read_byte(uint8_t *__p) { return (uint8_t)(*__p); }

void eeprom_write_byte(uint8_t *__p, uint8_t __value);
void eeprom_write_byte(uint8_t *__p, uint8_t __value) { *__p = __value; }



#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Stream_LE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(*((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_PStream_LE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(pgm_read_byte((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_EStream_LE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(eeprom_read_byte((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Stream_BE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(*((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_EStream_BE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(eeprom_read_byte((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_PStream_BE
#define  TEMPLATE_BUFFER_TYPE                      const void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearIN()
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(pgm_read_byte((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Stream_LE
#define  TEMPLATE_BUFFER_TYPE                      void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearOUT()
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         *((uint8_t*)BufferPtr++) = Endpoint_Read_Byte()
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_EStream_LE
#define  TEMPLATE_BUFFER_TYPE                      void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearOUT()
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         eeprom_write_byte((uint8_t*)BufferPtr++, Endpoint_Read_Byte())
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Stream_BE
#define  TEMPLATE_BUFFER_TYPE                      void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearOUT()
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         *((uint8_t*)BufferPtr--) = Endpoint_Read_Byte()
#include "Template/Template_Endpoint_RW.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_EStream_BE
#define  TEMPLATE_BUFFER_TYPE                      void*
#define  TEMPLATE_CLEAR_ENDPOINT()                 Endpoint_ClearOUT()
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         eeprom_write_byte((uint8_t*)BufferPtr--, Endpoint_Read_Byte())
#include "Template/Template_Endpoint_RW.c"

#endif

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_Stream_LE
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(*((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_PStream_LE
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(pgm_read_byte((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_EStream_LE
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(eeprom_read_byte((uint8_t*)BufferPtr++))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_Stream_BE
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(*((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_PStream_BE
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(pgm_read_byte((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Write_Control_EStream_BE
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         Endpoint_Write_Byte(eeprom_read_byte((uint8_t*)BufferPtr--))
#include "Template/Template_Endpoint_Control_W.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Control_Stream_LE
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         *((uint8_t*)BufferPtr++) = Endpoint_Read_Byte()
#include "Template/Template_Endpoint_Control_R.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Control_EStream_LE
#define  TEMPLATE_BUFFER_OFFSET(Length)            0
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         eeprom_write_byte((uint8_t*)BufferPtr++, Endpoint_Read_Byte())
#include "Template/Template_Endpoint_Control_R.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Control_Stream_BE
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         *((uint8_t*)BufferPtr--) = Endpoint_Read_Byte()
#include "Template/Template_Endpoint_Control_R.c"

#define  TEMPLATE_FUNC_NAME                        Endpoint_Read_Control_EStream_BE
#define  TEMPLATE_BUFFER_OFFSET(Length)            (Length - 1)
#define  TEMPLATE_TRANSFER_BYTE(BufferPtr)         eeprom_write_byte((uint8_t*)BufferPtr--, Endpoint_Read_Byte())
#include "Template/Template_Endpoint_Control_R.c"

#endif
