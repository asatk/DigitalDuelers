/*
 * main.c
 * @author anthony atkinson and jonathan poss
 */

#include <stdlib.h>
#include <time.h>
#include "GPIO_registers.h"

int score[2];
int i;
int j;

char BUZZER_FLAG = 0;

extern void Setup_Game(void);

void wait(int buffer);
int checkButtonOne(void);
int checkButtonTwo(void);
void Buzz(void);
void LEDOneOn(void);
void LEDOneOff(void);
void LEDTwoOn(void);
void LEDTwoOff(void);
void flashOne(void);
void flashTwo(void);

void wait(int buffer) {
    LEDOneOn();
    LEDTwoOn();
    for (i = 0; i < buffer; i++) {}
    LEDOneOff();
    LEDTwoOff();
}

int checkButtonOne(void) {
    int read = *pGPIODATA_PortE;
    return (read == 0x01) || (read == 0x03);
}

int checkButtonTwo(void) {
    int read = *pGPIODATA_PortE;
    return (read == 0x02) || (read == 0x03);
}

void Buzz(void) {
    j = 0;
    while (j < 3e3 && (1-checkButtonOne()) && (1-checkButtonTwo())) {
        for (i = 0; i < 2e1; i++) {}
        if (BUZZER_FLAG == 0) {
            *pGPIODATA_PortF &= ~0x04;
            BUZZER_FLAG = 1;
        } else {
            *pGPIODATA_PortF |= 0x04;
            BUZZER_FLAG = 0;
        }
        j++;
    }
}

void LEDOneOn(void) {
    *pGPIODATA_PortF |= 0x01;
}

void LEDTwoOn(void) {
    *pGPIODATA_PortF |= 0x02;
}

void LEDOneOff(void) {
    *pGPIODATA_PortF &= ~0x01;
}

void LEDTwoOff(void) {
    *pGPIODATA_PortF &= ~0x02;
}

void flashOne(void) {
    LEDTwoOff();
    while(Continue_Game()) {
        for (i = 0; i < 1e4; i++) {}
        LEDOneOn();
        for (i = 0; i < 1e4; i++) {}
        LEDOneOff();
    }
}

void flashTwo(void) {
    LEDOneOff();
    while(Continue_Game()) {
        for (i = 0; i < 1e4; i++) {}
        LEDTwoOn();
        for (i = 0; i < 1e4; i++) {}
        LEDTwoOff();
    }
}

void Play_Game() {
    while (score[0] < 5 && score[1] < 5) {
        wait(2000000);
        int premature = 0;
        int r = 1e5 * (rand() % 8);
        for (i = 0; i < r; i++) {
            if (checkButtonOne()) {
                score[1] += 1;
                LEDTwoOn();
                for (j = 0; j < 1e5; j++) {}
                premature = 1;
                break;
                // player 1 loses
            } else if (checkButtonTwo()) {
                score[0] += 1;
                LEDOneOn();
                premature = 1;
                for (j = 0; j < 1e5; j++) {}
                break;
                // player 2 loses
            }
        }
        if (premature) {
            continue;
        }
        Buzz();
        while ((1-checkButtonOne()) && (1-checkButtonTwo())) {
        }
        if (checkButtonOne()) {
            score[0] += 1;
            LEDOneOn();
            for (i = 0; i < 1e5; i++) {}
            // player 1 wins
        } else if (checkButtonTwo()) {
            score[1] += 1;
            LEDTwoOn();
            for (i = 0; i < 1e5; i++) {}
            // player two wins
        }
    }
    if (score[0] == 5) {
        flashOne();
    } else {
        flashTwo();
    }
}

void Reset_Scores() {
    score[0] = 0;
    score[1] = 0;
}

int Continue_Game() {
    return (*pGPIODATA_PortE & 0x08) == 0x08;
}

int main(void)
{
    srand(time(NULL));
    Setup_Game();
    while(1) {
        Play_Game();
        Reset_Scores();
    }

    return 0;
}

