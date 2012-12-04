
//LaunchPad Üzerindeki TXD ve RXD Jumplerlarýný Çýkarýnýz

#include "msp430.h"

void delay_ms(unsigned int ms)
{
  while(ms--)
    __delay_cycles(1000);
}

void main( void )
{
  int a=1,b=128;
  
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P2SEL = 0x00;
  P2SEL2= 0x00;

  while(1)
  {
    P1OUT = a;
    a<<=1;
    P2OUT = b;
    b>>=1;
   
    delay_ms(50);
    
    if(a > 128 && b < 1)
    {
      a=1;
      b=128;
    }
  }
}
