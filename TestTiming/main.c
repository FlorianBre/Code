#include <msp430fr6989.h>
void initTimer( );
void test(volatile unsigned int * adress, volatile unsigned char * adress2 );
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
CSCTL3 = DIVA_3; // Divide ACLK by 32;
CSCTL2 = SELA_2 | SELM_3 | SELS_3  ; // Select VLOCLK as source for ACLK, select DCO as MCLK source,  select DCOCLK as SMCLK source,

P1DIR |= BIT3 | BIT7; // Configure Port pin.
P1OUT = 0x00;
test(&TA0CTL, &P1OUT);
// initTimer();
     while(1){
     _no_operation();
   //  time = TA0R; // Get the time which was written in the timer register.
     }
    }

void test(volatile unsigned int * adress, volatile unsigned char * adress2 ){
    *adress = TASSEL_1 |  MC_2 ; // Select SMCLK as timer clock source, Up mode, TB start.
    *adress2 |= BIT0;
}
void initTimer( ){
    TA0CTL = TACLR;
    TA0CTL = TASSEL_1 |  MC_2 ; // Select SMCLK as timer clock source, Up mode, TB start.
    TA0CCTL1 |= CAP | CM_2 | CCIS_0 | SCS | CCIE; // Capturemode on, Capture mode neg Edge, Capture input CCI1A (P1.6), Capture synchronus mode, capture interrupt enable
    P1DIR &= ~BIT6; // P1.6 capture input for stopping the timer.
    P1OUT |= BIT6;
    P1SEL0 |= BIT6;
    P1SEL1 &= ~BIT6;
}
