/*
 * Timer.c
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */
#include <msp430.h>
#include <lib/Timer.h>

void timerCounter(selectTimerClock ){
    TA0CTL = TACLR; // Reset Timer
    TA0CTL |= TASSEL_1 |  MC_2 ; // Select ACLK as timer clock source, Up mode, TB start.
}

void timerCaptureCompare(int captureCompareInput, int selectTimerClock){
     TA0CTL = TACLR; // Reset Timer
     TA0CTL = selectTimerClock |  MC_2 ; // Select ACLK as timer clock source, Up mode, TB start.
     TA0CCTL1 |= CAP + CM_2 + captureCompareInput + SCS + CCIE; // Capturemode on/off, Capture mode neg Edge, Capture input CCI2A (P1.5), Capture synchronus mode, capture interrupt enable
     // Select CCIOA (P1.5)
     if(captureCompareInput == CCIS_0 ){
     P1DIR &= ~BIT6;
     P1OUT |= BIT6;
     P1SEL0 |= BIT6;
     P1SEL1 &= ~BIT6;
     }
     // Select CCI0B(P7.1)
     if(captureCompareInput == CCIS_1){
         P7DIR &= ~BIT1;
         P7OUT |= BIT1;
         P7SEL0 |= BIT1;
         P7SEL1 &= ~BIT1;
     }
}






