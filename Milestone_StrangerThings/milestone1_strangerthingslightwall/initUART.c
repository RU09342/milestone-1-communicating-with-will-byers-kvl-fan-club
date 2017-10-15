/*
 * initTIMER.c
 *
 *  Created on: Oct 12, 2017
 *      Author: Damon Boorstein
 */

#include <msp430fr6989.h>

void initUART(){
    UCA1CTLW1 = UCSWRST;                      // Put eUSCI in reset
    UCA1CTLW0 = UCSWRST;
    UCA0CTLW1 = UCSWRST;
    UCA0CTLW0 = UCSWRST;

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    //  8000000/(16*9600) = 52.083
    //  Fractional portion = 0.083
    //  User's Guide Table 21-4: UCBRSx = 0x04
    //  UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
