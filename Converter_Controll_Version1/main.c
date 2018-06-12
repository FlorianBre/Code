#include <lib/ADC.h>
#include <lib/Timer.h>
#include <lib/LCD.h>
#include <math.h>
#include <msp430fr6989.h>


//---Module Description --------------------------------------------------

/**
 * @file    main.c
 * @author  Florian Bregler
 * @version 2.0
 * @brief   Controlling of a boost converter for input impedance matching
 *          by adjusting the duty cycle .
 *
 * ---- Pinout ----
 * P1.6 PWM Signal.
 * P9.2 D-Flip Flop control.
 */

const double vIn_V = 2.25;
const double vOut_V = 5.0;
const double rIn_Ohm = 2000.0;
const double lCoil_H = 560.0e-6;
const double tPeriod = 93.0e-6;
double duty;
double calculateDutyCycle();
void initDFlipFlopControl();

void main(void)
{
    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    //duty = calculateDutyCycle();
    duty = 0.5;
    // Clock source = smclk.
    // Select Output 1 (P1.6).
    timerInitPWMA0(1000, TASSEL_2, duty, OUTMOD_7, 1);
    initDFlipFlopControl
    // Interrupt enable.
    while(1){
        _nop();
    }
}


double calculateDutyCycle()
{
    return sqrt(((1-(vIn_V/vOut_V))*2*lCoil_H)/(tPeriod*rIn_Ohm));
}

void initDFlipFlopControl() {
    TA0CCTL1 |= CCIE;
    TA0CCTL0 |= CCIE;
    P9DIR |= BIT3;
    P9OUT |= BIT3;
    P9SEL0 &= ~BIT3;
    P9SEL1 &= ~BIT3;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void)
{
    // Toggle Pin 9.3
    P9OUT ^= BIT3;
    P9OUT ^= BIT3;
    TA0CCTL1 &= ~CCIE;
    TA0CCTL1 &= ~TAIFG;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    TA0CCTL1 |= CCIE;
    TA0CCTL0 &= ~TAIFG;
}

