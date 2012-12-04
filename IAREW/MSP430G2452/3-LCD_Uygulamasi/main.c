
#include "io430.h"
#include "lcd_msp.h"

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  lcd_init();
  
  lcd_goto(1,2);
  lcd_puts("mcu-turkey.com");
  delay_ms(1000);
  lcd_goto(2,1);
  lcd_puts("M430 Egitim Kiti");
  
  __bis_SR_register(LPM4_bits + GIE);  
}
