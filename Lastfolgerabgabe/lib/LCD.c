/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * LCD_Launchpad.cpp
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 * on MSP-EXP430FR6989 and MSP-EXP430FR4133
 *
 * February 2015
 * E. Chen
 *
 * June 2015 StefanSch: Adopted for Energia
 *
 * Dec 12, 2015 Rei Vilo
 * 0b00000101 mask for LCDMEM[position+1] to avoid interferences with symbols
 * Tested on MSP430 FR4133 and FR6989
 *
 *May 2018 Florian Bregler:
 *Modified from cpp to c-file
 ******************************************************************************/
#include <msp430.h>
#include <lib/LCD.h>

#define pos1 9   /* Digit A1 begins at S18 */
#define pos2 5   /* Digit A2 begins at S10 */
#define pos3 3   /* Digit A3 begins at S6  */
#define pos4 18  /* Digit A4 begins at S36 */
#define pos5 14  /* Digit A5 begins at S28 */
#define pos6 7   /* Digit A6 begins at S14 */
const unsigned int digit_loc[6] =
{
 pos1,  // Position 1 = Digit A1
 pos2,  // Position 2 = Digit A2
 pos3,  // Position 3 = Digit A3
 pos4,  // Position 4 = Digit A4
 pos5,  // Position 5 = Digit A5
 pos6   // Position 6 = Digit A6
};

const char symbol[36][2] =
{
 {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
 {0x60, 0x20},  /* "1" */
 {0xDB, 0x00},  /* "2" */
 {0xF3, 0x00},  /* "3" */
 {0x67, 0x00},  /* "4" */
 {0xB7, 0x00},  /* "5" */
 {0xBF, 0x00},  /* "6" */
 {0xE4, 0x00},  /* "7" */
 {0xFF, 0x00},  /* "8" */
 {0xF7, 0x00},   /* "9" */
 {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
 {0xF1, 0x50},  /* "B" */
 {0x9C, 0x00},  /* "C" */
 {0xF0, 0x50},  /* "D" */
 {0x9F, 0x00},  /* "E" */
 {0x8F, 0x00},  /* "F" */
 {0xBD, 0x00},  /* "G" */
 {0x6F, 0x00},  /* "H" */
 {0x90, 0x50},  /* "I" */
 {0x78, 0x00},  /* "J" */
 {0x0E, 0x22},  /* "K" */
 {0x1C, 0x00},  /* "L" */
 {0x6C, 0xA0},  /* "M" */
 {0x6C, 0x82},  /* "N" */
 {0xFC, 0x00},  /* "O" */
 {0xCF, 0x00},  /* "P" */
 {0xFC, 0x02},  /* "Q" */
 {0xCF, 0x02},  /* "R" */
 {0xB7, 0x00},  /* "S" */
 {0x80, 0x50},  /* "T" */
 {0x7C, 0x00},  /* "U" */
 {0x0C, 0x28},  /* "V" */
 {0x6C, 0x0A},  /* "W" */
 {0x00, 0xAA},  /* "X" */
 {0x00, 0xB0},  /* "Y" */
 {0x90, 0x28}   /* "Z" */
};

void lcdInit(){
    LCDCMEMCTL = LCDCLRM; // Clear Memory
    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;
    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled
    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
    //Turn LCD on
    LCDCCTL0 |= LCDON;
}

void lcdShowChar(char symb, int position)
{
    position = digit_loc[position];
    if (symb >= 0 && symb <= 9) symb+= '0';
    if (symb >= 'a' && symb <= 'z') symb-= ('a' - 'A');
    if (symb == ' ')
    {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0 | (LCDMEM[position+1] & 0b00000101);
    }
    else if (symb >= '0' && symb <= '9')
    {
        // Display digit
        LCDMEM[position] = symbol[symb-48][0];
        LCDMEM[position+1] = symbol[symb-48][1] | (LCDMEM[position+1] & 0b00000101);
    }
    else if (symb >= 'A' && symb <= 'Z')
    {
        // Display alphabet
        LCDMEM[position] = symbol[symb-65+10][0];
        LCDMEM[position+1] = symbol[symb-65+10][1] | (LCDMEM[position+1] & 0b00000101);
    }
    else
    {
        // Turn all segments on if character is not a space, digit, or uppercase letter
        LCDMEM[position] = 0xFF;
        LCDMEM[position+1] = (0xFF & ~0b00000101) | (LCDMEM[position+1] & 0b00000101);
    }
}

void lcdShowText(char *a, int pos, int length)
{
    int i;
    for (i=0; i<pos; i++)
    {
        lcdShowChar(' ', i);
    }
    for (i=pos; i<(length-1+pos); i++)
    {
        lcdShowChar(a[i-pos], i);
        _no_operation();
    }
}

void lcdShowInt(unsigned long long number){

    /*
    int k = 1;
    int ergebnis = 0;
    do{
        ergebnis = number / (1000 * k);
        k*= 10;
        _no_operation();
    } while(ergebnis > 0); */

    unsigned long int i;
    int j = 0;
    char num[4];
    for(i = 1000;i >= 1; i = i/10){
        num[j] = (number % (i*10)) / i + '0' ;
        j++;
    }
    lcdShowText(num,1,5);
}

void lcdShowDouble(double number){
    int i;
    int j = 0;
    int k;
    int cipher;
    char num[4];
    for(i = 1; i <= 1000; i = i*10){
        cipher = number * i;
        int factor = 1;
        for(k = 0; k < j; k++){
            cipher %= i/(factor) ;
            factor *= 10;
        }
        num[j] = cipher + '0';
        j++;
    }
    lcdShowText(num,2,5);
    LCDM5 |= 0x01;
}

void lcdShowDutyCycle(double duty){
    lcdShowDouble(duty);
    lcdShowChar('D', 1);
    LCDM7 |= 0x04;

}
void lcdClear(){
    LCDCMEMCTL |= (LCDCLRM|LCDCLRBM);
}


