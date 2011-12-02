#define RS      0x20
#define RW      0x40
#define LCDCLK  0x80
#define LCDON   0x0F
#define LCDCLR  0x01
#define TWOLINE 0x38
#define CURMOV  0xFE
#define LINE1   0x80
#define LINE2   0xC0

unsigned char tencnt;
unsigned char tenths;
unsigned short tem1;
unsigned short tem2;
unsigned short tem3;
unsigned short tem4;
unsigned char maxDist;

void initialize(void);
void TIM_Init(void);
void PWM_Init(void);
void ATD_Init(void);
void SPI_Init(void);
void LCD_Init(void);
void static wait(unsigned short delay);
void LCD_Instruction(unsigned char instru);
void LCD_Send(unsigned char content);
void LCD_Printc(unsigned char character);
void LCD_Printmsg(char *pt);
void ATD_Convert(void);
void convertTemp(void);
void displayTemp(void);
void printLCDTemp(unsigned short);
