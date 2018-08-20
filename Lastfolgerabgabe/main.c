//--- Module Description ------------------------

/**
 * @file    main.c
 * @author  Florian Bregler
 * @version 1.0
 * @brief   Algorithm for resitive input impedance
 *  matching by using a boost converter
 */
// --- Pinout ---------------------------------------------
/** duty cycle generator
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
 * P3.3: Input, trigger for wakeup Timer.
 * P9.1 input ADC1 Uout.
 * P9.2 input ADC2 Uin.
 */

//--- Type Definition -------------------------------------

#define GLOBAL_IQ 18

//---Includes --------------------------------------------------


#include <msp430.h>
#include <lib/Timer.h>
#include <IQmathLib.h>
#include <lib/ADC.h>

// --- Variables ----------------------------------------------------

const unsigned int wakeUpTime = 100;
const unsigned long initDuty = 134086656;
const double l = 680.0E-6;
const double f = 6800.0;
const double r = 2000.0;
// Correction Factor Caused by Voltage Divider
const _iq voltageDividerFactor = _IQ(1.5);
// Factor duty cycle configuration
const _iq factor = _IQ(5115);
// correction factor for duty cycle
const unsigned int dutyCycleCorrectionfactor = 46;

// Create Look Up table of size 1024 in FRAM.
#pragma PERSISTENT ( lut )
long lut[1024] = {[0 ... 1023] = -1};

//--- Local Functions Declaration -------------------------

static void init();
static void calLut(_iq);
static long calDuty(_iq);
static long lookUp();
static long calRelation(_iq);
static void setDutyCycle(long);
static void pullDownPorts();

//--- Local Functions Definition  -------------------------

void main(void)
{
    init();
    setDutyCycle(initDuty);
    // Wait for boost convertert to settle Output voltage.
    __delay_cycles(10000000);
    const _iq c = _IQ((2 * l * f) / r);
    calLut(c);
    __bis_SR_register(LPM4_bits + GIE);
    _nop();
    while(1){
        adcMeasurementPolling();
        setDutyCycle( calDuty(c));
        setDutyCycle(lookUp());
        setDutyCycle(calRelation(c));
        __bis_SR_register(LPM4_bits + GIE);
    }
}

/*
 * @brief Method for configuring the peripheral registers
 * and I/O Pins.
 */
static void init(){
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    __enable_interrupt();
    // Pull Down all unused Ports for Reducing Energy Consumption
    pullDownPorts();
    // Init Pins for duty cycle Generator.
    P2DIR = 0xFC;
    P2SEL1 = 0x00;
    P2SEL0 = 0x00;
    P2OUT = 0x00;
    P8DIR = 0xF0;
    P8SEL1 = 0x00;
    P8SEL0 = 0x00;
    P8OUT = 0x00;

    // Init PWM timer.
    timerInitPWMB0(wakeUpTime, TASSEL_3, 0, OUTMOD_3);
    TB0CTL |= TBIE;
    TB0CCTL0 |= CCIE;

    // ADC12CLK = SMCLK, No Dif Mode,
    // 4 ADCcyles SHT, Ref = V_cc, No internal Ref, No IE,
    // Software trigger select, P9.1 start channel, P9.2 End
    //channel.
    adcInitSequence(ADC12SSEL_2, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_9,  ADC12INCH_10);
    P9SEL0 |= BIT1;
    P9SEL1 |= BIT1;
    P9SEL0 |= BIT2;
    P9SEL1 |= BIT2;

    CSCTL0 = CSKEY;
    // Set DCO to high speed, select  16 MHz
    CSCTL1 =  DCORSEL | DCOFSEL_4;
    // Select VLOCLK as source for ACLK
    CSCTL2 |= SELA_1;
    // Defines number of waitstates, required for 16 MHz clk.
    FRCTL0 = FWPW | NWAITS_1;
}

/*
 * @brief Method for calculating the values of the Look Up table.
 * @param boost converter parameters.
 */
static void calLut(_iq c){
    unsigned long i;
    for(i = 0; i < 1024; i++){
        // Calculate duty cycle in IQ18 shift 8 bits right too calculate T_On in IQ18
        // Fill the LUT by increasing from 0 too 1023 in 1 / 1023 stepsize
        lut[i] =_IQmpy((_IQsqrt(_IQmpy((262144 - (i << 8)),c))), factor);
    }
}

/*
 * @brief Method for calculating the duty cycle.
 * @param boost converter parameters.
 * @return calculated duty cycle
 */
static long calDuty(_iq c){
    unsigned long Uein = ADC12MEM1;
    return _IQmpy(_IQsqrt(_IQmpy(262144 - ((Uein << 18) /  _IQmpy(ADC12MEM0, voltageDividerFactor)), c)), factor);
}

/*
 * @brief Method for determining the duty cycle by using a look up table.
 * @param boost converter parameters.
 * @return calculated duty cycle
 */
static long lookUp(){
    unsigned long  Uein = ADC12MEM1;
    unsigned int i = (Uein << 10)/_IQmpy(ADC12MEM0, voltageDividerFactor);
    return lut[i];
}

/*
 * @brief Method for calculating the duty by using a dynamic look up table.
 * @param boost converter parameters.
 * @return calculated duty cycle.
 */
static long calRelation(_iq iqC){
    unsigned long  Uein = ADC12MEM1;
    unsigned long index = (Uein << 10)/ _IQmpy(ADC12MEM0, voltageDividerFactor);
    int tmp = lut[index];
    if(tmp > -1)
    {
        return tmp;
    }
    else{
        lut[index] = _IQmpy(_IQsqrt(_IQmpy((262144 - (index << 8)), iqC)), (factor));
        return lut[index];
    }
}

/*
 * @brief Method for configuring the duty cycle.
 * @param duty cycle.
 */
static void setDutyCycle(long output) {
    output >>= 18;
    output -= dutyCycleCorrectionfactor;
    P2OUT = output << 2;
    P8OUT = output >> 2;
}

// Timer B0 ISR for waking up MSP after
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
        __bic_SR_register_on_exit(LPM4_bits);
        break;                              // TB0CCR0 interrupt
    default: break;
    }
}

/*
 * @brief Method for pulling down every unused pin to reduce energy consumption.
 */
void pullDownPorts(){
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0x00;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0x00;
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0x00;
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0x00;
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0x00;
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0x00;
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0x00;
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0x00;
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0x00;
    P10DIR = 0x00;
    P10REN = 0xFF;
    P10OUT = 0x00;
    PJDIR = 0xFF;
}

// ---EOF--------------------------------------------------
