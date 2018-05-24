#include <lib/ADCMeasurement.h>
#include <lib/CalculateEmulatedResistance.h>
#include <lib/SetDutyCycle.h>
#include <lib/Timer.h>
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
 *
 */
void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    __enable_interrupt();
    PM5CTL0 = 0;
    _no_operation();
    timerCaptureCompareA0(CCIS_0, TASSEL_1);
    while(1){

    }
}

/*
 * Timer interrupt triggered by the change form a high to a low edge on pin 1.5.
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    _no_operation();
    if(TA1IV == 4){
        _no_operation();
    }
}
