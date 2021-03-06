/*
 * ADCMeasurement.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
#include <lib/ADC.h>
#include <msp430.h>
double adcInit(int clockSelect0, int clockSelect1, int differentialModeSelect, int sampleHoldSelect, int refSelect, int intRefSelecet, int channelSelect){
    ADC12CTL0 |= sampleHoldSelect | ADC12ON;    // Select SHT, Turn ADC on, Enable ADC
    ADC12CTL1 |= ADC12SHP | clockSelect0 | clockSelect1 ;   // select pulse sample mode, select clock source.
    ADC12MCTL0 |= refSelect | channelSelect | differentialModeSelect ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel
    if(refSelect == ADC12VRSEL_1) {
        REFCTL0 |= REFON | intRefSelecet;   // Turn on internal Reference Generator, select Reference
        while( REFCTL0 & REFGENBUSY){ };    // Wait for refernce to settle
    }
    // Calculates the ADC correction factor.
    return *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0;

}

int adcMeasurementPolling(){
    adcStartMeasurement();
    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY) { }
    return ADC12MEM0 & 0x0FFF;             // Read in lower 12 bits.
}
void adcMeasurementInterrupt() {
    adcStartMeasurement();
    __bis_SR_register(LPM3_bits + GIE);    // Low Power Mode 3 with interrupts enabled
}
static void adcStartMeasurement(){
    ADC12CTL0 &= ~ADC12SC;                 // Clear the start bit.
    ADC12CTL0 |= ADC12SC | ADC12ENC;       // Start the conversion.
}

