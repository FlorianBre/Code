#include <msp430.h>
#define GLOBAL_Q    14
#define GLOBAL_IQ 30
#include <QmathLib.h>
#include <IQmathLib.h>
#include <math.h>
/* #define a 2.2
#define b 2.1
#define c 5.3
#define d 3.1
#define e 34.0
#define f 4.34
#define g 0.04
#define h 2.02 */



_q qA, qB, qC;          // Q variables using global type
_iq iqA, iqB, iqC, iqD;

int a = 555;
int b = 255;
double calculate();
float testQmath(float, float);
double testIQmath(double, double);
double squareRoot(double);
double test;
/*
 * main.c
 *
 *  Created on: 21.06.2018
 *      Author: Flo
 */
void main() {
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    while(1){

    double j = 0.0034;
    double i = 0.25;
    _nop();
    test = sqrt(j);
    _nop();
    //test = testQmath(i,j);
    //test = testIQmath(i,j);
    //test = calculate();
    //test = squareRoot(j);

    //a += 1.0;
    }
}

double calculate(){
    //return i * j;
    return a * b;
}

float testQmath(float i, float j){
    /* double *testa = &a;
     double *testb = &b;
    qA = _Q(*testa);
    qB = _Q(*testb); */
    _nop();
    qA = _Q(i);
    qB = _Q(j);
    _nop();
    qC = _Qmpy(qA,qB);
    return _QtoF(qC);
}

double testIQmath(double i, double j){
    iqA = _IQ(i);
    iqB = _IQ(j);
    iqC = _IQmpy(iqA,iqB);
    return _IQtoF(iqC);
}

double squareRoot(double i){
    iqA = _IQ(i);
    iqB = _IQsqrt(iqA);
    return _IQtoF(iqB);
}





