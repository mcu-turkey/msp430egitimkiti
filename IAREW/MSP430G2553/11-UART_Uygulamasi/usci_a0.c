#include "msp430.h"
#include "usci_a0.h"
#include "lcd_msp.h"
#include <stdlib.h>

unsigned char rx_buffer;

void uart_init(void)
{ 
  P1SEL  |= BIT1 + BIT2;                    // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2;                    // P1.1 = RXD, P1.2=TXD
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt  
}

void uart_putch( unsigned char temp )
{
  while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
  UCA0TXBUF = temp;
}

void uart_write( char* buffer, unsigned int length )
{
  unsigned int buffer_index;
  
  for( buffer_index = 0; buffer_index < length; buffer_index++ )
  {
    while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
    UCA0TXBUF = buffer[buffer_index];
  }
}

void uart_puts( char* buffer)
{
  while(*buffer)
  {
    while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
    UCA0TXBUF = (*buffer++);
  }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{  
  rx_buffer = UCA0RXBUF;
  
  lcd_goto(2,1);
  lcd_putch(rx_buffer);
  
  switch(rx_buffer)
  {
  case '0': uart_puts(" = SIFIR\n\r"); lcd_puts(" = SIFIR"); break;
  case '1': uart_puts(" = BIR\n\r");   lcd_puts(" = BIR  "); break;
  case '2': uart_puts(" = IKI\n\r");   lcd_puts(" = IKI  "); break;
  case '3': uart_puts(" = UC\n\r");    lcd_puts(" = UC   "); break;
  case '4': uart_puts(" = DORT\n\r");  lcd_puts(" = DORT "); break;
  case '5': uart_puts(" = BES\n\r");   lcd_puts(" = BES  "); break;
  case '6': uart_puts(" = ALTI\n\r");  lcd_puts(" = ALTI "); break;
  case '7': uart_puts(" = YEDI\n\r");  lcd_puts(" = YEDI "); break;
  case '8': uart_puts(" = SEKIZ\n\r"); lcd_puts(" = SEKIZ"); break;    
  case '9': uart_puts(" = DOKUZ\n\r"); lcd_puts(" = DOKUZ"); break;    
  
  default : uart_puts(" = HATA !!!\n\r"); lcd_puts(" = HATA "); break;
  }  
}
