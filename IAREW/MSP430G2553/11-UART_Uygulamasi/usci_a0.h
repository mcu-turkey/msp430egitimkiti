#ifndef _USCI_A0_H
#define _USCI_A0_H

void uart_init (void);
void uart_putch(unsigned char);
void uart_write(char*,unsigned int);
void uart_puts (char*);

#endif  /* _USCI_A0_H */