#include <msp430fr6989.h>
#include <lib/Timer.h>
#include <lib/ADC.h>
void initTimerWakeUp();
void initPortInterruptWakeUp();
void lowPowerMode(int);
void pullDownPorts();
void pullUpPorts();
void outputHighPorts();
void init();
void select1MhzMCLK();
void select16MhzMCLK();
void selectACLKVlo();
void selectACLKLXFT();
void turnRAMOff();
void turnMPUoff();
void turnRefOn();
void activeMulti();

void main(void)
{

    init();


    // LPM4 Idle
    lowPowerMode(4);

    //LPM4 externer Timer trigger
    /*TB0CCR0 = 65530; // Period
    TB0CTL |= TASSEL_3 | MC_2; // Select timer clock source,Count up to the value in TB0CCR0.
    // Configure Pin as TB0 Trigger
    P3DIR &= ~BIT3;
    P3SEL1 = BIT3;
    P3SEL0 = BIT3;
    lowPowerMode(4); */


    // LPM3 Idle VLO
    //selectACLKVlo();
    //lowPowerMode(3);

    // LPM3 Idle LFXT
    //lowPowerMode(3);

    //LPM3 Timer Vlo
   /* selectACLKVlo();
    TB0CCR0 = 65530; // Period
    TB0CTL |= TASSEL_1 | MC_1; // Select timer clock source,Count up to the value in TB0CC
    lowPowerMode(3);
    */

    /*selectACLKVlo();
    timerInitPWMB0(2, TASSEL_1, 0.5, OUTMOD_3);
    lowPowerMode(3); */

    // LPM3 Referenzspannung an
    //turnRefOn();
    //lowPowerMode(3);

    // Idel 1 Mhz DCO
    /*select1MhzMCLK();
    while(1){
        _nop();
    } */

    // Idel 16 Mhz DCO
    /*select16MhzMCLK();
    while(1){
        _nop();
    } */

    // Multiplication 1 Mhz DCO
   // select1MhzMCLK();
   // activeMulti();

    // Multiplication 16 Mhz DCO
      //select16MhzMCLK();
      //activeMulti();

    // ADC software triggered.

    // All ADC channels, maximal SHtime
    /*adcInitSequence(ADC12SSEL_3, 0, ADC12SHT0_15, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_8,  ADC12INCH_14);
    //selectACLKVlo();
    select16MhzMCLK();
    ADC12CTL0 &= ~ADC12SC;                 // Clear the start bit.
    ADC12CTL0 |= ADC12SC | ADC12ENC;       // Start the conversion
    P9SEL0 = 0xFF;
    P9SEL1 = 0xFF;
    while(1){
    _nop();
    __bis_SR_register(LPM3_bits + GIE);
    }
    */
}

void init(){
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    pullDownPorts();
    __enable_interrupt();


}

void select1MhzMCLK(){
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    CSCTL0 = CSKEY;
    CSCTL1 = DCOFSEL_0; // Set DCO to high speed, select  24 MHz
    CSCTL3 = DIVM_0; // Divide MCLK by 0
}

void select16MhzMCLK(){
    FRCTL0 = FWPW | NWAITS_1; // Defines number of waitstates.
    CSCTL0 = CSKEY;
    CSCTL1 = DCORSEL | DCOFSEL_4; // Set DCO to high speed, select  16 MHz
    CSCTL3 = DIVM_0; // Divide MCLK by 0
}

void selectACLKVlo(){
    CSCTL0 = CSKEY;
    CSCTL2 = SELA_1; // Select VLOCLK as source for ACLK
}

void selectACLKLXFT(){
    CSCTL0 = CSKEY;
    CSCTL2 = SELA_0; // Select LFXT as source for ACLK
}

void turnRAMOff(){
    RCCTL0 =  RCKEY | RCRS0OFF_1;
    PMMCTL0 = 0xA500;
}

void turnMPUoff(){
    MPUCTL0 = 0xA500;
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
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0x00;
    P10DIR = 0x00;
    P10REN = 0xFF;
    P10OUT = 0x00;
    PJDIR = 0xFF;
}

void pullUpPorts(){
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0xFF;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0xFF;
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0xFF;
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0xFF;
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0xFF;
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0xFF;
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0xFF;
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0xFF;
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0xFF;
    P10DIR = 0x00;
    P10REN = 0xFF;
    P10OUT = 0xFF;
    PJDIR = 0xFF;
}





void outputHighPorts(){
    P1DIR = 0xFF;
    P1OUT = 0xFF;
    P2DIR = 0xFF;
    P2OUT = 0xFF;
    P3DIR = 0xFF;
    P3OUT = 0xFF;
    P4DIR = 0xFF;
    P4OUT = 0xFF;
    P5DIR = 0xFF;
    P5OUT = 0xFF;
    P6DIR = 0xFF;
    P6OUT = 0xFF;
    P7DIR = 0xFF;
    P7OUT = 0xFF;
    P8DIR = 0xFF;
    P8OUT = 0xFF;
    P9DIR = 0xFF;
    P9OUT = 0xFF;
    P10DIR = 0xFF;
    P10OUT = 0xFF;
}
void turnRefOn(){
    REFCTL0 |= REFON | REFVSEL_2;           // Turn on internal Reference Generator, internal ref = 2 V
    while( REFCTL0 & REFGENBUSY){} // Wait for refernce to settle
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


void activeMulti(){
    long a = 2;
    while(1){
        a *= 2;
        if(a > 2000000000){
            a = 2;
        }
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




#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
    ADC12IFGR0 = 0;
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;
}
