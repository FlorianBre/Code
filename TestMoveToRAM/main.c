#include <msp430fr6989.h>
#include <stdlib.h>
#include <math.h>
#define aa 2.2
#define ab 2.1
#define ac 5.3
#define ad 3.1
#define ae 34.0
#define af 4.34
#define ag 0.04
#define ah 2.02


/**
 * main.c
 */
double test;
/*//#pragma DATA_SECTION(aa, ".MY_MEM")
const double aa = 2.2;
//#pragma DATA_SECTION(ab, ".MY_MEM")
const double ab = 2.1;
//#pragma DATA_SECTION(ac, ".MY_MEM")
const double ac = 5.3;
//#pragma DATA_SECTION(ad, ".MY_MEM")
const double ad = 3.1;
//#pragma DATA_SECTION(ae, ".MY_MEM")
const double ae = 34.0;
//#pragma DATA_SECTION(af, ".MY_MEM")
const double af = 4.34;
//#pragma DATA_SECTION(ag, ".MY_MEM")
const double ag = 0.04;
//#pragma DATA_SECTION(ah, ".MY_MEM")
const double ah = 2.02;
//#pragma DATA_SECTION(ai, ".MY_MEM")
const double ai = 2.02;
//#pragma DATA_SECTION(aj, ".MY_MEM")
const double aj = 2.02;

 double vIn_V = 2.25;
 double vOut_V = 5.2;
double rIn_Ohm = 2.1;
 double lCoil_H = 5.6;
double tPeriod = 9.3;
double *a = &vIn_V;
double *b = &vOut_V;
 double *c = &rIn_Ohm ;
double *d = &lCoil_H;
double *e = &tPeriod;
int *g;

double multInefficient();
void pullDownPorts();
double testRAMMethod();
double testMethod();

void main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    pullDownPorts();
    // g = calloc(10,sizeof(double));

    while(1){
        _nop();
          // test = multInefficient();
         //  test =  testRAMMethod();
         test = testMethod();
        _nop();
    }
}

double multInefficient(){
    return sqrt(2.0 * 5.2 * 3.4 * 3.2 * 1.4);
    //return 1.0 * vIn_V / vOut_V * 2 * lCoil_H / tPeriod * rIn_Ohm;
    // return 1.0 * *a / *b * 2 * *c / *d * *e;
}

#pragma CODE_SECTION(testRAMMethod,".run_from_ram")
double testRAMMethod(){

   return  aa * ab * ac * ad * ae * af * ag * ah;

}

double testMethod(){

   /* static double aa = 2.2;

    static  double ab = 2.1;

    static  double ac = 5.3;

    static  double ad = 3.1;

    static  double ae = 34.0;

    static  double af = 4.34;

    static  double ag = 0.04;

    static const double ah = 2.02; */
    return aa * ab * ac * ad * ae * af * ag * ah;
}

void pullDownPorts(){
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0x00;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0x00;
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0x00;
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0x00;
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0x00;
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0x00;
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0x00;
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0x00;
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0x00;
    P10DIR = 0x00;
    P10REN = 0xFF;
    P10OUT =0x00;
    PJDIR = 0xFF;
}
