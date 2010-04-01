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

/** \ingroup Group_USB
 *  @defgroup Group_EndpointManagement Endpoint Management
 *
 *  Functions, macros and enums related to endpoint management when in USB Device mode. This
 *  module contains the endpoint management macros, as well as endpoint interrupt and data
 *  send/receive functions for various data types.
 *
 *  @{
 */

/** @defgroup Group_EndpointRW Endpoint Data Reading and Writing
 *
 *  Functions, macros, variables, enums and types related to data reading and writing from and to endpoints.
 */
 
/** \ingroup Group_EndpointRW  
 *  @defgroup Group_EndpointPrimitiveRW Read/Write of Primitive Data Types
 *
 *  Functions, macros, variables, enums and types related to data reading and writing of primitive data types
 *  from and to endpoints.
 */ 

/** \ingroup Group_EndpointRW  
 *  @defgroup Group_EndpointStreamRW Read/Write of Multi-Byte Streams
 *
 *  Functions, macros, variables, enums and types related to data reading and writing of data streams from
 *  and to endpoints.
 */ 

/** @defgroup Group_EndpointPacketManagement Endpoint Packet Management
 *
 *  Functions, macros, variables, enums and types related to packet management of endpoints.
 */

#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__


	/* Includes: */
		#include <LPC17xx.h>
		#include <stdbool.h>

		#include "../../../Common/Common.h"
		#include "../HighLevel/USBTask.h"
		#include "Device.h"

		#if !defined(NO_STREAM_CALLBACKS) || defined(__DOXYGEN__)
			#include "../HighLevel/StreamCallbacks.h"
		#endif
		
	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include LUFA/Drivers/USB.h instead.
		#endif
		
	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Endpoint data direction mask for \ref Endpoint_ConfigureEndpoint(). This indicates that the endpoint
			 *  should be initialized in the OUT direction - i.e. data flows from host to device.
			 */
			#define ENDPOINT_DIR_OUT                      0

			/** Endpoint data direction mask for \ref Endpoint_ConfigureEndpoint(). This indicates that the endpoint
			 *  should be initialized in the IN direction - i.e. data flows from device to host.
			 */
			#define ENDPOINT_DIR_IN                       1

			/** Mask for the bank mode selection for the \ref Endpoint_ConfigureEndpoint() macro. This indicates
			 *  that the endpoint should have one single bank, which requires less USB FIFO memory but results
			 *  in slower transfers as only one USB device (the AVR or the host) can access the endpoint's
			 *  bank at the one time.
			 */
			#define ENDPOINT_BANK_SINGLE                  0

			/** Mask for the bank mode selection for the \ref Endpoint_ConfigureEndpoint() macro. This indicates
			 *  that the endpoint should have two banks, which requires more USB FIFO memory but results
			 *  in faster transfers as one USB device (the AVR or the host) can access one bank while the other
			 *  accesses the second bank.
			 */
			#define ENDPOINT_BANK_DOUBLE                  1
			
			/** Endpoint address for the default control endpoint, which always resides in address 0. This is
			 *  defined for convenience to give more readable code when used with the endpoint macros.
			 */
			#define ENDPOINT_CONTROLEP                    0

			#if (!defined(FIXED_CONTROL_ENDPOINT_SIZE) || defined(__DOXYGEN__))
				/** Default size of the default control endpoint's bank, until altered by the Endpoint0Size value 
				 *  in the device descriptor. Not available if the FIXED_CONTROL_ENDPOINT_SIZE token is defined.
				 */
				#define ENDPOINT_CONTROLEP_DEFAULT_SIZE   8
			#endif
			
			/** Endpoint number mask, for masking against endpoint addresses to retrieve the endpoint's
			 *  numerical address in the device.
			 */
			#define ENDPOINT_EPNUM_MASK                   0x07

			/** Endpoint direction mask, for masking against endpoint addresses to retrieve the endpoint's
			 *  direction for comparing with the ENDPOINT_DESCRIPTOR_DIR_* masks.
			 */
			#define ENDPOINT_EPDIR_MASK                   0x80

			/** Endpoint bank size mask, for masking against endpoint addresses to retrieve the endpoint's
			 *  bank size in the device.
			 */
			#define ENDPOINT_EPSIZE_MASK                  0x7FF
			
			/** Maximum size in bytes of a given endpoint.
			 *
			 *  \param[in] n  Endpoint number, a value between 0 and (ENDPOINT_TOTAL_ENDPOINTS - 1)
			 */				
			#define ENDPOINT_MAX_SIZE(n)                  _ENDPOINT_GET_MAXSIZE(n)

			/** Indicates if the given endpoint supports double banking.
			 *
			 *  \param[in] n  Endpoint number, a value between 0 and (ENDPOINT_TOTAL_ENDPOINTS - 1)
			 */				
			#define ENDPOINT_DOUBLEBANK_SUPPORTED(n)      _ENDPOINT_GET_DOUBLEBANK(n)

			#if !defined(CONTROL_ONLY_DEVICE)
				#if defined(USB_SERIES_4_AVR) || defined(USB_SERIES_6_AVR) || defined(USB_SERIES_7_AVR) || defined(__DOXYGEN__)
					/** Total number of endpoints (including the default control endpoint at address 0) which may
					 *  be used in the device. Different USB AVR models support different amounts of endpoints,
					 *  this value reflects the maximum number of endpoints for the currently selected AVR model.
					 */
					#define ENDPOINT_TOTAL_ENDPOINTS          7
				#else
					#define ENDPOINT_TOTAL_ENDPOINTS          5			
				#endif
			#else
				#define ENDPOINT_TOTAL_ENDPOINTS              1
			#endif
			
		/* Pseudo-Function Macros: */
			#if defined(__DOXYGEN__)
				/** Indicates the number of bytes currently stored in the current endpoint's selected bank.
				 *
				 *  \note The return width of this function may differ, depending on the maximum endpoint bank size
				 *        of the selected AVR model.
				 *
				 *  \ingroup Group_EndpointRW
				 *
				 *  \return Total number of bytes in the currently selected Endpoint's FIFO buffer
				 */
				static inline uint16_t Endpoint_BytesInEndpoint(void);
			
				/** Get the endpoint address of the currently selected endpoint. This is typically used to save
				 *  the currently selected endpoint number so that it can be restored after another endpoint has
				 *  been manipulated.
				 *
				 *  \return Index of the currently selected endpoint
				 */
				static inline uint8_t Endpoint_GetCurrentEndpoint(void);
				
				/** Selects the given endpoint number. If the address from the device descriptors is used, the
				 *  value should be masked with the \ref ENDPOINT_EPNUM_MASK constant to extract only the endpoint
				 *  number (and discarding the endpoint direction bit).
				 *
				 *  Any endpoint operations which do not require the endpoint number to be indicated will operate on
				 *  the currently selected endpoint.
				 *
				 *  \param[in] EndpointNumber Endpoint number to select
				 */
				static inline void Endpoint_SelectEndpoint(uint8_t EndpointNumber);
				
				/** Resets the endpoint bank FIFO. This clears all the endpoint banks and resets the USB controller's
				 *  In and Out pointers to the bank's contents.
				 *
				 *  \param[in] EndpointNumber Endpoint number whose FIFO buffers are to be reset
				 */
				static inline void Endpoint_ResetFIFO(uint8_t EndpointNumber);
				
				/** Enables the currently selected endpoint so that data can be sent and received through it to
				 *  and from a host.
				 *
				 *  \note Endpoints must first be configured properly via \ref Endpoint_ConfigureEndpoint().
				 */
				static inline void Endpoint_EnableEndpoint(void);

				/** Disables the currently selected endpoint so that data cannot be sent and received through it
				 *  to and from a host.
				 */
				static inline void Endpoint_DisableEndpoint(void);
				
				/** Determines if the currently selected endpoint is enabled, but not necessarily configured.
				 *
				 * \return Boolean True if the currently selected endpoint is enabled, false otherwise
				 */
				static inline bool Endpoint_IsEnabled(void);
				
				/** Determines if the currently selected endpoint may be read from (if data is waiting in the endpoint
				 *  bank and the endpoint is an OUT direction, or if the bank is not yet full if the endpoint is an IN
				 *  direction). This function will return false if an error has occurred in the endpoint, if the endpoint
				 *  is an OUT direction and no packet (or an empty packet) has been received, or if the endpoint is an IN
				 *  direction and the endpoint bank is full.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \return Boolean true if the currently selected endpoint may be read from or written to, depending on its direction
				 */
				static inline bool Endpoint_IsReadWriteAllowed(void);
				
				/** Determines if the currently selected endpoint is configured.
				 *
				 *  \return Boolean true if the currently selected endpoint has been configured, false otherwise
				 */
				static inline bool Endpoint_IsConfigured(void);
				
				/** Returns a mask indicating which INTERRUPT type endpoints have interrupted - i.e. their
				 *  interrupt duration has elapsed. Which endpoints have interrupted can be determined by
				 *  masking the return value against (1 << {Endpoint Number}).
				 *
				 *  \return Mask whose bits indicate which endpoints have interrupted
				 */
				static inline uint8_t Endpoint_GetEndpointInterrupts(void);
				
				/** Determines if the specified endpoint number has interrupted (valid only for INTERRUPT type
				 *  endpoints).
				 *
				 *  \param[in] EndpointNumber  Index of the endpoint whose interrupt flag should be tested
				 *
				 *  \return Boolean true if the specified endpoint has interrupted, false otherwise
				 */
				static inline bool Endpoint_HasEndpointInterrupted(uint8_t EndpointNumber);
				
				/** Determines if the selected IN endpoint is ready for a new packet.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \return Boolean true if the current endpoint is ready for an IN packet, false otherwise.
				 */
				static inline bool Endpoint_IsINReady(void);
				
				/** Determines if the selected OUT endpoint has received new packet.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \return Boolean true if current endpoint is has received an OUT packet, false otherwise.
				 */
				static inline bool Endpoint_IsOUTReceived(void);
				
				/** Determines if the current CONTROL type endpoint has received a SETUP packet.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \return Boolean true if the selected endpoint has received a SETUP packet, false otherwise.
				 */
				static inline bool Endpoint_IsSETUPReceived(void);
				
				/** Clears a received SETUP packet on the currently selected CONTROL type endpoint, freeing up the
				 *  endpoint for the next packet.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \note This is not applicable for non CONTROL type endpoints.			 
				 */
				static inline void Endpoint_ClearSETUP(void);
				
				/** Sends an IN packet to the host on the currently selected endpoint, freeing up the endpoint for the
				 *  next packet and switching to the alternative endpoint bank if double banked.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 */
				static inline void Endpoint_ClearIN(void);
				
				/** Acknowledges an OUT packet to the host on the currently selected endpoint, freeing up the endpoint
				 *  for the next packet and switching to the alternative endpoint bank if double banked.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 */
				static inline void Endpoint_ClearOUT(void);
				
				/** Stalls the current endpoint, indicating to the host that a logical problem occurred with the
				 *  indicated endpoint and that the current transfer sequence should be aborted. This provides a
				 *  way for devices to indicate invalid commands to the host so that the current transfer can be
				 *  aborted and the host can begin its own recovery sequence.
				 *
				 *  The currently selected endpoint remains stalled until either the \ref Endpoint_ClearStall() macro
				 *  is called, or the host issues a CLEAR FEATURE request to the device for the currently selected
				 *  endpoint.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 */
				static inline void Endpoint_StallTransaction(void);
				
				/** Clears the STALL condition on the currently selected endpoint.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 */
				static inline void Endpoint_ClearStall(void);
				
				/** Determines if the currently selected endpoint is stalled, false otherwise.
				 *
				 *  \ingroup Group_EndpointPacketManagement
				 *
				 *  \return Boolean true if the currently selected endpoint is stalled, false otherwise
				 */
				static inline bool Endpoint_IsStalled(void);
				
				/** Resets the data toggle of the currently selected endpoint. */
				static inline void Endpoint_ResetDataToggle(void);
				
				/** Determines the currently selected endpoint's direction.
				 *
				 *  \return The currently selected endpoint's direction, as a ENDPOINT_DIR_* mask.
				 */
				static inline uint8_t Endpoint_GetEndpointDirection(void);

				/** Sets the direction of the currently selected endpoint.
				 *
				 *  \param[in] DirectionMask  New endpoint direction, as a ENDPOINT_DIR_* mask.
				 */
				static inline void Endpoint_SetEndpointDirection(uint8_t DirectionMask);

			#else // NOT if defined(__DOXYGEN__)


				// helper functions for running SIE commands, each consisting of a Command followed by 1 or more possibly optional read/write commands

				// Issue a SIE command with no/ignored data phase
				//USBDevIntClr = 0x30; // Clear both CCEMPTY & CDFULL
				//USBCmdCode = 0x00_command_0500; // CMD_CODE=0x_command_, CMD_PHASE=0x05(Command)
				//while (!(USBDevIntSt & 0x10)); // Wait for CCEMPTY
				//USBDevIntClr = 0x10; // Clear CCEMPTY interrupt bit
				inline void SIE_Command(uint8_t command) ATTR_ALWAYS_INLINE;
				inline void SIE_Command(uint8_t command)
				{
					LPC_USB->USBDevIntClr = 0x30;
					LPC_USB->USBCmdCode = (uint32_t)(((uint32_t)0x00000500)  |  ((uint32_t)(command << 16)));
					while (!(LPC_USB->USBDevIntSt & 0x10));
					LPC_USB->USBDevIntClr = 0x10;
				}


				// Issue a read SIE command and return the read byte
				inline uint8_t SIE_Read(uint8_t command) ATTR_ALWAYS_INLINE;
				inline uint8_t SIE_Read(uint8_t command)
				{
					// CMD_CODE=0x_command_, CMD_PHASE=0x02(Read)
					LPC_USB->USBCmdCode = (uint32_t)(((uint32_t)0x00000200)  |  ((uint32_t)(command << 16)));
					while (!(LPC_USB->USBDevIntSt & 0x20)); // Wait for CDFULL
					LPC_USB->USBDevIntClr = 0x20; // Clear CDFULL
					return LPC_USB->USBCmdData; // Read byte
				}


				// Issue a write SIE command of data byte, should be preceded by a SIE_Command call with related data location
				//USBCmdCode = (uint32_t)(((uint32_t)0x00000100)  |  ((uint32_t)(byte << 16))); // CMD_WDATA=0x_byte_, CMD_PHASE=0x01(Write)
				//while (!(USBDevIntSt & 0x10)); // Wait for CCEMPTY
				//USBDevIntClr = 0x10; // Clear CCEMPTY
				static inline void SIE_Write(uint8_t byte);
				#define SIE_Write(byte)	\
												MACROS {	\
													LPC_USB->USBCmdCode = (uint32_t)(((uint32_t)0x00000100)  |  ((uint32_t)(byte << 16)));	\
													while (!(LPC_USB->USBDevIntSt & 0x10));	\
													LPC_USB->USBDevIntClr = 0x10;	\
												} MACROE


				// need to AND with 0x3FF as only last 9 bits are number of bytes in endpoint
				#define Endpoint_BytesInEndpoint()        (LPC_USB->USBRxPLen & ((uint32_t)0x000003FF))

				
				#if !defined(CONTROL_ONLY_DEVICE)
					// current endpoint number is in bits 5:2, 0x3C = 111100, but other bits are not EPnum-related
					#define Endpoint_GetCurrentEndpoint()     ((LPC_USB->USBCtrl & ((uint32_t)0x0000003C)) >> 2)
				#else
					#define Endpoint_GetCurrentEndpoint()     ENDPOINT_CONTROLEP
				#endif


				// get the current index of the endpoint number
				static inline uint8_t Endpoint_GetCurrentEndpointIndex(void) ATTR_ALWAYS_INLINE;
				static inline uint8_t Endpoint_GetCurrentEndpointIndex(void)
				{
					// want the index of the current endpoint, i.e., the "logical" endpoint, not the "physical" endpoint currently selected
					uint32_t currEP = ((LPC_USB->USBCtrl & ((uint32_t)0x0000003C)) >> 2);
					return (uint8_t)((currEP >> 1) + (currEP & 0x00000001));
				}



				#if !defined(CONTROL_ONLY_DEVICE)
					// current endpoint number is in bits 5:2 so shift left 2 places
					#define Endpoint_SelectEndpoint(epnum)    MACROS{ LPC_USB->USBCtrl |= (epnum << 2); }MACROE
				#else
					#define Endpoint_SelectEndpoint(epnum)    (void)(epnum)
				#endif

				// Clear Buffer is the closest Cortex SIE equivalent, see Pg242 and Pg of UM10360 (User Manual for LPC17xx)
				#define Endpoint_ResetFIFO(epnum)	\
												MACROS {	\
													SIE_Command((uint8_t) epnum);	\
													SIE_Command(0xF2);	\
												} MACROE

				// "Realize" the currently selected endpoint, enable its interrupt, and enable it in the SIE in Endpoint Status
				#define Endpoint_EnableEndpoint()	\
												MACROS {	\
													LPC_USB->USBEpIntEn |= (1 << ((uint8_t)Endpoint_GetCurrentEndpoint()));	\
													LPC_USB->USBReEp |= (1 << ((uint8_t)Endpoint_GetCurrentEndpoint()));	\
													SIE_Command((uint8_t) (Endpoint_GetCurrentEndpoint() + 0x40));	\
													SIE_Write(0x00);	\
												} MACROE

				// "un-Realize" the currently selected endpoint, disable its interrupt, and disable it in the SIE in Endpoint Status
				#define Endpoint_DisableEndpoint()	\
												MACROS {	\
													LPC_USB->USBEpIntEn &= ~(1 << ((uint8_t)Endpoint_GetCurrentEndpoint()));	\
													LPC_USB->USBReEp &= ~(1 << ((uint8_t)Endpoint_GetCurrentEndpoint()));	\
													SIE_Command((uint8_t) (Endpoint_GetCurrentEndpoint() + 0x40));	\
													SIE_Write(1 << 5);	\
												} MACROE

				// is the endpoint "Realized"
				#define Endpoint_IsEnabled()	(((uint32_t)LPC_USB->USBReEp) & ((uint32_t)(1 << ((uint8_t)Endpoint_GetCurrentEndpoint()))))

				// for Control EPs - yes, the EP is always writable
				#if !defined(CONTROL_ONLY_DEVICE)
					#define Endpoint_IsReadWriteAllowed()	true
				#endif

				// LPC equivalent would be that the endpoint interrupt is enabled and configured
				#define Endpoint_IsConfigured()               ((LPC_USB->USBEpIntEn) ? true : false)

				#define Endpoint_GetEndpointInterrupts()	(LPC_USB->USBEpIntEn)

				// Endpoint Interrupt Status Register - assuming logical endpoints
				// TODO - account for the fact that logical endpoints have different IN/OUT interrupts
				#define Endpoint_HasEndpointInterrupted(n)    ((LPC_USB->USBEpIntSt & ((uint32_t)(1 << n))) ? true : false)

				// LPC busy loops before writing data to IN buffer so always true
				#define Endpoint_IsINReady()                  (true)

				// closest LPC equivalent would be whether the current endpoint interrupted
				// TODO - account for the fact that logical endpoints have different IN/OUT interrupts
				#define Endpoint_IsOUTReceived()              (((LPC_USB->USBEpIntSt & ((uint32_t)(1 << Endpoint_GetCurrentEndpointIndex()))) ? true : false))

				static inline uint8_t Endpoint_IsSETUPReceived(void) ATTR_ALWAYS_INLINE;
				static inline uint8_t Endpoint_IsSETUPReceived(void)
				{
					// bit 2 in the select endpoint command is STP
					SIE_Command((uint8_t)(Endpoint_GetCurrentEndpoint()));
					return (((uint8_t)(SIE_Read(Endpoint_GetCurrentEndpoint()) & (1 << 2))) ? true : false);
				}

				// Select Endpoint/Clear interrupt command will clear the STP(Setup) and PO(Packet Overwritten - previous packet was overwritten) bits
				#define Endpoint_ClearSETUP()	\
												MACROS {	\
													SIE_Command((uint8_t)(Endpoint_GetCurrentEndpoint() + 0x40));	\
													SIE_Read(Endpoint_GetCurrentEndpoint() + 0x40);	\
												} MACROE

				// send final data byte(s) and then Validate the buffer via the SIE to inform hardware the data is ready to send
				// same procedure for Control and normal endpoints
				#if !defined(CONTROL_ONLY_DEVICE)
