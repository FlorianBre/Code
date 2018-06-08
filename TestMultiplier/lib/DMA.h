/*
 * DMA.h
 *
 *  Created on: 07.06.2018
 *      Author: Flo
 */

/*
 * @brief Method for initializing the Direct Memory Access Channel 0.
 * @param triggerSource: Select source of triggering the transfer. (DMA0TSEL_X)
 *        srcAdr: Select source address.
 *        desAdr: Select destination address.
 *        srcInc: Select source no increment, increment, decrement of source address. (DMASRCINCR_X)
 *        desInc: Select source no increment, increment, decrement of destination address. (DMADSTINCR_X)
 *        transferSize: Select amount of transfers.
 *
 */
void DMAinit0(unsigned int, volatile unsigned int *, volatile unsigned int *, unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing the Direct Memory Access Channel 1.
 * @param triggerSource: Select source of triggering the transfer. (DMA1TSEL_X)
 *        srcAdr: Select source address.
 *        desAdr: Select destination address.
 *        srcInc: Select source no increment, increment, decrement of source address. (DMASRCINCR_X)
 *        desInc: Select source no increment, increment, decrement of destination address. (DMADSTINCR_X)
 *        transferSize: Select amount of transfers.
 *
 */
void DMAinit1(unsigned int, volatile unsigned int *, volatile unsigned int *, unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing the Direct Memory Access Channel 2.
 * @param triggerSource: Select source of triggering the transfer. (DMA2TSEL_X)
 *        srcAdr: Select source address.
 *        desAdr: Select destination address.
 *        srcInc: Select source no increment, increment, decrement of source address. (DMASRCINCR_X)
 *        desInc: Select source no increment, increment, decrement of destination address. (DMADSTINCR_X)
 *        transferSize: Select amount of transfers.
 *
 */
void DMAinit2(unsigned int, volatile unsigned int *, volatile unsigned int *, unsigned int, unsigned int, unsigned int);
