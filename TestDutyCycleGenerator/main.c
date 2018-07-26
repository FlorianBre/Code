#include <msp430.h> 
#define GLOBAL_IQ 20
#include <IQmathLib.h>
/**
 * Pinout
 * LSB
 * P2.2
 * P2.3
 * P2.4
 * P2.5
 * P2.6
 * P2.7
 * P8.4
 * P8.5
 * P8.6
 * P8.7
 * MSB
 */
long output;
const unsigned long factor = 5115;
double duty;
void setDutyCycle(double);
void init();

void main(void)
{
    init();
    _nop();
    duty = 0.03;
    setDutyCycle(duty);
    _nop();
}

void setDutyCycle(double dutyCycle) {
    output = _IQmpy(factor, _IQ(dutyCycle));
    P2OUT |= output << 2;
    P8OUT = output >> 2;
}

    void init(){
        PM5CTL0 &= ~LOCKLPM5;
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        P2DIR = 0xFC;
        P2SEL1 = 0x00;
        P2SEL0 = 0x00;
        P8DIR = 0xF0;
        P8SEL1 = 0x00;
        P8SEL0 = 0x00;
    }



