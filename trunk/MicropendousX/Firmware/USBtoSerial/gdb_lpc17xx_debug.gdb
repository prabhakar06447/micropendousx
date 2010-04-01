#
# MicropendousX LPC17xx Development Board
#
# http://www.MicropendousX.org
#
# Connect to a debugger controlling a LPC17xx IC
#
# Start your OpenOCD gdb server before running
# this script with:  arm-none-eabi-gdb -x gdb_lpc17xx_debug.gdb
#
# Note the 'monitor' command just passes its
# arguments to OpenOCD
#
# This file is released under the MIT License
#

# Connect to OpenOCD gdb server
target remote localhost:3333

# reset and halt the LPC17xx IC with OpenOCD commands
monitor reset
monitor halt

# load the firmware's symbol table into gdb
file USBtoSerial.elf

# set a breakpoint on the first function in main()
br prvSetupHardware

#configure debugging and then display settings
set debug arm
show arm disassembler
show arm abi
show debug arm

# after the target starts and breakpoints, resume
echo -----------Monitor Poll------------------------\n
monitor poll
echo -----------Resume, Sleep-----------------------\n
monitor resume
monitor sleep 200
echo -----------Monitor Poll------------------------\n
monitor poll
echo -----------print dev_175x----------------------\n
print dev_175x
echo -----------info registers----------------------\n
info registers
echo -----------------------------------------------\n
