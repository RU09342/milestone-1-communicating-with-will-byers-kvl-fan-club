/*
 * initGPIO.c
 *
 *  Created on: Oct 12, 2017
 *      Author: Damon Boorstein
 */

#include <msp430fr6989.h>

#define RECEIVE     BIT3
#define TRANSMIT    BIT1
#define LED1        BIT0
#define LED2        BIT7

void initGPIO(void){
    PM5CTL0 &= ~LOCKLPM5;           // Disable GPIO default high-impedance mode

    P4SEL1 &= ~RECEIVE;             // P4.3 = RXD
    P4SEL0 |= RECEIVE;

    P2SEL1 &= ~TRANSMIT;            // P1.2 = TXD
    P2SEL0 |= TRANSMIT;

    // Initialize unused ports to reduce current consumption
    P1DIR = LED1;                       /* Port 1 */
    P1OUT = 0x00;
    P3DIR = 0xFF;                       /* Port 3 */
    P3OUT = 0x00;
    P5DIR = 0xFF;                       /* Port 5 */
    P5OUT = 0x00;
    P6DIR = 0xFF;                       /* Port 6 */
    P6OUT = 0x00;
    P7DIR = 0xFF;                       /* Port 7 */
    P7OUT = 0x00;
    P8DIR = 0xFF;                       /* Port 8 */
    P8OUT = 0x00;
    P9DIR = LED2;                       /* Port 9 */
    P9OUT = 0x00;
    PADIR = 0xFF;                       /* Port A */
    PAOUT = 0x00;
    PBDIR = 0xFF;                       /* Port B */
    PBOUT = 0x00;
    PCDIR = 0xFF;                       /* Port C */
    PCOUT = 0x00;
    PDDIR = 0xFF;                       /* Port D */
    PDOUT = 0x00;
    PEDIR = 0xFF;                       /* Port E */
    PEOUT = 0x00;

}
