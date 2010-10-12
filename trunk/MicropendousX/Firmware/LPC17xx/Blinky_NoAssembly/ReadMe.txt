Blinky
~~~~~~

This example project uses CMSIS and demonstrates 
the use of Systick in interrupt driven mode
Blinky_NoAssembly uses C-based instead of
assembly-based startup routines. Take a look at
LPC17xx_Interrupts.c and LPC17xx_Linker_Script.ld

Board:  Any LPC17xx-based board with an LED controlled by
        the USB_UP_LED/P1.18 pin.
Device: LPC17xx

Example functionality:                                                   
 - Clock Settings:
   - XTAL      =     12.00 MHz
   - SYSCLK    =    100.00 MHz

The example program runs from Internal on-chip FLASH


-- CMSIS ---------------------------------------------
SystemInit       called from startup_LPC17xx.s
SystemCoreClock  is default 100000000Hz
SysTick          runs in interrupt mode


Usage:
1) Connect an OpenOCD-compatible JTAG to your board
2) Edit "interface" settings in OpenOCD.cfg accordingly
3) run  make  to compile
4) run  make program  to program via OpenOCD
5) In one terminal run 'openocd OpenOCD.cfg' and in
   another terminal run  make debug  to debug using GDB
