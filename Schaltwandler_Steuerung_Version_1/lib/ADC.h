/*
 * ADCMeasurement.h
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */

#ifndef LIB_ADC_H_
#define LIB_ADC_H_

/*
 * @brief Method for initialize the ADC converter.
 * @param clockSelect0: select ADC12CLK0 (ADC12SSEL0), clockSelect1: Select ADC12CLK1 (ADC12SSEL1)
 *        differentialModeSelect: select differential or single mode (ADC12DIF),
 *        sampleHoldSelect: select Sample and Hold time (ADC12SHT0_X),
 *        refSelect: select Reference (ADC12VRSEL_X),
 *        intRefSelecet: select internal reference select,
 *        channelSelect: select ADC channel (ADC12INCH_X)
 */
void adcInit(int, int, int, int, int, int, int);

/*
 * @brief  Method for carry out the ADC Measurement.
 *         Active Waiting during ADC Measurement
 * @return Converted ADC value.
 */
int adcMeasurementPolling();

/*
 * @brief  Method for carry out the ADC Measurement.
 *         Set Operationmode to sleeping Mode during ADC Measurement
 */
void adcMeasurementInterrupt();
static void adcStartMeasurement();
#endif /* LIB_ADC_H_ */
