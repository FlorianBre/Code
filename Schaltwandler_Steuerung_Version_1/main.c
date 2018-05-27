#include <lib/ADC.h>
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

const double rIn_ohm = 2000;
const double lCoil_H = 1;
int testValue;

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    __enable_interrupt();
    PM5CTL0 = 0;
    _no_operation();
    timerCaptureCompareA0(CCIS_0, TASSEL_1);
    while(1){

    }
}

double CalculateDutyCycle(int vIn, int vOut, int tPeriod)
    {
        return (1-(vIn/vOut))*(2*lCoil_H/(tPeriod*rIn_ohm));
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

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if(ADC12IFG0) {
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.x
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}

