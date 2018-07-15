#include <msp430.h> 

/**
 * main.c
 */
void test();

static const double L = 2.2E-6;
static const double f = 10000.0;
static const double R = 2000.0;

double k;
float a = 0.75;
unsigned int tmp;
int d;

int init = 0;
void main(void)
{

    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    _nop();
    const double c = (2 * L * f) / R;
    _nop();
    k = c;
    _nop();
    test();
    _nop();

    /*tmp = 0.75 * (1 << 12);
    _nop();
    tmp *= 23;
    a = tmp >> 12;
    _nop(); */
}

void test(){
    k = 2.2E-6 * 10000.0 ;
}

