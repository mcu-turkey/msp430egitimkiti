
#include "msp430.h"
#include "lcd_msp.h"

void integer_yaz(unsigned int,char);
void deger_goster(unsigned int);
void a0_init(void);

volatile char tick;

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);

  TA0CTL = TASSEL_2 + TACLR;
  TA0CCR0 = 50000-1;
  TA0CCTL0 = CCIE;

  a0_init();
  lcd_init();
  
  lcd_goto(1,2);
  lcd_puts("mcu-turkey.com");
  lcd_goto(2,2);
  lcd_puts("ADC POT1 Uyg.");
  delay_ms(2000);
  lcd_temizle();
 
  TA0CTL |= MC_1; //Timer Baþlatýlýyor...

  __bis_SR_register(LPM0_bits + GIE);   
}

void deger_goster(unsigned int ham)
{
  unsigned int volt=0;
  
  volt=(float)(ham/2.86978)*10;
  
  lcd_goto(1,1);
  lcd_puts("10 Bit  = ");
  integer_yaz(ADC10MEM,0);
  
  lcd_goto(2,1);
  lcd_puts("Gerilim = ");
  integer_yaz(volt,1);
}

void a0_init()
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE;
  ADC10CTL1 = INCH_0 + ADC10SSEL_0 + ADC10DIV_7;
  ADC10AE0  = BIT0;
}

void integer_yaz(unsigned int deger,char flag)
{
  lcd_putch( deger/10000+48);
  lcd_putch((deger%10000)/1000+48);
  if(flag==1) lcd_putch('.');
  lcd_putch(((deger%1000)/100)+48);
  lcd_putch((deger%100)/10+48);
  lcd_putch((deger)%10+48);
}

// TimerA0 Kesme Vektörü
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  tick++;
  if(tick==10)
  {
    ADC10CTL0 |= ENC + ADC10SC;           
    deger_goster(ADC10MEM);
    tick = 0;
  }
}
// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  //__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
  __no_operation();
}