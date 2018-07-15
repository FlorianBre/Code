//---Module Description --------------------------------------------------

/**
 * @file    main.c
 * @author  Florian Bregler
 * @version 1.0
 * @brief   Controlling of a boost converter for input impedance matching
 *          by adjusting the duty cycle through a Hill Climbing algorithm.
 *
 * ---- Pinout ----
 * P1.0 controlling the discharge of the shunt cap.
 * P1.6 testing purposes.
 * P1.7 starting the power measurement.
 * P2.1, P2.2 converter clk.
 * P2.3 testing purposes.
 * P2.4 capture compare input for stopping the timer.
 * P2.5 controlling the two mosfets in series to the shunt cap.
 * P1.3 High side measure cap.
 * P1.4 Low Side measure cap.
 * P1.5 Measure Voltage.
 */

//---Includes ------------------------------------------------------------

#include <msp430.h>

//--- Type Definition ----------------------------------------------------

#define T_ON TA1CCR1
#define T_Period TA1CCR0

// --- Variables ---------------------------------------------------------

unsigned volatile int blockInterrupt = 0;
unsigned volatile int measureCur = 0;
const unsigned volatile int debugMode = 1;
const unsigned int period = 4000;
const double minimalStepSize = 0.015625;
const double shuntResistor_ohm = 1.98;
const double referenceVoltage_volt = 2.5;
double duty;
double stepSize;
double currentPower_watt;
double measuredPower_watt;
double direction = 1.0;
double duty;
int sleep = 0;
int directionChange;

//--- Global Functions Definition ----------------------------------------

/*
 * @brief Method for setting the duty cycle.
 * @param duty cycle.
 */
void setDutyCycle(double);

/*
 * @brief Method for setting the period time.
 * @param period time.
 */
void setPeriod(int);

/*
 * @brief Method for calculating the average power by given measured values over one working cycle.
 * @param current, voltage, TOnOff
 * @return calculated power.
 */
double calculatePower( double, double, double );

/*
 * @brief Method for measuring and calculating the average power over one working cycle.
 * @return measured power.
 */
double measurePower(void);

/*
 * @brief Method for configuring the duty cycle with the maximum power.
 * @return
 */
double hillClimbing(void);

void main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer.
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ; // Calibrate the DCO to 16 MHz
    P2DIR = 0x2E;
    P2OUT = 0x18;
    P2SEL = 0x16;
    P2SEL2 = 0x06;
    P1DIR = 0x41;
    P1OUT = 0x41;
    P1SEL = 0x00;
    P1IE = 0x80; // Enable interrupt on Pin 1.7.
    P1IES = 0x00; // Set hi/lo edge interrupt.
    P1IFG = 0x00; // Clear IFG.

    TA1CTL |= TACLR; // Clear Timercounter
    //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    TA1CTL |= TASSEL_2 + MC_1;
    // Capturemode on, Capture mode neg Edge, Capture input CCI2A (P2.4), Capture synchronus mode, capture interrupt enable
    TA1CCTL2 |= CAP + CM_2 + CCIS_0 + SCS + CCIE;
    TA1CCTL1 |= OUTMOD_7; // Set reset mode for output unit 1, P2.1, P2.2 clk
    ADC10AE0 |= BIT4;
    /*
     * Turn on ADC converter, Sample and hold time of 4 x ADC10CLKs, Turn on internal Reference
     * configure 2,5 V internal reference, Select internal referernce for upper ADC reference voltage.
     */
    ADC10CTL0 = ADC10ON + ADC10SHT_0 + REFON + REF2_5V + SREF_1;
    // Delay two working cycles too settle the internal reference Generator.
    setPeriod(4000);
    setDutyCycle(0.5);
    __delay_cycles(6400);
    while (1) {
        _no_operation();
        // duty = hillClimbing();
    }
}

