#include <msp430fr6989.h>

/**
 * main.c
 */
void testDMAAdcTrigger();
void main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    testDMAAdcTrigger();
}

void testDMAAdcTrigger(){
    __data16_write_addr((unsigned short) &DMA2SA,(unsigned long ) &ADC12MEM0);
    __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) &ADC12MEM1);
    DMACTL1 |= DMA2TSEL_29;
    DMA2SZ = 1;
    DMA2CTL = DMADT_0 | DMASRCINCR_0 | DMADSTINCR_0 | DMAIE | DMAEN;
    __bis_SR_register(LPM4_bits + GIE);
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
        _no_operation();
        break;                          // DMA1IFG = DMA Channel 1
    case 6:
        _no_operation();
        break;                          // DMA2IFG = DMA Channel 2
    default: break;
    }
}
