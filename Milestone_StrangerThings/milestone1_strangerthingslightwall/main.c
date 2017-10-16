/*  Milestone 1: Stranger Things Light Wall
 *      Author: Damon Boorstein
 *              Rowan University
 *      Date Created: 10/12/17
 *          Updated: 10/14/17
 */

#include <msp430fr6989.h>

#define RECEIVE     BIT1
#define TRANSMIT    BIT0
#define RED         BIT5
#define GREEN       BIT6
#define BLUE        BIT7

void initGPIO(void);
void initTimer(void);
void initUART(void);

unsigned int count = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       // Disable GPIO default high-impedance mode

    initGPIO();            // initialize GPIO
    initTimer();           // initialize Timer module
    initUART();            // initialize UART registers

    __bis_SR_register(LPM3_bits + GIE);     // enter LPM3, interrupts enabled
    __no_operation();                       // for debugging
    return 0;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        switch(count){
          case 0:
              while(!(UCA0IFG&UCTXIFG));
                  UCA0TXBUF = (int)UCA0RXBUF-3; // '(int)' converts to integer subtraction
              __no_operation();
              break;
          case 1:
              TB0CCR4 = UCA0RXBUF;
              __no_operation();
              break;
          case 2:
              TB0CCR5 = UCA0RXBUF;
              __no_operation();
              break;
          case 3:
              TB0CCR6 = UCA0RXBUF;
              count = 0;
              __no_operation();
              break;
          default:
              UCA0RXBUF = UCA0RXBUF;
              count = 0;
              break;
          }
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }

}

void initGPIO(void){
    // Enable primary module function (PWM) on RXD and TXD pins
    P2SEL0 |= BLUE+GREEN+RED+TRANSMIT+RECEIVE;         // P2.0 = TXD, P2.1 = RXD
    P2SEL1 &= ~(BLUE+GREEN+RED+TRANSMIT+RECEIVE);
    P2DIR |= RED+GREEN+BLUE;            // P2.5 RED, P2.6 GREEN, P2.7 BLUE

    // Initialize unused ports to reduce current consumption
    P1DIR = 0xFF;                       /* Port 1 */
    P1OUT = 0x00;
    P3DIR = 0xFF;                       /* Port 3 */
    P3OUT = 0x00;
    P4DIR = 0xFF;                       /* Port 4 */
    P4OUT = 0x00;
    P5DIR = 0xFF;                       /* Port 5 */
    P5OUT = 0x00;
    P6DIR = 0xFF;                       /* Port 6 */
    P6OUT = 0x00;
    P7DIR = 0xFF;                       /* Port 7 */
    P7OUT = 0x00;
    P8DIR = 0xFF;                       /* Port 8 */
    P8OUT = 0x00;
    P9DIR = 0xFF;                      /* Port 9 */
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

void initTimer(){
      // Clock system setup
      CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
      CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
      CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
      CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
      CSCTL0_H = 0;                             // Lock CS registers

      // PWM timer initialization
      TB0CCR0 = 255;            // Period, 100% Duty Cycle
      TB0CCTL1 = OUTMOD_7;      // Output = Reset/Set
      TB0CCR1 = 122;            // initialize to 50% Duty Cycle
      TB0CCTL4 = OUTMOD_7;
      TB0CCR4 = 122;
      TB0CCTL5 = OUTMOD_7;
      TB0CCR5 = 122;
      TB0CCTL6 = OUTMOD_7;
      TB0CCR6 = 122;
      TB0CTL = TBSSEL__SMCLK + MC__UP + ID__8; // SMCLK/4 = ~4usec, Up mode
}

void initUART(){
    // Put eUSCI in reset
    UCA1CTLW1 = UCSWRST;
    UCA1CTLW0 = UCSWRST;
    UCA0CTLW1 = UCSWRST;
    UCA0CTLW0 = UCSWRST;

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    //  8000000/(16*9600) = 52.083
    //  Fractional portion = 0.083
    //  User's Guide Table 21-4: UCBRSx = 0x04
    //  UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 6;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~(UCSWRST + UCSYNC);                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

