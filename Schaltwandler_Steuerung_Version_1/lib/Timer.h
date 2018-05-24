/*
 * Timer.h
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */

#ifndef LIB_TIMER_H_
#define LIB_TIMER_H_

void timerCounter();
void timerCaptureCompare(int, volatile unsigned int *, int);
void clockSourceSelect(int);



#endif /* LIB_TIMER_H_ */
