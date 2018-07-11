/*
 * ADCMeasurement.h
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */

#ifndef LIB_ADC_H_
#define LIB_ADC_H_
// Adress of Calibration Values
#define ADC_ADC12VREF_FACTOR (unsigned int *) 0x01A1A
#define ADC_ADC20VREF_FACTOR (unsigned int *) 0x01A1E
#define ADC_ADC25VREF_FACTOR (unsigned int *) 0x01A24
#define CAL_ADC_OFFSET (unsigned int *) 0x01A18
#define CAL_ADC_GAIN_FACTOR (unsigned int *) 0x01A16
/*
 * @brief Method for initializing the ADC converter for single channel measurement.
 * @param clockSelect: select ADC12CLK (ADC12SSEL_X),
 *        differentialModeSelect: select differential or single mode (ADC12DIF)
 *        In differential Mode only analog channels with an even number can be selected.
 *        The next channel is automatically the differential partner  (e.g. select A4, pair: A4,A5)
 *        sampleHoldSelect: select Sample and Hold time (ADC12SHT0_X),
 *        refSelect: select Reference (ADC12VRSEL_X),
 *        intRefSelecet: select internal reference select (REFVSEL_X),
 *        channelSelect: select ADC channel (ADC12INCH_X),
 *        interruptEnable: Turn on or off complete ADC interrupt (ADC12IE0),
 *        triggerSelect: Select trigger of the ADC measurement (ADC12CTL1: ADC12SHS0, ADC12SHS1, ADC12SHS2).
 *        @return Correction Factor.
 */
void adcInitSingle(int, int, int, int, int, int, int, int);

/*
 * @brief Method for initializing the ADC converter for multiple channel measurement.
 *        (Only Channels in a row allowed).
 * @param clockSelect: select ADC12CLK (ADC12SSEL_X),
 *        differentialModeSelect: select differential or single mode (ADC12DIF)
 *        In differential Mode only analog channels with an even number can be selected.
 *        The next channel is automatically the differential partner  (e.g. select A4, pair: A4,A5)
 *        sampleHoldSelect: select Sample and Hold time (ADC12SHT0_X),
 *        refSelect: select Reference (ADC12VRSEL_X),
 *        intRefSelecet: select internal reference select (REFVSEL_X),
 *        interruptEnable: Turn on or off complete ADC interrupt (ADC12IE0),
 *        triggerSelect: Select trigger of the ADC measurement (ADC12CTL1: ADC12SHS0, ADC12SHS1, ADC12SHS2),
 *        startChannel: Select First ADC input channel,
 *        endChannel: Select Last ADC input channel.
 */
void adcInitSequence(int, int, int, int, int, int, int, int, int);

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
void adcStartMeasurement();
#endif /* LIB_ADC_H_ */
