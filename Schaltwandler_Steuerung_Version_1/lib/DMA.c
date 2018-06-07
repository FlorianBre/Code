/*
 * DMA.c
 *
 *  Created on: 07.06.2018
 *      Author: Flo
 */
#include <msp430.h>
#include <lib\DMA.h>
void DMAinit(unsigned int trigSrc, volatile unsigned int *srcAdr, volatile unsigned int *desAdr, unsigned int srcIncr, unsigned int desIncr, unsigned int transferSize ){
    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long ) srcAdr);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) desAdr);
    // Select ADC end of conversion.
    DMACTL0 |= trigSrc;
    // Single Block transfer
    DMA0SZ = transferSize;
    // Single Transfer Mode
    // select decrement or increment of source and destination adress
    DMA0CTL = DMADT_0 | srcIncr |desIncr;
    DMA0CTL |= DMAEN; // Enable DMA0
}


