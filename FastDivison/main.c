#include <msp430.h>
#define GLOBAL_IQ 24
#define GLOBAL_Q 15
#include <QmathLib.h>
#include <IQmathLib.h>
#include <math.h>

_iq iqC;

long a = 4094;
long b = 4095;
long c;
long d;
float m;

void main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    _nop();
    a  <<= 19;
    m = ADC12MEM0 / ADC12MEM1;
    //m = _IQ19toF(a / b);
    _nop();
}


