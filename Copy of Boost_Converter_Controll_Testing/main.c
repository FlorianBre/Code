#include <templateEMP.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define T_ON TA1CCR1
#define T_Period TA1CCR0

int time;
int getTime;
int period;
int j;
double precision_watt = 0.001;
double power;
double current1;
double current2;
double powerDifference_watt = 0.0;

double voltage;
double duty;
double stepSize;
double currentPower_watt;
double testPower_watt;
double direction = 1.0;
int block = 0;
int measureCur = 0;
double duty;
int sleep;
int b = 0;
double dutyArray[10];
int a = 0;
double diffV = 0.0;
double powerTest;
double currentTest;
int directionChange;
double differenceVolt;
double volts;
double calculatePower(void);
double measurePower(void);
void measureVoltage(void);
void measureCurrent(void);
void setDuty(double);
void setPeriod(int);
double hillClimbing(void);
double alternativeHillClimbing(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer for our convenience.
    initMSP();
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P2DIR |= BIT1 + BIT2; //Set port 2 to the output direction.
    P2SEL |= BIT1 + BIT2;

    P2DIR &= ~BIT4; // P2.4 capture input for stopping the timer.
    P2OUT |= BIT4;
    P2SEL |= BIT4;
    P2SEL2 &= ~BIT4;

    P1SEL &= ~BIT0; // P1.0 for controlling the discharge of the shunt cap.
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    P2SEL &= ~BIT5;// P2.5 for controlling the mos in series to the shunt cap at the higher potential.
    P2DIR |= BIT5;
    P2OUT &= ~BIT5;

    P2SEL &= ~BIT3; // P2.3 for testing purposes.
    P2DIR |= BIT3;
    P2OUT |= BIT3;

    P1SEL &= ~BIT6; // P1.6 for testing purposes.
    P1DIR |= BIT6;
    P1OUT |= BIT6;

    P1DIR &= ~BIT7; //Set pin 1.7 to the input direction. Pin 1.7 for starting the adc.
    P1IE |= BIT7;   // P1.7 interrupt enabled
    P1IES = BIT7; // P1.7 lo/hi edge
    P1IFG &= ~BIT7; // P1.7 IFG cleared
    TA1CTL |= TACLR;
    TA1CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    TA1CCTL2 |= CAP + CM_2 + CCIS_0 + SCS + CCIE; // Capturemode on, Capture mode neg Edge, Capture input CCI2A (P2.4), Capture synchronus mode, capture interrupt enable
    TA1CCTL1 = OUTMOD_7; // Set reset mode for output unit 1, P2.1, P2.2 clk
    ADC10AE0 |= BIT4;
    ADC10CTL0 = ADC10ON + ADC10SHT_0 + REFON + REF2_5V + SREF_1;
    setPeriod(4000);
    setDuty(0.8);
    __delay_cycles(320000);
    while (1) {
       duty = alternativeHillClimbing();
       P1OUT |= BIT6;
       __delay_cycles(32000000);
       P1OUT = ~BIT6;
   // power = measurePower();
  /*   if(b == 0){
            b = 1;
        duty = alternativeHillClimbing();

        }
       duty = 0.1;
    while(duty < 0.95){
        powerTest = 0;
        diffV = 0.0;
        volts = 0.0;
        a = 0.0
        setDuty(duty);
        __delay_cycles(6400);
        while(a < 20){
        powerTest += measurePower();
        diffV += differenceVolt;
        a+= 1.0;
        __delay_cycles(64000);
        }
        powerTest /= a;
        diffV /= a;
       // volts /= a;
        __delay_cycles(64000);
         duty += 0.05;
        } */



 //  Execute Hillclimbing 10 times for testing.
  /* a = 0;
        while ( a < 10){
            dutyArray[a] = alternativeHillClimbing();
           // setDuty(dutyArray[a]);
           __delay_cycles(6400);
          // powerTest[a] = measurePower();
            a++;
            __delay_cycles(32000);
        } */

/*
     if(sleep == 0){
         duty = hillClimbing();
     }
     if(sleep > 0){
         sleep ++ ;
         __delay_cycles(4000);
         if (sleep > 100 ){
             sleep = 0;
         }
     }
*/




   //     __delay_cycles(320000);
    }
}

/*
 * Method for calculating the duty cycle with the maximum power output.
 * Terminates at a certain power difference between two duty cycles.
 */
double hillClimbing(void) {
    duty = 0.1; // Init values for stepsize an duty cycle.
    stepSize = 0.1;
    direction = 1.0;
    setDuty(duty);
    __delay_cycles(6400); // Delay of two working cycles after change of dutycyle
    currentPower_watt = measurePower();
    __delay_cycles(6400);
    do{
        duty += direction * stepSize; // Caculate new duty cycle.
        __delay_cycles(6400);
        setDuty(duty);
        __delay_cycles(6400);
        testPower_watt = measurePower(); // Get power at the new duty cycle.
        __delay_cycles(6400);
        if(testPower_watt < currentPower_watt){ // When the power at the new duty cycle is smaller than at the former duty cycle.
            direction *= -1.0; // Change direction of the Hill climb.
            stepSize /= 2;
        }
        powerDifference_watt = fabs(testPower_watt - currentPower_watt);
        currentPower_watt = testPower_watt;
    } while (powerDifference_watt > precision_watt); // Terminate when the difference between old power and new power is small enough.
    return duty ;
}

