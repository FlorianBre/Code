#include <msp430f5529.h>
long i = 0;
void initTimerWakeUp();
void busyWaiting();
void initPortInterruptWakeUp();
void lowPowerMode(int);
void pullDownPorts();
void outputDirPorts();
void main(void)
{
__enable_interrupt();
WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
//CSCTL1 =  DCORSEL | DCOFSEL_0; // Set DCO to high speed, select  24 MHz
//CSCTL3 = DIVM_0; // Divide MCLK by 0
UCSCTL4 |= SELA_1; // Select VLOCLK as source for ACLK, SMCLK, DCO for MCLK
//CSCTL2 = SELA_1 | SELS_1 | SELM_3; // Select VLOCLK as source for ACLK, SMCLK, DCO for MCLK
// initTimerWakeUp( );
// initPortInterruptWakeUp();
//__delay_cycles(100000);

//

pullDownPorts();
//outputDirPorts();
// MPUCTL0 = 0xA500;
_nop();
//busyWaiting();
//__delay_cycles(500000);
//initTimerWakeUp();

//RCCTL0 =  RCKEY | RCRS0OFF_1;
//PMMCTL0 = 0xA500;
lowPowerMode(3);
    // while(1){
//REFCTL0 |= REFON | REFVSEL_2;           // Turn on internal Reference Generator, internal ref = 2 V
//   while( REFCTL0 & REFGENBUSY){} // Wait for refernce to settle

   //  }
    }
void outputDirPorts(){
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;
    P4DIR = 0xFF;
    P5DIR = 0xFF;
    P6DIR = 0xFF;
    P7DIR = 0xFF;
    P8DIR = 0xFF;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P3OUT = 0x00;
    P4OUT = 0x00;
    P5OUT = 0x00;
    P6OUT = 0x00;
    P7OUT = 0x00;
    P8OUT = 0x00;
    PJDIR = 0xFF;
}
void pullDownPorts(){
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0x00;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0x00;
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0x00;
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0x00;
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0x00;
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0x00;
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0x00;
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0x00;
    PJDIR = 0xFF;
}

void lowPowerMode(int mode){
    if ( mode == 0){
        __bis_SR_register(LPM0_bits + GIE);
    }
    if ( mode == 1){
        __bis_SR_register(LPM1_bits + GIE);
    }
    if ( mode == 2){
        __bis_SR_register(LPM2_bits + GIE);
    }
    if ( mode == 3){
        __bis_SR_register(LPM3_bits + GIE);
    }
    if ( mode == 4){
        __bis_SR_register(LPM4_bits + GIE);
    }
}


void busyWaiting(){
    while(1){
    while(i < 80000){
           i ++;
       }
      i = 0;
    }
}
void initPortInterruptWakeUp() {
        P1DIR &= ~BIT7; //Set pin 1.7 to the input direction. Pin 1.7 for starting the adc.
        P1IE |= BIT7;   // P1.7 interrupt enabled
        P1IES |= BIT7; // P1.7 lo/hi edge
        P1IFG &= ~BIT7; // P1.7 IFG cleared
}

void initTimerWakeUp(){
    TA0CCTL0 = CCIE; // Capture compare interrupt enable.
    TA0CTL = TASSEL_1; // Select ACLK as timer clock source.
    TA0CTL |= MC_1; // Up mode, TB start.
    TA0CCR0 = 50000;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
    {
    _nop();
    __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
    {
    P1IFG &= ~BIT7; // P1.7 IFG cleared
    _BIC_SR(LPM3_EXIT);
    //__bic_SR_register_on_exit(LPM0_bits+GIE); // Clear LPM bits upon ISR Exit
    }

