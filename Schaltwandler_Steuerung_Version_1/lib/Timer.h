/*
 * Timer.h
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */

#ifndef LIB_TIMER_H_
#define LIB_TIMER_H_

void timerCounterA0(int);
void timerCounterA1(int);
void timerCounterA2(int);
void timerCounterA3(int);
void timerCounterB0(int);
void timerCaptureCompareA0(int, int);
void timerCaptureCompareA1(int, int);
void timerCaptureCompareB0(int, int);
void clockSourceSelect(int);

#endif /* LIB_TIMER_H_ */
