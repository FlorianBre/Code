/*
 * ADCMeasurement.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
#include <lib/ADC.h>
#include <msp430.h>
void adcInitSingle(unsigned int clockSelect,unsigned int differentialModeSelect,unsigned int sampleHoldSelect,unsigned int refSelect,unsigned int intRefSelecet,unsigned int channelSelect,unsigned int interruptEnable,unsigned int triggerSelect){
    // Enable or disable ADC complete interrupt.
    ADC12IER0 = interruptEnable;
    ADC12CTL0   = sampleHoldSelect | ADC12ON;    // Select SHT, Turn ADC on, Enable ADC
    ADC12MCTL0 = refSelect | channelSelect | differentialModeSelect ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel
    // One trigger for each conversion.
    // select pulse sample mode, select clock source, select trigger source, select single conversion Mode
    ADC12CTL1 = ADC12SHP | clockSelect | triggerSelect | ADC12CONSEQ_0;
    // Select single channel conversion.
    //ADC12MCTL0 |= refSelect | channelSelect | differentialModeSelect ; // Set Upper Reference voltage to internal Ref Voltage, Select Channel
    if(refSelect == ADC12VRSEL_1) {
        REFCTL0 |= REFON | intRefSelecet;   // Turn on internal Reference Generator, select Reference
        while( REFCTL0 & REFGENBUSY){ };    // Wait for refernce to settle
    }
    // Enable Conversion for none Software Trigger
    if(triggerSelect > 0){
        ADC12CTL0 |= ADC12ENC;
    }
    configureChannel(channelSelect);
    // Calculates the ADC correction factor.
    // return *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0;
}

void adcInitSequence(unsigned int clockSelect,unsigned int differentialModeSelect,unsigned int sampleHoldSelect,unsigned int refSelect,unsigned int intRefSelecet,unsigned int interruptEnable, unsigned int triggerSelect,unsigned int startChannel, unsigned int endChannel){
    // Trigger multiple conversion with one trigger.
    ADC12CTL0 = sampleHoldSelect | ADC12ON | ADC12MSC;
    // Select SHT, Turn ADC on, Enable ADC
    // select pulse sample mode, select clock source, select trigger source, select sequence of channels mode.
    ADC12CTL1 = ADC12SHP | clockSelect | triggerSelect | ADC12CONSEQ_1;
    int sequenceLength = endChannel - startChannel;
    int i;
    // Initialize the memory controll
    for(i = 0; i <= sequenceLength; i++){
        unsigned int tmp = startChannel+i;
        *(&ADC12MCTL0+i) = tmp | refSelect | differentialModeSelect;
        configureChannel(tmp);
    }
    *(&ADC12MCTL0+i-1) |=   ADC12EOS;


    if(triggerSelect > 0){
        ADC12CTL0 |= ADC12ENC;
    }
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

void configureChannel(unsigned int InputChannel){
    switch(__even_in_range(InputChannel, ADC12INCH_15)) {
    case ADC12INCH_0:
        P1DIR &= ~BIT0;
        P1SEL0 |= BIT0;
        P1SEL1 |= BIT0;
        break;
    case ADC12INCH_1:
        P1DIR &= ~BIT1;
        P1SEL0 |= BIT1;
        P1SEL1 |= BIT1;
        break;
    case ADC12INCH_2:
        P1DIR &= ~BIT2;
        P1SEL0 |= BIT2;
        P1SEL1 |= BIT2;
        break;
    case ADC12INCH_3:
        P1DIR &= ~BIT3;
        P1SEL0 |= BIT3;
        P1SEL1 |= BIT3;
        break;
    case ADC12INCH_4:
        P8DIR &= ~BIT7;
        P8SEL0 |= BIT7;
        P8SEL1 |= BIT7;
        break;
    case ADC12INCH_5:
        P8DIR &= ~BIT6;
        P8SEL0 |= BIT6;
        P8SEL1 |= BIT6;
        break;
    case ADC12INCH_6:
        P8DIR &= ~BIT5;
        P8SEL0 |= BIT5;
        P8SEL1 |= BIT5;
        break;
    case ADC12INCH_7:
        P8DIR &= ~BIT4;
        P8SEL0 |= BIT4;
        P8SEL1 |= BIT4;
        break;
    case ADC12INCH_8:
        P9DIR &= ~BIT0;
        P9SEL0 |= BIT0;
        P9SEL1 |= BIT0;
        break;
    case ADC12INCH_9:
        P9DIR &= ~BIT1;
        P9SEL0 |= BIT1;
        P9SEL1 |= BIT1;
        break;
    case ADC12INCH_10:
        P9DIR &= ~BIT2;
        P9SEL0 |= BIT2;
        P9SEL1 |= BIT2;
        break;
    case ADC12INCH_11:
        P9DIR &= ~BIT3;
        P9SEL0 |= BIT3;
        P9SEL1 |= BIT3;
        break;
    case ADC12INCH_12:
        P9DIR &= ~BIT4;
        P9SEL0 |= BIT4;
        P9SEL1 |= BIT4;
        break;
    case ADC12INCH_13:
        P9DIR &= ~BIT5;
        P9SEL0 |= BIT5;
        P9SEL1 |= BIT5;
        break;
    case ADC12INCH_14:
        P9DIR &= ~BIT6;
        P9SEL0 |= BIT6;
        P9SEL1 |= BIT6;
        break;
    case ADC12INCH_15:
        P9DIR &= ~BIT7;
        P9SEL0 |= BIT7;
        P9SEL1 |= BIT7;
        break;
    default: break;
    }
}





