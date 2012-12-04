
#include "msp430.h"
#include "ds1302.h"
#include "lcd_msp.h"
#include "degisken.h"

void lcd_saat_goster(void);
void lcd_tarih_goster(void);

unsigned char saat[8]; 
unsigned char tarih[10]; 

void main(void)
{  
  WDTCTL = WDTPW + WDTHOLD;  
  BCSCTL1= CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  __delay_cycles(0xFFFF);
  
  init_DS1302();
  Reset_DS1302();
  lcd_init();
  
  TA0CCR0  = 65535-1;
  TA0CCTL0 = CCIE;
  TA0CTL   = MC_2 + TASSEL_2 + TACLR;
  
  __bis_SR_register(LPM1_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  BurstRead1302(rdata);
  
  lcd_saat_goster();
  lcd_tarih_goster(); 
} 

void lcd_saat_goster(void)
{  
  char i;
  
  saat[6] = rakam[(rdata[0]&0xF0) >> 4];
  saat[7] = rakam[ rdata[0]&0x0F];
  saat[3] = rakam[(rdata[1]&0xF0) >> 4];
  saat[4] = rakam[ rdata[1]&0x0F];
  saat[0] = rakam[(rdata[2]&0xF0) >> 4];
  saat[1] = rakam[ rdata[2]&0x0F];
  
  saat[2]=':'; 
  saat[5]=':'; 
  
  lcd_goto(1,1);
  
  for(i=0;i<8;i++)
    lcd_putch(saat[i]);
}

void lcd_tarih_goster(void)
{
  char i;
  
  tarih[0] = rakam[(rdata[3]&0xF0) >> 4];
  tarih[1] = rakam[ rdata[3]&0x0F];
  tarih[3] = rakam[(rdata[4]&0xF0) >> 4];
  tarih[4] = rakam[ rdata[4]&0x0F];
  tarih[6] = rakam[(rdata[6]&0xF0) >> 4];
  tarih[7] = rakam[ rdata[6]&0x0F];
  tarih[8] = rakam[(rdata[5]&0xF0) >> 4];
  tarih[9] = rakam[ rdata[5]&0x0F];  
  
  tarih[2]='/'; 
  tarih[5]='/';
  
  lcd_goto(1,12);
  for(i=0;i<5;i++)
    lcd_putch(tarih[i]);
  
  lcd_goto(2,13);
  lcd_puts("20");
  for(i=6;i<8;i++)
    lcd_putch(tarih[i]);
  
  lcd_goto(2,1);
  switch(rdata[5])
  {
  case 1 :lcd_puts("Pazartesi"); break;
  case 2 :lcd_puts("Sali     "); break;
  case 3 :lcd_puts("Carsamba "); break;
  case 4 :lcd_puts("Persembe "); break;
  case 5 :lcd_puts("Cuma     "); break;
  case 6 :lcd_puts("Cumartesi"); break;
  case 7 :lcd_puts("Pazar    "); break;
  }
}