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
void triggerCCRwithDMA();
void triggerDMAwithMultiplier();
void interruptAfterMultiplication();
void completeTestWithInterrupt();
void main(void){
    __enable_interrupt();
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
    completeTestWithInterrupt();
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
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &ADC12MEM5, DMASRCINCR_3, DMADSTINCR_3, 5, 0);
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

/*
 * Method to test Timer triggered ADC Sequence of Channel, plus transfering result into MUltiplier Operands
 */
void completeTest(){
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_10 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14,0);
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPYS, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    //__data16_write_addr((unsigned short) &DMA1DA,(unsigned long) &DMA0DA);
    _no_operation();
    ADC12CTL0 |= ADC12MSC;
    // Select sequence of channels mode.
    ADC12CTL1 |= ADC12CONSEQ_1;
    ADC12MCTL0 |= ADC12INCH_7; //Analog Channel 7
    ADC12MCTL1 |= ADC12INCH_8 | ADC12EOS; //Analog Channel 8
    // Select   TAO CCR1 as start for conversion
    ADC12CTL1 |= ADC12SHS0;
    ADC12CTL0 |= ADC12ENC;
    TA0CTL = TACLR;
    TA0CCR1 = 1000;
    TA0CCR0 = 2000;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
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
    adcInit(0, ADC12SSEL1, 0, ADC12SHT0_10 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14,0);
    DMAinit0(DMA0TSEL_26, &ADC12MEM0, &MPYS, DMASRCINCR_0, DMADSTINCR_0, 2, 0);
    DMAinit1(DMA1TSEL_30, &ADC12MEM1, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    DMAinit2(DMA0TSEL_29, &ADC12MEM18, &ADC12MEM18, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    //__data16_write_addr((unsigned short) &DMA1DA,(unsigned long) &DMA0DA);
    _no_operation();
    ADC12CTL0 |= ADC12MSC;
    // Select sequence of channels mode.
    ADC12CTL1 |= ADC12CONSEQ_1;
    ADC12MCTL0 |= ADC12INCH_7; //Analog Channel 7
    ADC12MCTL1 |= ADC12INCH_8 | ADC12EOS; //Analog Channel 8
    // Select   TAO CCR1 as start for conversion
    ADC12CTL1 |= ADC12SHS0;
    ADC12CTL0 |= ADC12ENC;
    TA0CTL = TACLR;
    TA0CCR1 = 1000;
    TA0CCR0 = 2000;
    TA0CCTL1 |= OUTMOD_1;
    // Select Aclk, count to TA0CCR0;
    _no_operation();
    TA0CTL |= TASSEL_2 |  MC_1;
    // Start conversion
    __bis_SR_register(LPM3_bits + GIE);
    _no_operation();
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
    case 2:                                 // DMA0IFG = DMA Channel 0
      break;
    case 4: break;                          // DMA1IFG = DMA Channel 1
    case 6:
        __bic_SR_register_on_exit(LPM3_bits+GIE);
        _no_operation();
        break;                          // DMA2IFG = DMA Channel 2

    case 8: break;                          // DMA3IFG = DMA Channel 3
    case 10: break;                         // DMA4IFG = DMA Channel 4
    case 12: break;                         // DMA5IFG = DMA Channel 5
    case 14: break;                         // DMA6IFG = DMA Channel 6
    case 16: break;                         // DMA7IFG = DMA Channel 7
    default: break;
  }
}



