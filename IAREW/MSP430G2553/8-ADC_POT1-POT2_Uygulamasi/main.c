
//LaunchPad Üzerindeki TXD ve RXD Jumplerlarýný Çýkarýnýz
// A0 = P1.0
// A1 = P1.1
// DTC(Data Transfer Controller) Uygulamasý
// Çok Kanal ADC Okumak için kullanýlýr...

#include "msp430.h"
#include "lcd_msp.h"

void adc_init(void);
void deger_goster(unsigned int sonuc);

volatile char tick;

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  TA0CTL  = TASSEL_2 + TACLR;
  TA0CCR0 = 50000-1;
  TA0CCTL0= CCIE;
  
  lcd_init();
  adc_init();
  
  lcd_puts(" mcu-turkey.com");
  lcd_goto(2,2);
  lcd_puts("DTC Uygulamasi");
  delay_ms(2000);
  lcd_temizle();
  
  lcd_puts("A0 = ");
  lcd_goto(2,1);
  lcd_puts("A1 = ");
  
  TA0CTL |= MC_1;
  
  __bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  tick++;
  
  if(tick == 10)
  {
    unsigned int sonuc[2];
    
    ADC10CTL0 &= ~ENC;
    
    while (ADC10CTL1 & BUSY);             // Wait if ADC10 core is active
    ADC10SA = (unsigned int)sonuc;
    ADC10CTL0 |= ENC + ADC10SC;
    
    lcd_goto(1,6);  deger_goster(sonuc[1]);
    lcd_goto(2,6);  deger_goster(sonuc[0]);
    
    tick=0;
  }
}
// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  //__bic_SR_register_on_exit(CPUOFF);
  __no_operation();
}

void adc_init(void)
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = ADC10SHT_1 + ADC10ON + ADC10IE + MSC;
  ADC10CTL1 = INCH_1 + ADC10SSEL_2 + ADC10DIV_7 + CONSEQ_1;
  ADC10AE0  = BIT0 + BIT1;
  ADC10DTC1 = 0x02;        //2 Çevrim
}

void deger_goster(unsigned int sonuc)
{
  lcd_putch(sonuc/1000+48);
  lcd_putch((sonuc/100)%10+48);
  lcd_putch((sonuc/10)%10+48);
  lcd_putch(sonuc%10+48);
}