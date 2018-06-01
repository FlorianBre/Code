/*
 * Display.c
 *
 *  Created on: 30.05.2018
 *      Author: Flo
 */
#include <lib/Display.h>
#include <msp430.h>

const char alphabetBig[26][2] =
{
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

void displayInit(){

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
}

void displayShow(int letter){
    LCDM8 = alphabetBig[letter][0];
    LCDM9 = alphabetBig[letter][1];
}