double hillClimbing(void){
    // Initial values for step size an duty cycle.
    duty = 0.1;
    stepSize = 0.1;
    direction = 1.0;
    setDutyCycle(duty);
    __delay_cycles(6400);
    currentPower_watt = measurePower();
    __delay_cycles(6400);
    // Terminate when the minimal step size is reached.
    while(stepSize > minimalStepSize){
        // Calculate the new duty cycle.
        duty += direction * stepSize;
        __delay_cycles(6400);
        setDutyCycle(duty);
        __delay_cycles(6400);
        measuredPower_watt = measurePower();
        __delay_cycles(6400);
        // When the power at the new duty cycle is smaller than at the former duty cycle.
        if(measuredPower_watt < currentPower_watt) {
            // Change direction of the Hill climb and register the direction change
            direction *= -1.0;
            directionChange ++;
        }
        else{
            // Reset the number of subsequent direction changes.
            currentPower_watt = measuredPower_watt;
            directionChange = 0;
        }
        // When the direction is changed more than two times in a row the top of the hill is reached.
        if(directionChange > 2){
            currentPower_watt = measuredPower_watt;
            directionChange = 0;
            stepSize /= 2;
        }
        __delay_cycles(10);
    }
    return duty;
}

double measurePower(){
    double measureVoltage(void);
    double measureCurrent(void);
    blockInterrupt = 0; // Set blockInterrupt to 0 for allowing to trigger the measuring.
    while( measureCur == 0); // Wait until the interrupt allows the measuring
    blockInterrupt = 1; // blockInterrupt the allowing of the measuring.
    measureCur = 0; // blockInterrupt the measuring
    double current = measureCurrent();
    double voltage = measureVoltage();
    double TOnOff = TA1CCR2; // Get the time which was written in the timer register.
    return calculatePower(current, voltage, TOnOff );
    }

double measureVoltage()
{
    ADC10CTL1 = INCH_5; // Enable ADC for Pin 1.5
    ADC10AE0 |= BIT5;
    ADC10CTL0 |= ENC + ADC10SC; // Start conversion
    while (ADC10CTL1 & ADC10BUSY); // Wait until result is ready
    double voltage = ADC10MEM;
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT5;  // Disable ADC for Pin 1.5
    return voltage;
}

double measureCurrent()
{
    P2OUT |= BIT5; // Disconnect the high side in series to the shunt cap.
    int k = 0;
    // Wait 10 cycles until the voltage is stabl.
    while(k < 10){
        k++;
    }
    ADC10CTL1 = INCH_3; // Enable ADC for Pin 1.3
    ADC10AE0 |= BIT3;
    if( debugMode == 1){
        P2OUT ^= BIT3;
    }

    ADC10CTL0 |= ENC + ADC10SC; // Start conversion
    while (ADC10CTL1 & ADC10BUSY){}
    double current1 = ADC10MEM;
    if( debugMode == 1){
        P2OUT ^= BIT3;
    }
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT3;  // Disable ADC for Pin 1.3
    ADC10CTL1 = INCH_4; // Enable ADC for Pin 1.4
    ADC10AE0 |= BIT4;
    if( debugMode == 1){
        P2OUT ^= BIT3;
    }
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY){}
    double current2 = ADC10MEM;
    if( debugMode == 1){
        P2OUT ^= BIT3;
    }
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT4;  // Disable ADC for Pin 1.3
    // Reconnect the shunt cap.
    P2OUT &= ~BIT5;
    P1OUT &= ~BIT0;
    k = 0;
    // Discharge the shunt cap for 10 cycles.
    while(k < 10){
        k++;
    }
    P1OUT |= BIT0;
    return current1-current2;
}

double calculatePower(double current, double voltage, double TOnOff)
{
    if(debugMode == 1)
    {
    return (((((current)*referenceVoltage_volt)/(1023.0*1.98))*(TOnOff / 16.0))/(2.0*(T_Period/16.0))) * ((voltage*referenceVoltage_volt / 1023.0));
    }
    return (current/ shuntResistor_ohm)*(TOnOff/(2.0*T_Period)) * (voltage);
}

void setDutyCycle(double d)
{
    duty = d;
    T_ON = (T_Period + 1) * duty;
}

void setPeriod(int p)
{
    T_Period = p - 1;
}

/*
 * Timer interrupt triggered by the change form a high to a low edge on pin 2.4.
 */
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void)
{
    if(TA1IV == 4){

    }
}

/*
 * Port interrupt triggered by the change form a high to a low edge on pin 1.7.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(blockInterrupt == 0){
    measureCur = 1;
    }
    P1IFG &= ~BIT7;
}
