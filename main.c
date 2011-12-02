#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "main.h" /* Function inits, global variables, definitions, etc. */

void main(void) {
  initialize(); //Initialize ALL THE THINGS.

  for(;;) {
    _FEED_COP(); /* feeds the dog */

    if(tenths == 1){ //if our tenth of a second flag is set...
      tenths = 0; //unset it...
      ATD_Convert(); //run an ATD Conversion
      convertTemp(); //convert the temperature from the conversion
      displayTemp(); //and display the temperature.
    }

  } /* loop forever */
  /* please make sure that you never leave main */
}

/* initialize */
/* Runs all of the initializations, also 
initializes global variables */
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

/* TIM-Init */
/* Initilizes the Timer */
void TIM_Init(void){
TSCR1 = 0x80;
TIOS  = 0x80;
TSCR2 = 0x0C;
TC7   = 15000;
TIE   = 0x80;            //enable or disable????   enable
}

/* PWM_Init */
/* Initilizes the PWM */
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

/* ATD_Init */
/* Initilizes the ATD */
void ATD_Init(void){
  ATDCTL2 = 0x80; 
  ATDCTL3 = 0x00; //Sequence length = 8
  ATDCTL4 = 0x05; //%00000101 10 bit resolution;2 ATD clock periods;
}

/* SPI_Init */
/* Initilizes the SPI */
void SPI_Init(void){
SPIBR  = 0x01;
SPICR1 = 0x50;
SPICR2 = 0x00; 
}

/* LCD_Init */
/* Initilizes the LCD */
void LCD_Init(void){
PTT |= LCDCLK;
PTT &= ~RW;
LCD_Instruction(LCDON);
LCD_Instruction(TWOLINE);
LCD_Instruction(LCDCLR);
wait(2998);              //delay for 2ms;
}

/* wait */
/* Waits for a given period of time.
wait(2998): appx 2ms
wait(2): appx 30 cycles
TCNT will increase every 0.667 us   
[1.5MHz(24/(prescalar=16))] */
void static wait(unsigned short delay){ 
unsigned short startTime;
  startTime = TCNT;
  while((TCNT-startTime) <= delay){}  
}

/* LCD_Instruction */
/* Sends an instruction to the LCD */
void LCD_Instruction(unsigned char instru){
PTT &= ~RS;
LCD_Send(instru);

}

/* LCD_Send */
/* Sends a byte to the LCD */
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


/* LCD_Printc */
/* Sends a character to the LCD */
void LCD_Printc(unsigned char character){
PTT |= RS;
LCD_Send(character);
}

/* LCD_Printmsg */
/* Prints a string to the LCD */
void LCD_Printmsg(char *pt){
while(*pt){
LCD_Printc((unsigned char)*pt);
pt++;
}
}

/*ATD_Convert */
/* Performs ATD Conversions for the Temperature Sensors and the
* IR receivers. */
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

/* Convert Temp */
/* Converts the given temperature from the actual
* sensor (via the ATD) to an actual temperature
* (right?) */
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
  
/* displayTemp */
/* Runs all of the LCD temperature display code */
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

/* printLCDTemp procedure */
/* prints arbitrary messages to the LCD based on 
   given temperature */
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
    LCD_Printmsg("COLD");}     
       
/* TIM Interrupt Procedure */  
/* increments tenth of a second counter */
void interrupt 15 timerInterrupt(void){
   TFLG1 = 0x80;
   tencnt++;
   if(tencnt >= 10){
    tenths = 1;
    tencnt = 0;
   }
}
   
