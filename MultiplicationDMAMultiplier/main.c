#include <msp430.h> 
#include <lib/DMA.h>

/**
 * main.c
 */
unsigned int calculateDMA(unsigned int,unsigned int);
unsigned int calculateInt(unsigned int,unsigned int, unsigned int, unsigned int, unsigned int);
double calculateDouble(double, double, double);
void testFloat();


float a = 0.75;
unsigned int tmp;
int d;
double k;
int init = 0;
void main(void)
{
    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.
    FRCTL0 = FWPW | NWAITS_0; // Defines number of waitstates.
    tmp = a * (1 << 12);
    _nop();
    tmp *= 23;
    a = tmp >> 12;
    _nop();
    volatile unsigned int a = 8;
    volatile unsigned int b = 2;
    volatile unsigned int c = 4;
    volatile unsigned int e = 5;
    volatile unsigned int f = 2;

    double h = 2.43534;
    double i = 3.45;
    double j = 2.4;

    _nop();
    d = calculateDMA(f,calculateDMA(e,calculateDMA(c,calculateDMA(a,b))));
    __delay_cycles(1000);
    //d = calculateInt(a,b,c,e,f);
    // k = calculateDouble(h,i,j);
    _nop();
}

unsigned int calculateInt(unsigned int a, unsigned int b, unsigned int c,unsigned int e,unsigned int f) {
    return a * b * c * e * f;
}

double calculateDouble(double h,double i, double j){
    return h * i * j;
}

unsigned int calculateDMA(unsigned int a,unsigned int b) {
    if(init == 0){
    init = 1;
    DMAinit0(DMA0TSEL_0, &a, &MPY, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    DMAinit1(DMA1TSEL_0, &b, &OP2, DMASRCINCR_0, DMADSTINCR_0, 1, 0);
    DMAinit2(DMA2TSEL_29, &ADC12MEM0, &ADC12MEM1, DMASRCINCR_0, DMADSTINCR_0, 1, DMAIE);
    }
    else {
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long ) &a);
    __data16_write_addr((unsigned short) &DMA1SA,(unsigned long ) &b);
    }
    DMA0CTL |= DMAEN;
    DMA1CTL |= DMAEN;
    DMA0CTL |= DMAREQ ;
    DMA1CTL |= DMAREQ ;
    __bis_SR_register(LPM2_bits + GIE);
    return RESLO;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(DMAIV,16))
    {
    case 0: break;
    case 2:                           // DMA0IFG = DMA Channel 0
        break;
    case 4:
        break;                          // DMA1IFG = DMA Channel 1
    case 6:
        _nop();
        __bic_SR_register_on_exit(LPM2_bits+GIE); // Clear LPM bits upon ISR Exit
        break;                          // DMA2IFG = DMA Channel 2
    default: break;
    }
}
