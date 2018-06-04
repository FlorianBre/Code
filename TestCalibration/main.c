#include <msp430.h> 

double a;
double test;
#define ADC_ADC12VREF_FACTOR (unsigned int *) 0x01A1A
#define ADC_ADC20VREF_FACTOR (unsigned int *) 0x01A1E
#define ADC_ADC25VREF_FACTOR (unsigned int *) 0x01A24
#define CAL_ADC_OFFSET (unsigned int *) 0x01A18
#define CAL_ADC_GAIN_FACTOR (unsigned int *) 0x01A16
void main(void)

{
    test = *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0  ;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    __enable_interrupt();
    PM5CTL0 = 0;
    _no_operation();

}
