#include <msp430.h>
#include <lib/ADC.h>
#define ADR1 (unsigned long *) 0x1C20
#define ADR2 (unsigned long *) 0x1C40
#define ADR3 (unsigned long *) 0x1C41
#define ADRADC (unsigned long *) &ADC12MEM0
#define TIMADR (unsigned long *) &TA0CCR0
int test1;
int test2;
int test3;
int test4;
void softwareTrigger();
void initDMA();
void adcTrigger();
void main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
    initDMA();
    // softwareTrigger();
    adcTrigger();


}
void initDMA(){

    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) ADRADC);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) TIMADR);
    // Single Block transfer
    DMACTL0 |= DMA0TSEL_26;
    DMA0SZ = 1;
    // Single Transfer Mode
    // No increment or decrement of soure and Destination adress
   // DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMADSTBYTE | DMASRCBYTE; // Rpt, inc
    DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0; // Rpt, inc
    DMA0CTL |= DMAEN;                         // Enable DMA0
}
void adcTrigger(){
    /*
     *  ADC testing
     *  ADC12SSEL0 = 1, ADC12SSEL1 = 0 => ADCclk = ACLK
     *  ADC12DIF = 0 = single Mode
     *  ADC12SHT0_1 = SHT 8 ADCCLK
     *  ADC12VRSEL_1 = Select reference as internal
     *  REFVSEL_2 = internal reference = 2,5 V
     *  ADC12INCH_4 = Channel 8.7
     */
    test1 = *ADRADC;
    test2 = *TIMADR;
    test3 = ADC12MEM0 & 0x0FFF;
    _no_operation();
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_1 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_4);
    adcMeasurementInterrupt();                // Trigger block transfer
    test1 = *ADRADC;
    test2 = *TIMADR;
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

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    // Memory 0 Interrupt
    if(ADC12IFG0) {
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}
