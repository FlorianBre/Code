#include <msp430fr6989.h>
#define T_ON_B0 TB0CCR6
#define T_PERIOD_B0 TB0CCR0
#include <lib/Timer.h>
#include <lib/ADC.h>

void timerInitTestPWMB0();
void timerSetDutyCycleTestB0(double);

void main( ) {
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    __enable_interrupt();
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    // Configure Pin as TB0 Trigger
    P3DIR &= ~BIT3;
    P3SEL1 = BIT3;
    P3SEL0 = BIT3;
    //timerInitTestPWMB0(100, TASSEL_3, 0.5, OUTMOD_0);
    //timerInitPWMB0(60000, TASSEL_1, 0.5, OUTMOD_3);
    //TB0CTL |= TBIE;
    //TB0CCTL0 = CCIE;


    // Configure Pin as TA1 Trigger
    P1DIR &= ~BIT1;
    P1SEL1 = BIT1;
    P1SEL0 &= ~BIT1;
    //P7DIR &= ~BIT0;
    //P7SEL1 &= ~BIT0;
    //P7SEL0 = BIT0;

    timerInitPWMA1(200, TASSEL_3,0,OUTMOD_3);
    TA1CCR1 = 1;
    adcInitSequence(ADC12SSEL_2, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_4, ADC12INCH_8,  ADC12INCH_10);

    //TA1CTL |= TAIE;
    //TA1CCTL1 = CCIE;


    //P1DIR &= ~BIT6;
    //P1IE |= BIT6;   // P1.6 interrupt disabled
    //P1IES &= ~BIT6; // P1.6 lo/hi edge
    //P1IFG = 0x00; // P1.6 IFG cleared

    _nop();
    while(1){
        // __bis_SR_register(LPM3_bits + GIE);
        // __delay_cycles(100000);
        // TB0R = 60000-1;
    }
}

void timerInitTestPWMB0(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode) {
    T_PERIOD_B0 = periodCycles - 1;
    TB0CTL |= TACLR; // Reset Timer
    TB0CTL |= clockSelect | MC_1; // Select timer clock source,Count up to the value in TB0CCR0.
    TB0CCTL6 |= pwmOutputMode; // Select output mode.
    P2DIR |= BIT0;
    P2SEL0 &= ~BIT0;
    P2SEL1 |= BIT0;
    timerSetDutyCycleTestB0(dutyCycle);

    TB0CTL |= TBIE;
    TB0CCTL0 = CCIE;
}
void timerSetDutyCycleTestB0(double dutyCycle){
    T_ON_B0 = (T_PERIOD_B0 + 1.0) * dutyCycle;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
    case TB0IV_NONE:
        TB0R = 0;
        __bic_SR_register_on_exit(LPM3_bits); // Clear LPM bits upon ISR Exit

        _nop();
        break;                              // No interrupt
    case TB0IV_TB0CCR1: break;              // TA0CCR1 interrupt
    default: break;
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TA1IV,TA1IV_TAIFG))
    {
    case TA1IV_NONE:
        TA1R = 0;
        _nop();

        break;                              // No interrupt
    case TA1IV_TA1CCR1: break;              // TA0CCR1 interrupt
    default: break;
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TA1IV,TA1IV_TAIFG))
    {
    case TA1IV_NONE:
        TA1R = 0;
        _nop();
        break;                              // No interrupt

    case TA1IV_TA1CCR1:
        _nop();
        TA1R = 0;
        _nop();
        break;              // TA0CCR1 interrupt
    default: break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    _nop();
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
    // Trigger ADC measurement when SH is ready (Output high)
    case  10:
        adcMeasurementPolling();
        break;
        // P1.5
    case  12:break;
    // P1.6
    // Start Timer A0 for measuring Tonoff
    case  P1IV_P1IFG6:
        P1IFG = 0x00; // P1.6 IFG cleared
        __bic_SR_register_on_exit(LPM3_bits);
        break;
    case  16: break;
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
        break;
    case ADC12IV_ADC12IFG1:
        _nop();
        ADC12IFGR0 &= ~ADC12IFG1;
        TA1R = 0;
        _nop();
        break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:
        _nop();
        ADC12IFGR0 &= ~ADC12IFG2;
        TA1R = 0;
        _nop();
        break;        // Vector 16:  ADC12MEM2
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