//					#define Endpoint_ClearIN()                MACROS{ uint8_t Temp = UEINTX; UEINTX = (Temp & ~(1 << TXINI)); \
//					                                                  UEINTX = (Temp & ~(1 << FIFOCON)); }MACROE
					#define Endpoint_ClearIN()	\
												MACROS {	\
													if (_endpoinWriteByteIndex != 0) { LPC_USB->USBTxData = _endpointWriteTempData;}	\
													SIE_Command((uint8_t)Endpoint_GetCurrentEndpoint());	\
													SIE_Command((uint8_t)0xFA);	\
												} MACROE
				#else
//					#define Endpoint_ClearIN()                MACROS{ UEINTX &= ~(1 << TXINI); }MACROE
					#define Endpoint_ClearIN()	\
												MACROS {	\
													if (_endpoinWriteByteIndex != 0){LPC_USB->USBTxData = _endpointWriteTempData;}	\
													SIE_Command((uint8_t)Endpoint_GetCurrentEndpoint());	\
													SIE_Command((uint8_t)0xFA);	\
												} MACROE
				#endif

				// no similar command for ClearOUT exists on LPC - data not read from buffer will be lost when new data arrives
				#if !defined(CONTROL_ONLY_DEVICE)
					#define Endpoint_ClearOUT()		__NOP();
				#else
					#define Endpoint_ClearOUT()		__NOP();
				#endif

				// Set Endpoint Status Command - bit 0 is Stall - setting to 1 stalls
				#define Endpoint_StallTransaction()	\
												MACROS {	\
													SIE_Command((uint8_t)(Endpoint_GetCurrentEndpoint() + 0x40));	\
													SIE_Write(1 << 0);	\
												} MACROE

				// Set Endpoint Status Command - bit 0 is Stall - setting to 0 unstalls
				#define Endpoint_ClearStall() \
												MACROS {	\
													SIE_Command((uint8_t)(Endpoint_GetCurrentEndpoint() + 0x40));	\
													SIE_Write(0 << 0);	\
												} MACROE


				static inline uint8_t Endpoint_IsStalled(void) ATTR_ALWAYS_INLINE;
				static inline uint8_t Endpoint_IsStalled(void)
				{
					// bit 1 in the select endpoint command is ST (Stall)
					SIE_Command((uint8_t)(Endpoint_GetCurrentEndpoint()));
					return (((uint8_t)(SIE_Read(Endpoint_GetCurrentEndpoint()) & (1 << 1))) ? true : false);
				}

				// TODO - the LPC has a toggle bit enable in the SIE and an error code for it but no way to toggle
				#define Endpoint_ResetDataToggle()		__NOP();

				// even# physical EPs are OUT, odd# physical EPs are IN
				#define Endpoint_GetEndpointDirection()			(Endpoint_GetCurrentEndpoint() & 0x01)

				// TODO - LPC does not explicitly set direction
				// 1 for IN, 0 for OUT - ENDPOINT_DIR_IN, ENDPOINT_DIR_OUT
				#define Endpoint_SetEndpointDirection(dir)		(dir & 0x00)
			#endif

		/* Enums: */
			/** Enum for the possible error return codes of the \ref Endpoint_WaitUntilReady() function.
			 *
			 *  \ingroup Group_EndpointRW
			 */
			enum Endpoint_WaitUntilReady_ErrorCodes_t
			{
				ENDPOINT_READYWAIT_NoError                 = 0, /**< Endpoint is ready for next packet, no error. */
				ENDPOINT_READYWAIT_EndpointStalled         = 1, /**< The endpoint was stalled during the stream
				                                                 *   transfer by the host or device.
				                                                 */
				ENDPOINT_READYWAIT_DeviceDisconnected      = 2,	/**< Device was disconnected from the host while
				                                                 *   waiting for the endpoint to become ready.
				                                                 */	
				ENDPOINT_READYWAIT_Timeout                 = 3, /**< The host failed to accept or send the next packet
				                                                 *   within the software timeout period set by the
				                                                 *   \ref USB_STREAM_TIMEOUT_MS macro.
				                                                 */
			};
		
			/** Enum for the possible error return codes of the Endpoint_*_Stream_* functions.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 */
			enum Endpoint_Stream_RW_ErrorCodes_t
			{
				ENDPOINT_RWSTREAM_NoError            = 0, /**< Command completed successfully, no error. */
				ENDPOINT_RWSTREAM_EndpointStalled    = 1, /**< The endpoint was stalled during the stream
				                                           *   transfer by the host or device.
				                                           */
				ENDPOINT_RWSTREAM_DeviceDisconnected = 2, /**< Device was disconnected from the host during
				                                           *   the transfer.
				                                           */
				ENDPOINT_RWSTREAM_Timeout            = 3, /**< The host failed to accept or send the next packet
				                                           *   within the software timeout period set by the
				                                           *   \ref USB_STREAM_TIMEOUT_MS macro.
				                                           */
				ENDPOINT_RWSTREAM_CallbackAborted    = 4, /**< Indicates that the stream's callback function
			                                               *   aborted the transfer early.
				                                           */
			};
			
			/** Enum for the possible error return codes of the Endpoint_*_Control_Stream_* functions..
			 *
			 *  \ingroup Group_EndpointStreamRW
			 */
			enum Endpoint_ControlStream_RW_ErrorCodes_t
			{
				ENDPOINT_RWCSTREAM_NoError            = 0, /**< Command completed successfully, no error. */
				ENDPOINT_RWCSTREAM_HostAborted        = 1, /**< The aborted the transfer prematurely. */
				ENDPOINT_RWCSTREAM_DeviceDisconnected = 2, /**< Device was disconnected from the host during
				                                            *   the transfer.
				                                            */
			};

		/* Inline Functions: */
			/** Reads one byte from the currently selected endpoint's bank, for OUT direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \return Next byte in the currently selected endpoint's FIFO buffer
			 */
