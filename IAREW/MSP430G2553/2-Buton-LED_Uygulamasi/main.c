
//LaunchPad Üzerindeki TXD ve RXD Jumplerlarýný Çýkarýnýz

#include "msp430.h"

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(1000000);
  
  P2DIR = 0x00;
  P2IE  = 0xFF;
  P2IES = 0x00;
  P2SEL = 0x00;
  P2SEL2= 0x00;  
  
  P1DIR = 0xFF;
  
  P1OUT = 0x00; 
  P2IFG = 0x00;
  
  __bis_SR_register(LPM4_bits + GIE);
}

// Port 2 Kesme Rutini
#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(void)
{
  P1OUT = P2IFG;
  P2IFG = 0x00;
}