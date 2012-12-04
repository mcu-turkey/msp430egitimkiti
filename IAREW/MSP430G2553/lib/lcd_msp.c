#include "msp430.h"
#include "lcd_msp.h"

void lcd_putch(char c)
{
  LCD_RS(1); 
  
  delay_us(200);       
         
  LCD_DATA =  (c & 0xF0);
  EN_ACKAPA();
  LCD_DATA = ((c & 0x0F)<<4);
  EN_ACKAPA();
}

void lcd_komut(unsigned char c)
{
  LCD_RS(0);
  
  delay_us(200);      
  
  LCD_DATA =  (c & 0xF0);
  EN_ACKAPA();
  LCD_DATA = ((c & 0x0F)<<4);
  EN_ACKAPA();
}

void lcd_temizle(void)
{  
  lcd_komut(0x01);
  delay_ms(2);
}

void lcd_puts(const char* s)
{ 
  while(*s)
    lcd_putch(*s++);
}

void lcd_goto(char x, char y)
{ 
  if(x==1)
    lcd_komut(0x80+((y-1)%16));
  else
    lcd_komut(0xC0+((y-1)%16));
}
	
void lcd_init()
{
  P1DIR |= BIT4 + BIT5 + BIT6 + BIT7; 
  P2DIR |= BIT6 + BIT7;
  P2SEL  = 0x00;
  P2SEL2 = 0x00;  
  P1OUT  = 0x00;
  P2OUT  = 0x00;
  
  LCD_RS(0);
  LCD_EN(0);

  lcd_komut(0x28);  // 4 Bit , Çift Satýr LCD
  lcd_komut(0x0C);  // Ýmleç Gizleniyor
  lcd_komut(0x06);  // Saða doðru yazma aktif
  lcd_komut(0x80);  // LCD Birinci Satýr Konumunda
  lcd_komut(0x28);  // 4 Bit , Çift Satýr LCD
  lcd_temizle();    // Ekran Temizleniyor 
}
