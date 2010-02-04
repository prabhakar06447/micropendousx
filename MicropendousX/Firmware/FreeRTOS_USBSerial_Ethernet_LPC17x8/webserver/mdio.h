/*****************************************************************************
 * @file:    mdio.h
 * @purpose: CMSIS Cortex-M3 Core Peripheral Access Layer Header File for 
 *           NXP LPC1758 Device Series.
 * @version: V1.1
 * @date:    14th May 2009
 * @license: This file is part of NXP Application Note AN10859
 *---------------------------------------------------------------------------*/

#ifndef __mdio_H__
#define __mdio_H__

/* Useful register settings and LPC model defines */
#include "LPC17xx_Useful_Defines.h"


#define MDIO    0x00000200
#define MDC     0x00000100


U32 mdio_read(int PhyReg);
void mdio_write(int PhyReg, int Value);


#endif  // __mdio_H__
