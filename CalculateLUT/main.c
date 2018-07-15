#include <msp430.h> 
#define GLOBAL_IQ 20
#include <IQmathLib.h>
const double l = 680.0E-6;
const double f = 6666.66;
const long t = 150;
const double r = 2000.0;
#pragma PERSISTENT ( lut )
unsigned long lut[1024] = {0};
unsigned int storedVal = 735;
int lookUp();
//int lookUpAbort();
void fillLut(_iq);
void main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    const _iq c = _IQ30((2 * l * f) / r);
    _nop();
    //test = _IQmpy(_IQ30sqrt(_IQ30mpy((1073741824 - (e * 1048576)),c)),(t+1)) >> 10;
    fillLut(c);

    _nop();
    TA0CCR1 = lookUp();
    _nop();
}

void fillLut(_iq c){
    unsigned long i;
    for(i = 0; i < 1024; i++){
    //test = _IQmpy(_IQsqrt((_IQmpy(1 - e * 0.0009765625, c))),(t + 1));
    // Calculate duty cycle in IQ30 shift 10 bits right too calculate T_On in IQ20
    // Fill the LUT by increasing from 0 too 1023 in 1 / 1023 stepsize
    lut[i] = (_IQmpy(_IQ30sqrt(_IQ30mpy((1073741824 - (i << 20)),c)),(t+1)) >> 10);
    }
}

int lookUp(){
    //ADC12MEM0 = 3000;
    //ADC12MEM1 = 4095;
    unsigned long adcMem0 = ADC12MEM0;
    return lut[((adcMem0 << 10)/ADC12MEM1)];
}

int lookUpAbort(){
    ADC12MEM0 = 3000;
    ADC12MEM1 = 4095;
    unsigned long adcMem0 = ADC12MEM0;
    unsigned long solution = (adcMem0 << 10) / ADC12MEM1;
    // Difference between old value and new value smaller new value divide by 64
    if(labs(storedVal - solution) < (solution >> 6)){
     _nop();
     return -1;
    }
    storedVal = solution;
    return lut[solution];
}

