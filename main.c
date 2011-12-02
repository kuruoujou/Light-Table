#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

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


void main(void) {
  initialize();
  
	
	  
	


  for(;;) {
    _FEED_COP(); /* feeds the dog */
    if(tenths == 1){
      tenths = 0;
      ATD_Convert();
      convertTemp();
      displayTemp();
    }
  } /* loop forever */
  /* please make sure that you never leave main */
}
void initialize(void){
  tencnt = 0;
  tenths = 0;
  DDRT = 0xFF;
  ATDDIEN =0x00;
  TIM_Init();
  PWM_Init();
  ATD_Init();
  SPI_Init();
  LCD_Init();
  
  
  EnableInterrupts;
}

void TIM_Init(void){
TSCR1 = 0x80;
TIOS  = 0x80;
TSCR2 = 0x0C;
TC7   = 15000;
TIE   = 0x80;            //enable or disable????   enable
}

void PWM_Init(void){
//sampling frequency approximately 100Hz
MODRR   = 0x0F;
PWME    = 0x0F;
PWMPOL  = 0x00;
PWMCTL  = 0x00;
PWMCAE  = 0x00;
PWMPER0 = 0xFF;
PWMPER1 = 0xFF;
PWMPER2 = 0xFF;
PWMPER3 = 0xFF;
PWMDTY0 = 0x00;
PWMDTY1 = 0x00;
PWMDTY2 = 0x00;
PWMDTY3 = 0x00;
PWMPRCLK= 0x77;
PWMCLK  = 0x0F;
PWMSCLA = 0x04;
PWMSCLB = 0x04;

//DDRT
//PTT
}

void ATD_Init(void){
  ATDCTL2 = 0x80; 
  ATDCTL3 = 0x00; //Sequence length = 8
  ATDCTL4 = 0x05; //%00000101 10 bit resolution;2 ATD clock periods;
}

void SPI_Init(void){
SPIBR  = 0x01;
SPICR1 = 0x50;
SPICR2 = 0x00; 
}

void LCD_Init(void){
PTT |= LCDCLK;
PTT &= ~RW;
LCD_Instruction(LCDON);
LCD_Instruction(TWOLINE);
LCD_Instruction(LCDCLR);
wait(2998);              //delay for 2ms;
}


//wait(2998);    //2ms
//wait(2);       //30cycle
//TCNT will increase every 0.667 us   1.5MHz(24/prescalar16)
void static wait(unsigned short delay){ 
unsigned short startTime;
  startTime = TCNT;
  while((TCNT-startTime) <= delay){}  
}

void LCD_Instruction(unsigned char instru){
PTT &= ~RS;
LCD_Send(instru);

}

void LCD_Send(unsigned char byt){
while (SPISR_SPTEF == 0) {} 
SPIDR = byt;
wait(2);
PTT &= ~LCDCLK;
asm nop;
asm nop
PTT |= LCDCLK;
wait(50); 
}


void LCD_Printc(unsigned char character){
PTT |= RS;
LCD_Send(character);
}

void LCD_Printmsg(char *pt){
while(*pt){
LCD_Printc((unsigned char)*pt);
pt++;
}
}

void ATD_Convert(void){
  ATDCTL5 = 0x10;                 //begin with channel 0  
  while((ATDSTAT1&0x01)==0){};    // wait for CCF0 
  tem1 = ATDDR4;
  tem2 = ATDDR5;
  tem3 = ATDDR6;
  tem4 = ATDDR7;
  PWMDTY0 = ATDDR0;               //modify the PWM duty cycle
  PWMDTY1 = ATDDR1;
  PWMDTY2 = ATDDR2;
  PWMDTY3 = ATDDR3;
  
}

void convertTemp(void){
  //Convert tem1
  asm(" ldd tem1");
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm(" std tem1");
  tem1 = tem1 * 500/1024;
  //Convert tem2
  asm(" ldd tem2");
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm(" std tem2");
  tem2 = tem2 * 500/1024;
  //Convert tem3
  asm(" ldd tem3");
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm(" std tem3");
  tem3 = tem3 * 500/1024;
  //Convert tem4
  asm(" ldd tem4");
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm lsrd
  asm(" std tem4");
  tem4 = tem4 * 500/1024;
}
  
void displayTemp(void){
  LCD_Instruction(LCDCLR);
  LCD_Instruction(CURMOV);
  LCD_Instruction(LINE1);
  LCD_Printmsg("    P1:");
  printLCDTemp(tem1);
  LCD_Printmsg("  P2:");
  printLCDTemp(tem2);
  LCD_Instruction(CURMOV);
  LCD_Instruction(LINE2);
  LCD_Printmsg("P3:");
  printLCDTemp(tem3);
  LCD_Printmsg("  P4:");
  printLCDTemp(tem4);
}

void printLCDTemp(unsigned short temp){
  if(temp >= 32)
    LCD_Printmsg("HOT ");
  else if(temp >= 26)
    LCD_Printmsg("WARM");
  else if(temp >= 21)
    LCD_Printmsg("NORM");
  else if(temp >= 15)
    LCD_Printmsg("COOL");
  else
    LCD_Printmsg("COLD");}     
       
  
void interrupt 15 ExtHan(void){
   TFLG1 = 0x80;
   tencnt++;
   if(tencnt >= 10){
    tenths = 1;
    tencnt = 0;
   }
}
   