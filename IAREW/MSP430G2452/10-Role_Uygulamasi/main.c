
// LaunchPad Üzerindeki TXD ve RXD Jumplerlarýný Çýkarýnýz
// SW4 Üzerindeki P1.2 ve P1.4 LED lerini açiniz.

#include "msp430.h"

#define	ROLE1(x)       ( (x) ? (P1OUT |= BIT2) : (P1OUT &= ~BIT2) )
#define ROLE2(x)       ( (x) ? (P1OUT |= BIT4) : (P1OUT &= ~BIT4) )
#define	ROLE1_TERSLE   (P1OUT ^= BIT2)
#define ROLE2_TERSLE   (P1OUT ^= BIT4)

unsigned int  tick  = 0;

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(1000000);
  
  P1DIR = BIT2 + BIT4;
  P1OUT = 0x00;
  
  ROLE1(0);
  ROLE2(1);
  
  TA0CTL   = TASSEL_2 + MC_1 + TACLR;
  TA0CCR0  = 50000 - 1;
  TA0CCTL0 = CCIE;

  //LPM1 Düþük güç moduna giriliyor ve kesmeler aktif
  __bis_SR_register(LPM1_bits + GIE);  
}

//TimerA0 Kesme Rutini
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMERA0_A0_ISR(void)
{
  tick++;
  
  if(tick == 40)        //2 saniye
    ROLE1_TERSLE;
  
  if(tick == 80)
  {
    ROLE2_TERSLE;
    tick = 0;
  }
}
