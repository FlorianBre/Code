/*
 * SetDutyCycle.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
#include <lib/SetDutyCycle.h>
#include <msp430fr6989.h>
#define T_ON TA0CCR1
#define T_PERIOD TA0CCR0

void initDutyCycle(int frequency, int clockSource) {
    int clockFrequency = getClockFrequency(clockSource);
    calculatePeriod(frequency, clockFrequency);
    initTimer(clockSource);
}

static void calculatePeriod(int frequency, int clockFrequency){
    T_PERIOD = (clockFrequency / frequency) - 1;
}

void selectDutyCycle(int duty){
    T_ON = (T_PERIOD + 1) * duty;
}

void initTimer(int clockSelect){
   TA1CTL |= TACLR; // Clear Timercounter
   TA1CTL |= TASSEL_1 | MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
   TA1CCTL1 |= OUTMOD_7; // Reset Set  mode for output unit 1, P2.1, P2.2 clk
   switch (clockSelect){
       case 1:
           CSCTL0 = CSKEY;
           CSCTL2 = SELA_0; // Select ALCK
           break;
   }

}

static int getClockFrequency(int clockSelect){
    unsigned int frequency;
    switch (clockSelect){
        case 1: frequency = 32768; break;
    }
    return frequency;
}



