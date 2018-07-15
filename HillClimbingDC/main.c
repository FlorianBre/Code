#include <msp430.h> 
#define GLOBAL_IQ 20
#include <lib/Timer.h>
#include <IQmathLib.h>
#include <lib/ADC.h>
const int period = 150;
const double initDuty  = 0.5;
const double rShunt = 1.0;
int calculate = 0;
void init();
void calculatePower();

/**
 * Pinout:
 * PWM: P2.1
 * ADCinit: P1.3 (Controll Signal to start SH circuit, Set High to start)
 * Sample Hold ready (Controll Input Signal when SH circuit is ready for ADC mesasurement): P1.4
 * Captur Compare input (measure T_off) P1.5
 * ADC1 Uin P8.4
 * ADC2 Ucap P9.0
 */

void main(void)
{
    int count = 0;
    int counter = 1;
    init();
    while(1){
        // calculate power after ADC is finished.
        if(calculate == 1){
            calculatePower();
        }

        // count Up when no calculation is pending
        if(counter == 1){
            count ++;
            if (count > 500){
            counter = 0;
            P1OUT |= BIT3;
            }
        }

    }
}


void calculatePower() {
    double Ipeak = ADC12MEM1 / rShunt;
    double power = (TA0CCR1 * Ipeak) / (2 * period)
}

void init(){
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    // PWM Timer
    // Set period cycles, clock select SMCLK, select duty, select outmode reset / set.
    timerInitPWMB0(period, TASSEL_2, initDuty, OUTMOD_7);
    // Timer for measuring Toff
    // clock select SMCLK, select Capture Compare input 0 (P1.5), triggered by rising edge.
    timerCaptureCompareA0(TASSEL_2, CCIS_0, CM_1)
    P1IFG &= ~BIT4;
    P1DIR |= BIT3;
    P1OUT &= ~BIT3;
    P1DIR &= ~BIT4;
    P1IE |= BIT4;   // P1.4 interrupt enabled
    P1IES &= ~BIT4; // P1.4 lo/hi edge
    P1IFG &= ~BIT4; // P1.4 IFG cleared
    adcInitSequence(ADC12SSEL_3, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_7,  ADC12INCH_8);
    __delay_cycles(1000);
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
      break;
    case ADC12IV_ADC12IFG1:
        ADC12IFGR1 &= ~ADC12IFG1;
        P1OUT ^= BIT5;
        // Reset SH circuit
        P1OUT &= ~BIT3;
        // Set flag for starting the calculation of the power.
        calculate = 1;
        break;        // Vector 14:  ADC12MEM1
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

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

  switch (__even_in_range(P1IV,16))
  {
  case  0: break;
  // P1.0 Interrupt
  case  2: break;
  // P1.1
  case  4: break;
  // P1.2
  case  6: break;
  // P1.3
  case  8: break;
  // P1.4
  case  10:
      P1OUT ^= BIT5;
      P1IFG &= ~BIT4;
      adcMeasurementPolling();
      break;
  // P1.5
  case  12:break;
  case  14:break;
  case  16: break;
  }
 }
