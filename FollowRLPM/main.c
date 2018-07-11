#include <msp430.h> 
#define GLOBAL_IQ 20
#include <lib/Timer.h>
#include <IQmathLib.h>
#include <lib/ADC.h>

/*
 * ---- Pinout ----
 * PWM Signal: P2.1
 * ADC1 Uin P8.4
 * ADC2 Uout P9.0
 */

const int period = 150;
const double initDuty = 0.2;
const double l = 680.0E-6;
const double f = 6666.66;
const double r = 2000.0;
#pragma PERSISTENT ( lut )
long lut[1024] = {[0 ... 1023] = -1};

_iq init();
void measure();
void fillLut(_iq);
int calculateDuty(_iq);
int calRelation(_iq);
int calOn(unsigned long, _iq);
int lookUp();

void main(void)
{
    __enable_interrupt();
    const _iq c = init();
    __delay_cycles(10000000);
    //fillLut(c);

    while(1){

        measure();
        __bis_SR_register(LPM3_bits + GIE);
        //ADC12MEM0 = 3000;
        //ADC12MEM1 = 4095;
          TB0CCR5 =  calculateDuty(c);
        //TB0CCR5 =  lookUp();
        //TB0CCR5 =  calRelation(c);
        _nop();
    }
}

_iq init(){
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    // Init PWM timer
    timerInitPWMB0(period, TASSEL_2 , initDuty, OUTMOD_7);
    // Init Timer for Sleepmode
    timerInitCounterA0(TASSEL_1, MC_0, 90);
    // ADC12CLK = SMCLK, No Dif Mode, 4 ADCcyles SHT, Ref = V_cc, No internal Ref, Interrupt Enable for mem1, Trigger by TA0 CCR1, P8.4 start channel, P9.0 End channel.
    adcInitSequence(ADC12SSEL_1, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, ADC12IE1, ADC12SHS_1, ADC12INCH_7,  ADC12INCH_8);
    // Calculate Constant
    return _IQ((2 * l * f) / r);
}

void measure(){
    TA0CTL |= TACLR;
    TA0CCR1 = 80;
    TA0CCTL1 |= OUTMOD_7;
    TA0CTL |= MC_1;
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 &= ~ADC12ON;
    ADC12CTL0 |= ADC12ENC | ADC12ON;
}


int calculateDuty(_iq c){
    unsigned long Uin = ADC12MEM0;
    //return _IQmpy(_IQsqrt((_IQmpy((1048576 - ((Uin << 20) / ADC12MEM1)), c))), period + 1);
    return _IQmpy(_IQsqrt(_IQmpy(1048576 - ((Uin << 20) / (ADC12MEM1 * 1.5)), c)), period + 1);
}

void fillLut(_iq c){
    unsigned long i;
    for(i = 0; i < 1024; i++){
    // Calculate duty cycle in IQ30 shift 10 bits right too calculate T_On in IQ20
    // Fill the LUT by increasing from 0 too 1023 in 1 / 1024 stepsize
    lut[i] =(_IQmpy(_IQsqrt(_IQmpy((1048576 - (i << 10)),c)),(period+1)));
    }
}

int lookUp(){
    unsigned long adcMem0 = ADC12MEM0;
    return lut[((adcMem0 << 10)/ADC12MEM1)];
}

int calRelation(_iq iqC){
    unsigned long adcMem0 = ADC12MEM0;
    unsigned long index = ((adcMem0 << 10)/ADC12MEM1);
    int tmp = lut[index];
    if(tmp > -1)
    {
        return tmp;
    }
    else{
        lut[index] = calOn(index, iqC);
        return lut[index];
    }
}

int calOn(unsigned long index, _iq iqC){
    //Shift index 10 Bits let to get IQ20 format
    return _IQmpy(_IQsqrt(_IQmpy((1048576 - (index << 10)), iqC)), (period + 1));
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    TA0CTL |= MC_0;
    _nop();
    if(ADC12IFG1) {
        ADC12IFGR0 = 0x0000;
        int test = ADC12MEM0;
        int test2 = ADC12MEM1;
      __bic_SR_register_on_exit(LPM3_bits+GIE); // Clear LPM bits upon ISR Exit
    }
}




