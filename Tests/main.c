#include <lib/ADC.h>
#include <lib/Timer.h>
#include <lib/LCD.h>
#include <lib/DMA.h>
#include <msp430fr6989.h>
unsigned int test1;
void twoDMAs();
void completeTest();
void timerTriggeredADC();
void sequenceADC();
void main(void){
    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    //twoDMAs();
    //timerTriggeredADC();
    sequenceADC();
    //completeTest();
}

void twoDMAs(){
    DMAinit0(DMA0TSEL_0, &ADC12MEM0, &MPY, DMASRCINCR_0, DMADSTINCR_0, 1);
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1);
    unsigned int value1 = 15;
    unsigned int value2 = 20;
    ADC12MEM0 = value1;
    ADC12MEM1 = value2;
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    __delay_cycles(100);
    _no_operation();
}

void timerTriggeredADC()
{
    /*
     *  ADC testing
     *  ADC12SSEL0 = 1, ADC12SSEL1 = 0 => ADCclk = ACLK
     *  ADC12DIF = 0 = single Mode
     *  ADC12SHT0_1 = SHT 8 ADCCLK
     *  ADC12VRSEL_1 = Select reference as internal
     *  REFVSEL_2 = internal reference = 2,5 V
     *  ADC12INCH_4 = Channel 8.7
     */
    //   adcInit(0, ADC12SSEL1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14);
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14,0);
    // Select   TBO CCRO as start for conversion
    ADC12CTL1 |= ADC12SHS0;
    // Start timer TB0
    ADC12CTL0 |= ADC12ENC;
    //timerInitCounterB0(TBSSEL_1, MC_1);
    TA0CTL = TACLR;
    TA0CCR0 = 20;
    TA0CCR1 = 10;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
    TA0CTL |= TASSEL_1 |  MC_1;
    __delay_cycles(20000);
    test1 = ADC12MEM0;
    _no_operation();
}

void sequenceADC(){
    // Multiple Samples after triggering
    _no_operation();
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14,0);
    //multiple ADC with one trigger
    ADC12CTL0 |= ADC12MSC;
    // Select sequence of channels mode.
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &ADC12MEM5, DMASRCINCR_3, DMADSTINCR_3, 5);
    ADC12CTL1 |= ADC12CONSEQ_1;
    ADC12MCTL0 |= ADC12INCH_0; //Analog Channel 7
    ADC12MCTL1 |= ADC12INCH_1; //Analog Channel 8
    ADC12MCTL2 |= ADC12INCH_2; //Analog Channel 8
    ADC12MCTL3 |= ADC12INCH_3; //Analog Channel 8
    ADC12MCTL4 |= ADC12INCH_4 | ADC12EOS; //Analog Channel 8
    adcMeasurementPolling();
    // Start conversion
    __delay_cycles(1000);
    _no_operation();
}

void completeTest(){
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14,0);
    // ADC12IER0 |= ADC12IE1;
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPY, DMASRCINCR_3, DMADSTINCR_0, 2);
    DMAinit1(DMA1TSEL_30, &ADC12MEM2, 0, DMASRCINCR_0, DMADSTINCR_0, 1);
    __data16_write_addr((unsigned short) &DMA1DA,(unsigned long) &DMA0DA);
    ADC12MEM2 = OP2;
    ADC12CTL0 |= ADC12MSC;
    // Select sequence of channels mode.
    ADC12CTL1 |= ADC12CONSEQ_1;
    ADC12MCTL0 |= ADC12INCH_7; //Analog Channel 7
    ADC12MCTL1 |= ADC12INCH_8 | ADC12EOS; //Analog Channel 8
    // Select   TBO CCRO as start for conversion
    ADC12CTL1 |= ADC12SHS0;
    // Start timer TB0
    ADC12CTL0 |= ADC12ENC;
    //timerInitCounterB0(TBSSEL_1, MC_1);
    TA0CTL = TACLR;
    TA0CCR0 = 20;
    TA0CCR1 = 10;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
    TA0CTL |= TASSEL_1 |  MC_1;
    // Start conversion
    __delay_cycles(1000000);
    _no_operation();
}
