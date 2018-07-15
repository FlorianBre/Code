#include <msp430.h> 
#include <lib\Timer.h>



/**
 * main.c
 */
void main(void)
{

    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
	

	return;
}
