These older designs are DIY'able with PCBs for the [LPC1343](http://batchpcb.com/index.php/Products/44193) and [LPC176x](http://batchpcb.com/index.php/Products/44387) designs available at cost from BatchPCB.  Contact Opendous Inc. through the [MicropendousX Group](http://groups.google.com/group/micropendousx) if you have any questions.

![http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-176x/MicropendousX-176x_Actionshot.jpg](http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-176x/MicropendousX-176x_Actionshot.jpg)

## Features ##

  * based on [LPC176x](http://ics.nxp.com/products/lpc1000/lpc17xx/) [ARM Cortex-M3](http://www.arm.com/products/CPUs/ARM_Cortex-M3.html) MCU
  * Full-Speed USB (12Mbps) and Ethernet (100Mbps) capability
  * USB-miniB and USB-A connectors and on-board GPIO-controllable [TS3USB221](http://focus.ti.com/docs/prod/folders/print/ts3usb221.html) USB switch for USB Device and Host modes
  * all pins not related to USB and Ethernet are exposed on the outer headers
  * microSD connector in [SPI Mode](http://en.wikipedia.org/wiki/MicroSD) on the underside of the board
  * 10/100 Ethernet based on the [LAN8720](http://www.smsc.com/index.php?tid=149&pid=59) PHY.  **HOWEVER**, an [inverter](http://search.digikey.com/us/en/products/NC7SZ14P5X/NC7SZ14P5XCT-ND/) needs to be added to the _CLK_ signal to make the PHY usable.  Such an inverter is not currently included.  Thanks to _ForrestG_ for pointing this out.
  * [Cortex Debug Header](http://infocenter.arm.com/help/topic/com.arm.doc.faqs/ka13634.html) for [JTAG](http://en.wikipedia.org/wiki/Joint_Test_Action_Group)/SWD
  * 3.3V 1A+ +/- 2% [ST1L05B](http://www.st.com/stonline/products/literature/ds/14492/st1l05b.htm) regulator
  * RESET button, NMI button, and USB status LED
  * 12MHz main crystal and 32.768kHz RTC crystal
  * Dimensions (LxWxH): 3.6" x 1" x 1"- plugs into standard solder-less [breadboard](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=438-1046-ND)
  * completely [OpenHardware](OpenHardware.md) Design, schematic/layout/assembly files available for Open Source [KiCAD](http://kicad.sourceforge.net/). Gerber and other technical files can be [customized](http://code.google.com/p/opendous/wiki/KiCADTutorialCreatingGerberFiles).
  * Design is 2-layers with 8mil/8mil trace/clearance and minimum 20mil Vias
  * Design uses minimum 0603 components for DIY'ability

### [LPC1343](http://ics.nxp.com/products/lpc1000/lpc13xx/)-Based DIP Board ###

![http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-1343/MicropendousX-1343-Small.jpg](http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-1343/MicropendousX-1343-Small.jpg)