/*
 * Method for calculating the duty cycle with the maximum power output.
 * Terminates at a certain minimal step size.
 */
double alternativeHillClimbing(void){
    duty = 0.1;  // Init values for step size an duty cycle.
    stepSize = 0.1;
    direction = 1.0;
    setDuty(duty);
    __delay_cycles(6400);
    currentPower_watt = measurePower();
    __delay_cycles(6400);
    while(stepSize > 0.00785){ // Terminate when the minimal step size is reached.
        duty += direction * stepSize;
        __delay_cycles(6400);
        setDuty(duty);
        __delay_cycles(6400);
        testPower_watt = measurePower();
        __delay_cycles(6400);
        if(testPower_watt < currentPower_watt) {  // When the power at the new duty cycle is smaller than at the former duty cycle.
            direction *= -1.0; // Change direction of the Hill climb.
            directionChange ++; // Register the direction Change.
        }
        else{
            currentPower_watt = testPower_watt;
            directionChange = 0; // Reset direction change.
        }
        if(directionChange > 2){ // When the direction is changed more than two times in a row the top of the hill is reached.
            currentPower_watt = testPower_watt;
            directionChange = 0;
            stepSize /= 2;
        }
        __delay_cycles(10);
    }
    return duty;
}

/*
 * Method for setting the duty cycle.
 */
void setDuty(double d)
{
    duty = d;
    T_ON = (T_Period + 1) * duty;
}

/*
 * Method for setting the period time.
 */
void setPeriod(int p)
{
    T_Period = p - 1;
}

/*
 * Method for measuring and calculating the delivered power once.
 */
double measurePower(){
    block = 0; // Set block to 0 for allowing to trigger the measuring.
    while( measureCur == 0); // Wait until the interrupt allows the measuring
    block = 1; // Block the allowing of the measuring.
    measureCur = 0; // Block the measuring
    measureCurrent();
    measureVoltage();
    time = TA1CCR2; // Get the time which was written in the timer register.
    return calculatePower();
    }

/*
 * Measure the Voltage at the input of the circuit.
 */
void measureVoltage()
{
    ADC10CTL1 = INCH_5; // Enable ADC for Pin 1.5
    ADC10AE0 |= BIT5;
    ADC10CTL0 |= ENC + ADC10SC; // Start conversion
    while (ADC10CTL1 & ADC10BUSY); // Wait until result is ready
    voltage = ADC10MEM;
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT5;  // Disable ADC for Pin 1.5
    return;
}

/*
 * Method for measuring the maximum current through the shunt resistor.
 */
void measureCurrent(void)
{

   // P2OUT |= BIT5; // Disconnect the high side in series to the shunt cap.
    int k = 0;
    while(k < 10){ // Wait 10 cycles
        k++;
    }
    ADC10CTL1 = INCH_3; // Enable ADC for Pin 1.3
    ADC10AE0 |= BIT3;
    P2OUT ^= BIT3;
    ADC10CTL0 |= ENC + ADC10SC; // Start conversion
    while (ADC10CTL1 & ADC10BUSY){}
    current1 = ADC10MEM;
    P2OUT ^= BIT3;
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT3;  // Disable ADC for Pin 1.3
    ADC10CTL1 = INCH_4; // Enable ADC for Pin 1.4
    ADC10AE0 |= BIT4;
    P2OUT ^= BIT3;
    ADC10CTL0 |= ENC + ADC10SC; // Start conversion
    while (ADC10CTL1 & ADC10BUSY){}
    current2 = ADC10MEM;
    P2OUT ^= BIT3;
    ADC10CTL0 &= ~ENC;
    ADC10AE0 &= ~BIT4;  // Disable ADC for Pin 1.3 */
    P2OUT &= ~BIT5; // Reconnect the shunt cap.
    k = 0;
       while(k < 50){ // Discharge the shunt cap for 10 cycles.
           k++;
       }
    P1OUT &= ~BIT0;
    k = 0;
    while(k < 50){ // Discharge the shunt cap for 10 cycles.
        k++;
    }
    P1OUT |= BIT0;

    return;
}

/*
 * Method for calculating the average power over one duty cycle by using the measured values.
 */
double calculatePower()
{
    differenceVolt = (((current1-current2)*2.5)/1023.0);
    power = ((differenceVolt*1.98*(time / 16.0))/(2.0*(T_Period/16.0))) * ((voltage*2.5/ 1023.0));
    return power;
}

/*
 * Timer interrupt triggered by the change form a high to a low edge on pin 2.4.
 */
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void)
{
    if(TA1IV == 4){
        getTime = 1;
    }
}

/*
 * Port interrupt triggered by the change form a high to a low edge on pin 1.7.
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(block == 0){
        P2OUT |= BIT5; // Disconnect the high side in series to the shunt cap.
    measureCur = 1;
    }
    P1IFG &= ~BIT7;
}
