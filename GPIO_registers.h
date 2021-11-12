/*
 * GPIO_registers.h
 *
 *  Created on: Nov 11, 2021
 *      Author: root
 */

#ifndef GPIO_REGISTERS_H_
#define GPIO_REGISTERS_H_

    // pRCGCGPIO is a pointer to the General-Purpose Input/Output Run Mode Clock
    // Gating Control Register (p 340)
    extern unsigned int volatile *pRCGCGPIO;

    // pGPIOLOCK_PortF is a pointer to the GPIO Lock register for port E (p 684)
    extern unsigned int volatile *pGPIOLOCK_PortE;
    extern unsigned int volatile *pGPIOLOCK_PortF;

    // pGPIOCR_PortF is a pointer to the GPIO Commit register for port E (p 685)
    extern unsigned int volatile *pGPIOCR_PortE;
    extern unsigned int volatile *pGPIOCR_PortF;

    // pGPIODIR_PortF is a pointer to the GPIO Direction register for port E (p 663)
    extern unsigned int volatile *pGPIODIR_PortE;
    extern unsigned int volatile *pGPIODIR_PortF;

    //pull up resistor for input
    extern unsigned int volatile *pGPIOPDR_PortE;

    // pGPIOAFSEL is a pointer to the GPIO Alternate Function Select register for port E (p 672)
    extern unsigned int volatile *pGPIOAFSEL_PortE;
    extern unsigned int volatile *pGPIOAFSEL_PortF;

    // pGPIODEN is a pointer to the GPIO Digital Enable register for port E (p 683)
    extern unsigned int volatile *pGPIODEN_PortE;
    extern unsigned int volatile *pGPIODEN_PortF;

    // pGPIODATA is a pointer to the GPIO Data register for port E (p 662)
    extern unsigned int volatile *pGPIODATA_PortE;
    extern unsigned int volatile *pGPIODATA_PortF;

#endif /* GPIO_REGISTERS_H_ */
