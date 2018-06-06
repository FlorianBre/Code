#include <msp430.h>
#include <lib/ADC.h>
#define ADR1 (unsigned long *) 0x1C20
#define ADR2 (unsigned long *) 0x1C40
#define ADR3 (unsigned long *) 0x1C41
#define ADRADC0 (unsigned long *) &ADC12MEM0
#define ADRADC1 (unsigned long *) &ADC12MEM1
#define TIMADR1 (unsigned long *) &TA0CCR0
#define TIMADR2 (unsigned long *) &TA1CCR0
int test1;
int test2;
int test3;
int test4;
void softwareTrigger();
void DMAinit(unsigned int, volatile unsigned int *, volatile unsigned int *);
void adcTrigger();
void main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
    DMAinit(DMA0TSEL_26, &ADC12MEM0, &ADC12MEM1);
    adcTrigger();
    // softwareTrigger();


}
void DMAinit(unsigned int triggerSource, volatile unsigned int *sourceAdr, volatile unsigned int *desAdr ){
    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long ) sourceAdr);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) desAdr);
    // Select ADC end of conversion.
    DMACTL0 |= triggerSource;
    // Single Block transfer
    DMA0SZ = 1;
    // Single Transfer Mode
    // No increment or decrement of soure and Destination adress
   // DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMADSTBYTE | DMASRCBYTE; // Rpt, inc
    DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0; // Rpt, inc
    DMA0CTL |= DMAEN; // Enable DMA0
}
void adcTrigger(){
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

void softwareTrigger()
{
    char a = 7;
    *ADR1 = a;
    test1 = *ADR1;
    test2 = *ADR2;
    test3 = *ADR3;
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    test1 = *ADR1;
    test2 = *ADR2;
    test3 = *ADR3;
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    test1 = *ADR1;
    test2 = *ADR2;
    test3 = *ADR3;
    _no_operation();
}
