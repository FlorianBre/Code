#include <lib/ADC.h>
#include <lib/Timer.h>
#include <lib/LCD.h>
#include <lib/DMA.h>
#include <msp430fr6989.h>
//unsigned int test1;
void twoDMAs();
void completeTest();
void timerTriggeredADC();
void sequenceADC();
void triggerCCRwithDMA();
void triggerDMAwithMultiplier();
void interruptAfterMultiplication();
void completeTestWithInterrupt();
void completeTestWithInterrupt2();
void testDMAMultiplierInterrupt();
void adcSequenceInit(unsigned int, unsigned int);
void testRegisterWrite();
void main(void){
    //__enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    //twoDMAs();
    //timerTriggeredADC();
    //sequenceADC();
    //completeTest();
    //triggerCCRwithDMA();
    //triggerDMAwithMultiplier();
    //interruptAfterMultiplication();
    //completeTestWithInterrupt();
    //completeTestWithInterrupt2();
    //adcSequenceInit(2,5);
    //testRegisterWrite();
    testDMAMultiplierInterrupt();
}

void twoDMAs(){
    DMAinit0(DMA0TSEL_0, &ADC12MEM0, &MPY, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
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
    adcInitSingle(ADC12SSEL_1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14, 0, 0);
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
    //test1 = ADC12MEM0;
    _no_operation();
}
/*
 * Method to test the sequence of channel Mode in Combination with the DMA.
 */
void sequenceADC(){
    // Multiple Samples after triggering
    _no_operation();
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, 0, 0, 2, 10);
    ADC12CTL0 &= ~ADC12SC;                 // Clear the start bit.
    ADC12CTL0 |= ADC12SC | ADC12ENC;       // Start the conversion.
    // Start conversion
    __delay_cycles(10000);
    _no_operation();
}

/*
 * Method to test Timer triggered ADC Sequence of Channel, plus transfering result into MUltiplier Operands
 */
void completeTest(){
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, 0, ADC12SHS_1, 7, 8);
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPYS, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    //__data16_write_addr((unsigned short) &DMA1DA,(unsigned long) &DMA0DA);
    _no_operation();
    TA0CTL = TACLR;
    TA0CCR1 = 1000;
    TA0CCR0 = 2000;
    //ADC12CTL1 |= ADC12SHS_1;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
    _no_operation();
    TA0CTL |= TASSEL_2 |  MC_1;
    // Start conversion
    __delay_cycles(15000);
    _no_operation();
}
/*
 * Method to the Test causing a CCI with a timer and DMA
 */
void triggerCCRwithDMA(){
    TA0CTL = TACLR;
    TA0CCR0 = 100;
    TA0CCR1 = 200;
    TA0CCTL1 |=  CCIE;
    TA0CTL |= TASSEL_2 |  MC_2 ;
    TA0R = 150;
    __delay_cycles(200);
    _no_operation();
}
/*
 * Method to test triggering a DMA after a Multiplication.
 */
void triggerDMAwithMultiplier(){
    ADC12MEM0 = 58;
    DMAinit0(DMA0TSEL_29, &ADC12MEM0, &ADC12MEM4, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    unsigned int multiplier = 0x7234;
    unsigned int operand = 578;
    MPY = multiplier;                        // Load first operand -signed mult
    OP2 = operand;                            // Load second operand
    __delay_cycles(1000);
    _nop();
}

/*
 * Method to test, triggering a CCI after Multiplication with a DMA transfer
 */
void interruptAfterMultiplication(){
    TA0CTL = TACLR;
    TA0CCR0 = 100;
    TA0CCR1 = 200;
    ADC12MEM30 = 150;
    TA0CCTL0 |=  CCIE;
    TA0CTL |= TASSEL_2 |  MC_1;
    DMAinit0(DMA0TSEL_29, &ADC12MEM30, &TA0R, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    unsigned int multiplier = 0x7234;
    unsigned int operand = 578;
    MPY = multiplier;                        // Load first operand -signed mult
    OP2 = operand;                            // Load second operand
    __delay_cycles(200);
    _no_operation();
}

/*
 * Method to test Timer triggered ADC Sequence of Channel, plus transfering result into Multiplier Operands.
 * Plus Causing an interrupt after the Multiplier is finished
 */
void completeTestWithInterrupt(){
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, 0, ADC12SHS_1, 7, 8);
    _no_operation();
    // DMA for transferring the first ADC measurement to Operand 1 of the hardware multiplier.
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPYS, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    // DMA for transferring the second ADC measurement to Operand 2 of the hardware multiplier and triggering the multiplication.
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    // DMA triggered by hardware multiplier, causing an interrupt after the transfer.
    //DMAinit2(DMA2TSEL_29, &TA2CCR0, &DMA2CTL, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    DMAinit2(DMA2TSEL_29, &ADC12MEM1, &ADC12MEM5, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    //DMAinit2(DMA2TSEL_29, &TA2CCR0, &DMA2CTL, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    //MPY32CTL0 |= MPYDLY32 | MPYDLYWRTEN | MPYM_0;
    //TA2CCR0 |= DMA2CTL | DMAIE;
    _no_operation();
    TA0CTL = TACLR;
    TA0CCR1 = 15000;
    TA0CCR0 = 20000;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
    // Start conversion
    //__bis_SR_register(SCG0);
    TA0CTL |= TASSEL_1 |  MC_1;
    __bis_SR_register(LPM3_bits + GIE);
    _no_operation();
}

void completeTestWithInterrupt2(){
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, 0, ADC12SHS_0, 7, 8);
    // DMA for transferring the first ADC measurement to Operand 1 of the hardware multiplier.
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPYS, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    // DMA for transferring the second ADC measurement to Operand 2 of the hardware multiplier and triggering the multiplication.
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    // DMA triggered by hardware multiplier, causing an interrupt after the transfer.
    //DMAinit2(DMA2TSEL_29, &ADC12MEM18, &ADC12MEM18, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    _no_operation();
    adcStartMeasurement();
    //__bis_SR_register(LPM3_bits + GIE);
    _no_operation();
    // Start conversion

}

