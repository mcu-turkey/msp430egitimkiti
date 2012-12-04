
#include "msp430.h"
#include "lcd_msp.h"
#include "onewire.h"

void onewire_init(onewire_t *ow)
{
  ow->port_out = &P2OUT;
  ow->port_in  = &P2IN;
  ow->port_dir = &P2DIR;
  ow->pin      =  BIT4;
}

int onewire_reset(onewire_t *ow)
{ 
  *(ow->port_out) &=  ~ow->pin;
  *(ow->port_dir) |=  ow->pin;
  
  __delay_cycles(550); // 480us minimum
  *(ow->port_dir) &=  ~ow->pin;
  __delay_cycles(70);  // slave waits 15-60us
  
  
  if (*(ow->port_in) & ow->pin) 
    return 1; // line should be pulled down by slave
  
  __delay_cycles(300); // slave TX presence pulse 60-240us
 
  return 0;
}

void onewire_write_bit(onewire_t *ow, int bit)
{
  __delay_cycles(2);   // recovery, min 1us
  *(ow->port_dir) |=   ow->pin;
  *(ow->port_out) &=  ~ow->pin;
  
  if (bit)
    __delay_cycles(6); // max 15us
  else
    __delay_cycles(64); // min 60us
  
  *(ow->port_dir) &=  ~ow->pin; // rest of the write slot
  
  if (bit)
    __delay_cycles(64);
  else
    __delay_cycles(6);
}

int onewire_read_bit(onewire_t *ow)
{
  int bit;
  __delay_cycles(2); // recovery, min 1us
  *(ow->port_out) &=  ~ow->pin;
  *(ow->port_dir) |=  ow->pin;
  
  __delay_cycles(5); // hold min 1us
  *(ow->port_dir) &=  ~ow->pin;
  __delay_cycles(10); // 15us window
  
  bit = *(ow->port_in) & ow->pin;
  __delay_cycles(60); // rest of the read slot
  
  return bit;
}

void onewire_write_byte(onewire_t *ow, unsigned char byte)
{
  int i;
  for(i = 0; i < 8; i++)
  {
    onewire_write_bit(ow, byte & 1);
    byte >>= 1;
  }
}

unsigned char onewire_read_byte(onewire_t *ow)
{
  int i;
  unsigned char byte = 0;
  for(i = 0; i < 8; i++)
  {
    byte >>= 1;
    if (onewire_read_bit(ow)) byte |= 0x80;
  }
  return byte;
}