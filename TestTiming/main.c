#include <msp430fr6989.h>
void initTimer( );
unsigned long time;
void main(void)
{
__enable_interrupt();
PM5CTL0 =  0;
FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer.
CSCTL0 = CSKEY;
CSCTL1 =  DCORSEL | DCOFSEL_2 ; // Set DCO to high speed, DCO 8 MHz
CSCTL3 = DIVM_0; // Divide MCLK by 0
CSCTL2 = SELA_2 | SELM_3 | SELS_3  ; // Select VLOCLK as source for ACLK, select DCO as MCLK source,  select DCOCLK as SMCLK source,

P1DIR |= BIT3 | BIT7; // Configure Port pin.
P1OUT = 0x00;
// initTimer();
     while(1){
     __delay_cycles(16000000);
     _no_operation();
   //  time = TA0R; // Get the time which was written in the timer register.
     }
    }

void initTimer( ){
    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 |  MC_2 ; // Select SMCLK as timer clock source, Up mode, TB start.
}
