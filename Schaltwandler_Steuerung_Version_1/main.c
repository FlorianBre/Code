#include <lib/ADC.h>
#include <lib/Timer.h>
#include <lib/LCD.h>
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
unsigned long time;

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
     * CCIS_0 = Caputer Compare input 0
     * CM_3 = both edges
     */
    // timerCaptureCompareA0(CCIS_0, TASSEL_2, CM_1);

    /*
     *  ADC testing
     *  ADC12SSEL0 = 1, ADC12SSEL1 = 0 => ADCclk = ACLK
     *  ADC12DIF = 0 = single Mode
     *  ADC12SHT0_1 = SHT 8 ADCCLK
     *  ADC12VRSEL_1 = Select reference as internal
     *  REFVSEL_2 = internal reference = 2,5 V
     *  ADC12INCH_4 = Channel 8.7
     */
     //   adcInit(0, ADC12SSEL1, 0, ADC12SHT0_0 , ADC12VRSEL_1, REFVSEL_0, ADC12INCH_14);
    /*
     * PWM testing
     * Amount off period cycles
     * TASSEL_2 = SMCLK
     * dutyCycle
     * OUTMOD_7 = reset/set
     */
    //timerInitPWMA0(30, TASSEL_2, 0.9, OUTMOD_3);
    /*lcdInit();
    char a[] = {"Hello"};
    lcdShowText(a,0,sizeof(a));
    __delay_cycles(1000000);
    lcdClear();
    */
}

double CalculateDutyCycle(int vIn, int vOut, int tPeriod)
    {
        return (1-(vIn/vOut))*(2*lCoil_H/(tPeriod*rIn_ohm));
    }

 #pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    time = TA0R;
    TA0CTL = TACLR | TASSEL_2 |  MC_2 ; // Reset Timer
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    // Memory 0 Interrupt
    if(ADC12IFG0) {
    //testValue = ADC12MEM0 & 0x0FFF;      // Read in lower 12 bits.x
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}

