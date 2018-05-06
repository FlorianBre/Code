#include <msp430.h>

void initTimerWakeUp();
void initPortInterruptWakeUp();

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer.
P1DIR= 0xFF; // Configure Port pin.
initTimerWakeUp();
initPortInterruptWakeUp();
__bis_SR_register(LPM0_bits + GIE);
P1OUT = 0x00; // Turn LED Off for testing.
}

void initPortInterruptWakeUp() {

}

void InitTimerWakeUp(){
    TA0CCTL0 = CCIE; // Capture compare interrupt enable.
    TA0CTL = TASSEL_1; // Select ACLK as timer clock source.
    TA0CTL |= MC_1; // Up mode, TB start.
    P1OUT = 0x01; // Turn LED On
    BCSCTL1 &= ~XTS;
    // BCSCTL1 |= DIVA0 + DIVA1; // Divide ACLK by 8
    // BCSCTL3 =  0x00;
    // Select VLOCLK (12 KHz Resonator)
    BCSCTL3 |= LFXT1S1;
    BCSCTL3 &= ~LFXT1S0;
    TA0CCR0 = 48000;
}





#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
TA0CTL &=~(MC_1); // Stop Timer_B
_BIC_SR(LPM3_EXIT);
__bic_SR_register_on_exit(LPM0_bits+GIE); // Clear LPM bits upon ISR Exit
}





