#include <msp430fr6989.h>
#include <stdio.h>
#include <stdlib.h>
int numberMeasurements = 20;
unsigned int testValue;
unsigned int test[20];
int abweichung = 0;
double durchschnitt;
unsigned long i = 0;
unsigned long time = 0;
void initADC( );
void initADCDifferential( );
void testMeasurement();
void initADCConfig( int, int, int, int, int, int, int);
void doConversionPolling( );
void doConversionInterrupt( );
void loopConversion();
void initTimer();
void main( )
{
    __enable_interrupt();
  PM5CTL0 =  0;
  FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
  CSCTL0 = CSKEY;
  //CSCTL2 = 0x0100;
  CSCTL2 = 0x0100 | 0x0003 ; // Select VLOCLK as source for ACLK, select DCOCLK as MCLKLK
  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
  initADC();
  while(1){
  __delay_cycles(1000);
  testMeasurement();
  }
  //initADCDifferential( );
  //loopConversion();
}

void loopConversion(){
    initTimer();
    while(i < 1000){
    // doConversionPolling( );
    doConversionInterrupt( );
    i++;
    }
    time = TA0R;
}

void testMeasurement(){
    for(i = 0; i < numberMeasurements; i++){
    doConversionPolling( );
    test[i] = ADC12MEM0 & 0x0FFF;
    }
    for(i = 1; i < numberMeasurements; i++){
        abweichung += abs(test[i-1] - test[i]);
    }
    durchschnitt = abweichung;
    durchschnitt /= 19.0;
    _no_operation();
}

void doConversionPolling( ) {
    ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
    ADC12CTL0 |= ADC12SC| ADC12ENC;       // Start the conversion
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY) { }
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.
}

void doConversionInterrupt(){
    ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
    ADC12CTL0 |= ADC12SC | ADC12ENC;       // Start the conversion
    __bis_SR_register(LPM3_bits + GIE);    // Low Power Mode 3 with interrupts enabled
}

void initADC() {
    ADC12IER0 |= ADC12IE0;   // Enable ADC conv complete interrupt
    ADC12CTL0 |= ADC12SHT0_1 | ADC12ON; // Select 512 ADC cykles as SHT, Turn ADC on, Enable ADC
    ADC12CTL1 |= ADC12SHP | ADC12SSEL0;  // select pulse sample mode, set clock to aclk.
    ADC12MCTL0 |= ADC12INCH_4;
    ADC12MCTL0 |= ADC12VRSEL_1 | ADC12INCH_4; // Set Upper Reference voltage to internal Ref Voltage, Select Channel A4 (8.7) for ADC
    REFCTL0 |= REFON | REFVSEL_2;           // Turn on internal Reference Generator, internal ref = 2 V
      while( REFCTL0 & REFGENBUSY){ // Wait for refernce to settle
      }
}

void initADCDifferential() {
    ADC12IER0 = ADC12IE0;               // Enable ADC conv complete interrupt
    ADC12CTL0 = ADC12SHT0_1 | ADC12ON;  // Select 64 ADC cykles as SHT, Turn ADC on
    ADC12CTL1 |= ADC12SHP | ADC12SSEL0;               // select pulse sample mode, set clock to aclk
    ADC12MCTL0 |= ADC12INCH_4 | ADC12DIF ; // Select Channel A4 (8.7) and A5 (8.6) for ADC, select Differential Mode

}

/*
 * Sample and hold time:
 * ADC12MEM0 to ADC12MEM7 and ADC12MEM24 to ADC12MEM31 ADC12CTL0 = 0xd0dd to 0xdFdd
 * while 0xd0dd means 4 ADC12CLK cycles and 0xdAdd the  maximum of 512 ADC12ClK cycles
 * ADC12MEM8 to ADC12MEM23 ADC12CTL0 = 0x0ddd to 0xFddd
 * while 0x0ddd means 4 ADC12CLK cycles and 0xAddd the  maximum of 512 ADC12ClK cycles
 */

/*
 * Channel selection controlled by ADC12MCTL0 register Channels A0 to A31
 * ADC12MCTL0 = 0  equals  channel A0, ADC12MCTL0 = 31 equals channel A31 selected.
 */

/*
 * Clock Source
 * Select Clock Soure with the ADC12CLT1 Register bits 4-3
 * MODOSC = 0x0000 = ADC12SSEL_0
 * ACLK = 0x0008 = ADC12SSEL_1
 */

 /*
  * Reference Voltage
  * Select Reference Voltage Source
  * ADC12MCTL0 = 0x0000 = VR+ = AVCC, VR- = AVSS
  * ADC12MCTL0 = 0x0080 = VR+ = VREF buffered, VR- = AVSS
  *
  * Differential Mode: ADC
  * Differential Mode On  ADC12MCTL0 = 0x2000
  *
  * Internal Ref on: REFCTL0 = 0x0001
  * Select internal Ref:
  * REFCTL0 = 0x0000 1,2 V, REFCTL0 = 0x0010 2 V REFCTL0 = 0x0020 2,5 V
  *
  *
  */

void initADCConfig(int channel, int selectReferenceVoltage, int internalReferenceVoltage, int internalRefOn, int sampleHoldTime, int clockSource, int differentialMode){
    ADC12CTL0 = ADC12ON | sampleHoldTime; // Turn ADC on, Select sample and Hold time
    ADC12CTL1 = ADC12SHP | clockSource ;  // select pulse sample mode, select the clockSource
    ADC12MCTL0 = channel | selectReferenceVoltage | differentialMode;  // Set Upper Reference voltage to internal Ref Voltage, Select Channel for ADC
    REFCTL0  = internalRefOn | internalReferenceVoltage;
    while( REFCTL0 & REFGENBUSY){ // Wait for refernce to settle
    }
}
void selectADCchannel(int channel){
    ADC12MCTL0 |= channel;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    if(ADC12IFG0) {
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.x
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}

void initTimer( ){
    TA0CTL = TACLR;
    TA0CTL = TASSEL_1 |  MC_2 ; // Select ACLK as timer clock source, Up mode, TB start.
}
