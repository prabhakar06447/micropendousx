For personal safety, only work in a well-ventilated area and wear eye and lung protection during reflow; [goggles](http://www.uline.com/BL_8966/Stealth-Goggles) plus [half-face respirator](http://www.uline.com/Product/Detail/H-3394/Dust-Masks-Respirators/3M-7503-Half-Face-Respirator-Large) with [multi-gas](http://www.uline.com/Product/Detail/S-12527/Dust-Masks-Respirators/3M-6006-Multiple-Toxic-Gases-Cartridge) cartridges.

The reflow process requires a toaster oven (preferably a convection type), [solder paste](http://www.digikey.com/product-detail/en/SMD291SNL/SMD291SNL-ND/), [flux](http://www.digikey.com/product-detail/en/SMD291NL/SMD291NL-ND), gloves, paper towel, IPA (99% Isopropanol/Isopropyl/Rubbing Alcohol), a drywall knife about as wide as your PCB and a [jig](http://en.wikipedia.org/wiki/Jig_(tool)) to hold the PCB in place made by gluing cut pieces of a [PCB](http://www.digikey.com/product-detail/en/PC55/PC55-ND) to a block of wood.
![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Materials.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Materials.jpg)

Secure the PCB in the corner of the glued PCB pieces on your jig.  Use another cut PCB strip at the opposite edge.  Overlay your stencil and hold everything firmly in place with one hand.  Squeeze out a bead of solder paste onto the edge of your drywall knife and run it across your stencil to apply the solder paste onto your PCB.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-StencilPositioning.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-StencilPositioning.jpg)

Your PCB now has solder paste.  Clean your drywall knife with the IPA-soaked paper towel.  Use a placement map to place all the components.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-SolderPasted.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-SolderPasted.jpg)

For BGA devices, it helps to spread a thin layer of flux onto the solder balls.  Wearing a glove, rub some flux between your fingers and then massage the flux onto the solder balls.  You want a very small amount of flux on the ball tops.  Too much in between the balls and the flux will cause the BGA device to move during reflow.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-PreparingBGA.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-PreparingBGA.jpg)

Check component placement under a magnifying glass.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-ExaminingPlacement.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-ExaminingPlacement.jpg)

Place the PCB onto your oven's tray closer to the back of the oven and farthest from the convection fan.  Heat dissipation is most even around this area.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-OvenTray.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-OvenTray.jpg)

Set your oven to 150C and wait until the oven signals it has reached this temperature, usually by shutting off its heating element.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step1.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step1.jpg)

Once the oven is at 150C, the Pre-Heat stage can begin.  Set your oven to 180C and wait 90 seconds.  This can be somewhat longer and depends on the amount of copper in your design but never wait more than 120 seconds.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step2.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step2.jpg)

Once the 90 or so seconds are up, crank your oven to full and watch the process carefully.  This is where the [goggles](http://www.uline.com/BL_8966/Stealth-Goggles) plus [half-face respirator](http://www.uline.com/Product/Detail/H-3394/Dust-Masks-Respirators/3M-7503-Half-Face-Respirator-Large) with [multi-gas](http://www.uline.com/Product/Detail/S-12527/Dust-Masks-Respirators/3M-6006-Multiple-Toxic-Gases-Cartridge) cartridges are critical.  It helps to shine a flashlight or lamp into the oven while waiting for reflow to occur.  You cannot miss it.  The dull grey solder paste shrinks and starts to shine.  The moment you see that solder paste at each corner of the board has just about fully reflowed, immediately turn off the oven, count five seconds, open the oven door, count five seconds, then gently pull the tray half-way out of the oven.  At this point you can relax and just wait for everything to cool down.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step3.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototyping-Oven-Step3.jpg)

All that is left are the thru-hole components.

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Picture.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Picture.jpg)

The back side:

![http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-LPC183x/MicropendousX-LPC183x-Back.jpg](http://micropendousx.googlecode.com/svn/trunk/MicropendousX/Hardware/MicropendousX-LPC183x/MicropendousX-LPC183x-Back.jpg)

Set up the jumpers for USB DFU boot:

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-JumperSettingsUSB0.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-JumperSettingsUSB0.jpg)

Connect the battery:

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-Small.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-Small.jpg)

The board enters USB DFU boot under Windows:

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-WindowsLibUSB.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-WindowsLibUSB.jpg)

The board enters USB DFU boot under Linux:

![http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-Linux_lsusb.jpg](http://micropendousx.googlecode.com/svn/trunk/Pictures/MicropendousX-LPC183x-Prototype-Testing-Linux_lsusb.jpg)