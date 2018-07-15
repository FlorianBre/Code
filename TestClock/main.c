#include <msp430fr6989.h>

void main(void)
{
__enable_interrupt();
PM5CTL0 =  0;
FRCTL0 = FWPW | NWAITS_1; // Defines number of waitstates.
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer.
CSCTL0 = CSKEY;
CSCTL1 =  DCORSEL | DCOFSEL_6 ; // Set DCO to high speed, select  24 MHz
CSCTL3 = DIVM_0; // Divide MCLK by 0
CSCTL2 = 0x0100 | 0x0003 ; // Select VLOCLK as source for ACLK, select DCOCLK as MCLK
P1DIR = 0x01; // Configure Port pin.
P1OUT = 0x01;

     while(1){
    __delay_cycles(24000000);
     P1OUT ^= 0x01;
     }
    }


