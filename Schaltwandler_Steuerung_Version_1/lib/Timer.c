/*
 * Timer.c
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */
#include <msp430.h>
#include <lib/Timer.h>
#define T_ON_A0 TA0CCR1
#define T_PERIOD_A0 TA0CCR0

void timerCounterA0(unsigned int clockSelect){
    TA0CTL = TACLR;
    TA0CTL |= clockSelect |  MC_2;
}

void timerCounterA1(unsigned int clockSelect){
    TA1CTL = TACLR;
    TA1CTL |= clockSelect |  MC_2;
}

void timerCounterA2(unsigned int clockSelect ){
    TA2CTL = TACLR;
    TA2CTL |= clockSelect |  MC_2;
}
void timerCounterA3(unsigned int clockSelect ){
    TA3CTL = TACLR;
    TA3CTL |= clockSelect |  MC_2;
}
void timerCounterB0(unsigned int clockSelect ){
    TB0CTL = TACLR;
    TB0CTL = clockSelect |  MC_2 ;
}

void timerCaptureCompareA0(unsigned int captureCompareInput, unsigned int clockSelect, unsigned int edgeSelect){
    TA0CTL = TACLR; // Reset Timer
    TA0CTL = clockSelect |  MC_2 ; // Select timer clock source, Up mode.
    TA0CCTL0 |= CAP + edgeSelect + captureCompareInput + SCS + CCIE; // Capturemode on/off, Capture mode neg Edge, Capture input  (P1.5), Capture synchronus mode, capture interrupt enable
    // Select CCIOA (P1.5)
    if(captureCompareInput == CCIS_0 ){
        P1DIR &= ~BIT5;
        P1OUT |= BIT5;
        P1SEL0 |= BIT5;
        P1SEL1 |= BIT5;
    }
    // Select CCI0B(P7.1)
    if(captureCompareInput == CCIS_1){
        P7DIR &= ~BIT1;
        P7OUT |= BIT1;
        P7SEL0 |= BIT1;
        P7SEL1 &= ~BIT1;
    }
}
    void timerCaptureCompareA1(unsigned int captureCompareInput, unsigned int clockSelect, unsigned int edgeSelect){
        TA1CTL = TACLR;
        TA1CTL = clockSelect |  MC_2 ;
        TA1CCTL0 |= CAP + edgeSelect + captureCompareInput + SCS + CCIE;
        // Select CCIOA (P1.4)
        if(captureCompareInput == CCIS_0 ){
            P1DIR &= ~BIT4;
            P1OUT |= BIT4;
            P1SEL0 |= BIT4;
            P1SEL1 |= BIT4;
        }
        // Select CCI0B(P5.2)
        if(captureCompareInput == CCIS_1){
            P5DIR &= ~BIT2;
            P5OUT |= BIT2;
            P5SEL0 |= BIT2;
            P5SEL1 &= ~BIT2;
        }
    }
    void timerCaptureCompareB0(unsigned int captureCompareInput, unsigned int clockSelect, unsigned int edgeSelect){
            TB0CTL = TACLR;
            TB0CTL = clockSelect |  MC_2 ;
            TB0CCTL0 |= CAP + edgeSelect + captureCompareInput + SCS + CCIE;
            // Select CCI (P3.4)
            if(captureCompareInput == CCIS_0 ){
                P3DIR &= ~BIT4;
                P3OUT |= BIT4;
                P3SEL0 |= BIT4;
                P3SEL1 |= BIT4;
            }
            // Select CCI (P6.4)
            if(captureCompareInput == CCIS_1){
                P6DIR &= ~BIT4;
                P6OUT |= BIT4;
                P6SEL0 |= BIT4;
                P6SEL1 &= ~BIT4;
            }
        }

void timerInitPWMA0(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode) {
        T_PERIOD_A0 = periodCycles;
        TA1CTL |= TACLR; // Reset Timer
        TA1CTL |= clockSelect | MC_1; // Select timer clock source,Count up to the value in TA0CCR0.
        TA1CCTL1 |= pwmOutputMode; // Select output mode.
        timerSetDutyCycleA0(dutyCycle);

    }
    void timerSetDutyCycleA0(double dutyCycle){
        T_ON_A0 = (T_PERIOD_A0 + 1) * dutyCycle;
    }









