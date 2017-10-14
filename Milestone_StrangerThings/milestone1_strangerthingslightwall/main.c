/***Milestone 1: Stranger Things Light Wall***
 *      Author: Damon Boorstein
 *              Rowan University
 *      Date Created: 10/12/17
 *          Updated: 10/12/17
 */

#include <msp430g2553.h>
#include "initGPIO.h"
#include "initTIMER.h"

void initGPIO();
void initTIMER();

/**
 * main.c
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    if (CALBC1_1MHZ == 0xFF)                    // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;

    void initGPIO();
    void initTIMER();

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
	
	return 0;
}
