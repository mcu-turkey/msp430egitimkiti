
#include "msp430.h"
#include "usci_a0.h"
#include "lcd_msp.h"

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  lcd_init();
  uart_init();

  lcd_puts(" fatihinanc.com");
  lcd_goto(2,1);
  lcd_puts("SW UART 9600 bps");
  
  delay_ms(2000);
  
  lcd_temizle();
  lcd_puts("Girilen Rakam");
  
  uart_puts("www.mcu-turkey.com\r\n");
  uart_puts("USCI_A0 9600 Baud UART Ornegi\r\n");
  uart_puts("Bir Rakam Giriniz;\r\n");  

  __bis_SR_register(LPM4_bits + GIE);  
}
