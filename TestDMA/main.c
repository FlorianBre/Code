#include <msp430.h>
#include <lib/ADC.h>
#define ADR1 (unsigned long *) 0x1C20
#define ADR2 (unsigned long *) 0x1C40
#define ADR3 (unsigned long *) 0x1C41
#define ADRADC0 (unsigned long *) &ADC12MEM0
#define ADRADC1 (unsigned long *) &ADC12MEM1
#define ADRADC2 (unsigned long *) &ADC12MEM2
#define TIMADR0 (unsigned long *) &TA0CCR0
#define TIMADR1 (unsigned long *) &TA1CCR0
unsigned int test1;
unsigned int test2;
unsigned int test3;
unsigned int test4;
void softwareTrigger();
void DMAinit(unsigned int, volatile unsigned int *, volatile unsigned int *, unsigned int, unsigned int, unsigned int);
void adcTrigger();
void timerTrigger();
void initTimer();
void main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
    //adcTrigger();
    softwareTrigger();
    //timerTrigger();
}
void DMAinit(unsigned int triggerSource, volatile unsigned int *sourceAdr, volatile unsigned int *desAdr, unsigned int sourceIncrement, unsigned int destinationIncrement, unsigned int blockSize ){
    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long ) sourceAdr);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) desAdr);
    // Select ADC end of conversion.
    DMACTL0 |= triggerSource;
    // Single Block transfer
    DMA0SZ = blockSize;
    // Single Transfer Mode
    // No increment or decrement of soure and Destination adress
   // DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMADSTBYTE | DMASRCBYTE; // Rpt, inc
    DMA0CTL = DMADT_0 | sourceIncrement | destinationIncrement; // Rpt, inc
    DMA0CTL |= DMAEN; // Enable DMA0
}
void adcTrigger(){
    DMAinit(DMA0TSEL_26, &ADC12MEM0, &ADC12MEM1, DMASRCINCR_0,  DMADSTINCR_0, 1);
    test1 = *ADRADC0;
    test2 = *ADRADC1;
    test3 = ADC12MEM0 & 0x0FFF;
    _no_operation();
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_1 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_4);
    adcMeasurementPolling();                // Trigger block transfer
    test1 = *ADRADC0;
    test2 = *ADRADC1;
    test3 = ADC12MEM0 & 0x0FFF;
    _no_operation();
}

void timerTrigger(){
      unsigned int a = 42;
      *ADRADC0 = a;
      DMAinit(DMA0TSEL_1, &ADC12MEM0, &ADC12MEM1, DMASRCINCR_0,  DMADSTINCR_0, 1);
       test1 = *ADRADC0;
       test2 = *ADRADC1;
       _no_operation();
       initTimer();
       __delay_cycles(1000000);
       test1 = *ADRADC0;
       test2 = *ADRADC1;
       _no_operation();
}

void initTimer(){
    TA0CTL = TACLR;
    TA0CCR0 = 10;
    // Select Aclk, count to TA0CCR0;
    TA0CTL |= TASSEL_1 |  MC_1;
}

void softwareTrigger()
{
    DMAinit(DMA0TSEL_0, &TA0CCR0, &ADC12MEM0, DMASRCINCR_0,  DMADSTINCR_3, 3);
    unsigned int a = 23;
    *TIMADR0 = a;
    test1 = *TIMADR0;
    test2 = *ADRADC0;
    test3 = *ADRADC1;
    test4 = *ADRADC2;
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    test1 = *TIMADR0;
    test2 = *ADRADC0;
    test3 = *ADRADC1;
    test4 = *ADRADC2;
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    test1 = *TIMADR0;
    test2 = *ADRADC0;
    test3 = *ADRADC1;
    test4 = *ADRADC2;
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    test1 = *TIMADR0;
    test2 = *ADRADC0;
    test3 = *ADRADC1;
    test4 = *ADRADC2;
    _no_operation();
}



