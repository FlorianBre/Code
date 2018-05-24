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
    __enable_interrupt();
    PM5CTL0 = 0;
    _no_operation();
    timerCaptureCompare(CCIS_0, &TA0CTL, TASSEL_1);
    while(1){

    }
}
