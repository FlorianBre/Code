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
 * LCD_Launchpad.h
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
 ******************************************************************************/


#ifndef LCD_H_
#define LCD_H_
/*
 * @brief Method for initializing the built in LCD Display.
 */
void lcdInit();

/*
 * @brief Method Displays a character at the selected position.
 * @param symb: Symbol which should be displayed.
 *        position: Segment on which the symbol should be displayed.
 */
void lcdShowChar(char, int);

/*
 * @brief Display a text at the selected position.
 * @param symb: Symbol which should be displayed.
 *        position: Offset of the text.
 */
void lcdShowText(char* , int, int);
void lcdShowInt(unsigned long long);
void lcdShowDouble(double);
void lcdShowDutyCycle(double);

/*
 * @brief Clear the display
 */
void lcdClear();

#endif /* LCD_H_ */
