/*
 * ADCMeasurement.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
#include <lib/ADC.h>
#include <msp430.h>
void adcInitSingle(int clockSelect0, int clockSelect1, int differentialModeSelect, int sampleHoldSelect, int refSelect, int intRefSelecet, int channelSelect, int interruptEnable, int triggerSelect){
    // Enable or disable ADC complete interrupt.
    ADC12IER0 |= interruptEnable;
    ADC12CTL0   = sampleHoldSelect | ADC12ON;    // Select SHT, Turn ADC on, Enable ADC
    if(triggerSelect > 0){
        ADC12CTL0 |= ADC12ENC;
    }
    ADC12MCTL0 = refSelect | channelSelect | differentialModeSelect ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel
    // One trigger for each conversion.
    ADC12CTL0 &= ~ADC12MSC;
    // select pulse sample mode, select clock source, select trigger source, select single conversion Mode
    ADC12CTL1 = ADC12SHP | clockSelect0 | clockSelect1 | triggerSelect | ADC12CONSEQ_0;
    // Select single channel conversion.
    //ADC12MCTL0 |= refSelect | channelSelect | differentialModeSelect ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel
    if(refSelect == ADC12VRSEL_1) {
        REFCTL0 |= REFON | intRefSelecet;   // Turn on internal Reference Generator, select Reference
        while( REFCTL0 & REFGENBUSY){ };    // Wait for refernce to settle
    }
    // Calculates the ADC correction factor.
    // return *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0;
}

void adcInitSequence(int clockSelect0, int clockSelect1, int differentialModeSelect, int sampleHoldSelect, int refSelect, int intRefSelecet, int interruptEnable, int triggerSelect, int startChannel, int endChannel){
    // Trigger multiple conversion with one trigger.
    ADC12CTL0 = sampleHoldSelect | ADC12ON | ADC12MSC;
    if(triggerSelect > 0){
        ADC12CTL0 |= ADC12ENC;
    }
    // Select SHT, Turn ADC on, Enable ADC
    // select pulse sample mode, select clock source, select trigger source, select sequence of channels mode.
    ADC12CTL1 = ADC12SHP | clockSelect0 | clockSelect1 | triggerSelect | ADC12CONSEQ_1;
    int sequenceLength = endChannel - startChannel;
    int i;
    // Initialize the memory controll
    for(i = 0; i <= sequenceLength; i++){
        *(&ADC12MCTL0+i) = startChannel+i | refSelect | differentialModeSelect;
    }
    *(&ADC12MCTL0+i-1) |=   ADC12EOS;
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
void adcStartMeasurement(){
    ADC12CTL0 &= ~ADC12SC;                 // Clear the start bit.
    ADC12CTL0 |= ADC12SC | ADC12ENC;       // Start the conversion.
}

