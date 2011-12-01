void SPI_Init(void){
SPIBR  = 0x01;
SPICR1 = 0x50;
SPICR2 = 0x00;

}