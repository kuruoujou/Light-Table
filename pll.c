#include <hidef.h>         
#include <mc9s12c32.h>     

void PLL_Init(void){  
  CLKSEL = 0x80; //disengage PLL from system
  PLLCTL = 0x40; //turn on PLL 
  SYNR = 0x02;   //set PLL multiplier
  REFDV = 0x00;  //set PLL divider
  asm nop           //-------necessary or not?
  asm nop
  while((CRGFLG&0x08) == 0){ 	  // Wait for PLLCLK to stabilize.
  }  
  CLKSEL_PLLSEL = 1;  // Switch to PLL clock
}

