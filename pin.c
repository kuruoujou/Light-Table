void PIN_Init(void){
ATDDIEN = 0xC0;   //0x00;   still need pushbutton?
DDRT    = 0xFF;
PTT     = 0x00;
DDRB    = 0x10;
PORTB   = 0x10;







}