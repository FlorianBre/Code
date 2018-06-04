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
#define ADR2 (unsigned char *) 0x00200A
void main(void)

{
    __enable_interrupt();
    PM5CTL0 = 0;
    // Set trigger to external trigger
    //DMACTL0 |= DMA1TSEL_31;
    //Set trigger to software controlled trigger
    DMACTL0 |= DMA1TSEL_0;
    // Single transfer mode, destination adress unchanged after tranfer, source adress unchanged after transfer.
    // destination byte, source byte, enable DMA
    DMA0CTL |=  DMADT_0 | DMADSTINCR_0 | DMASRCINCR_0 | DMADSTBYTE | DMASRCBYTE | DMAEN;
    DMA0SA = ADR1;
    DMA0DA = ADR2;
    test = *CAL_ADC_OFFSET + (*CAL_ADC_GAIN_FACTOR + *ADC_ADC12VREF_FACTOR) / 32768.0  ;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int a = 512;
    // Write Value into Adress.
	*ADR1 = a;
	// Read value from Adress
	 test1 = *ADR1;
	 test2 = *ADR2;
	 _no_operation();
	 // Trigger DMA by software request.
	 DMA0CTL |= DMAREQ;
	 test1 = *ADR1;
	 test2 = *ADR2;
	 _no_operation();
    _no_operation();

}
