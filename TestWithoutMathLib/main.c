#include <msp430.h> 
#include <math.h>

/**
 * main.c
 */
double a = 0.0045;
void main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	 double b = 0.0034;
	 a += 0.1;
	 _nop();
	 sqrt(b);
	 _nop();
	
}
