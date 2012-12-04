
#include "msp430.h"
#include "lcd_msp.h"

unsigned int sicaklik=0;
volatile char tick;

void sicaklik_init(void);
void deger_goster(unsigned int,unsigned int);
void sicaklik_oku(void);
void integer_yaz(unsigned int,char);

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  TA0CTL = TASSEL_2 + TACLR;
  TA0CCR0 = 50000-1;
  TA0CCTL0 = CCIE;
  
  lcd_init();
  sicaklik_init();
  
  lcd_goto(1,2);
  lcd_puts("mcu-turkey.com");
  lcd_goto(2,2);
  lcd_puts("Sicaklik Olcme");
  delay_ms(2000);
  lcd_temizle();
  
  TA0CTL |= MC_1; //Timer Baþlatýlýyor...
  
  __bis_SR_register(LPM0_bits + GIE);
}

// TimerA0 Kesme Vektörü
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  tick++;
  if(tick==10)
  {
    sicaklik_oku();
    tick = 0;
  }
}

// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

void deger_goster(unsigned int ham,unsigned int ham_s)
{
  lcd_goto(1,1);
  lcd_puts("10 Bit = ");
  integer_yaz(ham,0);
  lcd_goto(2,1);
  lcd_puts("oC = ");
  integer_yaz(ham_s,1);
} 

void sicaklik_init()
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + MSC + ADC10IE;  
  ADC10CTL1 = INCH_10 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_2;
  ADC10DTC1 = 32;
  __delay_cycles(256);
}

void sicaklik_oku()
{
  char i;
  float yeni=0;
  unsigned int sicaklik_ham,sicaklik,toplam=0;
  unsigned int dma_buf[32];
  
  ADC10SA = (unsigned int)dma_buf;
  ADC10CTL0 |= ENC + ADC10SC;        
  __bis_SR_register(CPUOFF + GIE);    
  
  for(i=0;i<32;i++)
    toplam+=dma_buf[i];
  
  toplam >>= 5;
  
  sicaklik_ham = toplam;
  yeni=(((sicaklik_ham - 673) * 423) / 1024.0f)*100;
  sicaklik=(unsigned int)yeni;
  
  deger_goster(sicaklik_ham,sicaklik);
}

void integer_yaz(unsigned int deger,char flag)
{
  lcd_putch( deger/1000+48);
  lcd_putch((deger%1000)/100+48);
  if(flag==1)lcd_putch('.');
  lcd_putch((deger%100)/10+48);
  lcd_putch((deger)%10+48);
}