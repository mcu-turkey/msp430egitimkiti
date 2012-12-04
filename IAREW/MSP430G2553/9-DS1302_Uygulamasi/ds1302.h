#define DS_RST          BIT2            //DS_RST = P2.2 
#define DS_SCL          BIT0            //DS_SCL = P2.0 
#define DS_SDA          BIT1            //DS_SDA = P2.1 

#define DS_RST_IN       P2DIR &= ~DS_RST
#define DS_RST_OUT      P2DIR |=  DS_RST
#define DS_RST0         P2OUT &= ~DS_RST
#define DS_RST1         P2OUT |=  DS_RST
#define DS_SCL_IN       P2DIR &= ~DS_SCL
#define DS_SCL_OUT      P2DIR |=  DS_SCL
#define DS_SCL0         P2OUT &= ~DS_SCL
#define DS_SCL1         P2OUT |=  DS_SCL
#define DS_SDA_IN       P2DIR &= ~DS_SDA
#define DS_SDA_OUT      P2DIR |=  DS_SDA
#define DS_SDA0         P2OUT &= ~DS_SDA
#define DS_SDA1         P2OUT |=  DS_SDA
#define DS_SDA_BIT      P2IN & DS_SDA

extern const unsigned char  wdata[7];
extern unsigned char  rdata[7];                    
extern const unsigned char bwdata[8];
extern unsigned char brdata[8];
extern const unsigned char rwdata[31];
extern unsigned char rrdata[31];

void delay(unsigned int time);
void Reset_DS1302(void);
void init_DS1302(void);
void Write1Byte(unsigned char wdata);
unsigned char Read1Byte(void);
void W_Data(unsigned char addr, unsigned char wdata);
unsigned char R_Data(unsigned char addr);
void BurstWrite1302(const unsigned char *ptr);
void BurstRead1302(unsigned char *ptr);
void BurstWriteRAM(const unsigned char *ptr);
void BurstReadRAM(unsigned char *ptr);
void Set_DS1302(const unsigned char *ptr);
void Get_DS1302(unsigned char *ptr);