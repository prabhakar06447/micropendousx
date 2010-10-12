/**
 * @file	: rtc_alarm_cntincr_int.c
 * @purpose	: A simple RTC example.
 * 			To generate interrupt in Second Counter Increment Interrupt (1s)
 *          and generate alarm interrupt at 10s
 * @version	: 1.0
 * @date	: 23. April. 2009
 * @author	: HieuNguyen
 *----------------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/
#include "lpc17xx_rtc.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"
#include "debug_frmwrk.h"


/************************** PRIVATE TYPES *************************/

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" RTC demo \n\r"
"\t - MCU: LPC17xx \n\r"
"\t - Core: ARM CORTEX-M3 \n\r"
"\t - Communicate via: UART0 - 115200 bps \n\r"
" A simple RTC example. \n\r"
" To generate interrupt in Second Counter Increment Interrupt (1s) \n\r"
" and generate Alarm interrupt at 10s \n\r"
"********************************************************************************\n\r";

// Toggle flag
__IO uint32_t toggle = 0;


/************************** PRIVATE FUNCTION *************************/
void RTC_IRQHandler(void);

/******************************************************************//**
 * @brief 		Main RTC interrupt sub-routine handler
 * @param[in]	None
 * @return		None
 *******************************************************************/
void RTC_IRQHandler(void)
{
	uint32_t secval;

	/* This is increment counter interrupt*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
	{
		if(toggle == 1)
		{
			toggle = 0;
		}else{
			toggle = 1;
		}

		secval = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_SECOND);

		/* Send debug information */
		_DBG ("Second: "); _DBD(secval);
		_DBG_("");

		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
	}

	/* Continue to check the Alarm match*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
	{
		/* Send debug information */
		_DBG_ ("ALARM 10s matched!");

		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
	}
}

// Print Menu 1
void print_menu(void)
{
	_DBG(menu1);
}


/*********************************************************************//**
 * @brief	Main RTC program body
 **********************************************************************/
int c_entry(void)
{
	RTC_TIME_Type RTCFullTime;

/* Configure the NVIC Preemption Priority Bits:
 * two (2) bits of preemption priority, six (6) bits of sub-priority.
 * Since the Number of Bits used for Priority Levels is five (5), so the
 * actual bit number of sub-priority is three (3)
 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif

	/* Initialize debug */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	/* RTC Block section ------------------------------------------------------ */
	// Init RTC module
	RTC_Init(LPC_RTC);

    /* Disable RTC interrupt */
    NVIC_DisableIRQ(RTC_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(RTC_IRQn, ((0x01<<3)|0x01));

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
	RTC_CalibCounterCmd(LPC_RTC, DISABLE);

	/* Set current time for RTC */
	// Current time is 8:00:00PM, 2009-04-24
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 20);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 4);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2009);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 24);

	/* Set ALARM time for second */
	RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 10);

	// Get and print current time
	RTC_GetFullTime (LPC_RTC, &RTCFullTime);
	_DBG( "Current time set to: ");
	_DBD((RTCFullTime.HOUR)); _DBG (":");
	_DBD ((RTCFullTime.MIN)); _DBG (":");
	_DBD ((RTCFullTime.SEC)); _DBG("  ");
	_DBD ((RTCFullTime.DOM)); _DBG("/");
	_DBD ((RTCFullTime.MONTH)); _DBG("/");
	_DBD16 ((RTCFullTime.YEAR)); _DBG_("");

	_DBG("Second ALARM set to ");
	_DBD (RTC_GetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND));
	_DBG_("s");

	/* Set the CIIR for second counter interrupt*/
	RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
	/* Set the AMR for 10s match alarm interrupt */
	RTC_AlarmIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

    /* Enable RTC interrupt */
    NVIC_EnableIRQ(RTC_IRQn);

    /* Loop forever */
    while(1);
    return 1;
}



/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}


#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

