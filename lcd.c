#define RS      0x04
#define RW      0x08
#define LCDCLK  0x20
#define LCDON   0x0F
#define LCDCLR  0x01
#define TWOLINE 0x38
#define CURMOV  0xFE
#define LINE1   0x80
#define LINE2   0xC0


void LCD_Init(void){
PTT |= LCDCLK;
PTT &= ~RW;
LCD_Instruction(LCDON);
LCD_Instruction(TWOLINE);
LCD_Instruction(LCDCLR);
LCD_Wait(2998);              //delay for 2ms;
}


//wait(2998);    //2ms
//wait(2);       //30cycle
//TCNT will increase every 0.667 us   1.5MHz(24/prescalar16)
void static wait(unsigned short delay){ 
unsigned short startTime;
  startTime = TCNT;
  while((TCNT-startTime) <= delay){}  
}

void LCD_Instruction(unsigned short instru){
PTT &= ~RS;
LCD_Send(instru);

}

void LCD_Send(unsigned short byte){
while (SPISR & 0x20 == 0) {} 
SPIDR = byte;
wait(2);
PTT &= ~LCDCLK;
//asm nop;
//asm nop
PTT |= LCDCLK;
wait(2998);

}


void LCD_Printc(unsigned char character){
PTT |= RS;
LCD_Send(character);
}

void LCD_Printmsg(char *pt){
while(*pt){
LCD_OutChar((unsigned char)*pt));
pt++;
}
}