extern volatile uint8_t _endpoinReadByteIndex;
extern volatile uint32_t _endpointReadTempData;
#define USBCtrl_RD_EN		0
			static inline uint8_t Endpoint_Read_Byte(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline uint8_t Endpoint_Read_Byte(void)
			{
				uint8_t temp = 0;

//				return UEDATX;
				// data is in little endian format

				if (_endpoinReadByteIndex == 0) {
					LPC_USB->USBCtrl |= (1 << USBCtrl_RD_EN);
					__NOP(); // need 3 clock cycles after setting USBCtrl
					_endpointReadTempData = LPC_USB->USBRxData;
					temp = (uint8_t)_endpointReadTempData;
					_endpoinReadByteIndex = 1;
				} else {
					temp = (uint8_t)(_endpointReadTempData << (8 * _endpoinReadByteIndex));
					_endpoinReadByteIndex++;
				}

				if (_endpoinReadByteIndex == 4) {
					_endpoinReadByteIndex = 0;
				}

				return temp;
			}

			/** Writes one byte from the currently selected endpoint's bank, for IN direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \param[in] Byte  Next byte to write into the the currently selected endpoint's FIFO buffer
			 */
#define USBCtrl_WR_EN		1
extern volatile uint8_t _endpoinWriteByteIndex;
extern volatile uint32_t _endpointWriteTempData;
			static inline void Endpoint_Write_Byte(const uint8_t Byte) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Write_Byte(const uint8_t Byte)
			{
//				UEDATX = Byte;
				// data is in little endian format (byte 0 at index 0 byte 1 at 1)
				// write the data every 4 bytes OR EndpointClearIN does remaining bytes before endpoint is sent

				if (_endpoinWriteByteIndex == 3) {
					_endpointWriteTempData |= (uint32_t)(Byte << (8 * _endpoinWriteByteIndex));
					LPC_USB->USBCtrl |= (1 << USBCtrl_WR_EN);
					__NOP(); // need 3 clock cycles after setting USBCtrl
					LPC_USB->USBTxData = _endpointWriteTempData;
					_endpoinWriteByteIndex = 0;
				} else {
					_endpointWriteTempData |= (uint32_t)(Byte << (8 * _endpoinWriteByteIndex));
					_endpoinWriteByteIndex++;
				}

			}

			/** Discards one byte from the currently selected endpoint's bank, for OUT direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 */
			static inline void Endpoint_Discard_Byte(void) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Discard_Byte(void)
			{
				uint8_t Dummy;
				
				Dummy = Endpoint_Read_Byte();
			}
			
			/** Reads two bytes from the currently selected endpoint's bank in little endian format, for OUT
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \return Next word in the currently selected endpoint's FIFO buffer
			 */
			static inline uint16_t Endpoint_Read_Word_LE(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline uint16_t Endpoint_Read_Word_LE(void)
			{
				union
				{
					uint16_t Word;
					uint8_t  Bytes[2];
				} Data;
				
				Data.Bytes[0] = Endpoint_Read_Byte();
				Data.Bytes[1] = Endpoint_Read_Byte();
			
				return Data.Word;
			}

			/** Reads two bytes from the currently selected endpoint's bank in big endian format, for OUT
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \return Next word in the currently selected endpoint's FIFO buffer
			 */
			static inline uint16_t Endpoint_Read_Word_BE(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline uint16_t Endpoint_Read_Word_BE(void)
			{
				union
				{
					uint16_t Word;
					uint8_t  Bytes[2];
				} Data;
				
				Data.Bytes[1] = Endpoint_Read_Byte();
				Data.Bytes[0] = Endpoint_Read_Byte();
			
				return Data.Word;
			}

			/** Writes two bytes to the currently selected endpoint's bank in little endian format, for IN
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \param[in] Word  Next word to write to the currently selected endpoint's FIFO buffer
			 */
			static inline void Endpoint_Write_Word_LE(const uint16_t Word) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Write_Word_LE(const uint16_t Word)
			{
				Endpoint_Write_Byte((Word & 0xFF));
				Endpoint_Write_Byte((Word >> 8));
			}
			
			/** Writes two bytes to the currently selected endpoint's bank in big endian format, for IN
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \param[in] Word  Next word to write to the currently selected endpoint's FIFO buffer
			 */
			static inline void Endpoint_Write_Word_BE(const uint16_t Word) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Write_Word_BE(const uint16_t Word)
			{
				Endpoint_Write_Byte((Word >> 8));
				Endpoint_Write_Byte((Word & 0xFF));
			}

			/** Discards two bytes from the currently selected endpoint's bank, for OUT direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 */
			static inline void Endpoint_Discard_Word(void) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Discard_Word(void)
			{
				uint8_t Dummy;
				
				Dummy = Endpoint_Read_Byte();
				Dummy = Endpoint_Read_Byte();
			}

			/** Reads four bytes from the currently selected endpoint's bank in little endian format, for OUT
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \return Next double word in the currently selected endpoint's FIFO buffer
			 */
			static inline uint32_t Endpoint_Read_DWord_LE(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline uint32_t Endpoint_Read_DWord_LE(void)
			{
				union
				{
					uint32_t DWord;
					uint8_t  Bytes[4];
				} Data;
				
				Data.Bytes[0] = Endpoint_Read_Byte();
				Data.Bytes[1] = Endpoint_Read_Byte();
				Data.Bytes[2] = Endpoint_Read_Byte();
				Data.Bytes[3] = Endpoint_Read_Byte();
			
				return Data.DWord;
			}

			/** Reads four bytes from the currently selected endpoint's bank in big endian format, for OUT
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \return Next double word in the currently selected endpoint's FIFO buffer
			 */
			static inline uint32_t Endpoint_Read_DWord_BE(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline uint32_t Endpoint_Read_DWord_BE(void)
			{
				union
				{
					uint32_t DWord;
					uint8_t  Bytes[4];
				} Data;
				
				Data.Bytes[3] = Endpoint_Read_Byte();
				Data.Bytes[2] = Endpoint_Read_Byte();
				Data.Bytes[1] = Endpoint_Read_Byte();
				Data.Bytes[0] = Endpoint_Read_Byte();
			
				return Data.DWord;
			}

			/** Writes four bytes to the currently selected endpoint's bank in little endian format, for IN
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \param[in] DWord  Next double word to write to the currently selected endpoint's FIFO buffer
			 */
			static inline void Endpoint_Write_DWord_LE(const uint32_t DWord) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Write_DWord_LE(const uint32_t DWord)
			{
				Endpoint_Write_Byte((DWord &  0xFF));
				Endpoint_Write_Byte((DWord >> 8));
				Endpoint_Write_Byte((DWord >> 16));
				Endpoint_Write_Byte((DWord >> 24));
			}
			
			/** Writes four bytes to the currently selected endpoint's bank in big endian format, for IN
			 *  direction endpoints.
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 *
			 *  \param[in] DWord  Next double word to write to the currently selected endpoint's FIFO buffer
			 */
			static inline void Endpoint_Write_DWord_BE(const uint32_t DWord) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Write_DWord_BE(const uint32_t DWord)
			{
				Endpoint_Write_Byte((DWord >> 24));
				Endpoint_Write_Byte((DWord >> 16));
				Endpoint_Write_Byte((DWord >> 8));
				Endpoint_Write_Byte((DWord &  0xFF));
			}

			/** Discards four bytes from the currently selected endpoint's bank, for OUT direction endpoints.	
			 *
			 *  \ingroup Group_EndpointPrimitiveRW
			 */
			static inline void Endpoint_Discard_DWord(void) ATTR_ALWAYS_INLINE;
			static inline void Endpoint_Discard_DWord(void)
			{
				uint8_t Dummy;
				
				Dummy = Endpoint_Read_Byte();
				Dummy = Endpoint_Read_Byte();
				Dummy = Endpoint_Read_Byte();
				Dummy = Endpoint_Read_Byte();
			}

		/* External Variables: */
			/** Global indicating the maximum packet size of the default control endpoint located at address
			 *  0 in the device. This value is set to the value indicated in the device descriptor in the user
			 *  project once the USB interface is initialized into device mode.
			 *
			 *  If space is an issue, it is possible to fix this to a static value by defining the control
			 *  endpoint size in the FIXED_CONTROL_ENDPOINT_SIZE token passed to the compiler in the makefile
			 *  via the -D switch. When a fixed control endpoint size is used, the size is no longer dynamically
			 *  read from the descriptors at runtime and instead fixed to the given value. When used, it is
			 *  important that the descriptor control endpoint size value matches the size given as the
			 *  FIXED_CONTROL_ENDPOINT_SIZE token - it is recommended that the FIXED_CONTROL_ENDPOINT_SIZE token
			 *  be used in the descriptors to ensure this.
			 *
			 *  \note This variable should be treated as read-only in the user application, and never manually
			 *        changed in value.
			 */
			#if (!defined(FIXED_CONTROL_ENDPOINT_SIZE) || defined(__DOXYGEN__))
				extern uint8_t USB_ControlEndpointSize;
			#else
				#define USB_ControlEndpointSize FIXED_CONTROL_ENDPOINT_SIZE
			#endif

		/* Function Prototypes: */
			#if !defined(NO_STREAM_CALLBACKS) || defined(__DOXYGEN__)
				#define __CALLBACK_PARAM     , StreamCallbackPtr_t Callback
			#else
				#define __CALLBACK_PARAM			
			#endif
		
			/** Configures the specified endpoint number with the given endpoint type, direction, bank size
			 *  and banking mode. Endpoints should be allocated in ascending order by their address in the
			 *  device (i.e. endpoint 1 should be configured before endpoint 2 and so on) to prevent fragmentation
			 *  of the USB FIFO memory.
			 *
			 *  The endpoint type may be one of the EP_TYPE_* macros listed in LowLevel.h and the direction
			 *  may be either \ref ENDPOINT_DIR_OUT or \ref ENDPOINT_DIR_IN.
			 *
			 *  The bank size must indicate the maximum packet size that the endpoint can handle. Different
			 *  endpoint numbers can handle different maximum packet sizes - refer to the chosen USB AVR's
			 *  datasheet to determine the maximum bank size for each endpoint.
			 *
			 *  The banking mode may be either \ref ENDPOINT_BANK_SINGLE or \ref ENDPOINT_BANK_DOUBLE.
			 *
			 *  \note The default control endpoint does not have to be manually configured, as it is automatically
			 *  configured by the library internally.
			 *
			 *  \note This routine will select the specified endpoint, and the endpoint will remain selected
			 *        once the routine completes regardless of if the endpoint configuration succeeds.
			 *
			 *  \return Boolean true if the configuration succeeded, false otherwise
			 */
			bool Endpoint_ConfigureEndpoint(const uint8_t  Number, const uint8_t Type, const uint8_t Direction,
			                                const uint16_t Size, const uint8_t Banks);

			/** Spin-loops until the currently selected non-control endpoint is ready for the next packet of data
			 *  to be read or written to it.
			 *
			 *  \note This routine should not be called on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointRW
			 *
			 *  \return A value from the \ref Endpoint_WaitUntilReady_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_WaitUntilReady(void);
			
			/** Completes the status stage of a control transfer on a CONTROL type endpoint automatically,
			 *  with respect to the data direction. This is a convenience function which can be used to
			 *  simplify user control request handling.
			 */
			void Endpoint_ClearStatusStage(void);

			/** Reads and discards the given number of bytes from the endpoint from the given buffer,
			 *  discarding fully read packets from the host as needed. The last packet is not automatically
			 *  discarded once the remaining bytes has been read; the user is responsible for manually
			 *  discarding the last packet from the host via the \ref Endpoint_ClearOUT() macro. Between
			 *  each USB packet, the given stream callback function is executed repeatedly until the next
			 *  packet is ready, allowing for early aborts of stream transfers.
			 *
			 *	The callback routine should be created according to the information in \ref Group_StreamCallbacks.
			 *  If the token NO_STREAM_CALLBACKS is passed via the -D option to the compiler, stream callbacks are
			 *  disabled and this function has the Callback parameter omitted.
			 *
			 *  \note This routine should not be used on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Length    Number of bytes to send via the currently selected endpoint.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Discard_Stream(uint16_t Length __CALLBACK_PARAM);

			/** Writes the given number of bytes to the endpoint from the given buffer in little endian,
			 *  sending full packets to the host as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Endpoint_ClearIN() macro. Between each USB packet, the given stream callback function
			 *  is executed repeatedly until the endpoint is ready to accept the next packet, allowing for early
			 *  aborts of stream transfers.
			 *
			 *	The callback routine should be created according to the information in \ref Group_StreamCallbacks.
			 *  If the token NO_STREAM_CALLBACKS is passed via the -D option to the compiler, stream callbacks are
			 *  disabled and this function has the Callback parameter omitted.
			 *
			 *  \note This routine should not be used on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Stream_LE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Write_Stream_LE().
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_EStream_LE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** FLASH buffer source version of \ref Endpoint_Write_Stream_LE().
			 *
			 *  \note The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_PStream_LE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** Writes the given number of bytes to the endpoint from the given buffer in big endian,
			 *  sending full packets to the host as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Endpoint_ClearIN() macro. Between each USB packet, the given stream callback function
			 *  is executed repeatedly until the endpoint is ready to accept the next packet, allowing for early
			 *  aborts of stream transfers.
			 *
			 *	The callback routine should be created according to the information in \ref Group_StreamCallbacks.
			 *  If the token NO_STREAM_CALLBACKS is passed via the -D option to the compiler, stream callbacks are
			 *  disabled and this function has the Callback parameter omitted.
			 *
			 *  \note This routine should not be used on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Stream_BE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Write_Stream_BE().
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_EStream_BE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** FLASH buffer source version of \ref Endpoint_Write_Stream_BE().
			 *
			 *  \note The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer    Pointer to the source data buffer to read from.
			 *  \param[in] Length    Number of bytes to read for the currently selected endpoint into the buffer.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_PStream_BE(const void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** Reads the given number of bytes from the endpoint from the given buffer in little endian,
			 *  discarding fully read packets from the host as needed. The last packet is not automatically
			 *  discarded once the remaining bytes has been read; the user is responsible for manually
			 *  discarding the last packet from the host via the \ref Endpoint_ClearOUT() macro. Between
			 *  each USB packet, the given stream callback function is executed repeatedly until the endpoint
			 *  is ready to accept the next packet, allowing for early aborts of stream transfers.
			 *
			 *	The callback routine should be created according to the information in \ref Group_StreamCallbacks.
			 *  If the token NO_STREAM_CALLBACKS is passed via the -D option to the compiler, stream callbacks are
			 *  disabled and this function has the Callback parameter omitted.
			 *
			 *  \note This routine should not be used on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer   Pointer to the destination data buffer to write to.
			 *  \param[in] Length    Number of bytes to send via the currently selected endpoint.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Stream_LE(void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Read_Stream_LE().
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer   Pointer to the destination data buffer to write to, located in EEPROM memory space.
			 *  \param[in] Length    Number of bytes to send via the currently selected endpoint.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_EStream_LE(void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);
			
			/** Reads the given number of bytes from the endpoint from the given buffer in big endian,
			 *  discarding fully read packets from the host as needed. The last packet is not automatically
			 *  discarded once the remaining bytes has been read; the user is responsible for manually
			 *  discarding the last packet from the host via the \ref Endpoint_ClearOUT() macro. Between
			 *  each USB packet, the given stream callback function is executed repeatedly until the endpoint
			 *  is ready to accept the next packet, allowing for early aborts of stream transfers.
			 *
			 *	The callback routine should be created according to the information in \ref Group_StreamCallbacks.
			 *  If the token NO_STREAM_CALLBACKS is passed via the -D option to the compiler, stream callbacks are
			 *  disabled and this function has the Callback parameter omitted.
			 *
			 *  \note This routine should not be used on CONTROL type endpoints.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer    Pointer to the destination data buffer to write to.
			 *  \param[in] Length    Number of bytes to send via the currently selected endpoint.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Stream_BE(void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Read_Stream_BE().
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer   Pointer to the destination data buffer to write to, located in EEPROM memory space.
			 *  \param[in] Length    Number of bytes to send via the currently selected endpoint.
			 *  \param[in] Callback  Name of a callback routine to call between successive USB packet transfers, NULL if no callback
			 *
			 *  \return A value from the \ref Endpoint_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_EStream_BE(void* Buffer, uint16_t Length __CALLBACK_PARAM) ATTR_NON_NULL_PTR_ARG(1);

			/** Writes the given number of bytes to the CONTROL type endpoint from the given buffer in little endian,
			 *  sending full packets to the host as needed. The host OUT acknowledgement is not automatically cleared
			 *  in both failure and success states; the user is responsible for manually clearing the setup OUT to
			 *  finalize the transfer via the \ref Endpoint_ClearOUT() macro.
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_Stream_LE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of Endpoint_Write_Control_Stream_LE.
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_EStream_LE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** FLASH buffer source version of \ref Endpoint_Write_Control_Stream_LE().
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_PStream_LE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** Writes the given number of bytes to the CONTROL type endpoint from the given buffer in big endian,
			 *  sending full packets to the host as needed. The host OUT acknowledgement is not automatically cleared
			 *  in both failure and success states; the user is responsible for manually clearing the setup OUT to
			 *  finalize the transfer via the \ref Endpoint_ClearOUT() macro.
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_Stream_BE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Write_Control_Stream_BE().
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_EStream_BE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** FLASH buffer source version of \ref Endpoint_Write_Control_Stream_BE().
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[in] Buffer  Pointer to the source data buffer to read from.
			 *  \param[in] Length  Number of bytes to read for the currently selected endpoint into the buffer.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Write_Control_PStream_BE(const void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** Reads the given number of bytes from the CONTROL endpoint from the given buffer in little endian,
			 *  discarding fully read packets from the host as needed. The device IN acknowledgement is not
			 *  automatically sent after success or failure states; the user is responsible for manually sending the
			 *  setup IN to finalize the transfer via the \ref Endpoint_ClearIN() macro.
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer  Pointer to the destination data buffer to write to.
			 *  \param[in] Length  Number of bytes to send via the currently selected endpoint.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Control_Stream_LE(void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** EEPROM buffer source version of \ref Endpoint_Read_Control_Stream_LE().
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer  Pointer to the destination data buffer to write to.
			 *  \param[in] Length  Number of bytes to send via the currently selected endpoint.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Control_EStream_LE(void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);

			/** Reads the given number of bytes from the CONTROL endpoint from the given buffer in big endian,
			 *  discarding fully read packets from the host as needed. The device IN acknowledgement is not
			 *  automatically sent after success or failure states; the user is responsible for manually sending the
			 *  setup IN to finalize the transfer via the \ref Endpoint_ClearIN() macro.
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer  Pointer to the destination data buffer to write to.
			 *  \param[in] Length  Number of bytes to send via the currently selected endpoint.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Control_Stream_BE(void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);		
			
			/** EEPROM buffer source version of \ref Endpoint_Read_Control_Stream_BE().
			 *
			 *  \note This function automatically clears the control transfer's status stage. Do not manually attempt
			 *        to clear the status stage when using this routine in a control transaction.
			 *
			 *  \note This routine should only be used on CONTROL type endpoints.
			 *
			 *  \warning Unlike the standard stream read/write commands, the control stream commands cannot be chained
			 *           together; i.e. the entire stream data must be read or written at the one time.
			 *
			 *  \ingroup Group_EndpointStreamRW
			 *
			 *  \param[out] Buffer  Pointer to the destination data buffer to write to.
			 *  \param[in] Length  Number of bytes to send via the currently selected endpoint.
			 *
			 *  \return A value from the \ref Endpoint_ControlStream_RW_ErrorCodes_t enum.
			 */
			uint8_t Endpoint_Read_Control_EStream_BE(void* Buffer, uint16_t Length) ATTR_NON_NULL_PTR_ARG(1);		

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
//			#define Endpoint_AllocateMemory()              MACROS{ UECFG1X |=  (1 << ALLOC); }MACROE
//			#define Endpoint_DeallocateMemory()            MACROS{ UECFG1X &= ~(1 << ALLOC); }MACROE
			
			#define _ENDPOINT_GET_MAXSIZE(n)               _ENDPOINT_GET_MAXSIZE2(ENDPOINT_DETAILS_EP ## n)
			#define _ENDPOINT_GET_MAXSIZE2(details)        _ENDPOINT_GET_MAXSIZE3(details)
			#define _ENDPOINT_GET_MAXSIZE3(maxsize, db)    maxsize

			#define _ENDPOINT_GET_DOUBLEBANK(n)            _ENDPOINT_GET_DOUBLEBANK2(ENDPOINT_DETAILS_EP ## n)
			#define _ENDPOINT_GET_DOUBLEBANK2(details)     _ENDPOINT_GET_DOUBLEBANK3(details)
			#define _ENDPOINT_GET_DOUBLEBANK3(maxsize, db) db
			
			#if defined(USB_SERIES_4_AVR) || defined(USB_SERIES_6_AVR) || defined(USB_SERIES_7_AVR)
				#define ENDPOINT_DETAILS_EP0               64,  true
				#define ENDPOINT_DETAILS_EP1               256, true
				#define ENDPOINT_DETAILS_EP2               64,  true
				#define ENDPOINT_DETAILS_EP3               64,  true
				#define ENDPOINT_DETAILS_EP4               64,  true
				#define ENDPOINT_DETAILS_EP5               64,  true
				#define ENDPOINT_DETAILS_EP6               64,  true
			#else
				#define ENDPOINT_DETAILS_EP0               64,  true
				#define ENDPOINT_DETAILS_EP1               64,  false
				#define ENDPOINT_DETAILS_EP2               64,  false
				#define ENDPOINT_DETAILS_EP3               64,  true
				#define ENDPOINT_DETAILS_EP4               64,  true			
			#endif

			#warning TODO - Endpoint.h - CRITICAL - properly enable EPs - TODO
			#define Endpoint_ConfigureEndpoint(Number, Type, Direction, Size, Banks)            \
			                                    Endpoint_ConfigureEndpoint_Prv(Number, Direction, Size)

		/* Function Prototypes: */
			void    Endpoint_ClearEndpoints(void);
			uint8_t Endpoint_BytesToEPSizeMaskDynamic(const uint16_t Size);
			bool    Endpoint_ConfigureEndpoint_Prv(const uint8_t Number, const uint8_t UECFG0XData, const uint8_t UECFG1XData);
			
		/* Inline Functions: */
			static inline uint8_t Endpoint_BytesToEPSizeMask(const uint16_t Bytes) ATTR_WARN_UNUSED_RESULT ATTR_CONST ATTR_ALWAYS_INLINE;
			static inline uint8_t Endpoint_BytesToEPSizeMask(const uint16_t Bytes)
			{
				uint8_t  MaskVal    = 0;
				uint16_t CheckBytes = 8;
				
				while (CheckBytes < Bytes)
				{
					MaskVal++;
					CheckBytes <<= 1;
				}
				
//				return (MaskVal << EPSIZE0);
// TODO - EPSIZE0 should be dynamic
				return (MaskVal);
			}

	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif
		
#endif

/** @} */