volatile unsigned int *adr;
void adcSequenceInit(unsigned int start, unsigned int end){
    _nop();
    int sequenceLength = end - start;
    int i;
    for(i = 0; i <= sequenceLength; i++){
        *(&ADC12MCTL0+i) = start+i;
    }
    *(&ADC12MCTL0+i-1) |=  ADC12EOS;
    _nop();
}

void testRegisterWrite(){
    _nop();
    ADC12CTL1 = ADC12CONSEQ_1 | ADC12SHP ;
    _nop();
    ADC12CTL1 = ADC12CONSEQ_2;
    _nop();
}

void testDMAMultiplierInterrupt(){
    /*
    //OP2 = 2;
    //MPY32CTL0 |= MPYDLY32 | MPYDLYWRTEN | MPYM_0;
    //__delay_cycles(1000);
    DMAinit2(DMA2TSEL_29, &ADC12MEM2, &ADC12MEM6, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
     MPY = 2;
    //DMA2CTL |= DMALEVEL;
    // DMAinit2(DMA2TSEL_29, &TA2CCR0, &DMA2CTL, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    //DMAinit2(DMA2TSEL_29, &TA2CCR0, &DMA2CTL, DMASRCINCR_0, DMADSTINCR_0, 2, DMAIE);
    //TA2CCR0 |= DMA2CTL | DMAIE;
   // MPY32CTL0 |= MPYDLY32 | MPYDLYWRTEN | MPYM_0;
    _nop();
    __bis_SR_register(GIE);
    int i;
    for(i = 0; i < 200; i++){
        _nop();
    }
    _nop();
    _nop();
    //unsigned int *a
    // a = READ_SR();

    __bis_SR_register(SCG0);
    __delay_cycles(10000);
    _nop();
    __bis_SR_register(SCG1);
    __delay_cycles(10000);
    _nop();
    __bis_SR_register(GIE);
    __delay_cycles(10000);
    _nop();
    __bis_SR_register(CPUOFF);
    // __bis_SR_register(LPM2_bits + GIE); */

    // Choose source and destination address of the transfer.
    __data16_write_addr((unsigned short) &DMA2SA,(unsigned long ) &ADC12MEM2);
    __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) &ADC12MEM6);
    // Set trigger source to: MPY ready.
    DMACTL1 |= DMA2TSEL_29;
    // Set transfer size to one.
    DMA2SZ = 1;
    // Choose single transfer, no source or destination adress increment, enable DMA interrupt, enable DMA.
    DMA2CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMAIE | DMAEN;
    __bis_SR_register(LPM4_bits + GIE);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    __delay_cycles(1000);
    _no_operation();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(DMAIV,16))
    {
    case 0: break;
    case 2:                           // DMA0IFG = DMA Channel 0
        break;
    case 4:
        _no_operation();
        break;                          // DMA1IFG = DMA Channel 1
    case 6:
        _no_operation();
        break;                          // DMA2IFG = DMA Channel 2
    default: break;
    }
}

/*
#include <msp430fr6989.h>

void main( ) {
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    // Choose source and destination address of the transfer.
    __data16_write_addr((unsigned short) &DMA1SA,(unsigned long) &ADC12MEM1);
    __data16_write_addr((unsigned short) &DMA1DA,(unsigned long) &ADC12MEM2);
    // Set trigger source to: MPY ready.
    DMACTL0 |= DMA2TSEL_29;
    // Set transfer size to one.
    DMA1SZ = 1;
    // Choose single transfer, no source or destination adress increment, enable DMA interrupt, enable DMA.
    DMA1CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMAIE | DMAEN;
    __bis_SR_register(LPM3_bits | GIE);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(DMAIV,16))
    {
    case 0: break;
    case 2:                           // DMA0IFG = DMA Channel 0
        break;
    case 4:
        _no_operation();
        break;                          // DMA1IFG = DMA Channel 1
    case 6:
        _no_operation();
        break;                          // DMA2IFG = DMA Channel 2
    default: break;
    }
} */




