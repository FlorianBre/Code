/*
 * SetDutyCycle.c
 *
 *  Created on: 23.05.2018
 *      Author: Flo
 */
static int calculatePeriod();
static void initTimer();
void setDutyCycle(double duty, int frequency) {
    int time = calculatePeriod();
    initTimer(time);
}
int calculatePeriod(){
    return 0;
}
void initTimer(){

}




