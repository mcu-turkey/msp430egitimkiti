#include "msp430.h"
#include "ds1302.h"

void delay(unsigned int time)
{
  unsigned int i;
  for(i = 0;i < time;i++)
    __no_operation();
}

void Reset_DS1302(void)
{
  DS_RST_OUT; 
  DS_SCL_OUT;  
  DS_SCL0;     //SCLK=0
  DS_RST0;     //RST=0
  delay(10);
  DS_SCL1;     //SCLK=1
}

void init_DS1302(void)  
{
  //Set_DS1302(wdata);          // Saati programlamak icin acilmali...  
  Get_DS1302(rdata); 
  __no_operation();         
  //BurstWrite1302(bwdata);     // Saati programlamak icin acilmali...
  BurstRead1302(brdata);
  __no_operation();
  //BurstWriteRAM(rwdata);      // Saati programlamak icin acilmali...
  BurstReadRAM(rrdata);
  __no_operation();  
}
void Write1Byte(unsigned char wdata) 
{ 
  unsigned char i;
  
  DS_SDA_OUT;     
  DS_RST1;        //REST=1;
  
  for(i = 8; i > 0; i--) 
  {
    if(wdata&0x01)  DS_SDA1;
    else            DS_SDA0;
    DS_SCL0;
    delay(10); 
    DS_SCL1;
    delay(10); 
    wdata >>= 1; 
  } 
}

unsigned char Read1Byte(void) 
{ 
  unsigned char i;
  unsigned char rdata = 0X00;
  
  DS_SDA_IN;  
  DS_RST1;    //REST=1;
  
  for(i = 8; i > 0; i--)
  {
    DS_SCL1;
    delay(10);
    DS_SCL0;
    delay(10);
    rdata >>= 1;
    if(DS_SDA_BIT)  rdata |= 0x80;
  } 
  
  return(rdata); 
}

void W_Data(unsigned char addr, unsigned char wdata)
{
  DS_RST0; 
  DS_SCL0;   
  __no_operation();
  DS_RST1;
  Write1Byte(addr);   
  Write1Byte(wdata);  
  DS_SCL1;
  DS_RST0;
} 

unsigned char R_Data(unsigned char addr)
{
  unsigned char rdata;
  
  DS_RST0; 
  DS_SCL0; 
  __no_operation();
  DS_RST1;
  Write1Byte(addr);    //????
  rdata = Read1Byte();  //???? 
  DS_SCL1;
  DS_RST0;
  
  return(rdata);
}

void BurstWrite1302(const unsigned char *ptr)
{
  unsigned char i;
  
  W_Data(0x8e,0x00);          
  DS_RST0; 
  DS_SCL0; 
  __no_operation();
  DS_RST1;
  Write1Byte(0xbe);        
  for (i = 8; i > 0; i--) 
  {
    Write1Byte(*ptr++); 
  }
  DS_SCL1;
  DS_RST0;
  W_Data(0x8e,0x80);
} 

void BurstRead1302(unsigned char *ptr)
{
  unsigned char i;
  
  DS_RST0; 
  DS_SCL0; 
  __no_operation();
  DS_RST1;
  Write1Byte(0xbf);            
  for (i = 8; i > 0; i--) 
  {
    *ptr++ = Read1Byte(); 
  }
  DS_SCL1;
  DS_RST0;
}

void BurstWriteRAM(const unsigned char *ptr)
{
  unsigned char i;
  
  W_Data(0x8e,0x00);        
  DS_RST0; 
  DS_SCL0; 
  __no_operation();
  DS_RST1;
  Write1Byte(0xfe);          
  for (i = 31; i>0; i--)     
  {
    Write1Byte(*ptr++); 
  }
  DS_SCL1;
  DS_RST0; 
  W_Data(0x8e,0x80);          
} 

void BurstReadRAM(unsigned char *ptr)
{
  unsigned char i;
  
  DS_RST0; 
  DS_SCL0; 
  __no_operation();
  DS_RST1;
  Write1Byte(0xff);            
  for (i = 31; i > 0; i--) 
  {
    *ptr++ = Read1Byte();   
  }
  DS_SCL1;
  DS_RST0;
}

void Set_DS1302(const unsigned char *ptr) 
{
  unsigned char i;
  unsigned char addr = 0x80; 
  
  W_Data(0x8e,0x00);   
  
  for(i = 7;i > 0;i--)
  { 
    W_Data(addr,*ptr++); 
    addr += 2;
  }
  W_Data(0x8e,0x80);      
}

void Get_DS1302(unsigned char *ptr) 
{
  unsigned char i;
  unsigned char addr = 0x81;
  
  for(i = 0;i < 7;i++)
  {
    ptr[i] = R_Data(addr);
    addr += 2;
  }
}