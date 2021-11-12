/*
 * main.c
 *
 * Some code for Lecture 12
 *
 * Port F has 5 pins.  The LaunchPad uses all of these 5
 * pins as shown below:
 *      PF0:        USR_SW2
 *      PF1:        LED_R
 *      PF2:        LED_B
 *      PF3:        LED_G <- The one we will play with
 *      PF4:        USR_SW1
 *
 * Important Note: The NMI (non-maskable interrupt) is on PF0.  This means
 * that the Alternate Function Select, Pull-Up Resistor, Pull-Down Resistor,
 * and Digital Enable are all locked for PF0 until a value of 0x4C4F434B
 * is written to the Port F GPIO Lock Register.  After Port F is
 * unlocked, bit 0 of the Port F GPIO Commit Register must be set to
 * allow access to PF0's control registers.  Requiring this procedure
 * makes it unlikely to accidentally re-configure the JTAG and NMI pins
 * as GPIO, which can lock the debugger out of the processor and make
 * it permanently unable to be debugged or re-programmed.
 *
 * Date 24 Feb 2020
 */

#include <stdlib.h>
#include <time.h>

int main(void) {

    /*
     * First, define the pointers to the key registers needed for configuring
     * the general purpose I/Os for Port F.  Note that the use of volatile ensures that
     * the compiler always carries out the memory accesses, rather than optimizing
     * them out (for example, if the access is in a loop).
     */

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

    /*
     * Now step through the cook book of steps.
     */

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

    // Done!

    //check is buzzer square wave is up or down
    char BUZZER_FLAG = 1;

    *pGPIODATA_PortF &= ~0x04;

    while(1) {
        int read = *pGPIODATA_PortE;

        //PF0 (blue side button pressed)
        if (read == 0x01)
        {
            *pGPIODATA_PortF |= 0x01;
            *pGPIODATA_PortF &= ~0x02;
        }
        //PF1 (red side button pressed)
        else if (read == 0x02)
        {
            *pGPIODATA_PortF |= 0x02;
            *pGPIODATA_PortF &= ~0x01;
        }
        //PF2 (both buttons pressed)
        else if (read == 0x03) {
            int i = 0;
            *pGPIODATA_PortF &= ~0x03;
            if (BUZZER_FLAG) {
                *pGPIODATA_PortF |= 0x04;
                BUZZER_FLAG = 0;
            } else {
                *pGPIODATA_PortF &= ~0x04;
                BUZZER_FLAG = 1;
            }
            for (i = 0; i < 2e1; i++) {}
        }
        //no buttons pressed
        else
        {
            *pGPIODATA_PortF &= ~0x01;
            *pGPIODATA_PortF &= ~0x02;
        }
    }
}
