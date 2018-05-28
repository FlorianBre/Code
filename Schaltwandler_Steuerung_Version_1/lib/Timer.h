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
void timerCounterA0(unsigned int);

/*
 * @brief Method for initializing timer A1
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA1(unsigned int);

/*
 * @brief Method for initializing timer A2
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA2(unsigned int);

/*
 * @brief Method for initializing timer A3
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterA3(unsigned int);

/*
 * @brief Method for initializing timer B0
 * @param clockSelect: Select timer clock (TASSEL_X)
 */
void timerCounterB0(unsigned int);

/*
 * @brief Method for initializing timer A0 in Capture Compare Mode
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel (CCIS_0 = P1.5 , CCIS_1 = P7.1)
 *        edgeSelect: Selects on which edge the CC is triggered. (CM_X)
 */
void timerCaptureCompareA0(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A1 in Capture Compare Mode
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel (CCIS_0 = P1.4 , CCIS_1 = P5.2)
 *        edgeSelect: Selects on which edge the CC is triggered. (CM_X)
 */
void timerCaptureCompareA1(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A0 in Capture Compare Mode
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel (CCIS_0 = P3.4 , CCIS_1 = P6.4)
 *        edgeSelect: Selects on which edge the CC is triggered. (CM_X)
 */
void timerCaptureCompareB0(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A0 in Capture Compare Mode.
 * @param clockSelect: Select timer clock. (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel. (CCIS_0 = P3.4 , CCIS_1 = P6.4)
 *        edgeSelect: Selects on which edge the CC is triggered. (CM_X)
 */

void timerInitPWMA0(int periodCycles, unsigned int clockSelect, double dutyCycle, unsigned int pwmOutputMode);
/*
 * @brief Method for initializing timer A0 as PWM Timer. Outputs: P1.5, P7.1, P10.1
 * @param periodCycles: Number of cycles for a period of the PWM signal.
 *        clockSelect: Select timer clock. (TASSEL_X)
 *        dutyCycle: Select the duty cycle.
 *        pwmOutputMode: Select the output mode. (OUTMOD_3 = set/reset, OUTMOD_7 = reset/set)
 */
void timerSetDutyCycleA0(double dutyCycle);

#endif /* LIB_TIMER_H_ */
