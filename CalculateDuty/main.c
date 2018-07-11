#include <msp430.h>
#define GLOBAL_IQ 20
#define GLOBAL_Q 15
#include <QmathLib.h>
#include <IQmathLib.h>
#include <math.h>

const double l = 560.0E-6;
const double f = 7500.0;
const int t = 133;
const double r = 2000.0;


float d;
/**
 * main.c
 */
float calculateOptimized(const double);
float calculateQOptimized(const double);
float calculateUnoptimized(const double);
int calculateDivOptimized(_iq);

void main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	const double c = (2 * l * f) / r;
	_iq iqC = _IQ(c);
	_nop();
	//d = calculateOptimized(c);
	//d = calculateUnoptimized(c);
	//d = calculateQOptimized(c);
	TA0CCR1 =  calculateDivOptimized(iqC);
	_nop();
}

/* float calculateOptimized(const double c) {

    return _IQtoF(_IQsqrt((_IQmpy(_IQ(1-(Uin/Uout)), _IQ(c)))));

}

float calculateQOptimized(const double c) {
    return _QtoF(_Qsqrt((_Qmpy(_Q(1-(Uin/Uout)), _Q(c)))));
   // d = _IQtoF(_IQmpy(_IQ(1-(Uin/Uout)), _IQ(c)));
}

float calculateUnoptimized(const double c){
    return sqrt(((1-(Uin/Uout)) * c));
    // return sqrt((((1-(Uin/Uout)) * 2 * f * l) / r));
} */

int calculateDivOptimized(_iq iqC){
    //ADC12MEM0 = 3000;
    //ADC12MEM1 = 4095;
    unsigned long Uin = ADC12MEM0;
    // Shift Uin 20 Bits left for integer division get solution in IQ20 format
    // Calculate Duty cycle
     return _IQmpy(_IQsqrt((_IQmpy((1048576 - ((Uin << 20) / ADC12MEM1)), iqC))), t + 1);
}


