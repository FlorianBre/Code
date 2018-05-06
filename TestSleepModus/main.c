#include <msp430.h>

void initTimerWakeUp(int);
void initPortInterruptWakeUp();

void main(void)
{
/*
 * Select Clock for Timer Wake Up
 * 0: TimerClk = Vlo (12 kHz)
 */
const int clockSelect = 0;
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer.
P1DIR= 0xFF; // Configure Port pin.
P1OUT = 0x01;
initTimerWakeUp(clockSelect);
// initPortInterruptWakeUp();
while(1) {
    __bis_SR_register(LPM0_bits + GIE);
    P1OUT ^= 0x01; // Toggle LED
    }
}

void initPortInterruptWakeUp() {
        P1DIR &= ~BIT7; //Set pin 1.7 to the input direction. Pin 1.7 for starting the adc.
        P1IE |= BIT7;   // P1.7 interrupt enabled
        P1IES |= BIT7; // P1.7 lo/hi edge
        P1IFG &= ~BIT7; // P1.7 IFG cleared
}

void initTimerWakeUp(int clockSelect){
    TA0CCTL0 = CCIE; // Capture compare interrupt enable.
    TA0CTL = TASSEL_1; // Select ACLK as timer clock source.
    TA0CTL |= MC_1; // Up mode, TB start.
    // BCSCTL1 |= DIVA0 + DIVA1; // Divide ACLK by 8
    // BCSCTL3 =  0x00;
    // Select VLOCLK (12 KHz Resonator)
    if(clockSelect == 0){
    BCSCTL1 &= ~XTS;
    BCSCTL3 |= LFXT1S1;
    BCSCTL3 &= ~LFXT1S0;
    }
    TA0CCR0 = 48000;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
// TA0CTL &=~(MC_1); // Stop Timer_B
_BIC_SR(LPM3_EXIT);
__bic_SR_register_on_exit(LPM0_bits+GIE); // Clear LPM bits upon ISR Exit
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~BIT7; // P1.7 IFG cleared
    _BIC_SR(LPM3_EXIT);
    __bic_SR_register_on_exit(LPM0_bits+GIE); // Clear LPM bits upon ISR Exit
}



