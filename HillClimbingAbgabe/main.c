// --- Module Description ------------------------------------------------

/**
 * @file    main.c
 * @author  Florian Bregler
 * @date    21.08.2018
 * @version 2.0
 *
 * @brief   Algorithm for impedance matching using a Hill Climbing procedure
 */

// --- Type Definitions --------------------------------------------------

#define GLOBAL_IQ 20

// --- Includes ----------------------------------------------------------

#include <msp430.h>
#include <lib/Timer.h>
#include <IQmathLib.h>
#include <lib/ADC.h>


// --- Variables ---------------------------------------------------------

const unsigned int timerCorrectionFactor = 32;
const unsigned long factor = 5115;
const unsigned int wakeUpTimeHill = 10000;
const unsigned int wakeUpTimePower = 10000;
const int maxDirectionChange = 4;
const double minimalStepSize =  0.0002;
double duty  = 0.05;
volatile unsigned int calculate = 0;
double stepSize;
double direction;
long currentPower;
long measuredPower;

// --- Local Functions Declaration ---------------------------------------

void init();
void setDutyCycle(double);
double hillClimbing();
unsigned long calculatePower();
unsigned long getPower( );
void delayHillClimbing( );
void delayPowerCalculation( );

// --- Pinout ------------------------------------------------------------

/**
 * duty cycle generator
 * LSB
 * P2.2
 * P2.3
 * P2.4
 * P2.5
 * P2.6
 * P2.7
 * P8.4
 * P8.5
 * P8.6
 * P8.7
 * MSB
 * P3.3: Input, Trigger For Wakeup Timer.
 * P2.1: ADCinit Output Timer B0 (Control Signal to start SH circuit, Set High to start)
 * P1.4: Sample Hold ready Input (Control Input Signal, high when SH circuit is ready for ADC mesasurement)
 * P1.7: Capture Compare Input store timer Value in CCR2 timer A0 (measure T_off)
 * P1.6: TimerStart, input (Starts timer A0 on a Rising Edge off PWM input Signal)
 * P9.1  ADC2 input Ucap
 * P9.2  ADC3 input Uin
 */

// --- Global Functions Definition ---------------------------------------------

void main(void)
{
    init();
    // Set intial duty
    setDutyCycle(duty);
    __delay_cycles(16000000);
    while(1){
        delayHillClimbing( );
        hillClimbing();
    }

}


// --- Local Functions Definition ----------------------------------------------

/**
 * @brief Description of global function
 *
 * @param Description of parameter
 * @return Description of return value
 */
void init(){
    TB0CCTL6 = 0;
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    // Timer For Waking Up MSP430 for Power Measurement.

    // Init Pins for duty cycle Generator
    P2DIR = 0xFC;
    P2SEL1 = 0x00;
    P2SEL0 = 0x00;
    P2OUT = 0x00;
    P8DIR = 0xF0;
    P8SEL1 = 0x00;
    P8SEL0 = 0x00;
    P8OUT = 0x00;

    timerInitPWMB0(wakeUpTimePower, TASSEL_3, 0.5 , OUTMOD_3);
    // Timer for measuring Toff
    // clock select SMCLK, select Capture Compare input 0 (P1.5), triggered by rising edge, Interrupt enabled.
    //timerCaptureCompareA0(CCIS_0,TASSEL_2, CM_2);
    TA0CTL |= TASSEL_2 |  MC_0; // Select ACLK as timer clock source, Up mode, TB start.
    TA0CCTL2 |= CAP | CM_1 | CCIS_0 | SCS; // Capturemode on/off, Capture mode pos Edge, Capture input CCI2A (P1.5), Capture synchronus mode, capture interrupt enable
    P1REN |= BIT7;
    P1OUT &= ~BIT7;
    // Select Capture Compare Function
    P1SEL0 |= BIT7;
    P1SEL1 |= BIT7;
    P1OUT &= ~BIT7;

    // Port Interrupt Starting ADC measurement
    P1DIR &= ~BIT4;
    P1IE |= BIT4;   // P1.4 interrupt enabled
    P1IES &= ~BIT4; // P1.4 lo/hi edge
    // Port Interrupt starting Timer
    P1DIR &= ~BIT6;
    P1IE &= ~BIT6;   // P1.6 interrupt disabled
    P1IES &= ~BIT6; // P1.6 lo/hi edge
    P1IFG = 0x00; // P1.6 IFG cleared
    P4DIR |= BIT7;

    // init ADCSequence Pin P9.0,P9.1,P9.2.
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_9,  ADC12INCH_10);

    REFCTL0 |= REFON | REFVSEL_0;   // Turn on internal Reference Generator, select Reference
    while( REFCTL0 & REFGENBUSY){ };    // Wait for refernce to settle
    ADC12MCTL0 |= ADC12VRSEL_1;

    P9SEL0 |= BIT1;
    P9SEL1 |= BIT1;
    P9SEL0 |= BIT2;
    P9SEL1 |= BIT2;

    // Configure 16 Mhz clock speed
    FRCTL0 = FWPW | NWAITS_1; // Defines number of waitstates.
    CSCTL0 = CSKEY;
    CSCTL1 =  DCORSEL | DCOFSEL_4; // Set DCO to high speed, select  16 MHz
    //CSCTL2 |= SELA_1; // Select VLOCLK as source for ACLK
    CSCTL3 = DIVM_0; // Divide MCLK by 0
    __enable_interrupt();
}

