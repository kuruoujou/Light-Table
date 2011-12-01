//generate interrupt

void TIM_Init(void){
TSCR1 = 0x80;
TIOS  = 0x80;
TSCR2 = 0x0C;
TC7   = 15000;
TIE   = 0x80;            //enable or disable????   enable
asm  cli;
}


#pragma interrupt_handler TIM_Interrupt()
void TIM_Interrupt(void){
TFLG1 |= 0x80;
TimCounter++;             //global variable TIMCouter
if(TIMCounter >= 10){ 
     tenths = 1;          //global boolean tenths
	 TimCounter = 0; 
}
}

