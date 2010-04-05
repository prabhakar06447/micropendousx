This directory contains ARM CMSIS v1.3 along with lpc17xx.cmsis from NXP.com

Changes to the defualt files are as follows:

CMSIS/CM3/DeviceSupport/NXP/LPC17xx/startup/gcc/startup_LPC17xx.S Lines [128...134]:

  .if (RAM_MODE)
      LDR     R0,=main
      BX      R0
  .else
      LDR     R0,=_start
      BX      R0
  .endif


were altered to:


      LDR     R0,=_start
      BX      R0