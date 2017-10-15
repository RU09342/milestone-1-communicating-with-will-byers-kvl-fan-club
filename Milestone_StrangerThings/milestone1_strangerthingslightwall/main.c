/*  Milestone 1: Stranger Things Light Wall
 *      Author: Damon Boorstein
 *              Rowan University
 *      Date Created: 10/12/17
 *          Updated: 10/14/17
 */

#include <msp430fr6989.h>

#define RECEIVE     BIT3;
#define TRANSMIT    BIT1;
#define LED2        BIT7;
#define LED1        BIT0;

void initGPIO();
void initTimer();
void initUART();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       // Disable GPIO default high-impedance mode
	//FR2xxx, FR4xxx, FR58xx, FR59xx, FR6xxx devices
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
	FRCTL0 = FWPW | NWAITS0; // Change the NACCESS_x value to add the right amount of waitstates, FWPW is the FRAM write password

    initGPIO();            // initialize GPIO
    initTimer();           // initialize Timer module
    initUART();            // initialize UART registers

    __bis_SR_register(LPM3_bits + GIE);     // enter LPM3, interrupts enabled
    __no_operation();                       // for debugging
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }

}

void initGPIO(void){
    // Enable primary module function (UART mode) on RXD and TXD pins
    P4SEL1 &= ~RECEIVE;             // P4.3 = RXD
    P4SEL0 |= RECEIVE;

    P2SEL1 &= ~TRANSMIT;            // P2.1 = TXD
    P2SEL0 |= TRANSMIT;

    // Initialize unused ports to reduce current consumption
    P1DIR = LED1;                       /* Port 1 */
    P1OUT &= ~LED2;
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
    P9OUT &= ~LED2;
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
      TA0CCR0 = 255;            // Period, 100% Duty Cycle
      TA0CCTL1 = OUTMOD_7;      // Output = Reset/Set
      TA0CCR1 = 122;            // initialize to 50% Duty Cycle
      TA0CTL = TASSEL__SMCLK + MC__UP + ID__4; // SMCLK/4 = ~4usec, Up mode
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
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

