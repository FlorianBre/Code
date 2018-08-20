/*
 * Timer.c
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */
#include <msp430.h>
#include <lib/Timer.h>
#include <IQmathLib.h>
#define T_ON_A0 TA0CCR1
#define T_PERIOD_A0 TA0CCR0
#define T_ON_A1 TA1CCR1
#define T_PERIOD_A1 TA1CCR0
#define T_ON_B0 TB0CCR5
#define T_ADD_B0 TB0CCR6
#define T_PERIOD_B0 TB0CCR0

void timerInitCounterA0(unsigned int clockSelect, unsigned int countMode, unsigned int countVal){
    TA0CTL = TACLR;
    TA0CTL |= clockSelect |  countMode;
    TA0CCR0 = countVal;
}

void timerInitCounterA1(unsigned int clockSelect, unsigned int countMode, unsigned int countVal){
    TA1CTL = TACLR;
    TA1CTL |= clockSelect |  countMode;
    TA1CCR0 = countVal;
}

void timerInitCounterA2(unsigned int clockSelect, unsigned int countMode, unsigned int countVal){
    TA2CTL = TACLR;
    TA2CTL |= clockSelect | countMode;
    TA2CCR0 = countVal;
}
void timerInitCounterA3(unsigned int clockSelect, unsigned int countMode, unsigned int countVal){
    TA3CTL = TACLR;
    TA3CTL |= clockSelect |  countMode;
    TA3CCR0 = countVal;
}
void timerInitCounterB0(unsigned int clockSelect, unsigned int countMode, unsigned int countVal){
    TB0CTL = TACLR;
    TB0CTL = clockSelect |  countMode ;
    TB0CCR0 = countVal;
}

void timerCaptureCompareA0(unsigned int captureCompareInput, unsigned int clockSelect, unsigned int edgeSelect){
    TA0CTL = TACLR; // Reset Timer
    TA0CTL |= clockSelect |  MC_2 ; // Select timer clock source, Timer Stop.
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
    TA0CTL |= TACLR; // Reset Timer.
    TA0CTL |= clockSelect | MC_1; // Select timer clock source,Count up to the value in TA0CCR0.
    TA0CCTL1 |= pwmOutputMode; // Select output mode.
    // configure Ports as CC Output
    if(pwmOutputMode > OUTMOD_0){
        P1DIR |= BIT6 | BIT0;
        P1SEL0 |= BIT6 | BIT0;
        P1SEL1 &= ~BIT0;
        P1SEL1 |= BIT6;
    }
    timerSetDutyCycleA0(dutyCycle);

}
void timerSetDutyCycleA0(double dutyCycle){
    //long test = T_PERIOD_A0 + 1;
    //T_ON_A0 = _IQ19toF((_IQ19mpy( test << 19, iqA )));
    T_ON_A0 = (T_PERIOD_A0 + 1.0) * dutyCycle;
}

void timerInitPWMA1(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode) {
    T_PERIOD_A1 = periodCycles;
    TA1CTL |= TACLR;
    TA1CTL |= clockSelect | MC_1;
    TA1CCTL1 |= pwmOutputMode;
    if(pwmOutputMode > OUTMOD_0){
        P1DIR |= BIT2;
        P3DIR |= BIT3;
        P1SEL0 |= BIT2;
        P1SEL1 &= ~BIT2;
        P3SEL0 &= ~BIT3;
        P3SEL1 &= ~BIT3;
    }
    timerSetDutyCycleA1(dutyCycle);

}
void timerSetDutyCycleA1(double dutyCycle){
    T_ON_A1 = (T_PERIOD_A1 + 1.0) * dutyCycle;
}

void timerInitPWMB0(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode) {
    T_PERIOD_B0 = periodCycles;
    TB0CTL |= TACLR;
    TB0CTL = clockSelect | MC_1;
    TB0CCTL5 = pwmOutputMode;
    if(pwmOutputMode > OUTMOD_0){
        P2DIR |= BIT0;
        P2SEL0 &= ~BIT0;
        P2SEL1 |= BIT0;
        P2DIR |= BIT1;
        P2SEL0 &= ~BIT1;
        P2SEL1 |= BIT1;
    }
    if (clockSelect == TASSEL_3){
        // Configure Pin as TB0 Trigger
        P3DIR &= ~BIT3;
        P3SEL1 = BIT3;
        P3SEL0 = BIT3;
    }

    timerSetDutyCycleB0(dutyCycle);

}
void timerSetDutyCycleB0(double dutyCycle){
    T_ON_B0 = (T_PERIOD_B0 + 1.0) * dutyCycle;
    T_ADD_B0 = T_ON_B0;
}






