/*
 * ADCMeasurement.h
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */

#ifndef LIB_ADCMEASUREMENT_H_
#define LIB_ADCMEASUREMENT_H_

/*
 * @brief Method for initialize the ADC converter.
 * @param select ACLK, differential or single mode, Sample and Hold time, Reference select
 * internal Reference select, ADC Method either polling or interrupt.
 */
extern void initADC(int, int, int, int, int);


/*
 * @brief Method for initialize the ADC converter.
 * @param Select ADC Method either polling or interrupt.
 * @return Returns the Measured Value.
 */
extern int measure(int);

#endif /* LIB_ADCMEASUREMENT_H_ */
