#include <msp430fr6989.h>
#include "lib/setDutyCycle.h"
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
    PM5CTL0 =  0;
    double duty = 0.5;
    int frequency = 10000;
    setDutyCycle(duty, frequency);
}
