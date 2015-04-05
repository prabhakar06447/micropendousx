DRAFT!


Download the latest [EABI GCC](http://www.codesourcery.com/sgpp/lite/arm/portal/release1033) for your platform and install.  Download the [MicropendousX Distribution](http://code.google.com/p/micropendousx/downloads/list) and extract it.  Open a Terminal or Command Prompt and change to the `/MicropendousX/Firmware/LPC17xx/Blinky` directory.  Run `make` to compile.
```
make
```

Procure any [OpenOCD](http://openocd.berlios.de/web/)-compatible JTAG debugger with a [Cortex Debug Header](http://infocenter.arm.com/help/topic/com.arm.doc.faqs/ka13634.html) or an ARM JTAG to Cortex Debug Header adapter.  [OpenOCD for Windows](http://www.freddiechopin.info/index.php/en/download/category/4-openocd) is also available.  Edit `/MicropendousX/Firmware/LPC17xx/Blinky/OpenOCD.cfg` for your particular JTAG `interface`.  Connect your MicropendousX to your OpenOCD debugger and run `make program` to program the firmware.
```
make program
```

Now run `openocd OpenOCD.cfg` to start the debugger.  In a separate Terminal or Command Prompt run `make debug`, or `arm-none-eabi-gdb.exe -x gdb_lpc17xx_debug.gdb` to start debugging your MicropendousX.
```
openocd OpenOCD.cfg
```

```
make debug
```

While not debugging you can connect to your MicropendousX with a serial terminal.  Typed letters or numbers will be incremented and returned when `FreeRTOS_USBSerial_Ethernet_LPC17x8` firmware is loaded.  See [SerialPortUsageLinux](http://code.google.com/p/micropendous/wiki/SerialPortUsageLinux) or [SerialPortUsageWindows](http://code.google.com/p/micropendous/wiki/SerialPortUsageWindows) for some notes which you will need to adapt for the LPC17xx.

If you are interested in using [Eclipse](http://www.eclipse.org/) for development and debugging there are some useful tutorials by [James P. Lynch](http://rockboxplayer.googlecode.com/svn/trunk/docs/Using_Open_Source_Tools_for_AT91SAM7S_Cross_Development_revision_C.pdf) and [Casainho](http://code.google.com/p/rockboxplayer/wiki/EclipseWithJTAGDebugSetup).

If you are having JTAG communication issues try slowing down the JTAG clock speed by altering the `jtag_khz 500` setting in `OpenOCD.cfg`.

If you are having many errors and are worried your firmware may not be uploading properly you can inject the vector checksum into your hex file with the `LPcrc` utility in the `/Software/` directory.

Here is a MicropendousX-1768 being debugged by an [Opendous FT2232H JTAG Adaptor](http://www.opendous.org/JTAG) but any [OpenOCD](http://openocd.berlios.de/web/)-compatible JTAG interface operating at 3.3V will work.

![http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-176x/MicropendousX-176x_JTAG_Connected-2.jpg](http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-176x/MicropendousX-176x_JTAG_Connected-2.jpg)