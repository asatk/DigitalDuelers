/*
 * Setup_Game.c
 *
 *  Created on: Nov 11, 2021
 *      Author: root
 */
#include "GPIO_registers.h"

void Setup_Game(void) {

    // Step 1a: Turn on the clocks for Ports E and F.
    *pRCGCGPIO = *pRCGCGPIO | 0x0010;
    *pRCGCGPIO = *pRCGCGPIO | 0x0020;

    // Step 1b: Check to be sure the clocks have started.
    while ( (*pRCGCGPIO & 0x0010) == 0 ) ; // Good thing this is volatile!

    // Step 2a: Unlock Ports E and F
    *pGPIOLOCK_PortE = 0x4C4F434B;
    *pGPIOLOCK_PortF = 0x4C4F434B;

    // Step 2b: Enable us to commit to all controls in Port E for PE[5:0] and Port F for PF[4:0]
    *pGPIOCR_PortE |= 0x1F;
    *pGPIOCR_PortF |= 0x0F;

    //set PE[0:1] for input
    *pGPIODIR_PortE &= ~0x03;

    //set pull down resistor for input mode
    *pGPIOPDR_PortE |= 0x03;

    //set PF[0:2] for output
    *pGPIODIR_PortF |= 0x07;

    // Step 4:  Set the pins to be used as a general purpose I/O pin.
    *pGPIOAFSEL_PortE &= ~0x03;
    *pGPIOAFSEL_PortF &= ~0x07;

    // Step 5:  Enable the pins
    *pGPIODEN_PortE |= 0x03;
    *pGPIODEN_PortF |= 0x07;
}


