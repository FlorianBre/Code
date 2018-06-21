#include <msp430.h> 
#include <lib/Timer.h>
#include <lib/ADC.h>
/*
 * ---- Pinout ----
 * PWM Signal: P2.1
 * Transmission Gates in Reihe P2.0
 * PMOS Cap entladen P4.3
 */

unsigned int count = 0;
unsigned int a;
void main(void)
{
    __enable_interrupt();
    P1DIR |= BIT5;
    P4DIR |= BIT3;
    P4OUT &= ~BIT3;
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    // Select clock cycles, clock source, dutycycle, outmode, additional outmode
    timerInitPWMB0(133, TASSEL_2, 0.95, OUTMOD_3, OUTMOD_0);
    TB0CCTL0 |= CCIE;
    adcInitSingle(ADC12SSEL_2, 0, ADC12SHT0_15, ADC12VRSEL_0, 0, ADC12INCH_7, ADC12IE0, ADC12SHS_0);
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B(void)
{
    if(count > 100){
        count = 0;
        P1OUT ^= BIT5;
        // Timer Function
        P2SEL1 |= BIT0;
        TB0CCTL6 = CCIE | OUTMOD_7;
        // Close PMOS above Cap.
        P4OUT |= BIT3;
    }
    count++;
}


// TimerB Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
    case TB0IV_NONE:
        break;              // No interrupt
    case TB0IV_TB0CCR1: break;              // TB0CCR1 interrupt
    case TB0IV_TB0CCR2: break;              // TB0CCR2 interrupt
    case TB0IV_TB0CCR3: break;              // TB0CCR3 interrupt
    case TB0IV_TB0CCR4: break;              // TB0CCR4 interrupt
    case TB0IV_TB0CCR5:
        break;              // TB0CCR5 interrupt
    case TB0IV_TB0CCR6:
        P1OUT ^= BIT5;
        P2OUT |= BIT0;
        // Select GPIO Function
        P2SEL1 &= ~BIT0;
        adcMeasurementInterrupt();
        break;              // TB0CCR6 interrupt
    case TB0IV_TB0IFG:      // overflow

        break;
    default:
        break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:        break;        // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG:  break;        // Vector  2:  ADC12MEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG:  break;        // Vector  6:  ADC12BHI
    case ADC12IV_ADC12LOIFG:  break;        // Vector  8:  ADC12BLO
    case ADC12IV_ADC12INIFG:  break;        // Vector 10:  ADC12BIN
    case ADC12IV_ADC12IFG0:                 // Vector 12:  ADC12MEM0 Interrupt
        P1OUT ^= BIT5;
        TB0CCTL6 &= ~CCIE;
        a = ADC12MEM0;
        // Open PMOS to discharge Cap.
        P4OUT &= ~BIT3;
        // Open transmission Gate
        P2OUT &= ~BIT0;
        _nop();
      break;
    case ADC12IV_ADC12IFG1:   break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:   break;        // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3:   break;        // Vector 18:  ADC12MEM3
    case ADC12IV_ADC12IFG4:   break;        // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5:   break;        // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6:   break;        // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7:   break;        // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8:   break;        // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9:   break;        // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10:  break;        // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11:  break;        // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12:  break;        // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13:  break;        // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14:  break;        // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15:  break;        // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16:  break;        // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17:  break;        // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18:  break;        // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19:  break;        // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20:  break;        // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21:  break;        // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22:  break;        // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23:  break;        // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24:  break;        // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25:  break;        // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26:  break;        // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27:  break;        // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28:  break;        // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29:  break;        // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30:  break;        // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31:  break;        // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;        // Vector 76:  ADC12RDY
    default: break;
  }
}


