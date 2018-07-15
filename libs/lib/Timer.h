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
 *        countMode: Select the timer count mode (MC_X).
 *        countVal: Select TAxCCR0 value.
 */
void timerInitCounterA0(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A1
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        countMode: Select the timer count mode (MC_X).
 *        countVal: Select TAxCCR0 value.
 */
void timerInitCounterA1(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A2.
 * @param clockSelect: Select timer clock (TASSEL_X).
 *        countMode: Select the timer count mode (MC_X).
 *        countVal: Select TAxCCR0 value.
 */
void timerInitCounterA2(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A3
 * @param clockSelect: Select timer clock (TASSEL_X).
 *        countMode: Select the timer count mode (MC_X).
 *        countVal: Select TAxCCR0 value.
 */
void timerInitCounterA3(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer B0
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        countMode: Select the timer count mode (MC_X).
 *        countVal: Select TAxCCR0 value.
 */
void timerInitCounterB0(unsigned int, unsigned int, unsigned int);

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
 * @brief Method for initializing timer B0 in Capture Compare Mode
 * @param clockSelect: Select timer clock (TASSEL_X)
 *        captureCompareInput: Select capture Compare Input Channel (CCIS_0 = P3.4 , CCIS_1 = P6.4)
 *        edgeSelect: Selects on which edge the CC is triggered. (CM_X)
 */
void timerCaptureCompareB0(unsigned int, unsigned int, unsigned int);

/*
 * @brief Method for initializing timer A0 as PWM Timer. Outputs: P1.0, P1.6
 * @param periodCycles: Number of cycles for a period of the PWM signal.
 *        clockSelect: Select timer clock. (TASSEL_X)
 *        dutyCycle: Select the duty cycle.
 *        pwmOutputMode: Select the output mode. (OUTMOD_3 = set/reset, OUTMOD_7 = reset/set)
 */
void timerInitPWMA0(int, unsigned int, double, unsigned int);

/*
 * @brief Method for initializing timer A1 as PWM Timer. Outputs: P1.2, P3.3
 * @param periodCycles: Number of cycles for a period of the PWM signal.
 *        clockSelect: Select timer clock. (TASSEL_X)
 *        dutyCycle: Select the duty cycle.
 *        pwmOutputMode: Select the output mode. (OUTMOD_3 = set/reset, OUTMOD_7 = reset/set)
 */
void timerInitPWMA1(int, unsigned int, double, unsigned int);

/*
 * @brief Method for initializing timer B0 as PWM Timer with additional Output.
 *        Outputs: P2.1 (CCR5) additional Output P2.0 (CCR6)
 * @param periodCycles: Number of cycles for a period of the PWM signal.
 *        clockSelect: Select timer clock. (TASSEL_X)
 *        dutyCycle: Select the duty cycle.
 *        pwmOutputMode: Select the output mode. (OUTMOD_3 = set/reset, OUTMOD_7 = reset/set)
 */
void timerInitPWMB0(int, unsigned int, double, unsigned int, unsigned int);

/*
 * @brief Method for changing the duty cycle of the timer A0 in PWM mode.
 * @param dutyCycle: Select the duty cycle.
 */
void timerSetDutyCycleA0(double);

/*
 * @brief Method for changing the duty cycle of the timer A1 in PWM mode.
 * @param dutyCycle: Select the duty cycle.
 */
void timerSetDutyCycleA1(double);

/*
 * @brief Method for changing the duty cycle of the timer B0 in PWM mode.
 * @param dutyCycle: Select the duty cycle.
 */
void timerSetDutyCycleB0(double);

#endif /* LIB_TIMER_H_ */
