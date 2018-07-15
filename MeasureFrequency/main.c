#include <templateEMP.h>

int measureVoltage(void);
void getTime(int);
unsigned int timer = 0;
int formerVoltage = 0;
int currentVoltage = 0;
int direction = 0;

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    timer++;
    TA0CTL &= ~TAIFG;
}

void main(void)
{
    initMSP();
    ADC10CTL0 = ADC10ON + ADC10SHT_2; // Turn on ADC with a sample and hold time of 16 cycles.
    ADC10CTL1 = INCH_5; // Enable ADC for Pin 1.5.
    ADC10AE0 |= BIT5;
    TA0CTL = TASSEL_1 + TAIE + MC_1; // Select the timer clock as SMCLK (1 MHz). Enable the interrupt. Select the count up mode.
    TA0CCR0 = 3000; // 3000 cycles is equal to 1 milliseconds at a clock frequency of 1 MHz.
    TA0CTL &= ~TAIFG; // Reset the interrupt flag.
    currentVoltage = measureVoltage();
    __enable_interrupt();
}

int measureVoltage( void )
{
    ADC10CTL0 |= ENC + ADC10SC; // Start conversion.
    while (ADC10CTL1 & ADC10BUSY); // Wait until result is ready.
    return ADC10MEM;

}
void getTime( voltage ){
    formerVoltage = currentVoltage;
    currentVoltage = measureVoltage();
    if( currentVoltage < formerVoltage){
        direction = -1;
    }
    else{
        direction = 1;
    }
    if( (currentVoltage > formerVoltage) && (direction == -1)){
        serialPrintln( " Time: ");
        serialPrintInt( timer );
        serialPrintln(" ");
    }
}

