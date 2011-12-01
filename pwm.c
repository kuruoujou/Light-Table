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