#include <msp430fr6989.h>
unsigned int testValue;
void initADC( );
void initADCDifferential( );
void initADCConfig( int, int, int, int, int, int, int);
void doConversionPolling( );
void doConversionInterrupt( );
void main( )
{
  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
  // initADC();
  initADCDifferential( );
  ADC12CTL0 |= ADC12ENC; // Enable conversion
  while(1){
  doConversionPolling( );
  // doConversionInterrupt( );
  _no_operation();
}
}
void doConversionPolling( ) {
    ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
    ADC12CTL0 |= ADC12SC;       // Start the conversion
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY) { }
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.
    __no_operation();
}

void doConversionInterrupt(){
    ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
    ADC12CTL0 |= ADC12SC;       // Start the conversion
    __bis_SR_register(LPM3_bits + GIE);    // Low Power Mode 3 with interrupts enabled
}

void initADC() {
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
    ADC12CTL0 |= ADC12SHT0_4 | ADC12ON; // Select 64 ADC cykles as SHT, Turn ADC on
    ADC12CTL1 = ADC12SHP;  // select pulse sample mode.
    ADC12MCTL0 |= ADC12VRSEL_1 | ADC12INCH_4; // Set Upper Reference voltage to internal Ref Voltage, Select Channel A4 (8.7) for ADC
    REFCTL0 |= REFON | REFVSEL_2;           // Turn on internal Reference Generator, internal ref = 2 V
    while( REFCTL0 & REFGENBUSY){ // Wait for refernce to settle
    }
}

void initADCDifferential() {
    ADC12IER0 = ADC12IE0;                  // Enable ADC conv complete interrupt
    ADC12CTL0 = ADC12SHT0_15 + ADC12ON; // Select 64 ADC cykles as SHT, Turn ADC on
    ADC12CTL1 = ADC12SHP;  // select pulse sample mode.
    ADC12MCTL0 |= ADC12INCH_4 | ADC12DIF ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel A4 (8.7) for ADC
    while( REFCTL0 & REFGENBUSY){ // Wait for refernce to settle
    }
}

void initADCConfig(int channel, int selectReferenceVoltage, int internalReferenceVoltage, int internalRefOn, int sampleHoldTime, int clockSource, int differentialMode){
    ADC12CTL0 = ADC12ON | sampleHoldTime; // Turn ADC on, Select sample and Hold time
    ADC12CTL1 = ADC12SHP | clockSource ;  // select pulse sample mode, select the clockSource
    ADC12MCTL0 = channel | selectReferenceVoltage | differentialMode;  // Set Upper Reference voltage to internal Ref Voltage, Select Channel for ADC
    REFCTL0  = internalRefOn | internalReferenceVoltage;
    __delay_cycles(100);     // delay to allow Ref to settle.
}
void selectADCchannel(int channel){
    ADC12MCTL0 |= channel;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.
    __bic_SR_register_on_exit(LPM3_bits); // Clear LPM bits upon ISR Exit
}


