#include <mc9s12c32.h>  
#include "atd.h"

void ATD_Convert(void){
  ATDCTL5 = 0x10;                 //begin with channel 0  
  while((ATDSTAT1&0x01)==0){};    // wait for CCF0 
  PWMDTY0 = ATDDR0;               //modify the PWM duty cycle
  PWMDTY1 = ATDDR1;
  PWMDTY2 = ATDDR2;
  PWMDTY3 = ATDDR3;
}

void ADC_Init(void){
  ATDCTL2 = 0x80; 
  ATDCTL3 = 0x20; //Sequence length = 4
  ATDCTL4 = 0x85; //%10000101 8 bit resolution;2 ATD clock periods;
}
