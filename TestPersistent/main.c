#include <msp430.h> 
    #pragma PERSISTENT ( count )
    float count[1000] = {0};

int test;
/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

    _nop();
    int i;
    for(i = 0; i < 1000; i++){
        count[i] = i;
    }
    _nop();
}
