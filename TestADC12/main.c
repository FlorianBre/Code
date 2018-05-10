#include <msp430fr6989.h>
unsigned int testValue;
void initADC( );
void initADCConfig( int, int, int, int, int, int, int);
void doConversionPolling( );
void doConversionInterrupt( );
void main( )
{
  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
  initADC();
  ADC12CTL0 |= ADC12ENC; // Enable conversion
  while(1){
  // doConversionPolling( );
  doConversionInterrupt( );
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
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.
}

void initADC() {
    ADC12CTL0 = ADC12SHT0_4 + ADC12ON; // Select 64 ADC cykles as SHT, Turn ADC on
    ADC12CTL1 = ADC12SHP;  // select pulse sample mode.
    REFCTL0 = REFON +  REFVSEL_2;           // Turn on internal Reference Generator, internal ref = 2 V
    ADC12MCTL0 = ADC12VRSEL_1 + ADC12INCH_4; // Set Upper Reference voltage to internal Ref Voltage, Select Channel A4 (8.7) for ADC
    __delay_cycles(100);                    // delay to allow Ref to settle
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
  */
void initADCConfig(int channel, int selectReferenceVoltage, int internalReferenceVoltage, int internalRefOn, int sampleHoldTime, int clockSource, int differentialMode){
    ADC12CTL0 = ADC12ON | sampleHoldTime; // Turn ADC on, Select sample and Hold time
    ADC12CTL1 = ADC12SHP | clockSource ;  // select pulse sample mode, select the clockSource
    ADC12MCTL0 = channel | selectReferenceVoltage | differentialMode;  // Set Upper Reference voltage to internal Ref Voltage, Select Channel for ADC
    REFCTL0  = internalRefOn | internalReferenceVoltage;
    // delay to allow Ref to settle
    while(~REFGENRDY){

    }
}
void selectADCchannel(int channel){
    ADC12MCTL0 |= channel;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
    __bic_SR_register_on_exit(LPM0_bits+GIE); // Clear LPM bits upon ISR Exit
}


