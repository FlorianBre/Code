#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 = 0;
    TA0CTL = TACLR; // Reset Timer
	TA0CTL |= TASSEL_1 |  MC_2 ; // Select ACLK as timer clock source, Up mode, TB start.
	TA0CCTL0 |= CAP | CM_1 | CCIS_0 | SCS | CCIE; // Capturemode on/off, Capture mode neg Edge, Capture input CCI2A (P1.5), Capture synchronus mode, capture interrupt enable
	// Select CCIOA (P1.5)
	P1DIR = 0x00;
	P1REN = BIT5;
	P1OUT = BIT5;
    P7DIR = 0x00;
	P9DIR = BIT7;
	P9OUT = BIT7;
	P1SEL0 = 0xFF;
	P1SEL1 = 0xFF;
    P7SEL0 = 0xFF;
    P7SEL1 = 0xFF;
    __enable_interrupt();
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P9OUT ^= BIT7;
    _no_operation();

}