void setDutyCycle(double dutyCycle) {
    unsigned long output = _IQmpy(factor, _IQ(dutyCycle));
    P2OUT |= output << 2;
    P8OUT = output >> 2;
}

void delayPowerCalculation(){
    TB0CCR0 = wakeUpTimePower;
    TB0CTL |= TACLR;
    TB0CTL = TASSEL_3 | MC_1;
    TB0CCTL5 =  OUTMOD_3;
    TB0CCTL0 = 0;
    _nop();
    __bis_SR_register(LPM4_bits + GIE);

}

void delayHillClimbing(){
    TB0CCTL5 = OUTMOD_0;
    TB0CCR0 = wakeUpTimeHill;
    TB0CCR5 = 5000;
    TB0CTL |= TACLR;
    TB0CTL = TBIE | TASSEL_3 | MC_1;
    TB0CCTL0 = CCIE;
    __bis_SR_register(LPM4_bits + GIE);
}

// Called after count value is reached.
unsigned long getPower(){
    // Start measurement
    _nop();
    adcMeasurementInterrupt();
    while(calculate == 0){
        _nop();
    }
    _nop();
    return calculatePower();
}

unsigned long calculatePower() {
    // Calculate Power Uin * Ucaphigh - Uin low * Tonoff
    _nop();
    calculate = 0;
    long tmp2 = TA0CCR2 + timerCorrectionFactor;
    long tmp3 = ADC12MEM0;
    long tmp4 = tmp2 * tmp3;
    long tmp5 = tmp4 * ADC12MEM1;
    return tmp5;
}

double hillClimbing(){
    // Initial values for step size an duty cycle.
    int directionChange = 0;
    stepSize = 0.04;
    direction = 1.0;
    delayPowerCalculation();
    currentPower = getPower();
    _nop();

    // Terminate when the minimal step size is reached.
    while(stepSize > minimalStepSize){
        // Calculate the new duty cycle.
        duty += direction * stepSize;
        if(duty < 0.025){
            duty = 0.025;
        }
        if(duty > 0.15){
            duty = 0.15;
        }
        setDutyCycle(duty);
        delayPowerCalculation();
        measuredPower = getPower();
        _nop();
        // When the power at the new duty cycle is smaller than at the former duty cycle.
        if(measuredPower < currentPower) {
            // Change direction of the Hill climb and register the direction change
            direction *= -1.0;
            directionChange ++;
        }
        else{
            // Reset the number of subsequent direction changes.
            currentPower = measuredPower;
            directionChange = 0;
        }
        // When the direction is changed more than two times in a row the top of the hill is reached.
        if(directionChange >  maxDirectionChange){
            currentPower = measuredPower;
            directionChange = 0;
            stepSize *= 0.5;
        }
    }
    return duty;
}

// --- Interrupt Service Routines Definition -----------------------------

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
    case ADC12IV_ADC12IFG1:
        // Reset interrupt Flag
        P1IFG = 0x00;
        ADC12IFGR0 = 0;
        // Reset SH circuit
        TB0R = wakeUpTimePower - 2;
        TA0R = 0;
        TA0CCTL2 = CAP | CM_1 | CCIS_0 | SCS | CCIE;
        TA0CTL = TASSEL_2;
        P1IE |= BIT6;   // P1.6 interrupt enabled

        __bic_SR_register_on_exit(LPM3_bits);
        _nop();
        _nop();
        break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:
        _nop();

        break;        // Vector 16:  ADC12MEM2
    default: break;
    }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    _nop();
    switch (__even_in_range(P1IV,16))
    {
    case  P1IV_P1IFG4:
        _nop();
        _nop();
        __bic_SR_register_on_exit(LPM4_bits);
        break;
        // P1.6
        // Start Timer A0 for measuring Tonoff
    case  P1IV_P1IFG6:
        TA0CTL |=  MC_2;
        break;
    default: break;
    }
}

// TimerA0 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    _nop();
    switch(__even_in_range(TA0IV,TA0IV_TAIFG))
    {
    case TA0IV_NONE:
        break;                              // No interrupt
    case TA0IV_TA0CCR1: break;              // TA0CCR1 interrupt
    case TA0IV_TA0CCR2:
        P1IE &= ~BIT6;   // Disable Interrupt.
        P1IFG = 0;
        // Stop Capture Compare Mode
        TA0CCTL2 = 0;
        // Stop Counter
        TA0CTL = 0;
        _nop();
        break;              // TA0CCR2 interrupt

    case TA0IV_TA0IFG:  break;              // overflow
    default: break;
    }
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
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
    case TB0IV_NONE:
        TB0R = 0;
        _nop();
        __bic_SR_register_on_exit(LPM4_bits);
        break;                              // No interrupt
    case TB0IV_TB0CCR1: break;              // TA0CCR1 interrupt
    default: break;
    }
}

// --- EOF ---------------------------------------------------------------
