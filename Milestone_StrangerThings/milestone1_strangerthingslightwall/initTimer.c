/*
 * initTimer.c
 *
 *  Created on: Oct 14, 2017
 *      Author: nomad
 */

#include <msp430fr6989.h>

void initTimer(){
      // Clock system setup
      CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
      CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
      CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
      CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
      CSCTL0_H = 0;                             // Lock CS registers

      // PWM timers
      TA0CCR0 = 255;            // Period
      TA0CCTL1 = OUTMOD_7;      // Output = Reset/Set
      TA0CCR1 = 122;      // Duty Cycle
      TA0CTL = TASSEL__SMCLK + MC__UP + ID__4; // SMCLK/4 = ~4usec, Up mode

}
