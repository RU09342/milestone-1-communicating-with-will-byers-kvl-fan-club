#include <msp430.h> 


unsigned int count = 0;
void initializeTimer();

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  P9DIR |= BIT7;
  P9OUT &= ~BIT7;
  initializeTimer();
  // Set up timer outputs
  P2DIR |= (BIT6 + BIT7); //CCR2 and CCR3
  P2SEL0 |= (BIT6+ BIT7);
  P2SEL1 &= ~(BIT6 + BIT7);

  P2DIR |= BIT5; //CCR4
  P2SEL0 |= BIT5;
  P2SEL1 &= ~(BIT5);

  // Configure GPIO
  P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 UART operation
  P2SEL1 &= ~(BIT0 | BIT1);

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Startup clock system with max DCO setting ~8MHz
  CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
  CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
  // Baud Rate calculation
  // 8000000/(16*9600) = 52.083
  // Fractional portion = 0.083
  // User's Guide Table 21-4: UCBRSx = 0x04
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BR0 = 52;                             // 8000000/16/9600
  UCA0BR1 = 0x00;
  UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
  UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}

void initializeTimer(void)
{
    TB0CCR0 = 255; // Set max value
    TB0CCTL4 = OUTMOD_7; // set output on reset
    TB0CCR4 = 0x00; // initialize compare register for 50% cycle
    TB0CTL = TBSSEL_2 + MC_1; // Use the SMCLK and up mode
    TB0CCTL5 = OUTMOD_7;
    TB0CCR5 = 0xFF;
    TB0CCTL6 = OUTMOD_7;
    TB0CCR6 = 0x00;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)

{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
      case USCI_NONE: break;
      case USCI_UART_UCRXIFG:
            if (count == 0)
            {
                while(!(UCA0IFG & UCTXIFG)); // Wait until buffer is ready
                UCA0TXBUF = (int)UCA0RXBUF - 3;
            }

            else if (count == 1)
            {
                TB0CCR4 = 255 - UCA0RXBUF;
            }

            else if (count == 2)
            {
                TB0CCR5 = 255 - UCA0RXBUF;
            }

            else if (count == 3)
            {
                TB0CCR6 = 255 - UCA0RXBUF;
            }

            else
            {
                while (!(UCA0IFG&UCTXIFG));
                   UCA0TXBUF = UCA0RXBUF;
            }
            if (UCA0RXBUF != 0x0D)
                count++;
            else if (UCA0RXBUF == 0x0D)
                count = 0;
            break;
      case USCI_UART_UCTXIFG: break;
      case USCI_UART_UCSTTIFG: break;
      case USCI_UART_UCTXCPTIFG: break;
    }
}
