/*
 * main.c
 * @author anthony atkinson and jonathan poss
 */

#include <stdlib.h>
#include <time.h>

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



void setup(void) {
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

}

int main(void) {

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

int state;

int[] score;

void wait(int buffer);
bool checkButtonOne(void);
bool checkButtonTwo(void);
void Buzz(void); // flash it on and off very fast, like wait 10 before turning on and off
void LEDOneOn(void);
void LEDOneOff(void);
void LEDTwoOn(void);
void LEDTwoOff(void);
void flashOne(void);
void flashTwo(void);

int main(void)
{
    srand(time(NULL));
    while (score[0] < 5 && score[1] < 5) {
        wait(100000);
        premature = false;
        int r = 1e5 * (rand() % 8);
        for (int i = 0; i < r; i++) {
            if (checkButtonOne()) {
                score[1] += 1;
                LEDTwoOn();
                premature = true;
                // player 1 loses
            } else if (checkButtonTwo()) {
                score[0] += 1;
                LEDOneOn();
                premature = true;
                // player 2 loses
            }
        }
        if (premature) {
            continue;
        }
        Buzz() // make buzzer buzz
        while (!checkButtonOne() || !checkButtonTwo()) {
            Buzz();
        }
        if (checkButtonOne()) {
            score[0] += 1;
            LEDOneOn();
            // player 1 wins
        } else if (checkButtonTwo()) {
            score[1] += 1;
            LEDTwoOn();
            // player two wins
        }
    }
    if (score[0] == 5) {
        flashOne();
    } else {
        flashTwo();
    }

    return 0;
}

