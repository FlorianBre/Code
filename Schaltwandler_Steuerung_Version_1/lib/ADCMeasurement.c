/*
 * ADCMeasurement.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
static void initADC(int, int, int, int, int, int);
int measure();

/*
 * @brief Method for Measuring.
 * @param select ACLK, differential or single mode, Sample and Hold time, Reference select
 * internal Reference select, ADC Method either polling or interrupt.
 * @return calculated power.
 */
void adcMeasurement(int clockSelect, int mode, int sampleHold, int refSel, int intSel, int ADCMethod){
    initADC(clockSelect, mode, sampleHold, refSel, intSel, ADCMethod);
}
static void initADC(clockSelect, mode, sampleHold, refSel, intSel, ADCMethod){

}

int measure(){
    return 0;
}

