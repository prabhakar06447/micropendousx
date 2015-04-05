MicropendousX is an OpenHardware Development Board for NXP's LPC1800 series of microcontrollers with a Free and Open Source Software (FOSS) development platform for hardware, firmware, and software.  It is designed for portable operation and for interfacing electronics to PCs via High Speed USB.

**Project Status**: _2011-11-03_ : Design has been prototyped and successfully enters USB DFU mode.  Check out the [Prototyping and Testing](http://code.google.com/p/micropendousx/wiki/MicropendousX_LPC183x_Prototyping) page for more information.  Contact _Opendous Inc._ through the [MicropendousX Group](http://groups.google.com/group/micropendousx) if you have any questions.

![http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-LPC183x/MicropendousX-LPC183x-Front.jpg](http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-LPC183x/MicropendousX-LPC183x-Front.jpg)

## Features ##

  * based on the [LPC183x](http://ics.nxp.com/products/lpc1000/lpc18xx/) [ARM Cortex-M3](http://www.arm.com/products/CPUs/ARM_Cortex-M3.html) MCU
  * High-Speed USB (480Mbps)
  * USB-microB
  * [MMC](http://en.wikipedia.org/wiki/MultiMediaCard) Memory card connector on back
  * [Cortex Debug Header](http://infocenter.arm.com/help/topic/com.arm.doc.faqs/ka13634.html) for [JTAG](http://en.wikipedia.org/wiki/Joint_Test_Action_Group)
  * [LTC3559](http://www.linear.com/product/LTC3559) Li-Ion charger and dual buck regulator
  * [STC3100](http://www.st.com/internet/analog/product/214267.jsp) Battery Guage
  * Opennect Connector compatible with [Opennect-SingleEnded-x4](http://code.google.com/p/opennect/wiki/Opennect_SingleEnded) and [Opennect-I2S](http://code.google.com/p/opennect/wiki/Opennect_I2S) standards.
  * Opennect Connector compatible with [Opennect-ADC-x2](http://code.google.com/p/opennect/wiki/Opennect_ADC) and [Opennect-DAC-x1](http://code.google.com/p/opennect/wiki/Opennect_DAC) standards.
  * 12MHz main crystal and 32.768kHz RTC crystal
  * Dimensions (LxWxH) with connectors: 86mm x 43mm x 15mm (~ 3.4" x 1.7" x 0.7")
  * completely [OpenHardware](OpenHardware.md) Design, schematic/layout/assembly/gerbers/BillOfMaterials files [available](http://code.google.com/p/micropendousx/source/browse/trunk/MicropendousX/Hardware/MicropendousX-LPC183x/) for Open Source [KiCAD](http://kicad.sourceforge.net/).
  * Design is 4-layer with 7mil/7mil minimum trace/clearance and minimum 15mil Vias
  * Design uses minimum 0603 components for DIY reflow