/*
 * DMA.c
 *
 *  Created on: 07.06.2018
 *      Author: Flo
 */
#include <msp430.h>
#include <lib\DMA.h>
void DMAinit0(unsigned int trigSrc, volatile unsigned int *srcAdr, volatile unsigned int *desAdr, unsigned int srcIncr, unsigned int desIncr, unsigned int transferSize, unsigned int interruptEnable ){
    //Source adress
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long ) srcAdr);
    // Destination adress.
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) desAdr);
    // Select Trigger.
    DMACTL0 |= trigSrc;
    // Amount of Transfers
    DMA0SZ = transferSize;
    // Single Transfer Mode
    // select decrement or increment of source and destination address.
    DMA0CTL = DMADT_0 | srcIncr |desIncr;
    DMA0CTL |= interruptEnable | DMAEN; // Enable DMA0
}

void DMAinit1(unsigned int trigSrc, volatile unsigned int *srcAdr, volatile unsigned int *desAdr, unsigned int srcIncr, unsigned int desIncr, unsigned int transferSize, unsigned int interruptEnable ){
    __data16_write_addr((unsigned short) &DMA1SA,(unsigned long ) srcAdr);
    __data16_write_addr((unsigned short) &DMA1DA,(unsigned long) desAdr);
    DMACTL0 |= trigSrc;
    DMA1SZ = transferSize;
    DMA1CTL = DMADT_0 | srcIncr |desIncr;
    DMA1CTL |= interruptEnable | DMAEN;
}

void DMAinit2(unsigned int trigSrc, volatile unsigned int *srcAdr, volatile unsigned int *desAdr, unsigned int srcIncr, unsigned int desIncr, unsigned int transferSize, unsigned int interruptEnable){
    __data16_write_addr((unsigned short) &DMA2SA,(unsigned long ) srcAdr);
    __data16_write_addr((unsigned short) &DMA2DA,(unsigned long) desAdr);
    DMACTL1 |= trigSrc;
    DMA2SZ = transferSize;
    DMA2CTL = DMADT_0 | srcIncr |desIncr;
    DMA2CTL |= interruptEnable | DMAEN; // Enable DMA0
}

