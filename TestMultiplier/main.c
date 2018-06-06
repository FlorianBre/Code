#include <msp430.h> 
#define OP1ADR (unsigned long *) &MPY
#define OP2ADR (unsigned long *) &OP2
#define TIMADR (unsigned long *) &TA0CCR0
signed long result;                       // global variable for multiply
float resultFrac;
float test1;
float test2;
float test3;
// result
void simpleTest();
void initDMA();
void multiplyDMA();
void multiplyDouble();
void main(void)
{
    __enable_interrupt();
    PM5CTL0 = 0;
    WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
    //simpleTest();
    //multiplyDMA();
    multiplyDouble();
    __no_operation();                         // For debugger
}
void multiplyDMA(){
    initDMA();
    unsigned int multiplier = 20;
    MPY = multiplier;                        // Load first operand -signed mult
    TA0CCR0 = 30;
    result = RESHI;                           // Load RESHI word result
    result = (result<<16)|RESLO;              // Shift RESHI left and concat with
    _no_operation();
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    result = RESHI;                           // Load RESHI word result
    result = (result<<16)|RESLO;              // Shift RESHI left and concat with
    _no_operation();
}

void multiplyDouble(){
    test1 = 1.0;
    test2 = 0.25;
    test3 = test1 * test2;
    // MPY32CTL0 = MPYFRAC;
    int multiplier = 0x4000 ;
    int operand =  0x4000;
    MPY = multiplier;                        // Load first operand -signed mult
    OP2 = operand;                            // Load second operand
    resultFrac = RESHI;
    _no_operation();
}

void simpleTest(){
    signed int multiplier = 0x7234;
    signed int operand = 578;
    MPY = multiplier;                        // Load first operand -signed mult
    OP2 = operand;                            // Load second operand
    result = RESHI;                           // Load RESHI word result
    result = (result<<16)|RESLO;              // Shift RESHI left and concat with
    // RESLO word result
}

void initDMA(){
    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) TIMADR);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) OP2ADR);
    // Select ADC end of conversion.
    DMA0SZ = 1;
    // Single Transfer Mode
    // No increment or decrement of soure and Destination adress
    // DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMADSTBYTE | DMASRCBYTE; // Rpt, inc
    DMA0CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0; // Rpt, inc
    DMA0CTL |= DMAEN;                         // Enable DMA0
}





