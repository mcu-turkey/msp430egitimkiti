
// SW4 1 ve 2 kapali konumda...

#include "msp430.h"
#include "lcd_msp.h"
#include "yazilimsal_uart.h"

extern unsigned char rxBuffer; 

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  for(unsigned int i = 65000;i>0;i--);
  
  lcd_init();
  TimerA_UART_init();
  
  lcd_puts(" mcu-turkey.com");
  lcd_goto(2,1);
  lcd_puts("SW UART 9600 bps");
  
  delay_ms(2000);
  
  lcd_temizle();
  lcd_puts("Girilen Rakam");
  
  TimerA_UART_print("www.mcu-turkey.com\r\n");
  TimerA_UART_print("9600 Baud Timer_A Yazilimsal UART Ornegi\r\n");
  TimerA_UART_print("Bir Rakam Giriniz;\r\n");  
  
  while(1)
  {
    __bis_SR_register(LPM0_bits); 
    
    lcd_goto(2,1);
    lcd_putch(rxBuffer);
    
    switch(rxBuffer)
    {
    case '0': TimerA_UART_print(" = SIFIR\n\r"); lcd_puts(" = SIFIR"); break;
    case '1': TimerA_UART_print(" = BIR\n\r");   lcd_puts(" = BIR  "); break;
    case '2': TimerA_UART_print(" = IKI\n\r");   lcd_puts(" = IKI  "); break;
    case '3': TimerA_UART_print(" = UC\n\r");    lcd_puts(" = UC   "); break;
    case '4': TimerA_UART_print(" = DORT\n\r");  lcd_puts(" = DORT "); break;
    case '5': TimerA_UART_print(" = BES\n\r");   lcd_puts(" = BES  "); break;
    case '6': TimerA_UART_print(" = ALTI\n\r");  lcd_puts(" = ALTI "); break;
    case '7': TimerA_UART_print(" = YEDI\n\r");  lcd_puts(" = YEDI "); break;
    case '8': TimerA_UART_print(" = SEKIZ\n\r"); lcd_puts(" = SEKIZ"); break;    
    case '9': TimerA_UART_print(" = DOKUZ\n\r"); lcd_puts(" = DOKUZ"); break;    
    
    default : TimerA_UART_print(" = HATA !!!\n\r"); lcd_puts(" = HATA "); break;
    }  
  }
  
}
