
OpenOCD 0.4.0

1) Connect your OpenOCD JTAG Debugger to your target board
2) Inject the proper Vector Checksum into the .hex file using the
   "LPC2000 Flash Utility" from NXP.com
   This step can be omitted but OpenOCD will complain about errors
   and you will not know if the firmware loaded successfully
3) Run 'make program'


Running 'make program' will produce the following output:

C:\MicropendousX\Firmware\FreeRTOS_USBSerial_Ethernet_LPC17x8>make program

openocd -f OpenOCD.cfg -d0 -c init -c targets -c reset -c halt \
	-c "sleep 200" -c wait_halt -c "flash probe 0" -c "flash info 0" \
	-c "flash write_image erase unlock FreeRTOS_USB_Eth_Main.hex" \
	-c "verify_image FreeRTOS_USB_Eth_Main.hex" -c "sleep 200"
	-c "reset run" -c shutdown

Open On-Chip Debugger 0.4.0 (2010-02-22-19:05)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.berlios.de/doc/doxygen/bugs.html
debug_level: 0
jtag_nsrst_delay: 200
jtag_ntrst_delay: 200
srst_only combined srst_gates_jtag srst_open_drain
500 kHz
    TargetName         Type       Endian TapName            State
--  ------------------ ---------- ------ ------------------ ------------
 0* lpc1768.cpu        cortex_m3  little lpc1768.cpu        running
target state: halted
target halted due to debug-request, current mode: Handler HardFault
xPSR: 0x61000003 pc: 0x00000e44 msp: 0x10007fbc
flash 'lpc2000' found at 0x00000000
#0 : lpc2000 at 0x00000000, size 0x00080000, buswidth 0, chipwidth 0
        #  0: 0x00000000 (0x1000 4kB) protected
        #  1: 0x00001000 (0x1000 4kB) protected
        #  2: 0x00002000 (0x1000 4kB) protected
        #  3: 0x00003000 (0x1000 4kB) protected
        #  4: 0x00004000 (0x1000 4kB) protected
        #  5: 0x00005000 (0x1000 4kB) protected
        #  6: 0x00006000 (0x1000 4kB) protected
        #  7: 0x00007000 (0x1000 4kB) protected
        #  8: 0x00008000 (0x1000 4kB) protected
        #  9: 0x00009000 (0x1000 4kB) protected
        # 10: 0x0000a000 (0x1000 4kB) protected
        # 11: 0x0000b000 (0x1000 4kB) protected
        # 12: 0x0000c000 (0x1000 4kB) protected
        # 13: 0x0000d000 (0x1000 4kB) protected
        # 14: 0x0000e000 (0x1000 4kB) protected
        # 15: 0x0000f000 (0x1000 4kB) protected
        # 16: 0x00010000 (0x8000 32kB) protected
        # 17: 0x00018000 (0x8000 32kB) protected
        # 18: 0x00020000 (0x8000 32kB) protected
        # 19: 0x00028000 (0x8000 32kB) protected
        # 20: 0x00030000 (0x8000 32kB) protected
        # 21: 0x00038000 (0x8000 32kB) protected
        # 22: 0x00040000 (0x8000 32kB) protected
        # 23: 0x00048000 (0x8000 32kB) protected
        # 24: 0x00050000 (0x8000 32kB) protected
        # 25: 0x00058000 (0x8000 32kB) protected
        # 26: 0x00060000 (0x8000 32kB) protected
        # 27: 0x00068000 (0x8000 32kB) protected
        # 28: 0x00070000 (0x8000 32kB) protected
        # 29: 0x00078000 (0x8000 32kB) protected
lpc2000 flash driver variant: 2, clk: 12000kHz
auto erase enabled
auto unlock enabled
wrote 98304 bytes from file FreeRTOS_USB_Eth_Main.hex in 12.734375s (7.539 kb/s)
verified 66100 bytes in 1.421875s (45.398 kb/s)
shutdown command invoked



Assuming your firmware is already loaded, you can run 'openocd' in one Terminal
and 'make debug' in another which will produce output similar to the following:

C:\MicropendousX\Firmware\FreeRTOS_USBSerial_Ethernet_LPC17x8>make debug
GNU gdb (Sourcery G++ Lite 2009q3-68) 6.8.50.20090630-cvs
Copyright (C) 2009 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-mingw32 --target=arm-none-eabi".
For bug reporting instructions, please see:
<https://support.codesourcery.com/GNUToolchain/>.
0x00000000 in ?? ()
JTAG tap: lpc1768.cpu tap/device found: 0x4ba00477 (mfg: 0x23b, part: 0xba00, ver: 0x4)
target state: halted
target halted due to debug-request, current mode: Thread
xPSR: 0x81000000 pc: 0x00002f9c psp: 0x10003510
Breakpoint 1 at 0x33a: file FreeRTOS_USB_Eth_Main.c, line 261.
The disassembly style is "std".
The current ARM ABI is "auto" (currently "AAPCS").
ARM debugging is on.
-----------Monitor Poll------------------------
background polling: on
TAP: lpc1768.cpu (enabled)
target state: halted
target halted due to debug-request, current mode: Thread
xPSR: 0x81000000 pc: 0x00002f9d psp: 0x10003510
-----------Resume, Sleep-----------------------
-----------Monitor Poll------------------------
background polling: on
TAP: lpc1768.cpu (enabled)
target state: running
-----------print dev_175x----------------------
$1 = 1 '\1'
-----------info registers----------------------
r0             0x10002fe8       268447720
r1             0x0      0
r2             0x0      0
r3             0x0      0
r4             0x10003048       268447816
r5             0xa5a5a5a5       2779096485
r6             0xa5a5a5a5       2779096485
r7             0x10003510       268449040
r8             0xa5a5a5a5       2779096485
r9             0xa5a5a5a5       2779096485
r10            0xa5a5a5a5       2779096485
r11            0xa5a5a5a5       2779096485
r12            0xa5a5a5a5       2779096485
sp             0x10003510       0x10003510
lr             0x2dc1   11713
pc             0x2f9d   0x2f9d <xQueueGenericReceive+1>
fps            0x0      0
cpsr           0x81000020       2164260896
-----------------------------------------------
(gdb)

