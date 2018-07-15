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
const double f = 6666.6666;
const double r = 2000.0;
// Korrektur Faktor vong wegen Spannungsteiler
const _iq correctionFactor = _IQ(1.5);
int test1;


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
    const _iq c = init();
    //fillLut(c);
    _nop();
    __delay_cycles(10000000);
    while(1){
        _nop();
        measure();
        //ADC12MEM0 = 3000;
        //ADC12MEM1 = 4000;
        TB0CCR5 =  calculateDuty(c);
        //TB0CCR5 =  lookUp();
        //TB0CCR5 =  calRelation(c);
        _nop();
    }
}

_iq init(){
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //pullDownPorts();
    // Init PWM timer
    timerInitPWMB0(period, TASSEL_2, initDuty, OUTMOD_7);
    //ADC12CLK = SMCLK, No Dif Mode, 4 ADCcyles SHT, Ref = V_cc, No internal Ref, No IE, Software trigger select, P8.4 start channel, P9.0 End channel.
    adcInitSequence(ADC12SSEL_3, 0, ADC12SHT0_0, ADC12VRSEL_0, 0, 0, ADC12SHS_0, ADC12INCH_7,  ADC12INCH_8);
    // Calculate Constant
    return _IQ((2 * l * f) / r);
}

void measure(){
    adcMeasurementPolling();
}


int calculateDuty(_iq c){
    unsigned long Uin = ADC12MEM0;
    // return _IQmpy(_IQsqrt(_IQmpy(1048576 - ((Uin << 20) / (ADC12MEM1 * 1.5)), c)), period + 1);
   return _IQmpy(_IQsqrt(_IQmpy(1048576 - ((Uin << 20) /  _IQmpy(ADC12MEM1, correctionFactor)), c)), period + 1);
}

void fillLut(_iq c){
    unsigned long i;
    //double Uin = ADC12MEM0;
    //double Uout = ADC12MEM1;
    //double periodtest +
    for(i = 0; i < 1024; i++){
    // Calculate duty cycle in IQ30 shift 10 bits right too calculate T_On in IQ20
    // Fill the LUT by increasing from 0 too 1023 in 1 / 1023 stepsize
     lut[i] =(_IQmpy(_IQsqrt(_IQmpy((1048576 - (i << 10)),c)),(period+1)));
    }
}

int lookUp(){
    unsigned long adcMem0 = ADC12MEM0;
    return lut[(adcMem0 << 10)/_IQmpy(ADC12MEM1, correctionFactor)];
}

int calRelation(_iq iqC){
    unsigned long adcMem0 = ADC12MEM0;
    unsigned long index = (adcMem0 << 10)/ _IQmpy(ADC12MEM1, correctionFactor);
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
