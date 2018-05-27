/*
 * Timer.h
 *
 *  Created on: 24.05.2018
 *      Author: Flo
 */

#ifndef LIB_TIMER_H_
#define LIB_TIMER_H_
/*
 * @brief Method for initializing timer A0
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA0(int);

/*
 * @brief Method for initializing timer A1
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA1(int);

/*
 * @brief Method for initializing timer A2
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA2(int);

/*
 * @brief Method for initializing timer A3
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA3(int);

/*
 * @brief Method for initializing timer B0
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterB0(int);

/*
 * @brief Method for initializing timer A0 in Capture Compare Mode
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel (CCIS_X)
 *        edgeSelect: Selects on which edge the CC is triggered.
 */
void timerCaptureCompareA0(int, int, int);
void timerCaptureCompareA1(int, int, int);
void timerCaptureCompareB0(int, int, int);

#endif /* LIB_TIMER_H_ */
