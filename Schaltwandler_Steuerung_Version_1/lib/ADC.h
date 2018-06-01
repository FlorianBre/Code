/*
 * ADCMeasurement.h
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */

#ifndef LIB_ADC_H_
#define LIB_ADC_H_

/*
 * @brief Method for initializing the ADC converter.
 * @param clockSelect0: select ADC12CLK0 (ADC12SSEL0),
 *        clockSelect1: Select ADC12CLK1 (ADC12SSEL1),
 *        differentialModeSelect: select differential or single mode (ADC12DIF)
 *        In differential Mode only analog channels with an even number can be selected.
 *        The next channel is automatically the differential partner  (e.g. select A4, pair: A4,A5)
 *        sampleHoldSelect: select Sample and Hold time (ADC12SHT0_X),
 *        refSelect: select Reference (ADC12VRSEL_X),
 *        intRefSelecet: select internal reference select (REFVSEL_X)
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
 *         Set Operation mode to sleeping Mode during ADC Measurement
 */
void adcMeasurementInterrupt();

/*
 * @brief  Method to start the ADC Measurement.
 */
static void adcStartMeasurement();
#endif /* LIB_ADC_H_ */
