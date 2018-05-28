#include <lib/ADC.h>
#include <lib/Timer.h>
#include <msp430fr6989.h>

//---Module Description --------------------------------------------------

/**
 * @file    main.c
 * @author  Florian Bregler
 * @version 2.0
 * @brief   Controlling of a boost converter for input impedance matching
 *          by adjusting the duty cycle .
 *
 * ---- Pinout ----
 *
 */

const double rIn_ohm = 2000;
const double lCoil_H = 1;
int testValue;

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    __enable_interrupt();
    PM5CTL0 = 0;
    _no_operation();
    /*
     * Counter Testing TASSEL_1 = ACLK
     */
    //timerCounterA0(TASSEL_1);
    /*
     * Capture Compare Testing
     * TASSEL_1 = ACLK
     * CCIS_0 = Caputer Compare input 1
     * CM_3 = both edges
     */
    //timerCaptureCompareA0(CCIS_0, TASSEL_1, CM_3);

    /*
     *  ADC testing
     *  ADC12SSEL0 = 1, ADC12SSEL1 = 0 => ADCclk = ACLK
     *  ADC12DIF = 0 = single Mode
     *  ADC12SHT0_1 = SHT 8 ADCCLK
     *  ADC12VRSEL_1 = Select reference as internal
     *  REFVSEL_2 = internal reference = 2,5 V
     *  ADC12INCH_4 = Channel 8.7
     */
    adcInit(ADC12SSEL0, 0, ADC12DIF, ADC12SHT0_1 , ADC12VRSEL_1, REFVSEL_2, ADC12INCH_7);
    __delay_cycles(10000);
    while(1){

    }
}

double CalculateDutyCycle(int vIn, int vOut, int tPeriod)
    {
        return (1-(vIn/vOut))*(2*lCoil_H/(tPeriod*rIn_ohm));
    }

/*
 * Timer ISR.
 */

 #pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    __delay_cycles(10000);
    _no_operation();
    __delay_cycles(10000);
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    // Memory 0 Interrupt
    if(ADC12IFG0) {
    testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.x
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}

