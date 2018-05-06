#include <msp430fr6989.h>
int test;
void initADC();
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;      // Stop WDT
  initADC();
  ADC12CTL0 |= ADC12ENC;                  // Enable conversion
  while(1)
  {
    ADC12CTL0 &= ~ADC12SC;      // Clear the start bit (precautionary)
    ADC12CTL0 |= ADC12SC;       // Start the conversion

    // Poll busy bit waiting for conversion to complete
    while (ADC12CTL1 & ADC12BUSY) {
        __no_operation(); // Could just leave body of loop empty
    }
    int test = ADC12MEM0 & 0x0FFF;      // Read in results if conversion
    __no_operation();                       // SET BREAKPOINT HERE
  }
}

void initADC() {
    ADC12CTL0 = ADC12SHT0_4 + ADC12ON; // Select 64 ADC cykles as SHT, Turn ADC on
    // ADC12CTL1:  Use default settings (just enable sample timer) select pulse sample mode.
    ADC12CTL1 = ADC12SHP;
    REFCTL0 = REFON +  REFVSEL_0;           // Turn on internal Reference Generator, internal ref = 1.2 V
    ADC12MCTL0 = ADC12VRSEL_1 + ADC12INCH_1; // Set Upper Reference voltage to internal Ref Voltage, Select Chanell A1 for ADC
    __delay_cycles(100);                    // delay to allow Ref to settle
}

 /*
// Global variables
unsigned int ADC_value=0;

// Function prototypes
void ConfigureAdc(void);

void main(void)
{
    {
        WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
        BCSCTL1 = CALBC1_1MHZ;          // Set range   DCOCTL = CALDCO_1MHZ;
        BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz
        P1SEL |= BIT3;                  // ADC input pin P1.3
        ConfigureAdc();                 // ADC set-up function call
        __enable_interrupt();           // Enable interrupts.

        while(1)
        {
            __delay_cycles(1000);               // Wait for ADC Ref to settle
            ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
            __bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled
            ADC_value = ADC10MEM;               // Assigns the value held in ADC10MEM to the integer called ADC_value

        }

    }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}

// Function containing ADC set-up
void ConfigureAdc(void)
{

    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    ADC10AE0 |= BIT3;                         // ADC input enable P1.3
}
*/


