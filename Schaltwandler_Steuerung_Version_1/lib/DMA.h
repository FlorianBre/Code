/*
 * DMA.h
 *
 *  Created on: 07.06.2018
 *      Author: Flo
 */

/*
 * @brief Method for initializing the Direct Memory Access.
 * @param triggerSource: Select source of triggering the transfer.
 *        srcAdr: Select source address.
 *        desAdr: Select destination address.
 *        srcInc: Select source no increment, increment, decrement of source address.
 *        desInc: Select source no increment, increment, decrement of destination address.
 *        transferSize: Select amount of byte transfers.
 *
 */
void DMAinit(unsigned int, volatile unsigned int *, volatile unsigned int *, unsigned int, unsigned int, unsigned int);


