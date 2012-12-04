
#include "lcd_msp.h"

unsigned char ds18b20_byte = 0;
unsigned int  ds18b20_word = 0;

onewire_t ow;

void ds18b20_read(void)
{
  unsigned char high_byte,low_byte,i;
  unsigned int  val = 625;
  
  ds18b20_word = 0;
  
  while(onewire_reset(&ow));
  onewire_write_byte(&ow, SKIPROM_CMD); // skip ROM command
  onewire_write_byte(&ow, CONVERT_TMP); // convert T command
  delay_ms(800); // at least 750 ms for the default 12-bit resolution
  
  while(onewire_reset(&ow));
  onewire_write_byte(&ow, SKIPROM_CMD); // skip ROM command
  onewire_write_byte(&ow, READ_SCRATCHPAD); // read scratchpad command
  delay_us(120);
  
  low_byte  = onewire_read_byte(&ow);
  high_byte = onewire_read_byte(&ow);
  
  ds18b20_byte = (low_byte >> 4) + (high_byte << 4);
  
  for(i = 0; i < 4; i++)
  {
    if(low_byte & 0x01)
      ds18b20_word += val;
      val  <<= 1;
      low_byte >>= 1;
  }
}

void ds18b20_lcdyaz (unsigned char t_byte, unsigned int t_word)
{
  lcd_goto(1,12);
  lcd_putch(t_byte/10 + 48);
  lcd_putch(t_byte%10 + 48);
  lcd_putch('.');
  lcd_putch((t_word/1000)+48);
  lcd_putch((t_word/100)%10+48);
  lcd_putch((t_word/10)%10+48);
  lcd_putch((t_word)%10+48);

}