#include <msp430.h> 
#define GLOBAL_IQ 20
#include <lib/Timer.h>
#include <IQmathLib.h>
#include <lib/ADC.h>
const int period = 2400;
double duty  = 0.2;
unsigned int calculate = 0;
const double minimalStepSize =  0.00125;
//const double minimalStepSize =  0.000625;
int count;
int counter;
long test;
double Ipeak;

double dutyTest;
double stepSize;
double direction;
long currentPower;
long measuredPower;

void init();
unsigned long calculatePower();
unsigned long getPower();
double hillClimbing();
/**
 * Pinout:
 * P2.1: PWM, Output
 * P1.3: ADCinit Input, Port Interrupt (Control Signal to start SH circuit, Set High to start)
 * P1.4: Sample Hold ready Input (Control Input Signal, high when SH circuit is ready for ADC mesasurement)
 * P1.7: Capture Compare Input store timer Value in CCR2 timer A0 (measure T_off)
 * P1.6: TimerStart, input (Starts timer A0 on a Rising Edge off PWM input Signal)
 * P4.7: Controll bit output, toggle bit for testing
 * P9.0: ADC1 input Ucaphigh
 * P9.1  ADC2 input Ucaplow
 * P9.2  ADC3 input Uin
 */

void main(void)
{

    init();
    __delay_cycles(70000000);
    count = 0;
    counter = 1;
    while(1){

        if(counter == 1){
            count ++;
            if (count > 2){
                counter = 0;
                __delay_cycles(10000);
                dutyTest = hillClimbing(dutyTest);
                _nop();
            }
        }

    }
}

void init(){

    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    // PWM Timer
    // Set period cycles, clock select SMCLK, select duty, select outmode reset / set.
    timerInitPWMB0(period, TASSEL_2, duty, OUTMOD_7);
    // Timer for measuring Toff
    // clock select SMCLK, select Capture Compare input 0 (P1.5), triggered by rising edge, Interrupt enabled.
    //timerCaptureCompareA0(CCIS_0,TASSEL_2, CM_2);
    TA0CTL |= TASSEL_2 |  MC_2; // Select ACLK as timer clock source, Up mode, TB start.
    TA0CCTL2 |= CAP | CM_1 | CCIS_0 | SCS; // Capturemode on/off, Capture mode pos Edge, Capture input CCI2A (P1.5), Capture synchronus mode, capture interrupt enable
    P1REN |= BIT7;
    P1OUT &= ~BIT7;
    // Select Capture Compare Function
    P1SEL0 |= BIT7;
    P1SEL1 |= BIT7;
    P1OUT &= ~BIT7;

    P1DIR |= BIT3;
    P1OUT &= ~BIT3;
    // Port Interrupt Starting ADC measurement
    P1DIR &= ~BIT4;
    P1IE |= BIT4;   // P1.4 interrupt enabled
    P1IES &= ~BIT4; // P1.4 lo/hi edge
    // Port Interruot starting Timer
    P1DIR &= ~BIT6;
    P1IE &= ~BIT6;   // P1.6 interrupt disabled
    P1IES &= ~BIT6; // P1.6 lo/hi edge
    P1IFG = 0x00; // P1.6 IFG cleared

    P4DIR |= BIT7;


    // init ADCSequence Pin P9.0,P9.1,P9.2.
    adcInitSequence(ADC12SSEL_3, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_8,  ADC12INCH_10);
    P9SEL0 |= BIT0;
    P9SEL1 |= BIT0;
    P9SEL0 |= BIT1;
    P9SEL1 |= BIT1;
    P9SEL0 |= BIT2;
    P9SEL1 |= BIT2;


    // Configure 16 Mhz clock speed
    FRCTL0 = FWPW | NWAITS_1; // Defines number of waitstates.
    CSCTL0 = CSKEY;
    CSCTL1 =  DCORSEL | DCOFSEL_4; // Set DCO to high speed, select  16 MHz
    CSCTL3 = DIVM_0; // Divide MCLK by 0
    __enable_interrupt();
}

// Called after count value is reached.
// Called after count value is reached.
unsigned long getPower(){
    // ADC init start measurement
    P1OUT |= BIT3;

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
    counter = 1;
    long tmp = TA0CCR2 * (ADC12MEM0 - ADC12MEM1);
    return tmp  * ADC12MEM2;
}

double hillClimbing(){
    // Initial values for step size an duty cycle.
    int directionChange = 0;
    const unsigned long waitCycles = 16000000;
    stepSize = 0.04;
    direction = 1.0;
    //timerSetDutyCycleB0(duty);
    //__delay_cycles(waitCycles);
    currentPower = getPower();
    _nop();

    // Terminate when the minimal step size is reached.
    while(stepSize > minimalStepSize){
        // Calculate the new duty cycle.
        duty += direction * stepSize;

        timerSetDutyCycleB0(duty);
        _nop();
        __delay_cycles(waitCycles);
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
        if(directionChange > 2){
            currentPower = measuredPower;
            directionChange = 0;
            stepSize /= 2;
        }
    }
    return duty;
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
        // Reset interrupt Flag
        P1IFG = 0x00;
        ADC12IFGR0 &= ~ADC12IFG1;
        // Reset SH circuit
        P1OUT &= ~BIT3;

        P1IE |= BIT6;   // P1.6 interrupt enabled
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
        TA0R = 0;
        TA0CCTL2 = CAP | CM_1 | CCIS_0 | SCS | CCIE;
        P4OUT ^= BIT7;
        P1IE &= ~BIT6;   // Disable Interrupt.
        break;
    case  16: break;
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
        P1IFG = 0x00;
        P4OUT ^= BIT7;
        // Stop Capture Compare Mode
        TA0CCTL2 = 0;

        // Set flag for starting the calculation of the power.
        calculate = 1;
        break;              // TA0CCR2 interrupt

    case TA0IV_TA0IFG:  break;              // overflow
    default: break;
    }
}


