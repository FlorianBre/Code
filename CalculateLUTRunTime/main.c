/**
 * Code to fill LUT in runtime
 * LUT is empty at the begining
 * the index LUT is calculated through the division of U_in/U_out
 * When the index is not in the LUT the value at this index got calculated
 */
#include <msp430.h> 
#define GLOBAL_IQ 20
#include <IQmathLib.h>
const double l = 680.0E-6;
const double f = 7500.0;
const long t = 133;
const double r = 2000.0;
#pragma PERSISTENT ( lut )
int lut[1024] = {[0 ... 1023] = -1};

/**
 * main.c
 */
int calRelation(_iq);
int calOn(unsigned long, _iq);

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	const _iq c = _IQ((2 * l * f) / r);
    while(1) {
    _nop();
	TA0CCR1 = calRelation(c);
	_nop();
    }
}

int calRelation(_iq iqC){
    ADC12MEM0 = 3000;
    ADC12MEM1 = 4095;
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
    return _IQmpy(_IQsqrt(_IQmpy((1048576 - (index << 10)), iqC)), t + 1);
}
