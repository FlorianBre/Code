#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 = 0;
    TA0CTL = TACLR; // Reset Timer
	TA0CTL |= TASSEL_1 |  MC_2 ; // Select ACLK as timer clock source, Up mode, TB start.
	TA0CCTL0 |= CAP | CM_1 | CCIS_0 | SCS | CCIE; // Capturemode on/off, Capture mode pos Edge, Capture input CCI2A (P1.5), Capture synchronus mode, capture interrupt enable
	// Select CCIOA (P1.5)
	P1DIR = 0x00;
	P1REN |= BIT5;
	P1OUT |= BIT5;
    P7DIR = 0x00;
	P9DIR = BIT7;
	P9OUT = BIT7;
	P1SEL0 = 0xFF;
	P1SEL1 = 0xFF;
    P7SEL0 = 0xFF;
    P7SEL1 = 0xFF;
    __enable_interrupt();
}

// TimerB Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
    case TA0IV_NONE:
        _nop();
        break;              // No interrupt

    case TA0IV_TA0CCR1:
        _nop();
        break;              // TA0CCR1 interrupt
    case TA0IV_TA0CCR2: break;              // TA0CCR2 interrupt
        _nop();
    case TA0IV_TA0IFG:      // overflow
        _nop();
        break;
    default:
        break;
    }
}
