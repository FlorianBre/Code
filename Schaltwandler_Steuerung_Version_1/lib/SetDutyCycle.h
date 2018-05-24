/*
 * setDutyCycle.h
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */

#ifndef LIB_SETDUTYCYCLE_H_
#define LIB_SETDUTYCYCLE_H_

void initDutyCycle(int, int);
static void calculatePeriod(int,int);
static int getClockFrequency(int);
static void initTimer(int);
void selectDutyCycle(int);

#endif /* LIB_SETDUTYCYCLE_H_ */
