#include <msp430.h> 

double a;
double test;
int test1;
int test2;
#define ADC_ADC12VREF_FACTOR (unsigned int *) 0x01A1A
#define ADC_ADC20VREF_FACTOR (unsigned int *) 0x01A1E
#define ADC_ADC25VREF_FACTOR (unsigned int *) 0x01A24
#define CAL_ADC_OFFSET (unsigned int *) 0x01A18
#define CAL_ADC_GAIN_FACTOR (unsigned int *) 0x01A16
#define ADR1 (unsigned char *) 0x002000
#define ADR2 (unsigned char *) 0x002004
void main(void)

{
    __enable_interrupt();
    PM5CTL0 = 0;
    test = *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0  ;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    int a = 128;
    // Write Value into Adress.
	*ADR1 = a;
	// Read value from Adress
	 test1 = *ADR1;
	 test2 = *ADR2;
    _no_operation();

}
