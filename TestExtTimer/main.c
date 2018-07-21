#include <msp430fr6989.h>
#define T_ON_B0 TB0CCR6
#define T_PERIOD_B0 TB0CCR0


void timerInitTestPWMB0();
void timerSetDutyCycleTestB0(double);

void main( ) {
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    __enable_interrupt();
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    // Choose source and destination address of the transfer.
    P3DIR &= ~BIT3;
    P3SEL1 = BIT3;
    P3SEL0 = BIT3;
    timerInitTestPWMB0(10, TASSEL_3, 0.5, OUTMOD_0);
    __bis_SR_register(LPM4_bits + GIE);
}

void timerInitTestPWMB0(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode) {
    T_PERIOD_B0 = periodCycles - 1;
    TB0CTL |= TACLR; // Reset Timer
    TB0CTL |= clockSelect | MC_1; // Select timer clock source,Count up to the value in TB0CCR0.
    TB0CCTL6 |= pwmOutputMode; // Select output mode.
    P2DIR |= BIT0;
    P2SEL0 &= ~BIT0;
    P2SEL1 |= BIT0;
    timerSetDutyCycleTestB0(dutyCycle);

    TB0CTL |= TBIE;
    TB0CCTL0 |= CCIE;
}
void timerSetDutyCycleTestB0(double dutyCycle){
    T_ON_B0 = (T_PERIOD_B0 + 1.0) * dutyCycle;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
    case TB0IV_NONE:
        _nop();
        break;                              // No interrupt
    case TB0IV_TB0CCR1: break;              // TA0CCR1 interrupt
    default: break;
    }
}
