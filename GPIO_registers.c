/*
 * GPIO_registers.c
 *
 *  Created on: Nov 11, 2021
 *      Author: root
 */

#include "GPIO_registers.h"

// pRCGCGPIO is a pointer to the General-Purpose Input/Output Run Mode Clock
// Gating Control Register (p 340)
unsigned int volatile *pRCGCGPIO = (unsigned int *) (0x400FE000 + 0x608);

// pGPIOLOCK_PortF is a pointer to the GPIO Lock register for port E (p 684)
unsigned int volatile *pGPIOLOCK_PortE = (unsigned int *)(0x40024000 + 0x520);
unsigned int volatile *pGPIOLOCK_PortF = (unsigned int *)(0x40025000 + 0x520);

// pGPIOCR_PortF is a pointer to the GPIO Commit register for port E (p 685)
unsigned int volatile *pGPIOCR_PortE = (unsigned int *)(0x40024000 + 0x524);
unsigned int volatile *pGPIOCR_PortF = (unsigned int *)(0x40025000 + 0x524);

// pGPIODIR_PortF is a pointer to the GPIO Direction register for port E (p 663)
unsigned int volatile *pGPIODIR_PortE = (unsigned int *) (0x40024000 + 0x400);
unsigned int volatile *pGPIODIR_PortF = (unsigned int *) (0x40025000 + 0x400);

//pull up resistor for input
unsigned int volatile *pGPIOPDR_PortE = (unsigned int *) (0x40024000 + 0x514);

// pGPIOAFSEL is a pointer to the GPIO Alternate Function Select register for port E (p 672)
unsigned int volatile *pGPIOAFSEL_PortE = (unsigned int *) (0x40024000 + 0x420);
unsigned int volatile *pGPIOAFSEL_PortF = (unsigned int *) (0x40025000 + 0x420);

// pGPIODEN is a pointer to the GPIO Digital Enable register for port E (p 683)
unsigned int volatile *pGPIODEN_PortE = (unsigned int *) (0x40024000 + 0x51C);
unsigned int volatile *pGPIODEN_PortF = (unsigned int *) (0x40025000 + 0x51C);

// pGPIODATA is a pointer to the GPIO Data register for port E (p 662)
unsigned int volatile *pGPIODATA_PortE = (unsigned int *) (0x40024000 + 0x3FC);
unsigned int volatile *pGPIODATA_PortF = (unsigned int *) (0x40025000 + 0x3FC);



