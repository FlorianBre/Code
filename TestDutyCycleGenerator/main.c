#include <msp430.h> 
#define GLOBAL_IQ 20
#include <IQmathLib.h>
/**
 * Pinout
 */
_iq Factor;
long output;
double Uout = 1.3565;
const double Uref = 6.0;
const double Uoutmax = 2.2;
_iq dutyCycle;
double stepSize;
void setPinOutput(_iq);
void init();

void main(void)
{
    init();
    Factor = _IQ((1536.0 / Uref) * Uoutmax);
    init();
    _nop();
    dutyCycle = _IQ(0.05);
    setPinOutput(dutyCycle);
    _nop();
}

void setPinOutput(_iq dutyCycle) {
    output = _IQmpy(Factor, dutyCycle);
    P2OUT = output;
    P3OUT = output >> 8;
}

    void init(){
        PM5CTL0 &= ~LOCKLPM5;
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        P2DIR = 0xFF;
        P2OUT = 0xFF;
        P2SEL1 = 0x00;
        P2SEL0 = 0x00;
        P3DIR = 0x03;
        P3OUT = 0x03;
        P3SEL1 = 0x00;
        P3SEL0 = 0x00;
    }



