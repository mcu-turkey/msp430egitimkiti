
#include "msp430.h"
#include "onewire.h"
#include "ds18b20.h"
#include "lcd_msp.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(500000);
 
  P2DIR |= BIT4;
  
  TA0CCTL0 = CCIE;  
  TA0CCR0  = 50000-1;  
  TA0CTL   = MC_3 + TASSEL_2;
  
  lcd_init();
  onewire_init(&ow);
  
  lcd_goto(1,2);
  lcd_puts("mcu-turkey.com");
  lcd_goto(2,2);
  lcd_puts("DS18B20 Testi");
  __delay_cycles(1000000);
  
  lcd_temizle();
  lcd_goto(1,1);
  lcd_puts("DS18B20");
  lcd_goto(2,1);
  lcd_puts("Sicaklik");
  lcd_goto(2,15); lcd_puts("oC");
  lcd_goto(1,10); lcd_putch('|');
  lcd_goto(2,10); lcd_putch('|');
  
  __bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{    
  ds18b20_read(); 
  ds18b20_lcdyaz(ds18b20_byte,ds18b20_word);
}