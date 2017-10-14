/*
 * initGPIO.c
 *
 *  Created on: Oct 12, 2017
 *      Author: Damon Boorstein
 */

#include <msp430g2553.h>

#define GPIO_SEL0   P1SEL
#define GPIO_SEL2   P1SEL2
#define RECEIVE     BIT0
#define TRANSMIT    BIT1

void initGPIO(void){
    GPIO_SEL0 = RECEIVE + TRANSMIT;      // P1.1 = RXD, P1.2 = TXD
    GPIO_SEL2 = RECEIVE + TRANSMIT;
}
